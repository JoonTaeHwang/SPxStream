/**
 * \copyright	(c) Copyright 2010 - 2019, Cambridge Pixel Ltd.
 *
 * \brief	Declares the SPxImgSendStream class. 
 *		An SPxImgSendManager class will contain zero
 *		or more of these channels.
 *
 * \revision
 *  28/03/19 v1.17   AGC	Rename SetRadarLUT lutId parameter to rid.
 *
 * \changes
 *  13/11/17 1.16   AGC	Rename private class.
 *  01/04/14 1.15   AGC	Add const to various parameters.
 *  26/03/14 1.14   AGC	Move static const definitions to source file.
 *  05/03/14 1.13   AGC	Add file rotation support.
 *  04/10/13 1.12   AGC	Simplify headers.
 *  05/09/13 1.11   AGC	Allocate SPxCompress object.
 *  25/04/13 1.10   AGC	Use new SPxCompress class.
 *  31/08/12 1.9    AGC	Reduce header dependencies.
 *  02/09/11 1.8    SP 	Support QNX build.
 *  09/06/11 1.7    AGC	Add casts to GetWidth()/GetHeight().
 *  28/10/10 1.6    AGC	Add void to SetConfigFromState() arg list.
 *  13/10/10 1.5    AGC	Fix SPxImgSendStreamObjectInfo structure.
 *  11/10/10 1.4    AGC	Remove unnecessary GetRadarScreenX/Y functions.
 *			Include north crossing in data rate calcs. 
 *  05/10/10 1.3    AGC	Avoid icc warnings.
 *  04/10/10 1.2    REW	Avoid icc warnings.
 *  04/10/10 1.1    AGC	Initial version. 
 */
#ifndef _SPX_IMG_SEND_STREAM_H
#define _SPX_IMG_SEND_STREAM_H

/*
 * Other headers required.
 */

/* Standard headers */

/* We need SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* We need SPxCommon for common types. */
#include "SPxLibUtils/SPxCommon.h"

/* We need SPxErrorCode enumeration. */
#include "SPxLibUtils/SPxError.h"

/* We need SPxTime_t structure. */
#include "SPxLibUtils/SPxTime.h"

/* We need SPxRadarFadeMode_t enumeration. */
#include "SPxLibSc/SPxSc.h"

/* We need DirtyBox structure. */
#include "SPxLibSc/SPxScDestDisplay.h"

/* For SPxObj base class. */
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Debugging */
#define SPX_IMG_SEND_STREAM_DEBUG_IMAGE		0x00010000	/* Image distribution */
#define SPX_IMG_SEND_STREAM_DEBUG_METADATA	0x00020000	/* Metadata distribution */
#define SPX_IMG_SEND_STREAM_DEBUG_FILE		0x00040000	/* File saving */

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

/** File save modes. */
enum SPxImgStreamSaveMode_t
{
    SPX_IMG_STREAM_SAVE_MODE_NONE	= 0,
    SPX_IMG_STREAM_SAVE_MODE_SINGLE	= 1,
    SPX_IMG_STREAM_SAVE_MODE_SEQUENCE	= 2,
    SPX_IMG_STREAM_SAVE_MODE_ROTATE	= 3
};

/** Compression types - RLE is run-length encoding. */
enum SPxImgStreamCompression_t
{
    SPX_IMG_STREAM_COMPRESSION_NONE	= 0,
    SPX_IMG_STREAM_COMPRESSION_RLE	= 1,
    SPX_IMG_STREAM_COMPRESSION_ZLIB	= 2
};

/** Image streaming modes - NONE disables streaming. */
enum SPxImgStreamMode_t
{
    SPX_IMG_STREAM_MODE_NONE	    = 0,
    SPX_IMG_STREAM_MODE_AUTO	    = 1,
    SPX_IMG_STREAM_MODE_MINIMAL	    = 2,
    SPX_IMG_STREAM_MODE_QUADRANT    = 3,
    SPX_IMG_STREAM_MODE_WHOLE	    = 4
};

/** Image streaming flags - describe radar. */
enum SPxImgStreamFlags_t
{
    SPX_IMG_STREAM_FLAGS_NONE	    = 0,
    SPX_IMG_STREAM_FLAGS_RAW	    = (1<<0),
    SPX_IMG_STREAM_FLAGS_PROCESSED  = (1<<1)
};

/** Type of stream interval. */
enum SPxImgStreamIntervalType_t
{
    SPX_IMG_STREAM_INTERVAL_AZIMUTH	= 0,
    SPX_IMG_STREAM_INTERVAL_TIME	= 1
};

/* Forward declarations. */
class SPxCompress;
class SPxThread;
class SPxImgSendManager;
class SPxImgSendStream;
class SPxRadarStream;
class SPxPIM;
class SPxScDestBitmap;
class SPxScSourceLocal;
class SPxRunProcess;
class SPxPacketSender;

/** Structure to describe each of the distribution
 *  streams - Normal image distribution, saving
 *  to file, and metadata distribution.
 */
struct SPxImgSendStreamObjectInfo
{
    SPxImgStreamIntervalType_t intervalType;
    UINT32 intervalAzimuth;
    UINT16 lastOutputAz;
    UINT32 intervalMSecs;
    UINT32 lastOutputTicker;
    UINT32 lastIntervalMSecs;
    UINT16 completeRevs;
    UINT16 prevLastAz;
    int flushed;
    void (*distribute)(SPxImgSendStream *stream);
};

/**
 * Define our class, derived from the SPxObj class.
 */
class SPxImgSendStream :public SPxObj
{
    /* SPxImgSendManager class needs access to internals of this class. */
    friend class SPxImgSendManager;

public:
    /*
     *	Public functions.
     */
    /* Enable/Disable object - disabled object does no
     * distribution or file saving.
     */
    void Enable(int enable=TRUE) { SetActive(enable); }
    void Disable(void)		 { SetActive(FALSE); }

    /* Stream name control. */
    SPxErrorCode SetStreamName(const char *name);
    const char *GetStreamName(void);

    /* Stream information retrieval. */
    UINT16 GetStreamID(void) { return m_streamID; }
    SPxErrorCode SetFlags(UINT32 flags);
    UINT32 GetFlags(void) { return m_flags; }
    UINT16 GetWidth(void);
    UINT16 GetHeight(void);
    double GetRadarEndRange(void);

    /* Address control. */
    SPxErrorCode SetAddress(const char *addr, int port=0, const char *ifAddr=NULL);
    SPxErrorCode SetAddress(UINT32 addr, int port=0, const char *ifAddr=NULL);
    SPxErrorCode SetPortNumber(int port);
    SPxErrorCode SetPort(int port) { return SetPortNumber(port); }
    UINT32 GetAddress(void);
    int GetPort(void);
    UINT32 GetIfAddress(void);
    
    /* Region to distribute control. */
    SPxErrorCode SetView(REAL32 x, REAL32 y, REAL32 w, REAL32 h);
    SPxErrorCode GetView(REAL32 *x, REAL32 *y, REAL32 *w, REAL32 *h);

    /* Fade control. */
    SPxErrorCode SetFade(SPxRadarFadeMode_t fadeMode, int fadeRate);
    SPxRadarFadeMode_t GetFadeMode(void) { return m_fadeMode; }
    int GetFadeRate(void) { return m_fadeRate; }

    /* Radar control. */
    SPxErrorCode SetRadarColour(UCHAR red, UCHAR green, UCHAR blue);
    SPxErrorCode GetRadarColour(UCHAR *red, UCHAR *greem, UCHAR *blue);
    SPxErrorCode SetRadarBright(UCHAR bright);
    UCHAR GetRadarBright(void) { return m_radarBright; }
    SPxErrorCode SetRadarLUT(UCHAR rid, UCHAR * lutData);
    SPxErrorCode ShowRadar(UCHAR state);
    UCHAR GetShowRadar(void) { return m_showRadar; }
    SPxErrorCode SetRadarPosition(REAL32 radarX, REAL32 radarY);
    void GetRadarPosition(double *radarX, double *radarY);

    /* Image distribution control. */
    /* Stream mode - none, quadrant, whole, minimal. */
    SPxErrorCode SetImageStreamMode(SPxImgStreamMode_t streamMode);
    SPxImgStreamMode_t GetImageStreamMode(void) { return m_streamMode; }
    /* Stream interval type. */
    SPxErrorCode SetImageStreamIntervalType(SPxImgStreamIntervalType_t intervalType);
    SPxImgStreamIntervalType_t GetImageStreamIntervalType(void);
    /* Stream interval seconds. */
    SPxErrorCode SetImageStreamIntervalSeconds(double seconds, int setIntervalType=TRUE);
    double GetImageStreamIntervalSeconds(void);
    /* Stream interval azimuths. */
    void SetImageStreamIntervalAz(unsigned int azimuths, int setIntervalType=TRUE);
    unsigned int GetImageStreamIntervalAz(void);
    
    /* Compression control. */
    SPxErrorCode SetCompression(SPxImgStreamCompression_t compression);
    SPxImgStreamCompression_t GetCompression(void) { return m_compression; }

    /* File saving control. */
    /* Save mode - none, single, sequence, rotate. */
    SPxErrorCode SetFileSaveMode(SPxImgStreamSaveMode_t mode); 
    SPxImgStreamSaveMode_t GetFileSaveMode(void) { return m_fileSaveMode; }
    /* File format. */
    SPxErrorCode SetFileFormat(SPxFileFormat_t format);
    SPxFileFormat_t GetFileFormat(void) { return m_fileFormat; }
    /* File save interval type. */
    SPxErrorCode SetFileSaveIntervalType(SPxImgStreamIntervalType_t intervalType);
    SPxImgStreamIntervalType_t GetFileSaveIntervalType(void);
    /* File save interval seconds. */
    SPxErrorCode SetFileSaveIntervalSeconds(double seconds, int setIntervalType=TRUE);
    double GetFileSaveIntervalSeconds(void);
    /* File save interval azimuths. */
    SPxErrorCode SetFileSaveIntervalAz(unsigned int azimuths, int setIntervalType=TRUE);
    unsigned int GetFileSaveIntervalAz();
    /* File name. */
    SPxErrorCode SetFileName(const char *name);
    const char *GetFileName(void);
    /* Save now. */
    SPxErrorCode SaveFileNow(const char *name=NULL);
  
    /* Meta data control. */
    SPxErrorCode SetMetadata(const char *name, const char *value);
    const char *GetMetadata(const char *name);
    const char *GetNextMetadataName(const char *prevName);
    SPxErrorCode SetMetadataOutputInterval(unsigned int seconds);
    unsigned int GetMetadataOutputInterval(void);

    /* MTU control. */
    void SetMTU(unsigned int mtu) { m_mtu = mtu; }
    unsigned int GetMTU(void) { return m_mtu; }

    /* Distribution information. */
    unsigned int GetBytesSentPerSecond(void) { return m_bytesSentPerSecond; }
    double GetCompressionFactor(void);

    /* Parameter assignment. */
    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Override base class set config from state. */
    SPxErrorCode SetConfigFromState(void);

    /* Debugging */
    static void SetDebug(UINT32 debug);
    static void SetLogFile(FILE *log) { m_logFile = log; }

    static const unsigned int MAX_METADATA_NAME_LEN;
    static const unsigned int MAX_METADATA_VALUE_LEN;

private:
    /* Private enum for stream info map. */
    static const int STREAM_TYPE_IMAGE;
    static const int STREAM_TYPE_FILE;
    static const int STREAM_TYPE_METADATA;

    /*
     *	Private functions.
     */
    /* Construction and destruction are private - only done by friend class. */
    SPxImgSendStream(SPxImgSendManager *manager,
		     SPxRadarStream *radarStream,
		     UINT16 streamID);
    SPxImgSendStream(SPxImgSendManager *manager,
		     SPxRunProcess *process,
		     SPxPIM *pim,
		     UINT16 streamID);
    void commonInit(SPxImgSendManager *manager, 
		    UINT16 streamID);
    virtual ~SPxImgSendStream(void);
    
    /* Must be called after instantiation. */
    SPxErrorCode Create(int width, int height, int bitmapDepth);

    /* Bitmap change functions - Used when image output type is azimuth based. */
    static void bitmapUpdateWrapper(SPxScDestBitmap *bitmap, 
                                    UINT16 changes,
                                    UINT16 firstAzimuth, 
                                    UINT16 endAzimuth,
                                    DirtyBox box,
                                    void *userPtr);
    void bitmapUpdate(SPxScDestBitmap *bitmap, 
                      UINT16 changes,
                      UINT16 firstAzimuth, 
                      UINT16 endAzimuth,
                      DirtyBox box,
                      void *userPtr);

    /* Distribute the appropriate region of the bitmap. */
    static void distributeImageWrapper(SPxImgSendStream *stream){ stream->distributeImage(); }
    void distributeImage(void);
    void distributeImageRegion(unsigned int regionX, unsigned int regionY,
			       unsigned int regionW, unsigned int regionH);

    /* Distribute metadata. */
    static void distributeMetadataWrapper(SPxImgSendStream *stream){ stream->distributeMetadata(); }
    void distributeMetadata(void);
    
    /* Save to a file. */
    static void saveImageWrapper(SPxImgSendStream *stream){ stream->saveImage(); }
    void saveImage(void);
    void saveImageToFile(const char *name, int mutexAlreadyLocked);

    /* Compression using run-length encoding. */
    SPxErrorCode compressRLE(unsigned char *dest, unsigned long *destSize,
			     unsigned char *src, unsigned int srcSize);

    static void *threadWrapper(SPxThread *thread);
    void* threadFn(SPxThread *thread);
    void writeToSendBuf(unsigned char* data, unsigned int numBytes);
    SPxErrorCode readFromSendBuf(void);
    
    /*
     *	Private fields.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;   /* Private structure. */
    int m_initialised;

    int m_licErrReported;
    SPxTime_t m_lastLicErrReport;

    SPxThread *m_thread;

    /* Data rate fields. */
    unsigned int m_bytesSentPerSecond;
    unsigned int m_bytesSent;
    unsigned int m_bytesToSendPerSecond;
    unsigned int m_bytesToSend;
    UINT32 m_lastBytesSentUpdate;
    UINT32 m_lastDataRateUpdateInterval;
    double m_radarPeriodEst;	/* Estimate of radar period. */
    UINT32 m_lastNorthCrossing;

    /* Stream ID. */
    UINT16 m_streamID;
    
    /* Manager that owns this stream. */
    SPxImgSendManager *m_manager;

    /* Scan converter objects. */
    SPxRadarStream *m_radarStream;
    SPxScDestBitmap *m_destBitmap;
    int m_bitmapDepth;
    SPxScSourceLocal *m_scSource;
    SPxRunProcess *m_scProcess;

    /* Copies of scan conversion parameters that
     * do not have a getter in the SPxScSourcelocal object.
     */
    SPxRadarFadeMode_t m_fadeMode;
    int m_fadeRate;
    UCHAR m_radarRed;
    UCHAR m_radarGreen;
    UCHAR m_radarBlue;
    UCHAR m_radarBright;
    UCHAR m_showRadar;

    /* Region of bitmap not yet distributed. */
    DirtyBox m_dirtyBox;

    /* Buffers. */
    SPxAutoPtr<SPxCompress> m_compress;
    unsigned char *m_bitmapBuf;
    unsigned char *m_regionBuf;
    unsigned char *m_compressBuf;
    unsigned char *m_sendBuf;
    unsigned char *m_sendBufFront;
    unsigned char *m_sendBufBack;
    unsigned int m_bitmapBufSize;
    unsigned int m_regionBufSize;
    unsigned int m_compressBufSize;
    unsigned int m_sendBufSize;
    unsigned int m_mtu;

    /* Packet Sender. */
    SPxPacketSender *m_pktSender;

    /* File settings. */
    SPxImgStreamSaveMode_t m_fileSaveMode;
    SPxFileFormat_t m_fileFormat;
    time_t m_lastFileSaveTime;
    UINT32 m_fileSaveRepeatCount;

    /* Image compression. */
    SPxImgStreamCompression_t m_compression;

    /* Distribution settings and records. */
    UINT16 m_lastAzimuthRecvd;
    SPxImgStreamMode_t m_streamMode;
    UINT16 m_sequenceNum;

    /* Flags. */
    UINT32 m_flags;

    /* Debugging. */
    static FILE *m_logFile;
    static UINT32 m_debug;
};

#endif /* _SPX_IMG_SEND_STREAM_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
