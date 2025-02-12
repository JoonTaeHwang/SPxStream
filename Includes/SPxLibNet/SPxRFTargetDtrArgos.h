/*********************************************************************
 *
 * (c) Copyright 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxRFTargetDtrArgos class which supports the decoding
 *   of RF target network messages from a MyDefence detector via the
 *   Argos interface software.
 *
 * Revision Control:
 *   31/10/18 v1.7    AGC	Suppress cppcheck warnings.
 *
 * Previous Changes:
 *   25/10/18 1.6   SP 	Support changes to SPxRFTargetDetector.
 *   22/10/18 1.5   SP 	Add support for source name.
 *   22/10/18 1.4   AGC	Suppress erroneous cppcheck warning.
 *   18/10/18 1.3   SP 	Add test code and more debug messages.
 *   17/10/18 1.2   SP 	Further development.
 *   15/10/18 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_RF_TARGET_DTR_ARGOS_H
#define _SPX_RF_TARGET_DTR_ARGOS_H

/*
 * Other headers required.
 */

/* Other classes that we need. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibNet/SPxRFTargetDetector.h"
#include "SPxLibNet/SPxWebsocket.h"

/* Forward declare any classes required. */

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

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
class SPxRFTargetDtrArgos : public SPxRFTargetDetector
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
    SPxRFTargetDtrArgos(SPxRFTargetDatabase *database);
    virtual ~SPxRFTargetDtrArgos(void);
    virtual SPxErrorCode Create(void);
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode Shutdown(void);

    /* Overrides. */
    // cpppcheck-suppress virtualCallInConstructor
    virtual double GetTargetWidthDegs(void);
    virtual SPxErrorCode IterateSources(SPxCallbackListFn_t fn, 
                                        void *userArg);

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

    /* General. */
    struct impl;
    struct Target_t;
    SPxAutoPtr<impl> m_p;
    SPxWebsocket *m_websocket;
    SPxThread *m_thread;
    SPxCriticalSection m_mutex;
    UINT32 m_lastdeviceListReqTime;
    UINT32 m_lastDeviceCheckAvailableTime;
    UINT32 m_lastDeviceCheckMountTime;
    UINT32 m_lastDeviceCheckLocationTime;
    UINT32 m_nextResponseID;
    unsigned int m_expectedNumSources;
    UINT32 m_nextSourceID; /* Bottom 16-bits only. */
    unsigned int m_nextTestMesgID;
    UINT32 m_lastTestMesgTime;

    /*
     * Private functions.
     */

    /* Message sending. */
    void sendMesg(const char *evt, const char *body);
    void sendRequestMesgs(void);

    /* Message receiving. */
    void handleMesg(const char *mesg);
    void handleDeviceListMesg(void);
    void handleDeviceAddedMesg(void);
    void handleDeviceRemovedMesg(void);
    void handleDeviceUpdatedMesg(void);
    void handleDeviceLocationChangedMesg(void);
    void handleThreatStartedMesg(Target_t *target);
    void handleThreatStoppedMesg(Target_t *target);
    void handleThreatUpdatedMesg(Target_t *target);

    /* Devices and detectors. */
    SPxErrorCode addOrUpdateDevice(void *jsonDevVoid);
    SPxErrorCode addOrUpdateDetector(void *deviceVoid, void *jsonDetVoid);
    void checkDevicesAvailable(void);
    void checkDevicesMounted(void);
    void checkDevicesLocation(void);

    /* General. */
    SPxErrorCode readDeviceConfigFile(const char *filename);
    int isTestMode(void);
    const char *getNextTestMesg(void);

    /*
     * Private static functions.
     */

    static void *threadHandler(SPxThread *thread);

    static void recvHandler(SPxWebsocket *ws,
                            void *userArg,
                            const SPxTime_t *timestamp,
                            const unsigned char *payload,
                            unsigned int numBytes);

}; /* SPxRFTargetDtrArgos */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_RF_TARGET_DTR_ARGOS_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
