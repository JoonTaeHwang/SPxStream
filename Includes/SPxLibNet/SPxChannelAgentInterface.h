/*********************************************************************
 *
 * (c) Copyright 2016, 2019, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPxChannelAgentInterface class header.
 *
 * Revision Control:
 *   12/03/19 v1.5    AGC	Remove unused impl.
 *
 * Previous Changes:
 *   28/09/16 1.4   SP 	Add IsNewConnection().
 *   23/09/16 1.3   SP 	Add safe value to GetRemoteParameter() functions.
 *   21/09/16 1.2   SP 	Derive from SPxObj.
 *   16/09/16 1.1   SP 	Initial version.
 *
 * *********************************************************************/

#ifndef _SPX_CHANNEL_AGENT_INTERFACE_H
#define _SPX_CHANNEL_AGENT_INTERFACE_H

/* Other headers required. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibSc/SPxScNet.h"

/*********************************************************************
 *
 *  Constants
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

/*********************************************************************
 *
 *   Class definitions
 *
 *********************************************************************/

/* Forward declare any classes required. */
class SPxChannel;
class SPxRemoteServer;

class SPxChannelAgentInterface : public SPxObj
{
public:

    /*
     * Public types.
     */

    /* Agent info. */
    typedef struct
    {
        UINT32 hostAddr;                    /* Host address. */
        unsigned int cmdPort;               /* Command port. */
        SPxRemoteServer *remoteSvr;         /* Interface to agent. */
        unsigned int refCount;              /* Reference count. */
        int isDisconnectPending;            /* Disconnect flag. */
        SPxThread *thread;                  /* Management thread. */
        int isNewConnection;                /* New connection? */

    } Agent_t;

    /*
     * Public variables.
     */

    /*
     * Public static functions.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxChannelAgentInterface(SPxChannel *channel);
    virtual ~SPxChannelAgentInterface(void);

    /* Control and status. */
    virtual SPxErrorCode SetAgent(UINT32 hostAddr, unsigned int cmdPort);
    virtual UINT32 GetHostAddr(void);
    virtual unsigned int GetCommandPort(void);
    virtual int IsConnected(void);
    virtual int IsNewConnection(int clearFlag);

    /* Remote parameter assignment. */
    virtual SPxErrorCode SetRemoteParameter(const char *objectName, 
                                            const char *paramName,
                                            const char *paramValue);
    virtual SPxErrorCode SetRemoteParameter(const char *objectName, 
                                            const char *paramName,
                                            int paramValue);
    virtual SPxErrorCode SetRemoteParameter(const char *objectName, 
                                            const char *paramName,
                                            unsigned int paramValue);
    virtual SPxErrorCode SetRemoteParameter(const char *objectName, 
                                            const char *paramName,
                                            double paramValue);

    /* Remote parameter retrieval. */
    virtual SPxErrorCode GetRemoteParameter(const char *objectName, 
                                            const char *paramName,
                                            int *valuePtr,
                                            int safeVal=0);
    virtual SPxErrorCode GetRemoteParameter(const char *objectName, 
                                            const char *paramName,
                                            unsigned int *valuePtr,
                                            unsigned int safeVal=0);
    virtual SPxErrorCode GetRemoteParameter(const char *objectName, 
                                            const char *paramName,
                                            double *valuePtr,
                                            double safeVal=0.0);
    virtual SPxErrorCode GetRemoteParameter(const char *objectName, 
                                            const char *paramName,
                                            char *valueBuf, 
                                            int bufLen,
                                            const char *safeVal=NULL);
    
protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);
    
private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    /* General. */
    SPxChannel *m_channel;
    Agent_t *m_agent;
    UINT32 m_pendingHostAddr;
    unsigned int m_pendingCmdPort;
    int m_isConnectionAllowed;
 
    /*
     * Private functions.
     */

    SPxErrorCode disconnect(void);

    /*
     * Private static functions.
     */

    static void *agentThread(SPxThread *thread);

    static void handleMsgExit(SPxRemoteServer *svr,
                              void *arg,
                              struct sockaddr_in *from,
                              char *msg,
                              int msgSizeBytes,
                              char *valPtr,
                              UINT32 val);

    static void manageAgent(Agent_t *agent);

}; /* SPxChannelAgentInterface */

#endif /* _SPX_CHANNEL_AGENT_INTERFACE */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
