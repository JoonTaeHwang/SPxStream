/*********************************************************************
 *
 * (c) Copyright 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxRFJammer class which supports the control
 *   of an RF jammer used to disable control drones.
 *
 * Revision Control:
 *   14/11/18 v1.7    AGC	Suppress spurious cppcheck warning.
 *
 * Previous Changes:
 *   13/11/18 1.6   SP 	Add Shutdown().
 *   31/10/18 1.5   AGC	Suppress cppcheck warnings.
 *   26/10/18 1.4   SP 	Support RF detection.
 *   04/06/18 1.3   AGC	Support any UINT32 for mode.
 *   17/01/18 1.2   SP 	Rename State_t to Mode_t.
 *                     	Use handler for received messages.
 *   10/01/18 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_RF_JAMMER_H
#define _SPX_RF_JAMMER_H

/*
 * Other headers required.
 */

/* Other classes that we need. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibNet/SPxNetTCP.h"
#include "SPxLibNet/SPxRFTargetDetector.h"

/* Forward declare any classes required. */
class SPxThread;
class SPxCamera;

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

/* Parameters. */

/* Elbit commands. */
#define SPX_JAMMER_ELBIT_MODE_OFF           0
#define SPX_JAMMER_ELBIT_MODE_RF_AND_GPS    1
#define SPX_JAMMER_ELBIT_MODE_RF            2
#define SPX_JAMMER_ELBIT_MODE_GPS           9

/* SteelRock commands. */
#define SPX_JAMMER_SR_MODE_OFF              1
#define SPX_JAMMER_SR_MODE_ARM              2
#define SPX_JAMMER_SR_MODE_RF               4
#define SPX_JAMMER_SR_MODE_GPS              8

/*********************************************************************
 *
 *   Macros
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/*
 * Define our class.
 */
class SPxRFJammer : public SPxRFTargetDetector
{
public:

    /*
     * Public types.
     */

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxRFJammer(SPxRFTargetDatabase *database=NULL);
    virtual ~SPxRFJammer(void);
    virtual SPxErrorCode Create(void);
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode Shutdown(void);
    virtual void SetHeartbeatsEnabled(int isEnabled) { m_heartbeatsEnabled = isEnabled; }
    virtual int AreHeartbeatsEnabled(void) { return m_heartbeatsEnabled; }
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetCamera(SPxCamera *camera);

    /* Control. */
    virtual SPxErrorCode SetMode(UINT32 mode);
    virtual UINT32 GetMode(void) { return m_mode; }

    /* Status. */
    virtual int IsConnected(void);
    virtual int IsStatusReceived(void);

    /* Overrides. */
    virtual SPxErrorCode IterateSources(SPxCallbackListFn_t fn, void *userArg);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    /* Private member variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    char m_addrStr[16];
    int m_port;
    UINT32 m_mode;
    int m_heartbeatsEnabled;
    UINT32 m_lastModeChangeTime;
    UINT32 m_lastStatusRecvTime;
    UINT32 m_lastHeartbeatTime;
    UINT32 m_lastTestMesgTime;
    SPxThread *m_thread;
    int m_isDisconnectPending;
    SPxNetTCP *m_socket;
    SPxCamera *m_camera;


    /*
     * Private functions.
     */

    /* Message handling. */
    unsigned int checkConnection(void);
    SPxErrorCode sendMesg(UINT32 value);
    void checkSendHeartbeat(void);
    void decodePacket(const unsigned char *payload, unsigned int numBytes);
    void processStatusPacket(UINT32 mode);
    void processDetectionPacket(const unsigned char *payload,
                                unsigned int numBytes);
    int isTestMode(void);
    void checkInputTestDetection(void);

    /*
     * Private static functions.
     */

    static void *threadHandler(SPxThread *thread);

    static void recvHandler(SPxNetTCP *tcp,
                            SPxErrorCode errorCode,
                            SPxNetTCPAction action,
                            void* userArg,
                            const unsigned char* buffer,
                            unsigned int bufLen,
                            UINT32 fromAddr,
                            SOCKET_TYPE sock);

    static int cameraDeletedHandler(void *invokingObject, 
                                    void *userObject,
                                    void *arg);

}; /* SPxRFJammer */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_RF_JAMMER_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
