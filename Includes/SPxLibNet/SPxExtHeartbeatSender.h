/*********************************************************************
 *
 * (c) Copyright 2019 - 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxExtHeartbeatSender class.
 *
 * Revision Control:
 *   17/06/22 v1.11  SP 	Add application uptime to heartbeat.
 *
 * Previous Changes:
 *   26/10/21 1.10  SP 	Add ModifyStream().
 *   21/12/20 1.9   SP 	Rework timers and report web interface port.
 *   04/03/20 1.8   SP 	Various changes to support SPxOutputManager.
 *   04/02/20 1.7   SP 	Add SetTitle().
 *   07/01/20 1.6   SP 	Compress packets that exceed MTU.
 *   06/12/19 1.5   SP 	Remove project code from AddStream().
 *   15/11/19 1.4   SP 	Don't assign default name to streams.
 *   13/11/19 1.3   SP 	Fix build warnings.
 *   12/11/19 1.2   SP 	Support access to fields.
 *   08/11/19 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_EXT_HEARTBEAT_SENDER_H
#define _SPX_EXT_HEARTBEAT_SENDER_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxTimer.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxSdbApplication.h"
#include "SPxLibUtils/SPxSdbStream.h"
#include "SPxLibNet/SPxPacketSend.h"

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

/* Forward declare classes we use. */

/*
 * SPxExtHeartbeatSender class.
 */
class SPxExtHeartbeatSender : public SPxPacketSender
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
    SPxExtHeartbeatSender(void);
    virtual ~SPxExtHeartbeatSender(void);
    SPxErrorCode Create(SPxSdbApplication::Type_t appType);

    /* General. */
    SPxSdbApplication::Type_t GetApplicationType(void);
    SPxErrorCode SetTitle(const char *title);
    SPxErrorCode StartSending(void);
    SPxErrorCode StopSending(void);
    int IsSending(void) { return m_isSending; }

    /* Application. */
    SPxSdbApplication *GetApplicationAndLock(void);
    SPxErrorCode UnlockApplication(SPxSdbApplication **app);
    SPxSdbField *GetAppFieldAndLock(const char *key);

    /* Streams. */
    SPxErrorCode AddStream(SPxObj *obj,
                           SPxNetAddr *netAddr,
                           SPxSdbStream::Type_t type,
                           SPxSdbStream::Subtype_t subtype);
    SPxErrorCode RemoveStream(SPxObj *obj);
    SPxErrorCode ModifyStream(SPxObj *obj, 
                              SPxSdbStream::Type_t type, 
                              SPxSdbStream::Subtype_t subtype);
    SPxObj *GetStreamObj(SPxNetAddr *netAddr);
    SPxErrorCode SetStreamName(SPxObj *obj, const char *name);
    SPxSdbField *GetStreamFieldAndLock(SPxObj *obj, const char *key);
    SPxErrorCode SetStreamPIM(SPxObj *obj, SPxPIM *pim);

    /* Misc. */
    SPxErrorCode UnlockField(SPxSdbField **field);

    /*
     * Public static functions.
     */

protected:

    /*
     * Protected types.
     */

    /* Stream context. */
    struct Stream_t
    {
        SPxSdbStream *sdbStream;    /* The application stream. */
        SPxObj *obj;                /* Key object. */
        SPxNetAddr *netAddr;        /* SPxNetAddr for obj. */
    };

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* General. */
    virtual Stream_t *findStream(SPxObj *obj, unsigned int *indexRtn=NULL);
    virtual void syncStream(Stream_t *stream);

private:

    /*
     * Private variables.
     */

    /* General. */
    struct impl;
    SPxAutoPtr<impl> m_p;           /* Private data. */
    SPxTimer m_timers;              /* Thread timer. */
    int m_isSending;                /* Timer running? */
    SPxSdbApplication *m_app;       /* Application object. */
    char *m_jsonBuf;                /* JSON send buffer. */
    unsigned int m_jsonBufSize;     /* Size of JSON send buffer. */
    UINT8 *m_zlibBuf;               /* ZLIB compressed send buffer. */
    unsigned int m_zlibBufSize;     /* Size of compressed send buffer. */
    unsigned int m_mtu;             /* Maximum transmission unit. */
    SPxTime_t m_startTime;          /* Application start time. */

    /*
     * Private functions.
     */

    /* General. */
    void sendHeartbeat(void);
    void syncApplication(void);
    void syncStreams(void);

    /*
     * Private static functions.
     */

    /* Timer callbacks. */
    static void sendHeartbeatTimer(void *userArg);

}; /* SPxExtHeartbeatSender */

#endif /* SPX_EXT_HEARTBEAT_SENDER_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
