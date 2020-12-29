/*
 * menu.c
 *
 *  Created on: Mar 7, 2012
 *      Author: kogami
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include "lib.h"
#include "menu.h"
#include "utils.h"
#include "main.h"
#include "fancy.h"
#include "pdf_lib.h"

// image header system menu
#include "fnt\\datetime.fnt"	// date time icon
#include "fnt\\barcode.fnt"	// barcode icon
#include "fnt\\brain.fnt"   	// brain icon
#include "fnt\\cards.fnt"	// card boc icon
#include "fnt\\nodata.fnt"	// empty carbox icon
#include "fnt\\tools.fnt"	// tools icon
#include "fnt\\trashfl.fnt"	// filled trashbin icon
#include "fnt\\trashemp.fnt"	// empty trashbin icon
#include "fnt\\version.fnt"	// question mark on sheet icon
#include "fnt\\battsm.fnt"	// battery icon
#include "fnt\\lr2.fnt"		// left right arrow icon
#include "fnt\\flashi.fnt"	// inversed lightning beam
#include "fnt\\comp.fnt"		// searial computer connector icon
#include "fnt\\standby.fnt"	// computer screen icon with ZZZ
#include "fnt\\battvolt.fnt" //
#include "fnt\\opl9723.fnt"
#include "fnt\\opldata.fnt"
#include "fnt\\oplsend.fnt"
#include "fnt\\any_key.fnt"	// press any key bar and yes no bar
#include "fnt\\msgicons.fnt" // message icons
#include "fnt\\light.fnt"	// light bulb icon

#include "fnt\\optiobar.fnt"     // contains a bar that holds back quantity delete
#include "fnt\\KA_bar.fnt"       // contains a bar that holds back  delete
#include "fnt\\KA8_exit.fnt"     //bar for EXIT  SCAN  OPTIONS - batch mode scan
#include "fnt\\KA_utils.fnt"     // icon for Sys Menu Utilities
#include "fnt\\KA_int.fnt"	    //icon for Sys menu Interface
#include "fnt\\KA8_back.fnt"		//BACK SCAN - batch mode
#include "fnt\\opl_blue.fnt"     //icon for Data Mode
#include "fnt\\qty.fnt"          //icon for OTY
#include "fnt\\splash.fnt"       //icon for Splash screen
#include "fnt\\b_tooth.fnt"      //icon for BT
#include "fnt\\KA8_real.fnt"     //font for EXIT SCAN
#include "fnt\\KA_Ma_Sl.fnt"     //Master/Slave icon for BT Menu
#include "fnt\\no_bt_ad.fnt"     //icon for Clear BT address

#include "fnt\\scrolbar.fnt" // caintains the bar that is displayed in the scroll data menu

#include "fnt\\globe.fnt"        //GPS menu picture
#include "fnt\\breadcrumbs.fnt"  //bread crumb menu picture
#include "fnt\\gpstest.fnt"      //GPS test picture
#include "fnt\\gpsformat.fnt"    //GPS format menu picture
#include "fnt\\gpstimezone.fnt"  //GPS time zone picture

extern scannerType scanner;

#ifndef NULL
#define NULL	0
#endif

const char back_string[]        = "    (back)";
const char jan13_string[]       = "JAN 13";
const char ean13_string[]       = "EAN 13";
const char upca_string[]        = "UPC A";
const char jan8_string[]        = "JAN 8";
const char ean8_string[]        = "EAN 8";
const char upce_string[]        = "UPC E";
const char instore13_string[]   = "INSTORE 13";
const char instore8_string[]    = "INSTORE 8";
const char code39_string[]      = "CODE 39";
const char codabar_string[]     = "CODABAR";

const char d2of5_string[]       = "INDUST 2 OF 5";
const char i2of5_string[]       = "INTERL 2 OF 5";
const char code93_string[]      = "CODE 93";
const char code128_string[]     = "CODE 128";
const char msi_string[]         = "MSI PLESSEY";
const char iata_string[]        = "IATA";
const char uk_string[]          = "UK PLESSEY";
const char telepen_string[]     = "TELEPEN";
const char m2of5_string[]       = "MATRIX 2 OF 5";
const char chinpost_string[]    = "CHINESE POST";
const char nw7_abc_string[]     = "CODABAR ABC";
const char nw7_cx_string[]      = "CODABAR CX";
const char scode_string[]       = "SCODE";
const char laetus_string[]      = "LAETUS";
const char c39_fa_string[]      = "CD 39 FULL ASCII";
const char c39_it_string[]      = "ITALIAN PHARMA.";
const char unknown_string[]     = "UNKNOWN CODE";
const char all_addon_string[]   = "All ADDONS";

const char ean_string[]         = "EAN";
const char upc_string[]         = "UPC";
const char rss14_string[]       = "RSS-14";
const char rssL_string[]        = "RSS-Limited";
const char rssE_string[]        = "RSS-Expanded";
const char pdf417_string[]      = "PDF417";
const char updf417_string[]     = "MicroPDF417";

const char DataMatrix_string[]  = "DataMatrix";
const char Aztec_string[]		= "Aztec";
const char Aztec_Runes_string[]	= "Aztec Runes";
const char QR_Code_string[]		= "QR Code";
const char Micro_QR_Code_string[] = "Micro QR Code";
const char Maxicode_string[] 	= "Maxicode";

const char jan13_2_string[]     = "JAN 13 + 2";
const char ean13_2_string[]     = "EAN 13 + 2";
const char upca_2_string[]      = "UPC A + 2";
const char jan8_2_string[]      = "JAN 8 + 2";
const char ean8_2_string[]      = "EAN 8 + 2";
const char upce_2_string[]      = "UPC E + 2";
const char code128_2_string[]   = "CODE 128 + 2";

const char jan13_5_string[]     = "JAN 13 + 5";
const char ean13_5_string[]     = "EAN 13 + 5";
const char upca_5_string[]      = "UPC A + 5";
const char jan8_5_string[]      = "JAN 8 + 5";
const char ean8_5_string[]      = "EAN 8 + 5";
const char upce_5_string[]      = "UPC E + 5";
const char code128_5_string[]   = "CODE 128 + 5";

const char menu_string[]        = "MENU CODE";

const fancyselmenus barcodem_with2d[] =
{
		{(char*)back_string,         -1},
		{(char*)code39_string,       ID_CODE39},
		{(char*)ean_string,          ID_EAN},
		{(char*)upc_string,          ID_UPC},
		{(char*)i2of5_string,        ID_I2OF5},
		{(char*)d2of5_string,        ID_D2OF5},
		{(char*)codabar_string,      ID_CODABAR},
		{(char*)code93_string,       ID_CODE93},
		{(char*)code128_string,      ID_CODE128},
		{(char*)msi_string,          ID_MSI_PLESSEY},
		{(char*)telepen_string,      ID_TELEPEN},
		{(char*)uk_string,           ID_UK_PLESSEY},
		{(char*)iata_string,         ID_IATA},
		{(char*)scode_string,        ID_SCODE},
		{(char*)m2of5_string,        ID_M2OF5},
		{(char*)rss14_string,        ID_RSS_14},
		{(char*)rssL_string,         ID_RSS_LIMITED},
		{(char*)rssE_string,         ID_RSS_EXPANDED},
		{(char*)all_addon_string,    ID_ADDONS},
		{(char*)pdf417_string,       ID_PDF417},
		{(char*)updf417_string,      ID_MICRO_PDF417},
		{(char*)DataMatrix_string,   ID_DATAMATRIX },
		{(char*)Aztec_string,        ID_AZTEC },
		{(char*)Aztec_Runes_string,	 ID_AZTEC_RUNES },
		{(char*)QR_Code_string,		 ID_QR_CODE },
		{(char*)Micro_QR_Code_string,ID_MICRO_QR_CODE },
		{(char*)Maxicode_string,	 ID_MAXICODE },
};

const fancyselmenus barcodem[] =
{
		{(char*)back_string,         -1},
		{(char*)code39_string,       ID_CODE39},
		{(char*)ean_string,          ID_EAN},
		{(char*)upc_string,          ID_UPC},
		{(char*)i2of5_string,        ID_I2OF5},
		{(char*)d2of5_string,        ID_D2OF5},
		{(char*)codabar_string,      ID_CODABAR},
		{(char*)code93_string,       ID_CODE93},
		{(char*)code128_string,      ID_CODE128},
		{(char*)msi_string,          ID_MSI_PLESSEY},
		{(char*)telepen_string,      ID_TELEPEN},
		{(char*)uk_string,           ID_UK_PLESSEY},
		{(char*)iata_string,         ID_IATA},
		{(char*)scode_string,        ID_SCODE},
		{(char*)m2of5_string,        ID_M2OF5},
		{(char*)rss14_string,        ID_RSS_14},
		{(char*)rssL_string,         ID_RSS_LIMITED},
		{(char*)rssE_string,         ID_RSS_EXPANDED},
		{(char*)pdf417_string,       ID_PDF417},
		{(char*)updf417_string,      ID_MICRO_PDF417},
		{(char*)all_addon_string,    ID_ADDONS},
};


const char day_gb_sunday[]      = "  Sunday  ";
const char day_gb_monday[]      = "  Monday  ";
const char day_gb_tuesday[]     = "  Tuesday ";
const char day_gb_wednesday[]   = " Wednesday";
const char day_gb_thursday[]    = " Thursday ";
const char day_gb_friday[]      = "  Friday  ";
const char day_gb_saturday[]    = " Saturday ";

const days day_gb[] =
{
		{ (char*)day_gb_sunday },
		{ (char*)day_gb_monday },
		{ (char*)day_gb_tuesday },
		{ (char*)day_gb_wednesday },
		{ (char*)day_gb_thursday },
		{ (char*)day_gb_friday },
		{ (char*)day_gb_saturday }
};

const char baud_115200[]    = "115200 Baud";
const char baud_57600[]     = " 57600 Baud";
const char baud_38400[]     = " 38400 Baud";
const char baud_19200[]     = " 19200 Baud";
const char baud_9600[]      = "  9600 Baud";
const char baud_4800[]      = "  4800 Baud";
const char baud_2400[]      = "  2400 Baud";


const fancyselmenus baudm[] =
{
		{(char*)back_string, -1},
		{(char*)baud_115200, ID_115200},
		{(char*)baud_57600,  ID_57600},
		{(char*)baud_38400,  ID_38400},
		{(char*)baud_19200,  ID_19200},
		{(char*)baud_9600,   ID_9600},
		{(char*)baud_4800,   ID_4800},
		{(char*)baud_2400,   ID_2400}
};

const char int_cradle[]          = "     Cradle";
const char int_BT_Batch[]        = "BT Batch Mode";
const char int_BT_RealTime[]     = " BT Real Time";
const char int_USB[]             = "  USB Cable";

const fancyselmenus interfacem[] =
{
		{(char*)back_string,  		-1},
		{(char*)int_cradle, 		INTERFACE_CRADLE},
		{(char*)int_BT_Batch, 		INTERFACE_BT_BATCH},
		{(char*)int_BT_RealTime, 	INTERFACE_BT_REAL_TIME},
		{(char*)int_USB, 			INTERFACE_USB}
};

const char qty_enter_only[]   = "   Enter QTY";
const char qty_required[]     = "  Collect QTY";
const char qty_not_required[] = "    No QTY";

const fancyselmenus qtym[]=
{    //dta collection options menu
		{(char*)back_string,             -1},
		{(char*)qty_required,            QTY_REQUIRED},
		{(char*)qty_enter_only,          QTY_ENTER_ONLY},
		{(char*)qty_not_required,        QTY_NOT_REQUIRED}
};


const char standby_5[]      = " 5 seconds";
const char standby_10[]     = "10 seconds";
const char standby_15[]     = "15 seconds";
const char standby_30[]     = "30 seconds";
const char standby_45[]     = "45 seconds";
const char standby_60[]     = "60 seconds";
const char standby_120[]    = "120 seconds";
const char standby_180[]    = "180 seconds";

const fancyselmenus standbym[] =
{
		{(char*)back_string,         -1},
		{(char*)standby_5,           ST_05_SEC},
		{(char*)standby_10,          ST_10_SEC},
		{(char*)standby_15,          ST_15_SEC},
		{(char*)standby_30,          ST_30_SEC},
		{(char*)standby_45,          ST_45_SEC},
		{(char*)standby_60,          ST_60_SEC},
		{(char*)standby_120,         ST_120_SEC},
		{(char*)standby_180,         ST_180_SEC}
};

const char backlight_off[]      = "     OFF";
const char backlight_on[]       = "     ON";
const char backlight_auto[]     = "    AUTO";

const fancyselmenus backlightm[] =
{
		{(char*)back_string,         -1},
		{(char*)backlight_off,       OFF},
		{(char*)backlight_on,        ON},
		{(char*)backlight_auto,      AUTO},
};

const char protocol_no[]		= "No Protocol";
const char protocol_neto[]		= "NetO Protocol";
const char protocol_xmodem[]	= "XMODEM Protocol";
const char protocol_osecomm[]	= "OseComm Protocol";

const fancyselmenus protocolm[] =
{
		{(char*)back_string, -1},
		{(char*)protocol_no,     ID_NO_PROTOCOL},
		{(char*)protocol_neto,   ID_NETO_PROTOCOL},
		{(char*)protocol_xmodem, ID_XMODEM_PROTOCOL},
		{(char*)protocol_osecomm, ID_OSECOMM_PROTOCOL}
};


const char gps_brd_off[]         = "Off";
const char gps_brd_1m[]          = "1 minute";
const char gps_brd_5m[]          = "5 minutes";
const char gps_brd_15m[]         = "15 minutes";
const char gps_brd_30m[]         = "30 minutes";

const fancyselmenus gpsBrdCrmb[] =
{
		{(char*)back_string,  -1},
		{(char*)gps_brd_off, GPS_BRD_OFF},
		{(char*)gps_brd_1m, GPS_BRD_1_M},
		{(char*)gps_brd_5m, GPS_BRD_5_M},
		{(char*)gps_brd_15m, GPS_BRD_15_M},
		{(char*)gps_brd_30m, GPS_BRD_30_M}
};

const char gps_format_raw[]         = "NMEA";
const char gps_format_DD[]          = "Degrees";
const char gps_format_DMS[]         = "Deg:Min:Sec";

const fancyselmenus gpsFormats[] =
{
		{(char*)back_string,  -1},
		{(char*)gps_format_raw, GPS_RAW},
		{(char*)gps_format_DD, GPS_DECIMAL_DEGREE},
		{(char*)gps_format_DMS, GPS_DEG_MIN_SEC}
};

const char timeZoneLabel[24][16] =
{
		"Midway",           // UTC-11
		"Hawaii",           // UTC-10
		"Anchorage",        // UTC-9
		"Pacific",          // UTC-8
		"Mountain",         // UTC-7
		"Central",          // UTC-6
		"Eastern",          // UTC-5
		"Bermuda",          // UTC-4
		"Sao Paolo",        // UTC-3
		"Sandwich Isles",   // UTC-2
		"Azores",           // UTC-1
		"London",           // UTC
		"Paris",            // UTC+1
		"Athens",           // UTC+2
		"Belarus",          // UTC+3
		"Moscow",           // UTC+4
		"Pakistan",         // UTC+5
		"Kazakhstan",       // UTC+6
		"Singapore",        // UTC+7
		"Beijing",          // UTC+8
		"Tokyo",            // UTC+9
		"Sydney",           // UTC+10
		"Vladivostok",      // UTC+11
};

fancymenus mainmenu[6];           //main menu selections

int menu_state;
int NOT_IN_MENU_MODE =     0;
int IN_MENU_MODE   =       1;
int EXITING_MENU_MODE   = 2;     // end label was scanned when in menu mode

//defaults
int menu_layers[5];
long enabled_barcodes;
long baudrate_enabled;
long protocol_enabled;
long standby_enabled;
long backlight_enabled;
long interface_enabled;
long gps_bread_crumbs;
long gps_data_format;
long time_zone_offset;
long time_zone_change;

char *protocol_desc;        //description for display purposes
char *interface_desc;       //description for display purposes

char temp_mac [MAX_ADDRESS+1];    //stores temporary MAC address

int battery_type = BAT_CHARGE;                 //global var to test if model has a chargable battery (OPL) or alkaline (DCL)
//to avoid problems with check_battery_empty() & is_on_cradle() functions
int batt_power;
int battery_msg_status;

int g_fSuspendGPS;
int g_fSuspendConversion;

int prefix;
int suffix;
Param param_tab[MAX_SYMBOLOGIES];

const char time_mask[] = "29:59:59";
const char date_mask[] = "39/19/2999/E";

menuSettings currentMenu;

// Bluetooth PIN entry exchange variables
int g_bltPINCallback;

extern splash_screen splash_msg;
// _signs hold special sign font
extern char _signs[];

// GPS breadcrumb timer
extern int g_GPS_breadcrumbTimer;
extern int g_GPS_Test_Mode;
// breadcrumb record string name + placeholder for seven digit bread crumb number
extern const char g_pszGPSBreadCrumb[];
extern int g_nBreadCrumbIndex;

extern unsigned long g_nGPSStandbyTimeout;   // GPS standby timeout as configured by system menu
extern char g_Longitude[];
extern char g_Latitude[];
extern const char g_EmptyLatitude[];
extern const char g_EmptyLongitude[];
extern int g_gpsfix;
extern int g_BreadCrumbFlag;

extern int g_BreadCrumbTime;

// timer started flag for utilities
extern int g_util_timer;
extern int g_gpsOptions;

extern char BTLocalName[];

extern int BTState;
extern int BTStateChange;
extern int com_opened;
extern char g_termID[5];
extern char app_version[];

extern int UpdateBluetoothState(int *stateChange);
extern void set_font_input( void );
extern int KAfancyselmenu(fancyselmenus *items, int layers[], char* elements, long* value, int more);

// Local functions
static int put_barcode_real_time( char* barcode );

//------------------------------------------------------------------
//	InitMenu
//  ============================
//	Initializes the main menu array.
//------------------------------------------------------------------
void InitMenu(void)
{

	strcpy(param_tab->suffix,DEFAULT_SUFFIX);
	menu_state = NOT_IN_MENU_MODE;

	battery_msg_status = 0;

	mainmenu[0].itemfunc = main_scan;
	mainmenu[1].itemfunc = main_scroll;
	mainmenu[2].itemfunc = system_menu;
	mainmenu[3].itemfunc = main_delete;
	mainmenu[4].itemfunc = main_send;
	mainmenu[5].itemfunc = main_version;

	mainmenu[2].fnt_picture = (unsigned char*)_tools;
	mainmenu[4].fnt_picture = (unsigned char*)_oplsend;
	mainmenu[5].fnt_picture = (unsigned char*)_version;

	mainmenu[0].item = " Scan labels";
	mainmenu[1].item = " Scroll data";
	mainmenu[2].item = " System menu";
	mainmenu[3].item = " Delete data";
	mainmenu[4].item = "  Send data ";
	mainmenu[5].item = "  Versions  ";

	memset( menu_layers, 0, sizeof( menu_layers ));
	menu_layers[0] = 1;                   // start item of the main menu
}

//------------------------------------------------------------------
//	InitMenu
//  ============================
//	Executes the main menu.
//------------------------------------------------------------------
void MainMenu(void)
{
	int ret;

	if ( fsize((char*)DATA_BASE) == -1L )
	{
		// No database present
		mainmenu[0].fnt_picture = (unsigned char*)_opl9723;
		mainmenu[1].fnt_picture = (unsigned char*)_nodata;
		mainmenu[3].fnt_picture = (unsigned char*)_trashemp;
	}
	else
	{
		// data present in database
		mainmenu[0].fnt_picture = (unsigned char*)_opldata;
		mainmenu[1].fnt_picture = (unsigned char*)_cards;
		mainmenu[3].fnt_picture = (unsigned char*)_trashfl;
	}

	ret = fancymenu( mainmenu, menu_layers, sizeof( mainmenu ) / sizeof( fancymenus) );
	if ( ret == -1 )
	{
		if ( !menu_layers[0] )
			menu_layers[0] = 1;
		goto MENU_SINGLE_RETURN;
	}

	if (scanner.BT_Interface == INTERFACE_BT_REAL_TIME && ret != 0)
	{
		close_connection();   //close connection when not in scan mode to save power
	}

	mainmenu[ ret ].itemfunc();
	MENU_SINGLE_RETURN:
	return;
}

//------------------------------------------------------------------
//	print_message_bar
//  ============================
//	Prints the the bottom message bar according to the specified type.
//------------------------------------------------------------------
void print_message_bar( int bartype  )
{
	if ( bartype & MB_ALL )
		setfont( USER_FONT, (const unsigned char *) _any_key );
	else if ( bartype & MB_YESNO )
		setfont( USER_FONT, (const unsigned char *) _yesno );
	else if ( bartype & OPTION_BAR )
		setfont( USER_FONT, (const unsigned char *) _optiobar );
	else if ( bartype & OPTION_BAR_NO_QTY )               //KA font for BACK        DELETE bar for no QTY option
		setfont( USER_FONT, (const unsigned char *) _KA_bar );
	else if ( bartype & EXIT_OK_OPTIONS )                 //KA font for BACK        DELETE bar for no QTY option
		setfont( USER_FONT, (const unsigned char *) _KA8_exit );
	else if ( bartype & EXIT_SCAN )               //KA font for EXIT  SCAN bar for real time scanning
		setfont( USER_FONT, (const unsigned char *)  _KA8_real );
	else if ( bartype & BACK_SCAN )               //KA font for EXIT  SCAN bar for real time scanning
		setfont( USER_FONT, (const unsigned char *)  _KA8_back );


	else if ( bartype & SCROLL_BAR )
		setfont( USER_FONT, (const unsigned char *) _scrolbar );
	else
		return;

	gotoxy(0,7);
	printsymbol( 0 );
	gotoxy(1,7);
	printsymbol( 1 );
	gotoxy(2,7);
	printsymbol( 2 );
	gotoxy(3,7);
	printsymbol( 3 );
	gotoxy(4,7);
	printsymbol( 4 );
	gotoxy(5,7);
	printsymbol( 5 );
	gotoxy(6,7);
	printsymbol( 6 );
	gotoxy(7,7);
	printsymbol( 7 );
	gotoxy(8,7);
	printsymbol( 8 );
	gotoxy(9,7);
	printsymbol( 9 );
	gotoxy(10,7);
	printsymbol( 10 );
	gotoxy(11,7);
	printsymbol( 11 );
	gotoxy(12,7);
	printsymbol( 12 );
	gotoxy(13,7);
	printsymbol( 13 );

	set_font_input();
}

//------------------------------------------------------------------
//	main_scan
//  ============================
//	Scanning barcodes from the main menu item.
//------------------------------------------------------------------
void main_scan( void )
{
	static char barc[ BCR_MAX + 1 ];
	static char quant[ SZ_SIGN+SZ_INPUT_QUANT];
	static db_record db_rec;
	static db_record db_found;
	static char readed_rec[ SZ_RECORD + 1];
	long found_record;
	long fnd_quantity;
	long new_quantity;
	int ret_code;
	int qu_ret;
	int check_ret = 0;
	int illegal;
	int real_time_mode = 0;
	int pos = 0;
	int offset = 0;
	struct date d;
	struct time t;

	memset( barc, '\0', sizeof( barc ));
	memset( &db_rec, '\0', sizeof( db_record ));

	if (interface_enabled == INTERFACE_BT_REAL_TIME)    //
	{
		real_time_mode = 1;
		clear_screen();
		if (init_connection() < 0 )
		{
			clear_screen();
			setfont(MEDIUM_FONT,NULL);
			gotoxy(0,1);
			printf("  Bluetooth Error\n\n\n   Press any Key");
			errbeep();
			resetkey();
			KAwaitkey( battery_type );
			close_connection();
			return;
		}
	}

	else
	{
		real_time_mode = 0;
	}

	putchar('\f');

	for (;;)
	{
		if (real_time_mode == 0)  //batch mode
		{
			pos    = 1;             //line 3
			offset = 0;

			display_scan_barcode( &db_rec );     //display normal screen QTY and bottom bar (Back     Options)
		}
		else     //real time mode
		{
			clear_screen();
			setfont(MEDIUM_FONT,NULL);
			cursor(NOWRAP);
			gotoxy(0,0);
			printf ("\tr    CONNECTED     \tr");
			gotoxy(0,1);
			printf ( "%s",barc ); //

			print_message_bar( EXIT_SCAN );
		}

		memset( barc, '\0', sizeof( barc ) );

		//get the barcode
		ret_code = ScanOrKeyboardInput( barc, 1, BCR_MAX, ALPHA | NUM , 0, pos, offset );

		if (ret_code == MENU_CODE)
		{
			process_menu_code(barc);
			continue;
		}

		if ( ret_code == SCANNED || ret_code == KEYBOARD )
		{
			okbeep();
			goodreadled( GREEN, 25 );
			if (real_time_mode == 1)
			{
				getdate( &d );
				gettime( &t );

				if(IsScannerGPS())
				{
					sprintf(readed_rec,"%s;%02d:%02d:%02d;%02d/%02d/%04d;%s,%s%s",
					barc, t.ti_hour, t.ti_min, t.ti_sec, d.da_day, d.da_mon, d.da_year, g_Latitude, g_Longitude, param_tab->suffix);
				}
				else
				{
					sprintf(readed_rec,"%s;%02d:%02d:%02d;%02d/%02d/%04d%s",barc, t.ti_hour, t.ti_min, t.ti_sec, d.da_day, d.da_mon, d.da_year,param_tab->suffix);
				}

				if(IsScannerGPS() && IsGPSTag())
				{
					put_barcode_real_time( readed_rec );
				}
				else
				{
					sprintf(readed_rec,"%s%s", barc, param_tab->suffix);
					put_barcode_real_time( readed_rec );
				}
				continue;
			} //end if real time mode

			fnd_quantity = 0L;
			create_db_rec_struct( &db_rec, barc, 0L );

			// only search if the quantity is REQUIRED
			if (scanner.QTY_Required == QTY_REQUIRED)
			{
				// add the quantity
				// but first search if the previous barcode already exists
				found_record =  bin_search( (char*)DATA_BASE, readed_rec, db_rec.barcode, SZ_BARCODE, 0, SZ_RECORD);
				if ( found_record != -1L )
				{
					fill_record_struct( &db_found, readed_rec );
					fnd_quantity = string_quantity_to_long( db_found.quantity, &illegal );
				}
			}
			else
			{
				// this forces the bar codes to be stored individually without accumulating the quantities
				found_record = -1L;
				// the quantity is incremented so that it will be 1
				fnd_quantity = 0L;
			}

			if (scanner.QTY_Required != QTY_REQUIRED)
			{
				// store in database  without showing Qty options
				if (coreleft() > 5000)      //use 5K to leave some RAM for Neto & OS
				{
					create_db_rec_struct( &db_rec, barc, (fnd_quantity + 1)  );
					save_data( &db_rec, -1L );
					if(scanner.QTY_Required == QTY_ENTER_ONLY)
					{
						found_record = GetLastRecord(SZ_RECORD);
					}
				}
				else
				{
					memory_full_error();
				}
				continue;
			}

			// get Qty
			for (;;)           // start of input for loop
			{
				display_scan_quantity( &db_rec, fnd_quantity );    //display barcode and current total

				// setfont( LARGE_FONT, NULL );
				memset( quant, '\0', sizeof( quant ));

				if (scanner.Full_Keypad == 0)
				{
					memset(quant,'\0',sizeof(quant));
					get_qty_no_keyboard(quant);   //3-button models
					qu_ret =1;

				}
				else
				{
					//full keypad models
					qu_ret = KeyboardQuantity( quant,   // string that holds the typed quantity
							SZ_INPUT_QUANT,     // the max input quantity size
							NEGATIVE | NUM,     // negative and numeric value possible
							5,                  // x starting possition
							3,                  // y position
							0,                  // offset position
							1L,                 // default value
							SZ_INPUT_QUANT+SZ_SIGN, // the max display size
							FALSE,              // show an input rectangle
							4,                  // number of special keys
							CLR_KEY, TRIGGER_KEY, DOWN_KEY, ENT_KEY ); // the special keys to return from function

					// cut off the default flag if it exists
					if ( (qu_ret & 0xFF) == CLR_KEY || (qu_ret & 0xFF) == DOWN_KEY)
					{
						return;
					}
				}//end else - full keypad

				if ( (check_ret = check_input_quant( quant, (qu_ret & DEFAULT), 1L, fnd_quantity, &new_quantity )) == ERR_QUANTITY_ILLEGAL )
				{
					continue;
				}
				break;
			} // end of input for loop

			if ( check_ret != ERR_QUANTITY_MIN_MAX )
			{
				if ( (fnd_quantity + new_quantity) == 0L )
				{
					remove_record( found_record );
					memset( &db_rec, '\0', sizeof( db_record ));
				}
				else
				{
					// store in database
					if (coreleft () > 5000)  //use 5K to leave some RAM for Neto & OS
					{
						create_db_rec_struct( &db_rec, barc, (fnd_quantity + new_quantity) );
						save_data( &db_rec, found_record );
						gotoxy(0,3);
					}
					else
					{
						memory_full_error();
					}
				}
			}
			else
			{
				update_quantity_rec_struct( &db_rec, fnd_quantity );
			}

		}	//ENDIF scanned or keyboard input
		if ( ret_code == CLR_KEY || ret_code == UP_KEY )
		{
			// leave the connection open (kyo)
			//            close_connection();
			return;
		}

		else if ( ret_code == DOWN_KEY && db_rec.barcode[0] != '\0' )
		{
			// change quantity and delete a record from
			// the dtabase are found in the options
			// menu
			show_options_menu( &db_rec );

		}
	}

}

//------------------------------------------------------------------
//	main_scroll
//  ============================
//	Scrolls through stored records.
//------------------------------------------------------------------
void main_scroll( void )
{
	long current;
	long max;
	static db_record db_rec;
	int  infoType;

	if ( fsize((char*)DATA_BASE) == -1L )
	{
		g_fSuspendConversion = TRUE;
		g_fSuspendGPS = TRUE;
		disp_no_database();
		g_fSuspendConversion = FALSE;
		g_fSuspendGPS = FALSE;
		return;
	}

	current = 0L;
	if ( get_record( &db_rec, &current, &max ) < 0 )
	{
		errbeep();
		return;
	}
	set_font_input();
	putchar('\f');
	// start with date time display
	infoType = 0;
	for (;;)
	{
		display_scroll_data( &db_rec, current, max, infoType );
		switch ( KAwaitkey( battery_type ) )
		{
		case RIGHT_KEY:
			current = max;
			break;
		case DOWN_KEY:
			current++;
			if(current > max)
			{
				errkeybeep();
				current = max;
			}
			break;

		case LEFT_KEY:
			current = 0L;
			break;
		case UP_KEY:
			current--;
			break;
		case TRIGGER_KEY:
		case CLR_KEY:
			return;
		case BS_KEY:
			if (scanner.GPS_Model == 1)
			{
				// toggle info display type
				infoType = (infoType == 0) ? 1 : 0;
			}
			break;
		}
		get_record( &db_rec, &current, &max );
	}
}

//------------------------------------------------------------------
//	main_delete
//  ============================
//	Deletes stored file.
//------------------------------------------------------------------
void main_delete( void )
{
	// suspend GPS coordinate conversion for this part
	g_fSuspendConversion = TRUE;
	g_fSuspendGPS = TRUE;

	if ( fsize((char*)DATA_BASE) == -1L )
	{
		disp_no_database();
		display_splash_screen();
		g_fSuspendConversion = FALSE;
		g_fSuspendGPS = FALSE;
		return;
	}

	info_sound();

	putchar('\f');
	printbar( 7, 0, 0 );
	set_font_input();

	gotoxy(0,0);

	printf("\tr    DELETE DATA    \tr");

	if ( show_message("Are you sureyou want to delete the data   file?", MB_YESNO | MB_ICONWARNING ) == YES )
	{
		//remove( (char*)DATA_BASE );
		format2(DRIVE_A);
		info_sound();
		putchar('\f');    //clear screen
		gotoxy (0,2);
		printf(" File Was Deleted.");
		info_sound();
		print_message_bar(MB_ALL);
		resetkey();
		KAwaitkey( battery_type );

		menu_layers[0] = 1;     // start item of the main menu

		// also reset the bread crumb index
		g_nBreadCrumbIndex = 0;
	}

	// resume GPS coordinate conversion
	g_fSuspendConversion = FALSE;
	g_fSuspendGPS = FALSE;

	// don't display the splash screen if the this was called from the gps test
	if(g_GPS_Test_Mode == 0)
	{
		display_splash_screen();
	}
}

//------------------------------------------------------------------
//	interface_menu
//  ============================
//	System communication interface menu.
//------------------------------------------------------------------
void interface_menu( void )
{
	menu_layers[3] = 1;

	KAfancyselmenu( (fancyselmenus *)interfacem, menu_layers,scanner.Interface_options , &interface_enabled, 0);

}

//------------------------------------------------------------------
//	baudrate_menu
//  ============================
//	System communication baudrate menu.
//------------------------------------------------------------------
void baudrate_menu( void )
{
	menu_layers[3] = 1;
	fancyselmenu( (fancyselmenus *)baudm, menu_layers, sizeof(baudm) / sizeof(fancyselmenus), &baudrate_enabled, 0);
}

//------------------------------------------------------------------
//	protocol_menu
//  ============================
//	System communication protocol menu.
//------------------------------------------------------------------
void protocol_menu( void )
{
	menu_layers[3] = 1;
	fancyselmenu( (fancyselmenus *)protocolm, menu_layers, sizeof(protocolm) / sizeof(fancyselmenus), &protocol_enabled, 0);
}

//------------------------------------------------------------------
//	comm_menu
//  ============================
//	System communication menu.
//------------------------------------------------------------------
void comm_menu( void )
{
	int ret;
	fancymenus commmenu[4];

	commmenu[0].itemfunc = NULL;
	commmenu[1].itemfunc = interface_menu;      //sets interface_enabled var
	commmenu[2].itemfunc = baudrate_menu;       //sets baudreate_enabled var
	commmenu[3].itemfunc = protocol_menu;       //sets protocol_enabled var

	commmenu[0].fnt_picture = NULL;
	commmenu[1].fnt_picture = (unsigned char*)_KA_int;    //interface icon
	commmenu[2].fnt_picture = (unsigned char*)_flashi;
	commmenu[3].fnt_picture = (unsigned char*)_lr2;

	commmenu[0].item = (char*) back_string;
	commmenu[1].item = "   Interface";
	commmenu[2].item = "   Baudrate";
	commmenu[3].item = "   Protocol";


	menu_layers[2] = 1;

	for (;;)
	{


		ret = fancymenu( commmenu, menu_layers, sizeof( commmenu ) / sizeof( fancymenus) );
		if ( ret == -1 )
			return; // return to previous menu
		commmenu[ ret ].itemfunc();

	}
}

//------------------------------------------------------------------
//	qty_menu
//  ============================
//	System quantity options menu.
//------------------------------------------------------------------
void qty_menu(void)
{
	menu_layers[2] = 1;
	fancyselmenu( (fancyselmenus *)qtym, menu_layers, sizeof(qtym) / sizeof(fancyselmenus), &scanner.QTY_Required, 0);
}

char messageStr[] = "! U1 BEGIN-PAGE\r\n! U1 SETLP 7 0 24\r\n\r\n\r\n              CONAGRA FOODS MEXICO\r\nID Cliente: 687019\r\nFecha: 06/07/2012 12:33 PM\r\n\r\nCANT DESC                PRECIO        TOTAL    \r\n------------------------------------------------\r\n   1 A2 Nat 85gr             5,00           5,00\r\n   1 A2 Ch L cja           402,00         402,00\r\n   1 Isadora R Bay          12,50          12,50\r\n   1 DM Chic 400gr           7,50           7,50\r\n   1 ARETE                   0,00           0,00\r\n   1 Exhibidor Pir           0,00           0,00\r\n   1 A2 Q Jal Reg            0,00           0,00\r\n   1 A2 Ext M 85gr           5,00           5,00\r\n   1 DM Ch Z 410gr           8,50           8,50\r\n   1 HUNTS 4 Q 360           7,00           7,00\r\n   1 SAL MEX V 475          15,00          15,00\r\n   2 DM En V 215gr           5,50          11,00\r\n   2 A2 Val 93.5gr           5,50          11,00\r\n------------------------------------------------\r\n! U1 SETLP 7 1 48\r\nTOTAL:                                    484,50\r\n! U1 SETLP 7 0 24\r\nProductos: 13\r\nArticulos: 15\r\n\r\n\r\n\r\n! U1 END-PAGE\r\n";

void PutBufferTest(void)
{

	clear_screen();
	// check for the bluetooth connection
	if(blt_is_connected() == FALSE)
	{
		// if not connected, then connect bluetooth
		if (init_connection() < 0 )
		{
			clear_screen();
			setfont(MEDIUM_FONT,NULL);
			gotoxy(0,1);
			printf("  Bluetooth Error\n\n\n   Press any Key");
			sound(TLONG,VMEDIUM,SLOW,SMEDIUM,SHIGH,0);
			resetkey();
			WaitKey();
			close_connection();
			return;
		}
	}
}


//------------------------------------------------------------------
//	bluetooth_menu
//  ============================
//	System bluetooth menu.
//------------------------------------------------------------------
void bluetooth_menu(void)
{
	int ret;
	fancymenus bluetoothmenu[6];

	bluetoothmenu[0].itemfunc = NULL;
	bluetoothmenu[1].itemfunc = get_master_slave_mode;            //allows user to swith Master /Slave modes
	bluetoothmenu[2].itemfunc = SetBluetoothMode;
	bluetoothmenu[3].itemfunc = SetPINCode;
	bluetoothmenu[4].itemfunc = ConnectBluetooth;            //connect to Bluetooth
	bluetoothmenu[5].itemfunc = DisconnectBluetooth;            //disconnect from Bluetooth

	bluetoothmenu[0].fnt_picture = NULL;
	bluetoothmenu[1].fnt_picture = (unsigned char*)_KA_Ma_Sl;
	bluetoothmenu[2].fnt_picture = (unsigned char*)_b_tooth;
	bluetoothmenu[3].fnt_picture = (unsigned char*)_b_tooth;
	bluetoothmenu[4].fnt_picture = (unsigned char*)_b_tooth;
	bluetoothmenu[5].fnt_picture = (unsigned char*)_b_tooth;

	bluetoothmenu[0].item = (char*) back_string;
	bluetoothmenu[1].item = "Master/Slave";
	bluetoothmenu[2].item = "Set Mode";
	bluetoothmenu[3].item = "Set BT PIN";
	bluetoothmenu[4].item = "BT Connect";
	bluetoothmenu[5].item = "BT Disconnect";


	menu_layers[2] = 1;

	for (;;)
	{


		ret = fancymenu( bluetoothmenu, menu_layers, sizeof( bluetoothmenu ) / sizeof( fancymenus) );
		if ( ret == -1 )
			return; // return to previous menu
		bluetoothmenu[ ret ].itemfunc();

	}
}

//------------------------------------------------------------------
//	SetBluetoothMode
//  ============================
//	System bluetoth set mode handler.
//------------------------------------------------------------------
void SetBluetoothMode()
{
	int key;

	setfont( LARGE_FONT, NULL );
	putchar('\f');
	gotoxy(0,0);
	printf("\tr VCP/HID \tr");
	gotoxy(0,2);
	printf("DOWN = VCP");
	gotoxy(0,3);
	printf("UP = HID");
	resetkey();
	key = KAwaitkey( battery_type );

	if (key == DOWN_KEY)
	{
		Bluetooth_VCP_Default();
	}
	else if (key == UP_KEY)
	{
		Bluetooth_HID_Default();
	}
}

//------------------------------------------------------------------
//	SetPINCode
//  ============================
//	System bluetooth set PIN code handler.
//------------------------------------------------------------------
void SetPINCode(void)
{
	int key;
	char pin[MAX_PINCODE+1];

	strncpy(pin, scanner.BT_PIN, MAX_PINCODE+1);

	InputPincode(pin);
	clear_screen();
	printf("Accept PIN Code?\n\n");
	printf("   %s",pin);

	print_message_bar(MB_YESNO);
	resetkey();
	key = KAwaitkey( battery_type );

	if (key == UP_KEY)     //user accepted the PIN
	{
		strcpy(scanner.BT_PIN,pin);
		SetBltPin( pin );
	}

}

//------------------------------------------------------------------
//	SetMACAddress
//  ============================
//	System bluetooth set MAC address handler.
//------------------------------------------------------------------
void SetMACAddress(void)
{
	int key;
	char MACAddress[MAX_ADDRESS+1];

	set_font_input();

	putchar('\f');
	gotoxy(0,0);
	printf("\tr Set BT MAC Addr  \tr");
	gotoxy(0,1);
	printf("1: Discovery");
	gotoxy(0,2);
	printf("2: Scan Barcode");
	gotoxy(0,3);
	printf("3: Keyboard");

	key = KAwaitkey(battery_type);

	switch(key)
	{
	case '1':
		getMACAddress(DISCOVERY_MAC);
		break;
	case '2':
		getMACAddress(SCAN_MAC);
		break;
	case '3':
		strncpy(MACAddress, scanner.BT_Address, MAX_ADDRESS+1);
		InputMACAddress(MACAddress);
		strncpy(temp_mac, MACAddress, MAX_ADDRESS+1);
		break;
	}
	clear_screen();
	gotoxy(0,0);
	printf("Accept BT Addr?");
	gotoxy(0,1);
	printf("%s", temp_mac);
	print_message_bar(MB_YESNO);
	resetkey();
	key = KAwaitkey( battery_type );

	if (key == UP_KEY)     //user accepted the PIN
	{
		strcpy(scanner.BT_Address, temp_mac);
		SetBltAddress(scanner.BT_Address);
	}
}

//------------------------------------------------------------------
//	sys_bluetooth_reset
//  ============================
//	System bluetooth reset handler.
//------------------------------------------------------------------
void sys_bluetooth_reset(void)
{
	//displays BT options menu
	int key;
	putchar ('\f');
	printf("\tr Clear BT Addr\tr\n");
	// printf("Are You Sure\n");
	info_sound();
	printf("\nClear BT Addr?");
	print_message_bar(MB_YESNO);
	resetkey();
	if ((key = KAwaitkey( battery_type )) == UP_KEY)
	{
		memset(scanner.BT_Address,'\0',sizeof(scanner.BT_Address));
		SetBltAddress(scanner.BT_Address);
		scanner.BT_Master_Slave = -1;
		//clear pin code if any
		memset(scanner.BT_PIN ,'\0',sizeof(scanner.BT_PIN));
		SetBltPin(scanner.BT_PIN);
		info_sound();
		putchar ('\f');
		gotoxy(0,1);
		printf(" Bluetooth Address\n was cleared.");
		print_message_bar(MB_ALL);
		resetkey();
		KAwaitkey( battery_type );
	}
}

//------------------------------------------------------------------
//	sys_battery
//  ============================
//	System battery status handler.
//------------------------------------------------------------------
void sys_battery( void )
{
	char buffer[10];

	set_font_input();
	printf("\f\tr  BATTERY VOLTAGE  \tr");
	while(!kbhit())
	{
		gotoxy(0,0);

		printbattery();
		gotoxy(5,2);

		sprintf(buffer,"%04d",GetBatteryVoltage_mV());
		buffer[2]=buffer[1];buffer[3]=buffer[2];buffer[1]='.';
		printf("%s V",buffer);
		rectangle3d( (5*6)-2, ((5-1)*6)-1, ((6+5)*6), ((5+1)*6)-1, 1);

		print_message_bar( MB_ALL );

		is_on_cradle();
		delay(5);
	}
	keybeep();
	resetkey();

}

//------------------------------------------------------------------
//	gps_menu
//  ============================
//	System GPS menu handler.
//------------------------------------------------------------------
void gps_menu( void )
{
	int ret;
	fancymenus gpsmenu[6];

	gpsmenu[0].itemfunc = NULL;
	gpsmenu[1].itemfunc = gps_time_zone_menu;   //displays gps time zone menu
	gpsmenu[2].itemfunc = gps_format_menu;      //displays gps format menu
	gpsmenu[3].itemfunc = gps_brd_menu;         //displays gps bread crumb menu
	gpsmenu[4].itemfunc = gps_test;             //displays gps test
	gpsmenu[5].itemfunc = gps_tag;              //displays gps tag option

	gpsmenu[0].fnt_picture = NULL;
	gpsmenu[1].fnt_picture = (unsigned char*)_GPSTimeZone;  //gps time zone icon
	gpsmenu[2].fnt_picture = (unsigned char*)_GPSFormat;    //gps format icon
	gpsmenu[3].fnt_picture = (unsigned char*)_breadcrumbs;  //gps bread crumb icon
	gpsmenu[4].fnt_picture = (unsigned char*)_gpstest;      //gps test icon
	gpsmenu[5].fnt_picture = (unsigned char*)_GPSFormat;      //gps test icon

	gpsmenu[0].item = (char*) back_string;
	gpsmenu[1].item = "GPS Time Zone";
	gpsmenu[2].item = "  GPS Format";
	gpsmenu[3].item = " Bread Crumbs";
	gpsmenu[4].item = "   GPS Test";
	gpsmenu[5].item = "   GPS Tag";

	menu_layers[2] = 1;

	for (;;)
	{
		ret = fancymenu( gpsmenu, menu_layers, sizeof( gpsmenu ) / sizeof( fancymenus) );
		if ( ret == -1 )
			return; // return to previous menu
		gpsmenu[ ret ].itemfunc();
	}
}

//------------------------------------------------------------------
//	gps_brd_menu
//  ============================
//	System GPS breadcrumb config menu.
//------------------------------------------------------------------
void gps_brd_menu(void)
{
	menu_layers[3] = 1;

	KAfancyselmenu( (fancyselmenus *)gpsBrdCrmb, menu_layers, "111111", &gps_bread_crumbs, 0);
	if (gps_bread_crumbs != GPS_BRD_OFF)
	{
		if (gps_bread_crumbs != g_BreadCrumbTime)
		{
			g_BreadCrumbTime = gps_bread_crumbs;
			g_GPS_breadcrumbTimer = g_BreadCrumbTime;
		}
		if (g_util_timer == 0)
		{
			g_util_timer = 1;
		}
		g_BreadCrumbFlag = TRUE;
		ResetBreadCrumbTimer();
	}
	else
	{
		// turn off the timer
		g_util_timer = 0;
		g_BreadCrumbFlag = FALSE;
		StopMyTimer(TIMER_ID_GPS_BREADCRUMB);
	}
}

//------------------------------------------------------------------
//	gps_format_menu
//  ============================
//	System GPS format menu.
//------------------------------------------------------------------
void gps_format_menu(void)
{
	menu_layers[3] = 1;

	KAfancyselmenu( (fancyselmenus *)gpsFormats, menu_layers, "1111", &gps_data_format, GPS_RAW);

}

//------------------------------------------------------------------
//	gps_test
//  ============================
//	System GPS test handler.
//------------------------------------------------------------------
void gps_test(void)
{
	char key;

	key = show_message("Do you want to record   the test results?", MB_YESNO | MB_ICONQUESTION);

	if(key == YES)
	{
		g_GPS_Test_Mode = 2;
		main_delete();
	}
	else
	{
		g_GPS_Test_Mode = 1;
	}

	putchar('\f');
	//    set_font_input();
	setfont(SMALL_FONT, NULL);
	gotoxy(0,0);
	printf("\tr   GPS TEST      \tr");

	GPSUpdateTestDisplay();
	// set up for 20 second display update
	StartMyTimer(TIMER_ID_GPS_TEST, GPS_TEST_UPDATE, TRUE);

	//    print_message_bar( MB_ALL );
	gotoxy(0,7);
	printf("\trEXIT\tr");

	resetkey();
	do
	{
		key = waitkey_no_charge();
	}while(!uppressed());

	g_GPS_Test_Mode = 0;
	StopMyTimer(TIMER_ID_GPS_TEST);
	if (g_BreadCrumbFlag == TRUE)
	{
		// reset minute counter
		ResetBreadCrumbTimer();
	}
}

//------------------------------------------------------------------
//	gps_time_zone_menu
//  ============================
//	System GPS time zone config handler.
//------------------------------------------------------------------
void gps_time_zone_menu(void)
{
	int nIndex = time_zone_offset;
	int nResponse = 0;
	char setMessage[50];
	int nPosition;

	g_fSuspendConversion = TRUE;
	g_fSuspendGPS = TRUE;

	menu_layers[3] = 1;


	for (;(nResponse != TRIGGER_KEY) && (nResponse != CLR_KEY);)
	{
		clear_screen();
		setfont( LARGE_FONT, NULL );
		nPosition = (14 - strlen(timeZoneLabel[nIndex + 11])) / 2;
		gotoxy(nPosition, 1);
		printf(timeZoneLabel[nIndex + 11]);
		if(nIndex>0)
		{
			sprintf(setMessage, "UTC+%d", nIndex);
		}
		else
		{
			sprintf(setMessage, "UTC%d", nIndex);
		}
		nPosition = (14 - strlen(setMessage)) / 2;
		gotoxy(nPosition, 2);
		printf(setMessage);
		print_message_bar(SCROLL_BAR);

		nResponse = KAwaitkey( battery_type );

		switch ( nResponse )
		{
		case CLR_KEY:   // return to previous menu
		case TRIGGER_KEY:
		{
			break;
		}

		case UP_KEY:
			nIndex--;
			break;
		case DOWN_KEY:
			nIndex++;
			break;
		default:
			break;
		}   //ENDSWITCH key
		if(nIndex < TIME_ZONE_MINIMUM)
		{
			// wraparound to the left
			nIndex = TIME_ZONE_MAXIMUM;
		}
		else if(nIndex > TIME_ZONE_MAXIMUM)
		{
			// wraparound to the left
			nIndex = TIME_ZONE_MINIMUM;
		}
	}   //ENDFOR loop

	if((nResponse == TRIGGER_KEY) && (nIndex != time_zone_offset))
	{
		set_font_input();
		clear_screen();
		sprintf(setMessage, "Do you want to change to      UTC%d", nIndex);
		PauseMyTimer(TIMER_ID_POWER_DOWN);
		nResponse = show_message(setMessage, MB_YESNO | MB_ICONQUESTION);
		ResumeMyTimer(TIMER_ID_POWER_DOWN);
		if(nResponse == YES)
		{
			time_zone_offset = nIndex;
			time_zone_change = TRUE;
		}
	}

	g_fSuspendConversion = FALSE;
	g_fSuspendGPS = FALSE;

	menu_layers[3] = 0;
}

//------------------------------------------------------------------
//	sys_memory
//  ============================
//	System config/memory menu.
//------------------------------------------------------------------
void sys_memory( void )
{
	int ret;
	fancymenus memorymenu[5];

	memorymenu[0].itemfunc = NULL;
	memorymenu[1].itemfunc = display_mem;   //displays memory status
	memorymenu[2].itemfunc = save_config;      //saves config to flash
	memorymenu[3].itemfunc = send_Z3;      //sends config to serial port
	memorymenu[4].itemfunc = DefaultConfig;      //sends config to serial port

	memorymenu[0].fnt_picture = NULL;
	memorymenu[1].fnt_picture = (unsigned char*)_brain;  //gps time zone icon
	memorymenu[2].fnt_picture = (unsigned char*)_brain;    //gps format icon
	memorymenu[3].fnt_picture = (unsigned char*)_brain;    //gps format icon
	memorymenu[4].fnt_picture = (unsigned char*)_brain;    //gps format icon

	memorymenu[0].item = (char*) back_string;
	memorymenu[1].item = "Show free mem";
	memorymenu[2].item = "Store config";
	memorymenu[3].item = " Send config";
	memorymenu[4].item = " Default cfg";

	menu_layers[2] = 1;

	for (;;)
	{
		ret = fancymenu( memorymenu, menu_layers, sizeof( memorymenu ) / sizeof( fancymenus) );
		if ( ret == -1 )
			return; // return to previous menu
		memorymenu[ ret ].itemfunc();
	}
}

//------------------------------------------------------------------
//	save_config
//  ============================
//	System config/mem save configuration handler.
//------------------------------------------------------------------
void save_config( void )
{
	int nResponse = 0;
	char setMessage[50];

	set_font_input();
	clear_screen();
	sprintf(setMessage, "Do you want to save the configuration?");
	nResponse = show_message(setMessage, MB_YESNO | MB_ICONQUESTION);
	if(nResponse == YES)
	{
		SaveAppSettings();
		LoadMenuSettings(&currentMenu);
	}
}

//------------------------------------------------------------------
//	send_Z3
//  ============================
//	System config/mem send configuration handler.
//------------------------------------------------------------------
void send_Z3(void)
{
	clear_screen();
	set_font_menu();
	printf("\fTransmit Config\n");
	printf("via %s\n",interface_desc);

	print_message_bar( MB_ALL );

	waitkey_no_charge();
	systemsetting("Z3");
}

//------------------------------------------------------------------
//	DefaultConfig
//  ============================
//	System config/mem default conifguration handler.
//------------------------------------------------------------------
void DefaultConfig(void)
{
	int response = 0;

	response = show_message("Do you want to set to   factory defaults?", MB_YESNO | MB_ICONQUESTION);
	if(response == YES)
	{
		Application_Default();
		info_sound();
	}
}


//------------------------------------------------------------------
//	system_menu
//  ============================
//	Main system menu handler.
//------------------------------------------------------------------
void system_menu( void )
{
	int ret;
	fancymenus sysmenu[SYSTEM_MENU_SIZE];

	sysmenu[SYSTEM_MENU_BACK_INDEX].itemfunc = NULL;
	sysmenu[SYSTEM_MENU_BARC_INDEX].itemfunc = sys_barcodes;
	sysmenu[SYSTEM_MENU_COMM_INDEX].itemfunc = comm_menu;
	sysmenu[SYSTEM_MENU_QTY_INDEX].itemfunc = qty_menu;                //display qty sel menu
	sysmenu[SYSTEM_MENU_BLT_INDEX].itemfunc = bluetooth_menu;     //BT Options sys_bluetooth_reset
	sysmenu[SYSTEM_MENU_BATT_INDEX].itemfunc = sys_battery;
	sysmenu[SYSTEM_MENU_MEM_INDEX].itemfunc = sys_memory;
	sysmenu[SYSTEM_MENU_TIME_INDEX].itemfunc = sys_datetime;
	sysmenu[SYSTEM_MENU_STBY_INDEX].itemfunc = sys_standby;
	sysmenu[SYSTEM_MENU_LITE_INDEX].itemfunc = sys_backlight;
	sysmenu[SYSTEM_MENU_GPS_INDEX].itemfunc = gps_menu;

	sysmenu[SYSTEM_MENU_BACK_INDEX].fnt_picture = NULL;
	sysmenu[SYSTEM_MENU_BARC_INDEX].fnt_picture = (unsigned char*)_barcode;
	sysmenu[SYSTEM_MENU_COMM_INDEX].fnt_picture = (unsigned char*)_comp;
	sysmenu[SYSTEM_MENU_QTY_INDEX].fnt_picture = (unsigned char*)_qty;
	sysmenu[SYSTEM_MENU_BLT_INDEX].fnt_picture = (unsigned char*)_b_tooth;
	sysmenu[SYSTEM_MENU_BATT_INDEX].fnt_picture = (unsigned char*)_battsm;
	sysmenu[SYSTEM_MENU_MEM_INDEX].fnt_picture = (unsigned char*)_brain;
	sysmenu[SYSTEM_MENU_TIME_INDEX].fnt_picture = (unsigned char*)_datetime;
	sysmenu[SYSTEM_MENU_STBY_INDEX].fnt_picture = (unsigned char*)_standby;
	sysmenu[SYSTEM_MENU_LITE_INDEX].fnt_picture = (unsigned char*)_light;
	sysmenu[SYSTEM_MENU_GPS_INDEX].fnt_picture = (unsigned char*)_globe_am;

	sysmenu[SYSTEM_MENU_BACK_INDEX].item = (char*) back_string;
	sysmenu[SYSTEM_MENU_BARC_INDEX].item = "   Barcodes";
	sysmenu[SYSTEM_MENU_COMM_INDEX].item = " Communicate";
	sysmenu[SYSTEM_MENU_QTY_INDEX].item = "  QTY Options";
	sysmenu[SYSTEM_MENU_BLT_INDEX].item = "  Bluetooth";
	sysmenu[SYSTEM_MENU_BATT_INDEX].item = " Battery volt";
	sysmenu[SYSTEM_MENU_MEM_INDEX].item = "  Config/Mem";
	sysmenu[SYSTEM_MENU_TIME_INDEX].item = " Date / time";
	sysmenu[SYSTEM_MENU_STBY_INDEX].item = " Standby time";
	sysmenu[SYSTEM_MENU_LITE_INDEX].item = "   Backlight";
	sysmenu[SYSTEM_MENU_GPS_INDEX].item = "     GPS";

	// save the current menu settings
	LoadMenuSettings(&currentMenu);

	menu_layers[1] = 1;

	resetkey();

	for (;;)
	{
		ret = KAfancymenu( sysmenu, menu_layers,scanner.Sys_menu_options ) ;  //,sizeof(sysmenu ) / sizeof( fancymenus)
		if ( ret == -1 )
		{
			// compare menu settings
			CompareMenuSettings(&currentMenu);
			return;                 // return to previous menu
		}
		sysmenu[ ret ].itemfunc();
	}
}

//------------------------------------------------------------------
//	getMACAddress
//  ============================
//	System bluetooth get MAC address handler.
//------------------------------------------------------------------
int getMACAddress(int MAC_MODE)
{
	int MAC_OK = 0;
	int discovery_result;
	char strBTAddress[MAX_ADDRESS+1];
	int  code_id = 0;

	switch (MAC_MODE)
	{
	//*****************************************************************************

	case   DISCOVERY_MAC:

		discovery_result = Discovery(strBTAddress);

		if (discovery_result < 0)      //0 - no devices, -3 - user aborted, <-10 - discovery failed
		{
			return(-1);
		}

		MAC_OK = 1;
		break;
		//********************************************************************************

	case   SCAN_MAC:

		setfont( MEDIUM_FONT, 0 );
		cursor(NOWRAP);
		gotoxy(0,0);
		printf("\f\tr  SCAN BT ADDR.  \tr");
		gotoxy(0,4);
		printf("\trCANCEL             \tr");
		gotoxy(0,1);
		printf("Scan Barcode of\nDevice BT Addr:");
		scannerpower(TRIGGER, 1000);

		if ( ScanBarcode(strBTAddress, 1,MAX_ADDRESS+1,&code_id  ) != OK )
		{
			return(-2);
		}
		okbeep();
		if (verifyBTAddress(strBTAddress) < 0)
		{
			errbeep();
			scannerpower(OFF,0);

			return(-4);
		}

		MAC_OK = 1;

		scannerpower(OFF,0);
		break;
	case KEY_MAC:
		setfont( MEDIUM_FONT, 0 );
		cursor(NOWRAP);
		gotoxy(0,0);
		printf("\f\tr  KEY IN BT ADDR.  \tr");
		gotoxy(0,4);
		printf("\trCANCEL             \tr");
		gotoxy(0,1);
		break;
	}//switch


	if (MAC_OK < 0)
		return(-5);


	strcpy(temp_mac,strBTAddress);       //save BT address into the temp location for later checking
	return(0);


}

//------------------------------------------------------------------
//	Discovery
//  ============================
//  BT_Discovery, does the discovery of other bluetooth devices, and allows
// 	the selection of discovered devices.
//------------------------------------------------------------------
int Discovery(char *address)            //
{
	int    x,ret;
	static device_info   blt_dev_info [15]; //dev_address[20];
	int key;



	printf("\f\tr   BT Discovery    \tr\n" );
	gotoxy( 0, 4 );
	printf(  "\tr      CANCEL       \tr");

	gotoxy(0,1);
	printf(" Searching....    ");


	if ((ret = blt_discovery( blt_dev_info, TRIGGER_KEY)) <= 0)
	{
		return(ret);
	}

	x=0;
	while (1)
	{
		printf("\f\trSelect address %i/%i\tr\n",x+1,ret);
		gotoxy( 0, 4 );
		printf(  "\tr1=up Tr=sel 2=down \tr");
		gotoxy(0,1);
		printf("%s        \n",blt_dev_info[x].bd_address);
		printf("%s        \n",blt_dev_info[x].name);

		key = KAwaitkey( battery_type );
		switch (key)
		{
		case UP_KEY:
			x++;
			if (x==ret)
				x=0;
			break;
		case DOWN_KEY:
			if (x==0)
				x=ret-1;
			else
				x--;
			break;
		case TRIGGER_KEY:
			strcpy(address,blt_dev_info[x].bd_address);
			return(0);
		}
	}
	return(0);
}

//------------------------------------------------------------------
//	verifyBTAddress
//  ============================
//	Verify bluetooth address.
//------------------------------------------------------------------
int verifyBTAddress(char * BTAddress)
{
	int result =0;
	int i=0;

	if (strlen(BTAddress) != 12)
		return -1;

	for (i=0; BTAddress[i] != '\0'; i++)
	{
		if (BTAddress[i] >= '0' && BTAddress[i] <= '9')
			continue;
		else if (BTAddress[i] >= 'A' && BTAddress[i] <= 'F')
			continue;
		else if (BTAddress[i] >= 'a' && BTAddress[i] <= 'f')
			continue;
		else
		{
			result = -1;
			break;
		}
	}

	return(result);
}

//------------------------------------------------------------------
//	BT_address_setup
//  ============================
//	Setup bluetooth MAC address handler.
//------------------------------------------------------------------
int BT_address_setup(void)
{
	int key;
	//    int x;
	int result = 0;
	char pin[MAX_PINCODE+1];


	//	controlScreen(TRUE);

	setfont( MEDIUM_FONT, 0 );
	cursor(NOWRAP);
	printf(" MASTER/SLAVE ");

	printf("\f\trSet Bluetooth Addr \tr");
	gotoxy(0,4);
	printf(  "\tr                   \tr");
	gotoxy(0,1);
	printf("DOWN = Scan Label\n");
	printf("UP = Discovery\n");
	printf("Tr = Cancel\n");



	resetkey();
	key = KAwaitkey( battery_type );
	switch (key)
	{
	case TRIGGER_KEY:
		result = -6;
		break;
	case UP_KEY:
		result = getMACAddress(DISCOVERY_MAC);     //param = address
		break;
	case DOWN_KEY:
		result = getMACAddress(SCAN_MAC);
		break;
	}

	if (result < 0)         //bt address unsuccessful
		return(result);

	clear_screen();
	gotoxy(0,1);
	printf("Accept BT Address?\n\n");
	printf("   %s",temp_mac);

	print_message_bar(MB_YESNO);
	resetkey();
	key = KAwaitkey( battery_type );

	if (key == DOWN_KEY)
		return(-7);

	strcpy(scanner.BT_Address, temp_mac);      //save BT address
	SetBltAddress(scanner.BT_Address);

	//ask if user wants to setup a pin code
	putchar('\f');
	printf ("\tr  PIN CODE SETUP  \tr\n\n");
	printf("  Enter PIN Code?\n" );
	print_message_bar( MB_YESNO );
	resetkey();

	if ( KAwaitkey( battery_type ) == UP_KEY )     //Q1=YES
	{
		memset(pin,'\0',sizeof(pin) );           //erase previous PIN if any
		strcpy(scanner.BT_PIN,pin);
		SetBltPin( pin );
		return(1);
	}

	memset(pin,'\0',sizeof(pin) );           //erase previous PIN if any
	InputPincode(pin);                       //get PIN from user
	clear_screen();
	printf("Accept PIN Code?\n\n");
	printf("   %s",pin);

	print_message_bar(MB_YESNO);
	resetkey();
	key = KAwaitkey( battery_type );

	if (key == DOWN_KEY)     //user rejected the PIN
		return(2);

	strcpy(scanner.BT_PIN,pin);
	SetBltPin( pin );

	return(0);
}

//------------------------------------------------------------------
//	ConnectBluetooth
//  ============================
//	System bluetooth connection handler.
//------------------------------------------------------------------
void ConnectBluetooth(void)
{
	int result;
	int port;
	int fDone;

	set_font_input();

	port = get_active_port();
	com_opened = port;

	SetBltLocalName(BTLocalName);

	fDone = FALSE;

	// suspend GPS while connecting
	g_fSuspendGPS = TRUE;

	do
	{
		putchar('\f');
		gotoxy(0,0);
		printf("\tr CONNECT BT \tr");
		gotoxy(0,1);
		printf("MAC:%s", GetBltAddress());
		gotoxy(0,2);
		printf("PIN: %s", GetBltPin());
		gotoxy(0,3);
		printf("Port: %d", port);
		gotoxy(0,4);
		printf("%s", GetBltLocalName());

		KAwaitkey( battery_type );

		BluetoothDefault();

		do
		{
			result = init_connection();
			if(result != TRUE)
			{
				if(result == BT_ABORT_OPERATION)
				{
					fDone = TRUE;
				}
			}
			else
			{
				fDone = TRUE;
			}
		} while(fDone == FALSE);


	} while(fDone == FALSE);

	// resume GPS
	g_fSuspendGPS = FALSE;
}

//------------------------------------------------------------------
//	DisconnectBluetooth
//  ============================
//	Systme bluetooth disconnect handler.
//------------------------------------------------------------------
void DisconnectBluetooth(void)
{
	int fResult = show_message("Are you sureyou want to disconnect?", MB_YESNO | MB_ICONWARNING );

	if(fResult == YES)
	{
		BTStateChange = STATE_TO_DISCONNECT;
		errbeep();
		// clear the suffix
		systemsetting("RZ");
	}
}

//------------------------------------------------------------------
//	main_version
//  ============================
//	Main version display handler.
//------------------------------------------------------------------
void main_version( void )
{
	char strVersion[10];

	set_font_input();

	putchar('\f');
	gotoxy(0,0);
	printf("\tr SOFTWARE VERSION  \tr");

	gotoxy(5,2);
	printf("%-8.8s", app_version );
	rectangle3d( (5*6)-2, ((5-1)*6)-1, ((5+8)*6), ((5+1)*6)-1, 1);
	print_message_bar( MB_ALL );

	setfont(SMALL_FONT,NULL);
	gotoxy(3,5);
	printf("%s",scanner.ModelName);
	gotoxy(3,6);
	printf("(%s)", g_termID);

	KAwaitkey( battery_type );

	set_font_input();

	putchar('\f');
	gotoxy(0,0);
	printf("\tr    OS VERSION     \tr");

	version_os(strVersion);
	gotoxy(5,2);
	printf("%-8.8s", strVersion );
	rectangle3d( (5*6)-2, ((5-1)*6)-1, ((5+8)*6), ((5+1)*6)-1, 1);
	print_message_bar( MB_ALL );

	setfont(SMALL_FONT,NULL);
	gotoxy(3,5);
	printf("%s",scanner.ModelName);
	gotoxy(3,6);
	printf("(%s)", g_termID);

	KAwaitkey( battery_type );

}

//------------------------------------------------------------------
//	send_barcode_real_time
//  ============================
//	Sends a barcode string over the active comm port using putcom().
//------------------------------------------------------------------
int send_barcode_real_time( char* barcode )
{
	char* ptr;

	ptr = barcode;
	while ( *ptr  )
	{
		if ( putcom( *ptr ) == ERROR )
			return ERROR;
		ptr++;
	}
	return OK;
}

//------------------------------------------------------------------
//	put_barcode_real_time
//  ============================
//	Sends a barcode string over the active comm port using PutBuffer().
//------------------------------------------------------------------
int put_barcode_real_time( char* barcode )
{
	PutBuffer((const unsigned char *)barcode, strlen(barcode));
	return OK;
}

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//	Unused code left for reference purposes.
/*
//-----------------------------------------------------------------------------//
void get_screen_size(int* rows, int* cols)
//-----------------------------------------------------------------------------//
{
    unsigned int height, width;
    getfontsize(&height, &width);
 *rows=SCREEN_HEIGHT/height;
 *cols=SCREEN_WIDTH/width;
}

 */

/*
//-------------------------------------------------------------------------------------------//
void display_center(int y,char* strFrmt, int inverted  )
//-------------------------------------------------------------------------------------------//
{
    int rows,cols;
    unsigned width, height;
    int start_x;

    if(strlen(strFrmt)==0)
        return;

    get_screen_size(&rows, &cols);
    getfontsize(&height,&width);


   start_x = ( strlen(strFrmt) > cols )?0:(cols-strlen(strFrmt))/2;
    gotoxy(start_x,y);

    if( inverted == TRUE)
    {
        printf("\tr%-*.*s\tr",cols-start_x,cols-start_x,strFrmt);
    }
    else
        printf("%-*.*s",cols-start_x,cols-start_x,strFrmt);
}

 */

//------------------------------------------------------------------
//	check_connection
//  ============================
//	Check if bluetooth is connected.
//------------------------------------------------------------------
int check_connection( void )
{
	int result;

	//
	// Check if the bluetooth device is still connected
	//
	for (;;)
	{
		result = blt_is_connected();
		switch ( result )
		{
		case NG:
			//
			// Connection can not be determined at
			// the moment we have to wait a little while.
			//
			delay(5);
			break;
		default:
			return result;

		}
	}
	return(0);
}


//------------------------------------------------------------------
//	init_connection
//  ============================
//	Initiates bluetooth connection.
//------------------------------------------------------------------
int init_connection()
{
	int ret_val;

	SetBltLocalName(BTLocalName);

	if (check_valid_BT_address() < 0)    //check if a BT is already in flash
		return(-1);

	if (scanner.BT_Master_Slave < 0)
		get_master_slave_mode();

	g_bltPINCallback = BLT_PIN_CALLBACK_INACTIVE;

	if ( scanner.BT_Master_Slave == BT_MASTER )
	{
		display_master_connection_screen();
		ret_val = init_master_connection();

	} else
	{
		display_slave_connection_screen();
		ret_val = init_slave_connection();
	}

	if ( ret_val == TRUE )
	{
		sound(TVLONG, VHIGH, SMEDIUM, NULL);
	}
	else
	{
		sound(TVLONG, VLOW);
	}

	return(ret_val);

}

//------------------------------------------------------------------
//	close_connection
//  ============================
//	Closes bluetooth connection.
//------------------------------------------------------------------
int close_connection(void)
{
	comclose((unsigned int)get_active_port());
	//	BTStateChange = STATE_TO_DISCONNECT;

	return OK;
}

//------------------------------------------------------------------
//	init_master_connection
//  ============================
//	Initiates bluetooth master connection.
//------------------------------------------------------------------
int init_master_connection()
{
	int key;

	// signal the state machine to connect as master
	BTStateChange = STATE_TO_CONNECT_AS_MASTER;

	while ((key=getchar()) != UP_KEY)     //Q1 = EXIT
	{
		if(g_bltPINCallback == BLT_PIN_CALLBACK_INACTIVE)
		{
			UpdateBluetoothState(&BTStateChange);
			if(BTState == STATE_CONNECTED_MASTER)
			{
				return(TRUE);
			}
			if(kbhit())
			{
				key = getchar();
			}
			else
			{
				key = EOF;
			}
		}
		else if(g_bltPINCallback == BLT_PIN_CALLBACK_DONE)
		{
			display_master_connection_screen();
			g_bltPINCallback = BLT_PIN_CALLBACK_INACTIVE;
		}
		idle();
	}

	// close the connection when aborting
	close_connection();

	return(BT_ABORT_OPERATION);
}


//------------------------------------------------------------------
//	init_slave_connection
//  ============================
//	Initiates bluetooth slave connection.
//------------------------------------------------------------------
int init_slave_connection()
{
	int key;

	resetkey();

	// signal state machine to make discoverable
	BTStateChange = STATE_TO_DISCOVERABLE;

	key = EOF;
	while (key != UP_KEY)
	{

		if(g_bltPINCallback == BLT_PIN_CALLBACK_INACTIVE)
		{
			UpdateBluetoothState(&BTStateChange);
			if(BTState == STATE_CONNECTED_SLAVE)
			{
				return(TRUE);
			}
			if(kbhit())
			{
				key = getchar();
			}
			else
			{
				key = EOF;
			}
		}
		else if(g_bltPINCallback == BLT_PIN_CALLBACK_DONE)
		{
			display_slave_connection_screen();
			g_bltPINCallback = BLT_PIN_CALLBACK_INACTIVE;
		}
		idle();
	}



	// close the connection when aborting
	BTStateChange = STATE_TO_DISCONNECT;

	return(BT_ABORT_OPERATION);
}


//------------------------------------------------------------------
//	get_active_port
//  ============================
//	Gets the active comm port number.
//------------------------------------------------------------------
long get_active_port(void)
{
	long port = -1;

	switch (interface_enabled)
	{
	case  INTERFACE_CRADLE:
		port = COM2;
		interface_desc = "Cradle";
		break;

	case  INTERFACE_BT_BATCH:
	case  INTERFACE_BT_REAL_TIME:
		if(app.blt_interface == BLT_VCP_DEVICE)
		{
			// use VCP port options
			if (scanner.BT_Master_Slave == BT_MASTER)
			{
				port = MASTER_COM;
				interface_desc = "BT VCP Master";
				break;
			}
			else if (scanner.BT_Master_Slave == BT_SLAVE)
			{
				port = SLAVE_COM;
				interface_desc = "BT VCP Slave";
				break;
			}
			else
				// check_valid_BT_address();    //check if all BT info is available
			{
				port = -1;
				break;
			}
		}
		else
		{
			if(scanner.BT_Master_Slave == BT_MASTER)
			{
				// use HID options
				port = HID_MASTER_COM;
				interface_desc = "BT HID Master";
			}
			else
			{
				// use HID options
				port = HID_SLAVE_COM;
				interface_desc = "BT HID Slave";
			}
			break;
		}
	case  INTERFACE_USB:
		port = COM0;
		interface_desc = "USB Port";
		break;

	}

	return(port);  //	port
}

//------------------------------------------------------------------
//	display_master_connection_screen
//  ============================
//	Displays master bluetooth conenction screen.
//------------------------------------------------------------------
void display_master_connection_screen(void)
{
	clear_screen();
	setfont (LARGE_FONT, NULL);
	printf("Connecting To: \n");
	printf(" %s",scanner.BT_Address);

	gotoxy(0,3);
	printf("\trEXIT\tr");
}

//------------------------------------------------------------------
//	display_slave_connection_screen
//  ============================
//	Displays bluetooth slave connection screen.
//------------------------------------------------------------------
void display_slave_connection_screen(void)
{
	clear_screen();
	setfont (LARGE_FONT, NULL);
	printf(" Waiting Host \n Connection ..\n");
	//    printf(" %s",scanner.BT_Address);

	gotoxy(0,3);
	printf("\trEXIT\tr");
}

//------------------------------------------------------------------
//	InputPincode
//  ============================
//	System bluetooth PIN code input handler.
//------------------------------------------------------------------
void InputPincode(char *pincode)
{
	int c,x,i,key;

	setfont( MEDIUM_FONT, 0 );
	cursor(NOWRAP);

	printf("\f\tr  Set Pincode      \tr");
	gotoxy(0,4);
	printf("\tr1=up Tr=done 2=move\tr");
	gotoxy(0,1);

	for (x=0;x<MAX_PINCODE;x++)
	{
		if (((pincode[x]>'9') || (pincode[x]<'0')))
		{
			memset(pincode+x,'.',MAX_PINCODE-x);
			break;
		}
	}

	x=0;
	for (;;)
	{
		gotoxy(0,1);

		printf("Pincode =                  \n");

		for (i=0;i<MAX_PINCODE;i++)
		{
			if (i == x)
				printf("\tr%c\tr",pincode[i]);
			else
				printf("%c",pincode[i]);
		}
		printf("\n");

		key = KAwaitkey( battery_type );
		switch (c=key)
		{
		case UP_KEY:
			if (pincode[x] == '.')
				pincode[x]='0';
			else if (pincode[x] == '9')
				pincode[x]='.';
			else
				pincode[x]++;
			break;
		case DOWN_KEY:
			x++;
			if (x==MAX_PINCODE)
				x=0;
			break;
		case 48:      //0
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case 54:
		case 55:
		case 56:
		case 57:      //9


		pincode[x]= c;
		break;
		case TRIGGER_KEY:
			for (x=0;x<MAX_PINCODE;x++)
				if (pincode[x] == '.')
					pincode[x]='\0';

			//				controlScreen(FALSE);
			return;


		}
	}
}

//------------------------------------------------------------------
//	InputMACAddress
//  ============================
//	System bluettoh MAC addres input handler.
//------------------------------------------------------------------
void InputMACAddress(char *address)
{
	int c,x,i,key;


	setfont( MEDIUM_FONT, 0 );
	cursor(NOWRAP);

	printf("\f\tr  Input MAC Address  \tr");
	gotoxy(0,4);
	printf("\tr1=up Tr=done 2=move\tr");
	gotoxy(0,1);

	for (x=0;x<MAX_ADDRESS;x++)
	{
		if (((address[x]>'9') || (address[x]<'0')))
		{
			if((address[x]>'F') || (address[x]<'A'))
			{
				address[x] = '.';
			}
		}
	}

	x=0;
	for (;;)
	{
		gotoxy(0,1);

		printf("MAC =                  \n");

		for (i=0;i<MAX_ADDRESS;i++)
		{
			if (i == x)
				printf("\tr%c\tr",address[i]);
			else
				printf("%c",address[i]);
		}
		printf("\n");

		key = KAwaitkey( battery_type );
		switch (c=key)
		{
		case UP_KEY:
			if (address[x] == '.')
			{
				address[x]='0';
			}
			else if (address[x] == '9')
			{
				address[x]='A';
			}
			else if (address[x] == 'F')
			{
				address[x]='0';
			}
			else
			{
				address[x]++;
			}
			break;
		case DOWN_KEY:
			x++;
			if (x==MAX_ADDRESS)
				x=0;
			break;
		case 48:      //0
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case 54:
		case 55:
		case 56:
		case 57:      //9


		address[x]= c;
		break;
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			address[x] = toupper(c);
			break;
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			address[x] = c;
			break;
		case TRIGGER_KEY:
			for (x=0;x<MAX_ADDRESS;x++)
				if (address[x] == '.')
					address[x]='\0';

			return;


		}
	}
}

//------------------------------------------------------------------
//	check_flash_BT_address
//  ============================
//	Checks bluetooth MAC address as read from OS.
//------------------------------------------------------------------
int check_flash_BT_address(void)
{
	char  BT_Address[MAX_ADDRESS+1];


	memset(BT_Address,'\0',sizeof(BT_Address));
	sprintf(BT_Address ,"%s",GetBltAddress());

	if (strlen( BT_Address ) == MAX_ADDRESS )
	{
		strcpy(scanner.BT_Address, BT_Address);
		return(0);
	}

	return(-1);
}

//------------------------------------------------------------------
//	check_flash_PIN_code
//  ============================
//	Checks bluetooth PIN code as read from OS.
//------------------------------------------------------------------
int check_flash_PIN_code(void)
{
	char  pin [MAX_PINCODE+1];

	memset(pin,'\0', sizeof(pin) );
	sprintf(pin ,"%s",GetBltPin() );

	if (strlen( pin ) > 0 )
	{
		strcpy(scanner.BT_PIN, pin);
		return(0);
	}

	return(-1);
}

//------------------------------------------------------------------
//	get_master_slave_mode
//  ============================
//	System bluetooth master/slave mode selection handler.
//------------------------------------------------------------------
void get_master_slave_mode(void)
{
	int key;

	setfont( LARGE_FONT, NULL );
	putchar('\f');
	gotoxy(0,0);
	printf("\tr MASTER/SLAVE \tr");
	gotoxy(0,2);
	printf("DOWN = MASTER");
	gotoxy(0,3);
	printf("UP = SLAVE");
	resetkey();
	key = KAwaitkey( battery_type );

	if (key == DOWN_KEY)
	{
		scanner.BT_Master_Slave = BT_MASTER;
		scanner.COM_Port = MASTER_COM;
	} else if (key == UP_KEY)
	{
		scanner.BT_Master_Slave = BT_SLAVE;
		scanner.COM_Port = SLAVE_COM;
	}
}

//------------------------------------------------------------------
//	get_BT_interface
//  ============================
//	System bluetooth data mode selection handler.
//------------------------------------------------------------------
void get_BT_interface(void)
{
	int ret;

	setfont( LARGE_FONT, NULL );
	putchar('\f');
	gotoxy(0,0);
	printf("  Data Mode:");
	gotoxy(0,2);
	printf ("DOWN= Batch Mode");
	gotoxy(0,3);
	printf ("UP= Real Time");

	resetkey();
	ret = KAwaitkey( battery_type );
	if (ret  == DOWN_KEY)
	{
		scanner.BT_Master_Slave = BT_MASTER;
		scanner.BT_Interface = INTERFACE_BT_BATCH;
		interface_enabled =INTERFACE_BT_BATCH;
		protocol_enabled = ID_NETO_PROTOCOL;
	} else if (ret  == UP_KEY)
	{
		scanner.BT_Interface = INTERFACE_BT_REAL_TIME;
		interface_enabled = INTERFACE_BT_REAL_TIME;
	}
}

//------------------------------------------------------------------
//	check_valid_BT_address
//  ============================
//	Checks bluetooth MAC address against OS setting.
//------------------------------------------------------------------
int check_valid_BT_address(void)
{
	int ret;
	int key;

	// don't need address for slave connection
	if(scanner.BT_Master_Slave == BT_SLAVE)
	{
		return(TRUE);
	}
	setfont(LARGE_FONT,NULL);
	//get Master Slave mode


	while ( check_flash_BT_address () < 0 )  //scanner does not have a MAC address assigned yet
	{

		putchar ('\f');
		setfont(LARGE_FONT,NULL);
		gotoxy(0,0);
		printf ("No BT Address\nis found.\n");
		printf("Setup One Now?");
		print_message_bar(MB_YESNO);
		resetkey();

		key = KAwaitkey( battery_type );
		if ( key == UP_KEY)    //NO
			return(-1);
		if (key != DOWN_KEY)    //any other key except Q1 key stay in loop
			continue;

		while (scanner.BT_Master_Slave < 0)
			get_master_slave_mode();             //determines Master/Slave mode and sets the approprirte COM port

		ret =  BT_address_setup();                    //get a BT address

		if (ret < 0 )        //
		{
			display_BT_error(ret);
			scanner.BT_Master_Slave = -1;              //reset value
			continue;
		}

	}  //while

	return(0);
}

//------------------------------------------------------------------
//	bat_test
//  ============================
//	Battery voltage test.
//------------------------------------------------------------------
void bat_test( void )
{
#define MAXBATTVOLT  300L
#define MAX_AD       1023L
	unsigned int v1,v2,v3,v4,v5;

	unsigned int voltage;
	unsigned int output;

	cursor( NOWRAP );
	delay(10);     //delay 200 mil sec	(10 * 20msec)

	v1= (long) batteryvoltage();
	v2= (long) batteryvoltage();
	v3= (long) batteryvoltage();
	v4= (long) batteryvoltage();
	v5= (long) batteryvoltage();

	output = (v1+v2+v3+v4+v5)/5;           //take avarage reading
	voltage = ((long) output * MAXBATTVOLT ) / MAX_AD;
	printf("\fBat vol: %u\n", output);
	gotoxy(0,1);
	printf("%1u.%02u V", (voltage/100), (voltage-100*(voltage/100)));
	sound( TCLICK, VMEDIUM, SHIGH, 0);
	goodreadled( GREEN, TCLICK);
	delay(5*50);
}

//------------------------------------------------------------------
//	direct_character_input
//  ============================
//	Translates direct input menu codes to integer value.
//------------------------------------------------------------------
char direct_character_input(unsigned char byte1, unsigned char byte2)
{
	unsigned char ret_char;

	ret_char = 0xFF;

	switch (byte1)
	{
	case '0':
		if ((byte2 >= 'A') && (byte2 <= 'Z'))
			ret_char = byte2;
		break;
	case '1':
		if ((byte2 >= 'A') && (byte2 <= 'Z'))
			ret_char = byte2 - 'A' + 0x01;
		break;
	case '5':
		if ((byte2 >= 'A') && (byte2 <= 'P'))
			ret_char = byte2 - 'A' + 0x20;
		break;
	case '6':
		if ((byte2 >= 'A') && (byte2 <= 'G'))
			ret_char = byte2 - 'A' + 0x3A;
		break;
	case '7':
		switch (byte2)
		{
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
			ret_char = byte2 - 'A' + 0x5B;
			break;
		}
		break;
		case '9':
			switch (byte2)
			{
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
				ret_char = byte2 - 'A' + 0x1B;
				break;
			case 'F':
				ret_char = 0x7F;
				break;
				//				case 'G':										 0x00 is the string terminator, and therfore it cannot be used.
				//					ret_char = 0x00;
				//					break;
			case 'T':
			case 'U':
			case 'V':
			case 'W':
				ret_char = byte2 - 'T' + 0x7B;
				break;
			}
			break;
			case 'Q':
				if ((byte2 >= '0') && (byte2 <= '9'))
					ret_char = byte2;
				break;
			case '$':
				if ((byte2 >= 'A') && (byte2 <= 'Z'))
					ret_char = byte2 - 'A' + 'a';
				else if (byte2 == '2')
					ret_char = 0xAF;
				else if (byte2 == '3')
					ret_char = 0xAE;
				break;
			default:
				break;
	}

	return ret_char;
}

//------------------------------------------------------------------
//	SetDefault
//  ============================
//	Set default symbology prefix and suffix.
//------------------------------------------------------------------
void SetDefault(void)
{
	int x;

	prefix = -1;
	suffix = -1;

	for (x=0; x < MAX_SYMBOLOGIES; x++)
	{
		memset(param_tab[x].prefix, '\0', MAX_PREFIX_LENGTH);
		memset(param_tab[x].suffix, '\0', MAX_SUFFIX_LENGTH);
		strcpy(param_tab[x].suffix, DEFAULT_SUFFIX);
	}
}

//------------------------------------------------------------------
//	process_menu_code
//  ============================
//	Process menu code label.
//------------------------------------------------------------------
void process_menu_code(char *barcode)
{

	int error;
	unsigned int command;
	int symbology = 0;
	int ch;

	if (menu_state == NOT_IN_MENU_MODE)         // Menu label while not in menu mode is considered an error
	{
		goodreadled(RED, 10);
		sound(TLONG, VHIGH, SERROR, NULL);
		return;
	}
	else if (menu_state == EXITING_MENU_MODE)
	{
		goodreadled(GREEN, 10);
		sound(TSTANDARD, VHIGH, SLOW, SMEDIUM, NULL);
		menu_state = NOT_IN_MENU_MODE;
		return;
	}

	if (strlen(barcode) == 3 || strlen(barcode) == 4)
	{
		barcode[5] = '\0';
		barcode[4] = barcode[3];
		barcode[3] = barcode[2];
		barcode[2] = barcode[1];
		barcode[1] = barcode[0];
		barcode[0] = (strlen(barcode+1) == 3) ? '[' : ']';
	}

	error = 0;
	if (systemsetting(barcode) == OK)
	{
		if ((barcode[0] == 'U') && (barcode[1] == '2'))
			SetDefault();

		systemsetting("YC");    // enable menu label setting
		if ((barcode[0] == 'Z') && (barcode[1] == '2'))
			SaveAppSettings();
		command = 0;
	} else switch ((command = (((unsigned int)barcode[0]) << 8) + barcode[1]))
	{
	case ('R' << 8) + 'Y':      // set prefix all codes
            		for (symbology = 0; symbology < MAX_SYMBOLOGIES; symbology++)
            			memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	symbology = MAX_SYMBOLOGIES;
	prefix = 0;
	break;

	case ('N' << 8) + '1':      // set prefix UPC-A
            		symbology = SYMBOLOGY_UPCA;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('M' << 8) + '0':      // set prefix UPC-A + addon
            		symbology = SYMBOLOGY_UPCA_2;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	memset(param_tab[SYMBOLOGY_UPCA_5].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('N' << 8) + '2':      // set prefix UPC-E
            		symbology = SYMBOLOGY_UPCE;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('M' << 8) + '1':      // set prefix UPC-E + addon
            		symbology = SYMBOLOGY_UPCE_2;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	memset(param_tab[SYMBOLOGY_UPCE_5].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('N' << 8) + '3':      // set prefix EAN_13
            		symbology = SYMBOLOGY_EAN13;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('M' << 8) + '2':      // set prefix EAN-13 + addon
            		symbology = SYMBOLOGY_EAN13_2;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	memset(param_tab[SYMBOLOGY_EAN13_5].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('N' << 8) + '4':      // set prefix EAN-8
            		symbology = SYMBOLOGY_EAN8;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('M' << 8) + '3':      // set prefix EAN-8 + addon
            		symbology = SYMBOLOGY_EAN8_2;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	memset(param_tab[SYMBOLOGY_EAN8_5].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('M' << 8) + '4':      // set prefix code 39
            		symbology = SYMBOLOGY_CODE39;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('M' << 8) + '5':      // set prefix codabar
            		symbology = SYMBOLOGY_CODABAR;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('M' << 8) + '6':      // set prefix industrial 2 of 5
            		symbology = SYMBOLOGY_D2OF5;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('M' << 8) + '7':      // set prefix interleaved 2 of 5
            		symbology = SYMBOLOGY_I2OF5;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('M' << 8) + 'B':      // set prefix S code
            		symbology = SYMBOLOGY_SCODE;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('G' << 8) + 'L':      // set prefix Matrix 2of5 code
            		symbology = SYMBOLOGY_M2OF5;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('I' << 8) + '8':      // set prefix IATA code
            		symbology = SYMBOLOGY_IATA;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('M' << 8) + '8':      // set prefix Code 93 code
            		symbology = SYMBOLOGY_CODE93;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('M' << 8) + '9':      // set prefix Code 128 code
            		symbology = SYMBOLOGY_CODE128;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('N' << 8) + '0':      // set prefix MSI/Plessey code
            		symbology = SYMBOLOGY_CODE128;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('L' << 8) + '8':      // set prefix telepen code
            		symbology = SYMBOLOGY_TELEPEN;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('M' << 8) + 'A':      // set prefix UK/Plessey code
            		symbology = SYMBOLOGY_UK_PLESSEY;
	memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = 0;
	break;

	case ('M' << 8) + 'G':      // clear all prefixes
            		for (symbology = 0; symbology < MAX_SYMBOLOGIES; symbology++)
            			memset(param_tab[symbology].prefix, '\0', MAX_PREFIX_LENGTH);
	prefix = -1;
	break;

	case ('R' << 8) + 'Z':      // set suffix all codes
            		for (symbology = 0; symbology < MAX_SYMBOLOGIES; symbology++)
            			memset(param_tab[symbology].suffix, '\0', MAX_PREFIX_LENGTH);
	symbology = MAX_SYMBOLOGIES;
	suffix = 0;
	break;

	case ('N' << 8) + '6':      // set suffix UPC-A
            		symbology = SYMBOLOGY_UPCA;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('O' << 8) + '0':      // set suffix UPC-A + addon
            		symbology = SYMBOLOGY_UPCA_2;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	memset(param_tab[SYMBOLOGY_UPCA_5].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('N' << 8) + '7':      // set suffix UPC-E
            		symbology = SYMBOLOGY_UPCE;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('O' << 8) + '1':      // set suffix UPC-E + addon
            		symbology = SYMBOLOGY_UPCE_2;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	memset(param_tab[SYMBOLOGY_UPCE_5].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('N' << 8) + '8':      // set suffix EAN_13
            		symbology = SYMBOLOGY_EAN13;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('O' << 8) + '2':      // set suffix EAN-13 + addon
            		symbology = SYMBOLOGY_EAN13_2;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	memset(param_tab[SYMBOLOGY_EAN13_5].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('N' << 8) + '9':      // set suffix EAN-8
            		symbology = SYMBOLOGY_EAN8;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('O' << 8) + '3':      // set suffix EAN-8 + addon
            		symbology = SYMBOLOGY_EAN8_2;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	memset(param_tab[SYMBOLOGY_EAN8_5].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('O' << 8) + '4':      // set suffix code 39
            		symbology = SYMBOLOGY_CODE39;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('O' << 8) + '5':      // set suffix codabar
            		symbology = SYMBOLOGY_CODABAR;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('O' << 8) + '6':      // set suffix industrial 2 of 5
            		symbology = SYMBOLOGY_D2OF5;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('O' << 8) + '7':      // set suffix interleaved 2 of 5
            		symbology = SYMBOLOGY_I2OF5;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('O' << 8) + 'B':      // set suffix S code
            		symbology = SYMBOLOGY_SCODE;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('G' << 8) + 'M':      // set suffix Matrix 2of8 code
            		symbology = SYMBOLOGY_M2OF5;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('I' << 8) + '9':      // set suffix IATA code
            		symbology = SYMBOLOGY_IATA;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('O' << 8) + '8':      // set suffix Code 93 code
            		symbology = SYMBOLOGY_CODE93;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('O' << 8) + '9':      // set suffix Code 128 code
            		symbology = SYMBOLOGY_CODE128;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('N' << 8) + '5':      // set suffix MSI/Plessey code
            		symbology = SYMBOLOGY_CODE128;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('L' << 8) + '9':      // set suffix telepen code
            		symbology = SYMBOLOGY_TELEPEN;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('O' << 8) + 'A':      // set suffix UK/Plessey code
            		symbology = SYMBOLOGY_UK_PLESSEY;
	memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = 0;
	break;

	case ('P' << 8) + 'R':      // clear all suffixes
            		for (symbology = 0; symbology < MAX_SYMBOLOGIES; symbology++)
            			memset(param_tab[symbology].suffix, '\0', MAX_SUFFIX_LENGTH);
	suffix = -1;
	break;

	default:
		error = 0;
		if ((ch = direct_character_input(barcode[0],barcode[1])) !=0xFF)
		{
			if ( ((prefix != -1) && (prefix >= MAX_PREFIX_LENGTH)) ||
					((suffix != -1) && (suffix >= MAX_SUFFIX_LENGTH)) ||
					((prefix == -1) && (suffix == -1)))
			{
				error = 1;
				break;
			}

			if (prefix != -1)
			{
				if (symbology == MAX_SYMBOLOGIES)
					for (symbology = 0; symbology < MAX_SYMBOLOGIES; symbology++)
						param_tab[symbology].prefix[prefix] = (char)ch;
				else
				{
					param_tab[symbology].prefix[prefix] = (char)ch;
					switch (symbology)
					{
					case SYMBOLOGY_UPCA_2:
						param_tab[SYMBOLOGY_UPCA_5].prefix[prefix] = (char)ch;
						break;

					case SYMBOLOGY_UPCE_2:
						param_tab[SYMBOLOGY_UPCE_5].prefix[prefix] = (char)ch;
						break;

					case SYMBOLOGY_EAN13_2:
						param_tab[SYMBOLOGY_EAN13_5].prefix[prefix] = (char)ch;
						break;

					case SYMBOLOGY_EAN8_2:
						param_tab[SYMBOLOGY_EAN8_5].prefix[prefix] = (char)ch;
						break;

					}
				}
				prefix++;
			} else
			{
				if (symbology == MAX_SYMBOLOGIES)
					for (symbology = 0; symbology < MAX_SYMBOLOGIES; symbology++)
						param_tab[symbology].suffix[suffix] = (char)ch;
				else
				{
					param_tab[symbology].suffix[suffix] = (char)ch;
					switch (symbology)
					{
					case SYMBOLOGY_UPCA_2:
						param_tab[SYMBOLOGY_UPCA_5].suffix[suffix] = (char)ch;
						break;

					case SYMBOLOGY_UPCE_2:
						param_tab[SYMBOLOGY_UPCE_5].suffix[suffix] = (char)ch;
						break;

					case SYMBOLOGY_EAN13_2:
						param_tab[SYMBOLOGY_EAN13_5].suffix[suffix] = (char)ch;
						break;

					case SYMBOLOGY_EAN8_2:
						param_tab[SYMBOLOGY_EAN8_5].suffix[suffix] = (char)ch;
						break;

					}
				}
				suffix++;
			}
		} else
			error = 1;
		break;
	}   // End switch
	if (error)
	{
		//KA check what causes error
		//	goodreadled(RED, 10);
		//	sound(TLONG, VHIGH, SERROR, NULL);
		return;
	}
	goodreadled(GREEN, 10);
	sound(TSTANDARD, VHIGH, SMEDIUM, SHIGH, NULL);
	return;
}

//------------------------------------------------------------------
//	main_send
//  ============================
//	Main send stored records handler.
//------------------------------------------------------------------
void main_send( void )
{
	int x;
	if ( fsize((char*)DATA_BASE) == -1L )
	{
		g_fSuspendConversion = TRUE;
		g_fSuspendGPS = TRUE;
		disp_no_database();
		g_fSuspendConversion = FALSE;
		g_fSuspendGPS = FALSE;
		return;
	}

	protocol_desc = "NETO Protocol";

	if (protocol_enabled == ID_NO_PROTOCOL)
	{
		protocol_desc = "ASCII Mode";
	}
	else if(protocol_enabled == ID_XMODEM_PROTOCOL)
	{
		protocol_desc = "XMODEM Protocol";
	}
	else if(protocol_enabled == ID_OSECOMM_PROTOCOL)
	{
		protocol_desc = "OseComm Protocol";
	}

	info_sound();
	printf("\fTransmit Data\n");
	printf("via %s\n%s",interface_desc,protocol_desc);

	print_message_bar( MB_ALL );

	waitkey_no_charge();    //wait for user to press a key to start

	// suspend GPS for this entire operation
	g_fSuspendConversion = TRUE;
	g_fSuspendGPS = TRUE;
	if ( protocol_enabled == ID_NO_PROTOCOL )
	{
		x = send_plain();
	}
	else if(protocol_enabled == ID_XMODEM_PROTOCOL )
	{
		clear_screen();
		gotoxy(0,1);
		printf("Sending File XMODEM\n");
		printf("Connection ...");
		x = send_xmodem();
	}
	else if(protocol_enabled == ID_OSECOMM_PROTOCOL )
	{
		clear_screen();
		gotoxy(0,1);
		printf("Sending File..\nWaiting OseComm\n");
		printf("Connection ...");
		if (OseComm( -1L, ANY_KEYS_ABORT, "OseComm Example", OnOseComInfo) == COMM_OK)
			x = OK;
		else
			x = NOT_OK;
	}
	else
	{
		clear_screen();
		gotoxy(0,1);
		printf("Sending File..\nWaiting NetO\n");
		printf("Connection ...");
		x = send_neto();
	}

	g_fSuspendConversion = FALSE;
	g_fSuspendGPS = FALSE;

	if ( x == NOT_OK )
	{
		errbeep();
		putchar('\f');    //clear screen
		print_message_bar( MB_ALL );
		gotoxy(0,1);
		printf(" Send File Err\n");
		return;

	}

	//KA
	info_sound();
	putchar('\f');    //clear screen
	print_message_bar( MB_YESNO );
	gotoxy(0,0);
	printf("  File Was Sent.");
	gotoxy(0,2);
	printf(" Delete Data File?");

	resetkey();
	if (waitkey_no_charge()== UP_KEY)
	{
		putchar('\f');    //clear screen
		main_delete();
	}


	putchar('\f');    //clear screen
	display_splash_screen();
	menu_layers[0] = 1; // start item of the main menu
}

//------------------------------------------------------------------
//	OnOseComInfo
//  ============================
//	OseComm status.
//------------------------------------------------------------------

void OnOseComInfo(int status, int errorsuccess, int progress, const char *info)
{
	switch (status)
	{
	case STAT_GET_TIME_DATE:
		break;
	case STAT_SET_TIME_DATE:
		break;
	case STAT_GET_OS_VERSION:
		break;
	case STAT_GET_APPL_VERSION:
		break;
	case STAT_XMIT_FILE_TO_PC:
		// if successful then delete the file
		if (errorsuccess == SUCC_COMPLETE)
			//remove(info);    // Delete the transfered file
			break;
	case STAT_RECV_FILE_FROM_PC:
		break;
	case STAT_LIST_FILES:
		break;
	}
}

//------------------------------------------------------------------
//	sys_barcodes
//  ============================
//	System barcode config menu.
//------------------------------------------------------------------
void sys_barcodes( void )
{
	menu_layers[2] = 1;

	if( scanner.ModelNumber == PX36 )
		fancyselmenu( (fancyselmenus*)barcodem_with2d, menu_layers, sizeof(barcodem_with2d) / sizeof(fancyselmenus), &enabled_barcodes, 1 );
	else
		fancyselmenu( (fancyselmenus*)barcodem, menu_layers, sizeof(barcodem) / sizeof(fancyselmenus), &enabled_barcodes, 1 );

	set_barcodes();
}

//------------------------------------------------------------------
//	sys_datetime
//  ============================
//	System date/time handler.
//------------------------------------------------------------------
void sys_datetime( void )
{
	int key;
	struct time t;
	struct date d;

	putchar('\f');
	printsign(0, 7, 6 );    // back
	printsign(11, 7, 10 );  // set

	set_font_input();
	gotoxy(0,0);
	printf("\tr    DATE / TIME    \tr");

	resetkey();
	while (1)
	{
		idle();
		gettime( &t );
		getdate( &d );
		gotoxy(4,1);
		printf("%-10.10s", day_gb[what_day_of_week()].item);
		gotoxy(4,2);
		printf("%02d-%02d-%04d", d.da_day, d.da_mon, d.da_year );
		gotoxy(5,3);
		printf("%02d:%02d:%02d", t.ti_hour, t.ti_min, t.ti_sec );

		if ( !kbhit())
		{
			if (battery_type == BAT_CHARGE)
			{
				is_on_cradle();
			}
			check_batt_empty(); // check the battery
			idle();
			delay(5); // 100 ms delay
			continue;
		}

		key = KAwaitkey( battery_type );
		if ( key == UP_KEY )
			return;
		else if ( key ==  DOWN_KEY )
		{
			set_time_and_date();
			putchar('\f');
			printsign(0, 7, 6 );    // back
			printsign(11, 7, 10 );  // set

			set_font_input();
			gotoxy(0,0);
			printf("\tr    DATE / TIME    \tr");
		}
	}
}

//------------------------------------------------------------------
//	sys_standby
//  ============================
//	System standby menu.
//------------------------------------------------------------------
void sys_standby( void )
{
	menu_layers[2] = 1;
	fancyselmenu( (fancyselmenus*)standbym, menu_layers, sizeof(standbym) / sizeof(fancyselmenus), &standby_enabled, 0 );

	set_standby();
}

//------------------------------------------------------------------
//	sys_backlight
//  ============================
//	System backlight menu.
//------------------------------------------------------------------
void sys_backlight( void )
{
	menu_layers[2] = 1;
	fancyselmenu( (fancyselmenus*)backlightm, menu_layers, sizeof(backlightm) / sizeof(fancyselmenus), &backlight_enabled, 0 );

	backlight( (unsigned short) backlight_enabled, 500 );
}

//------------------------------------------------------------------
//	printsign
//  ============================
//	Displays preset symbols at specified coordinates.
//------------------------------------------------------------------
void printsign( int x_start, int y, int signid )
{
	setfont( USER_FONT, (const unsigned char*)_signs );
	gotoxy(x_start,y);
	switch ( signid )
	{
	case 0: // arrow left
		printsymbol(0); // arrow left
		break;
	case 1: // arrow right
		printsymbol(1); // arrow right
		break;
	case 2: // arrow down
		printsymbol(2); // arrow down
		break;
	case 3: // arrow up
		printsymbol(3); // arrow up
		break;
	case 4: // select
		printsymbol(4); // select 1/4
		gotoxy(x_start+1,y);
		printsymbol(5); // select 2/4
		gotoxy(x_start+2,y);
		printsymbol(6); // select 3/4
		gotoxy(x_start+3,y);
		printsymbol(7); // select 4/4
		break;
	case 5: // clear
		printsymbol(8);  // clear 1/4
		gotoxy(x_start+1,y);
		printsymbol(9);  // clear 2/4
		gotoxy(x_start+2,y);
		printsymbol(10); // clear 3/4
		gotoxy(x_start+3,y);
		printsymbol(11); // clear 4/4
		break;
	case 6: // back
		printsymbol(12); // back 1/4
		gotoxy(x_start+1,y);
		printsymbol(13); // back 2/4
		gotoxy(x_start+2,y);
		printsymbol(14); // back 3/4
		gotoxy(x_start+3,y);
		printsymbol(15); // back 4/4
		break;
	case 7: // enter
		printsymbol(16); // enter 1/4
		gotoxy(x_start+1,y);
		printsymbol(17); // enter 2/4
		gotoxy(x_start+2,y);
		printsymbol(18); // enter 3/4
		gotoxy(x_start+3,y);
		printsymbol(19); // enter 4/4
		break;
	case 8: // keyboard
		printsymbol(20); // keyboard 1/6
		gotoxy(x_start+1,y);
		printsymbol(21); // keyboard 2/6
		gotoxy(x_start+2,y);
		printsymbol(22); // keyboard 3/6
		gotoxy(x_start+3,y);
		printsymbol(23); // keyboard 4/6
		gotoxy(x_start+4,y);
		printsymbol(24); // keyboard 5/6
		gotoxy(x_start+5,y);
		printsymbol(25); // keyboard 6/6
		break;
	case 9: // empty
		printsymbol(26); // empty
		break;
	case 10: // set
		printsymbol( 27 ); // set 1/3
		gotoxy( x_start+1, y);
		printsymbol( 28 ); // set 2/3
		gotoxy( x_start+2, y);
		printsymbol( 29 ); // set 3/3
		break;
	case 11: // options
		printsymbol(30); // options 1/4
		gotoxy(x_start+1,y);
		printsymbol(31); // options 2/4
		gotoxy(x_start+2,y);
		printsymbol(32); // options 3/4
		gotoxy(x_start+3,y);
		printsymbol(33); // options 4/4
		break;

	}
}

//------------------------------------------------------------------
//	printbar
//  ============================
//	Displays menu bar at speicified row.
//------------------------------------------------------------------
void printbar( int y, int type, int LeftRight )
{
	if ( LeftRight )
		printsign( 0, y, 0 ); // arrow left
	else
		printsign( 0, y, 3 ); // arrow up

	switch ( type )
	{
	case 0:     // select
		printsign( 5, y, 4); // select
		break;
	case 1:     // back
		printsign( 5, y, 6 ); // back
		break;
	case 2:     // clear
		printsign( 5, y, 5 ); // clear
		break;
	case 3:     // enter
		printsign( 5, y, 7 ); // enter
		break;
	}
	if ( LeftRight )
		printsign( 13, y, 1 ); // arrow right
	else
		printsign( 13, y, 2 ); // arrow down
}

//------------------------------------------------------------------
//	set_standby
//  ============================
//	Sets autopower down according to menu cofiguration.
//------------------------------------------------------------------
void set_standby( void )
{
	switch ( standby_enabled )
	{
	case ST_05_SEC:
		autopowerdown( ON, 250 );
		g_nGPSStandbyTimeout = 250;
		break;
	case ST_10_SEC:
		autopowerdown( ON, 500 );
		g_nGPSStandbyTimeout = 500;
		break;
	case ST_15_SEC:
		autopowerdown( ON, 750 );
		g_nGPSStandbyTimeout = 750;
		break;
	case ST_30_SEC:
		autopowerdown( ON, 1500 );
		g_nGPSStandbyTimeout = 1500;
		break;
	case ST_45_SEC:
		autopowerdown( ON, 2250 );
		g_nGPSStandbyTimeout = 2250;
		break;
	case ST_60_SEC:
		autopowerdown( ON, 3000 );
		g_nGPSStandbyTimeout = 3000;
		break;
	case ST_120_SEC:
		autopowerdown( ON, 6000 );
		g_nGPSStandbyTimeout = 6000;
		break;
	case ST_180_SEC:
		autopowerdown( ON, 9000 );
		g_nGPSStandbyTimeout = 9000;
		break;
	default:
		standby_enabled = ST_30_SEC;
		autopowerdown( ON, 1500 );
		g_nGPSStandbyTimeout = 1500;
		break;
	}
	// for GPS, turn off the normal autopower and use a separate timer
	if(scanner.GPS_Model == 1)
	{
		autopowerdown(OFF, 0);
		StartMyTimer(TIMER_ID_POWER_DOWN, g_nGPSStandbyTimeout, TRUE);
	}
}

//------------------------------------------------------------------
//	set_barcodes
//  ============================
//	Sets bar code enables according to menu configuration.
//------------------------------------------------------------------
void set_barcodes( void )
{
	static char options[100 + 1];

	strcpy(options, "B0");

	if ( enabled_barcodes & ID_CODE39 )
	{
		strcat(options, "B2");
	}
	if ( enabled_barcodes & ID_EAN )
	{
		strcat(options, "R4");
	}
	if ( enabled_barcodes & ID_UPC )
	{
		strcat(options, "R1");
	}
	if ( enabled_barcodes & ID_I2OF5 )
	{
		strcat(options, "R8");
	}
	if ( enabled_barcodes & ID_D2OF5 )
	{
		strcat(options, "R7");
	}
	if ( enabled_barcodes & ID_CODABAR )
	{
		strcat(options, "B3");		
	}
	if ( enabled_barcodes & ID_CODE93 )
	{
		strcat(options, "B5");
	}
	if ( enabled_barcodes & ID_CODE128 )
	{

		strcat(options, "B6");
	}
	if ( enabled_barcodes & ID_MSI_PLESSEY )
	{

		strcat(options, "B7");
	}
	if ( enabled_barcodes & ID_TELEPEN )
	{

		strcat(options, "B9");
	}
	if ( enabled_barcodes & ID_UK_PLESSEY )
	{

		strcat(options, "B1");
	}
	if ( enabled_barcodes & ID_IATA  )
	{

		strcat(options, "B4");
	}
	if ( enabled_barcodes & ID_SCODE )
	{

		strcat(options, "R9");
	}
	if ( enabled_barcodes & ID_M2OF5 )
	{

		strcat(options, "BB");
	}
	if ( enabled_barcodes & ID_RSS_14 )
	{

		strcat(options, "JX");
	}
	if ( enabled_barcodes & ID_RSS_LIMITED )
	{

		strcat(options, "JY");
	}
	if ( enabled_barcodes & ID_RSS_EXPANDED )
	{
		strcat(options, "DR");
	}
	if ( enabled_barcodes & ID_PDF417 )
	{
		strcat(options, "[BCF");
	}
	if ( enabled_barcodes & ID_MICRO_PDF417 )
	{
		strcat(options, "[BCG");
	}
	if ( enabled_barcodes & ID_ADDONS )
	{
		strcat(options, "R2R3R5R6H3"); // UPC +2 +5, EAN +2 +5, NW7 ABC CX
	}

	if( scanner.ModelNumber == PX36 )
	{
		if ( enabled_barcodes & ID_DATAMATRIX )
		{
			strcat( options, "[BCC" );
		}

		if ( enabled_barcodes & ID_AZTEC )
		{
			strcat( options, "[BCH" );
		}
		if ( enabled_barcodes &  ID_AZTEC_RUNES )
		{
			strcat( options, "[BF2" );
		}
		if ( enabled_barcodes & ID_QR_CODE )
		{
			strcat( options, "[BCD" );
		}
		if ( enabled_barcodes & ID_MICRO_QR_CODE )
		{
			strcat( options, "[D2U" );
		}
		if ( enabled_barcodes & ID_MAXICODE )
		{
			strcat( options, "[BCE" );
		}
	}


	// alwais set these...
	strcat(options,"RZ");

	strcat(options,"OG");        //Enable EAN128


	strcat(options,"[BHC");      //Transmit RSS link flag
	//	'0' = no 2D-composite
	//	'1' = CC-A or CC-B composite attached

	strcat(options,"[BI0");      //Transmit MicroPDF/CC-A/CC-B nr of data columns
	//	'1' = MicroPDF symbol with 1 data column
	//	'2' = MicroPDF or CC-A/B symbol with 2 data columns
	//	'3' = MicroPDF or CC-A/B symbol with 3 data columns
	//	'4' = MicroPDF or CC-A/B symbol with 4 data columns

	strcat(options,"OT");        //Transmit EAN128 link flag
	//	'0' = no 2D-composite attacked
	//	'1' = CC-A or CC-B composite attached
	//	'2' = CC-C composite attached
	//

	systemsetting( options );

}

//------------------------------------------------------------------
//	display_mem
//  ============================
//	System config/mem free memory display handler.
//------------------------------------------------------------------
void display_mem(void)
{
	long mem_size;
	mem_size = coreleft();

	putchar('\f');
	set_font_input();
	gotoxy(0,0);
	printf("\tr    FREE MEMORY    \tr");

	gotoxy(3,2);
	printf("%6ld Bytes", mem_size );
	rectangle3d( (3*6)-2, ((5-1)*6)-1, ((3+12)*6), ((5+1)*6)-1, 1);

	print_message_bar( MB_ALL );

	resetkey();
	KAwaitkey( battery_type );
}

//------------------------------------------------------------------
//	disp_no_database
//  ============================
//	Displays empty databse file message.
//------------------------------------------------------------------
void disp_no_database( void )
{
	set_font_menu();
	errbeep();
	show_message("Database notpresent on  the RAM disk!!!", MB_ALL | MB_ICONWARNING );
}

//------------------------------------------------------------------
//	display_scroll_data
//  ============================
//	Display data record.
//------------------------------------------------------------------
void display_scroll_data( db_record *db_rec, long curr, long max, int infoType )
{
	// char pszShortLatitude[12];
	// char pszShortLongitude[12];
	int nBreadCrumbMatch;
	int fSmallDisplay = FALSE;
	char pszShortBarCode[19];

	set_font_input();
	gotoxy(0,0);
	printf("\trSCROLL %04ld / %04ld \tr", curr+1, max+1);
	// setfont( LARGE_FONT, NULL );
	// setoffset( -4 );
	if((gps_data_format == GPS_DEG_MIN_SEC) && ((infoType == 1) || (scanner.QTY_Required != QTY_REQUIRED)))
	{
		setfont(SMALL_FONT, NULL);
		cursor(NOWRAP);
		setoffset(5);
		fSmallDisplay = TRUE;
	}
	gotoxy(0,1);
	memcpy(pszShortBarCode, db_rec->barcode, 18);
	pszShortBarCode[18] = 0;
	printf(pszShortBarCode);

	// check for normal scroll display.
	if ((scanner.GPS_Model == 0) || (scanner.QTY_Required != QTY_NOT_REQUIRED))
	{
		gotoxy(0,2);

		// check for a bread crumb
		nBreadCrumbMatch = memcmp(g_pszGPSBreadCrumb, db_rec->barcode, strlen(g_pszGPSBreadCrumb)-1);
		if (nBreadCrumbMatch == 0)
		{
			// show quantity as number
			if(fSmallDisplay == FALSE)
			{
				printf("Index:   %-*.*s", SZ_SIGN+SZ_QUANTITY, SZ_SIGN+SZ_QUANTITY, db_rec->quantity );
			}
			else
			{
				printf("Index%s", db_rec->quantity );
			}
		} else
		{
			// normal display
			if(fSmallDisplay == FALSE)
			{
				printf("Total:   %-*.*s", SZ_SIGN+SZ_QUANTITY, SZ_SIGN+SZ_QUANTITY, db_rec->quantity );
			}
			else
			{
				printf("Total%s", db_rec->quantity );
			}
		}

	}
	setoffset( 0 );

	print_message_bar( SCROLL_BAR );
}

//------------------------------------------------------------------
//	fill_record_struct
//  ============================
//	Populate data record from character array.
//------------------------------------------------------------------
void fill_record_struct( db_record *db_rec, char* record )
{
	int offset;
	memset( db_rec, '\0', sizeof( db_record ));
	offset = 0;
	memcpy( db_rec->barcode, record+offset, SZ_BARCODE );
	offset+=SZ_BARCODE+1;
	memcpy( db_rec->quantity, record+offset, SZ_SIGN+SZ_QUANTITY );
}

//------------------------------------------------------------------
//	get_record
//  ============================
//	Get specified data record from file.
//------------------------------------------------------------------
int get_record( db_record *db_rec, long *rec_nr, long *max_rec )
{
	FILE * pFile;
	char record[ SZ_RECORD + 1];

	if (((*max_rec) = fsize( DATA_BASE )) == -1L )
	{
		errbeep();
		show_message("Database size mismatch.", MB_ALL | MB_ICONWARNING );
		return -1;
	}
	(*max_rec) /= SZ_RECORD;
	(*max_rec)--;

	if ( (*rec_nr) > (*max_rec) )
	{
		errkeybeep();
		show_message("Record number out of range.", MB_ALL | MB_ICONWARNING );
		(*rec_nr) = (*max_rec);
	} else if ( (*rec_nr) < 0L )
	{
		errkeybeep();
		(*rec_nr) = 0L;
	}

	if ((pFile = fopen((char*)DATA_BASE, "rb")) == NULL)
	{
		errbeep();
		show_message("File open error.", MB_ALL | MB_ICONWARNING );
		return -1;
	}
	fseek( pFile, (long)((*rec_nr) * SZ_RECORD), SEEK_SET);
	if ( fread(record, 1, SZ_RECORD, pFile) != SZ_RECORD )
	{
		errbeep();
		show_message("Record read error.", MB_ALL | MB_ICONWARNING );
		fclose(pFile);
		return -10;
	}
	fclose( pFile );

	fill_record_struct( db_rec, record );
	return 1;
}

//------------------------------------------------------------------
//	show_message
//  ============================
// 	Show a message with an icon max message size is 60 chars
//------------------------------------------------------------------
int show_message( const char* msg, int icons )
{
	int key;

	putchar('\f');
	print_message_bar( icons );

	print_msg_icons(0, 0, icons );  // show the matching icon
	set_font_input();
	setoffset( 8 );
	if ( strlen( msg ) > 0 )
	{
		gotoxy( 6, 0 );
		printf("%-12.12s", msg );
	}
	if ( strlen( msg ) > 12 )
	{
		gotoxy( 6, 1 );
		printf("%-12.12s", msg+12 );
	}
	if ( strlen( msg ) > 24 )
	{
		gotoxy( 0, 2 );
		printf("%-18.18s", msg+24 );
	}
	if ( strlen( msg ) > 42 )
	{
		gotoxy( 0, 3 );
		printf("%-18.18s", msg+42 );
	}
	setoffset( 0 );
	resetkey();

	for (;;)
	{
		key = KAwaitkey( battery_type );

		if ( icons & MB_ALL )
			return( OK );

		switch ( key )
		{
		case UP_KEY:
			return( YES );

		case DOWN_KEY:
			return( NO );
		}
	}
	return(0);
}

//------------------------------------------------------------------
//	printbattery
//  ============================
//	Displays the battery icon.
//------------------------------------------------------------------
void printbattery( void )
{
	setfont( USER_FONT, (const unsigned char*)_battvolt );

	display_icon( 9, 2, 0 );
	set_font_input();
}

//------------------------------------------------------------------
//	display_BT_error
//  ============================
//	Display bluetooth error status.
//------------------------------------------------------------------
void display_BT_error(int err)
{
	char err_msg[25+1];

	memset(err_msg,'\0',sizeof(err_msg));

	switch (err)
	{
	case -1:
		strcpy(err_msg , "Discovery Failure");
		break;
	case -2:
	case -4:
	case -5:
		strcpy(err_msg, "Invalid BT Address");
		break;
	case -6:
	case -7:
		strcpy(err_msg , "User Canceled");
		break;

	}

	putchar('\f');
	gotoxy(0,0);
	printf ("\tr  BLUETOOTH ERROR  \tr");
	gotoxy(0,2);
	printf("Err: %d\n%s",err,err_msg);
	resetkey();
	print_message_bar(MB_ALL);
	KAwaitkey( battery_type );
}

//------------------------------------------------------------------
//	set_time_and_date
//  ============================
//	Sets the time and date from keypad inputs.
//------------------------------------------------------------------
void set_time_and_date(void)
{
	struct date dates;
	struct time times;
	char time_str[8 + 1];       // HH:MM:SS
	char date_str[14 + 1];      // YYYY/MM/DD
	int x, y;
	int direction;
	gettime(&times);
	getdate(&dates);
	sprintf(time_str, "%02d:%02d:%02d", times.ti_hour, times.ti_min, times.ti_sec);
	sprintf(date_str, "%02d/%02d/%04d End", dates.da_day, dates.da_mon, dates.da_year);

	putchar('\f');
	setstartpos( &x, &y );
	direction = 0;


	for (;;)
	{
		display_time_date( time_str, date_str );

		setcursorpos( &x, &y, direction );
		if ('E' == (( y == 2)? time_str[x]:date_str[x]))
			printbar( 7, 1 , 1);
		else
			printbar( 7, 0 , 1);
		set_font_input();
		gotoxy( x, y );
		cursor( ON );

		switch ( KAwaitkey( battery_type ))
		{
		case UP_KEY: // to left
		    x--;
		    direction = -1;
		    break;
		case DOWN_KEY: // to right
			x++;
			direction = +1;
			break;
		case TRIGGER_KEY:
			if ('E' == (( y == 2)? time_str[x]:date_str[x]))
			{
				cursor( OFF );
				switch ( display_do_setting() )
				{
				case 0:     // store settings
					x = is_time_string_ok( time_str);
					x |= is_date_string_ok( date_str );
					if ( x == 0 )
					{
						store_time_date( time_str, date_str );
						return;
					}
					display_time_date_error( x );
					putchar('\f');
					setstartpos( &x, &y );
					cursor( ON );
					break;
				case 1:     // cancel
					return;
				}
			} else
				change_character( x, y, time_str, date_str );

			break;
		case CLR_KEY:
		{
			cursor( OFF );
			switch ( display_do_setting() )
			{
			case 0:     // store settings
			x = is_time_string_ok( time_str);
			x |= is_date_string_ok( date_str );
			if ( x == 0 )
			{
				store_time_date( time_str, date_str );
				return;
			}
			display_time_date_error( x );
			putchar('\f');
			setstartpos( &x, &y );
			cursor( ON );
			break;
			case 1:     // cancel
				return;
			}
		}
		break;
		}
	}
}

//------------------------------------------------------------------
//	display_change_quantity
//  ============================
//	Displays the change quantity screen.
//------------------------------------------------------------------
void display_change_quantity( db_record *db_rec )
{
	set_font_input();
	gotoxy(0,0);
	clear_screen();
	printf("\tr  CHANGE QUANTITY  \tr");
	// setfont( MEDIUM_FONT, NULL );
	// set_offset( -4 );
	gotoxy(0,1);
	printf("%-18.18s", db_rec->barcode );
	gotoxy(0,2);
	printf("Total: %-*.*s ", SZ_SIGN+SZ_QUANTITY, SZ_SIGN+SZ_QUANTITY, db_rec->quantity );
	// set_offset( -7 );
	gotoxy(0,3);
	printf("New:              " );
	printsign(0, 7, 6 ); // back
	printsign(5, 7, 7 ); // enter
}

//------------------------------------------------------------------
//	display_barcode_options
//  ============================
//	Displays the quantity change options screen.
//------------------------------------------------------------------
void display_barcode_options( db_record *db_rec )
{
	set_font_input();
	gotoxy(0,0);
	clear_screen();
	printf("\tr  BARCODE OPTIONS  \tr");
	setfont( MEDIUM_FONT, NULL );
	// set_offset( -4 );
	gotoxy(0,1);
	printf("%-18.18s", db_rec->barcode );

	gotoxy(0,2);
	printf("Total: %-*.*s ", SZ_SIGN+SZ_QUANTITY, SZ_SIGN+SZ_QUANTITY, db_rec->quantity );
	// set_offset( -7 );
	gotoxy(0,3);
	printf("%-18.18s", " "); // clear the line

	setoffset( 0 );
	if (scanner.QTY_Required != QTY_NOT_REQUIRED)
		print_message_bar( OPTION_BAR );     //KA this bar shows the CHANGE option in the middle
	else
		print_message_bar( OPTION_BAR_NO_QTY );    //KA This bar does NOT show  the CHANGE option (middle)

}

//------------------------------------------------------------------
//	change_quantity
//  ============================
//	Change quantity handler.
//------------------------------------------------------------------
void change_quantity( db_record* db_rec )
{
	static char quant[ SZ_INPUT_QUANT + 1 ];
	static char readed_rec[ SZ_RECORD + 1 ];
	int qu_ret;
	long org_quant;
	long new_quantity;
	long found_record = -1;
	int illegal;

	set_font_input();

	org_quant = string_quantity_to_long( db_rec->quantity, &illegal );
	for (;;) // start of input for loop
	{
		display_change_quantity( db_rec );

		memset( quant, '\0', sizeof( quant ));
		set_font_input();
		qu_ret = KeyboardQuantity( quant,   // string that holds the typed quantity
				SZ_INPUT_QUANT,         // the max quantity size
				NEGATIVE | NUM,     // negative and numeric value possible
				5,                  // x starting possition
				3,                  // y position
				0,                  // offset position
				org_quant,          // default value
				SZ_INPUT_QUANT+SZ_SIGN,// the max display size
				FALSE,              // show an input rectangle
				4,                  // number of special keys
				CLR_KEY, TRIGGER_KEY, DOWN_KEY, ENT_KEY ); // the special keys to return from function

		set_font_input();
		// cut off the default flag if it exists
		if ( (qu_ret & 0xFF) == CLR_KEY || (qu_ret & 0xFF) == DOWN_KEY)
		{
			return;
		}

		if ( check_input_quant( quant, (qu_ret & DEFAULT), org_quant, 0L, &new_quantity ) != OK )
		{
			continue;
		}
		if ( new_quantity != org_quant )
		{
			// some message if the user is sure
			// that he changed the quantity
			if ( show_message("Are you sureyou want to store the new     quantity?", MB_YESNO | MB_ICONWARNING ) == YES )
			{
				// search and accumulate only for quantity required
				if(scanner.QTY_Required == QTY_REQUIRED)
				{
					// find the matching record
					found_record =  bin_search( (char*)DATA_BASE, readed_rec, db_rec->barcode, SZ_BARCODE, 0, SZ_RECORD);
				}
				else if(scanner.QTY_Required == QTY_ENTER_ONLY)
				{
					// for quantity entered, point to the last record
					if ( (found_record = fsize( DATA_BASE )) != -1L)
					{
						found_record /= SZ_RECORD;
						found_record --;
					}
				}

				if(found_record != -1L)
				{
					if ( new_quantity == 0L )
					{
						remove_record( found_record );
						memset( db_rec, '\0', sizeof( db_record ));
					}
					else
					{
						// store it
						update_quantity_rec_struct( db_rec, new_quantity );
						save_data( db_rec, found_record );
					}
				}
			}
		}
		return;
	} // end of input for loop
}

//------------------------------------------------------------------
//	show_options_menu
//  ============================
//	Barcode scan options handler.
//------------------------------------------------------------------
void show_options_menu( db_record *db_rec )
{
	static char readed_rec[ SZ_RECORD +1 ];
	long found_rec;
	for (;;)
	{
		display_barcode_options( db_rec );
		switch ( KAwaitkey( battery_type ) )
		{
		case TRIGGER_KEY: // change the quantity value
			if (scanner.QTY_Required != QTY_NOT_REQUIRED)
			{
				g_fSuspendConversion = TRUE;
				g_fSuspendGPS = TRUE;
				change_quantity( db_rec );
				g_fSuspendConversion = FALSE;
				g_fSuspendGPS = FALSE;
				return;
			}
			break;

		case UP_KEY:
		case CLR_KEY:   // return to previous menu
			return;


		case DOWN_KEY: // delete the current record from database
			info_sound();
			g_fSuspendConversion = TRUE;
			g_fSuspendGPS = TRUE;
			if ( show_message("Are you sureyou want to remove the item from the database?", MB_YESNO | MB_ICONWARNING ) == YES )
			{
				if(scanner.QTY_Required == QTY_REQUIRED)
				{
					found_rec =  bin_search( (char*)DATA_BASE, readed_rec, db_rec->barcode, SZ_BARCODE, 0, SZ_RECORD);
				}
				else
				{
					// point to the last record
					if ( (found_rec = fsize( DATA_BASE )) != -1L)
					{
						found_rec /= SZ_RECORD;
					}
				}
				if(found_rec != -1L)
				{
					// remove the record
					remove_record( found_rec );
				}

				// clear the db_record structure
				memset( db_rec, '\0', sizeof( db_record ));
				return;
			}
			g_fSuspendConversion = FALSE;
			g_fSuspendGPS = FALSE;
			break;
		default:
			break;
		}
	}
}

//------------------------------------------------------------------
//	check_input_quant
//  ============================
//	Checks the quantity string input and returns the resulting quantity.
//------------------------------------------------------------------
int check_input_quant( char* input_quant, int use_default, long def_value, long fnd_quantity, long *new_quantity )
{
	static char max_val[ SZ_QUANTITY + 1];
	static char min_val[ SZ_SIGN+ SZ_QUANTITY + 1];
	long max;
	long min;
	int illegal;

	if ( use_default )
	{
		*new_quantity = def_value;
	} else
	{
		// quantity input over here
		*new_quantity = string_quantity_to_long( input_quant, &illegal );
		if ( *new_quantity == 0L && illegal )
		{
			// zero input or no input at all do not
			// change anything in the database
			// show a message on screen that input is not valid
			info_sound();
			show_message("Quantity    is not a    valid value!", MB_ALL | MB_ICONINFORMATION );
			return ERR_QUANTITY_ILLEGAL;
		}
	}

	// test for underflow and overflow
	memset(max_val, '9', sizeof( max_val ));
	max_val[ SZ_QUANTITY ] = '\0';
	max = atol( max_val );

	memset( min_val, '9', sizeof( min_val ));
	min_val[0] = '-';
	min_val[ SZ_SIGN + SZ_QUANTITY ] = '\0';
	min = atol( min_val );

	if ( (fnd_quantity + *new_quantity ) > max )
	{
		// message total new quantity more then space available
		info_sound();
		show_message("Quantity    exceeding   max input value,  value not stored!", MB_ALL | MB_ICONINFORMATION );
		return ERR_QUANTITY_MIN_MAX;
	}
	if ( (fnd_quantity + *new_quantity ) < min )
	{
		// message total new quantity more then space available
		info_sound();
		show_message("Quantity    exceeding   min input value,  value not stored!", MB_ALL | MB_ICONINFORMATION );
		return ERR_QUANTITY_MIN_MAX;
	}
	return OK;
}

//------------------------------------------------------------------
//	string_quantity_to_long
//  ============================
//	Converts the quantity string to a long value.
//------------------------------------------------------------------
long string_quantity_to_long( char* quantity, int* illegal )
{
	static char tmp[SZ_SIGN+SZ_QUANTITY+1];
	char* ptr;
	int pos;

	*illegal = FALSE;

	memset( tmp, '\0', sizeof( tmp ));
	ptr = tmp;
	for ( pos = 0; pos < (SZ_SIGN+SZ_QUANTITY); pos++ )
	{
		if ( quantity[pos] == '\0' )
			break;

		if ( quantity[pos] == ' ') // cut off all spaces
			continue;

		*ptr++ = quantity[pos];
	}
	if ( atol( tmp ) == 0L && tmp[0] == '-' )
		*illegal = TRUE; // illegal value a '-' or -0 are not legal values
	return( atol( tmp ));
}

//------------------------------------------------------------------
//	bin_search
//  ============================
//	Binary search of data records for a matching string.
//	Returns -1L if string not found.
//	The data records must be sorted in order for this function to work.

//------------------------------------------------------------------
long bin_search(char *fname, char *record, char *sstring, int sstr_len, int start, int rec_size)
{
	FILE * pFile;
	int test;
	long min, max, current;
	int readSize;

	if ((max = fsize((char*)DATA_BASE)) == -1L )
	{
		// file does not exist
		return -1L ;
	}

	max /= (long) rec_size;
	if ((pFile = fopen(fname, "rb")) == NULL)
	{
		// cannot open the file the file exist
		// this should never happen
		return(-1L);
	}

	min = 0L;
	for (;;)
	{
		current = ((max - min) >> 1) + min;
		fseek(pFile, current * (long) rec_size, SEEK_SET);
		readSize = fread(record, 1, rec_size, pFile);
		if ( readSize != rec_size)
		{
			fclose(pFile);
			return(-1L);
		}
		test = memcmp(sstring, record + start, sstr_len);
		// test code to show compare results
		//        clear_screen();
		//        gotoxy(0,1);
		//        printf("offset %d", current * rec_size);
		//        gotoxy(0,2);
		//        printf(sstring);
		//        gotoxy(0,3);
		//        printf(record+start);
		//        gotoxy(0,4);
		//        printf("test %d", test);
		//        WaitKey();
		// for some reason the compare returns -32 when there is a match
		if ((test == -32) || (test == 0))
		{   // found the barcode in the database
			fclose(pFile);
			return(current);
		}
		if (test < 0)
			max = current - 1L;
		else
			min = current + 1L;
		if (min > max)
		{
			fclose(pFile);
			// item is not found in the file
			return(-1L);
		}
	}
	fclose(pFile);
	return(-1L);
}

//------------------------------------------------------------------
//	GetLastRecord
//  ============================
//	Gets the record number of the last record stored.
//------------------------------------------------------------------
long GetLastRecord(int rec_size)
{
	long lastRec;
	long max;

	max = fsize((char*)DATA_BASE);
	lastRec = max / (long) rec_size;

	return lastRec;
}

//------------------------------------------------------------------
//	print_msg_icons
//  ============================
//	Displays the selected icon.
//------------------------------------------------------------------
void print_msg_icons( int x, int y, int icons )
{
	int start;
	setfont(USER_FONT, (const unsigned char*)_msgicons );

	if ( icons & MB_ICONWARNING )
		start = 0;
	if ( icons & MB_ICONQUESTION )
		start = 1;
	if ( icons & MB_ICONERROR )
		start = 2;
	if ( icons & MB_ICONINFORMATION )
		start = 3;

	display_icon( x, y, start );
}

//------------------------------------------------------------------
//	send_neto
//  ============================
//	Send data records using neto handler.
//------------------------------------------------------------------
int send_neto( void )
{
	int x;
	long port;

	//	baudrate_enabled = scanner.Baud;
	setbaudrate( baudrate_enabled );
	port = get_active_port();
	if ( port  < 0)
		return( NOT_OK );

	if (port == MASTER_COM || port == SLAVE_COM)
		init_connection();
	else   //open port only for non BT connection otherwise it will fail
		if ( comopen ((unsigned int) port ) != 0 ) // OPL9728 needs COM1 also on cradle
		{
			errbeep();
			show_message("Error while opening COM port!", MB_ALL | MB_ICONERROR );
			return( NOT_OK );
		}


	setfont(LARGE_FONT,NULL);
	clear_screen();
	printf(" Sending File\n\nWaiting NetO\n");
	printf("Connection ...");


	x = neto_transmit( (char(*)[15+1])DATA_BASE, 1, g_termID, TRIGGER_KEY, 3 );
	comclose( (unsigned int)port );
	set_font_input();
	if ( x != OK )
	{
		errbeep();
		show_message("Transmissionerror duringtransfer protocol!", MB_ALL | MB_ICONERROR );
		return( NOT_OK );
	}
	return( OK );
}

//------------------------------------------------------------------
//	send_xmodem
//  ============================
//	Send data records using xmodem handler.
//------------------------------------------------------------------
int send_xmodem(void)
{
	int x;
	long port;

	setbaudrate( baudrate_enabled );
	port = get_active_port();
	if ( port  < 0)
		return( NOT_OK );

	if (port == MASTER_COM || port == SLAVE_COM)
		init_connection();
	else   //open port only for non BT connection otherwise it will fail
		if ( comopen ((unsigned int) port ) != 0 ) // OPL9728 needs COM1 also on cradle
		{
			errbeep();
			show_message("Error while opening COM port!", MB_ALL | MB_ICONERROR );
			return( NOT_OK );
		}


	setfont(LARGE_FONT,NULL);
	clear_screen();
	printf(" Sending File\n\nWaiting Xmodem\n");
	printf("Connection ...");


	x = xmodemtransmit( (const char*)DATA_BASE );
	comclose( (unsigned int)port );
	set_font_input();
	if ( x != OK )
	{
		errbeep();
		show_message("Transmissionerror duringtransfer protocol!", MB_ALL | MB_ICONERROR );
		return( NOT_OK );
	}
	return( OK );
}

//------------------------------------------------------------------
//	send_plain
//  ============================
//	Send data records with no protocol handler.
//------------------------------------------------------------------
int send_plain( void )
{
	static char record[ SZ_RECORD + 1 ];
	long count, max;
	int tx_count;
	FILE *pFile;
	long port;
	int fBluetooth = FALSE;



	setbaudrate( baudrate_enabled );
	port = get_active_port();
	if ( port < 0 )
		return( NOT_OK );

	if (port == MASTER_COM || port == SLAVE_COM || port == HID_MASTER_COM || port == HID_SLAVE_COM )
	{
		init_connection();
		fBluetooth = TRUE;
	}
	else   //open port only for non BT connection otherwise it will fail

		if ( comopen((unsigned int) port ) != 0) // opl9728 needs COM1 also when using cradle
		{
			errbeep();
			show_message("Error while opening COM port!", MB_ALL | MB_ICONERROR );
			return( NOT_OK );
		}


	max = fsize( (char*)DATA_BASE ) / SZ_RECORD;

	if ((pFile = fopen((char*)DATA_BASE, "rb")) == NULL)
	{
		//	comclose( COM1 );
		comclose( (unsigned int) port );
		errbeep();
		show_message("Error while opening file", MB_ALL | MB_ICONERROR );
		return( NOT_OK );
	}

	for ( count = 0L; count < max; count++ )
	{
		fseek( pFile, (long)((count) * SZ_RECORD), SEEK_SET);
		if ( fread(record, 1, SZ_RECORD, pFile) != SZ_RECORD )
		{
			//	comclose( COM1 );
			comclose( (unsigned int) port );
			fclose(pFile);
			show_message("Error while reading datafrom the database!", MB_ALL | MB_ICONERROR );
			return( NOT_OK );

		}
		if(fBluetooth == FALSE)
		{
			for (tx_count = 0; tx_count < SZ_RECORD; tx_count++)
			{

				putcom( record[tx_count] );
				printf("%c", record[tx_count]);

			}
		}
		else
		{
			PutBuffer( (unsigned char*)record, sizeof(record) );
			printf("%s", record);
		}
		delay(10);
	}
	fclose( pFile );
	return(OK);
}

//------------------------------------------------------------------
//	display_time_date_error
//  ============================
//	Display time/date error message.
//------------------------------------------------------------------
void display_time_date_error( int err_code )
{
	static char temp[60+1];
	static char errm[18+1];

	memset( temp, '\0', sizeof( temp ));
	memset( errm, '\0', sizeof( errm ));
	if ( err_code & 1 )
		strcat( errm, "Time ");
	if ( (err_code & 1 ) && (err_code & 2))
		strcat( errm, "and ");
	if ( err_code & 2 )
		strcat( errm, "Date");

	sprintf(temp, "Error in    setting:    %s", errm );
	errbeep();
	show_message( temp, MB_ALL | MB_ICONERROR );
}

//------------------------------------------------------------------
//	display_do_setting
//  ============================
//	Displays save or cancel bar and waits for response.
//------------------------------------------------------------------
int display_do_setting( void )
{
	int status;
	char text[ 6 + 1];
	status = 0;

	printbar( 7, 0, 0 );

	set_font_input();
	gotoxy(0,2);
	printf("%18.18s", " ");
	gotoxy(0,3);
	printf("%18.18s", " ");
	for (;;)
	{
		if ( status == 0 )
			strcpy( text, "SAVE  ");
		else
			strcpy( text, "CANCEL");

		gotoxy(6,2);
		printf("%-6.6s", text);
		rectangle3d( (6*6)-2, ((5-1)*6)-1, ((6+6)*6), ((5+1)*6)-1, 1);

		if ( KAwaitkey( battery_type ) == TRIGGER_KEY )
			return status;
		status ^= 1;
	}
}

//------------------------------------------------------------------
//	display_time_date
//  ============================
//	Displays time and date.
//------------------------------------------------------------------
void display_time_date( char* time_str, char* date_str )
{
	printbar( 7, 0 , 1);

	set_font_input();
	gotoxy(0, 0);
	printf("\tr  SET TIME & DATE \tr");
	gotoxy(0, 2);
	printf("%-18.18s", time_str);
	gotoxy(0, 3);
	printf("%-18.18s", date_str);
	gotoxy(0, 4);
	cursor( ON );
}

//------------------------------------------------------------------
//	change_character
//  ============================
//	Changes character to next greater value.
//------------------------------------------------------------------
void change_character( int x, int y, char* time_str, char* date_str )
{
	char c;
	char mask_c;

	c = ( y == 2) ? time_str[x]:date_str[x];
	mask_c = ( y == 2)? time_mask[x]:date_mask[x];

	if ( ++c > mask_c )
		c = '0';

	if ( y == 2 )
		time_str[x] = c;
	else
		date_str[x] = c;
}

//------------------------------------------------------------------
//	store_time_date
//  ============================
//	saves time and date.
//------------------------------------------------------------------
void store_time_date( char* time_str, char* date_str )
{
	char temp[ 14 + 1];
	struct date dates;
	struct time times;

	strcpy(temp, time_str);
	temp[10]= '\0';
	times.ti_sec = atoi(temp + 6);
	temp[5] = '\0';
	times.ti_min = atoi(temp + 3);
	temp[2] = '\0';
	times.ti_hour = atoi(temp);
	settime( &times );

	strcpy(temp, date_str);
	dates.da_year = atoi(temp + 6);
	temp[5] = '\0';
	dates.da_mon = atoi(temp + 3);
	temp[2] = '\0';
	dates.da_day = atoi(temp);
	setdate( &dates );
}

//------------------------------------------------------------------
//	is_date_string_ok
//  ============================
//	Checks date string.
//------------------------------------------------------------------
int is_date_string_ok( char* date_str )
{
	char temp[ 14 + 1];
	struct date dates;

	strcpy(temp, date_str);
	temp[10]= '\0';
	dates.da_year = atoi(temp + 6);
	temp[5] = '\0';
	dates.da_mon = atoi(temp + 3);
	temp[2] = '\0';
	dates.da_day = atoi(temp);
	return( ((checkdate( &dates )==OK)?0:2) );
}

//------------------------------------------------------------------
//	is_time_string_ok
//  ============================
//	Checks time string.
//------------------------------------------------------------------
int is_time_string_ok( char* time_str )
{
	char temp[ 8 + 1];
	struct time times;
	strcpy(temp, time_str);
	times.ti_sec = atoi(temp + 6);
	temp[5] = '\0';
	times.ti_min = atoi(temp + 3);
	temp[2] = '\0';
	times.ti_hour = atoi(temp);
	return( ((checktime( &times )==OK)?0:1) );
}

//------------------------------------------------------------------
//	setstartpos
//  ============================
//	Initializes x and y for time date editting.
//------------------------------------------------------------------
void setstartpos( int *x, int *y )
{
	*x = 0;
	*y = 2;
}

//------------------------------------------------------------------
//	setcursorpos
//  ============================
//	Sets the cursor position.
//------------------------------------------------------------------
int setcursorpos( int* x, int* y, int direction )
{
	if ( (*x) == -1 )
	{
		(*y) = ( (*y) == 2)?3:2;
		(*x) = (((*y) == 2)? strlen( time_mask):strlen( date_mask))-1;
	}
	if ( *y == 2 )
	{
		for (;;)
		{
			switch ( time_mask[ (*x) ] )
			{
			case '\0':
			if ( ++(*y) == 4 )
				(*y) = 2;
			(*x) = 0;
			break;
			case ':':
				(*x) += direction;
				break;
			default:
				gotoxy( (*x), (*y));
				return( 1 );
			}
		}
	} else if ( *y == 3 )
	{
		for (;;)
		{
			switch ( date_mask[ (*x) ] )
			{
			case '\0':
			if ( ++(*y) == 4 )
				(*y) = 2;
			(*x) = 0;
			break;
			case '/':
				(*x) += direction;
				break;
			default:
				gotoxy( (*x), (*y));
				return( 1 );
			}
		}
	}
	return( 0 );

}

//------------------------------------------------------------------
//	remove_record
//  ============================
//	Deletes the specified data record from the file.
//------------------------------------------------------------------
void remove_record( long  record_no )
{
	static char readed_rec[ SZ_RECORD +1 ];
	long rc;
	long records;
	int fd;

	// not a valid record number
	if ( record_no == -1L )
		return;

	// remove one record from database
	if ( (records = fsize( DATA_BASE )) == -1L)
		return; // database does not exist
	records /= SZ_RECORD; //
	if ( records == 1L )
	{
		remove( (char*) DATA_BASE );    // only 1 record in DB
		return;
	}

	fd = open( (char*) DATA_BASE, O_RDWR | O_TRUNC );

	if ( record_no == (records-1L))
	{
		// last record in database needs to be removed
		// the file can be trunced easy way to make file
		// smaller
		chsize( fd, (records-1L)*SZ_RECORD );
		close(fd);
		return;
	}

	for ( rc = record_no+1L; rc < records; rc++ )
	{
		lseek( fd, rc*SZ_RECORD, SEEK_SET );
		read( fd, readed_rec, SZ_RECORD );

		// one record back;
		lseek( fd, (rc-1L)*SZ_RECORD, SEEK_SET );
		write( fd, readed_rec, SZ_RECORD );
	}
	chsize( fd, (records-1L)*SZ_RECORD );
	close( fd );
}

//------------------------------------------------------------------
//	update_quantity_rec_struct
//  ============================
//	Updates the quantity, time and date stamp and GPS coordinates for the record.
//------------------------------------------------------------------
void update_quantity_rec_struct( db_record *db_rec, long quantity )
{
	struct date d;
	struct time t;

	getdate( &d );
	gettime( &t );

	sprintf( db_rec->quantity, "%*ld", SZ_SIGN + SZ_QUANTITY, quantity );
	// sprintf( db_rec->time, "%02d:%02d:%02d", t.ti_hour, t.ti_min, t.ti_sec );
	// sprintf( db_rec->date, "%02d/%02d/%04d", d.da_day, d.da_mon, d.da_year );

	// update GPS cordinates
	if (scanner.GPS_Model == 1)
	{
		// save latitude and longitude
		// strcpy(db_rec->latitude, g_Latitude);
		// strcpy(db_rec->longitude,  g_Longitude);
	}
}

//------------------------------------------------------------------
//	create_db_rec_struct
//  ============================
//	Fills in a data record with the specified barcode string and quantity.
//	Updates time, date and GPS coordinates.
//------------------------------------------------------------------
void create_db_rec_struct( db_record *db_rec, char* barcode, long quantity )
{
	struct date d;
	struct time t;

	getdate( &d );
	gettime( &t );

	memset(db_rec->barcode, 0, SZ_BARCODE);
	memcpy(db_rec->barcode, barcode, strlen(barcode)+1);
	sprintf( db_rec->quantity, "%*ld", SZ_SIGN + SZ_QUANTITY, quantity );
}

//------------------------------------------------------------------
//	create_db_rec_struct
//  ============================
// insertion sort is a very fast
// sorting function for very small changes
// even faster for small changes then the
// heap sort algorithem
//------------------------------------------------------------------
void insertion_sort( FILE *pFile, long items )
{
	long i, j;
	static char buffer[SZ_RECORD + 1];
	static char temp[SZ_RECORD + 1];
	int nReadCount = 0;
	int test;

	for (i = 1; i < items; i++)
	{
		test = fseek(pFile, i*SZ_RECORD, SEEK_SET);
		nReadCount = fread(temp, 1, SZ_RECORD, pFile );
		if(nReadCount == SZ_RECORD)
		{
			j = i;
			while (j > 0)
			{
				fseek( pFile, (j-1L)*SZ_RECORD, SEEK_SET);
				fread( buffer, 1, SZ_RECORD, pFile);
				test = memcmp( buffer, temp, SZ_BARCODE);
				if ( test <= 0)
					break;

				fseek(pFile, j*(long)SZ_RECORD,SEEK_SET);
				fwrite(buffer, 1, SZ_RECORD, pFile);
				j--;
			}
			if(j < i)
			{
				fseek(pFile, j*(long)SZ_RECORD, SEEK_SET );
				fwrite(temp, 1, SZ_RECORD, pFile);
			}
		}
	}
}

//------------------------------------------------------------------
//	save_data
//  ============================
//	Saves the data record at the specified position.
//------------------------------------------------------------------
void save_data( db_record *db_rec, long pos )
{
	FILE * pFile;
	static char record[ SZ_RECORD + 1 ];
	int  nSeekLoc = 0;
	unsigned char fBreadCrumbeEnabled = FALSE;

	if ( coreleft() < 5000L )
	{
		errbeep();
		show_message("RAM disk    full!       Data is NOT storedin database.", MB_ALL | MB_ICONWARNING );
		return;
	}
	if ( coreleft() < 7000L )
	{
		errbeep();
		show_message("RAM disk    almost full!Data is stored    in database.", MB_ALL | MB_ICONINFORMATION );
	}

	// record with GPS
	sprintf(record, "%-*.*s,%-*.*s\r\n",
			SZ_BARCODE, SZ_BARCODE, db_rec->barcode,
			SZ_SIGN+SZ_QUANTITY, SZ_SIGN+SZ_QUANTITY, db_rec->quantity );

	// suspend GPS bread crumb
	if(g_BreadCrumbFlag == TRUE)
	{
		fBreadCrumbeEnabled = TRUE;
		g_BreadCrumbFlag = FALSE;
	}

	if(fsize(DATA_BASE) == -1L)
	{
		pFile = fopen( (char*) DATA_BASE, "ab+");
	}
	else
	{
		pFile = fopen( (char*) DATA_BASE, "rb+");
	}

	if (pFile == NULL)
	{
		errbeep();
		show_message("Error while opening file", MB_ALL | MB_ICONERROR );
		// resume GPS service
		if(fBreadCrumbeEnabled == TRUE)
		{
			g_BreadCrumbFlag = TRUE;
		}
		return;
	}
	if ( pos == -1L )
	{
		// append the file
		nSeekLoc = fseek( pFile, 0L, SEEK_END );
		if ((nSeekLoc = fwrite( record, 1, SZ_RECORD, pFile )) != SZ_RECORD )
		{
			errbeep();
			show_message("Error while writing datato the database!", MB_ALL | MB_ICONERROR );
		}
	}
	else
	{
		nSeekLoc = fseek( pFile, pos * SZ_RECORD, SEEK_SET );
		if ((nSeekLoc = fwrite( record, 1, SZ_RECORD, pFile )) != SZ_RECORD )
		{
			errbeep();
			show_message("Error while writing datato the database!", MB_ALL | MB_ICONERROR );
		}
	}

	fclose( pFile );
	// sort if quantity is required
	if ( (pos == -1L) && (scanner.QTY_Required == QTY_REQUIRED))
	{
		// sort the database
		// reopn the file for the sort
		pFile = fopen( (char*) DATA_BASE, "rb+");
		insertion_sort( pFile, 1+(fsize( DATA_BASE ) / SZ_RECORD) );
		fclose(pFile);
	}

	// resume GPS service
	if(fBreadCrumbeEnabled == TRUE)
	{
		g_BreadCrumbFlag = TRUE;
	}

	return;
}

//------------------------------------------------------------------
//	get_qty_no_keyboard
//  ============================
//	Quantity entry when no keypad available handler.
//------------------------------------------------------------------
int get_qty_no_keyboard(char *qty)
{
	//get the qty from 3-button models that do not have a keypad
	int c,x,i,key;



	setfont( MEDIUM_FONT, 0 );
	cursor(NOWRAP);

	printf("\f\tr  Enter Quantity  \tr");
	gotoxy(0,4);
	printf("\tr UP   Done   Move \tr");
	gotoxy(9,2);

	memset (qty, '\0', SZ_INPUT_QUANT );

	for (x=0; x < SZ_INPUT_QUANT; x++)
	{
		if (((qty[x]>'9') || (qty[x]<'0')))
		{
			memset(qty+x,'.',SZ_INPUT_QUANT-x);
			break;
		}
	}


	x=0;
	for (;;)
	{
		gotoxy(0,2);

		printf("Add QTY: ");

		for (i=0;i < SZ_INPUT_QUANT;i++)
		{
			if (i == x)
				printf("\tr%c\tr",qty[i]);
			else
				printf("%c",qty[i]);
		}
		printf("\n");

		key = KAwaitkey(battery_type);
		switch (c=key)
		{
		case UP_KEY:
			if (qty[x] == '.')
				qty[x]='0';
			else if (qty[x] == '9')
				qty[x]='.';
			else
				qty[x]++;
			break;
		case DOWN_KEY:
			if (qty[x] != '.')     //move to next digit only if the previous one is already filled
			{
				x++;
				if (x==SZ_INPUT_QUANT)
					x=0;
			}
			break;
		case TRIGGER_KEY:
			for (x=0;x<SZ_INPUT_QUANT;x++)    //remove unused digits
				if (qty[x] == '.')
					qty[x]='\0';


			if (strlen(qty) < 1)
				strcpy(qty,"1");            //default value

			return(0);
		}
	}
	return(0);
}

//------------------------------------------------------------------
//	setbaudrate
//  ============================
//	Set comm port baud rate according to specified number.
//------------------------------------------------------------------
void setbaudrate( long baudrate )
{
	switch ( baudrate )
	{
	case ID_115200:
		systemsetting("SZ");
		break;
	case ID_57600:
		systemsetting("K9");
		break;
	case ID_38400:
		systemsetting("K8");
		break;
	case ID_19200:
		systemsetting("K7");
		break;
	case ID_9600:
		systemsetting("K6");
		break;
	case ID_4800:
		systemsetting("K5");
		break;
	case ID_2400:
		systemsetting("K4");
		break;
	default:
		baudrate_enabled = ID_115200;
		systemsetting("SZ");
		break;
	}
}

//------------------------------------------------------------------
//	display_scan_quantity
//  ============================
//	Display screen for barcode scanning when quantity is accumulated.
//------------------------------------------------------------------
void display_scan_quantity( db_record *db_rec, long total )
{
	set_font_input();
	gotoxy(0,0);
	clear_screen();
	printf("\tr   SCAN BARCODE   \tr");
	gotoxy(0,1);
	printf(db_rec->barcode);
	gotoxy(0,2);
	printf("Total: %*ld ", SZ_SIGN+SZ_QUANTITY, total );

	gotoxy(0,3);
	printf("Add               " );
	gotoxy(0,4);
	printf("               ");

	// print the special keys bar
	printsign(0, 7, 6 ); // back
	set_font_input();
}

//------------------------------------------------------------------
//	display_scan_barcode
//  ============================
//	Display screen for barcode scanning when quantity is not being accumulated.
//------------------------------------------------------------------
void display_scan_barcode( db_record *db_rec )
{
	set_font_input();
	clear_screen();
	printf("\tr   SCAN BARCODE   \tr");

	gotoxy(0,1);                    // 2 display barcode on line 3

	printf(db_rec->barcode);

	//raise line 4 by 7 pix then display Total QTY on line 4
	if ( db_rec->barcode[0] == '\0' )
	{
		gotoxy(0,3);
		printf("%-18.18s", "  ");     // clear the line
	} else
	{
		gotoxy(0,3);
		printf("Total: %-*.*s", SZ_SIGN+SZ_QUANTITY, SZ_SIGN+SZ_QUANTITY, db_rec->quantity );
	}


	// print the special keys bar
	print_message_bar( BACK_SCAN );

	if ( db_rec->barcode[0] != '\0' )
	{
		print_message_bar( EXIT_OK_OPTIONS );     //EXIT_SCAN_OPTIONS bar
	}
	set_font_input();
}

//------------------------------------------------------------------
//	memory_full_error
//  ============================
//	Display low memory warning screen.
//------------------------------------------------------------------
void memory_full_error(void)
{
	errbeep();
	clear_screen();
	setfont(LARGE_FONT, NULL);
	gotoxy(0,0);
	printf("  LOW MEMORY  ");
	gotoxy(0,1);
	printf("PLEASE UPLOAD");
	gotoxy(0,2);
	printf("& DELETE FILE");
	print_message_bar(MB_ALL);
	resetkey();
	KAwaitkey( battery_type );

}

//------------------------------------------------------------------
//	gps_tag
//  ============================
//	System GPS real-time time and coordinate tag config setting handler.
//------------------------------------------------------------------
void gps_tag(void)
{
	int result;

	result = show_message("Do you want to enable   GPS tags for real-time scans?", MB_YESNO | MB_ICONQUESTION );
	if(result == YES)
	{
		g_gpsOptions |= GPS_TAG_REAL_TIME;
		okbeep();
	}
	else
	{
		g_gpsOptions &= ~GPS_TAG_REAL_TIME;
		errbeep();
	}
}

//------------------------------------------------------------------
//	LoadMenuSettings
//  ============================
//	Populate structure with current menu settings.
//------------------------------------------------------------------
void LoadMenuSettings(menuSettings * menu)
{
	menu->enabled_barcodes	= enabled_barcodes;
	menu->baudrate_enabled	= baudrate_enabled;
	menu->protocol_enabled	= protocol_enabled;
	menu->standby_enabled	= standby_enabled;
	menu->backlight_enabled	= backlight_enabled;
	menu->interface_enabled = interface_enabled;
	menu->gps_bread_crumbs	= gps_bread_crumbs;
	menu->gps_data_format	= gps_data_format;
	menu->time_zone_offset	= time_zone_offset;
	menu->quantity_required = scanner.QTY_Required;
	menu->gps_options		= g_gpsOptions;
	menu->blt_setting		= app.blt_interface;

}

//------------------------------------------------------------------
//	IsEqualMenuSettings
//  ============================
//	Compare current menu settings against submitted structure.
//------------------------------------------------------------------
int IsEqualMenuSettings(menuSettings * menu)
{
	int retVal = FALSE;

	if(menu->enabled_barcodes != enabled_barcodes)
	{
		retVal = TRUE;
	}
	if(menu->baudrate_enabled != baudrate_enabled)
	{
		retVal = TRUE;
	}
	if(menu->protocol_enabled != protocol_enabled)
	{
		retVal = TRUE;
	}
	if(menu->standby_enabled != standby_enabled)
	{
		retVal = TRUE;
	}
	if(menu->backlight_enabled != backlight_enabled)
	{
		retVal = TRUE;
	}
	if(menu->interface_enabled != interface_enabled)
	{
		retVal = TRUE;
	}
	if(menu->gps_bread_crumbs != gps_bread_crumbs)
	{
		retVal = TRUE;
	}
	if(menu->gps_data_format != gps_data_format)
	{
		retVal = TRUE;
	}
	if(menu->time_zone_offset != time_zone_offset)
	{
		retVal = TRUE;
	}
	if(menu->quantity_required != scanner.QTY_Required)
	{
		retVal = TRUE;
	}
	if(menu->gps_options != g_gpsOptions)
	{
		retVal = TRUE;
	}
	if(menu->blt_setting != app.blt_interface)
	{
		retVal = TRUE;
	}

	return retVal;
}

//------------------------------------------------------------------
//	CompareMenuSettings
//  ============================
//	Menu settings changed handler.
//------------------------------------------------------------------
void CompareMenuSettings(menuSettings * menu)
{
	int result = FALSE;
	menuSettings currentMenu;

	LoadMenuSettings(&currentMenu);
	if(IsEqualMenuSettings(menu) == TRUE)
	{
		result = show_message("Settings    changed. Do you want to save?", MB_YESNO | MB_ICONQUESTION );
		if(result == YES)
		{
			SaveAppSettings();
		}
	}
}
