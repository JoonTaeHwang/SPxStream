/*********************************************************************
*
* (c) Copyright 2014, 2015, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxNetReplay.h,v $
* ID: $Id: SPxNetReplay.h,v 1.2 2015/11/03 11:34:12 rew Exp $
*
* Purpose:
*	Header for network packet replay class.
*
*
* Revision Control:
*   03/11/15 v1.2    AGC	Packet decoder callback data now const.
*
* Previous Changes:
*   18/07/14 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_NETREPLAY_H
#define _SPX_NETREPLAY_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr class. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* We need base class. */
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
struct SPxTime_tag;
struct SPxPacketNet_tag;
class SPxThread;

/*
 * Define our network recorder class.
 */
class SPxNetworkReplay :public SPxObj
{
public:
    typedef void (*StreamFn)(unsigned int stream,
	const unsigned char *data, unsigned int numBytes, void *userArg);

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    explicit SPxNetworkReplay(void);
    virtual ~SPxNetworkReplay(void);

    void EnableOutput(int enable);
    int IsOutputEnabled(void) const;

    void SetNumClientsWanted(unsigned int numClientsWanted);
    unsigned int GetNumClientsWanted(void) const;

    SPxErrorCode OverrideAddress(unsigned int streamIndex, const char *addr);

    int IsPaused(void) const;

    /* Define the basic controls. */
    int SetFileName(const char *fileName);
    const char *GetFileName(void) const;
    const char *GetFullPath(void) const;
    virtual void Pause(void);
    virtual void Play(void);
    virtual void Rewind(void);

    /* Flag to say replay should automatically loop again at end of file. */
    void SetAutoLoop(int enable);
    int GetAutoLoop(void) const;

    /* Speed of replay */
    void SetSpeedupFactor(double factor);
    double GetSpeedupFactor(void) const;

    /* Table of contents and searching functions. */
    int IsTOCAvailable(void) const;
    SPxErrorCode GotoFileTime(const struct SPxTime_tag *timestamp);
    SPxErrorCode GotoFileTime(UINT32 secs);
    SPxErrorCode GotoFileTimePercent(double percent);

    /* File time retrieval. */
    void GetFileTimeStart(struct SPxTime_tag *ptr, int inHandler=FALSE) const;
    void GetFileTimeCur(struct SPxTime_tag *ptr, int inHandler=FALSE) const;
    void GetFileTimeEnd(struct SPxTime_tag *ptr, int inHandler=FALSE) const;
    SPxErrorCode GetFileTimePercent(double *percentPtr) const;
    
    /* Base class status function. */
    virtual SPxErrorCode GetInitialised(void) const;

    /* For use by other SPx objects to get direct access to packets. */
    SPxPacketDecoderFile *GetPacketDecoder(void);

    SPxErrorCode AddHandler(StreamFn fn, void *userArg);
    SPxErrorCode RemoveHandler(StreamFn fn, void *userArg);

    SPxErrorCode ProcessNextPacket(UINT32 *sleepTimeMsecs,
	int mutexAlreadyLocked=FALSE, int useTiming=TRUE);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Packet handlers. */
    SPxErrorCode addStream(struct SPxPacketNet_tag *netHdr);
    SPxErrorCode clearStreams(void);
    static void pktFnWrapper(SPxPacketDecoder *decoder,
	void *userArg, UINT32 packetType, struct sockaddr_in *from,
	struct SPxTime_tag *time, const unsigned char *payload,
	    unsigned int numBytes);
    void pktFn(const unsigned char *payload, unsigned int numBytes);
    static void *threadFnWrapper(SPxThread *thread);
    void *threadFn(SPxThread *thread);
 
}; /* SPxNetworkReplay class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NETREPLAY_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
