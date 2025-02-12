/*********************************************************************
*
* (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxServerBase object which encapsulates a
*	server with heartbeats and a remote parameter interface.
*
*
* Revision Control:
*   23/06/22 v1.31   AJH	Add SetHeartbeatInterface().
*
* Previous Changes:
*   20/05/22 1.30   AGC	Suppress spurious cppcheck warning.
*   18/05/22 1.29   AGC	Support initial inactive state (no remote connections).
*   05/04/22 1.28   AGC	Support checking for help availability.
*   07/01/22 1.27   AGC	Add setConfigFromState option to SaveConfig().
*   12/08/21 1.26   AGC	Add GetAppType().
*   26/07/21 1.25   AGC	Use const for heartbeat address param.
*   19/07/21 1.24   AJH	Support set/get of heartbeat address/port.
*   01/07/21 1.23   AGC	Make SaveConfig() virtual.
*   30/06/21 1.22   AGC	Add Save parameter.
*   06/05/21 1.21   AGC	Add heartbeat port config.
*			Allow timer interval override.
*   22/04/21 1.20   AJH	Add SetPortNumber().
*   11/12/20 1.19   SP 	Add GetNumClients().
*   09/03/20 1.18   SP 	Support remote control password protection.
*   04/02/20 1.17   SP 	Make Create() virtual.
*   13/11/19 1.16   SP 	Fix build warnings.
*   12/11/19 1.15   SP 	Support extended heartbeats.
*   11/11/19 1.14   AGC	Fix data race on shutdown when testing.
*   04/11/19 1.13   AGC	Support enable/disable of heartbeats.
*   23/05/19 1.12   AGC	Support multiple remote clients.
*			Support asynchronous messages.
*			Support setting number of channels.
*   21/06/18 1.11   AGC	Allow config of interface for heartbeats/control.
*   06/06/18 1.10   AGC	Improve behaviour with app count limit.
*   08/02/16 1.9    AGC	Support for disabling client connections.
*   21/09/15 1.8    AGC	Add GetServer().
*   26/06/14 1.7    AGC	Add GetAppBit().
*   20/06/14 1.6    AGC	Add HasLicenseExpired().
*   01/05/14 1.5    AGC	Add IsGracePeriodOver().
*   10/04/14 1.4    AGC	Support setting title.
*   13/12/13 1.3    AGC	Add grace period option to GetLicenseCaption().
*   08/11/13 1.2    AGC	Add Create() function.
*   21/10/13 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_SERVER_H
#define _SPX_SERVER_H

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPX_SCNET_HB_ID_SERVER. */
#include "SPxLibSc/SPxScNet.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/* For SPxSdbApplication::Type_t */
#include "SPxLibUtils/SPxSdbApplication.h"

/* Forward declarations. */
class SPxRemoteControlIface;
class SPxExtHeartbeatSender;

/*
 * Types
 */

class SPxServerBase : public SPxObj
{
public:
    explicit SPxServerBase(const char *title, UINT32 projectCode,
			   int port=SPX_SCNET_DEFAULT_PORT_SERVER,
			   int id=SPX_SCNET_HB_ID_SERVER, UINT32 appBit=0,
			   int maxClients=1,
			   const char *heartbeatAddr=NULL,
			   const char *clientInterface=NULL,
			   SPxSdbApplication::Type_t appType=SPxSdbApplication::TYPE_UNKNOWN,
			   int hbPort=SPX_SCNET_DEFAULT_PORT_HEARTBEAT,
			   int initiallyActive=TRUE);
    virtual ~SPxServerBase(void);
    virtual SPxErrorCode Create(void);
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode Shutdown(void);
    // cppcheck-suppress virtualCallInConstructor
    virtual void SetActive(int active);

    const char *GetTitle(void) const;
    UINT32 GetProjectCode(void) const;
    int IsControlAvailable(void) const;
    int GetPort(void) const;
    int GetAppBit(void) const;
    virtual int IsLicensed(int allowGrace=FALSE) const;
    virtual int IsGracePeriodOver(void) const;
    int HasLicenseExpired(void) const;
    SPxErrorCode GetLicenseDesc(char *buffer, unsigned int numBytes) const;
    SPxErrorCode GetLicenseCaption(char *buffer, unsigned int numBytes,
	UINT32 gracePeriodSecs=3600) const;
    SPxRemoteControlIface *GetServer(void);
    unsigned int GetNumClients(void) const;
    SPxSdbApplication::Type_t GetAppType(void) const;

    SPxErrorCode SendMessage(const char *buf, int len=0);

    void SetNumChannels(UINT8 numChannels);
    UINT8 GetNumChannels(void) const;
    void EnableHeartbeats(int enable);
    int AreHeartbeatsEnabled(void) const;
    SPxExtHeartbeatSender *GetExtHeartbeatSender(void);
    SPxErrorCode SetPortNumber(int port);
    SPxErrorCode SetPassword(const char *password);
    SPxErrorCode GetPassword(char *buf, unsigned int bufLen) const;

    SPxErrorCode SetHelpAvailable(int helpAvailable);
    int IsHelpAvailable(void) const;

    SPxErrorCode SetConfigDomain(const char *domain);
    virtual SPxErrorCode SaveConfig(const char *filename)
    {
	return SaveConfig(filename, TRUE);
    }
    virtual SPxErrorCode SaveConfig(const char *filename,
				    int setConfigFromState);

    SPxErrorCode SetHeartbeatAddress(const char *addr);
    SPxErrorCode GetHeartbeatAddress(char *buf, int buflen);
    SPxErrorCode SetHeartbeatInterface(const char *ifAddr);

    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    virtual UINT32 GetTimerIntervalMS(void) const { return 1000; }

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Derived classes may override this function. */
    virtual void background(void) {};

    /* Private functions. */
    static void timerWrapper(void *userArg);
    void timer(void);
    void sendHeartbeat(void);
};

#endif /* _SPX_SERVER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
