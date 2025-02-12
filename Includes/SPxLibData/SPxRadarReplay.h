/*********************************************************************
*
* (c) Copyright 2007 - 2018, 2021, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header file for main Radar Replay class.
*
*
* Revision Control:
*   05/04/21 v1.36   AGC	Support file list retrieval.
*
* Previous Changes:
*   03/03/21 1.35   REW	Support replay of Doppler data.
*   03/02/21 1.34   REW	Support SetSuppressReplayedErrors().
*   22/11/18 1.33   AGC	Derive from SPxRadarReplayBase.
*   25/06/18 1.32   AGC	Correctly suppress cppcheck warning.
*   05/04/18 1.31   AGC	Multi-channel improvements.
*   26/09/17 1.30   REW	Add optional arg to Step().
*   08/03/17 1.29   AGC	Add Shutdown() function for derived testing classes.
*   31/01/17 1.28   AGC	Support multiple nav data objects
*			for each channel.
*   04/03/16 1.27   SP 	Add GetChannelRIB() and GetChannelNavData().
*   04/02/16 1.26   AGC	Support installing step check functions.
*   01/02/16 1.25   AGC	Add Step().
*   11/01/16 1.24   SP 	Add Set/GetAutoRewind().
*   16/11/15 1.23   SP 	Add Set/GetAutoLink().
*   03/11/15 1.22   AGC	Packet decoder callback data now const.
*   10/07/15 1.21   AGC	Let SPxPacketDecoderFile handle errors.
*   05/02/15 1.20   SP 	Add multi-channel support.
*			Add InstallDefaultHandler().
*   02/06/14 1.19   AGC	Add nav data replay options.
*   04/10/13 1.18   AGC	Simplify headers.
*   25/04/13 1.17   AGC	Use new SPxCompress class.
*   18/04/12 1.16   REW	Add inHandler arg to the GetFileTime functions.
*   30/08/11 1.15   SP 	Change decodeReturn() arg.
*   08/07/11 1.14   REW	Add GetPacketDecoder().  Tidy line endings.
*   06/04/11 1.13   AGC	Refactor to use SPxPacketDecoderFile.
*   17/12/10 1.12   AGC	Use SPxPath for storing filename.
*   26/03/10 1.11   REW	Add InstallDataFn().
*   21/12/09 1.10   REW	SetFileName() takes a const.
*   17/08/09 1.9    REW	Add Get/SetAutoLoop() and more Get functions.
*   19/06/09 1.8    REW	Support table of contents and goto times.
*   09/02/09 1.7    REW	Implement GetInitialised().
*   06/02/09 1.6    REW	Add m_ribFull flag and support.
*   13/11/08 1.5    REW	Add Get/SetParameter() support.
*   18/07/08 1.4    REW	Add support for file time retrieval.
*   07/07/08 1.3    REW	Keep thisPacketTime and lastReturnTime.
*   26/06/08 1.2    REW	Add GetFileName() access function.
*   11/12/07 1.1    REW	Initial Version from SPxRadarReplayOldHdr.
**********************************************************************/

#ifndef _SPX_RADARREPLAY_H
#define _SPX_RADARREPLAY_H

/*
 * Other headers required.
 */
/* We need SPXxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxTime.h"

/* We need the base class header. */
#include "SPxLibData/SPxRadarReplayBase.h"

/* We also need SPxRIB definitions. */
#include "SPxLibData/SPxRib.h"

/* We need the definitions of the packets in the files. */
#include "SPxLibData/SPxPackets.h"

/* We need the file decoder class. */
#include "SPxLibData/SPxPacketDecoderFile.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/


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
class SPxDopplerStore;
class SPxNavData;

typedef int (*SPxRadarReplayStepCheckFn_t)(void *userArg, int *stepArg);

/*
 * Define the main replay class.
 */
class SPxRadarReplay : public SPxRadarReplayBase
{
public:
    /*
     * Public functions.
     */
    /* Constructor/destructor */
    explicit SPxRadarReplay(SPxRIB *buffer);
    virtual ~SPxRadarReplay();

    /* Add/remove channel. */
    SPxErrorCode AddChannel(unsigned int chanID,
			    SPxRIB *buffer,
			    SPxNavData *navData = NULL);
    SPxErrorCode RemoveChannel(unsigned int chanID,
			       SPxRIB *buffer = NULL,
			       SPxNavData *navData = NULL);

    /* Get channel objects. */
    SPxRIB *GetChannelRIB(unsigned int chanID);
    SPxNavData *GetChannelNavData(unsigned int chanID,
				  unsigned int navDataID=0) const;

    int IsPaused(void) const		{ return m_pktDecoder->IsPaused(); }

    /* Define the basic controls. */
    int SetFileName(const char *fileName);
    const char *GetFileName(void) const	{ return m_pktDecoder->GetFileName(); }
    const char *GetFullPath(void) const	{ return m_pktDecoder->GetFullPath(); }
    virtual void Pause(void)		{ m_pktDecoder->Pause(); }
    virtual void Play(void)		{ m_pktDecoder->Play(); }
    virtual void Rewind(void);
    void Step(UINT16 azi16Bit=0);

    /* Flag to say replay should automatically loop again at end of file. */
    void SetAutoLoop(int enable)	{ m_pktDecoder->SetAutoLoop(enable); }
    int GetAutoLoop(void) const		{ return m_pktDecoder->GetAutoLoop(); }

    /* Flag to say replay should automatically rewind at end of file. */
    void SetAutoRewind(int enable)	{ m_pktDecoder->SetAutoRewind(enable); }
    int GetAutoRewind(void) const	{ return m_pktDecoder->GetAutoRewind(); }
    int GetAutoWind(void) const		{ return GetAutoRewind(); }

    /* Flag to say replay should auto link to next file in sequence. */
    void SetAutoLink(int enable)	{ m_pktDecoder->SetAutoLink(enable); }
    int GetAutoLink(void) const		{ return m_pktDecoder->GetAutoLink(); }

    /* Speed of replay */
    void SetSpeedupFactor(double factor){ m_pktDecoder->SetSpeedupFactor(factor); }
    double GetSpeedupFactor(void) const	{ return m_pktDecoder->GetSpeedupFactor(); }

    /* Table of contents and searching functions. */
    int IsTOCAvailable(void) const	{ return m_pktDecoder->IsTOCAvailable(); }
    SPxErrorCode GotoFileTime(const SPxTime_t *timestamp) { return m_pktDecoder->GotoFileTime(timestamp); }
    SPxErrorCode GotoFileTime(UINT32 secs) { return m_pktDecoder->GotoFileTime(secs); }
    SPxErrorCode GotoFileTimePercent(double percent) { return m_pktDecoder->GotoFileTimePercent(percent); }

    /* File time retrieval. */
    void GetFileTimeStart(SPxTime_t *ptr, int inHandler=FALSE) const
    {
	m_pktDecoder->GetFileTimeStart(ptr, inHandler);
    }
    void GetFileTimeCur(SPxTime_t *ptr, int inHandler=FALSE) const
    {
	m_pktDecoder->GetFileTimeCur(ptr, inHandler);
    }
    void GetFileTimeEnd(SPxTime_t *ptr, int inHandler=FALSE) const
    {
	m_pktDecoder->GetFileTimeEnd(ptr, inHandler);
    }
    SPxErrorCode GetFileTimePercent(double *percentPtr) const { return m_pktDecoder->GetFileTimePercent(percentPtr); }

    /* Available files. */
    SPxErrorCode GetFileList(char *buffer, int bufLen) const;
    
    /* Base class status function. */
    virtual SPxErrorCode GetInitialised(void) { return m_pktDecoder->GetInitialised(); }

    /* Per-channel information retrieval. */
    SPxRIB *GetBuffer(unsigned int chanID=0) const;
    int GetNumSamples(unsigned int chanID=0) const;
    REAL32 GetStartRange(unsigned int chanID=0) const;
    REAL32 GetEndRange(unsigned int chanID=0) const;
    SPxErrorCode SetNavData(SPxNavData *navData, unsigned int chanID=0,
			    unsigned int navDataID=0);
    SPxNavData *GetNavData(unsigned int chanID=0,
			   unsigned int navDataID=0) const;
    unsigned int GetNumNavData(unsigned int chanID=0) const;

    /* Enable/disable nav data. */
    void SetReplayNavData(int replayNavData) { m_replayNavData = replayNavData; }
    int GetReplayNavData(void) const { return m_replayNavData; }


    /* Override PIM flags. */
    void SetPimFlags(UINT8 pimFlags) { m_pimFlags = pimFlags; }
    UINT8 GetPimFlags(void) const { return m_pimFlags; }

    /* Enable/disable replay of error packets from recordings.  */
    void SetSuppressReplayedErrors(int suppress);
    int GetSuppressReplayedErrors(void) const;

    /* Doppler support. */
    SPxErrorCode SetDopplerStore(SPxDopplerStore *ds, unsigned int chanID=0);
    SPxDopplerStore *GetDopplerStore(unsigned int chanID=0) const;

    /* Install callback for direct access to data before RIB (may not be
     * supported by all derived classes).
     */
    SPxErrorCode InstallDataFn(void (*fn)(SPxRadarReplay *, void *,
					SPxReturnHeader *,
					unsigned char *),
				void *arg)
    {
	m_fnDataArg = arg;
	m_fnData = fn;
	return(SPX_NO_ERROR);
    }

    /* Install a handler for packets not handled by this class. */
    SPxErrorCode InstallDefaultHandler(SPxPacketDecoderFn_t fn,
				       void *userArg);
    SPxErrorCode RemoveDefaultHandler(SPxPacketDecoderFn_t fn,
				      void *userArg);

    /* Install a step check handler. */
    SPxErrorCode InstallStepCheckHandler(SPxRadarReplayStepCheckFn_t fn,
					 void *userArg);
    SPxErrorCode RemoveStepCheckHandler(SPxRadarReplayStepCheckFn_t fn,
					void *userArg);

    /* For use by other SPx objects to get direct access to packets. */
    virtual SPxPacketDecoderFile *GetPacketDecoder(void) {return(m_pktDecoder); }

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /*
     * Protected variables.
     */

    /* Callback functions. */
    void (*m_fnData)(SPxRadarReplay *, void *,
		     SPxReturnHeader *, unsigned char *);
    void *m_fnDataArg;

    /* Access control. */
    mutable SPxCriticalSection m_mutex;

    /* Nav data. */
    int m_replayNavData;		/* Replay nav data? */

    /* PIM flags. */
    UINT8 m_pimFlags;

    /*
     * Protected functions.
     */
    /* Derived classes should call shutdown in their destructor. */
    // cppcheck-suppress virtualCallInConstructor
    virtual void Shutdown(void);

    /* Thread function (needs to be protected for classes to override). */
    static void *ThreadFuncWrapper(SPxThread *thread);
    virtual void *ThreadFunc(SPxThread *thread);

    /* Callbacks called by packet decoder. */
    static void processRadarConfigWrapper(SPxPacketDecoder *pktDecoder,
				 void *userArg,
				 UINT32 packetType,
				 struct sockaddr_in *from,
				 SPxTime_t *time,
				 const unsigned char *payload,
				 unsigned int payloadSize);
    void processRadarConfig(SPxPacketDecoder *pktDecoder,
				 void *userArg,
				 UINT32 packetType,
				 struct sockaddr_in *from,
				 SPxTime_t *time,
				 const unsigned char *payload,
				 unsigned int payloadSize);
    static void processRadarReturnWrapper(SPxPacketDecoder *pktDecoder,
				 void *userArg,
				 UINT32 packetType,
				 struct sockaddr_in *from,
				 SPxTime_t *time,
				 const unsigned char *payload,
				 unsigned int payloadSize);
    void processRadarReturn(SPxPacketDecoder *pktDecoder,
				 void *userArg,
				 UINT32 packetType,
				 struct sockaddr_in *from,
				 SPxTime_t *time,
				 const unsigned char *payload,
				 unsigned int payloadSize);
    static void processNmeaWrapper(SPxPacketDecoder *pktDecoder,
				 void *userArg,
				 UINT32 packetType,
				 struct sockaddr_in *from,
				 SPxTime_t *time,
				 const unsigned char *payload,
				 unsigned int payloadSize);
    void processNmea(SPxPacketDecoder *pktDecoder,
				 void *userArg,
				 UINT32 packetType,
				 struct sockaddr_in *from,
				 SPxTime_t *time,
				 const unsigned char *payload,
				 unsigned int payloadSize);
    static void processDopplerWrapper(SPxPacketDecoder *pktDecoder,
				 void *userArg,
				 UINT32 packetType,
				 struct sockaddr_in *from,
				 SPxTime_t *time,
				 const unsigned char *payload,
				 unsigned int payloadSize);
    void processDoppler(SPxPacketDecoder *pktDecoder,
				 void *userArg,
				 UINT32 packetType,
				 struct sockaddr_in *from,
				 SPxTime_t *time,
				 const unsigned char *payload,
				 unsigned int payloadSize);
    SPxErrorCode decodeReturn(const unsigned char *encodedData,
				unsigned int numEncodedBytes,
				unsigned char *buffer,
				unsigned int bufferSizeBytes,
				UINT8 compFlags);

private:
    /*
     * Private variables.
     */

    /* Used to store standard template variables, etc. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Worker thread to do the reading etc. */
    SPxThread *m_thread;

    /* ZLIB decompression. */
    SPxCompress *m_compress;

    /* Decode buffer etc. */
    unsigned char *m_decodeBuffer;	/* Buffer */
    unsigned int m_decodeBufferSize;	/* Size of decode buffer */

    SPxPacketDecoderFile *m_pktDecoder; /* File decoder. */

    /* Other options. */
    int m_suppressReplayedErrors;	/* Flag */

    /*
     * Private functions.
     */

    int isStandaloneMode(void) const;
    void resetChannels(const SPxTime_t *lastReturnTime);

}; /* SPxRadarReplay */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_RADARREPLAY_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
