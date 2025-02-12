/**
 * \copyright	(c) Copyright 2010 - 2012, 2017, Cambridge Pixel Ltd.
 *
 * \file	$RCSfile: SPxImgSendManager.h,v $
 * \ID		$Id: SPxImgSendManager.h,v 1.5 2017/11/13 16:01:59 rew Exp $
 *
 * \brief	Declares the SPxImgSendManager class, which allows
 *		an application to scan-convert radar video and
 *		distribute or save the scan-converted data as
 *		images.
 *
 * \revision	
 *  13/11/17 V1.5    AGC	Rename private class.
 *
 * \changes
 *  31/08/12 1.4    AGC	Reduce header dependencies.
 *  17/11/11 1.3    AGC	Use const char * for initCheck().
 *  11/10/10 1.2    AGC Rename HwAvailable to HwIdx.
 *  04/10/10 1.1    AGC Initial version. 
 */
#ifndef _SPX_IMG_SEND_MANAGER_H
#define _SPX_IMG_SEND_MANAGER_H

/*
 * Other headers required.
 */

/* We need SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* We need SPxCommon for common types. */
#include "SPxLibUtils/SPxCommon.h"

/* We need SPxErrorCode enumeration. */
#include "SPxLibUtils/SPxError.h"

/* We need SPxTime_t structure. */
#include "SPxLibUtils/SPxTime.h"

/* We need SPX_BITMAP_32BITS. */
#include "SPxLibSc/SPxScDestBitmap.h"

/* For SPxObj base class. */
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Maximum number of image streams per manager. */
#define SPX_IMG_SEND_MAX_STREAMS (4)

/*
 * Debug flags.
 */
#define	SPX_IMG_SEND_MNGR_DEBUG_STREAM_INFO	0x00000001	/* Stream information output */
#define	SPX_IMG_SEND_MNGR_DEBUG_STREAM_ADD	0x00000002	/* Stream addition */
#define SPX_IMG_SEND_MNGR_DEBUG_STREAM_REMOVE   0x00000004	/* Stream removal */

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
class SPxRunProcess;
class SPxPIM;
class SPxImgSendStream;
class SPxRadarStream;
class SPxRIB;
class SPxNetworkReceive;
class SPxTestGenerator;
class SPxRadarReplay;
class SPxHPx100Source;
class SPxPacketSender;
class SPxThread;

/**
 * Define our class, derived from the SPxObj class.
 */
class SPxImgSendManager :public SPxObj
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructors, destructor etc. */
    SPxImgSendManager(unsigned int id, 
		      unsigned int numSamples=2048,
		      unsigned int numAzis=2048);
    SPxImgSendManager(unsigned int id,
		      SPxRunProcess *process,
		      SPxPIM *pim=NULL);
    virtual ~SPxImgSendManager();
    SPxErrorCode Create();

    /* Set/Get Manager ID */
    void SetID(UINT16 id) { m_managerID = id; }
    UINT16 GetID(void) { return m_managerID; }

    /* Stream addition/removal */
    SPxImgSendStream *AddStream(int width=512, 
			        int height=512, 
			        int bitmapDepth=SPX_BITMAP_32BITS,
				int initiallyEnabled=TRUE);
    SPxErrorCode RemoveStream(SPxImgSendStream *stream);
    unsigned int GetNumStreams(void);
    SPxImgSendStream *GetNextStream(SPxImgSendStream *prevStream);

    /* Select the input source. */
    SPxErrorCode SetSource(UINT32 sourceID, UINT32 arg1=0, UINT32 arg2=0);
    SPxErrorCode GetSource(UINT32 *sourceID, UINT32 *arg1=NULL, UINT32 *arg2=NULL);

    /* Get input source objects. */
    SPxRadarStream *GetRadarStream(void)    { return m_radarStream; }
    SPxRIB *GetRIB(void);
    SPxPIM *GetPIM(void);
    SPxNetworkReceive *GetNetSource(void);
    SPxTestGenerator *GetTestSource(void);
    SPxRadarReplay *GetReplaySource(void);
    SPxHPx100Source *GetHwSource(void)	    { return m_useRadarStream ? m_hwSource : NULL; }
    SPxErrorCode SetHwIdx(int boardIdx);
    int GetHwIdx(void) { return m_hwBoardIdx; }
    SPxErrorCode SetProjectCode(UINT16 projectCode);
    UINT16 GetProjectCode(void) { return m_projectCode; }    

    /* Stream info publish control. */
    SPxErrorCode SetPublishInterval(unsigned int seconds);
    unsigned int GetPublishInterval(void) { return m_publishIntervalMSecs / 1000U; }
    SPxErrorCode SetPublishAddress(UINT32 address, int port=0, const char *ifAddr=NULL);
    SPxErrorCode SetPublishAddress(const char *address, int port=0, const char *ifAddr=NULL);
    UINT32 GetPublishAddress(void);
    SPxErrorCode SetPublishPort(int port);
    int GetPublishPort(void);
    UINT32 GetPublishIfAddress(void);

    /* Parameter assignment. */
    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Debugging */
    static void SetDebug(UINT32 debug);
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
    SPxAutoPtr<impl> m_p;	/* Private structure. */

    /* Scan conversion */
    int m_useRadarStream;
    UINT32 m_sourceID;
    UINT32 m_sourceArg1;
    UINT32 m_sourceArg2;
    SPxRunProcess *m_process;
    SPxPIM *m_pim;
    SPxRadarStream *m_radarStream;
    SPxHPx100Source *m_hwSource;
    int m_hwBoardIdx;

    /* Manager ID */
    UINT16 m_nextStreamID;
    UINT16 m_managerID;

    /* Stream information publishing. */
    SPxPacketSender *m_pktSender;
    UINT32 m_lastPublishMSecs;		/* Time of last stream info message */
    UINT32 m_publishIntervalMSecs;	/* Milliseconds between stream info publishes */

    /* Initialisation */
    int m_initialised;

    /* Worker thread. */
    SPxThread *m_thread;		/* The thread object */

    /* License */
    int m_licErrReported;
    SPxTime_t m_lastLicErrReport;

    /* Project code. */
    UINT16 m_projectCode;

    /*
     * Private functions.
     */
    void commonInit(unsigned int id);

    /* Thread functions. */
    static void *threadWrapper(SPxThread *thread);
    void *threadFn(SPxThread *thread);
    SPxErrorCode initCheck(const char *fnName);
    UINT32 publishSender();

}; /* SPxImgSendManager class */

#endif /* _SPX_IMG_SEND_MANAGER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
