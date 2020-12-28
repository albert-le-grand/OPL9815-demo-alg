#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "lib.h"
#include "gps.h"
#include "menu.h"
#include "main.h"
#include "utils.h"

/************************************************************************/
/*      Defines                                                         */
/************************************************************************/

#define MAX_TIME_OFFSET			60UL		// max sec. allowed time difference between GPS and RTC
#define MAX_FIX_TIMEOUT			30UL		// max sec. that "no fix" is still seen as a "fix" after a fix is lost
#define MAX_UPDATE_TIMEOUT		30UL		// max sec that a "no fix" record is corrected in the file

#define AUTOPOWERDOWN_TIME		(3*60)      // time in seconds. If set too short GPS might not get a fix in that time

#define MAX_BUF     			300
#define DATE					10   // XX/XX/XXXX
#define TIME		 			8    // XX:XX:XX

#define SECS_PER_HOUR			(60UL*60UL)
#define SECS_PER_DAY			(60UL*60UL*24UL)
//01   02   03   04   05   06   07   08   09   10   11   12
#define DAYS_PER_YEAR   		(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31)

#define BREAD_CRUMB_TRAIL_TIME  15		// default time in minutes

int g_BreadCrumbTime;
int g_BreadCrumbFlag;
unsigned long g_NextBreadCrumb;  // next time setting for breadcrumb

// GPS globals
char g_gpsDate[DATE+1];
char g_gpsTime[TIME+1];
unsigned long last_fix;
unsigned char g_fSuspendConversion;
char g_Latitude[LATITUDE_DMS+1];
char g_Longitude[LONGITUDE_DMS+1];
char g_LatitudeRaw[LATITUDE_DMS+1];
char g_LongitudeRaw[LONGITUDE_DMS+1];
char g_LatitudeDD[LATITUDE_DMS+1];
char g_LongitudeDD[LONGITUDE_DMS+1];
char g_LongitudeDMS[LONGITUDE_DMS+2];
char g_LatitudeDMS[LATITUDE_DMS+1];
const char g_EmptyLatitude[] = "0000.0000X";
const char g_EmptyLongitude[] = "00000.0000X";

// GPS breadcrumb timer
int g_GPS_breadcrumbTimer;
int g_GPS_Test_Mode;
// breadcrumb record string name
const char g_pszGPSBreadCrumb[] = "GPS Bread Crumb";
int g_nBreadCrumbIndex;
unsigned long g_nGPSStandbyTimeout;   // GPS standby timeout as configured by system menu


extern long gps_data_format;
extern long time_zone_offset;
extern long time_zone_change;
extern unsigned int timer[];
extern int g_util_timeout;
#define GPS_IDLE_STATE              0
#define GPS_READING_RECORD_STATE    1
#define GPS_READING_CHECKSUM_STATE  2
#define GPS_END_OF_RECORD_STATE     3

int gpsReadState;
int gpsCharCount;
// indicates that gps is active: 0 = no fix, non zero = okay
int g_gpsfix;
char gpsString[MAX_BUF+1];
char gpsLastString[MAX_BUF+1];
int g_gpsOptions;

char GPSBuffer[100];
gps currentFix;


const unsigned short int month_len[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // Normal years.


//------------------------------------------------------------------
//	checksum
//  ============================
//	Calculates the NMEA183 checksum
//------------------------------------------------------------------
int checksum(const char *string)
{
    const char hex[]="0123456789ABCDEF";
    char *ptr;
    unsigned char checksum;

    checksum=0;
    ptr=(char*) string;
    if (*ptr != '$')
        return(0);
    for (ptr++;*ptr;ptr++)
    {
        if (*ptr == '*')
            break;
        checksum^=*ptr;
    }
    ptr++;
    if (*ptr != hex[(checksum & 0xF0) >> 4])
        return(0);
    ptr++;
    if (*ptr != hex[checksum & 0x0F])
        return(0);
    return(1);
}

//------------------------------------------------------------------
//	get_gps_pos_date_time
//  ============================
//	Get the latitude, longitude and GPS date/time from the NMEA183 RMC (Recommended Minimum Specific GPS data) sentence
//------------------------------------------------------------------
int get_gps_pos_date_time(
                         const char *string,
                         char *pszLatitude,
                         char *longitude,
                         char *time,
                         char *date)
{
    char *ptr;
    char ch;
    int ret;

    ret=0;
    ptr=(char*) string;

    if (*ptr++ != '$')
        return(-1);
    if (*ptr++ != 'G')
        return(-1);
    if (*ptr++ != 'P')
        return(-1);
    if (*ptr++ != 'R')
        return(-1);
    if (*ptr++ != 'M')
        return(-1);
    if (*ptr++ != 'C')
        return(-1);
    ptr++;                          // skip ,
    *time++ = *ptr++;
    *time++ = *ptr++;
    *time++ = ':';
    *time++ = *ptr++;
    *time++ = *ptr++;
    *time++ = ':';
    *time++ = *ptr++;
    *time++ = *ptr++;
    *time='\0';
    while (*ptr != ',')
    {
        if (*ptr == '\0')
            return(-1);
        ptr++;
    }
    ptr++;
    if (*ptr++ == 'A')
        ret=1;
    ptr++;
    while ((ch=*ptr++) != ',')
    {
        if (ch == '\0')
            return(-1);
        *pszLatitude++=(char)ch;
    }
    *pszLatitude++ = *ptr++;
    *pszLatitude='\0';
    ptr++;
    while ((ch=*ptr++) != ',')
    {
        if (ch == '\0')
            return(-1);
        *longitude++=(char)ch;
    }
    *longitude++ = *ptr++;
    *longitude = '\0';
    ptr++;
    while (*ptr != ',')
    {
        if (*ptr == '\0')
            return(-1);
        ptr++;
    }
    ptr++;
    while (*ptr != ',')
    {
        if (*ptr == '\0')
            return(-1);
        ptr++;
    }
    ptr++;
    *date++=*ptr++;
    *date++=*ptr++;
    *date++='/';
    *date++=*ptr++;
    *date++=*ptr++;
    *date++='/';
    *date++='2';
    *date++='0';
    *date++=*ptr++;
    *date++=*ptr++;
    *date='\0';
    return(ret);
}

//------------------------------------------------------------------
//	_time
//  ============================
//	Converts current date and time to system time value.
//------------------------------------------------------------------
unsigned long _time(void)
{
    struct date d = {0,0,0};
    struct time t = {0,0,0};


    getdate(&d);
    gettime(&t);
    return(_mktime(&t,&d));
}

//------------------------------------------------------------------
//	_mktime
//  ============================
//	converts current system time to timae and date structures.
//------------------------------------------------------------------
unsigned long _mktime(struct time *t,struct date *d)
{
    unsigned long sec;
    int leap;
    int count;

    sec = t->ti_sec;
    sec += (t->ti_min * 60UL);
    sec += (t->ti_hour * SECS_PER_HOUR);
    sec += (d->da_day - 1) * SECS_PER_DAY;
    leap = (d->da_year % 4 == 0 && d->da_year % 100 != 0) || d->da_year % 400 == 0;
    for (count = 0; count < (d->da_mon - 1); count++)
        sec += month_len[count] * SECS_PER_DAY;
    if (leap && (d->da_mon > 2))
        sec += SECS_PER_DAY;
    for (count = 1970; count < d->da_year; count++)
    {
        sec += (DAYS_PER_YEAR * SECS_PER_DAY);
        if ((count % 4 == 0 && count % 100 != 0) || count % 400 == 0)
            sec += SECS_PER_DAY;
    }
    return(sec);
}

//------------------------------------------------------------------
//	GPSStart
//  ============================
//	Inititalizes the GPS system.
//------------------------------------------------------------------
void GPSStart(void)
{
	char buffer[100];
	int status;

	strcpy(g_Longitude,(char*) "00000.0000?");
    strcpy(g_Latitude, (char*) "0000.0000?");

    g_gpsfix = 0;

    if (g_BreadCrumbTime == 0)
    {
        // default the bread crumb time
        g_BreadCrumbTime = BREAD_CRUMB_TRAIL_TIME;
        g_BreadCrumbFlag = TRUE;
    }
	GpsPower(GPS_ON);
	for(status=0;status != 3;)
	{
		if (GpsGetFrame(buffer,100) == ERROR)
			continue;

		if (strncmp(buffer,"$PMTK010",8) == 0)
			status |= 1;

		if (strncmp(buffer,"$PMTK011",8) == 0)
			status |= 2;

		if (status == 3)
			break;
	}

	// only enable the GPRMC sentence to speed up GPS fix
	if (GpsSendCommand("314,0,1,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0",150) < 0)
	{
		printf("ERROR!!!!!\n");
		while(1)
			idle();
	}

	StartMyTimer(TIMER_ID_GPS_POLL, 200, 1);
    g_fSuspendConversion = FALSE;
}

//------------------------------------------------------------------
//	GPSService
//  ============================
//	GPS radio polling service
//------------------------------------------------------------------
void GPSService(void)
{
    struct time t={0};
    struct date d={0};
    static unsigned long gps_time;
    static unsigned long rtc_time;
    static char t_longitude[LONGITUDE+1];
    static char t_latitude[LONGITUDE+1];

    if(CheckMyTimer(TIMER_ID_GPS_POLL) == TIMER_STATUS_EXPIRED)
    {
    	ResetMyTimer(TIMER_ID_GPS_POLL);
		// read a frame from the GPS receiver
		GpsGetFrame(GPSBuffer,100);

		// check for GPRMC string
		if (strncmp(GPSBuffer,"$GPRMC",6) == 0)
		{
			g_gpsfix = get_gps_pos_date_time(GPSBuffer, t_latitude, t_longitude, g_gpsTime, g_gpsDate);
			if (g_gpsfix == 1)
			{
				strncpy(g_LatitudeRaw, t_latitude,  LATITUDE);
				strncpy(g_LongitudeRaw, t_longitude,  LONGITUDE);
				CalculateAlternateGPSFormat();
				rtc_time = _time();
				d.da_day  =  (g_gpsDate[0] - '0') * 10 + g_gpsDate[1] - '0';
				d.da_mon  =  (g_gpsDate[3] - '0') * 10 + g_gpsDate[4] - '0';
				d.da_year = 2000;
				d.da_year += (g_gpsDate[8] - '0') * 10 + g_gpsDate[9] - '0';
				t.ti_hour =  (g_gpsTime[0] - '0') * 10 + g_gpsTime[1] - '0';
				t.ti_min  =  (g_gpsTime[3] - '0') * 10 + g_gpsTime[4] - '0';
				t.ti_sec  =  (g_gpsTime[6] - '0') * 10 + g_gpsTime[7] - '0';
				gps_time=_mktime(&t,&d);

				/* if last GPS fix was more then 10 Min ago then give GPS device 45 sec time   */
				/* to get more data for the satalite, so reset the autopowerdown time          */
//                  if (last_fix < (gps_time - (10*60)))
//                      PokeActivity();

				// offset by the time zone in seconds
				gps_time += time_zone_offset * 60 * 60;
				last_fix = gps_time;
				if ((gps_time > (rtc_time+MAX_TIME_OFFSET)) ||
					(gps_time < (rtc_time-MAX_TIME_OFFSET)) ||
					(time_zone_change == TRUE)) // need to set RTC ?
				{
					getDateTime(gps_time, &t, &d);
					setdate(&d);
					settime(&t);
					if(time_zone_change == TRUE)
					{
						time_zone_change = FALSE;
					}
				}

			}
		}
		// keep flashing the LED to show whether there is a fix or not
		if(g_gpsfix == 1)
		{
			goodreadled(GREEN,20);
		}
		else
		{
			goodreadled(RED,20);    // show user that there is no fix
		}
    }
}

//------------------------------------------------------------------
//	CalculateAlternateGPSFormat
//  ============================
// 	Converts the raw GPS format from NMEA to deg:min:sec and decimal deg
// 	NMEA format is degrees prepended onto the minutes
//
void CalculateAlternateGPSFormat(void)
{
    if(g_fSuspendConversion == FALSE)
    {
        // convert the longitude and latitude
        ConvertNMEAToAlternateFormats(g_LatitudeRaw, g_LatitudeDD, g_LatitudeDMS, FALSE);
        ConvertNMEAToAlternateFormats(g_LongitudeRaw, g_LongitudeDD, g_LongitudeDMS, TRUE);
        // set the cordinates according to the format setting
        switch(gps_data_format)
        {
            case GPS_RAW:
                strncpy(g_Latitude, g_LatitudeRaw, LATITUDE);
                g_Latitude[LATITUDE] = 0;
                strncpy(g_Longitude, g_LongitudeRaw, LONGITUDE);
                g_Longitude[LONGITUDE] = 0;
                break;
            case GPS_DECIMAL_DEGREE:
                strncpy(g_Latitude, g_LatitudeDD, LATITUDE);
                g_Latitude[LATITUDE] = 0;
                strncpy(g_Longitude, g_LongitudeDD, LONGITUDE);
                g_Longitude[LONGITUDE] = 0;
                break;
            case GPS_DEG_MIN_SEC:
                strncpy(g_Latitude, g_LatitudeDMS, LATITUDE_DMS);
                g_Latitude[LATITUDE_DMS] = 0;
                strncpy(g_Longitude, g_LongitudeDMS, LONGITUDE_DMS);
                g_Longitude[LONGITUDE_DMS] = 0;
                break;
            default:
                strncpy(g_Latitude, g_LatitudeRaw, LATITUDE);
                g_Latitude[LATITUDE] = 0;
                strncpy(g_Longitude, g_LongitudeRaw, LONGITUDE);
                g_Longitude[LONGITUDE] = 0;
                break;
        }
    }
}

//------------------------------------------------------------------
//	ConvertNMEAToAlternateFormats
//  ============================
//	Calculates and converts the coordinates from NMEA reading
//------------------------------------------------------------------
void ConvertNMEAToAlternateFormats(char * pszNMEA, char * pszDD, char * pszDMS, int fLong)
{
    static char degrees[4];
    static char minutes_int[3];
    static char minutes_frac[6];
    static char direction[2];
    static int minutes_int_num;
    static unsigned long minutes_frac_num;
    static unsigned long degrees_frac_num;
    static unsigned long seconds_frac;
    static unsigned long seconds_int;
    int nIndex = 0;

    // extract the degrees
    if(fLong == TRUE)
    {
        // for longitude degrees are 3 digits
        strncpy(degrees, pszNMEA, 3);
        degrees[3] = 0;
        nIndex = 3;
    }
    else
    {
        // for latitude degrees are 2 digits
        strncpy(degrees, pszNMEA, 2);
        degrees[2] = 0;
        nIndex = 2;
    }
    // extract the minutes
    strncpy(minutes_int, &pszNMEA[nIndex], 2);
    minutes_int[2] = 0;
    minutes_int_num = atol(minutes_int);
    nIndex += 3;
    // extract the fractional minutes
    strncpy(minutes_frac, &pszNMEA[nIndex], 4);
    minutes_frac[4] = 0;
    minutes_frac_num = atol(minutes_frac);
    nIndex += 4;
    // extract the direction
    direction[0] =  pszNMEA[nIndex];
    direction[1] = 0;
    // calculate the decimal degrees
    degrees_frac_num = (minutes_int_num * 1000000L + minutes_frac_num * 100L) / 60L;
    // calculate degree minutes and seconds
    // first caluclate the seconds * 10000
    seconds_frac = minutes_frac_num * 60L;
    // divide in a separte line to make sure that the precision is maintained
    seconds_frac /= 100L;
    seconds_int = seconds_frac / 100L;
    // format the coordinate strings
    if(fLong == TRUE)
    {
        sprintf(pszDD, "%3s.%06lu", degrees, degrees_frac_num);
        pszDD[LONGITUDE-1] = direction[0];
        pszDD[LONGITUDE] = 0;
        sprintf(pszDMS, "%3s:%02d:%02lu.%02lu", degrees, minutes_int_num, seconds_int, seconds_frac);
    }
    else
    {
        sprintf(pszDD, "%2s.%06lu", degrees, degrees_frac_num);
        pszDD[LATITUDE-1] = direction[0];
        pszDD[LATITUDE] = 0;
        sprintf(pszDMS, "%2s:%02d:%02lu.%02lu", degrees, minutes_int_num, seconds_int, seconds_frac);
    }
}

//------------------------------------------------------------------
//	getDateTime
//  ============================
//	Converts a system time value to a time and date structure.
//------------------------------------------------------------------
void getDateTime(unsigned long ulTime, struct time *t, struct date *d)
{
    unsigned int nIndex;
    int nLeap = 0;
    unsigned long ulSecondsLeft;

    ulSecondsLeft = ulTime;
    // subtract years to calculate the year
    for (nIndex=1970;ulSecondsLeft>((DAYS_PER_YEAR + nLeap) * SECS_PER_DAY);)
    {
        // subtract a years worth of seconds, including the leap day for leap years
        ulSecondsLeft -= (DAYS_PER_YEAR + nLeap) * SECS_PER_DAY;
        // keep track if the following year is a leap year
        nIndex++;
        if ((nIndex % 4 == 0 && nIndex % 100 != 0) || nIndex % 400 == 0)
        {
            nLeap = 1;
        }
        else
        {
            nLeap = 0;
        }
    }
    // set the year
    d->da_year = nIndex;
    // keep track if this is a leap year
    nLeap = (nIndex % 4 == 0 && nIndex % 100 != 0) || nIndex % 400 == 0 ? 1 : 0;
    // subtract months starting from January
    for(nIndex=0;ulSecondsLeft>(SECS_PER_DAY * month_len[nIndex]);nIndex++)
    {
        ulSecondsLeft -= SECS_PER_DAY * month_len[nIndex];
        // check for leap year and february
        if((nIndex == 1) && (nLeap == 1))
        {
            // subtract an extra day
            ulSecondsLeft -= SECS_PER_DAY;
        }
    }
    // set the month
    d->da_mon = (unsigned char)nIndex + 1;
    // divide by days
    d->da_day = (unsigned char)(ulSecondsLeft / SECS_PER_DAY);
    ulSecondsLeft -= d->da_day * SECS_PER_DAY;
    d->da_day++;
    // divide by hours
    t->ti_hour = (unsigned char)(ulSecondsLeft / SECS_PER_HOUR);
    ulSecondsLeft -= t->ti_hour * SECS_PER_HOUR;
    // divice by minutes
    t->ti_min = (unsigned char)(ulSecondsLeft / 60);
    ulSecondsLeft -= t->ti_min * 60;
    t->ti_sec = (unsigned char) ulSecondsLeft;
}

//------------------------------------------------------------------
//	GPSUpdateTestDisplay
//  ============================
//	Displays the GPS coordinates in their current format for the GPS test.
//------------------------------------------------------------------
void GPSUpdateTestDisplay(void)
{
    struct date d;
	struct time t;

	getdate( &d );
    gettime( &t );

    setfont(SMALL_FONT, NULL);

    gotoxy(1,1);
    printf("latitude:");
    gotoxy(1,2);
    printf("%s", g_Latitude);
    gotoxy(1,3);
    printf("longitude:");
    gotoxy(1,4);
    printf("%s", g_Longitude);
    gotoxy(0,5);
    printf( "%02d:%02d:%02d", t.ti_hour, t.ti_min, t.ti_sec);
    gotoxy(0,6);
    printf("%02d/%02d/%04d", d.da_day, d.da_mon, d.da_year);

    PokeActivity();

    if(g_GPS_Test_Mode == 2)
    {
    	GPSSaveBreadCrumb();
    }
}

//------------------------------------------------------------------
//	ResetBreadCrumbTimer
//  ============================
//	Resets or disables the GPS breadcrumb timer
//------------------------------------------------------------------
void ResetBreadCrumbTimer(void)
{
	struct time alTime;
	struct date alDate;

	if(g_BreadCrumbFlag == TRUE)
	{
		g_GPS_breadcrumbTimer = g_BreadCrumbTime;
		// set the next breadcrumb time
		g_NextBreadCrumb = _time() + g_BreadCrumbTime * 60;
		// set the alarm
		getDateTime(g_NextBreadCrumb, &alTime, &alDate);
		SetAlarm(&alTime, ON);
		// restart the timer
		StartMyTimer(TIMER_ID_GPS_BREADCRUMB, GPS_BREADCRUMB_TIMER, TRUE);
	}
	else
	{
		SetAlarm(&alTime, OFF);
		StopMyTimer(TIMER_ID_GPS_BREADCRUMB);
	}
}

//------------------------------------------------------------------
//	ServiceBreadCrumbTimer
//  ============================
//	Services the bread crumb timer or updates the GPS test display..
// 	This function is called beck from waitkey when the timer expires
//------------------------------------------------------------------
void ServiceBreadCrumbTimer(void)
{
    unsigned long nCurrentTime = _time();

    if (g_GPS_Test_Mode == 0)
    {
        // this call is needed to keep the power management going
        CheckMyTimer(TIMER_ID_GPS_BREADCRUMB);
        // don't service the breadcrumb timer if after a power down and before the power down update
        if(g_BreadCrumbFlag == TRUE)
        {
            if(nCurrentTime >= g_NextBreadCrumb)
            {
                // bread crumb timeout
                GPSSaveBreadCrumb();
                // reset minute counter
                ResetBreadCrumbTimer();
            }
        }
    }
    else
    {
        // GPS test mode
        if(CheckMyTimer(TIMER_ID_GPS_TEST) == TIMER_STATUS_EXPIRED)
        {
            // update the GPS test display
            GPSUpdateTestDisplay();
            sound(TSTANDARD, VHIGH, SMEDIUM, SHIGH, NULL);
        }
    }

}

//------------------------------------------------------------------
//	GPSSaveBreadCrumb
//  ============================
//	Saves breadcrumb.
//------------------------------------------------------------------
void GPSSaveBreadCrumb(void)
{
    static char record[ SZ_RECORD + 1 ];
    char pszBreadCrumb[sizeof(g_pszGPSBreadCrumb)+1];
    char pszBreadCrumbQty[SZ_SIGN+SZ_QUANTITY+1];
    int fd;
    db_record db_rec;
    long nBreadCrumbNumber = 0;

    // make sure there is a fix
    if (g_gpsfix == 0)
    {
        // no bread crumb if no fix
        return;
    }

    strcpy(pszBreadCrumb, g_pszGPSBreadCrumb);
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

    // make a breadcrumb record
    create_db_rec_struct(&db_rec, pszBreadCrumb, g_nBreadCrumbIndex);

    if ((fd = open( (char*) DATA_BASE, O_RDWR |O_CREAT, 0x0)) == -1)
    {
        errbeep();
        show_message("Error while opening file", MB_ALL | MB_ICONERROR );
        return;
    }

    // mske the quantity string
    sprintf(pszBreadCrumbQty, "%*ld", SZ_SIGN+SZ_QUANTITY, nBreadCrumbNumber);
    sprintf(record, "%-*.*s,%-*.*s,%-*.*s,%-*.*s,%-*.*s,%-*.*s\r\n",
            SZ_BARCODE, SZ_BARCODE, db_rec.barcode,
            SZ_SIGN+SZ_QUANTITY, SZ_SIGN+SZ_QUANTITY, db_rec.quantity,
            SZ_TIME, SZ_TIME, db_rec.time,
            SZ_DATE, SZ_DATE, db_rec.date,
            SZ_LATITUDE, SZ_LATITUDE, db_rec.latitude,
            SZ_LONGITUDE, SZ_LONGITUDE, db_rec.longitude);
    // append the bread crumb to the file
    lseek( fd, 0L, SEEK_END );

    if ( write( fd, record, SZ_RECORD ) != SZ_RECORD )
    {
        errbeep();
        show_message("Error while writing data to the database!", MB_ALL | MB_ICONERROR );
    }
    close( fd );
    // increment the bread crumb index
    g_nBreadCrumbIndex++;
    // bread crumb drop tone
    sound(TSTANDARD, VHIGH, SMEDIUM, SHIGH, NULL);
}


