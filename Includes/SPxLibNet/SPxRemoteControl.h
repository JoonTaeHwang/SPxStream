/*********************************************************************
*
* (c) Copyright 2019 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxRemoteControl class, which allows server
*	applications to accept one or more client connections
*	for remote control.
*
* Revision Control:
*   25/07/22 v1.8    AGC	Add client ID to command handling.
*
* Previous Changes:
*   31/03/22 1.7    AGC	Support use with SPxNetTCP for testing.
*   29/03/22 1.6    AGC	Add connection ID to handle socket re-use.
*   28/06/21 1.5    AGC	Handle from address.
*   05/05/21 1.4    AGC	Handle fragmented requests.
*   15/02/21 1.3    SP 	Add Set/GetMaxNumClients().
*			Add SetPortNumber().
*   09/03/20 1.2    SP 	Support password protection.
*   23/05/19 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_REMOTE_CONTROL_H
#define _SPX_REMOTE_CONTROL_H

/* For SPX_SCNET_DEFAULT_PORT_SERVER. */
#include "SPxLibSc/SPxScNet.h"

/* For SPxNetTCPAction. */
#include "SPxLibNet/SPxNetTCP.h"

/* For base classes. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibSc/SPxScSourceNet.h"

class SPxRemoteControl : public SPxObj, public SPxRemoteControlIface
{
public:
    explicit SPxRemoteControl(int maxClients,
			      int port=SPX_SCNET_DEFAULT_PORT_SERVER,
			      const char *interfaceAddr=NULL);
    virtual ~SPxRemoteControl(void);

    SPxErrorCode SetPortNumber(UINT16 port);
    UINT16 GetPort(void) const;
    UINT32 GetClientAddr(void) const;
    SPxErrorCode SetMaxNumClients(int num);
    int GetMaxNumClients(void) const;
    unsigned int GetNumClients(void) const;

    SPxErrorCode SetHeartbeatPort(UINT16 hbPort);

    SPxErrorCode EnableKeepAlive(void);

    SPxErrorCode SetPassword(const char *password);
    SPxErrorCode GetPassword(char *buf, unsigned int bufLen) const;

    virtual int ProcessCommand(const char *commandString, int len,
			       char *returnBuf, int returnBufLen,
			       int *returnBufFilledPtr,
			       SOCKET_TYPE sock,
			       UINT32 fromAddr,
			       UINT32 id,
			       const char *clientID);
    virtual void CloseCommand(SOCKET_TYPE sock, UINT32 id);
    virtual int SendMessage(const char *buf, int len=0);

    void SetLogFile(FILE *logFile);
    void SetDebug(UINT32 debug);

protected:
    /* Constructor for testing. */
    explicit SPxRemoteControl(int maxClients,
			      int port,
			      const char *interfaceAddr,
			      SPxNetTCP *tcp);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    static void connectHandler(SPxNetTCP *tcp,
			       void *userArg,
			       UINT32 fromAddr,
			       SOCKET_TYPE sock,
			       UINT32 id);
    static void disconnectHandler(SPxNetTCP *tcp,
				  void *userArg,
				  SOCKET_TYPE sock,
				  UINT32 id);
    static void recvFnWrapper(SPxNetTCP *tcp,
			      SPxErrorCode errorCode,
			      SPxNetTCPAction action,
			      void* userArg,
			      const unsigned char* buffer,
			      unsigned int bufLen,
			      UINT32 fromAddr,
			      SOCKET_TYPE sock,
			      UINT32 id);
    void recvFn(SPxNetTCP *tcp,
		SPxErrorCode errorCode,
		SPxNetTCPAction action,
		const unsigned char* buffer,
		unsigned int bufLen,
		UINT32 fromAddr,
		SOCKET_TYPE sock,
		UINT32 id);

    int processCommand(const char *buf,
		       int bufLen,
		       char *returnBuf,
		       int returnBufLen,
		       unsigned int *returnBufUsedLenPtr,
		       int *returnBufFilledPtr,
		       int *isBinPtr,
		       int *isCmdPtr,
		       SOCKET_TYPE sock,
		       UINT32 fromAddr,
		       UINT32 id,
		       const char *clientID,
		       SPxErrorCode *errRtn);
    SPxErrorCode handleCommand(const char *command,
			       const char *msg,
			       char *returnBuf,
			       int returnBufLen,
			       unsigned int *returnBufUsedLenPtr,
			       int *returnBufFilledPtr,
			       int *isBinPtr,
			       SOCKET_TYPE sock,
			       UINT32 fromAddr,
			       UINT32 id,
			       const char *clientID);

};


#endif /* _SPX_REMOTE_CONTROL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
