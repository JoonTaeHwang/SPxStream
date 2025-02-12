/**
 * \copyright	(c) Copyright 2010 - 2017, Cambridge Pixel Ltd. 
 *
 * \file	$RCSfile: SPxImgRecvStream.h,v $
 * \ID		$Id: SPxImgRecvStream.h,v 1.13 2017/11/13 16:01:59 rew Exp $
 *
 * \brief	Declares the SPxImgRecvStream class, which 
 *		receives and makes available one channel
 *		of images from an image server.
 *
 * \revision
 *  13/11/17 v1.13   AGC	Rename private class.
 *
 * \changes
 *  03/11/15 1.12   AGC	Packet decoder callback data now const.
 *  01/04/14 1.11   AGC	Add const to various parameters.
 *  25/04/13 1.10   AGC	Use new SPxCompress class.
 *  01/10/12 1.9    AGC	Move SPxMetadataInfo to cpp file.
 *  31/08/12 1.8    AGC	Reduce header dependencies.
 *  16/12/11 1.7    AGC	Add SPX_IMG_RECV_METADATA_OPTIONS.
 *  17/11/11 1.6    AGC	Use const char * for commonInit().
 *  07/07/11 1.5    AGC	Support v2 stream info messages.
 *			Use format to determine data type.
 *  06/04/11 1.4    AGC	Use new SPxPacketDecoderNet class.
 *  11/10/10 1.3    AGC	Remove simplest constructor.
 *  04/10/10 1.2    REW Avoid icc warnings.
 *  04/10/10 1.1    AGC Initial version. 
 */
#ifndef _SPX_IMG_RECV_STREAM_H
#define _SPX_IMG_RECV_STREAM_H

/*
 * Other headers required.
 */

/* We need SPxCommon for common types. */
#include "SPxLibUtils/SPxCommon.h"

/* We need the SPxErrorCode enumeration. */
#include "SPxLibUtils/SPxError.h"

/* We need this for image enumerations. */
#include "SPxLibNet/SPxImgSendStream.h"

/* For SPxObj base class. */
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Debugging. */
#define SPX_IMG_RECV_STREAM_DEBUG_IMAGE	    0x00010000 /* Image region received */
#define SPX_IMG_RECV_STREAM_DEBUG_METADATA  0x00020000 /* Metadata received */

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

enum SPxImgRecvMetadataFlags_t
{
    SPX_IMG_RECV_METADATA_NONE	    =   0,
    SPX_IMG_RECV_METADATA_NEW	    =   (1 << 0),
    SPX_IMG_RECV_METADATA_EXPIRED   =	(1 << 1),
    SPX_IMG_RECV_METADATA_CHANGE    =	(1 << 2),
    SPX_IMG_RECV_METADATA_OPTIONS   =	(1 << 3)
};

/* Forward declarations. */
struct SPxTime_tag;
class SPxCompress;
class SPxThread;
class SPxPacketDecoderNet;
class SPxPacketDecoder;
class SPxImgRecvStream;
class SPxImgSendStreamInfo;

/* Callback function types. */
typedef void (*SPxImgRecvStreamImgChangeFn)(SPxImgRecvStream *recv,
					    unsigned int x,
					    unsigned int y,
					    unsigned int w,
					    unsigned int h,
					    void *userArg);

typedef void (*SPxImgRecvStreamMetadataChangeFn)(SPxImgRecvStream *recv,
						 UINT32 flags,
						 const char *name,
						 const char *oldValue,
						 const char *newValue,
						 void *userArg);


/**
 * Define our class, derived from the SPxObj class.
 */
class SPxImgRecvStream :public SPxObj
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Construction and destruction. */
    SPxImgRecvStream(SPxImgSendStreamInfo *streamInfo, char *ifAddr=NULL,
	unsigned char *buffer=NULL, unsigned int numBytes=0);
    SPxImgRecvStream(const char *address, int port=0, const char *ifAddr=NULL,
	unsigned char *buffer=NULL, unsigned int numBytes=0);
    SPxImgRecvStream(UINT32 address, int port=0, const char *ifAddr=NULL,
	unsigned char *buffer=NULL, unsigned int numBytes=0);
    virtual ~SPxImgRecvStream();
    static int Delete(SPxImgSendStreamInfo *streamInfo);

    /* Address functions. */
    SPxErrorCode SetAddress(const char *address, int port=0, const char *ifAddr=NULL);
    SPxErrorCode SetAddress(UINT32 address, int port=0, const char *ifAddr=NULL);
    SPxErrorCode SetPort(int port);
    UINT32 GetAddress(void);
    int GetPort(void);

    /* Image input information. */
    unsigned int GetWidth(void) { return m_width; }
    unsigned int GetHeight(void) { return m_height; }

    /* Image output configuration. */
    SPxErrorCode SetOutputSize(unsigned int width, 
			       unsigned int height);
    unsigned int GetOutputWidth(void) { return m_outputWidth; }
    unsigned int GetOutputHeight(void) { return m_outputHeight; }

    /* Image callback. */
    void SetCallbackImageChange(SPxImgRecvStreamImgChangeFn fn,
			        void *userArg);

    /* Image data retrieval. */
    unsigned char *GetBuffer(void){ return m_outBuf; }

#ifdef _WIN32
    HBITMAP GetHBitmap(void) { return m_destBitmap; }
#endif

    /* Metadata retrieval. */
    int CheckMetadata(const char *name);
    SPxErrorCode GetMetadata(const char *name, char *valueBuf, 
			     unsigned int bufLength);
    void IterateMetadata(void);

    /* Metadata callback. */
    void SetCallbackMetadataChange(const char *name,
				   SPxImgRecvStreamMetadataChangeFn fn,
				   void *userArg,
				   int invokeCallbacks=TRUE);

    SPxErrorCode SetMetadataExpiredInterval(double seconds);
    double GetMetadataExpiredInterval(void) { return m_metadataExpiredInterval; }

    /* Parameter assignment. */
    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    static void SetDebug(UINT32 debug) { m_debug = debug; }
    static void SetLogFile(FILE *log) { m_logFile = log; }

protected:
    /*
     * Protected fields
     */
    static FILE *m_logFile;		/* Log file. */
    static UINT32 m_debug;		/* Flags for debugging. */
 
private:
    /*
     * Private fields
     */
    struct impl;
    SPxAutoPtr<impl> m_p;   /* Private structure. */
    SPxThread *m_thread;
    SPxPacketDecoderNet *m_imageDecoder;
    SPxCompress *m_compress;

    /* Buffers. */
    unsigned char *m_compressedBuf;
    unsigned char *m_regionBuf;
    unsigned char *m_imageBuf;
    unsigned int m_compressedBufSize;
    unsigned int m_regionBufSize;
    unsigned int m_imageBufSize;

    /* Output buffer (may be user supplied). */
    int m_userBuffer;
    unsigned char *m_outBuf;
    unsigned int m_outBufSize;

    /* Reassembly of patches */
    unsigned int m_reassemblyChunksSoFar;
    unsigned int m_reassemblyChunksTotal;
    unsigned int m_reassemblySequence;

    /* Received image size. */
    unsigned int m_width;
    unsigned int m_height;

    /* Output image size. */
    unsigned int m_outputWidth;
    unsigned int m_outputHeight;

    /* Installed callback functions/userargs */
    SPxImgRecvStreamImgChangeFn m_imageChangeFn;
    void *m_imageChangeUserArg;

    /* Metadata information. */
    SPxTime_tag m_lastMetadataPurgeTime;
    double m_metadataPurgeInterval;
    double m_metadataExpiredInterval;

#ifdef _WIN32
    /* For rescaling images using StretchBlt */
    HDC m_srcHDC;
    HDC m_destHDC;
    HBITMAP m_srcBitmap;
    HBITMAP m_destBitmap;
    BITMAPINFOHEADER m_srcBitmapHdr;
    BITMAPINFOHEADER m_destBitmapHdr;
#endif
  
    /*
     * Private functions.
     */
    /* Common initialiser. */
    void commonInit(const char *address, int port, const char *ifAddr, 
	unsigned char *buffer, unsigned int numBytes);

    /* Thread functions for receiving data. */
    static void* threadWrapper(SPxThread *thread);
    void* threadFn(SPxThread *thread);

    /* Keep metadata data structure clean. */
    void purgeExpiredMetadata(void);

    /* Receive image region packets. */
    static void receiveImageRegionsWrapper(SPxPacketDecoder *pktDecoder,
					   void *userArg,
					   UINT32 packetType,
					   struct sockaddr_in *from,	
					   SPxTime_tag *time,		
					   const unsigned char *payload,
					   unsigned int numBytes);
    void receiveImageRegions(SPxPacketDecoder *pktDecoder,
			     void *userArg,
			     UINT32 packetType,
			     struct sockaddr_in *from,	
			     SPxTime_tag *time,		
			     const unsigned char *payload,
			     unsigned int numBytes);
    
    void decompressImage(unsigned int compression, UINT8 format,
			 unsigned int compressedBytes,
			 unsigned int regionX, unsigned int regionY, 
			 unsigned int regionW, unsigned int regionH);
    void scaleAndOutputImage(unsigned int regionX,
			     unsigned int regionY,
			     unsigned int regionW,
			     unsigned int regionH,
			     int mutexAlreadyLocked=FALSE);

    /* Receive metadata packets. */
    static void receiveMetadataWrapper(SPxPacketDecoder *pktDecoder,
				       void *userArg,
				       UINT32 packetType,
				       struct sockaddr_in *from,	
				       SPxTime_tag *time,		
				       const unsigned char *payload,
				       unsigned int numBytes);
    void receiveMetadata(SPxPacketDecoder *pktDecoder,
			 void *userArg,
			 UINT32 packetType,
			 struct sockaddr_in *from,	
			 SPxTime_tag *time,		
			 const unsigned char *payload,
			 unsigned int numBytes);

    /* Decompression using run-length encoding. */
    SPxErrorCode uncompressRLE(unsigned char *dest, unsigned long *destSize,
		   	       unsigned char *src, unsigned int srcSize);

}; /* SPxImgRecvStream class */

#endif /* _SPX_IMG_RECV_STREAM_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
