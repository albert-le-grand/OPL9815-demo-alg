/*
 * utils.c
 *
 *  Created on: Mar 7, 2012
 *      Author: kogami
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <limits.h>
#include <setjmp.h>
#include "lib.h"
#include "gps.h"
#include "menu.h"
#include "main.h"
#include "utils.h"

#include "fnt\\battery.fnt" // the battery font file 7 images and batt empty and almost empty
#include "fnt\\numrev.fnt"  // the reversed numeric font file shown with battery charging icons

#define SCREEN_DUMP	"~src.tmp"
#define BAT_CHARGE         1      //OPL
#define BAT_NO_CHARGE      0      //DCL


// the jump buffer when battery power is empty
extern int batt_power;

// this variable controls the icon to show when battery is low or empty
extern int 	battery_msg_status;

extern int battery_type;
extern int menu_state;
extern int IN_MENU_MODE;
extern int NOT_IN_MENU_MODE;
extern int EXITING_MENU_MODE;
extern int prefix;
extern int suffix;
extern int BTStateChange;
extern int IsScannerGPS(void);
extern int IsScannerBluetooth(void);
extern int UpdateBluetoothState(int *stateChange);

extern scannerType scanner;

// these variables are used by the gps demo
int g_util_timer;
int g_util_timeout;
int g_gps_mode;

// these variables are used by the timer manager
typedef struct
{
	unsigned long   nTimerCount;        // countdown of timer
	unsigned long   nTimerDuration;     // duration of timer
	int             fAutoRestart;       // TRUE - autorestart, FALSE - one-time
	int             fEnabled;           // TRUE - timer enabled
} TIMER_STRUCT;

TIMER_STRUCT rgTimers[TIMER_ID_MAX+1];

// these functions are used by the gps demo
extern void ServiceBreadCrumbTimer(void);
extern void GPSService(void);

// this function is defined for the gps demo
int check_timeout( void );

void restore_font( int width, int height );
void display_icon( int x, int y, int state );
void check_batt_empty( void );

//------------------------------------------------------------------
//	KAwaitkey
//  ============================
//	Wait key depending on battery type.
//------------------------------------------------------------------
int KAwaitkey(int bat_type)
{
	if (bat_type >= BAT_CHARGE)
		return ( waitkey() );	                //models with chargable batteries (OPL)
	else
		return ( waitkey_no_charge() );        //models with alkaline batteries (DCL)
}

//------------------------------------------------------------------
//	empty_batt_beep
//  ============================
//	Bettery low beep.
//------------------------------------------------------------------
void empty_batt_beep( void )
{
	sound(TLONG,VHIGH,SERROR,SPAUSE,SERROR,SPAUSE, SERROR,0);
}

//------------------------------------------------------------------
//	display_battery_txt_msg
//  ============================
//	Displays battery charge message.
//------------------------------------------------------------------
void display_battery_txt_msg( int empty )
{
	putchar('\f');
	setfont( MEDIUM_FONT, NULL );

	if( empty )
	{
		gotoxy(6, 0);
		printf("Battery is");
		gotoxy(6, 1);
		printf("empty.");
		gotoxy(0, 3);
		printf("Put Scannr in");
		gotoxy(0, 4);
		printf("cradle to charge.");
	}
	else
	{
		gotoxy(6, 0);
		printf("Battery is");
		gotoxy(6, 1);
		printf("near empty.");
		gotoxy(6, 2);
		printf("Press a key");
		gotoxy(0, 3);
		printf("or put Scannr in");
		gotoxy(0, 4);
		printf("cradle to charge.");
	}
}

//------------------------------------------------------------------
//	show_empty_batt_msg
//  ============================
//dISPLAYS BATTERY EMPTY MESSAGE.
//------------------------------------------------------------------
void show_empty_batt_msg( int empty )
{
	int state;
	unsigned int width, height;	// for font size
	int curs_x, curs_y; 		// for cursor postion

	// store the user settings
	savedisplay((char*) SCREEN_DUMP );
	getfontsize( &height, &width ); // get the font size
	curs_x = wherex();	// retrieve cursor x pos
	curs_y = wherey(); 	// retrieve cursor y_pos

	empty_batt_beep();
	display_battery_txt_msg( empty );
	if( empty )
		setfont( USER_FONT, (const unsigned char*)_empty );  // the empty batter font
	else
		setfont( USER_FONT, (const unsigned char*)_almost ); // the almost empty battery font

	state = 0;
	resetkey();
	do{
		display_icon( 0, 0, state );
		state ^= 1;

		delay( 15 ); // wait 300ms some time to display image
		if( !empty )
			check_batt_empty();
	}while((ischarging() == 0) && (( getchar() == EOF ) || empty ));

	// restore the saved settings
	loaddisplay((char*) SCREEN_DUMP );  // restore screen
	remove( (char*) SCREEN_DUMP );
	restore_font( width, height );    	// restore font
	gotoxy( curs_x, curs_y ); 		    // restore cursor position
}

//------------------------------------------------------------------
//	check_batt_empty
//  ============================
//	Checks battery status.
//------------------------------------------------------------------
void check_batt_empty( void )
{
	unsigned output;
	unsigned bat_low;
	unsigned bat_empty;

	bat_low = BATT_LOW;
	bat_empty = BATT_EMPTY;

	output = GetBatteryVoltage_mV();

	if( output <= bat_empty )       //BATT_EMPTY
	{
		scannerpower( OFF, 0 );		// turn scanner laser off

		// to start of main application
		longjmp( (int *)batt_power, 5 );
	}

	if (output <= bat_low && battery_type == BAT_NO_CHARGE  )   //DCL
	{
		clear_screen();
		printf("\f\nBatteries low\nPlease replace\nbatteries");
		resetkey();
		waitkey_no_charge();
		return;

	}

	if( output <=  bat_low && battery_msg_status == 0 )	//BATT_LOW   OPL
	{
		battery_msg_status++;
		show_empty_batt_msg( FALSE ); // near empty battery message
	}
}


//------------------------------------------------------------------
//	restore_font
//  ============================
//	Restores font based on fint size set.
//------------------------------------------------------------------
void restore_font( int width, int height )
{
	// use the font size to restore the font type
	if( width == 8 && height == 16 )
		setfont( LARGE_FONT, NULL );
	else if( width == 6 && height == 12 )
		setfont( MEDIUM_FONT, NULL );
	else if( width == 8 && height == 8 )
		setfont( SMALL_FONT, NULL );
	else if( width == 6 && height == 6 )
		setfont( TINY_FONT, NULL );
	else
		setfont( MEDIUM_FONT, NULL );
}

//------------------------------------------------------------------
//	display_icon
//  ============================
//	Displays icon store in flash at x, y, submitted.
//------------------------------------------------------------------
void display_icon( int x, int y, int state )
{
	int startpos, a, b;

	startpos = state * 16;

	for(b=0;b<4;b++)
	{
		for(a=0;a<4;a++)
		{
			gotoxy( x+a, y+b );
			printsymbol( startpos++ );
		}
	}

}

//------------------------------------------------------------------
//	display_rev_voltage
//  ============================
//	Displays voltage in battery icon with shadow.
//------------------------------------------------------------------
void display_rev_voltage( int x, int y, unsigned int voltage )
{
	unsigned char buffer[10];
	sprintf((char *)buffer,"%04u", voltage);

	gotoxy( x, y );
	printsymbol( 11 ); // V
	gotoxy( x+1, y );
	printsymbol( buffer[2]-'0' );
	gotoxy( x+2, y );
	printsymbol( buffer[1]-'0');
	gotoxy( x+3, y );
	printsymbol( 10 ); // .
	gotoxy( x+4, y );
	printsymbol( buffer[0]-'0');

	rectangle3drev( (x*6)-1, (y*12), ((x+5)*6)+1, (y+1)*12, 1); // draw 3D box in reversed fashion

}

//------------------------------------------------------------------
//	is_on_cradle
//  ============================
//	Tests for charging and displays battery icon when charging.
//------------------------------------------------------------------
void is_on_cradle( void )
{
	int state;
	int charge_state;
	unsigned int width, height; // for font size
	int curs_x, curs_y; 		// for cursor postion
	unsigned output;

	if( ischarging() )
	{
		// quickly store an image of the screen
		savedisplay((char*) SCREEN_DUMP );
		getfontsize( &height, &width ); // get the font size
		curs_x = wherex();	// retrieve cursor x pos
		curs_y = wherey(); 	// retrieve cursor y_pos
		putchar('\f'); 		// clear the screen to show battery

		flipscreen( OFF );	// ??

		state = 0;
		while( (charge_state = ischarging()) > 0 )
		{
			// the font with the battery images
			setfont( USER_FONT, (const unsigned char*)_battery );
			if( charge_state == 2 )
				state = 6;
			display_icon( 8, 2, state );

			if(++state > 6 )
				state = 0;

			setfont( USER_FONT, (const unsigned char*)_num_rev ); // the reversed font

			output = GetBatteryVoltage_mV();
			display_rev_voltage( 3, 2,output );

			if( (unsigned)output > BATT_CHARGE_LOW )
				battery_msg_status = 0;     // set icon to almost empty

			delay( 15 ); // wait 300ms some time to display image
		}

		flipscreen( AUTO );	 // ??

		loaddisplay((char*) SCREEN_DUMP ); 	// restore screen
		remove( (char*) SCREEN_DUMP );
		restore_font( width, height ); 		// restore font

		gotoxy( curs_x, curs_y ); 			// retore cursor position
	}
}



//------------------------------------------------------------------
//	draw_bar
//  ============================
//	Draws a bar with submitted specification.
//------------------------------------------------------------------
void draw_bar( int x_pos, int y_pos, int width, int height, int color )
{
	int count;
	if( width > height )
	{
		for( count = 0; count < height; count++ )
			drawline( x_pos, y_pos+count, x_pos+width, y_pos+count, color );
		return;
	}
	for( count = 0; count < width; count++ )
		drawline( x_pos+count, y_pos, x_pos+count, y_pos+height, color );
}

// make sound to inform the user
//------------------------------------------------------------------
//	info_sound
//  ============================
//	Sounds information beep.
//------------------------------------------------------------------
void info_sound( void )
{
	sound( TSHORT, VMEDIUM, SHIGH, SLOW, SHIGH, SHIGH, SLOW, 0);
}

//------------------------------------------------------------------
//	keybeeplow
//  ============================
//	Sounds low volume key beep.
//------------------------------------------------------------------
void keybeeplow(void)
{
	sound(TSHORT,VLOW,SHIGH,0);
}

//------------------------------------------------------------------
//	keybeep
//  ============================
//	Sounds normal volume key beep.
//------------------------------------------------------------------
void keybeep(void)
{
	sound(TSHORT,VMEDIUM,SHIGH,0);
}

//------------------------------------------------------------------
//	errkeybeep
//  ============================
//	Sounds key error beep.
//------------------------------------------------------------------
void errkeybeep(void)
{
	sound(TLONG,VMEDIUM,SLOW,0);
}

//------------------------------------------------------------------
//	okbeep
//  ============================
//	Sounds okay beep.
//------------------------------------------------------------------
void okbeep(void)
{
	sound(TSTANDARD,VMEDIUM,SHIGH,SMEDIUM,SHIGH,0);
}

//------------------------------------------------------------------
//	errbeep
//  ============================
//	Sounds error beep.
//------------------------------------------------------------------
void errbeep(void)
{
	sound(TLONG,VMEDIUM,SLOW,SMEDIUM,SHIGH,0);
}

//------------------------------------------------------------------
//	waitkey
//  ============================
//	Waits for a key entry while monitoring charge state.
//	While waiting, provides GPS and bluetooth state monitoring.
//------------------------------------------------------------------
int waitkey( void )
{
	int c;


	while((c=getchar())==EOF)
	{
		check_batt_empty(); // check the battery
		is_on_cradle();		// check to see if OPL972x is placed on the cradle

		// for GPS mode use this as a background loop
		if(IsScannerGPS() == 1)
		{
			// run the GPS receiver
			GPSService();
			// check the timer
			ServiceBreadCrumbTimer();
		}
		// for bluetooth scanners maintain the bluetooth state machine
		if(IsScannerBluetooth())
		{
			UpdateBluetoothState(&BTStateChange);
		}
		idle();
	}
	if(IsScannerGPS())
	{
		PokeActivity();
	}
	keybeep(); // make the beeping sound
	return c;
}

//------------------------------------------------------------------
//	waitkey_no_charge
//  ============================
//	Waits for key entry.  Use when waiting for key entry and device is put in cradle.
//	While waiting, provides GPS and bluetooth state monitoring.
//------------------------------------------------------------------
int waitkey_no_charge( void )
{
	int c;

	while((c=getchar())==EOF)
	{
		// for GPS mode use this as a background loop
		if(IsScannerGPS() == 1)
		{
			// run the GPS receiver
			GPSService();
			// check the timer
			ServiceBreadCrumbTimer();
		}
		// for bluetooth scanners maintain the bluetooth state machine
		if(IsScannerBluetooth())
		{
			UpdateBluetoothState(&BTStateChange);
		}
		idle();
	}
	if(IsScannerGPS())
	{
		PokeActivity();
	}
	keybeep();
	return c;
}

//------------------------------------------------------------------
//	KACheckKey
//  ============================
//	Checks for key entry.
//	Maintains GPS and bluetooth monitoring.
//------------------------------------------------------------------
int KACheckKey(void)
{

	// for GPS mode use this as a background loop
	if(IsScannerGPS() == 1)
	{
		// run the GPS receiver
		GPSService();
		// check the timer
		ServiceBreadCrumbTimer();
	}
	// for bluetooth scanners maintain the bluetooth state machine
	if(IsScannerBluetooth())
	{
		UpdateBluetoothState(&BTStateChange);
	}
	if(IsScannerGPS())
	{
		PokeActivity();
	}

	return getchar();
}

//------------------------------------------------------------------
//	rectangle
//  ============================
//	Draws a rectangle with submitted specifications.
//------------------------------------------------------------------
void rectangle(int x1,int y1,int x2,int y2,int color)
{
	drawline(x1,y1,x2,y1,color);
	drawline(x1,y2,x2,y2,color);
	drawline(x1,y1,x1,y2,color);
	drawline(x2,y1,x2,y2,color);
}


//------------------------------------------------------------------
//	rectangle3d
//  ============================
//	Draws a rectangle with shadowing to submitted specifications.
//------------------------------------------------------------------
void rectangle3d(int x1,int y1,int x2,int y2,int color)
{
	rectangle(x1,y1,x2,y2,color);
	drawline(x1+2,y2+1,x2+2,y2+1,1);
	drawline(x1+2,y2+2,x2+2,y2+1,1);
	drawline(x2+1,y1+2,x2+1,y2+2,1);
	drawline(x2+2,y1+2,x2+2,y2+2,1);
}

//------------------------------------------------------------------
//	rectangle3drev
//  ============================
//	Draws a rectangle with upward shadowing to submitted specifications.
//------------------------------------------------------------------
void rectangle3drev(int x1,int y1,int x2,int y2,int color)
{
	rectangle(x1,y1,x2,y2,color);

	drawline(x1-2, y1-1, x2-2, y1-1, 1);
	drawline(x1-2, y1-2, x2-2, y1-2, 1);
	drawline(x1-1, y2-2, x1-1, y1-2, 1);
	drawline(x1-2, y2-2, x1-2, y1-2, 1);
}

//------------------------------------------------------------------
//	ScanBarcode
//  ============================
//	Waits for a scanned barcode label.
//	Provides proecssing for scanning menu coes.
//	While waiting, provides GPS and bluetooth state monitoring.
//------------------------------------------------------------------
int ScanBarcode( char* string, int min_length, int max_length, int *code_id)  //
{
	int key;
	struct barcode code;
	struct barcode prev_code;
	static char prev_buff[400];

	unsigned long time_out_value;

	prev_code.min = min_length;
	prev_code.max = 400;
	prev_code.id = 0;
	prev_code.text = prev_buff;
	time_out_value = 0;

	code.min =  min_length;
	code.max =  max_length;
	code.text = string;

	while( readbarcode( &code ) != OK || composite_post_processing( &code, &prev_code, &time_out_value, 150)!=0  )
	{
		if(!triggerpressed())
			prev_code.id = 0;

		if( kbhit() )
		{
			if(( key = getchar() ) != TRIGGER_KEY )
			{
				// put the character back in the input buffer
				// so it can be handled by another function
				menu_state = NOT_IN_MENU_MODE;
				ungetc( key, stdin);
				scannerpower( OFF, 0);
				return( ERROR );
			}else
			{
				scannerpower( ON, 10000 );
			}
		}

		if(CheckMyTimer(TIMER_ID_SCANNER) == TIMER_STATUS_EXPIRED)
		{
			if (menu_state == IN_MENU_MODE)
			{
				sound(TSTANDARD, VHIGH, SMEDIUM, NULL);	  //programming beeping sound
				goodreadled( RED, 10 );
			}
		}
		// on cradle
		if (battery_type == BAT_CHARGE)
		{
			check_batt_empty();
			is_on_cradle();
		}
		// for GPS mode use this as a background loop
		if(IsScannerGPS() == 1)
		{
			// run the GPS receiver
			GPSService();
			// check the timer
			ServiceBreadCrumbTimer();
		}
		// for bluetooth scanners maintain the bluetooth state machine
		if(IsScannerBluetooth())
		{
			UpdateBluetoothState(&BTStateChange);
		}
		idle();
	}
	scannerpower( OFF, 0 );

	if(IsScannerGPS())
	{
		PokeActivity();
	}
	*code_id = conv_symbology(code.id);	  //KA
	return( OK );
}

/******************************************************************************************
Combines all allowed combinations of 2D-composite bar codes and linear bar codes

The following combinations of a linear bar codes with a 2D composite are allowed:
-	RSS-14(standard)	-	CCA-COMPOSITE (4 data columns)
-	RSS-14(standard)	-	CCB-COMPOSITE (4 data columns)

-	RSS-14(stacked)	-	CCA-COMPOSITE (2 data columns)
-	RSS-14(stacked)	-	CCB-COMPOSITE (2 data columns)

-	RSS-LIMITED		-	CCA-COMPOSITE (3 data columns)
-	RSS-LIMITED		-	CCB-COMPOSITE (3 data columns)

-	RSS_EXPANDED		-	CCA-COMPOSITE (4 data columns)
-	RSS_EXPANDED		-	CCB-COMPOSITE (4 data columns)

-	EAN128			-	CCA_COMPOSITE (4 data columns)
-	EAN128			-	CCB_COMPOSITE (4 data columns)
-	EAN128			-	CCC_COMPOSITE

Returns:
	0		Normal barcode read OR both parts of a linear/2D composite combination read
	1		First part of a linear/2D composite combination read
 **********************************************************************************************/
//------------------------------------------------------------------
//	composite_post_processing
//  ============================
//	Combines all allowed combinations of 2D-composite bar codes and linear bar codes
//
//	The following combinations of a linear bar codes with a 2D composite are allowed:
//	-	RSS-14(standard)	-	CCA-COMPOSITE (4 data columns)
//	-	RSS-14(standard)	-	CCB-COMPOSITE (4 data columns)
//
//	-	RSS-14(stacked)	-	CCA-COMPOSITE (2 data columns)
//	-	RSS-14(stacked)	-	CCB-COMPOSITE (2 data columns)
//
//	-	RSS-LIMITED		-	CCA-COMPOSITE (3 data columns)
//	-	RSS-LIMITED		-	CCB-COMPOSITE (3 data columns)
//
//	-	RSS_EXPANDED		-	CCA-COMPOSITE (4 data columns)
//	-	RSS_EXPANDED		-	CCB-COMPOSITE (4 data columns)
//
//	-	EAN128			-	CCA_COMPOSITE (4 data columns)
//	-	EAN128			-	CCB_COMPOSITE (4 data columns)
//	-	EAN128			-	CCC_COMPOSITE
//
//	Returns:
//		0		Normal barcode read OR both parts of a linear/2D composite combination read
//		1		First part of a linear/2D composite combination read
//------------------------------------------------------------------
int composite_post_processing(struct barcode *code, struct barcode *prev_code,
		unsigned long *time_out_value, short reset_time)
{
	char format_info,format_info_prev;

	if( scanner.ModelNumber == PX36 )
		return 0;

	switch(code->id)
	{
	case MICRO_PDF417:	//Remove transmitted 'nr of data columns' char at first position
		memmove(code->text,&(code->text)[1],code->length);
		code->text[--code->length] = '\0';
		prev_code->id = 0;
		return 0;			//Normal bar code, return OK
	case EAN128:
	case RSS_14:
	case RSS_LIMITED:
	case RSS_EXPANDED:
		if(code->text[0] == '0') //If no link flag set remove transmitted 'link-flag' at first pos.
		{
			memmove(code->text,&(code->text)[1],code->length);
			code->text[--code->length] = '\0';
			return 0;		// Normal bar code, return OK
		}
		break;
	case CCA_COMPOSITE:
	case CCB_COMPOSITE:
		format_info = code->text[0]; //Store format info = link-flag(RSS) or nr-of-data-colums(CC-A/B)
		break;
	case CCC_COMPOSITE:
		break;						//Note: CC-C composite symbols have no format info
	default:
		prev_code->id = 0;
		return 0;			//Normal bar codes, return OK

	}

	if(code->length > prev_code->max)		//Check maximum length for valid combinations
		return 0;


	//If 1st half of a combination is already known, try to combine
	if(prev_code->id != 0)
	{
		if(GetSystemTime() < *time_out_value)
		{
			format_info_prev = prev_code->text[0];

			if (code->id == prev_code->id)	//if same bar code ID
			{
				if(strcmp((char*)code->text,(char*)prev_code->text)==0) //if same data
				{
					*time_out_value = GetSystemTime() + reset_time; //time out value
					return 1;		//First part read again!
				}

				prev_code->id = 0;		//Store this part as 1st part
			}
			else if( code->id == CCA_COMPOSITE ||  code->id == CCB_COMPOSITE )
			{
				switch(prev_code->id)		//Check if combination is invalid
				{
				case RSS_14:
				case RSS_LIMITED:
				case RSS_EXPANDED:
				case EAN128:
				{
					if(prev_code->id == RSS_14 && (format_info == '2' || format_info == '4'))
						break;
					if(prev_code->id == RSS_LIMITED && format_info == '3')
						break;
					if(prev_code->id == RSS_EXPANDED && format_info == '4')
						break;
					if(prev_code->id == EAN128 && format_info_prev == '1' && format_info == '4')
						break;
					break;
				}
				default:
					prev_code->id = 0;	//Store this part as 1st part
					sound(TCLICK,VSTANDARD,SMEDIUM,0);
					break;
				}
			}
			else if( code->id == CCC_COMPOSITE )
			{
				switch(prev_code->id)		//Check if combination is invalid
				{
				case EAN128:
					if(format_info_prev == '2') 	//Check link flag value
						break;
					break;
				default:
					prev_code->id = 0; 		//Store this part as 1st part
					break;
				}
			}
			else if (code->id >= RSS_14 &&  code->id <= RSS_EXPANDED)
			{
				switch(prev_code->id)
				{
				case CCA_COMPOSITE: 	//Check if combination is invalid
				case CCB_COMPOSITE:
					if(code->id == RSS_14 && (format_info_prev == '2' || format_info_prev == '4'))
						break;
					if(code->id == RSS_LIMITED && format_info_prev == '3')
						break;
					if(code->id == RSS_EXPANDED && format_info_prev == '4')
						break;
					break;
				default:
					prev_code->id = 0;	//Store this part as 1st part
					sound(TCLICK,VSTANDARD,SMEDIUM,0);
					break;
				}
			}
			else if( code->id == EAN128 )
			{
				switch(prev_code->id)		//Check if combination is invalid
				{
				case CCA_COMPOSITE:
				case CCB_COMPOSITE:
				case CCC_COMPOSITE:
					if((prev_code->id==CCA_COMPOSITE || prev_code->id==CCB_COMPOSITE)
							&& format_info == '1' && format_info_prev == '4')
						break;
					if(prev_code->id == CCC_COMPOSITE && format_info == '2')
						break;
					break;
				default:
					prev_code->id = 0;	//Store this part as 1st part
					sound(TCLICK,VSTANDARD,SMEDIUM,0);
					break;
				}
			}
			else return 0;
		}
		else 	// if Time out store this part as 1st part
		{
			prev_code->id = 0;
			sound(TCLICK,VSTANDARD,SMEDIUM,0);
		}
	}
	else
		sound(TCLICK,VSTANDARD,SMEDIUM,0);

	if(prev_code->id != 0)	// if 2 parts read, try to combine
	{
		if(code->length + prev_code->length - 2 > code->max)		// too large combination
			return 0;

		if(code->id != CCC_COMPOSITE)	 //If not CC-C remove link flag/nr of columns of 2nd part
		{
			memmove(code->text,&(code->text)[1],code->length);
			code->text[--code->length] = '\0';
		}

		if(prev_code->id != CCC_COMPOSITE) //If not CC-C remove link flag/nr of columns of 1st part
		{
			memmove(prev_code->text,&(prev_code->text)[1],prev_code->length);
			prev_code->text[--prev_code->length] = '\0';
		}

		//Put RSS and EAN128 Codes in front and 2D (Micro)PDF417 variants behind it
		if((code->id >= RSS_14 && code->id <= RSS_EXPANDED) || code->id == EAN128)
		{
			strcpy(&(code->text)[code->length+1],prev_code->text);
			(code->text)[code->length] = A_GS;	//Add Primary/Secondary data seperation char.
		}
		else
		{
			memmove(&(code->text)[prev_code->length+1],code->text,code->length+1);
			memcpy(code->text,prev_code->text,prev_code->length);

			(code->text)[prev_code->length] = A_GS; 	//Add Primary/Secondary data seperation char.

			code->id = prev_code->id;				//Change ID to ID of linear part
		}

		code->length += prev_code->length;			//Set combined length

		prev_code->id = 0;							//Clear stored ID

		return 0;	//Combining of a linear code with a 2D composite code -> succesfull
	}

	prev_code->id = code->id;						//Store ID of 1st part
	prev_code->length = code->length;				//Store length of 1st part
	strcpy(prev_code->text,code->text);				//Store 1st part in prev_code
	*time_out_value = GetSystemTime() + reset_time;	//Set time out value

	return 1;	// Return 'Fist part of EAN*UCC Composite Symbol stored'
}



//------------------------------------------------------------------
//	what_day_of_week
//  ============================
//	Returns integer value representing the day of the week.
// 	0 = sunday, 1 = monday, 2 = tuesday, 3 = wensday
// 	4 = thursday, 5 = friday, 6 = saturday
//------------------------------------------------------------------
int what_day_of_week( void )
{
	int m, y, a;
	struct date curr_date;
	getdate( &curr_date );
	a= (14 - curr_date.da_mon ) / 12;
	y= curr_date.da_year - a;
	m= curr_date.da_mon + (12*a) - 2;
	return((curr_date.da_day + y + (y/4) - (y/100) + (y/400) + ((31*m)/12))%7 );
}

//------------------------------------------------------------------
//	store_char_in_string
//  ============================
//	Converts a numeric value to a string.
//------------------------------------------------------------------
static void store_char_in_string( int key, int max_len, char *string, int typ, int decimals )
{
	int i, ok;
	char *ptr;
	i = strlen( string );

	if(( i >= max_len ) && (key != '-'))
		return;

	ok = 0;
	if( typ & FLOAT )
	{
		ok = (isdigit( key ) || ((strchr( string, '.') == NULL )&& (key == '.')));
		if( ok && ( ptr = strchr( string, '.')))
		{	// we now need to check the decimal length
			if((strlen(string) - (ptr-string)) > decimals )
				ok = FALSE;
		}
		else if( ok && (key != '.'))
		{
			if( (strlen(string) - ((typ & NEGATIVE)? 1:0)+1) > ((max_len-decimals) -((typ & NEGATIVE)? 1:0) - 1))
				ok = FALSE;
		}
	}
	if( !ok && ( typ & NUM ))
		ok = isdigit( key );
	if( !ok && (typ & ALPHA ))
		ok = (isalpha(key) || key == ' ');
	if( !ok && ( typ & NEGATIVE ))
		ok = ( key == '-')? TRUE : FALSE;
	if( ok )
	{
		if(key == '-')
		{
			if( i )
				string[0] = (string[0] == '-')? ' ' : '-';
			else
			{
				string[i++] = (string[0] == '-')? ' ' : '-';
				string[i] = '\0';
			}
		}
		else if( typ & NEGATIVE )
		{
			if( i == 0 )
				string[i++] = (string[0] == '-')? '-' : ' ';
			string[i++] = key;
			string[i] = '\0';
		}
		else
		{
			string[i++] = key;
			string[i] = '\0';
		}
	}
}

// check special keys
//------------------------------------------------------------------
//	check_key_input
//  ============================
//	Checks for special key input.
//------------------------------------------------------------------
static int check_key_input( int key, int num, va_list key_list )
{
	int count;

	for( count = 0; count < num; count++ )
	{
		if( (int) va_arg( key_list, int ) == key )
		{
			va_end( key_list ); // needed for normal returning
			return( OK );
		}
	}
	va_end( key_list );
	return( ERROR );
}

//------------------------------------------------------------------
//	display_input
//  ============================
//	Displays a string with truncation.
//------------------------------------------------------------------
static void display_input( char* string, int x, int y, int max_length,int max_x )
{
	int length;
	int output;

	if( x < 0 || y < 0 )
		return;

	length = strlen( string );

	gotoxy(x,y);
	if( (x + length) < max_x )
	{
		if( length == max_length )
			output = printf("%s", string );
		else
			output = printf("%s ", string );
	}
	else
	{
		if( length == max_length )
			output = printf("%s", string + (length - max_x + x));
		else
			output = printf("%s ", string + (length - max_x + x));
	}
	gotoxy( x+output-1, y); // set the cursor to the correct position
}

//------------------------------------------------------------------
//	remove_one_from_buffer
//  ============================
//	Truncates the last character from a string.
//------------------------------------------------------------------
static void remove_one_from_buffer( char* string )
{
	if( strlen( string ) > 0 )
		string[ strlen( string ) - 1 ] = '\0';
}

//------------------------------------------------------------------
//	KeyboardBarcode
//  ============================
//	Displays key entry with backspace processing until a special key is entered.
//------------------------------------------------------------------
int KeyboardBarcode( char* string, int min_length, int max_length, int typ, int x, int y,int num, ... )
{
	va_list key_list;
	int key;

	if( num <= 0 )
		return( EOF );

	for(;;)
	{
		va_start( key_list, num );

		//		if( strlen(string) < max_length )
		//			cursor( ON );
		//		else
		//			cursor( OFF );

		if((( key = KAwaitkey(battery_type )) == ENT_KEY ) && (strlen(string) < min_length ))	  //KA chaned waitkey()
			continue;

		if( check_key_input( key, num, key_list ) == OK )
		{
			va_end( key_list );
			cursor( OFF );
			return( key );
		}
		va_end(key_list);

		if( key == BS_KEY )
		{
			remove_one_from_buffer( string );
			display_input(string, x, y, 15, 15);
			continue;
		}

		store_char_in_string( key,  max_length, string, typ, 0 );
		display_input(string, x, y, 15, 15);
	}
	return (FALSE);
}

//------------------------------------------------------------------
//	ScanOrKeyboardInput
//  ============================
//	Waits for a scanned barcode label or key entry.
//	Includes processing for menu labels.
//------------------------------------------------------------------
int ScanOrKeyboardInput( char* string, int min_length, int max_length, int typ, int x, int y, int offset )
{
	int	key;
	int code_id = 0;
	int nRetVal;

	for(;;)
	{
		string[0] = '\0';
		setoffset( offset );

		if( ScanBarcode( string, min_length, max_length , &code_id) == OK ) //
		{
			goodreadled( GREEN, 10 );
			//		   	scannerpower( OFF, 0);
			sound( TSTANDARD, VSTANDARD, SMEDIUM, 0 );
			setoffset( 0 );

			if (code_id == MENU_CODE)
			{

				if (strcmp(string, "ZZ") == 0)
				{

					menu_state = !menu_state;
					if (menu_state == NOT_IN_MENU_MODE)
					{
						suffix = -1;
						prefix = -1;
						StopMyTimer(TIMER_ID_SCANNER);
						// change state to exiting menu mode in oreder to get the right beep
						menu_state = EXITING_MENU_MODE;
					}
					else     //in menu mode
					{
						StartMyTimer(TIMER_ID_SCANNER, 50, TRUE);
						sound(TSTANDARD, VHIGH, SLOW, SMEDIUM, NULL);
						goodreadled(GREEN, 10);
					}
				}

				nRetVal = MENU_CODE;
				goto SINGLE_RETURN;

			}  //code is a menu code

			nRetVal = SCANNED;
			goto SINGLE_RETURN;
		}

		key = KeyboardBarcode( string, min_length, max_length, typ, x, y, 5, TRIGGER_KEY, CLR_KEY, ENT_KEY, DOWN_KEY, UP_KEY );
		setoffset( 0 );
		switch( key )
		{
		case ENT_KEY:
			nRetVal = KEYBOARD;
			goto SINGLE_RETURN;
		case TRIGGER_KEY:
			break;

		case CLR_KEY:
			if( strlen( string ) != 0)
			{
				break;
			}
			nRetVal = key;
			goto SINGLE_RETURN;
			break;
		default:
			nRetVal = key;
			goto SINGLE_RETURN;
		}

		// for GPS mode use this as a background loop
		if(IsScannerGPS() == 1)
		{
			// run the GPS receiver
			GPSService();
			// check the timer
			ServiceBreadCrumbTimer();
		}
		// for bluetooth scanners maintain the bluetooth state machine
		if(IsScannerBluetooth())
		{
			UpdateBluetoothState(&BTStateChange);
		}
	}
	SINGLE_RETURN:
	// use a single return point to make sure that the device stays awake.
	if(IsScannerGPS())
	{
		PokeActivity();
	}
	return nRetVal;
}

//------------------------------------------------------------------
//	store_char_in_string_quant
//  ============================
//	Appends a key entry to a string for numeric entry only.
//------------------------------------------------------------------
static void store_char_in_string_quant( int key, int max_len, char *string, int typ )
{
	int i, ok;

	max_len += ((typ & NEGATIVE)&&(string[0]=='-'))?1:0;
	i = strlen( string );

	if(( i >= max_len ) && (key != '-'))
	{
		return;
	}

	ok = 0;

	if( typ & NUM )
		ok = isdigit( key );
	if( !ok && ( typ & NEGATIVE ))
		ok = ( key == '-')? TRUE : FALSE;
	if( ok )
	{
		if( key == '-')
		{
			if( string[0] == '-' )
			{
				memmove( string, string+1, i);
			}
			else
			{
				memmove( string+1, string, i);
				string[0] = '-';
			}
		}
		else
		{
			string[i++] = key;
			string[i] = '\0';
		}
	}
}

//------------------------------------------------------------------
//	display_input_quantity
//  ============================
//	Displays a quantity string.
//------------------------------------------------------------------
static void display_input_quantity( char* string, int x, int y, int max_length,int max_x, int start, long def_val )
{
	int output;

	if( x < 0 || y < 0 )
		return;

	gotoxy(x,y);
	if( start )
		output = printf("%*ld", max_x, def_val );
	else
		output = printf("%*.*s", max_x, max_x, string );

	gotoxy( x+output-1, y); // set the cursor to the correct position
}

//
// max_lenght is the maximum length without the minus sign (-)
//------------------------------------------------------------------
//	KeyboardQuantity
//  ============================
//	Quantity entry and display.
//------------------------------------------------------------------
int KeyboardQuantity( char* string, int max_length, int typ, int x, int y, int offset, long def_val, int max_x_d, int rect, int num, ... )
{
	va_list key_list;
	int key;
	int defaul;

	defaul = TRUE;
	if( num <= 0 )
		return( EOF );

	for(;;)
	{
		va_start( key_list, num );

		setoffset( offset );

		display_input_quantity(string, x, y, max_length, max_x_d, defaul, def_val);
		if( rect )
			rectangle( (x*8)-2, (y*16)+offset, ((x+max_x_d)*8), ((y+1)*16)-3+offset, 1);

		key =KAwaitkey(battery_type );    //KA changed waitkey()

		if( check_key_input( key, num, key_list ) == OK )
		{
			va_end( key_list );
			setoffset( 0 );

			if( defaul )
				return( key | DEFAULT );
			else
				return( key  );
		}
		va_end(key_list);
		defaul = FALSE;

		if( key == BS_KEY )
		{
			remove_one_from_buffer( string );
			continue;
		}

		store_char_in_string_quant( key,  max_length, string, typ );
	}
}

//  The following functions are used for a timer system that allows multiple timers to be maintained.

//------------------------------------------------------------------
//	check_timeout
//  ============================
//	Check if the general timer has expired.
//------------------------------------------------------------------
int check_timeout( void )
{
	// only check if the timer is enabled
	if(g_util_timer == 1)
	{
		return(endtimer());
	}
	else
	{
		// return as if time has not elapsed
		return(0);
	}
}

// call if main timer has expired (1 second interval)
//------------------------------------------------------------------
//	CheckTimerExpired
//  ============================
//	Checks if the specified timer has expired.
//------------------------------------------------------------------
int CheckTimerExpired(int nTimerID)
{
	int nTimerExpired = TIMER_STATUS_DISABLED;

	if(rgTimers[nTimerID].fEnabled == TRUE)
	{
		if(rgTimers[nTimerID].nTimerCount == 0)
		{
			nTimerExpired = TIMER_STATUS_EXPIRED;
			if(rgTimers[nTimerID].fAutoRestart == TRUE)
			{
				rgTimers[nTimerID].nTimerCount = rgTimers[nTimerID].nTimerDuration;
			}
		}
		else
		{
			nTimerExpired = TIMER_STATUS_RUNNING;
		}
	}

	return nTimerExpired;
}

//------------------------------------------------------------------
//	CheckMyTimer
//  ============================
//	Maintains the timer system and checks if the specified timer has expired.
//	This function must be called periodically to operate the timer system.
//	For gPS devices, thei function also operates the auto shutoff in order to ensure
//	that the GPS receiver is turned off.
//------------------------------------------------------------------
int CheckMyTimer(int nTimerID)
{
	int nRetVal = TIMER_STATUS_DISABLED;
	int nIndex;

	// if one second has elapsed
	if(endtimer() == TRUE)
	{
		// decrement all active timers
		for(nIndex=0;nIndex<=TIMER_ID_MAX;nIndex++)
		{
			if((rgTimers[nIndex].fEnabled == TRUE) && (rgTimers[nIndex].nTimerCount > 0))
			{
				rgTimers[nIndex].nTimerCount--;
			}
		}
		// restart for another second
		starttimer(50);
	}
	// service other timers for the power management for GPS models
	if(IsScannerGPS())
	{
		if(CheckTimerExpired(TIMER_ID_POWER_DOWN) == TIMER_STATUS_EXPIRED)
		{
			// power down the GPS and the terminal
			GpsPower(OFF);
			autopowerdown(NOW, 0);
		}
		if(powereddown() == 1)
		{
			// unit has been powered down
			GpsPower(ON);
		}
	}

	nRetVal = CheckTimerExpired(nTimerID);

	return nRetVal;
}

//------------------------------------------------------------------
//	StartMyTimer
//  ============================
//	Starts the specified timer with the duration and auto restart.
//------------------------------------------------------------------
int StartMyTimer(int nTimerID, unsigned long nDuration, int fAutoRestart)
{
	int nRetVal = TIMER_STATUS_DISABLED;
	int nScaledDuration = (nDuration / 50) + 1;       // divide by 50 and add one to get duration in seconds

	rgTimers[nTimerID].fEnabled = TRUE;
	rgTimers[nTimerID].fAutoRestart = fAutoRestart;
	rgTimers[nTimerID].nTimerCount = nScaledDuration;
	rgTimers[nTimerID].nTimerDuration = nScaledDuration;

	// if the timer is not running already
	if(endtimer() == TRUE)
	{
		starttimer(50);
	}

	return nRetVal;
}

//------------------------------------------------------------------
//	StopMyTimer
//  ============================
//	Stops the specified timer.
//------------------------------------------------------------------
int StopMyTimer(int nTimerID)
{
	int nRetVal = TIMER_STATUS_DISABLED;

	rgTimers[nTimerID].fEnabled = FALSE;
	rgTimers[nTimerID].fAutoRestart = FALSE;
	rgTimers[nTimerID].nTimerCount = 0L;
	rgTimers[nTimerID].nTimerDuration = 0L;

	return nRetVal;
}

//------------------------------------------------------------------
//	InitializeMyTimer
//  ============================
//	Stops all timers
//------------------------------------------------------------------
void InitializeMyTimer(void)
{
	int nIndex;

	// clear the timer structures
	for(nIndex=0;nIndex<TIMER_ID_MAX;nIndex++)
	{
		StopMyTimer(nIndex);
	}
}

//------------------------------------------------------------------
//	ResetMyTimer
//  ============================
//	Resets the specified timer to its full duration.
//------------------------------------------------------------------
void ResetMyTimer(int nTimerID)
{
	if(rgTimers[nTimerID].fEnabled == TRUE)
	{
		rgTimers[nTimerID].nTimerCount = rgTimers[nTimerID].nTimerDuration;
	}
}

//------------------------------------------------------------------
//	PokeActivity
//  ============================
//	Resets the autopower down timer.
//------------------------------------------------------------------
void PokeActivity(void)
{
	ResetMyTimer(TIMER_ID_POWER_DOWN);
	GpsPower(ON);
}

//------------------------------------------------------------------
//	PauseMyTimer
//  ============================
//	Pauses the specified timer.
//------------------------------------------------------------------
void PauseMyTimer(int nTimerID)
{
	rgTimers[nTimerID].fEnabled = FALSE;
}

//------------------------------------------------------------------
//	ResumeMyTimer
//  ============================
//	Resumes the specified timer.
//------------------------------------------------------------------
void ResumeMyTimer(int nTimerID)
{
	rgTimers[nTimerID].fEnabled = TRUE;
}

// end timer functions

//------------------------------------------------------------------
//	set_font_menu
//  ============================
//	Sets font and cursor for menus.
//------------------------------------------------------------------
void set_font_menu( void )
{
	setfont( LARGE_FONT, NULL );
	cursor( NOWRAP );
	cursor( OFF );
}

//------------------------------------------------------------------
//	set_font_input
//  ============================
//	Sets font and cursor for input.
//------------------------------------------------------------------
void set_font_input( void )
{
	setfont( MEDIUM_FONT, NULL );

	cursor( NOWRAP );
	cursor( OFF );
}

//------------------------------------------------------------------
//	clear_screen
//  ============================
//	Clears the screen.
//------------------------------------------------------------------
void clear_screen(void)
{
	putchar('\f');
}

