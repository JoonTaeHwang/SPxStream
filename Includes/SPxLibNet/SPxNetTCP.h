/*********************************************************************
*
* (c) Copyright 2012 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header of SPxNetTCP class - An class for handling a TCP
*	connection with in-built asynchronous support.
*
* Revision Control:
*   06/09/22 v1.33   AGC	Suppress spurious cppcheck ODR warning.
*
* Previous Changes:
*   31/03/22 1.32   AGC	Support overriding idle sleep time for testing.
*   29/03/22 1.31   AGC	Fix warnings in last change.
*   29/03/22 1.30   AGC	Add connection ID to handle socket re-use.
*   01/03/22 1.29   AGC	Support recvAll option for synchronous receive.
*   23/02/22 1.28   AGC	Fix typo in serverIfaceAddr constructor arg.
*   18/10/21 1.27   AGC	Fix build errors in last change.
*   18/10/21 1.26   AGC	Increase maximum client connections to 1024 on Windows.
*   15/02/21 1.25   SP 	Add Set/GetMaxNumClients().
*   23/05/19 1.24   AGC	Support server interface set.
*			Support keepalive.
*			Support limited number of clients.
*			Add alternative connection handler.
*   24/01/19 1.23   AGC	Support user buffer for default receipt.
*   10/01/19 1.22   SP 	Rename SetPort() to SetPortNumber().
*   10/08/18 1.21   AGC	Support deferred starting of thread.
*   04/07/18 1.20   AGC	Add CloseClient() function.
*   25/06/18 1.19   AGC	Fix behaviour with multiple SendRecvs in queue.
*   15/06/18 1.18   AGC	Set thread name.
*   06/06/18 1.17   AGC	Add client connect callback.
*   22/02/17 1.16   AGC	Add SetPortNumber() function.
*   23/01/17 1.15   AGC	Add more functions for testing.
*   18/01/17 1.14   AGC	Support continue of existing Connect().
*   11/01/17 1.13   AGC	Support client disconnect callback.
*   12/12/16 1.12   AGC	Add functions for testing.
*   07/10/16 1.11   AGC	Support changing the port.
*   23/09/16 1.10   AGC	Support OS assigned TCP port.
*   20/04/16 1.9    AGC	Support reporting all connection failures.
*   17/02/16 1.8    AJH	Add sock to completion fn parameters.
*   11/02/16 1.7    AJH	Renamed to SPxNetTCP; removed std headers.
*   20/01/16 1.6    AGC	Support receiving minimum number of bytes.
*   22/10/15 1.5    AGC	Support for handling multiple clients.
*   06/10/15 1.4    AGC	Support callback function received data.
*   19/07/13 1.3    AGC	Fix issues under Windows.
*			Support retrieve locally bound address.
*   25/04/13 1.2    AGC	Support synchronous commands.
*			Improve error reporting from Connect failures.
*   02/08/12 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_NET_TCP_H
#define _SPX_NET_TCP_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SOCKET_TYPE. */
#include "SPxLibUtils/SPxCommon.h"

/* For SPX_SOCK_INVALID. */
#include "SPxLibSc/SPxScNet.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxNetTCP;
struct SPxNetTCPActionData;
class SPxThread;
struct timeval;

/* Supported actions. */
enum SPxNetTCPAction
{
    SPX_NET_TCP_ACTION_NONE=0,
    SPX_NET_TCP_ACTION_CONNECT=1,
    SPX_NET_TCP_ACTION_DISCONNECT=2,
    SPX_NET_TCP_ACTION_SEND=3,
    SPX_NET_TCP_ACTION_RECEIVE=4,
    SPX_NET_TCP_ACTION_SET_PORT=5,
    SPX_NET_TCP_ACTION_CLOSE_CLIENT=6,
    SPX_NET_TCP_ACTION_SET_INTERFACE=7,
    SPX_NET_TCP_ACTION_ENABLE_KEEPALIVE=8
};

typedef void (*SPxNetTCPCompletionFn)(SPxNetTCP *tcp,
				      SPxErrorCode errorCode,
				      SPxNetTCPAction action,
				      void* userArg,
				      const unsigned char* buffer,
				      unsigned int bufLen,
				      UINT32 fromAddr,
				      SOCKET_TYPE sock);
typedef void (*SPxNetTCPCompletionFn2)(SPxNetTCP *tcp,
				       SPxErrorCode errorCode,
				       SPxNetTCPAction action,
				       void* userArg,
				       const unsigned char* buffer,
				       unsigned int bufLen,
				       UINT32 fromAddr,
				       SOCKET_TYPE sock,
				       UINT32 id);
typedef void(*SPxNetTCPConnectFn)(SPxNetTCP *tcp,
				  void *userArg,
				  SOCKET_TYPE sock);
typedef void(*SPxNetTCPConnectFn2)(SPxNetTCP *tcp,
				   void *userArg,
				   UINT32 fromAddr,
				   SOCKET_TYPE sock);
typedef void(*SPxNetTCPConnectFn3)(SPxNetTCP *tcp,
				   void *userArg,
				   UINT32 fromAddr,
				   SOCKET_TYPE sock,
				   UINT32 id);
typedef void (*SPxNetTCPDisconnectFn)(SPxNetTCP *tcp,
				      void *userArg,
				      SOCKET_TYPE sock);
typedef void (*SPxNetTCPDisconnectFn2)(SPxNetTCP *tcp,
				       void *userArg,
				       SOCKET_TYPE sock,
				       UINT32 id);

/*
 * Define our class.
 */
class SPxNetTCP : public SPxObj
{
public:
    /* Public functions. */
    explicit SPxNetTCP(int supportAsync=TRUE, int port=0, const char *threadName=NULL,
		       int maxClients=-1, const char *serverIfaceAddr=NULL);
    virtual ~SPxNetTCP(void);
    void Start(void);
    void Stop(void);
    SPxErrorCode SetMaxNumClients(int num);
    int GetMaxNumClients(void) const;
    void SetServerIfAddr(const char *serverIfAddr);
    void SetPortNumber(int port);
    void SetPort(int port) { SetPortNumber(port); }
    UINT16 GetPort(void) const;
    void Connect(SPxNetTCPCompletionFn fn, void *userArg,
		 const char *addr, UINT16 port, const char *ifAddr=NULL,
		 int reportAllFailures=FALSE, int allowRetry=FALSE);
    SPxErrorCode Connect(const char *addr, UINT16 port, const char *ifAddr=NULL, int retry=TRUE);
    void Disconnect(SPxNetTCPCompletionFn fn, void *userArg);
    SPxErrorCode Disconnect(void);
    int IsConnected(void) const;
    UINT32 GetIfAddr(void) const;

    void EnableKeepAlive(void);

    void Send(SPxNetTCPCompletionFn fn, void *userArg,
	      const unsigned char *buffer, int bufLen,
	      SOCKET_TYPE sock=SPX_SOCK_INVALID);
    SPxErrorCode Send(const unsigned char *buffer, int bufLen,
		      SOCKET_TYPE sock=SPX_SOCK_INVALID);
    void Recv(SPxNetTCPCompletionFn fn, void *userArg,
	      UINT32 maxNumBytes, UINT32 timeoutMsecs, int recvAll=FALSE,
	      const unsigned char *recvdBuf=NULL, unsigned int recvdBytes=0);
    int Recv(unsigned char *recvBuffer, int recvBufLen, UINT32 timeoutMsecs,
	     int recvAll=FALSE);
    void SendRecv(SPxNetTCPCompletionFn fn, void *userArg,
		  const unsigned char *buffer, int bufLen,
		  UINT32 maxNumBytes, UINT32 timeoutMsecs);
    int SendRecv(const unsigned char *buffer, int bufLen,
		 unsigned char *recvBuffer, int recvBufLen,
		 UINT32 timeoutMsecs);
    void CloseClient(SOCKET_TYPE sock);

    SPxErrorCode SetDefaultRecv(SPxNetTCPCompletionFn fn, void *userArg,
				UINT32 maxNumBytes, unsigned char *userBuffer=NULL);
    SPxErrorCode SetDefaultRecv(SPxNetTCPCompletionFn2 fn, void *userArg,
				UINT32 maxNumBytes, unsigned char *userBuffer=NULL);
    SPxErrorCode SetDefaultRecv(int /*nullFn*/, void *userArg,
				UINT32 maxNumBytes, unsigned char *userBuffer=NULL)
    {
	return SetDefaultRecv(static_cast<SPxNetTCPCompletionFn>(NULL),
			      userArg, maxNumBytes, userBuffer);
    }
    SPxErrorCode SetDefaultRecv(long /*nullFn*/, void *userArg,
				UINT32 maxNumBytes, unsigned char *userBuffer=NULL)
    {
	return SetDefaultRecv(static_cast<SPxNetTCPCompletionFn>(NULL),
			      userArg, maxNumBytes, userBuffer);
    }
    SPxErrorCode SetConnectHandler(SPxNetTCPConnectFn fn, void *userArg);
    SPxErrorCode SetConnectHandler(SPxNetTCPConnectFn2 fn, void *userArg);
    SPxErrorCode SetConnectHandler(SPxNetTCPConnectFn3 fn, void *userArg);
    SPxErrorCode SetDisconnectHandler(SPxNetTCPDisconnectFn fn, void *userArg);
    SPxErrorCode SetDisconnectHandler(SPxNetTCPDisconnectFn2 fn, void *userArg);

    int IsQueueEmpty(void) const;

protected:
    static const int SPX_FD_SETSIZE = 1024;

    // Suppression required due to differing definitions on Win/Linux.
    // cppcheck-suppress ctuOneDefinitionRuleViolation
    struct SPxFDSet
    {
	int fd_count;
	SOCKET_TYPE fd_array[SPX_FD_SETSIZE];
    };

    /* Network functions that may be overriden for testing. */
    virtual int connect(SOCKET_TYPE s, const sockaddr *name, int nameLen);
    virtual int select(SPxFDSet *readfds, timeval *timeout);
    virtual int send(SOCKET_TYPE s, const char *buf, int len, int flags);
    virtual int recv(SOCKET_TYPE s, char *buf, int len, int flags);
    virtual int listen(SOCKET_TYPE s, int backlog);
    virtual SOCKET_TYPE accept(SOCKET_TYPE s, sockaddr *addr, SOCKLEN_TYPE *addrLen);
    virtual UINT32 GetIdleSleepMS(void) const { return 10; }

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    static void *threadFnWrapper(SPxThread *thread);
    void *threadFn(SPxThread *thread);
    void closeServerSockets(void);
    void tryAccept(void);
    int getAction(SPxNetTCPActionData& action);
    void execAction(SPxNetTCPActionData& action);
    void handleConnect(SPxNetTCPActionData& action, int async=TRUE);
    void handleDisconnect(SPxNetTCPActionData& action);
    void handleSend(SPxNetTCPActionData& action);
    int handleReceive(SPxNetTCPActionData& action, int all=FALSE, int mutexLocked=FALSE);
    void handleSetPort(SPxNetTCPActionData& action);
    void handleCloseClient(SPxNetTCPActionData& action);
    void handleSetInterface(SPxNetTCPActionData& action);
    void handleEnableKeepAlive(void);
    void reportError(const SPxNetTCPActionData& action, SPxErrorCode errorCode);
    int readSock(SPxNetTCPActionData& action, SOCKET_TYPE sock);
    static void sendComplete(SPxNetTCP *tcp, SPxErrorCode errorCode,
	SPxNetTCPAction action, void* userArg, const unsigned char* buffer,
	unsigned int bufLen, UINT32 fromAddr, SOCKET_TYPE sock);

    static void FDZero(SPxFDSet *fds);
    static void FDSet(SOCKET_TYPE sock, SPxFDSet *fds);
    static int FDIsSet(SOCKET_TYPE sock, const SPxFDSet *fds);

}; /* SPxNetTCP */

#endif /* _SPX_NET_TCP_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
