#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "lib.h"
#include "gps.h"
#include "menu.h"
#include "main.h"
#include "utils.h"

// Global variables
application_vars app;			// Structure of configuration settings that is stored in non-volatile (flash) memory
int reset_appl_to_default;
int reset_read_mode;

extern int batt_power;
extern int battery_msg_status;

scannerType scanner;              //KA	  - struc to hold scanner info

char buffer[100];
int g_TickCounter;


int BTState;
int BTStateChange;
int com_opened;
int attempts_left;
int pressed;

splash_screen splash_msg;

volatile unsigned int timer[MAX_TIMERS];			// Array of various timers

char BTLocalName[32] = "";							//

char g_termID[7];

// GPS breadcrumb timer
extern int g_GPS_breadcrumbTimer;
extern int g_GPS_Test_Mode;
// breadcrumb record string name + placeholder for seven digit bread crumb number
extern int g_nBreadCrumbIndex;
extern unsigned long g_nGPSStandbyTimeout;   // GPS standby timeout as configured by system menu

extern long gps_bread_crumbs;
extern long gps_data_format;
extern long time_zone_offset;
extern long time_zone_change;
extern int g_util_timer;
extern int g_util_timeout;
extern int g_gps_mode;
extern int g_BreadCrumbTime;
extern int g_BreadCrumbFlag;
extern int g_gpsOptions;

extern int g_fSuspendGPS;

extern long enabled_barcodes;
extern long baudrate_enabled;
extern long protocol_enabled;
extern long standby_enabled;
extern long backlight_enabled;
extern long interface_enabled;
extern long gps_bread_crumbs;
extern long gps_data_format;
extern long time_zone_offset;
extern long time_zone_change;

extern int battery_type;

extern char app_version[];

extern int g_bltPINCallback;


//------------------------------------------------------------------
//  main
//  ============================
//  Main program entry
//------------------------------------------------------------------
void main(void)
{
	int status;
	long termID;
	char * btLocalAdd;
	char serialNo[10];

	// enable charge indicator
	systemsetting("7G");

	get_device_info(&scanner);
    set_model_defaults(&scanner);
	if( scanner.ModelNumber == PX36 )
		display_splash_screen_no_waitkey();
	
    if (scanner.GPS_Model == 1)
    {
        g_BreadCrumbTime = 0;
        g_BreadCrumbFlag = FALSE;
        // initialize the menu variable
        gps_bread_crumbs = g_BreadCrumbTime;
        gps_data_format = GPS_RAW;
        g_gps_mode = 1;
        g_GPS_Test_Mode = 0;
        g_nBreadCrumbIndex = 0;
        g_fSuspendGPS = FALSE;
        time_zone_offset = -7;
        time_zone_change = FALSE;
    }
    else
    {
        // make sure the timer is disabled
        g_util_timer = 0;
        g_util_timeout = 0;
        // disable gps polling during waitkey
        g_gps_mode = 0;
    }
    flipscreen( AUTO );

    status = ApplicationSettingsMemorizing(ON,(void*) &app, sizeof(application_vars),(char *) app_version);

    if(status < 0)
    {
        time_zone_offset = -7;
	    Bluetooth_VCP_Default();
		Application_Default();
		SaveAppSettings();
    }
    else
    {
    	// restore settings from flash
    	LoadAppSettings();
		set_barcodes();
		
    }

    btLocalAdd = (char*)GetSerial();
    strcpy(serialNo, btLocalAdd);
    termID = atol(serialNo);

    if(termID > 999999)
	{
		while(termID > 1000000)
		{
			termID -= 1000000;
		}
	}
	sprintf(g_termID, "%06ld", termID);

	// build the default bluetooth local name
	if(scanner.BT_Model)
	{
		strcpy(BTLocalName, scanner.ModelName );
		strcat(BTLocalName, "_" );
		strncat(BTLocalName, &g_termID[2], 4);
		SetBltLocalName( BTLocalName );
	}

	g_TickCounter = 0;

	InstallHeartbeatHandler((unsigned long)DemoTickHandler);

	g_fSuspendGPS = FALSE;

    // initialize timers
    InitializeMyTimer();

    if (scanner.GPS_Model == 1)
    {
        // start up GPS
        GPSStart();
        // initialize the menu variable
        if (gps_bread_crumbs != GPS_BRD_OFF)
        {
			g_BreadCrumbTime = gps_bread_crumbs;
			g_GPS_breadcrumbTimer = g_BreadCrumbTime;
            if (g_util_timer == 0)
            {
                g_util_timer = 1;
            }
            g_BreadCrumbFlag = TRUE;
            ResetBreadCrumbTimer();
        }
        else
        {
        	g_BreadCrumbFlag = FALSE;
        }
    }

	InitMenu();

	BTState = STATE_DISCONNECTED;
	com_opened = -1;

	if( scanner.BT_Model )
	{
		// start the Bluetooth update timer
		StartMyTimer(TIMER_ID_BT_STATE_UPDATE, 1, 1);
		g_gpsOptions = GPS_TAG_REAL_TIME;
		blt_set_pin_callback(BltPINCallback);
	}

	set_standby();              // set the standby time

	display_splash_screen();

	clear_screen();

	setfont(SMALL_FONT,0);
	status = 0;
	BTStateChange = STATE_TO_SAME;

	for(;;)
	{
		MainMenu();
		idle();
	}
}

//------------------------------------------------------------------
//	UpdateBluetoothState
//  ============================
//	Bluetooth stat machine.
//------------------------------------------------------------------
int UpdateBluetoothState(int *stateChange)
{
	int timer_no;
	int myTimerStatus;


	myTimerStatus = CheckMyTimer(TIMER_ID_BT_STATE_UPDATE);

	if(myTimerStatus != TIMER_STATUS_EXPIRED)
	{
		return(0);
	}
	else
	{
		ResetMyTimer(TIMER_ID_BT_STATE_UPDATE);
	}
	// Main Bluetooth connection state machine
	//
	switch(BTState)
	{
		case STATE_DISCONNECTED:			// Idle state (not connected)

			if(*stateChange == STATE_TO_CONNECT_AS_MASTER)
			{
				timer[MAKE_CONNECTABLE] = 0xFFFF;

				timer[CONNECT_MASTER] = 0xFFFF;

				if(memcmp(GetBltAddress(), "000000000000", BLT_BD_ADDR_LEN) != 0)	// If no bluetooth address is configured, there's no point in trying to connect
				{
					sound(TSTANDARD, VHIGH, SHIGH, SMEDIUM, 0);

//						goodreadled(ORANGE_FLASH, FOREVER);

					autopowerdown(OFF, 0);				// Don't turn off while connecting

					com_opened = (app.blt_interface == BLT_HID_DEVICE) ? COM13 : COM3;	// Connect as master as either VCP or HID

					if(comopen(com_opened) == OK)		// Start the connection attempt
					{
						BTState = STATE_CONNECTING;
						timer[CONNECTING_TIME_OUT] = CONNECTING_TIME * 50;
						attempts_left = 1;				// <- causes a 2nd connect attempt if the first failed
						*stateChange = STATE_TO_SAME;
					}
					else								// If Bluetooth initialization failed -> abort
					{
//							goodreadled(BLUE_FLASH, 0);
						comclose(com_opened);
						com_opened = -1;
						sound(TVLONG, VHIGH, SLOW, 0);
						BTState = STATE_WAIT_FOR_RELEASE;
					}
				}
				else
				{
					BTState = STATE_WAIT_FOR_RELEASE;
				}
				// keep from powering down
				PokeActivity();
			}
			else if(*stateChange == STATE_TO_DISCOVERABLE)
			{
				timer[MAKE_CONNECTABLE] = 0xFFFF;

				sound( TSTANDARD, VHIGH, SHIGH, SMEDIUM, 0);

//					goodreadled(ORANGE_FLASH, FOREVER);

				autopowerdown(OFF, 0);		// Don't turn off while waiting for a connection

				com_opened = (app.blt_interface == BLT_HID_DEVICE) ? COM12 : COM5;	// Make connectable as slave for either VCP or HID

				// if initialization fails, then just stay in this state and keep trying to change to connectibla
				if(comopen(com_opened) == OK)		// Make device connectable
				{
					BTState = STATE_CONNECTABLE;
					timer[CONNECTABLE_TIME_OUT] = (unsigned int)app.connectable_time * 10 * 50;
					*stateChange = STATE_TO_SAME;
				}
				// keep from powering down
				PokeActivity();
			}
			else
			{
				timer[MAKE_CONNECTABLE] = 0xFFFF;
				timer[CONNECT_MASTER] = 0xFFFF;
			}


			break;

		case STATE_CONNECTING:							// Wait for a connection to be established
		case STATE_CONNECTABLE:

			if(blt_is_connected() == TRUE)				// If Connection established -> go to connected state
			{
				sound(TVLONG, app.buzzer_volume, SHIGH, 0);
//				goodreadled(BLUE, 20);
//				timer[BLUE_FLASH_SLOW] = BLUE_FLASH_SLOW_TIME;
				goodreadled(GREEN, 20);
				BTState = (BTState == STATE_CONNECTING) ? STATE_CONNECTED_MASTER : STATE_CONNECTED_SLAVE;
				attempts_left = 0;
				pressed = FALSE; // no need to send a wake-up key when you've just connected (device is on already)
				autopowerdown(ON, g_nGPSStandbyTimeout);
//				autopowerdown(ON, CONNECTED_POWER_DOWN_TIME);
				if(*stateChange != STATE_TO_DISCONNECT)
				{
					*stateChange = STATE_TO_SAME;
				}
				break;
			}

			timer_no = (BTState == STATE_CONNECTING) ? CONNECTING_TIME_OUT : CONNECTABLE_TIME_OUT;

//				if(downpressed())					// Clear key can be used to abort the connection attempt
			if(*stateChange == STATE_TO_DISCONNECT)
			{
				timer[timer_no] = 0;		// Simulates that the connection attempt timed out
				attempts_left = 0;
				*stateChange = STATE_TO_SAME;
			}
			else
			{
				timer[DISCONNECT] = 0xFFFF;
			}

			if(timer[timer_no] == 0)			// If connection attempt has timed out -> abort
			{
				timer[DISCONNECT] = 0xFFFF;
				timer[timer_no] = 0xFFFF;
				comclose(com_opened);
				com_opened = -1;

				if(attempts_left && BTState == STATE_CONNECTING)
				{
//					goodreadled(BLUE_FLASH, -1);
//					goodreadled(ORANGE_FLASH, -1);
					BTState = STATE_WAIT_TO_RECONNECT;
					timer[CONNECTING_TIME_OUT] = RECONNECT_WAIT_TIME * 50;
				}
				else
				{
//					goodreadled(BLUE_FLASH, 0);
//					goodreadled(ORANGE_FLASH, 0);
					sound(TVLONG, app.buzzer_volume, SLOW, 0);
					BTState = STATE_WAIT_FOR_RELEASE;
				}
			}

			// keep from powering down
			PokeActivity();

			break;

		case STATE_CONNECTED_MASTER:			// Connected state
		case STATE_CONNECTED_SLAVE:

			if(blt_is_connected() == FALSE)		// If Bluetooth connection lost
			{
				timer[DISCONNECT] = 0xFFFF;
//				goodreadled(BLUE_FLASH, -1);
//				goodreadled(ORANGE_FLASH, -1);
				sound(TVLONG, app.buzzer_volume, SLOW, 0);
				autopowerdown(OFF, 0);			// Don't turn off while trying to reconnect or being connectable

				if(BTState == STATE_CONNECTED_MASTER || com_opened == COM12)		// If connected as master, then reconnect as master
				{																// If connected as HID device that was connected as slave (COM12), also reconnect as master (COM13)
					comclose(com_opened);
					com_opened = -1;

					if(app.reconnect_attempts)
					{
						BTState = STATE_WAIT_TO_RECONNECT;
						timer[CONNECTING_TIME_OUT] = RECONNECT_WAIT_TIME * 50;
						attempts_left = app.reconnect_attempts;
					}
					else
					{
//						goodreadled(BLUE_FLASH, 0);
//						goodreadled(ORANGE_FLASH, 0);
						BTState = STATE_WAIT_FOR_RELEASE;
					}
				}
				else															// Else -> stay connectable (as slave) and wait for an incomming connection
				{
					BTState = STATE_CONNECTABLE;
					timer[CONNECTABLE_TIME_OUT] = (unsigned int)app.reconnect_attempts * 15 * 50;
				}

				break;
			}

//			if(timer[BLUE_FLASH_SLOW] == 0xFFFF)								// Implementation of slow blinking blued LED as connection indicator
//			{																	// Note: Connected + sleep mode (powerdown) = no blinking blue LED
//				timer[BLUE_FLASH_SLOW] = BLUE_FLASH_SLOW_TIME;
//			}
//			else if(timer[BLUE_FLASH_SLOW] == 0)
//			{
//				timer[BLUE_FLASH_SLOW] = BLUE_FLASH_SLOW_TIME;
////				goodreadled(BLUE, 10);
//			}

			//
			// Wake up iPhones/iPads on trigger press (if enabled)
			//
			if( (app.options & ENABLE_WAKEUP_KEY) && !pressed && triggerpressed() )
			{
				pressed = TRUE;
//					putnstring("", 0);
			}
			else if(!triggerpressed())
			{
				pressed = FALSE;
			}
			
			if(*stateChange == STATE_TO_DISCONNECT)
			{
				timer[DISCONNECT] = 0xFFFF;
				sound( TVLONG, app.buzzer_volume, SHIGH, SLOW, 0 );
//					goodreadled(BLUE_FLASH, 0);
//					goodreadled(ORANGE_FLASH, 0);
				comclose(com_opened);
				com_opened = -1;
				BTState = STATE_WAIT_FOR_RELEASE;
				*stateChange = STATE_TO_SAME;
			}
			else
			{
				timer[DISCONNECT] = 0xFFFF;
			}

			break;

		case STATE_WAIT_TO_RECONNECT:

//				if(triggerpressed())		// Trigger key can be used to do an immediat reconnect attempt
			if(*stateChange == STATE_TO_RECONNECT)
			{
				timer[DISCONNECT] = 0xFFFF;

				timer[CONNECT_MASTER] = 0xFFFF;
				sound(TSTANDARD, app.buzzer_volume, SHIGH, SMEDIUM, 0);
				timer[CONNECTING_TIME_OUT] = 0;				// Forces the waiting time to reconnect to pass immediatly
				attempts_left = 0;
				*stateChange = STATE_TO_SAME;
			}
			else if(*stateChange == STATE_TO_DISCONNECT)
			{
				timer[CONNECT_MASTER] = 0xFFFF;

				*stateChange = STATE_TO_SAME;
				timer[DISCONNECT] = 0xFFFF;
				sound( TVLONG, app.buzzer_volume, SHIGH, SLOW, 0 );
//					goodreadled(BLUE_FLASH, 0);
//					comclose(COM3);
//					goodreadled(ORANGE_FLASH, 0);
				comclose(com_opened);
				BTState = STATE_WAIT_FOR_RELEASE;
				attempts_left = 0;
			}
			else
			{
				timer[CONNECT_MASTER] = 0xFFFF;
				timer[DISCONNECT] = 0xFFFF;
			}

			if(timer[CONNECTING_TIME_OUT] == 0)				// Waiting time to reconnect passed -> try to reconnect
			{
				timer[CONNECTING_TIME_OUT] = 0xFFFF;
				timer[CONNECT_MASTER] = 0xFFFF;
				timer[DISCONNECT] = 0xFFFF;

//				goodreadled(BLUE_FLASH, FOREVER);

				com_opened = (app.blt_interface == BLT_HID_DEVICE) ? COM13 : COM3;	// Connect as master as either VCP or HID

				if(comopen(com_opened) == OK)
				{
					BTState = STATE_CONNECTING;
					timer[CONNECTING_TIME_OUT] = RECONNECTING_TIME * 50;
					autopowerdown(OFF, 0);						// Don't turn off while trying to reconnect

					if(attempts_left)
						--attempts_left;
				}
				else
				{
//					goodreadled(BLUE_FLASH, 0);
//					goodreadled(ORANGE_FLASH, 0);
					comclose(com_opened);
					com_opened = -1;
					sound(TVLONG, app.buzzer_volume, SLOW, 0);
					BTState = STATE_WAIT_FOR_RELEASE;
					attempts_left = 0;
				}
			}

			break;

		case STATE_WAIT_FOR_RELEASE:		// Waits here until all keys are released before returning to idle state
		default:
//				if(!triggerpressed() && !downpressed())
			if((*stateChange == STATE_TO_SAME) || (*stateChange == STATE_TO_DISCONNECT))
			{
				BTState = STATE_DISCONNECTED;
//				autopowerdown(ON, IDLE_POWER_DOWN_TIME);
				autopowerdown(ON, g_nGPSStandbyTimeout);
			}
			break;
	}

	return BTState;
}

//------------------------------------------------------------------
//	IsScannerGPS
//  ============================
//	Checks if the scanner has GPS and it is actively being polled.
//------------------------------------------------------------------
int IsScannerGPS(void)
{
    return (scanner.GPS_Model == 1) && (g_fSuspendGPS == FALSE);
}

//------------------------------------------------------------------
//	IsScannerBluetooth
//  ============================
//	Checks if the scanner implements bluetooth.
//------------------------------------------------------------------
int IsScannerBluetooth(void)
{
	return scanner.BT_Model;
}

//------------------------------------------------------------------
//	conv_symbology
//  ============================
//	Table lookup to convert symbology type from decode to menu.
//------------------------------------------------------------------
int conv_symbology(int symbo)
{
    switch (symbo)
    {

    case MENU_CODE:
        return(MENU_CODE);

//    case JAN13:
    case EAN13:
//    case INSTORE13:
        return(SYMBOLOGY_EAN13);

    case UPCA:
        return(SYMBOLOGY_UPCA);

//    case JAN8:
    case EAN8:
//    case INSTORE8:
        return(SYMBOLOGY_EAN8);

    case UPCE:
        return(SYMBOLOGY_EAN8);

    case CODE39:
        return(SYMBOLOGY_CODE39);

    case CODABAR:
        return(SYMBOLOGY_CODABAR);

    case D2OF5:
        return(SYMBOLOGY_D2OF5);

    case I2OF5:
        return(SYMBOLOGY_I2OF5);

    case CODE93:
        return(SYMBOLOGY_CODE93);

    case CODE128:
    case CODE128_2:
    case CODE128_5:
        return(SYMBOLOGY_CODE128);

    case MSI_PLESSEY:
        return(SYMBOLOGY_MSI_PLESSEY);

    case IATA:
        return(SYMBOLOGY_IATA);

    case UK_PLESSEY:
        return(SYMBOLOGY_UK_PLESSEY);

    case TELEPEN:
        return(SYMBOLOGY_TELEPEN);

    case M2OF5:
        return(SYMBOLOGY_M2OF5);

    case CHINPOST:
        return(SYMBOLOGY_CHINPOST);

    case NW7_ABC:
        return(SYMBOLOGY_NW7_ABC);

    case NW7_CX:
        return(SYMBOLOGY_NW7_CX);

    case SCODE:
        return(SYMBOLOGY_SCODE);

    case LAETUS:
        return(SYMBOLOGY_LAETUS);

    case C39_FA:
        return(SYMBOLOGY_C39_FA);
    case C39_IT:
        return(SYMBOLOGY_C39_IT);

//    case JAN13_2:
    case EAN13_2:
        return(SYMBOLOGY_EAN13_2);

    case UPCA_2:
        return(SYMBOLOGY_UPCA_2);

//    case JAN8_2:
    case EAN8_2:
        return(SYMBOLOGY_EAN8_2);

    case UPCE_2:
        return(SYMBOLOGY_UPCE_2);

//    case JAN13_5:
    case EAN13_5:
        return(SYMBOLOGY_EAN13_5);

    case UPCA_5:
        return(SYMBOLOGY_UPCA_5);

//    case JAN8_5:
    case EAN8_5:
        return(SYMBOLOGY_EAN8_5);

    case UPCE_5:
        return(SYMBOLOGY_UPCE_5);

    case RSS_14:
        return(SYMBOLOGY_RSS_14);

    case RSS_LIMITED:
        return(SYMBOLOGY_RSS_LIMITED);

    case RSS_EXPANDED:
        return(SYMBOLOGY_RSS_EXPANDED);

    case PDF417:
        return(SYMBOLOGY_PDF417);

    case CCC_COMPOSITE:
        return(SYMBOLOGY_CCC_COMPOSITE);

    case MICRO_PDF417:
        return(SYMBOLOGY_MICRO_PDF417);

    case CCA_COMPOSITE:
        return(SYMBOLOGY_CCA_COMPOSITE);

    case CCB_COMPOSITE:
        return(SYMBOLOGY_CCB_COMPOSITE);

    }
    return(SYMBOLOGY_CODE39);  // keep compiler happy //
}

//------------------------------------------------------------------
//	get_device_info
//  ============================
//	this function determines the scanner mode based on OS 3-letter prefix.
//------------------------------------------------------------------
void get_device_info(scannerType* scanner)
{
    static char os[8+1];
    version_os( os );

    if (strncmp(os,"LBD",3) == 0)
        scanner->ModelNumber =OPL9728;
    else if (strncmp(os,"LBO",3) == 0)
        scanner->ModelNumber = OPL9712;
    else if (strncmp(os,"LBB",3) == 0)
        scanner->ModelNumber = OPL9723;
    else if (strncmp(os,"LBC",3) == 0 || strncmp(os,"LBR",3) ==0)  //LBC -->Zeevo chip, LPR--> Taiyo Yuden chip
        scanner->ModelNumber = OPL9724;
    else if (strncmp(os,"LBK",3) == 0)
        scanner->ModelNumber = DCL1530;
    else if (strncmp(os,"LBN",3) == 0)
        scanner->ModelNumber = DCL1531;
    else if (strncmp(os,"LBP",3) == 0)
        scanner->ModelNumber = OPL9713;
    else if (strncmp(os,"LBW",3) == 0)
        scanner->ModelNumber = OPL9715;
    else if (strncmp(os,"XBO",3) == 0)
        scanner->ModelNumber = OPL9815;
    else if (strncmp(os,"XBT",3) == 0)
        scanner->ModelNumber = PX36;

	

}

//------------------------------------------------------------------
//	set_model_defaults
//  ============================
//	Sets the defaults for the scanner model.
//------------------------------------------------------------------
void set_model_defaults(scannerType* scanner)
{

    memset(scanner->Sys_menu_options,'\0',sizeof(scanner->Sys_menu_options));
    scanner->BT_Interface = -1;
    scanner->BT_Master_Slave = -1;

    switch (scanner->ModelNumber)
    {
    case OPL9728:
        strcpy(scanner->ModelName,"OPL9728");
        scanner->QTY_Required = QTY_REQUIRED;
        scanner->Full_Keypad =1;
        scanner->BT_Model = 0;
        scanner->GPS_Model = 0;

        strcpy(scanner->Sys_menu_options,"11110111110");      //no BT
        strcpy(scanner->Interface_options,"11000");
        protocol_enabled  = ID_NETO_PROTOCOL;
        interface_enabled = INTERFACE_CRADLE;
        baudrate_enabled = ID_115200;
        battery_type = BAT_CHARGE;

        break;

    case OPL9712:
        strcpy(scanner->ModelName,"OPL9712");
        scanner->QTY_Required = QTY_REQUIRED;
        scanner->Full_Keypad =1;
        scanner->BT_Model = 1;
        scanner->GPS_Model = 0;

        scanner->BT_Master_Slave = BT_MASTER;
        strcpy(scanner->Sys_menu_options,"11111111110");
        strcpy(scanner->Interface_options,"11110");
        protocol_enabled  = ID_NETO_PROTOCOL;
        baudrate_enabled = ID_115200;
        interface_enabled =  INTERFACE_CRADLE;
        battery_type = BAT_CHARGE;
        break;

    case OPL9723:
        strcpy(scanner->ModelName,"OPL9723");
        scanner->QTY_Required =0;
        scanner->Full_Keypad =0;
        scanner->GPS_Model = 0;

        scanner->BT_Model = 0;
        strcpy(scanner->Sys_menu_options,"11110111100");       //no BT, no back light
        strcpy(scanner->Interface_options,"11000");         //allow only Cradle
        protocol_enabled  = ID_NETO_PROTOCOL;
        baudrate_enabled = ID_115200;
        interface_enabled =  INTERFACE_CRADLE;
        battery_type = BAT_CHARGE;
        break;

    case OPL9724:
        strcpy(scanner->ModelName,"OPL9724");
        scanner->QTY_Required =0;
        scanner->Full_Keypad =0;
        scanner->BT_Model = 1;
        scanner->GPS_Model = 0;

        strcpy(scanner->Sys_menu_options,"11111111100");     //no backlight
        strcpy(scanner->Interface_options,"11110");          //no USB
        scanner->COM_Port = COM2;
        interface_enabled =  INTERFACE_CRADLE;
        battery_type = BAT_CHARGE;
        break;

    case DCL1530:
        strcpy(scanner->ModelName,"DCL1530");
        scanner->QTY_Required =0;
        scanner->Full_Keypad =0;
        scanner->BT_Model = 0;
        scanner->GPS_Model = 0;

        strcpy(scanner->Sys_menu_options,"11110111100");       //no BT, no back light
        strcpy(scanner->Interface_options,"11001");         //allow only Cradle  & USB
        scanner->COM_Port = COM2;                           //cradle
        interface_enabled =  INTERFACE_CRADLE;
        protocol_enabled  = ID_NETO_PROTOCOL;
        battery_type = BAT_NO_CHARGE;
        break;

    case DCL1531:
        strcpy(scanner->ModelName,"DCL1531");
        scanner->QTY_Required = QTY_REQUIRED;
        scanner->Full_Keypad =1;
        scanner->BT_Model = 0;
        scanner->GPS_Model = 0;

        strcpy(scanner->Sys_menu_options,"11110111100");       //no BT, no back light
        strcpy(scanner->Interface_options,"11001");         //allow only Cradle  & USB
        scanner->COM_Port = COM2;                           //cradle
        interface_enabled =  INTERFACE_CRADLE;
        protocol_enabled  = ID_NETO_PROTOCOL;
        battery_type = BAT_NO_CHARGE;
        break;

    case OPL9715:
        strcpy(scanner->ModelName,"OPL9715");
        scanner->QTY_Required = QTY_NOT_REQUIRED;
        scanner->Full_Keypad =1;
        scanner->BT_Model = 0;
        scanner->GPS_Model = 1;

        strcpy(scanner->Sys_menu_options,"11110111111");      //no BT
        strcpy(scanner->Interface_options,"11000");

        interface_enabled =  INTERFACE_CRADLE;
        battery_type = BAT_CHARGE;
        break;

    case OPL9815:
        strcpy(scanner->ModelName,"OPL9815");
        scanner->QTY_Required = QTY_NOT_REQUIRED;
        scanner->Full_Keypad =1;
        scanner->BT_Model = 1;
        scanner->GPS_Model = 1;

        strcpy(scanner->Sys_menu_options,"11111111111");
        strcpy(scanner->Interface_options,"11110");

        interface_enabled =  INTERFACE_CRADLE;
        battery_type = BAT_CHARGE;
        SetBltLocalName(BTLocalName);
        break;

   case PX36:
        strcpy(scanner->ModelName,"PX36");
        scanner->QTY_Required = QTY_NOT_REQUIRED;
        scanner->Full_Keypad =1;
        scanner->BT_Model = 1;
        scanner->GPS_Model = 0;

        strcpy(scanner->Sys_menu_options,"11111111110");  // No GPS
        strcpy(scanner->Interface_options,"11110");

        interface_enabled = INTERFACE_CRADLE;
        battery_type = BAT_CHARGE;
        SetBltLocalName(BTLocalName);
        break;		
		
    default:
        strcpy(scanner->ModelName,"OPLxxxx");
        scanner->QTY_Required =0;
        scanner->Full_Keypad =0;
        scanner->GPS_Model = 0;
        strcpy(scanner->Sys_menu_options,"1111111111");
        strcpy(scanner->Interface_options,"11111");

        break;

    }
}

//------------------------------------------------------------------
//	display_splash_screen_no_waitkey
//  ============================
//	Displays the splash screen
//------------------------------------------------------------------
void display_splash_screen_no_waitkey(void)
{
    int x;    //determines the x position for each so the line will be cetralized

    strcpy(splash_msg.L1,"Opticon Inc.");
    strcpy(splash_msg.L2,"Demo");
    strcpy(splash_msg.L3,"Application");

    clear_screen();

    setfont( LARGE_FONT, NULL );
    x = (14 - strlen(splash_msg.L1))/2;

    gotoxy(x,0);
    printf("%s",splash_msg.L1);
    x = (14 - strlen(splash_msg.L2))/2;
    gotoxy(x,1);
    printf("%s",splash_msg.L2);
    x = (14- strlen(splash_msg.L3))/2;
    gotoxy(x,2);
    printf("%s",splash_msg.L3);
}



//------------------------------------------------------------------
//	display_splash_screen
//  ============================
//	Displays the splash screen
//------------------------------------------------------------------
void display_splash_screen(void)
{
    int x;    //determines the x position for each so the line will be cetralized

    strcpy(splash_msg.L1,"Opticon Inc.");
    strcpy(splash_msg.L2,"Demo");
    strcpy(splash_msg.L3,"Application");

    clear_screen();

    setfont( LARGE_FONT, NULL );
    x = (14 - (int)strlen(splash_msg.L1))/2;

    gotoxy(x,0);
    printf("%s",splash_msg.L1);
    x = (14 - (int)strlen(splash_msg.L2))/2;
    gotoxy(x,1);
    printf("%s",splash_msg.L2);
    x = (14- (int)strlen(splash_msg.L3))/2;
    gotoxy(x,2);
    printf("%s",splash_msg.L3);

    print_message_bar( MB_ALL );     //press any key bar

    resetkey();
    KAwaitkey( battery_type );
}

//------------------------------------------------------------------
//	DemoTickHandler
//  ============================
//	Installed for the heartbeat callback.
//------------------------------------------------------------------
void DemoTickHandler(void)
{
	// tick divided by 8 handler
	g_TickCounter++;
	if((g_TickCounter & 0x7) == 0)
	{
		// scaled tick count handler
	}
	for( int x = 0; x < MAX_TIMERS; x++ )
	{
		if( timer[x] != 0 && timer[x] != 0xFFFF )
			timer[x]--;
	}
}

//------------------------------------------------------------------
//	DisplayBluetoothMenu
//  ============================
//	Displays bluetooth menu.
//------------------------------------------------------------------
void DisplayBluetoothMenu(void)
{
	clear_screen();

    gotoxy(0,0);
    printf("1: Discoverable");
    gotoxy(0,1);
    printf("2: Disconnect");
    gotoxy(0,2);
    printf("3: Reconnect");
    gotoxy(0,3);
    printf("4: BL Info");
    gotoxy(0,4);
    printf(">");
}

//------------------------------------------------------------------
//  Application_Default
//  ============================
//  Reset all application specific settings to default
//------------------------------------------------------------------
void Application_Default(void)
{
	reset_appl_to_default = FALSE;

	app.blt_hid_manual_pin = TRUE;									// PIN code for HID must be entered using numeric direct input labels (Menu book chapter 4.3.3)
	app.trigger_to_connect_time = TRIGGER_TO_CONNECT_TIME;			// Trigger to connect time (5 seconds)
	app.trigger_to_connectable_time = TRIGGER_TO_CONNECTABLE_TIME;	// Trigger to make connectable time (5 seconds)
	app.trigger_to_disconnect_time = TRIGGER_TO_DISCONNECT_TIME;	// Trigger to disconnect time (5 seconds)
	app.reconnect_attempts = RECONNECT_ATTEMPTS;					// Default number of reconnect attempts after connection loss (4)
	app.connectable_time = CONNECTABLE_TIME;						// Time that the device stays connectable (120 seconds)

	app.read_mode = SINGLE;											// Single read
	app.read_time = 0;												// 2 seconds
	app.goodread_duration = 10;										// Good read led duration (200ms)
	app.buzzer_volume = VHIGH;										// Buzzer volume (high)
	app.buzzer_duration = TSTANDARD;								// Buzzer duration (200ms)
	app.buzzer_options = BUZZER_ENABLED | STARTUP_BUZZER;			// Buzzer enabled, startup buzzer enabled
	app.buzzer_options |= BUZZER_TONE_HIGH_LOW;						// Buzzer tone (High-low tone)
	app.ser_cmd_options = 0;										// Serial command options (OSE serial commands allowed)
	app.ser_cmd_format = CMD_HEADER_ESC | CMD_TERM_CR;				// Supported serial command format: ESC / CR
	app.ser_cmd_format |= CMD_HEADER_STX | CMD_TERM_ETX;			// Supported serial command format: STX / ETX

    enabled_barcodes = ID_CODE39 | ID_EAN | ID_UPC | ID_I2OF5 | ID_D2OF5 | ID_CODABAR;
    enabled_barcodes |= ID_CODE93 | ID_CODE128 | ID_MSI_PLESSEY | ID_TELEPEN;
    enabled_barcodes |= ID_UK_PLESSEY | ID_IATA | ID_SCODE | ID_M2OF5;
    enabled_barcodes |= ID_RSS_14 | ID_RSS_LIMITED | ID_RSS_EXPANDED | ID_PDF417 | ID_MICRO_PDF417;

    set_barcodes(); // set the enabled barcodes

    set_standby();

    //set defaults
    baudrate_enabled = (long)ID_115200;
    standby_enabled = (long)ST_180_SEC;     // 3 min on
    backlight_enabled = (long)AUTO;         // automatic backlight

	protocol_enabled	= (long)ID_NETO_PROTOCOL;
	interface_enabled 	= (long)INTERFACE_CRADLE;

							
}

//------------------------------------------------------------------
//  Bluetooth_VCP_Default
//  ============================
//  Reset all Bluetooth settings to default
//------------------------------------------------------------------
void Bluetooth_VCP_Default(void)
{
	systemsetting("YC");			// Enable menu-labels (as data)

	app.blt_interface = BLT_VCP_DEVICE;						// Select interface: VCP

	app.options &= (char)~(USB_VCP_MODE | USB_HID_MODE);			// Deselect USB-VCP and USB-HID transmission
	app.options |= (char)USB_VCP_MODE;		// Enable transmission of barcode data through USB-HID when not connected to Bluetooth

	systemsetting("RZ1M");								// Set suffix: <CR>


	//
	// Note: A full reset is deliberately not executed in menu-option functions Bluetooth_VCP_Default / Bluetooth_HID_Default, so please don't change this!
	//
	// This is done because the possibity to use 'systemsetting()' in a function that's indirectly called by 'systemsetting() itself is limited due to
	// a protection against recursion. Therefor it is not possible to configure complex menu option strings, i.e. pre- and suffixes.
	//
	reset_appl_to_default = TRUE;
}

//------------------------------------------------------------------
//  Bluetooth_HID_Default
//  ============================
//  Reset all Bluetooth settings to default
//------------------------------------------------------------------
void Bluetooth_HID_Default(void)
{
	systemsetting("YC");			// Enable menu-labels (as data)

	app.blt_interface = BLT_HID_DEVICE;						// Select interface

	app.options &= (char)~(USB_VCP_MODE | USB_HID_MODE);			// Deselect USB-VCP and USB-HID transmission
	app.options |= (char)USB_HID_MODE;		// Enable transmission of barcode data through USB-HID when not connected to Bluetooth

	systemsetting("RZ7I");								// Set suffix: Enter key

	reset_appl_to_default = TRUE;
}

//------------------------------------------------------------------
//  BluetoothDefault
//  ============================
//  Reset all Bluetooth settings to default depending on selected bluetooth mode
//------------------------------------------------------------------
void BluetoothDefault(void)
{
	if(app.blt_interface == BLT_VCP_DEVICE)
	{
		Bluetooth_VCP_Default();
	}
	else
	{
		Bluetooth_HID_Default();
	}
}

//------------------------------------------------------------------
//	DisplayBluetoothStatus
//  ============================
//	Displays bluetooth status.
//------------------------------------------------------------------
void DisplayBluetoothStatus(int BLState, int rowNo)
{
	gotoxy(0,rowNo);
	switch(BLState)
	{
		case STATE_DISCONNECTED:
			printf("Disconnected");
			break;
		case STATE_CONNECTING:
			printf("Connecting  ");
			break;
		case STATE_CONNECTABLE:
			printf("Connectable  ");
			break;
		case STATE_CONNECTED_MASTER:
			printf("Master Conn  ");
			break;
		case STATE_CONNECTED_SLAVE:
			printf("Slave Conn  ");
			break;
		case STATE_WAIT_TO_RECONNECT:
			printf("Reconn Wait  ");
			break;
		case STATE_WAIT_FOR_RELEASE:
			printf("Rel Wait   ");
			break;
		default:
			printf("Undefined  ");
			break;
	}
}

//------------------------------------------------------------------
//	CloseBluetoothPorts
//  ============================
//	Close all bluettoth ports.
//------------------------------------------------------------------
void CloseBluetoothPorts(void)
{
	comclose(COM3);		// close SPP master
	comclose(COM5);		// close SPP slave
	comclose(COM12);		// close HID master
	comclose(COM13);		// close HID slave
}

//------------------------------------------------------------------
//	SaveAppSettings
//  ============================
//	Saves application settings from the menu.
//------------------------------------------------------------------
void SaveAppSettings(void)
{
	app.enabled_barcodes	= enabled_barcodes;
	app.baudrate_enabled	= (char)baudrate_enabled;
	app.protocol_enabled	= (char)protocol_enabled;
	app.standby_enabled		= (char)standby_enabled;
	app.backlight_enabled	= (char)backlight_enabled;
	app.interface_enabled 	= (char)interface_enabled;
	app.gps_bread_crumbs	= (char)gps_bread_crumbs;
	app.gps_data_format		= (char)gps_data_format;
	app.time_zone_offset	= (char)time_zone_offset;
	app.quantity_required   = (char)(scanner.QTY_Required);
	app.gps_options			= (char)g_gpsOptions;
	systemsetting("Z2");
	info_sound();
}

//------------------------------------------------------------------
//	IsGPSTag
//  ============================
//	Checks if GPS tag is desired for real-time bar code transmission.
//------------------------------------------------------------------
int IsGPSTag(void)
{
	int retVal = FALSE;

	if((g_gpsOptions & GPS_TAG_REAL_TIME) == GPS_TAG_REAL_TIME)
	{
		retVal = TRUE;
	}

	return retVal;
}

//------------------------------------------------------------------
//	LoadAppSettings
//  ============================
//	Loads application settings for use by the menu.
//------------------------------------------------------------------
void LoadAppSettings(void)
{
	char negtime;

	enabled_barcodes	= app.enabled_barcodes;
	baudrate_enabled	= (long)app.baudrate_enabled;
	protocol_enabled	= (long)app.protocol_enabled;
	standby_enabled		= (long)app.standby_enabled;
	backlight_enabled	= (long)app.backlight_enabled;
	interface_enabled 	= (long)app.interface_enabled;
	gps_bread_crumbs	= (long)app.gps_bread_crumbs;
	gps_data_format		= (long)app.gps_data_format;
	g_gpsOptions		= (long)app.gps_options;
	scanner.QTY_Required = (long)app.quantity_required;
	memcpy(scanner.BT_PIN, GetBltPin(), MAX_PINCODE+1);
	// do this conversion since the casting is not working and time_zone_offset is signed

	if(app.time_zone_offset > 128)
	{
		// take the 2's complement
		negtime = (char)(~app.time_zone_offset + 1);
		time_zone_offset = -1 * negtime;
	}
	else
	{
		time_zone_offset = (long)app.time_zone_offset;
	}
}

void BltPINCallback(char *pin, int* len)
{
	char ch;
	char bltPIN[MAX_PINCODE+1];
	if( g_bltPINCallback == BLT_PIN_CALLBACK_WAITING  && pin == NULL )
		g_bltPINCallback = BLT_PIN_CALLBACK_DONE;

	g_bltPINCallback = BLT_PIN_CALLBACK_WAITING;
	if( *len == 0 && (strnlen( pin, 6 ) != 0 ) )   // Wea are master so other side should confirm! so just show pin and wait
 	{
		printf( "\f\t""    PIN.   \t");
		printf( "\r%6.6s", pin );
		//printf( "\rPress any key");
	}else											
	if( *len == 6 && (strnlen( pin, 6 ) != 0) ) 	// We are slave, show pin and ask to confirm 
	{
		printf( "\f\t"" CONFIRM PIN! \t");
		printf( "\n%6.6s", pin );
		printf( "\nENT = OK");
		ch = WaitKey();
		
		if( ch == ENT_KEY )
			*len = 1;    // confirm number on display is equal to number on pc.
		else
			*len = 0;
	}else											// OLD non ssp device..
	{
	InputPincode(bltPIN);
	strcpy(pin, bltPIN);
	*len = strlen(pin);
	}
	g_bltPINCallback = BLT_PIN_CALLBACK_DONE;
}

