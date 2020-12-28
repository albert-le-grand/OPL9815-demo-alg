/*
 * menu.h
 *
 *  Created on: Mar 7, 2012
 *      Author: kogami
 */

#ifndef MENU_H_
#define MENU_H_

#include "gps.h"

// system menu constants
#define SYSTEM_MENU_SIZE		11
#define SYSTEM_MENU_BACK_INDEX	0
#define SYSTEM_MENU_BARC_INDEX	1
#define SYSTEM_MENU_COMM_INDEX	2
#define SYSTEM_MENU_QTY_INDEX	3
#define SYSTEM_MENU_BLT_INDEX	4
#define SYSTEM_MENU_BATT_INDEX	5
#define SYSTEM_MENU_MEM_INDEX	6
#define SYSTEM_MENU_TIME_INDEX	7
#define SYSTEM_MENU_STBY_INDEX	8
#define SYSTEM_MENU_LITE_INDEX	9
#define SYSTEM_MENU_GPS_INDEX	10

#define ERR_QUANTITY_MIN_MAX	-1	// exceeding min max value
#define ERR_QUANTITY_ILLEGAL 	-2	// illegal input (-) only a minus is an illegal input vale

#define OPTION_BAR	4
#define SCROLL_BAR	8
#define OPTION_BAR_NO_QTY 16        //KA option for no QTY option
#define EXIT_OK_OPTIONS   32        //KA Bar for EXIT OK OPTIONS
#define EXIT_SCAN         64        //KA bar for EXIT  SCAN - used for real time scanning
#define BACK_SCAN        128        // BACK SCAN - batch mode scanning


#define SELCTION_BIT( bit )		(1L<<bit)

#define ID_CODE39		SELCTION_BIT(0)
#define ID_EAN 			SELCTION_BIT(1)
#define ID_UPC 			SELCTION_BIT(2)
#define ID_I2OF5		SELCTION_BIT(3)
#define ID_D2OF5    	SELCTION_BIT(4)
#define ID_CODABAR		SELCTION_BIT(5)
#define ID_CODE93		SELCTION_BIT(6)
#define ID_CODE128		SELCTION_BIT(7)
#define ID_MSI_PLESSEY 	SELCTION_BIT(8)
#define ID_TELEPEN		SELCTION_BIT(9)
#define ID_UK_PLESSEY	SELCTION_BIT(10)
#define ID_IATA			SELCTION_BIT(11)
#define ID_SCODE		SELCTION_BIT(12)
#define ID_M2OF5		SELCTION_BIT(13)
#define ID_RSS_14		SELCTION_BIT(14)
#define ID_RSS_LIMITED	SELCTION_BIT(15)
#define ID_RSS_EXPANDED	SELCTION_BIT(16)
#define ID_PDF417		SELCTION_BIT(17)
#define ID_MICRO_PDF417	SELCTION_BIT(18)
#define ID_DATAMATRIX	SELCTION_BIT(19)
#define ID_AZTEC		SELCTION_BIT(20)
#define ID_AZTEC_RUNES	SELCTION_BIT(21)
#define ID_QR_CODE		SELCTION_BIT(22)
#define ID_MICRO_QR_CODE	SELCTION_BIT(23)
#define ID_MAXICODE			SELCTION_BIT(24)

#define ID_ADDONS		SELCTION_BIT(25)

#define OFF_PDF417 		26
#define OFF_uPDF417		27
#define OFF_ADDON_2 	28
#define OFF_ADDON_2_128	34
#define OFF_ADDON_5 	35
#define OFF_ADDON_5_128	41
#define OFF_MNU_CDE		42

#define OPTION_BAR	4
#define SCROLL_BAR	8
#define OPTION_BAR_NO_QTY 16        //KA option for no QTY option
#define EXIT_OK_OPTIONS   32        //KA Bar for EXIT OK OPTIONS
#define EXIT_SCAN         64        //KA bar for EXIT  SCAN - used for real time scanning
#define BACK_SCAN        128        // BACK SCAN - batch mode scanning

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
#define SZ_DATE		10
#define SZ_TIME		8
#define SZ_LONGITUDE LONGITUDE_DMS
#define SZ_LATITUDE  LATITUDE_DMS
#define SZ_RECORD	( SZ_BARCODE +1+ SZ_SIGN+ SZ_QUANTITY+1+SZ_DATE+1+SZ_TIME+2+SZ_LATITUDE+1+SZ_LONGITUDE+1 )

#define ID_115200	1
#define ID_57600    2
#define ID_38400    3
#define ID_19200    4
#define ID_9600     5
#define ID_4800     6
#define ID_2400     7

#define ST_05_SEC	1
#define ST_10_SEC	2
#define ST_15_SEC	3
#define ST_30_SEC	4
#define ST_45_SEC	5
#define ST_60_SEC	6
#define ST_120_SEC	7
#define ST_180_SEC	8

#define ID_NO_PROTOCOL		1
#define ID_NETO_PROTOCOL	2
#define ID_XMODEM_PROTOCOL	3
#define ID_OSECOMM_PROTOCOL	4

#define GPS_BRD_OFF     0
#define GPS_BRD_1_M     1
#define GPS_BRD_5_M     5
#define GPS_BRD_15_M    15
#define GPS_BRD_30_M    30

// these defines leave out the wierd pacific island time zones
#define TIME_ZONE_MINIMUM -11
#define TIME_ZONE_MAXIMUM  11

struct smenus
{
    char *item;
    int value;
};

typedef struct
{
    char *item;
}days;

typedef struct
{
	long enabled_barcodes;
	long baudrate_enabled;
	long protocol_enabled;
	long standby_enabled;
	long backlight_enabled;
	long interface_enabled;
	long gps_bread_crumbs;
	long gps_data_format;
	long gps_options;
	long time_zone_offset;
	long quantity_required;
	char blt_setting;
}menuSettings;

typedef struct
{
    char barcode[ SZ_BARCODE +1 ];
    char quantity[ SZ_SIGN+ SZ_QUANTITY+1 ];
    char time[ SZ_TIME+1 ];
    char date[ SZ_DATE+1 ];
    char longitude[LONGITUDE_DMS+1];
    char latitude[LATITUDE_DMS+1];
}db_record;



void InitMenu(void);
void MainMenu(void);
long GetLastRecord(int rec_size);
void LoadMenuSettings(menuSettings * menu);
void CompareMenuSettings(menuSettings * menu);
int IsEqualMenuSettings(menuSettings * menu);
void main_scan( void );
void save_data( db_record *db_rec, long pos );


void printsign( int x_start, int y, int signid );

void main_delete( void );
void printbar( int y, int type, int LeftRight );
void print_message_bar( int bartype  );
void print_msg_icons( int x, int y, int icons );
void fill_record_struct( db_record *db_rec, char* record );
long string_quantity_to_long( char* quantity, int* illegal );
void display_splash_screen(void);                          // display splash screen
void display_splash_screen_no_waitkey(void);			   // display startup splash screen 
void qty_menu(void);                                       // displays QTY sel Menu
void sys_bluetooth_reset(void);
int BT_address_setup(void);
int getMACAddress(int MAC_MODE);
int Discovery(char *address);
int verifyBTAddress(char * BTAddress);

int init_connection();
int close_connection();
int init_master_connection();
void display_master_connection_screen(void);
void display_slave_connection_screen(void);
long get_active_port(void);
int init_slave_connection();
int check_connection( void );
void InputPincode(char *pincode);
void display_BT_error(int err);
int send_barcode_real_time( char* barcode );
int check_valid_BT_address(void);

void get_master_slave_mode(void);
void process_menu_code(char *barcode);
int get_qty_no_keyboard(char *qty);
void bluetooth_menu(void);
int GetStringLength(char * pszlabel, int nMaxLength);

void main_scroll(void);
void system_menu(void);
void main_send(void);
void OnOseComInfo(int status, int errorsuccess, int progress, const char *info);
void main_version(void);
void gps_menu( void );
void gps_time_zone_menu(void);
void gps_format_menu(void);
void gps_brd_menu(void);
void gps_test(void);
void save_config( void );
void send_Z3(void);
void sys_barcodes(void);
void sys_datetime(void);
void sys_backlight(void);
void sys_standby(void);
void sys_memory(void);
void set_standby( void );
void display_mem(void);
void set_barcodes( void );
void disp_no_database( void );
int get_record( db_record *db_rec, long *rec_nr, long *max_rec );
void fill_record_struct( db_record *db_rec, char* record );
void display_scroll_data( db_record *db_rec, long curr, long max, int infoType );
void printbattery( void );
int show_message( const char* msg, int icons );
void display_BT_error(int err);
void set_time_and_date(void);
void display_change_quantity( db_record *db_rec );
void display_barcode_options( db_record *db_rec );
long bin_search(char *fname, char *record, char *sstring, int sstr_len, int start, int rec_size);
int check_input_quant( char* input_quant, int use_default, long def_value, long fnd_quantity, long *new_quantity );
void show_options_menu( db_record *db_rec );
void change_quantity( db_record* db_rec );
void print_msg_icons( int x, int y, int icons );
int send_plain( void );
int send_neto( void );
int send_xmodem(void);
int send_unformatted(void);
void display_time_date_error( int err_code );
int display_do_setting( void );
void display_time_date( char* time_str, char* date_str );
void change_character( int x, int y, char* time_str, char* date_str );
void store_time_date( char* time_str, char* date_str );
int is_date_string_ok( char* date_str );
int is_time_string_ok( char* time_str );
void setstartpos( int *x, int *y );
int setcursorpos( int* x, int* y, int direction );
void remove_record( long  record_no );
void update_quantity_rec_struct( db_record *db_rec, long quantity );
void create_db_rec_struct( db_record *db_rec, char* barcode, long quantity );
void insertion_sort( FILE *pFile, long items );
void SetDefault(void);
int get_qty_no_keyboard(char *qty);
void setbaudrate( long baudrate );
void display_scan_barcode( db_record *db_rec );
void display_scan_quantity( db_record *db_rec, long total );
void memory_full_error(void);
void interface_menu( void );
void get_BT_interface(void);
int check_flash_PIN_code(void);
int check_flash_BT_address(void);
void protocol_menu( void );
void comm_menu( void );
void baudrate_menu( void );
void bat_test( void );
void ConnectBluetooth(void);
void SetPINCode(void);
void SetMACAddress(void);
void InputMACAddress(char *address);
void DisconnectBluetooth(void);
void SetBluetoothMode();
void gps_tag(void);
void DefaultConfig(void);

#endif /* MENU_H_ */
