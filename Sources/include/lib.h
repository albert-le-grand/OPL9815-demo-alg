/***********************************************************************
 *
 *  Copyright (C) Opticon Sensors Europe B.V.
 *
 ***********************************************************************/

#ifndef __LIB_H__
#define __LIB_H__

#include <unistd.h>
#include <fcntl.h>


/*
 * Read write support
 */

#ifndef O_BINARY
    #define O_BINARY        0x00    /*   binary file                 */
#endif

#ifndef O_RDONLY
#define O_RDONLY        0x01    /*   open for read only          */
#define O_WRONLY        0x02    /*   open for write only         */
#define O_RDWR          0x03    /*   open for read and write     */
#define O_APPEND        0x08    /*   writes performed at EOF     */
#define O_CREAT         0x10    /*   create file                 */
#define O_TRUNC         0x20    /*   truncate file               */
#define O_FORM          0x40    /*   text file                   */
#define O_BINARY        0x80    /*   binary file                 */
#endif


/*
 * General definitions
 */
#define     OK          0
#define     ERROR       -1
#define     NG          100
#define     ON          1
#define     OFF         0
#define     YES         1
#define     NO          0
#define     TRUE        1
#define     FALSE       0
#define     NONE		0xFF
#define		DISABLED	-2
#ifndef 	NULL
#define     NULL		0
#endif
/*
 * ASCII definitions
 */
#define     SOH         0x01
#define     STX         0x02
#define     ETX         0x03
#define     EOT         0x04
#define     ENQ         0x05
#define     ACK         0x06
#define     BEL         0x07
#define     BS          0x08
#define     HT          0x09
#define     LF          0x0A
#define     VT          0x0B
#define     FF          0x0C
#define     CR          0x0D
#define     SO          0x0E
#define     SI          0x0F
#define     DLE         0x10
#define     DC1         0x11
#define     DC2         0x12
#define     DC3         0x13
#define     DC4         0x14
#define     NAK         0x15
#define     SYN         0x16
#define     ETB         0x17
#define     A_CAN       0x18
#define     EM          0x19
#define     SUB         0x1A
#define     ESC         0x1B
#define     A_FS        0x1C
#define     A_GS        0x1D
#define     A_RS        0x1E
#define     US          0x1F
#define     SPACE       0x20
#define     DEL         0x7F

#ifndef MAX
#define MAX(a, b) ((a)>(b)?(a):(b))
#endif

#ifndef MIN
#define MIN(a, b) ((a)>(b)?(b):(a))
#endif

#define MAX_FNAME (12 + 3 + 1)

/*
 * Constants for id member
 */
#define EAN13           0x01
#define UPCA            0x02
#define EAN8            0x03
#define UPCE            0x04
#define UPCE1			0x05
#define ISBN			0x06
#define ISSN			0x07
#define ISMN			0x08
#define CODE39          0x09
#define CODABAR         0x0A
#define NW7             CODABAR
#define D2OF5           0x0B                    // industrial 2 of 5
#define I2OF5           0x0C                    // interleaved 2 of 5
#define CODE93          0x0D
#define CODE128         0x0E
#define MSI_PLESSEY     0x0F
#define IATA            0x10
#define UK_PLESSEY      0x11
#define TELEPEN         0x12
#define M2OF5           0x13
#define CHINPOST        0x14
#define NW7_ABC         0x15
#define NW7_CX          0x16
#define SCODE           0x17
#define LAETUS          0x18
#define TRIOPTIC        LAETUS
#define C39_FA          0x19                    // Full ascii
#define C39_IT          0x1A                    // Italian pharma code
#define EAN128          0x1B
#define RSS_14          0x1C
#define RSS_LIMITED     0x1D
#define RSS_EXPANDED    0x1E
#define PDF417          0x1F
#define CCC_COMPOSITE   0x20
#define MICRO_PDF417    0x21
#define CCA_COMPOSITE   0x22
#define CCB_COMPOSITE   0x23
#define CODE11          0x24
#define EAN128_CCC      0x25                    // EAN-128 with CC-C composite
#define CODE3OF5        0x26                    // Code 3 of 5 A.K.A. Korean Postal Code
#define CODABLOCK_F		0x27

/*** ADD ON +2 ***/
#define ADD_ON_2        0x40
#define EAN13_2         0x41
#define UPCA_2          0x42
#define EAN8_2          0x43
#define UPCE_2          0x44
#define UPCE1_2         0x45
#define CODE128_2       0x4E                    // the same group CODE-128

/*** ADD ON +5 ***/
#define ADD_ON_5        0x80
#define EAN13_5         0x81
#define UPCA_5          0x82
#define EAN8_5          0x83
#define UPCE_5          0x84
#define UPCE1_5         0x85
#define CODE128_5       0x8E                    // the same group CODE-128
#define PARTIAL_READ	0x40
#define ADD_ON_MASK     0xC0
#define MENU_CODE       0xF0                    // OSE Code-39 Menu label
#define MENU_CODE_PDF   0xF1                    // OSE (u)PDF417 Menu label
#define MENU_CODE_C128  0xF2                    // OSE Code-128 Menu label
#define NO_READ         0xF3                    // Internal code-id for a No-read message

/*
 * Constants for ScannerPower
 */
#define TRIGGER             2
#define SINGLE              3
#define MULTIPLE            4
#define CONTINUOUS          5

/*
 * Constants for redundancy
 */
#define DIFFERENT       1
#define SAME            2
#define SUFFICIENT      3
#define NOT_SUFFICIENT  4


/*
 * Constants for AutoPowerDown
 */
#define  RESET      	0x02
#define  NOW        	0x03
#define  RESUME			0x04
#define  SUSPEND		0x05

/*
 * Constants for function 'GoodRead'
 */
#define RED             1
#define GREEN           2
#define ORANGE          RED|GREEN
#define RED_FLASH       4
#define GREEN_FLASH     8
#define ORANGE_FLASH    RED_FLASH|GREEN_FLASH
#define BLUE			0x10
#define BLUE_FLASH      0x20
#define WHITE          	RED|GREEN|BLUE
#define WHITE_FLASH   	RED_FLASH|GREEN_FLASH|BLUE_FLASH
#define FOREVER         -1

/*
 * definitions for sound volume level
 */
#define VOFF        0
#define VLOW        1
#define VSTANDARD   8
#define VMEDIUM     32
#define VHIGH       127

/*
 * definitions for sound tone duration
 */
#define TCLICK      1
#define TSHORT      3
#define TSTANDARD   5
#define TLONG       10
#define TVLONG      20


/*
 * definitions for sound tone
 */
#define SPAUSE      1
#define SERROR      100
#define SLOW        1000
#define SMEDIUM     2500
#define SHIGH       3500

/*
 * Key defines
 */
#define TRIGGER_KEY     0x80
#define OK_KEY     		TRIGGER_KEY
#define UP_KEY          0x81
#define DOWN_KEY        0x82
#ifdef OPH1005
#define Q1_KEY          UP_KEY
#define Q2_KEY          DOWN_KEY
#else
#define Q1_KEY          0x83
#define Q2_KEY          0x84
#endif
#define	ENT_KEY			0x85
#define CLR_KEY			0x86
#define ESC_KEY			0x1B
#define BS_KEY			0x87
#define LEFT_KEY		0x88
#define RIGHT_KEY		0x89
#define SHIFT_KEY		0x8A

#define F1_KEY          0x90
#define F2_KEY          0x91
#define F3_KEY          0x92
#define F4_KEY          0x93
#define SHIFT_F1_KEY    0x94
#define SHIFT_F2_KEY    0x95
#define SHIFT_F3_KEY    0x96
#define SHIFT_F4_KEY    0x97
#define ALT_KEY			0x98
#define PWR_KEY         0xFF


/*
 * LCD defines
 */
#if defined OPH3000 || defined OPH1005
#define DISP_WIDTH              240
#define DISP_HEIGHT             320
#elif defined OPL9815 || defined OPL9813
#define DISP_WIDTH  		112
#define DISP_HEIGHT 		64
#else // H13+
#define DISP_WIDTH              176
#define DISP_HEIGHT             220
#endif

#define DEFAULT_BL_TIME         500             // 10 seconds (or the powerdown time if that is less)
#define DEFAULT_POFTIME         30
#define INVERT              	2           // setpixel / drawline

/* SetFont */
#define TINY_FONT           0x01
#define SMALL_FONT          0x02
#define MEDIUM_FONT         0x03
#define LARGE_FONT          0x04
#define HUGE_FONT    		0x05
#define USER_FONT           0x06
#define FILE_FONT           0x07

/* cursor */
#define BLINKING            2
#define AUTOWRAP            3
#define NOWRAP              4
#define DOICONS				5
#define NOICONS				6
#define CUR_SAVE			7
#define CUR_RESTORE			8
/* cursor color*/
#define CUR_COLOR_WHITE		0x10
#define CUR_COLOR_RED		0x20
#define CUR_COLOR_GREEN		0x30
#define CUR_COLOR_BLUE		0x40
#define CUR_COLOR_MAGENTA	0x50
#define CUR_COLOR_CYAN		0x60
#define CUR_COLOR_YELLOW	0x70
#define CUR_COLOR_BLACK		0x80

/* DisplayDirection */
#define DISP_NORMAL         0
#define DISP_REVERSE        1

/* backlight / AutoPowerDown / flipscreen */
#define OFF                 0
#define ON                  1
#define TOGGLE              2
#define RESET_APD           3
#define AUTO                4
#define AP_NOW              5
#define AP_SUSPEND          6
#define RESUME_APD          7
#define RETURN_TIME         8

#define LOW_BL				1
#define MEDIUM_BL			2
#define HIGH_BL				3

//RGB values for some colors
// r = MSB
// g = Middle byte
// b = LSB
#define RGB_WHITE   0xFFFFFF
#define RGB_BLUE    0x0000FF
#define RGB_GREEN   0x00FF00
#define RGB_RED     0xFF0000
#define RGB_MAGENTA 0xFF00FF
#define RGB_CYAN    0x00FFFF
#define RGB_YELLOW  0xFFFF00
#define RGB_BLACK   0x000000

// BitBlt operations
#define WRITE_TO_SCREEN							0		// Just write all the pixels
#define TRANSPARENT								1		// Write all the colored pixels. The black pixels in the buffer remain unchanged on the screen
#define READ_BACK								2		// Only read the datat from the screen
#define MONOCHROME								3
#define WRITE_COLOR								4
#define MONOCHROME_PACKED						5
#define MONOCHROME_PACKED_TRANSPARANT_NO_SHADOW	6
#define MONOCHROME_PACKED_TRANSPARANT			7
#define REFRESH									8
#define MONOCHROME_NO_SHADOW					9
#define WRITE_TO_SCREEN_NO_SHADOW				10
#define TRANSPARENT_NO_SHADOW					11
#define WRITE_COLOR_NO_SHADOW					12
#define MONOCHROME_PACKED_NO_SHADOW				13

typedef void (*print_char_function)(unsigned char data);

/*
 * Com ports
 */
#define COM0                0		// USB-VCP (when connected to USB),  Cradle (when in cradle)
#define COM1                COM0
#define COM2                COM0
#define COM3                3		// Bluetooth SPP/Master
#define COM4				4		//
#define COM5                5		// Bluetooth SPP/Slave
#define COM9				9		// USB VCP (using COM0 is highly recommended to allow automatic downloading of software to stay operational)
#define COM10				10		// USB HID
#define COM11				11		// USB MSD
#define	COM12               12		// Bluetooth HID Slave
#define	COM13               13		// Bluetooth HID Master
/* ischarging */
#define OFF_CRADLE          0
#define CHARGING            1
#define ON_CRADLE           2           /* in cradle and charging is complete */
#define CHARGING_DISABLED   3           /* in cradle and not able to charge for various reasons */
/* (i.e. when using dry cell battery, temperature is out of range, charging process is initializing */

// GpsPower defines
#define GPS_ON		ON
#define GPS_OFF		OFF
#define GPS_SUSPEND 2
#define GPS_RESUME  3

/*
 * Dat & time structures
 */
struct time
{
    unsigned char ti_hour;
    unsigned char ti_min;
    unsigned char ti_sec;
};

struct date
{
    unsigned short  da_year;
    unsigned char da_day;
    unsigned char da_mon;
};

/*
 * Barcode structure
 */
struct barcode
{
    char *text;
    int length;
    int id;
    int min;
    int max;
};

/*
 * Bluetooth remote device info structure
 */
typedef struct
{
	char			bd_address[13];
	char			name[20];
	unsigned long 	address_l;
	unsigned long 	address_h;
	unsigned char 	page_scan_rpt_mode;
	unsigned char 	page_scan_period_mode;
	unsigned short 	clock_offset;
}device_info;


/*
 * GPS structure
 */
typedef struct
{
	struct time t;
	float latitude;
	char n_s;
	float longitude;
	char e_w;
	char fix;
	int satellites;
	float hdop;
	float altitude;
	char a_units;
	float separation;
	char s_units;
}gps;

/*
 *--------------------------------------
 *  Memory/file functions
 *--------------------------------------
 */
#define RAM_DISK            0
#define INTERNAL_DISK       1
#define DRIVE_A             RAM_DISK
#define DRIVE_C             INTERNAL_DISK

#define D_NAME              8
#define D_EXT               3

struct ffblk {
	char name[D_NAME + 1];
	char ext[D_EXT + 1];
	int filesize;
	char search_name[D_NAME + 1];
	char search_ext[D_EXT + 1];
	int slot;
	int drive;
};

/*
 * Neto
 */
#define ERR_USER_BREAK      -3
#define ERR_EOT_HOST        -5
#define ERR_HOST_REQ        -6
#define ERR_OPEN_FILE       -8
#define ERR_WRITE_FILE      -9
#define ERR_READ_FILE       -10
#define ERR_OUT_SYNC        -12
#define ERR_ALOC_MEM        -13
#define ERR_BCC_FAIL        -2          // checksum error (internal)
#define ERR_TIME_OUT        -4          // time out error (internal)
#define ERR_HOST_LOST       -7          // time out on first char (internal)
#define ERR_NAK_RECV        -11         // NAK received from host (internal)

/*
 * definitions for xmodem receive
 */
#define XMR_OK              0
#define XMR_CREATE_ERR      1
#define XMR_POLL_ERR        2
#define XMR_ABORT_ERR       3
#define XMR_BLOCK_ERR       4
#define XMR_CRC_ERR         5
#define XMR_CHECKSUM_ERR    6
#define XMR_WRITE_ERR       7
#define XMR_RESPONSE_ERR    8
#define XMR_MEM_ERR         9

/*
 * definitions for xmodem transmit
 */
#define XMT_OK              0
#define XMT_OPEN_ERR        1
#define XMT_RESPONSE_ERR    2
#define XMT_POLL_ERR        3
#define XMT_READ_ERR        4
#define XMT_ACK_ERR1        5
#define XMT_ACK_ERR2        6
#define XMT_MEM_ERR         7

#define format() format2(set_default_drive(-1))


/*
 *  OS function prototypes
 */
void autopowerdown(int what, unsigned long time);
int backlight(int what, unsigned short time);
unsigned int batteryvoltage(void);
short checktime(struct time *timep);
short checkdate(struct date *datep);
int chsize(int handle, long size);
int comopen(int port);
void comclose(int port);
int contrast(int value);
unsigned long coreleft(void);
unsigned long Heap_CoreLeft(void);
void cursor(int mode);
void delay(unsigned short time);
void downloadapplication(int port);
int downpressed(void);
void drawline(int x1, int y1, int x2, int y2, int color);
int endtimer(void);
void erasesector(unsigned short sector);
void eraseblock(unsigned short block);
int findnext(struct ffblk *ffblk);
int findfirst(const char *fname, struct ffblk *ffblk);
int flipscreen(int mode);
void format2(unsigned int disk);
long fsize(const char *filename);
int getcom(int timeout);
short getdate(struct date *datep);
short getdatetime(struct date *datep, struct time *timep);
void getfontsize(unsigned int *height, unsigned int *width);
unsigned long GetSystemTime(void);
unsigned int getterminalid(void);
unsigned int GetTickCount(void);
short gettime(struct time *timep);
void goodreadled(int on_off, int time);
void gotoxy(int x, int y);
void idle(void);
short isBuzzerOn(void);
unsigned short ischarging(void);
char* itoa(int val, char *buf, int base);
unsigned int kbhit(void);
int loaddisplay(char *filename);
int neto_receive(char (*filename)[MAX_FNAME], const char *term_id, int tkey, int ypos);
int neto_receive2(char (*filename)[MAX_FNAME], const char *term_id, unsigned int timeout, int tkey, int ypos);
int neto_transmit(char (*filename)[MAX_FNAME], int file_count, const char *term_id, int tkey, int ypos);
int neto_transmit2(char (*filename)[MAX_FNAME], int file_count, const char *term_id, unsigned int timeout, int tkey, int ypos);
int powereddown(void);
void printsymbol(unsigned int data);
void programbuffer(unsigned char *src, unsigned char *dest, unsigned int len);
void programbyte(unsigned char *address, unsigned char d);
void programword(unsigned short data, unsigned long address);
int putcom(unsigned char data);
unsigned int readbarcode(struct barcode *barcodep);
void resetkey(void);
void resetreceive(void);
int runapplfromfile(char *filename);
int savedisplay(char *filename);
int scanneroff(void);
void scannerpower(int mode, int time);
void secretstring(char *secr);
short setdate(struct date *datep);
short setdatetime(struct date *datep, struct time *timep);
int set_default_drive(int drive);
void setoffset(int offset);
#define set_offset setoffset
int setfont(unsigned int type, const unsigned char *font_ptr);
void setpixel(int x, int y, int state);
unsigned int setterminalid(unsigned int id);
short settime(struct time *timep);
void showpicture(int x_offs, int y_offs, const unsigned char *ptr);
void sound(int time, int vol, ...);
void starttimer(unsigned int timevalue);
int systemsetting(const char *string);
long tell(int handle);
unsigned int timervalue(void);
int triggerpressed(void);
int uppressed(void);
void version_os(char os[10]);
void version_rom(char rom[10]);
int wherex(void);
int wherey(void);
int xmodemreceive(const char *filename);
int xmodemtransmit(const char *filename);
void BackLightLvl(int bl_level);
void BitBlt(unsigned short start_x, unsigned short start_y, unsigned short end_x, unsigned short end_y, unsigned char *ptr, unsigned short mode);
void ClearDisplay(void);
void DelayMs(unsigned short ms);
void DelayUs(unsigned long us);
void DisableInts(void);
void DrawCircle(int cx, int cy, int r, int color);
void DrawEllipse(int cx, int cy, int xradius, int yradius, int color);
void EnableInts(void);
void FillCircle(int cx, int cy, int r, int color);
void FillEllipse(int cx, int cy, int xradius, int yradius, int color);
void FillScreen(unsigned short start_x, unsigned short start_y, unsigned short end_x, unsigned short end_y, unsigned long color);
unsigned int GetBatteryVoltage_mV(void);
char *GetBootVersion(void);
unsigned long GetFlashSize(void);
char *GetOsVersion(void);
int InstallHeartbeatHandler(unsigned long handler);
void PrintChar(unsigned char data);
int PutBuffer(const unsigned char *string, unsigned int len);
void PutByte(unsigned char l);
void PutShort(unsigned short l);
#if defined(OPL9815) || defined(OPL9813)
int PutString(const char *string, ...);
#else
int PutString(const char *string);
#endif
int RemoveHeartbeatHandler(unsigned long handler);
void ScrollUp(unsigned short height);
short SetAlarm(struct time *timep, unsigned short onoff);
int SetAutoBacklight(void);
long SetBackColor(long rgb);
void SetBackLightLvl(void);
long SetCursorColor(long rgb);
print_char_function SetPrintCharFunction(print_char_function fie);
void SetSystemTime(unsigned long value);
long SetTextColor(long rgb);
void SetTickCount(unsigned int value);
int shiftstate(int mode);
void ShowBMP(int x_offs, int y_offs, unsigned short mode, unsigned char *bmp);
int ShowGIF(unsigned short x_offs, unsigned short y_offs, unsigned short mode, unsigned char *picture);
int ShowJPG(int x_offs, int y_offs, unsigned short mode, unsigned char *picture);
int ShowJPG_File(int x_offs, int y_offs, unsigned short mode, char *filename);
void SoftReset(void);
int WaitKey(void);
// Additional function used on the H13++ device to obtain device serial number
const char* GetSerial( void );
void  SetPowerOffDelay( short ticks );

//gps.
void GpsPower(unsigned short onoff);
int GpsRead(unsigned int time);
void GpsClearReceiver(void);
void GpsWrite(unsigned char data);
int GpsSendCommand(char *command, int to);
int GpsGetFrame(char* buffer, int timeout);
int ParseGpsFrame(char *frame, gps *g);

//bluetooth.
unsigned char *GetBltAddress(void);
unsigned char *GetBltPin(void);
void SetBltAddress(const char *address);
void SetBltPin(const char *pincode);
unsigned char *GetBltLocalAddress(void);

int blt_is_connected(void);

unsigned char *GetBltLocalName(void);
void SetBltLocalName(const char *localname);

typedef void (*pin_callback) (char* pin, int* len );
void blt_set_pin_callback(pin_callback cb);
int blt_discovery(device_info *dev_inf, int abortkey);
void blt_delete_pairs(void);

#define blt_unpair 		blt_delete_pairs
#define IsBltConnected 	blt_is_connected
#define BltDiscovery 	blt_discovery

#define VERSION_INFO  	__attribute__ (( section (".app_version")))				// Use to add version info to  the application header ( 16 byte maximum )

#define CRADLE_ABORT			0x0100
#define ANY_KEYS_ABORT			0x0200

// Define return states
#define COMM_OK					(0)		// Communication OK
#define ERR_PR_NO_RESPONSE		(-1)	// No response
#define ERR_PR_CRC				(-2)	// CRC-16 error
#define ERR_PR_CMD_MISMATCH		(-3)	// Wrong command received
#define ERR_PR_FRAME_LENGTH		(-4)	// Frame length error
#define ERR_PR_SESSION_ID		(-5)	// Session ID mismatch
#define ERR_PR_SENDING_FRAME	(-6)	// Error sending frame
#define ERR_PR_OPEN_FILE		(-7)	// Cannot open or create the requested file
#define ERR_PR_READ_FROM_FILE	(-8)	// Cannot read from file
#define ERR_PR_WRITE_TO_FILE	(-9)	// cannot write to file
#define ERR_PR_WRONG_BLOCK		(-10)	// Requested the wrong block number
#define ERR_PR_FILE_SIZE		(-11)	// Error in filesizes
#define ERR_PR_POLL_COMMAND		(-12)	// unexpected poll command
#define ERR_PR_FILE_UNAVAILABLE	(-13)	// File cannot be found on the terminal
#define ERR_PR_LINE_BUSY		(-14)	// An other terminal is already communicating
#define ERR_PR_TIME_DATE		(-15)	// Error in the time or date when it is being set

#define ERR_PR_USER_ABORT		(-20)	// User aborted the protocol.

#define SUCC_COMPLETE			1		// 

// Defines for STATUS that are used in the callback
#define STAT_GET_TIME_DATE		1
#define STAT_SET_TIME_DATE		2
#define STAT_GET_OS_VERSION		3
#define STAT_GET_APPL_VERSION	4
#define STAT_XMIT_FILE_TO_PC	5
#define STAT_RECV_FILE_FROM_PC	6
#define STAT_LIST_FILES			7

// Callback function prototype
typedef void (*pOnOseCommInfo)( int status, int errorsuccess, int progress, const char *info );

//-------------------------------------------------------------------------------------------------------
// long OseComm( long sessionID, int terminateKey, const char *appVersion, pOnOseCommInfo OnOseCommInfo )
//
// Blocking function that wait for the PC to start the communication
// 
// long sessionID -		communication identification number 
//						valid range from 0 til 16777215 ( 0x000000 til 0xFFFFFF )
//						if sessionID matches -1 the a sessionID is generated by the terminal per session
//
// int terminateKey -	terminate key, when this key is pressed the protocol is aborted
// 
// const char *appVersion - A string holding the application version. when no version string
//							or an empty version string is used the terminal will send an unknown
//							to a get application version command.
//
//	pOnOseCommInfo OnOseCommInfo - Callback function that gets the status, error and progress information
//
// return:	COMM_OK
//			ERR_PR_USER_ABORT
//
//-------------------------------------------------------------------------------------------------------
long OseComm( long sessionID, int terminateKey, const char *appVersion, pOnOseCommInfo OnOseCommInfo );

/*
	
*/
int ApplicationSettingsMemorizing(int on_off, void *application_settings, int size, char *match_string );
int LoadApplicationSettings(int on_off, void *application_settings, int size, char *match_string );
void StoreApplicationSettings( void );

int InstallReceiveCallBack(void (*func)(unsigned char));

#endif // __LIB_H__
