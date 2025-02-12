/*********************************************************************
*
* (c) Copyright 2013 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for ASTERIX Cat-253 base class.
*
* Revision Control:
*   25/06/19 v1.8    SP 	Fix GetLastSequenceID().
*
* Previous Changes:
*   20/06/19 1.7    SP 	Fixes after P395 testing.
*   23/05/19 1.6    SP 	Support I253/040 - Message Type Identifier.
*                      	Other changes to support P395.
*   16/12/15 1.5    AJH	WriteMesgToSendBuf() takes offset into buffer.
*   27/01/14 1.4    SP 	Make Message Sequence ID optional.
*   24/10/13 1.3    SP 	Support I253/050 - Message Sequence ID.
*   04/10/13 1.2    AGC	Simplify headers.
*   08/04/13 1.1    SP 	Initial Version.
*
**********************************************************************/

#ifndef _SPX_NET_CAT_253_H
#define _SPX_NET_CAT_253_H

/*
 * Other headers required.
 */

/* We need SPxLibUtils for common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxError.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Maximum length of application data, exlcuding null terminator. */
#define SPX_CAT253_MAX_APP_DATA_BYTES 255

/* Maximum length of application data, including null terminator. */
#define SPX_CAT253_MAX_APP_DATA_INC_TERM_BYTES (SPX_CAT253_MAX_APP_DATA_BYTES - 1)

/* Message type identifiers in I253/040 (6 LSBs). */
#define SPX_CAT253_MTI_NONE                             0   /* Do not include I253/040. */
#define SPX_CAT253_MTI_TIME_OF_DAY                      1
#define SPX_CAT253_MTI_CONNECT_REQUEST                  2
#define SPX_CAT253_MTI_CONNECT_RESPONSE                 3
#define SPX_CAT253_MTI_CONNECT_RELEASE                  4
#define SPX_CAT253_MTI_COMMAND_TOKEN_REQUEST            5
#define SPX_CAT253_MTI_COMMAND_TOKEN_RELEASE            6
#define SPX_CAT253_MTI_COMMAND_TOKEN_ASSIGN             7
#define SPX_CAT253_MTI_COMMAND                          8
#define SPX_CAT253_MTI_COMPLETE_STATUS_TRANSFER         9
#define SPX_CAT253_MTI_DELTA_STATUS_TRANSFER            10
#define SPX_CAT253_MTI_COMPLETE_STATUS_TRANSFER_REQUEST 11
#define SPX_CAT253_MTI_CENTRE_EXCHANGE                  12
#define SPX_CAT253_MTI_CENTRE_STATUS                    13
#define SPX_CAT253_MTI_CENTRE_TRANSPARENT               14
#define SPX_CAT253_MTI_CENTRE_RESYNC_REQUEST            15
#define SPX_CAT253_MTI_RADAR_MONITORING_START           16
#define SPX_CAT253_MTI_RADAR_MONITORING_STOP            17
#define SPX_CAT253_MTI_ACKNOWLEDGE                      18
#define SPX_CAT253_MTI_ERROR                            19
#define SPX_CAT253_MTI_ALARM                            20
#define SPX_CAT253_MTI_ALIVE                            21

/* Message type identifier priority and delivery bits that
 * can be ORed together with the MTI value in I253/040.
 */
#define SPX_CAT253_MTI_HIGH_PRIORITY                    (1 << 7)
#define SPX_CAT253_MTI_EXPLICT_ACK                      (1 << 8)

/*********************************************************************
*
*   Macros
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
struct SPxTime_tag;

class SPxNetCat253
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */

    /* Constructor, destructor etc. */
    SPxNetCat253(void);
    virtual ~SPxNetCat253();

    /* Configuration functions. */
    virtual SPxErrorCode SetSourceSIC(UINT8 sic) 
    { 
        m_srcSIC = sic; 
        return SPX_NO_ERROR; 
    }
    virtual UINT8 GetSourceSIC(void) { return m_srcSIC; }

    virtual SPxErrorCode SetSourceSAC(UINT8 sac) 
    { 
        m_srcSAC = sac; 
        return SPX_NO_ERROR; 
    }
    virtual UINT8 GetSourceSAC(void) { return m_srcSAC; }

    virtual SPxErrorCode SetSourceLocalID(UINT8 localID) 
    { 
        m_srcLocalID = localID; 
        return SPX_NO_ERROR; 
    }
    virtual UINT8 GetSourceLocalID(void) { return m_srcLocalID; }

    virtual SPxErrorCode SetDestSIC(UINT8 sic) 
    { 
        m_dstSIC = sic; 
        return SPX_NO_ERROR; 
    }
    virtual UINT8 GetDestSIC(void) { return m_dstSIC; }

    virtual SPxErrorCode SetDestSAC(UINT8 sac) 
    { 
        m_dstSAC = sac; 
        return SPX_NO_ERROR; 
    }
    virtual UINT8 GetDestSAC(void) { return m_dstSAC; }

    virtual SPxErrorCode SetDestLocalID(UINT8 localID) 
    { 
        m_dstLocalID = localID; 
        return SPX_NO_ERROR; 
    }
    virtual UINT8 GetDestLocalID(void) { return m_dstLocalID; }

    virtual SPxErrorCode SetFilterOnSACEnabled(int isEnabled)
    {
        m_filterOnSACEnabled = isEnabled;
        return SPX_NO_ERROR; 
    }
    virtual int IsFilterOnSACEnabled(void) 
    { 
        return m_filterOnSACEnabled; 
    }

    virtual SPxErrorCode SetFilterOnSICEnabled(int isEnabled)
    {
        m_filterOnSICEnabled = isEnabled;
        return SPX_NO_ERROR; 
    }
    virtual int IsFilterOnSICEnabled(void) 
    { 
        return m_filterOnSICEnabled; 
    }

    virtual SPxErrorCode SetFilterOnLocalIDEnabled(int isEnabled)
    {
        m_filterOnLocalIDEnabled = isEnabled;
        return SPX_NO_ERROR; 
    }
    virtual int IsFilterOnLocalIDEnabled(void) 
    { 
        return m_filterOnLocalIDEnabled; 
    }

    virtual SPxErrorCode SetIncludeTimeOfDay(int include)
    {
        m_includeTimeOfDay = include;
        return SPX_NO_ERROR; 
    }
    virtual int IsTimeOfDayIncluded(void) { return m_includeTimeOfDay; }

    virtual SPxErrorCode SetIncludeSequenceID(int include)
    {
        m_includeSequenceID = include;
        return SPX_NO_ERROR; 
    }
    virtual int IsSequenceIDIncluded(void) { return m_includeSequenceID; }

    virtual SPxErrorCode SetNullTermAppDataEnabled(int isEnabled)
    {
        m_nullTermAppDataEnabled = isEnabled;
        return SPX_NO_ERROR;
    }
    virtual int IsNullTermAppDataEnabled(void) 
    { 
        return m_nullTermAppDataEnabled; 
    }

    virtual SPxErrorCode SetMTI(UINT8 mti, 
                                int isHighPriority=FALSE,
                                int isExplicitAck=FALSE)
    {
        m_mti = mti & 0x3F;
        m_mti |= isExplicitAck ? 0x40 : 0x00;
        m_mti |= isHighPriority ? 0x80 : 0x00;
        return SPX_NO_ERROR; 
    }
    virtual UINT8 GetMTI(void) { return (m_mti & 0x3F); }
    virtual int IsMTIHighPriority(void) { return (m_mti & 0x80) ? TRUE : FALSE; }
    virtual int IsMTIExplictAck(void) { return (m_mti & 0x40) ? TRUE : FALSE; }

    virtual SPxErrorCode SetAutoPollMesgs(int isEnabled)
    {
        m_autoPollMesgs = isEnabled;
        return SPX_NO_ERROR; 
    }
    virtual int IsAutoPollMesgsEnabled(void) 
    { 
        return m_autoPollMesgs; 
    }

    /* Sent data. */
    virtual UINT16 GetLastSentSequenceID(void);

    /* Received data. */
    virtual SPxErrorCode GetLastSource(UINT8 *sacRtn,
                                       UINT8 *sicRtn,
                                       UINT8 *localIdRtn,
                                       UINT32 expiryMsecs=0);

    virtual SPxErrorCode GetLastDest(UINT8 *sacRtn,
                                     UINT8 *sicRtn,
                                     UINT8 *localIdRtn,
                                     UINT32 expiryMsecs=0);

    virtual SPxErrorCode GetLastTimeOfDayMsecs(UINT32 *timeOfDayRtn, 
                                               UINT32 expiryMsecs=0);

    virtual SPxErrorCode GetLastSequenceID(UINT16 *sequenceIDRtn,
                                           UINT32 expiryMsecs=0);

    virtual SPxErrorCode GetLastMTI(UINT8 *mtiRtn,
                                    int *isHighPriorityRtn=NULL,
                                    int *isExplicitAckRtn=NULL,
                                    UINT32 expiryMsecs=0);

    /* Install callback that will be invoked when message is received. */
    virtual SPxErrorCode SetMesgHandler(void (*fn)(SPxNetCat253 *obj,
                                                   void *arg,
                                                   const UINT8 *appData,
                                                   unsigned int numAppDataBytes),
                                        void *arg)
    {
        m_fnAppDataArg = arg;
        m_fnAppData = fn;
        return SPX_NO_ERROR;
    }

protected:

    /*
     * Protected fields.
     */

    /* Send buffer. */
    UINT8 *m_sendBuf;                   /* Send buffer. */
    unsigned int m_sendBufBytes;        /* Send buffer size. */
    SPxCriticalSection m_sendMutex;     /* Send mutex. */

    /* Receive buffer. */
    UINT8 *m_recvBuf;                   /* Recv buffer. */
    unsigned int m_recvBufBytes;        /* Recv buffer size. */
    unsigned int m_recvDataBytes;       /* Amount of data in recv buffer. */
    SPxCriticalSection m_recvMutex;     /* Recv mutex. */    

    /* Application data buffer. */
    UINT8 *m_appDataBuf;                /* Application data buffer. */
    unsigned int m_appDataBufBytes;     /* Application data buffer size. */
    unsigned int m_lastAppDataBytes;    /* Size of last app data recv'd. */

    /*
     * Protected functions.
     */

    SPxErrorCode WriteMesgToSendBuf(UINT8 *data, 
                                    unsigned int numDataBytes,
                                    unsigned int bufOffset,
                                    unsigned int *numMesgBytesRtn,
                                    UINT32 debug=0,
                                    FILE *logFile=NULL);

    unsigned int CopyDataToRecvBuf(const unsigned char *data,
                                   unsigned int numBytes,
                                   int allOrNothing);

    void ProcessRecvData(UINT32 debug=0,
                         FILE *logFile=NULL);

    unsigned int HandleMesg(unsigned char *data,
                            unsigned int numBytes,
                            UINT32 debug=0,
                            FILE *logFile=NULL);
    
private:

    /*
     * Private fields.
     */

    /* Configurable items. */
    int m_includeTimeOfDay;        /* Include Time of Day in sent mesg? */
    int m_includeSequenceID;       /* Include Sequence ID in sent mesg? */
    UINT8 m_srcSAC;                /* Source System Area Code. */
    UINT8 m_srcSIC;                /* Source System Identification Code.*/
    UINT8 m_srcLocalID;            /* Source Local ID. */
    UINT8 m_dstSAC;                /* Dest System Area Code. */
    UINT8 m_dstSIC;                /* Dest System Identification Code. */
    UINT8 m_dstLocalID;            /* Dest Local ID. */
    int m_filterOnSACEnabled;      /* Filter received messages using SAC. */
    int m_filterOnSICEnabled;      /* Filter received messages using SIC. */
    int m_filterOnLocalIDEnabled;  /* Filter received msgs using Local ID. */
    int m_nullTermAppDataEnabled;  /* Enable to null terminate data. */
    int m_autoPollMesgs;           /* Enable to poll for mesgs in thread. */

    /* Sent data. */
    UINT16 m_sequenceID;            /* Next sequence number to send. */
    UINT8 m_mti;                    /* Message Type Identifier to send. */

    /* Received data. */
    UINT8 m_lastSrcSAC;             /* Received source System Area Code. */
    UINT8 m_lastSrcSIC;             /* Received source System Ident Code. */
    UINT8 m_lastSrcLocalID;         /* Received source Local ID. */
    UINT8 m_lastDstSAC;             /* Received dest System Area Code. */
    UINT8 m_lastDstSIC;             /* Received dest System Ident Code. */
    UINT8 m_lastDstLocalID;         /* Received dest Local ID. */
    UINT32 m_lastTimeOfDayMsecs;    /* Received time-of-day. */
    int m_isLastTimeOfDaySet;       /* True if last mesg had time-of-day. */
    UINT16 m_lastSequenceID;        /* Last sequence number received. */
    int m_isLastSequenceIDSet;      /* True if last mesg had sequence ID. */
    UINT8 m_lastMTI;                /* Last received MTI. */
    int m_isLastMTISet;             /* True if last mesg had MTI. */

    /* Message timestamps. */
    UINT32 m_lastRxMesgTimeMsecs;   /* Time of last received mesg. */
    int m_isLastRxMesgTimeSet;      /* True if received mesg timestamp set. */
    UINT32 m_lastTxMesgTimeMsecs;   /* Time of last sent mesg. */
    UINT32 m_lastLogFileFlushMsecs; /* Last log file flush. */

    /* Application message callback. */
    void *m_fnAppDataArg;
    void (*m_fnAppData)(SPxNetCat253 *obj,
                        void *arg,
                        const UINT8 *appData,
                        unsigned int numBytes);

    /*
     * Private functions.
     */

    UINT32 get128thSecsSinceMidnight(SPxTime_tag *timestamp);
    void checkSequenceID(UINT16 sequenceID, UINT32 debug, FILE *logFile);

    /*
     * Private static functions.
     */

}; /* SPxNetCat253 class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NET_CAT_253_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
