/**
 * \copyright	(c) Copyright 2010 - 2017, Cambridge Pixel Ltd. 
 *
 * \file	$RCSfile: SPxImgRecvManager.h,v $
 * \ID		$Id: SPxImgRecvManager.h,v 1.11 2017/11/13 16:01:59 rew Exp $
 *
 * \brief	Declares the SPxImgRecvManager class, which 
 *		receives image sender manager stream info messages, and
 *		allows listings of the streams available.
 *
 * \revision
 *  13/11/17 v1.11   AGC	Rename private classes.
 *
 * \changes
 *  03/11/15 1.10   AGC	Packet decoder callback data now const.
 *  01/04/14 1.9    AGC	Add const to various parameters.
 *  01/10/12 1.8    AGC	Move SPxImgSendStreamInfo internals to cpp file.
 *  31/08/12 1.7    AGC	Reduce header dependencies.
 *  07/07/11 1.6    AGC	Add version to stream info.
 *  27/06/11 1.5    AGC	Support new stream info fields added for video server.
 *  06/04/11 1.4    AGC	Use new SPxPacketDecoderNet class.
 *  05/10/10 1.3    AGC	Avoid icc warnings.
 *  04/10/10 1.2    REW	Avoid icc warnings.
 *  04/10/10 1.1    AGC	Initial version. 
 */
#ifndef _SPX_IMG_RECV_MANAGER_H
#define _SPX_IMG_RECV_MANAGER_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* We need SPxCommon for common types. */
#include "SPxLibUtils/SPxCommon.h"

/* We need SPxErrorCode enumeration. */
#include "SPxLibUtils/SPxError.h"

/* We need SPxTime_t structure. */
#include "SPxLibUtils/SPxTime.h"

/* For SPxObj base class. */
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Debugging. */
#define SPX_IMG_RECV_MNGR_DEBUG_STREAM_INFO	0x00000001 /* Stream info messages */
#define SPX_IMG_RECV_MNGR_DEBUG_STREAM_NEW	0x00000002 /* New stream observed */
#define SPX_IMG_RECV_MNGR_DEBUG_STREAM_CHANGE	0x00000004 /* Stream has changed */
#define SPX_IMG_RECV_MNGR_DEBUG_STREAM_EXPIRED	0x00000008 /* Stream expired. */

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
class SPxThread;
class SPxPacketDecoderNet;
class SPxPacketDecoder;

/* Flags indicating changes to a stream configuration. */
enum SPxImgRecvManagerFlags_t
{
    SPX_IMG_RECV_MNGR_NONE		    = 0,

    SPX_IMG_RECV_MNGR_NEW_STREAM	    = (1 << 0),
    SPX_IMG_RECV_MNGR_EXPIRED_STREAM	    = (1 << 1),

    SPX_IMG_RECV_MNGR_CHANGE_ADDRESS	    = (1 << 2),
    SPX_IMG_RECV_MNGR_CHANGE_PORT	    = (1 << 3),
    SPX_IMG_RECV_MNGR_CHANGE_FLAGS	    = (1 << 4),
    SPX_IMG_RECV_MNGR_CHANGE_WIDTH	    = (1 << 5),
    SPX_IMG_RECV_MNGR_CHANGE_HEIGHT	    = (1 << 6),
    SPX_IMG_RECV_MNGR_CHANGE_RADAR_RANGE    = (1 << 7),
    SPX_IMG_RECV_MNGR_CHANGE_VIEW	    = (1 << 8),
    SPX_IMG_RECV_MNGR_CHANGE_NAME	    = (1 << 9),
    SPX_IMG_RECV_MNGR_CHANGE_PREVIEW_ADDRESS= (1 <<10),
    SPX_IMG_RECV_MNGR_CHANGE_PREVIEW_PORT   = (1 <<11)
};

/** Class to describe an SPxImgSendManager
 *  that is being described in stream info messages.
 */
class SPxImgSendMngrInfo
{
    friend class SPxImgRecvManager;

public:
    /* This function returns true if this item should be 
     * placed earlier than the other item in a sorted list.
     */
    bool operator < (SPxImgSendMngrInfo other) const
    {
	/* Sort by managerID first, then address, then port. */
	if(managerID < other.GetManagerID())
	{
	    return true;
	}
	else if(managerID == other.GetManagerID())
	{
	    if(address < other.GetAddress())
	    {
		return true;
	    }
	    else if(address == other.GetAddress())
	    {
		if(port < other.GetPort())
		{
		    return true;
		}
	    }
	}
	return false;
    }
    UINT16 GetManagerID(void) const { return managerID; }
    UINT32 GetAddress(void) const { return address; }
    UINT16 GetPort(void) const { return port; }

private:
    UINT16 managerID;
    UINT32 address;
    UINT16 port;
};

/** Class using to describe an SPxImgSendStream
 *  that is being described in stream info messages.
 */
class SPxImgSendStreamInfo
{
    friend class SPxImgRecvManager;

public:
    SPxImgSendStreamInfo(void);
    SPxImgSendStreamInfo(const SPxImgSendStreamInfo&);
    SPxImgSendStreamInfo& operator=(const SPxImgSendStreamInfo&);
    UINT8 GetVersion(void) const;
    UINT16 GetStreamID(void) const;
    UINT32 GetAddress(void) const;
    UINT32 GetFlags(void) const;
    UINT16 GetPort(void) const;
    UINT32 GetPreviewAddress(void) const;
    UINT16 GetPreviewPort(void) const;
    UINT16 GetWidth(void) const;
    UINT16 GetHeight(void) const;
    REAL32 GetViewX(void) const;
    REAL32 GetViewY(void) const;
    REAL32 GetViewW(void) const;
    REAL32 GetViewH(void) const;
    REAL32 GetRadarRangeEnd(void) const;
    const char* GetName(void) const;

private:
    /* Private structure. */
    struct impl;
    SPxAutoPtr<impl> m_p;
};

/**
 * Define our class, derived from the SPxObj class.
 */
class SPxImgRecvManager :public SPxObj
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Construction and destruction. */
    SPxImgRecvManager();
    virtual ~SPxImgRecvManager();

    /* Callback function types. */
    typedef void (*SPxImgRecvManagerFn)(SPxImgRecvManager *recvMngr,
					const SPxImgSendMngrInfo *mngrInfo,
					SPxImgSendStreamInfo *streamInfo,
					UINT32 flags,
					void *userArg);

    /* Set the address/port for receiving stream info messages. */
    SPxErrorCode SetAddress(const char *addr, int port=0, const char *ifAddr=NULL);
    SPxErrorCode SetAddress(UINT32 addr, int port=0, const char *ifAddr=NULL);
    SPxErrorCode SetPort(int port);
    UINT32 GetAddress(void);
    int GetPort(void);
    UINT32 GetIfAddress(void);

    unsigned int GetNumStreams(void);

    /* Set the callback functions. */
    void SetCallbackNewStream(SPxImgRecvManagerFn fn, void *userArg);
    void SetCallbackExpiredStream(SPxImgRecvManagerFn fn, void *userArg);
    void SetCallbackChangeStream(SPxImgRecvManagerFn fn, void *userArg);

    /* Set the expired interval. */
    SPxErrorCode SetExpiredStreamInterval(double seconds);
    double GetExpiredStreamInterval(void) { return m_sndInfoExpiredInterval; }

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

    /* Callback functions and user arguments. */
    SPxImgRecvManagerFn m_newFn;
    SPxImgRecvManagerFn m_expiredFn;
    SPxImgRecvManagerFn m_changeFn;
    void *m_newUserArg;
    void *m_expiredUserArg;
    void *m_changeUserArg;

    /* Decoding thread. */
    SPxThread *m_thread;

    /* Observed server information. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxTime_t m_lastSndInfoPurgeTime;
    double m_sndInfoPurgeInterval;
    double m_sndInfoExpiredInterval;

    /* Packet decoder. */
    SPxPacketDecoderNet *m_pktDecoder;
  
    /*
     * Private functions.
     */
    void purgeExpiredSenderInfo(void);

    /* Thread functions. */
    static void* threadWrapper(SPxThread *thread);
    void* threadFn(SPxThread *thread);

    /* Function to install in packet decoder to receive stream info messages. */
    static void streamInfoReceiverWrapper(SPxPacketDecoder * pktDecoder,
					  void *userArg,
					  UINT32 packetType,
					  struct sockaddr_in *from,
					  SPxTime_t *time,
					  const unsigned char *payload,
					  unsigned int numBytes);
    void streamInfoReceiver(SPxPacketDecoder * pktDecoder,
			    void *userArg,
			    UINT32 packetType,
			    struct sockaddr_in *from,
			    SPxTime_t *time,
			    const unsigned char *payload,
			    unsigned int numBytes);
}; /* SPxImgRecvManager class */

#endif /* _SPX_IMG_RECV_MANAGER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
