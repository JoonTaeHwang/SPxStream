/*********************************************************************
*
* (c) Copyright 2018 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxRemoteRDR class, which allows client
*	applications to remotely control instances of SPx RDR.
*
*
* Revision Control:
*   23/02/21 v1.17   BTB	Support record-only and replay-only control.
*
* Previous Changes:
*   21/07/20 1.16   AGC	Detect replay loop.
*   17/07/20 1.15   AGC	Support control of playback speed.
*   09/07/20 1.14   AGC	Improve remote server initialisation.
*   07/07/20 1.13   AGC	Add Record_StartTimed() function.
*   06/07/20 1.12   AGC	Support control of client live/replay.
*			Support reading control port from heartbeat.
*			Support lat/long notes.
*			Strip quotes from marker text.
*   23/06/20 1.11  BTB	Revert change of variable type for setting/getting replay session index.
*   22/06/20 1.10  BTB	Use correct variable type for setting/getting replay session index.
*   19/06/20 1.9   BTB	Support storing and fetching replay marker information.
*                     	Fix functions for fetching record/replay timestamps.
*   16/06/20 1.8   BTB	Add goto and loop replay functions.
*   29/04/19 1.7   SP 	Add channel input enable functions.
*   13/03/19 1.6   SP  	Support new RDR channel parameters.
*   30/11/18 1.5   BTB 	Store address in own memory, don't rely on parameter related memory.
*   11/06/18 1.4   BTB 	Use auto-reconnect functionality in SPxRemoteServer.
*   26/04/18 1.3   BTB 	Class should derive from SPxObj.
*   25/04/18 1.2   BTB 	Add bounds checking to functions.
*   25/04/18 1.1   BTB 	Initial Version.
*
**********************************************************************/

#ifndef _SPX_REMOTE_RDR_H
#define _SPX_REMOTE_RDR_H

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxCriticalSection. */
#include "SPxLibUtils/SPxCriticalSection.h"

/* For SPxTimer. */
#include "SPxLibUtils/SPxTimer.h"

/* For SPxObj. */
#include "SPxLibUtils/SPxObj.h"

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

/* Forward declare classes we need in case of wrong order headers. */
class SPxRemoteServer;
class SPxExtHeartbeatDecoder;
class SPxNetAddr;
struct SPxScNetHeartbeatStruct_tag;
struct SPxLatLong_tag;

/*
 * Define our class.
 */
class SPxRemoteRDR : public SPxObj
{
public:

    /*
     * Public functions.
     */

    /* Constructors, destructor etc. */
    explicit SPxRemoteRDR(const char *addr=NULL, unsigned int port=0);
    virtual ~SPxRemoteRDR(void);

    SPxErrorCode SetRDRAddress(const char *addr);
    const char *GetRDRAddress(void) const;
    SPxErrorCode SetRDRPort(unsigned int port);
    unsigned int GetRDRPort(void) const;
    SPxErrorCode SetUseRDRPortFromHeartbeat(int enable);
    int IsUsingRDRPortFromHeartbeat(void) const;
    SPxErrorCode SetRDRHeartbeatAddress(const char *hbAddress,
					unsigned int hbPort,
					const char *hbIfAddress = NULL);
    const char *GetRDRHeartbeatAddress(void) const;
    unsigned int GetRDRHeartbeatPort(void) const;
    const char *GetRDRHeartbeatIfAddress(void) const;
    SPxErrorCode SetCtrlFeatures(UINT32 crtlFeatures);
    UINT32 GetCtrlFeatures(void) const;

    /* Application live/replay input. */
    SPxErrorCode RegisterInput(const char *objName, SPxObj *obj);
    SPxErrorCode RegisterInputParam(const char *objName,
				    const char *objParam,
				    const char *rdrParam=NULL);
    SPxErrorCode RegisterLiveObject(SPxObj *obj, int isActiveWhenLive);
    SPxErrorCode SetInputChanID(const char *objName, UINT8 chanID);
    UINT8 GetInputChanID(const char *objName) const;
    SPxErrorCode SetLiveInput(int live, int forceUpdate=FALSE);
    int IsLiveInput(void) const;

    /* Connection related functions. */
    SPxErrorCode OpenConnection(void);
    SPxErrorCode CloseConnection(void);
    int IsConnected(void);

    SPxErrorCode GetProjectName(char *projectName, int bufSize);
    SPxErrorCode GetVersionNumber(char *versionString, int bufSize);

    /* Recording related functions. */

    SPxErrorCode Record_Start(void);
    SPxErrorCode Record_StartTimed(unsigned int seconds);
    SPxErrorCode Record_Stop(void);

    /* Recording or not. */
    SPxErrorCode Record_State(int *state);

    SPxErrorCode Record_GetStatus(SPxStatus_t *status);

    /* These functions are for use during recording. */
    SPxErrorCode Record_GetCurrentSessionName(char *sessionName, int bufSize);
    SPxErrorCode Record_GetSessionStartTime(unsigned int *time);
    SPxErrorCode Record_GetSessionStartTimeString(char *timeString, int bufSize);
    SPxErrorCode Record_GetSessionCurrentTime(unsigned int *time);
    SPxErrorCode Record_GetSessionCurrentTimeString(char *timeString, int bufSize);
    SPxErrorCode Record_GetSessionDurationSecs(unsigned int *duration);
    SPxErrorCode Record_GetSessionDurationString(char *durationString, int bufSize);

    /* Logging related functions. */
    SPxErrorCode Record_LogNote(const char *note, const SPxLatLong_tag *ll=NULL);
    SPxErrorCode Record_LogEvent(int eventID);

    /* Functions used to set/get events. */
    SPxErrorCode Record_GetNumEvents(unsigned int *numEvents);
    SPxErrorCode Record_GetEventIDList(int *eventIDbuf, int bufSize);
    SPxErrorCode Record_GetEventName(int eventID, char *eventName, int bufSize);

    /* Functions used to set/get channels. */
    SPxErrorCode Record_GetNumChannels(unsigned int *numChannels);
    SPxErrorCode Record_GetChannelsStatus(SPxStatus_t *status);
    SPxErrorCode Record_GetChannelsDataRate(unsigned int *bytesPerSec);
    SPxErrorCode Record_GetChannelIDList(int *channelIDbuf, int bufSize);
    SPxErrorCode Record_GetChannelID(unsigned int index, int *channelID);
    SPxErrorCode Record_GetChannelName(int channelID, char *channelName, int bufSize);
    SPxErrorCode Record_GetChannelStatus(int channelID, SPxStatus_t *status);
    SPxErrorCode Record_GetChannelDataRate(int channelID, unsigned int *bytesPerSec);
    SPxErrorCode Record_SetChannelInputEnabled(int channelID, int isEnabled);
    SPxErrorCode Record_GetChannelInputEnabled(int channelID, int *isEnabled);

    /* Replay related functions. */

    SPxErrorCode Replay_Start(void);
    SPxErrorCode Replay_Pause(void);
    SPxErrorCode Replay_Stop(void);
    SPxErrorCode Replay_GotoTime(unsigned int time);

    /* Replay started, paused or stopped. */
    SPxErrorCode Replay_State(int *state);
    
    SPxErrorCode Replay_GetStatus(SPxStatus_t *pStatus);

    /* Speedup factor. */
    SPxErrorCode Replay_SetSpeedupFactor(float factor);
    SPxErrorCode Replay_GetSpeedupFactor(float *factor);

    /* Autoloop enabled/disabled. */
    SPxErrorCode Replay_SetAutoloop(int enable);
    SPxErrorCode Replay_GetAutoloop(int *enable);

    /* Functions related to session load state. */
    SPxErrorCode Replay_SessionLoading(int *state);
    SPxErrorCode Replay_SessionLoaded(int *state);

    /* Functions used to get session list. */
    SPxErrorCode Replay_GetNumSessions(unsigned int *numSessions);
    SPxErrorCode Replay_GetSessionName(int sessionID, char *sessionName, int bufSize);
    SPxErrorCode Replay_SetCurrentSession(int sessionID);
    SPxErrorCode Replay_GetCurrentSession(int *sessionID);
    SPxErrorCode Replay_GetSessionStartTime(unsigned int *time);
    SPxErrorCode Replay_GetSessionStartTimeString(char *timeString, int bufSize);
    SPxErrorCode Replay_GetSessionCurrentTime(unsigned int *time);
    SPxErrorCode Replay_GetSessionCurrentTimeString(char *timeString, int bufSize);
    SPxErrorCode Replay_GetSessionEndTime(unsigned int *time);
    SPxErrorCode Replay_GetSessionEndTimeString(char *timeString, int bufSize);
    SPxErrorCode Replay_GetSessionDurationSecs(unsigned int *duration);
    SPxErrorCode Replay_GetSessionDurationString(char *durationString, int bufSize);

    /* Functions used to set/get channels. */
    SPxErrorCode Replay_GetNumChannels(unsigned int *numChannels);
    SPxErrorCode Replay_GetChannelsStatus(SPxStatus_t *status);
    SPxErrorCode Replay_GetChannelsDataRate(unsigned int *bytesPerSec);
    SPxErrorCode Replay_GetChannelIDList(int *channelIDbuf, int bufSize);
    SPxErrorCode Replay_GetChannelID(unsigned int index, int *channelID);
    SPxErrorCode Replay_GetChannelName(int channelID, char *channelName, int bufSize);
    SPxErrorCode Replay_GetChannelStatus(int channelID, SPxStatus_t *status);
    SPxErrorCode Replay_GetChannelDataRate(int channelID, unsigned int *bytesPerSec);
    SPxErrorCode Replay_SetChannelInputEnabled(int channelID, int isEnabled);
    SPxErrorCode Replay_GetChannelInputEnabled(int channelID, int *isEnabled);

    /* Functions used to get marker info. */
    SPxErrorCode Replay_GetNumMarkers(unsigned int *numMarkers);
    SPxErrorCode Replay_GetMarkerIDList(int *markerIDbuf, int bufSize);
    SPxErrorCode Replay_GetMarkerType(int markerID, int *markerType);
    SPxErrorCode Replay_GetMarkerTime(int markerID, double *markerTime);
    SPxErrorCode Replay_GetMarkerText(int markerID, char *markerText, int bufSize);

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:
    /*
     * Private fields.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Mutex */
    SPxCriticalSection m_mutex;

    SPxAutoPtr<SPxTimer> m_timer;

    /* Object used to connect to RDR. */
    SPxAutoPtr<SPxRemoteServer> m_rdrConnection;
    unsigned int m_port;
    int m_usePortFromHeartbeat;
    UINT32 m_rdrVersion;
    UINT32 m_ctrlFeatures;

    /*
     * Private functions.
     */

    void InitConnection(void);
    SPxErrorCode CheckSupported(UINT32 minVersion);
    SPxErrorCode CheckCtrlFeatures(UINT32 crtlFeatures);

    /* Functions to update internal lists. */
    void PopulateRecordEventList(void);
    void PopulateRecordChannelList(void);
    void PopulateReplaySessionList(void);
    void PopulateReplayChannelList(void);
    void PopulateReplayMarkerList(void);
    void CheckForAutoLoop(void);

    /* Schedulable function to call the above functions. */
    void UpdateLists(void);

    /* Callbacks for receiving heartbeats. */
    static void staticHbFn(SPxRemoteServer *remoteServer,
			   void *userArg,
			   struct sockaddr_in *from,
			   SPxScNetHeartbeatStruct_tag *hb);
    void hbFn(SPxRemoteServer *remoteServer,
	      struct sockaddr_in *from,
	      SPxScNetHeartbeatStruct_tag *hb);

}; /* SPxRemoteRDR class */

#endif /* _SPX_REMOTE_RDR_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
