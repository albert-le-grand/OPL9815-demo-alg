/*
 * utils.h
 *
 *  Created on: Mar 7, 2012
 *      Author: kogami
 */

#ifndef UTILS_H_
#define UTILS_H_

// special defines for the battery status
//#define BATT_EMPTY	1025
//#define BATT_LOW	1050
//#define BATT_CHARGE_LOW	1150
#define DCL_BATT_EMPTY  600
#define DCL_BATT_LOW    630

#define BATT_EMPTY	3550
#define BATT_LOW	3600
#define BATT_CHARGE_LOW	3650




#define BCR_MAX  			200

#define XL                    0
#define XR                  111
#define Y0                    0
#define Y1                   15
#define Y2                   31
#define Y3                   47
#define Y4                   63

#define SCAN                  1
#define KEYS                  2
#define ALPHA                 8
#define NUM                  16
#define FLOAT                32
#define NEGATIVE			 64
#define PRINT				 128
#define NOT_OK               -1
#define CLEAR                -1
#define CRADLE               20
#define POWERDOWN            30
#define KBHIT                40

#define DEFAULT			0x200 	// default return value
#define KEYBOARD		0x400	// used when input is done by keyboard
#define SCANNED			0x800	// used when input is scanned

#define TIMER_TIMEOUT   0xff    // indicates that the timer has timed out rather than a key being pressed

// timer ID's
#define TIMER_ID_POWER_DOWN     1       // autopower down timer, always running for GPS models
#define TIMER_ID_SCANNER        2       // scanner timer
#define TIMER_ID_GPS_BREADCRUMB 3       // GPS breadcrumb timer
#define TIMER_ID_GPS_TEST       4       // GPS test timer
#define TIMER_ID_GPS_POLL	    5		// GPS poll timer
#define TIMER_ID_BT_STATE_UPDATE	    6		// Bluetooth state update timer
#define TIMER_ID_MAX            6       // highest timer ID

// timer status
#define TIMER_STATUS_DISABLED   0
#define TIMER_STATUS_RUNNING    1
#define TIMER_STATUS_EXPIRED    2

void info_sound( void );
void keybeep(void);
void keybeeplow(void);
void errkeybeep(void);
void okbeep(void);
void errbeep(void);
int waitkey(void);
void is_on_cradle( void );
void check_batt_empty( void ); // check the battery
int waitkey_no_charge( void );
int KAwaitkey(int bat_type);
int KACheckKey(void);


void rectangle(int x1,int y1,int x2,int y2,int color);
void rectangle3d(int x1,int y1,int x2,int y2,int color);
void rectangle3drev(int x1,int y1,int x2,int y2,int color);
void draw_bar( int x_pos, int y_pos, int width, int height, int color );

// input functions
int ScanBarcode( char* string, int min_length, int max_length, int *code_id); //
int composite_post_processing(struct barcode *code, struct barcode *prev_code, unsigned long *time_out_value, short reset_time);
int ScanOrKeyboardInput( char* string, int min_length, int max_length, int typ, int x, int y, int offset );
int KeyboardQuantity( char* string, int max_length, int typ, int x, int y, int offset, long def_val, int max_x_d, int rect, int num, ... );

// returns the day of the week 0 is sunday, 1 is monday etc..
int what_day_of_week( void );

// display a 32*32 bit font icon
void display_icon( int x, int y, int state );

void set_font_input(void);
void set_font_menu( void );
void clear_screen(void);

void show_empty_batt_msg( int empty );

// timer maintenance
int CheckMyTimer(int nTimerID);
int StartMyTimer(int nTimerID, unsigned long nDuration, int fAutoRestart);
int StopMyTimer(int nTimerID);
void InitializeMyTimer(void);
void ResetMyTimer(int nTimerID);
void PokeActivity(void);
void PauseMyTimer(int nTimerID);
void ResumeMyTimer(int nTimerID);


#endif /* UTILS_H_ */
