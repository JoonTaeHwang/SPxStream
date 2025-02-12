/*********************************************************************
*
* (c) Copyright 2008 - 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxVisualServer.h,v $
* ID: $Id: SPxVisualServer.h,v 1.12 2016/05/19 13:14:38 rew Exp $
*
* Purpose:
*	Header containing definitions for SPx Visual Server
*
* Revision Control:
*   19/05/16 v1.12   AGC	Fix Clang on Windows warnings.
*
* Previous Changes:
*   04/10/13 1.11   AGC	Simplify headers.
*   02/08/12 1.10   AGC	Replace SPxScNet.h include with SPxLibUtils.h.
*   12/05/11 1.9    AGC	Make GetActive() const to match change in SPxObj.
*   15/09/10 1.8    REW	Add missing destructor.
*   15/09/10 1.7    REW	Make destructor virtual.
*   21/10/08 1.6    DGJ	Add destructor.  Now derived from SPxObj.
*   23/06/08 1.5    DGJ	Add parameter sending, change track report API
*   17/01/08 1.4    REW	Avoid icc compiler warnings.
*   09/01/08 1.3    DGJ	Add tags for target information.
*   07/01/08 1.2    REW	Correct direction of slash in include name.
*   07/01/08 1.1    DGJ	Initial Version.
**********************************************************************/

#ifndef _SPX_VISUALSERVER_H
#define _SPX_VISUALSERVER_H

#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"

/* Magic number used to indicate a Visual Server message */
#define SPX_VISUAL_MAGIC 0xABCD7485

/* Message types */
#define SPX_VISUAL_TYPE_STATUS 300
#define SPX_VISUAL_TYPE_SCALER 100
#define SPX_VISUAL_TYPE_SHORT_RADAR_RETURN 310
#define SPX_VISUAL_TYPE_MESSAGE 320
#define SPX_VISUAL_TYPE_BLOBS_IN_SECTOR 330
#define SPX_VISUAL_TYPE_RADAR_INFO 340
#define SPX_VISUAL_TYPE_BLOB_REPORT 350
#define SPX_VISUAL_TYPE_TRACK_REPORT 360

/* Scalar IDs. These are tags that indicate a specific parameter for a scalar message */
#define SPX_VISUAL_TAG_CPU_LOAD 1000
#define SPX_VISUAL_TAG_SERVCOUNT 1001
#define SPX_VISUAL_TAG_SERVOBJCOUNT 1002
#define SPX_VISUAL_TAG_PROCCOUNT 1003
#define SPX_VISUAL_TAG_SERVNAME 1004
#define SPX_VISUAL_TAG_LASTRECEIVEADDR 1005
#define SPX_VISUAL_TAG_LASTRECEIVESZ 1006
#define SPX_VISUAL_TAG_NETWORK_USE 1007
#define SPX_VISUAL_TAG_RADAR_SOURCE_COUNT 1008
#define SPX_VISUAL_TAG_REFRESH_INTERVAL_MS 1009
#define SPX_VISUAL_TAG_NUM_BLOB_SECTORS 1010
#define SPX_VISUAL_TAG_LAST_BLOB_SECTOR 1011
#define SPX_VISUAL_TAG_BLOBS_IN_SECTOR 1012
#define SPX_VISUAL_TAG_TOTAL_BLOBS 1013
#define SPX_VISUAL_TAG_LAST_AZIMUTH 1014
#define SPX_VISUAL_TAG_START_RANGE 1015
#define SPX_VISUAL_TAG_END_RANGE 1016
#define SPX_VISUAL_TAG_SCAN_MODE 1017
#define SPX_VISUAL_TAG_SOURCE_TYPE 1018
#define SPX_VISUAL_TAG_PARAMETER_VALUE 1019


/* Target information */
#define SPX_VISUAL_TAG_TARGET_ID 1500
#define SPX_VISUAL_TAG_TARGET_RANGE 1501
#define SPX_VISUAL_TAG_TARGET_AZIMUTH 1502
#define SPX_VISUAL_TAG_TARGET_SPEED 1503
#define SPX_VISUAL_TAG_TARGET_HEADING 1504
#define SPX_VISUAL_TAG_TARGET_TYPE 1505
#define SPX_VISUAL_TAG_TARGET_AZIMUTH_ALPHA 1506
#define SPX_VISUAL_TAG_TARGET_AZIMUTH_BETA 1507
#define SPX_VISUAL_TAG_TARGET_RANGE_ALPHA 1508
#define SPX_VISUAL_TAG_TARGET_RANGE_BETA  1509
#define SPX_VISUAL_TAG_TARGET_FILTERED_RANGE_SIZE 1510
#define SPX_VISUAL_TAG_TARGET_FILTERED_AZIMUTH_SIZE 1511
#define SPX_VISUAL_TAG_TARGET_MEASURED_RANGE_SIZE 1512
#define SPX_VISUAL_TAG_TARGET_MEASURED_AZIMUTH_SIZE 1513
#define SPX_VISUAL_TAG_TARGET_COAST_COUNT 1514
#define SPX_VISUAL_TAG_TARGET_TRACK_AGE 1515
#define SPX_VISUAL_TAG_TARGET_MEASURED_X 1516
#define SPX_VISUAL_TAG_TARGET_MEASURED_Y 1517


/* Radar video tags. */
#define SPX_VISUAL_TAG_RADAR1 2000
#define SPX_VISUAL_TAG_RADAR2 2001
#define SPX_VISUAL_TAG_RADAR3 2002
#define SPX_VISUAL_TAG_RADAR4 2003
#define SPX_VISUAL_TAG_RADAR5 2004
#define SPX_VISUAL_TAG_RADAR6 2005
#define SPX_VISUAL_TAG_RADAR7 2006
#define SPX_VISUAL_TAG_RADAR8 2007
#define SPX_VISUAL_TAG_RADAR9 2008
#define SPX_VISUAL_TAG_RADAR10 2009

/* PPI window */
#define SPX_VISUAL_TAG_PPI1 2100

/* Radar info */
#define SPX_VISUAL_TAG_RADAR_INFO1 2200
#define SPX_VISUAL_TAG_RADAR_INFO2 2201
#define SPX_VISUAL_TAG_RADAR_INFO3 2202
#define SPX_VISUAL_TAG_RADAR_INFO4 2203
#define SPX_VISUAL_TAG_RADAR_INFO5 2204


/* Test tags */
#define SPX_VISUAL_TAG_TEST_BITMAP 5000
#define SPX_VISUAL_TAG_TEST_ASCAN1 5001
#define SPX_VISUAL_TAG_TEST_ASCAN2 5002
#define SPX_VISUAL_TAG_TEST_ASCAN3 5003
#define SPX_VISUAL_TAG_TEST_ASCAN4 5004
#define SPX_VISUAL_TAG_TEST_BSCAN  5100

/* Size of a short radar return sent to client */
#define SPX_VS_SHORT_RADAR_RETURN_LEN 128


/* The class that represents a visual server. */

class SPxVisualServer;
class SPxRunProcess;
class SPxProcessRunParameter;
class SPxThread;

/* Class to hold a process that the visual server is reporting parameters for. */
class SPxVisualServerReportProcess
{
friend class SPxVisualServer;
    SPxVisualServerReportProcess *m_next;
    SPxRunProcess *m_runProcess;

public:
    SPxVisualServerReportProcess(SPxRunProcess *process);
    virtual ~SPxVisualServerReportProcess() {}
};

/* The visual server class itself. */
class SPxVisualServer :public SPxObj
{
    struct sockaddr_in m_destination;	    /* The socket that we will use to send reports */
    SOCKET_TYPE m_socket;
    SPxThread *m_thread;		    /* The thread that we are running in */
    UINT32 *m_buf;			    /* A general-purpose buffer */
    char *m_serverName;			    /* The name of the server that we are providing data for */
    UINT32 m_count;			    /* An incrementing conter */
    int m_active;			    /* If 0 do no network output */
    
    void SendData(const char * message, int length);
    	    				    /* Send block of data */

    /* Report processes */
    SPxVisualServerReportProcess *m_firstProcessReport;
    SPxVisualServerReportProcess *m_lastProcessReport;
    SPxVisualServerReportProcess *m_currentSend;
    SPxProcessRunParameter *m_currentSendParameter;
    void SendNextProcessParameter(void);

public:
    SPxVisualServer(const char *name, const char *ipAddress, UINT16 ipPort);
					    /* Create a visual server with a name (which is
					    sent across the network to the client display),
					    an IP address to send to and a port number */
    virtual ~SPxVisualServer(void);

    /* Define a process that the visual server will report parameters for */
    void ReportProcess(SPxRunProcess *runProcess);

    void *ThreadFunc(SPxThread *thread);	    
    void SendStatus(void);		    /* Send status message */
    void SendValue(UINT32 tag, const char * value);
					    /* Send a tag+value (string) pair */
    void SendValue(UINT32 tag, int value);  /* Send tag_value (int) */
    void SendRadarData(UINT32 id, UINT16 azimuth, 
			REAL32 startRange, REAL32 endRange, const unsigned char * data);
					    /* Send a mini-return of radar video */
    void SendRadarInfo(UINT32 tag, UINT16 azimuth, REAL32 startRange, REAL32 endRange,
			UCHAR scanMode, UINT16 thisLength, UINT16 nominalLength,
			UINT16 sourceType);
					    /* Send data from a radar header */
    void SendMessage(const char *message);	    /* Send a string message */
    void SendRadarSourceMessage(void);	    /* Send a radar source message */
    void SetActive(int state) {m_active = state;}
					    /* Set whether we are active (will send data
					    to network) or inactive. */
    int GetActive(void) const {return m_active;}
					    /* Get current activity state. */
    void SendBlobsInSectorMessage(int sector, int numSectors, int numBlobs);
					    /* Send blobs in sector message */
    void SendBlobMessage(REAL32 rangeMetres, REAL32 sizeMetres,
					REAL32 azimuthDegrees, REAL32 sizeAzimuthDegrees,
					UINT16 strength, UINT16 weight);
					    /* Send blob report */
    void SendTrackReport(
				UINT32 trackID,         /* Track identification code */
				REAL32 filteredX,
				REAL32 filteredY,	/* Filtered x,y position in metres relative to radar */ 
				REAL32 speed,           /* Filtered speed in metres/s */
				REAL32 heading,         /* Filtered heading in degrees with 0 being North and
							positive headings being clockwise. */
				REAL32 rangeAlpha, 			
				REAL32 azimuthAlpha, 					
							
				REAL32 measuredX,
				REAL32 measuredY, 
							/* The measured X,Y from last observation */
				REAL32 filteredRangeSize,
				REAL32 filteredAzimuthSize,   
							/* Filtered dimensions in metres and degrees */
				UINT32 timeStampS,      
				UINT32 timeStampUS,	/* Time stamp of last update (seconds and useconds) */
				UINT32 coastCount,      /* Number of scans coasted. */
				UINT32 trackAge		/* Age in scans */
                  );
					/* Send track report */

};

#endif /* _SPX_VISUALSERVER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
