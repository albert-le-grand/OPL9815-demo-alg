#ifndef _GPS_H
#define _GPS_H

#define GPS_ON		ON
#define GPS_OFF		OFF
#define GPS_SUSPEND 2
#define GPS_RESUME  3
#define GPS_BLOCK	4
#define GPS_UNBLOCK	5

// dd,ddd - 2 and 4 digit degree
// mm.mmmm - minutes with 4-digit fraction
// V - N or S
// H - E or W
#define LONGITUDE           11      // Length of raw NMEA longitude string dddmm.mmmmH
                                    // same as decimal degree format ddd.ddddddH
#define LATITUDE  		    10      // Length of raw NMEA latitude string ddmm.mmmmV
                                    // same as decimal degree format dd.ddddddV
#define LONGITUDE_DMS       13      // Length of DMS longitude ddd:mm:ss.ssH
#define LATITUDE_DMS        12      // Length of DMS latitude dd:mm:ss.ssV

// GPS data format defines
#define GPS_RAW             0   // Raw NMEA format
#define GPS_DECIMAL_DEGREE  1   // Decimal degree format
#define GPS_DEG_MIN_SEC     2   // Degree-Minute Second format

#define GPS_BREADCRUMB_MINUTE   50*60   // 1 minute
#define GPS_TEST_UPDATE         50*20   // 20 seconds
#define GPS_BREADCRUMB_TIMER    50*5    // 5 second time poll


void InitGps(void);
void GpsPower(unsigned short onoff);
void GpsClearReceiver(void);
int GpsRead(unsigned int time);
void GpsWrite(unsigned char data);
int GpsSendCommand(char *command, int to);
int GpsGetFrame(char *buffer, int max_frame);
int ParseGpsFrame(char *frame, gps *g);

int checksum(const char *string);
int get_gps_pos_date_time(const char *string,char *pszLatitude,char *longitude,char *time,char *date);
void CalculateAlternateGPSFormat(void);
void ConvertNMEAToAlternateFormats(char * pszNMEA, char * pszDD, char * pszDMS, int fLong);

unsigned long _time(void);
unsigned long _mktime(struct time *t,struct date *d);
void GPSStart(void);
void GPSService(void);
void getDateTime(unsigned long ulTime, struct time *t, struct date *d);
void GPSUpdateTestDisplay(void);
void ResetBreadCrumbTimer(void);
void ServiceBreadCrumbTimer(void);
void GPSSaveBreadCrumb(void);

#endif
