/*********************************************************************
 *
 * (c) Copyright 2017 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx camera manager class header.
 *
 * Revision Control:
*   13/09/22 v1.20   AGC	Support increased pan/tilt speed resolution.
 *
 * Previous Changes:
*   08/08/22 1.19   AGC	Support string based presets.
*   29/03/22 1.18   AGC	Correctly handle socket re-use.
*   17/03/22 1.17   AGC	Fix single connection controlling multiple cameras.
 *  16/03/22 1.16   AGC	Support control of multipe cameras through single connection.
 *  28/07/21 1.15   AGC	Do not take control for specific standard commands.
 *  28/06/21 1.14   AGC	Pass on from address for direct control.
 *  21/06/21 1.13   AGC	Support releasing camera control.
 *  22/03/21 1.12   AGC	Avoid const_cast in SPxCamCtrlCamMngr.
 *  16/03/21 1.11   AGC	Support direct Pelco message input.
 *  06/08/20 1.10   AGC	Improve Onvif support.
 *  31/07/18 1.9    AGC	Support checking for secondary sensor support.
 *  07/06/18 1.8    AGC	Support auto-focus one-shot.
 *  10/05/18 1.7    AGC	Do not call Create() on user provided controllers.
 *  17/04/18 1.6    AGC	Add WaitForEmptyQueue().
 *  12/04/18 1.5    AGC	Support use with Nexus cameras.
 *  21/02/17 1.4    AGC	Support camera information retrieval.
 *  24/01/17 1.3    AGC	Improvements to maintain command interval.
 *  23/01/17 1.2    AGC	Modifications for testing.
 *  18/01/17 1.1    AGC	Initial version.
 *
 *********************************************************************/

#ifndef SPX_CAMERA_MANAGER_H_
#define SPX_CAMERA_MANAGER_H_

/* Other headers required. */
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibData/SPxCamCtrlBase.h"
#include "SPxLibNet/SPxNetTCP.h"

/* Forward declare any classes required. */
class SPxNetTCP;
class SPxSerialPort;
class SPxThread;
class SPxCamCtrlBase;
class SPxCamCtrlPelco;

/* Current camera manager communication version. */
#define SPX_CAMERA_MANAGER_VERSION (8)

class SPxCameraManager : public SPxObj
{
public:
    struct ConnInfo;
    struct ConnCamInfo;
    typedef void(*CameraFn)(UINT8 camIndex, void *userArg);
    typedef unsigned int(*ResponseFn)(const unsigned char *payload,
				      unsigned int payloadBytes,
				      void *userArg);

    /* Constructor and destructor. */
    SPxCameraManager(void);
    virtual ~SPxCameraManager(void);

    /* Set/get network port for receiving messages. */
    SPxErrorCode SetPortNumber(int port);
    UINT16 GetPortNumber(void) const;

    /* Set timeout for releasing control of camera. */
    SPxErrorCode SetControlTimeoutMS(unsigned int timeoutMS);
    unsigned int GetControlTimeoutMS(void) const;

    /* Set interval for summary debug messages. */
    SPxErrorCode SetSummaryIntervalMS(unsigned int intervalMS);
    unsigned int GetSummaryIntervalMS(void) const;

    /* Add/remove cameras. */
    SPxErrorCode AddCamera(UINT8 camIndex, SPxCameraType camType);
    SPxErrorCode RemoveCamera(UINT8 camIndex);

    /* Get camera information. */
    unsigned int GetNumCameras(void) const;
    int IsCameraAvailable(UINT8 camIndex) const;
    SPxErrorCode IterateCameras(CameraFn fn, void *userArg) const;
    SPxCamCtrlBase *GetCameraControl(UINT8 camIndex);

    /* Status messages and updates. */
    SPxErrorCode CheckStatus(void);
    unsigned int GetNumTotalClients(void) const;
    unsigned int GetNumClients(UINT8 camIndex) const;

    /* Direct camera control. */
    void SetResponseCallback(SOCKET_TYPE sock,
			     UINT32 id,
			     UINT8 cameraAddress,
			     ResponseFn responseFn,
			     void *responseUserArg);
    void HandlePelco(SOCKET_TYPE sock, UINT32 fromAddr,
		     UINT32 id,
		     const unsigned char *data,
		     unsigned int numBytes) const;

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

    void SetVerbosity(unsigned int verbosity);
    void SetLogFile(std::FILE *logFile);
    void SetLogFile(const char *logFileName);

protected:
    /* Protected functions that may be called by derived testing classes. */
    explicit SPxCameraManager(SPxNetTCP *netTCP, UINT8 version);
    SPxErrorCode AddCamera(UINT8 camIndex, SPxCamCtrlBase *camCtrl);

    SPxErrorCode WaitForEmptyQueue(UINT32 timeoutMS) const;

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    void initialise(void);

    SPxErrorCode addCamera(UINT8 camIndex,
			   SPxCameraType camType,
			   SPxCamCtrlBase *camCtrl,
			   int createCamCtrl);

    void handleData(const unsigned char* buffer,
		    unsigned int bufLen,
		    UINT32 fromAddr,
		    SOCKET_TYPE sock,
		    UINT32 id);

    unsigned int handleMesg(const unsigned char *data,
			    unsigned int numBytes,
			    UINT32 fromAddr,
			    SOCKET_TYPE sock,
			    UINT32 id) const;

    void handlePelcoDMesg(const unsigned char *data,
			  unsigned int numBytes,
			  UINT32 fromAddr,
			  SOCKET_TYPE sock,
			  UINT32 id) const;

    unsigned int handlePelcoDExtMesg(const unsigned char *data,
				     unsigned int numBytes,
				     UINT32 fromAddr,
				     SOCKET_TYPE sock,
				     UINT32 id) const;

    void handlePelcoDQueryMesg(const unsigned char *data,
			       unsigned int numBytes,
			       UINT32 fromAddr,
			       SOCKET_TYPE sock,
			       UINT32 id) const;

    void forwardMesg(SPxCamCtrlBase *camCtrl,
		     UINT16 command,
		     UINT8 panSpeed, UINT8 tiltSpeed,
		     int isSecondary,
		     float *zoomSpeed,
		     float *focusSpeed,
		     UINT8 camMngrVersion,
		     const char *str);

    void sendPelcoDGenResponse(UINT8 addr,
			       SOCKET_TYPE sock,
			       ResponseFn responseFn,
			       void *responseUserArg);

    void sendPelcoDExtResponse(UINT8 addr,
			       UINT16 opcode,
			       UINT16 value,
			       SOCKET_TYPE sock,
			       ResponseFn responseFn,
			       void *responseUserArg);

    void sendResponse(const unsigned char *data,
		      unsigned int numBytes,
		      SOCKET_TYPE sock,
		      ResponseFn responseFn,
		      void *responseUserArg);
    
    int canConnectionHaveControl(UINT8 camAddress,
				 UINT32 fromAddr,
				 SOCKET_TYPE sock,
				 UINT32 id) const;
    void connectionTakeControl(UINT8 camAddress,
			       UINT32 fromAddr,
			       SOCKET_TYPE sock,
			       UINT32 id);

    void netDisconnect(SPxNetTCP *tcp, SOCKET_TYPE sock, UINT32 id);

    void outputSummary(void) const;

    void threadFn(SPxThread *thread, UINT8 camIndex);

    void purgeRemoteConnections(void);

    /*
     * Private static functions.
     */
    static void netMesgHandler(SPxNetTCP *tcp,
			       SPxErrorCode errorCode,
			       SPxNetTCPAction action,
			       void* userArg,
			       const unsigned char* buffer,
			       unsigned int bufLen,
			       UINT32 fromAddr,
			       SOCKET_TYPE sock,
			       UINT32 id);

    static void netDisconnectHandler(SPxNetTCP *tcp,
				     void *userArg,
				     SOCKET_TYPE sock,
				     UINT32 id);

    static int isPelcoDExtGetCommand(UINT16 command);
    static int isPelcoDExtStrCommand(UINT16 command);
    static int isPelcoDSSGetCommand(UINT16 command);

    static void *threadFnWrapper(SPxThread *thread);

}; /* SPxCameraManager */

#endif /* SPX_CAMERA_MANAGER_H_ */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
