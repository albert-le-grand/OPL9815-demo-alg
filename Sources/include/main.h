/*
 * demo.h
 *
 *  Created on: Mar 7, 2012
 *      Author: kogami
 */

#ifndef MAIN_H_
#define MAIN_H_

#define MAX_BUFFER	100
#define DATA_BASE	"A:\\DATA.TXT"


//define for models

#define OPL9728           28
#define OPL9712           12
#define OPL9723           23
#define OPL9724           24
#define OPL9713           13
#define OPL9715           25
#define DCL1530           30
#define DCL1531           31
#define OPL9815			  15
#define PX36			  36

#define BAT_CHARGE         1      //OPL
#define BAT_NO_CHARGE      0      //DCL

//define for terminal features
//interface options
#define INTERFACE_CRADLE                1
#define INTERFACE_BT_BATCH    	    	2
#define INTERFACE_BT_REAL_TIME    		3
#define INTERFACE_USB                   4

//QTY selection
#define QTY_REQUIRED      1
#define QTY_NOT_REQUIRED  0
#define QTY_ENTER_ONLY    2

//Mac Address options
#define DISCOVERY_MAC 2
#define SCAN_MAC 3
#define KEY_MAC  4

// baud rates
#define ID_115200	1
#define ID_57600    2
#define ID_38400    3
#define ID_19200    4
#define ID_9600     5
#define ID_4800     6
#define ID_2400     7


//UMB Implementation
#define SYMBOLOGY_UPCA	        0x01
#define SYMBOLOGY_UPCA_2        0x02
#define SYMBOLOGY_UPCA_5        0x03
#define SYMBOLOGY_UPCE	        0x04
#define SYMBOLOGY_UPCE_2	    0x05
#define SYMBOLOGY_UPCE_5	    0x06
#define SYMBOLOGY_EAN13	        0x07
#define SYMBOLOGY_EAN13_2 	    0x08
#define SYMBOLOGY_EAN13_5     	0x09
#define SYMBOLOGY_EAN8	        0x0A
#define SYMBOLOGY_EAN8_2	    0x0B
#define SYMBOLOGY_EAN8_5	    0x0C
#define SYMBOLOGY_CODE39        0x0D
#define SYMBOLOGY_CODABAR       0x0E
#define SYMBOLOGY_I2OF5	        0x0F
#define SYMBOLOGY_D2OF5         0x10
#define SYMBOLOGY_M2OF5	        0x11
#define SYMBOLOGY_DTL2OF5       0x12
#define SYMBOLOGY_IATA          0x13
#define SYMBOLOGY_CODE93        0x14
#define SYMBOLOGY_CODE128       0x15
#define SYMBOLOGY_MSI_PLESSEY   0x16
#define SYMBOLOGY_UK_PLESSEY    0x17
#define SYMBOLOGY_TELEPEN       0x18
#define SYMBOLOGY_SCODE         0x1A
#define SYMBOLOGY_LAETUS	    0x1B
#define SYMBOLOGY_C39_FA	    0x1C
#define SYMBOLOGY_C39_IT	    0x1D
#define SYMBOLOGY_NW7_ABC		0x1E
#define SYMBOLOGY_NW7_CX		0x1F
#define SYMBOLOGY_CHINPOST		0x20
#define SYMBOLOGY_RSS_14          0x21
#define SYMBOLOGY_RSS_LIMITED     0x22
#define SYMBOLOGY_RSS_EXPANDED    0x23

#define SYMBOLOGY_PDF417          0x24
#define SYMBOLOGY_CCC_COMPOSITE   0x25

#define SYMBOLOGY_MICRO_PDF417    0x26
#define SYMBOLOGY_CCA_COMPOSITE   0x27
#define SYMBOLOGY_CCB_COMPOSITE   0x28

#define MAX_SYMBOLOGIES       SYMBOLOGY_CCB_COMPOSITE
#define MENU_SET_LABEL        "ZZ"

#define MAX_PREFIX_LENGTH       20
#define MAX_SUFFIX_LENGTH       20
#define DEFAULT_SUFFIX        "\r\n"

typedef struct
{
    char prefix[MAX_PREFIX_LENGTH + 1];
    char suffix[MAX_SUFFIX_LENGTH + 1];
}Param;

/* GoodRead */

#define FOREVER             -1

//bluetooth
#define BT_MASTER    1
#define BT_SLAVE     2
#define MAX_ADDRESS  12
#define MASTER_COM 		COM3
#define SLAVE_COM 		COM5
#define HID_MASTER_COM	COM13
#define HID_SLAVE_COM	COM12
#define MAX_PINCODE  16
#define BLT_PIN_CALLBACK_INACTIVE	0
#define BLT_PIN_CALLBACK_WAITING	1
#define BLT_PIN_CALLBACK_DONE		2

//flash programming
/****************************************************************************/
#define FLASH_BLOCK_NUMBER      7               // Do NOT change this value!!!
#define FLASH_ADDRESS           0xC70000UL;     // Do NOT change this value!!!
#define CHECK_STRING            "SPLASH_OK"
/****************************************************************************/

// Defines for app.blt_interface
#define BLT_VCP_DEVICE					0x00
#define BLT_HID_DEVICE					0x01

// Defines for app.trigger_to_connect_options
#define TRIGGER_TO_CONNECTABLE			0x01		// If set, trigger to connect acts as 'trigger to make connectable'

// Defines for app.connect_to
#define CONNECT_TO_MASK					0x03
#define CONNECT_TO_PC					0x00
#define CONNECT_TO_CRADLE				0x01
#define CONNECT_TO_CRADLE_HID			0x02

// Defines for app.memorizing
#define MEMORIZING_ENABLED				0x01
#define MEMORIZING_AFTER_CONNECT_LOSS	0x00
#define MEMORIZING_WHEN_NOT_CONNECTED	0x04

// Defines for app.read_time
#define READ_TIME_INDEFINITELY			0xFE		// Read time indefinitely

// Defines for app.buzzer_options
#define BUZZER_ENABLED					0x01
#define STARTUP_BUZZER					0x02
#define BUZZER_TONE_SINGLE				0x00
#define BUZZER_TONE_HIGH_LOW			0x10
#define BUZZER_TONE_LOW_HIGH			0x20
#define BUZZER_TONE_MASK				0x30

// Defines for app.serial_options
#define RS_CONFIG_DISABLED   			0x01
#define RS_BUZZER_DISABLED   			0x02
#define RS_TRIGGER_DISABLED  			0x04
#define RS_GR_LED_DISABLED   			0x08
#define RS_ACK_NAK_ON		 			0x10
#define RS_LASER_DISABLED    			0x20
#define RS_MOTOR_DISABLED    			0x80

// Defines for app.ser_cmd_format
#define CMD_TERM_CR    					0x01      // Serial command terminator: CR
#define CMD_TERM_LF    					0x02      // Serial command terminator: LF
#define CMD_TERM_ETX   					0x04      // Serial command terminator: ETX
#define CMD_TERM_EOT   					0x08      // Serial command terminator: EOT
#define CMD_TERM_MASK  					0x0F
#define CMD_HEADER_ESC 					0x10      // Serial command header: ESC
#define CMD_HEADER_STX 					0x20      // Serial command header: STX
#define CMD_HEADER_MASK					0x30

// Defines for app.options
#define ENABLE_KEYBOARD_TOGGLE			0x01
#define ENABLE_WAKEUP_KEY				0x02
#define USB_HID_MODE					0x04
#define USB_VCP_MODE					0x08

// Verification string used by 'LoadApplicationSettings' to if any possible application settings
// found in flash memory are compatible with this application. If this version check fails only change this version if the application settings of the previous version aren't
// all application settings will be set back to default
#define	VERSION_CHECK					"BLT_DEMOv1.4"

// defines for the messages
#define MB_ALL		1         //-Press Any Key - bar
#define MB_YESNO	2	      //YES_NO bar

#define MB_ICONWARNING			0x10
#define MB_ICONERROR			0x20
#define MB_ICONQUESTION			0x40
#define MB_ICONINFORMATION		0x80
#define SZ_INPUT_QUANT	8     // max 9999 or neg max -9999 original = 4

#define SZ_BARCODE	200
#define SZ_SIGN		1
#define SZ_QUANTITY	8
#define SZ_RECORD	( SZ_BARCODE +1+ SZ_SIGN+ SZ_QUANTITY+1 )

// Return values of ProcessBarcode
#define NORMAL_BARCODE		0				// A normal barcode label was read
#define MENU_LABEL			1				// A normal menu label was read
#define CONNECT_SAME		2				// A valid Bluetooth address label was read that matches the address that was currently stored in flash
#define CONNECT_NEW			3				// A valid Bluetooth address label was read that was different than the address that was currently stored in flash
#define DISCONNECT_LABEL	4				// A +-DISC-+ label was read
#define CONNECT_LABEL		5				// A +-CONN-+ label was read
#define CONNECTABLE_LABEL	6				// A +-DSCO-+ label was read
#define CONFIG_IPHONE_LABEL 7				// A +-IOSON-+ or +-IOSOFF-+ lable was read

// Bluetooth address / PIN code defines
#define BLT_BD_ADDR_LEN		12				// Length of a Bluetooth address (label)
#define BLT_MAX_PIN_LEN		16				// Maximum length of a Bluetooth PIN code (label)

// Defines and variables used for the various timers used by this application

#define MAX_TIMERS				9
#define CONNECT_MASTER			0		// Timer used for 'Trigger to connect' time
#define MAKE_CONNECTABLE		1		// Timer used for 'Trigger to make connectable' time
#define CONNECTING_TIME_OUT		2		// Timer used for time-out mechanism on a connection attempt
#define CONNECTABLE_TIME_OUT	3		// Timer used for time-out mechanism when connectable
#define DISCONNECT				4		// Timer used for 'Trigger to disconnect' time
#define BLUE_FLASH_SLOW		    5		// Timer used for slow blinking blue connection LED (indicating an active connection)
#define PIN_REQUEST_TIME_OUT	6		// Timer used for simulating the menu book beeping sound
#define PIN_REQUEST_MENU_BEEP	7		// Timer used for simulating the menu book beeping sound when entering a PIN code
#define GPS_TIMER_POLL			8		// Timer used to poll GPS radio

#define TRIGGER_TO_CONNECT_TIME			  5			// Trigger to connect time: 5 seconds (using the trigger key)
#define TRIGGER_TO_CONNECTABLE_TIME		  5			// Trigger to make connectable time: 5 seconds (using the clear key)
#define TRIGGER_TO_DISCONNECT_TIME		  5			// Trigger to disconnect time: 5 seconds (using the clear key)
#define RECONNECT_WAIT_TIME			      5			// Time till between reconnects attempt: 5 seconds
#define RECONNECT_ATTEMPTS			      8			// Default number of reconnect attempts after a connection loss: 8 ( 8 x (10+5) = 120 seconds)
#define CONNECTABLE_TIME			     12			// Time-out when connectable: 120 seconds (stored in 10s steps)
#define CONNECTING_TIME				     10			// Time-out on a connecting attempt: 10 seconds
#define RECONNECTING_TIME			     10			// Time-out on a reconnecting attempt: 10 seconds
#define GPS_POLL_TIME					 25			// GPS poll time: 0.5 seconds

#define BLUE_FLASH_SLOW_TIME		   5*50			// Slow blinking blue connection LED: blink every 5 seconds
#define IDLE_POWER_DOWN_TIME		   3*50			// Powerdown time when idle: 3 seconds
#define CONNECTED_POWER_DOWN_TIME	  10*50			// Powerdown (sleep) time when connected: 10 seconds

#define PIN_REQUEST_TIME			   30			// Max. time allowed to enter the pin code (30 seconds)
#define PIN_REQUEST_MENU_BEEP_TIME	   1*50			// Time between beeps of the menu book beeping sound
//#define MAX_PINCODE						6

//
// States of the main state machine of this application
//
#define STATE_DISCONNECTED		 0		// Idle state: No Bluetooth activity
#define STATE_CONNECTING		 1		// Connecting state: Trying to connect (as master)
#define STATE_CONNECTABLE		 2		// Connectable state: Waiting for a connection (as slave) & discoverable
#define STATE_CONNECTED_MASTER	 3		// Connected (as master) state
#define STATE_CONNECTED_SLAVE	 4		// Connected (as slave) state
#define STATE_WAIT_TO_RECONNECT	 5		// Wait to reconnect state: After a connection loss, waits until reconnect time has passed and then tries to reconnect
#define STATE_WAIT_FOR_RELEASE	 6		// Wait state: Waits until both keys are not pressed and then jumps back idle state
// Character to go to the next line depends on the interface
#define NEXT_LINE	(comopen(-1) == COM10 || comopen(-1) == COM12 || comopen(-1) == COM13) ? KB_ENT_KEY : '\r'

// bluetooth state change
#define STATE_TO_SAME			 0 		// no state change
#define STATE_TO_DISCOVERABLE    1		// go to discoverable state
#define STATE_TO_DISCONNECT      2		// go to disconnect state
#define STATE_TO_RECONNECT		 3		// go to reconnect state
#define STATE_TO_CONNECT_AS_MASTER	4	// go to connect as master

#define BT_ABORT_OPERATION		 -1

// interface options
#define INTERFACE_NETO			0x1000
#define INTERFACE_NO_PROTOCOL	0x2000
#define INTERFACE_REAL_TIME		0x0100
#define INTERFACE_BATCH			0x0200
#define INTERFACE_BT_SLAVE		0x0010
#define INTERFACE_BT_MASTER		0x0020

// GPS options
#define GPS_NMEA_FORMAT			0x1000
#define GPS_DEC_DEG_FORMAT		0x2000
#define GPS_DMS_FORMAT			0x4000
#define GPS_TAG_REAL_TIME		0x0100
#define GPS_BRD_CRUMB_PERIOD_MASK	0x000F

//
void SaveAppSettings(void);
void LoadAppSettings(void);
int IsGPSTag(void);

//
// Defines a structure of configuration settings that is stored in non-volatile (flash) memory
// to make it possible that certain configurations aren't lost after a restart
//
typedef struct
{
	unsigned char blt_interface;							// Indicates the currently selected interface (HID or VCP)
	unsigned char blt_hid_manual_pin;						// If TRUE, the PIN code must be entered using numeric direct input labels (HID only, Menu book chapter 4.3.3)
	unsigned char trigger_to_connect_time;					// Trigger to connect time (stored in seconds, 0 = disabled)
	unsigned char trigger_to_connectable_time;				// Trigger to make connectable time (stored in seconds, 0 = disabled)
	unsigned char trigger_to_disconnect_time;				// Trigger to disconnect time (stored in seconds, 0 = disabled)
	unsigned char reconnect_attempts;						// Number of reconnect attempts after connection loss (0 = disabled)
	unsigned char connectable_time;							// Time that the device stays connectable (stored in 10s steps)
	unsigned char read_mode;								// Read mode
	unsigned char read_time;								// Read time (stored in 0,1s steps)
	unsigned char goodread_duration;						// Good read led duration (stored in 20ms steps)
	unsigned char buzzer_volume;							// Buzzer volume
	unsigned char buzzer_duration;							// Buzzer duration (stored in 40ms steps)
	unsigned char buzzer_options;							// Buzzer tone
	unsigned char ser_cmd_options;							// Serial command options (OSE serial commands allowed)
	unsigned char ser_cmd_format;							// Serial command format options
	unsigned char options;									// Additional options, for now only used for iOS 'enable keyboeard toggle flag'
	unsigned char interfaceOption;							// Interface options
	long  		  enabled_barcodes;
	char baudrate_enabled;
	char protocol_enabled;
	char standby_enabled;
	char backlight_enabled;
	char interface_enabled;
	char gps_bread_crumbs;
	char gps_data_format;
	char gps_options;
	char time_zone_offset;
	char quantity_required;
} application_vars;


extern application_vars app;

typedef struct                 //holds each scanner model info
{
    char  ModelName [ 7+1 ];
    int   ModelNumber;             //values OPL9828, OPL9712 etc
    int   BT_Model;                //1=BT model, 0=not a BT model
    int   BT_Master_Slave;         //values BT_MASTER, BT_SLAVE
    int   BT_Interface;            // INTERFACE_BT_BATCH or INTERFACE_BT_REAL_TIME
    char  BT_Address [MAX_ADDRESS+1];
    char  BT_PIN     [MAX_PINCODE+1];
    int   GPS_Model;               //1=GPS model, 0=not a GPS model
    int   Full_Keypad;              // 1 = full keypad, 0 = 3-buttons only
    long  QTY_Required;            //0 = No, 1 = Yes
    long  Data_Mode;               //values  DATA_BATCH or DATA_REALTIME
    long COM_Port;                 //values COM0(USB), COM2(Cradle), COM3 (BT Master), COM5(BT Slave)
    char Sys_menu_options[SYSTEM_MENU_SIZE+1];   //holds array of which system menu options will be displayed
                                   //1=display option, 0=do not display
    char Comm_options[5+1];        //options for Comm menu
    char Interface_options[7+1];   //holds options to be displayed in the interface menu. 1= display option, 0=don't display

}scannerType;

typedef struct                 //holds splash screen lines of text (3 lines, up to 14 chards each)
{
    char L1 [14+1];
    char L2 [14+1];
    char L3 [14+1];
}splash_screen;

void Bluetooth_VCP_Default(void);
void Bluetooth_HID_Default(void);
void Application_Default(void);
void BluetoothDefault(void);
int IsScannerGPS(void);
int conv_symbology(int symbo);
void DemoTickHandler(void);
void DisplayBluetoothMenu(void);
void DisplayBluetoothStatus(int BLState, int rowNo);
void get_device_info(scannerType* scanner);
void display_splash_screen(void);
void set_model_defaults(scannerType* scanner);
int UpdateBluetoothState(int *stateChange);
int IsScannerBluetooth(void);
void CloseBluetoothPorts(void);
void BltPINCallback(char *pin, int* len);

#endif /* DEMO_H_ */
