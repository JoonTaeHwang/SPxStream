/*********************************************************************
*
* (c) Copyright 2013, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxNetRecord.h,v $
* ID: $Id: SPxNetRecord.h,v 1.2 2015/11/03 11:34:12 rew Exp $
*
* Purpose:
*	Header for network packet recorder class.
*
*
* Revision Control:
*   03/11/15 v1.2    AGC	Packet decoder callback data now const.
*
* Previous Changes:
*   23/12/13 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_NETRECORD_H
#define _SPX_NETRECORD_H

/*
 * Other headers required.
 */
/* For FILE. */
#include <stdio.h>

/* We need SPxLibUtils files for common types, events, errors etc. */
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxTime.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*
 * Debug flags.
 */
#define	SPX_NET_RECORD_DEBUG_SOCKET	0x00000001	/* Socket control */
#define	SPX_NET_RECORD_DEBUG_RECEIPT	0x00000002	/* Data receipt */
#define	SPX_NET_RECORD_DEBUG_VERBOSE	0x80000000	/* Verbose */


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
class SPxCompress;
class SPxPacketDecoder;
class SPxPacketDecoderNet;

/*
 * Define our network recorder class.
 */
class SPxNetworkRecord :public SPxObj
{
public:
    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    SPxNetworkRecord(unsigned int maxStreams=8);
    virtual ~SPxNetworkRecord();

    /* Override the base class SetActive() function. GetActive() is left
     * unchanged.
     */
    virtual void SetActive(int active);

    /* Addition of streams to record. */
    SPxErrorCode AddStream(UINT32 addr, int port, const char *ifAddr=NULL);
    SPxErrorCode AddStream(const char *addr, int port, const char *ifAddr=NULL);
    SPxErrorCode AddStream(SPxPacketDecoderNet *packetDecoder);

    /* Output control. */
    SPxErrorCode SetHeaderless(int headerless);
    int GetHeaderless(void) const	{ return(m_headerless); }
    SPxErrorCode SetDailyFiles(int daily);
    int GetDailyFiles(void) const	{ return(m_dailyFiles); }
    SPxErrorCode SetMaxFileSecs(unsigned int secs);
    unsigned int GetMaxFileSecs(void) const { return(m_maxFileSecs); }
    SPxErrorCode SetMaxFileBytes(unsigned int bytes);
    unsigned int GetMaxFileBytes(void) const { return(m_maxFileBytes); }
    SPxErrorCode SetFilePrefix(const char *prefix);
    const char *GetFilePrefix(void) const { return(m_filePrefix); }
    SPxErrorCode SetCompress(int compress);
    int GetCompress(void) const		{ return(m_compressEnabled); }


    /* Debug support. */
    void SetLogFile(FILE *logFile)	{ m_logFile = logFile; }
    const FILE *GetLogFile(void) const	{ return(m_logFile); }
    void SetDebug(UINT32 debug)		{ m_debug = debug; }
    UINT32 GetDebug(void) const		{ return(m_debug); }

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:
    /*
     * Private fields.
     */
    /* Maximum number of streams we're created for. */
    const unsigned int m_maxStreams;

    /* Critical section protection. */
    SPxCriticalSection m_mutex;

    /* Recording file. */
    FILE *m_file;			/* Handle of file */
    unsigned int m_fileStartSecs;	/* Start time for file */
    int m_fileDay;			/* Start day number for file */
    UINT64 m_fileSizeBytes;		/* Size of file */

    /* List of streams to record. */
    struct SPxNetworkRecordStream_tag *m_streams;

    /* Output control. */
    int m_headerless;			/* Is file data headerless? */
    int m_dailyFiles;			/* New file every day? */
    unsigned int m_maxFileSecs;		/* Maximum length if not 0 */
    unsigned int m_maxFileBytes;	/* Maximum size if not 0 */
    char *m_filePrefix;			/* Optional filename prefix */

    /* Compression. */
    int m_compressEnabled;		/* Flag */
    SPxCompress *m_compressor;		/* Object for compressing data */
    unsigned char *m_compressBuf;	/* Buffer for compressing data */
    unsigned int m_compressBufSize;	/* Size of buffer */

    /* Debug variables. */
    FILE *m_logFile;			/* Destination for debug messages */
    UINT32 m_debug;			/* Debug flags */

    /*
     * Private functions.
     */
    SPxErrorCode addStreamCommon(SPxPacketDecoderNet *packetDecoder,
					int allocatedDecoder);

    /* Packet handlers. */
    static void staticPacketHandler(SPxPacketDecoder *decoder, void *userArg,
					UINT32 packetType,
					struct sockaddr_in *from,
					SPxTime_t *timestamp,
					const unsigned char *payload,
					unsigned int numBytes);
    void packetHandler(SPxPacketDecoder *decoder,
					UINT32 packetType,
					struct sockaddr_in *from,
					SPxTime_t *timestamp,
					const unsigned char *payload,
					unsigned int numBytes);

    /* File functions. */
    SPxErrorCode checkFileIsOpen(SPxTime_t *timestamp);
    SPxErrorCode writeDataToFile(SPxTime_t *timestamp,
				struct SPxNetworkRecordStream_tag *stream,
				const unsigned char *data,
				unsigned int numBytes);
    SPxErrorCode closeTheFile(void);

}; /* SPxNetworkRecord class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NETRECORD_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
