/*********************************************************************
 *
 * (c) Copyright 2015 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Used to add webserver and websocket functionality to an
 *   application, typically to implement a web interface.
 *
 * Revision Control:
 *   14/10/22 v1.53  AGC	Add errCode to getHttpExt().
 *
 * Previous Changes:
 *   22/09/22 1.52  AGC	Support installing resource providers.
 *   25/07/22 1.51  AGC	Support client ID for websockets.
 *   05/07/22 1.50  AJH	Support variable reply buffer size.
 *   27/05/22 1.49  AGC	Update for re-organised web files.
 *   29/04/22 1.48  AGC	Support reading default web UI settings.
 *   05/04/22 1.47  AGC	Support querying help availability.
 *   29/03/22 1.46  AGC	Add connection ID to handle socket re-use.
 *   16/02/22 1.45  AGC	Support plaintext external user auth passwords.
 *   26/01/22 1.44  AGC	Support external user authentication.
 *   21/01/22 1.43  AGC	Support simple user authentication.
 *   17/12/21 1.42  AGC	Only show remote control page when supported.
 *   07/12/21 1.41  AGC	Support num async history messages config.
 *   25/11/21 1.40  AGC	Improve callback on writable behaviour.
 *   28/10/21 1.39  AGC	Restrict use of getFile().
 *   05/10/21 1.38  BTB	Support optionally storing historical message strings for any output.
 *   04/08/21 1.37  AGC	Fix Linux UTF-8 websocket issues.
 *   19/07/21 1.36  SP 	Make IsSecure() virtual to avoid
 *			dependency on libwebsockets when called by
 *			SPxExtHeartbeatSender.
 *   16/07/21 1.35  SP 	Add IsSecure().
 *   08/07/21 1.34  AGC	Add developer mode.
 *   28/06/21 1.33  AGC	Pass from address to command processing.
 *   21/06/21 1.32  AGC	Web resource header content now gzipped.
 *   18/06/21 1.31  AGC	Store and report historic messages to clients.
 *   18/06/21 1.30  AGC	Report async messages to clients.
 *   15/06/21 1.29  AGC	Support buffer output via websocket.
 *   19/05/21 1.28  AGC	Support adding auto-generated resources.
 *   19/05/21 1.27  AGC	Rename getHttpUnbounded() to getHttpExt().
 *   05/05/21 1.26  AGC	Handle fragmented commands.
 *   13/01/21 1.25  AGC	Fix mutex errors when setting config from state.
 *   11/01/21 1.24  AGC	Support SSL.
 *   21/12/20 1.23  SP 	Support access to single global object.
 *   11/12/20 1.22  REW	Maintain copy of current client address.
 *   06/01/20 1.21  AGC	Support unbounded HTTP get/post.
 *   27/11/19 1.20  AGC	Suppress cppcheck warning.
 *   26/11/19 1.19  AGC	Improve testability.
 *   23/05/19 1.18  AGC	Use SPxRemoteControlIface for server interface.
 *   01/05/19 1.17  AGC	Support serving help files.
 *   08/01/19 1.16  AGC	Support OS allocated port.
 *   20/08/18 1.15  AGC	Tidy up line endings.
 *   10/05/18 1.14  AJH	Change long to size_t.
 *   09/05/18 1.13  AJH	Support file upload.
 *   26/03/18 1.12  AJH	Make getFile() protected.
 *   10/10/17 1.11  AGC	Support specifying network interface.
 *			Add getHttpPost() function.
 *   06/06/17 1.10  AGC	Allow user arg for AddProtocol().
 *			Add RemoveProtocol().
 *   08/03/17 1.9   AGC	Improve mutex locking.
 *   23/09/16 1.8   AJH	Add Shutdown().
 *   23/08/16 1.7   AGC	Fix lock-ups under Linux.
 *   11/05/16 1.6   AGC	Update to match changes in new libwebsockets version.
 *   20/04/16 1.5   AGC	Support adding protocols.
 *   12/02/16 1.4   SP 	Add Set/GetPortNumber().
 *   21/09/15 1.3   AGC	Forward declare SPxThread.
 *   26/08/15 1.2   SP  Many changes.
 *   29/07/15 1.1   SP  Initial version.
 *
 *********************************************************************/

#ifndef _SPX_WEB_INTERFACE_H
#define _SPX_WEB_INTERFACE_H

/* Library headers. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxMsgRpt.h"

/* Forward declare any classes required. */
class SPxRemoteControlIface;
class SPxThread;
class SPxWebProviderIface;

/* Forward declare data types required so that we do not need to
 * include the libwebsockets.h header file here. Note that we cannot
 * forward declare libwebsocket_callback_reasons here as it is an
 * enum. Instead we use int for this argument and cast it within
 * the function.
 */
struct lws_context;
struct lws;

/* Structure used to install web resources. */
struct SPxWebResource
{
    const char *page;
    const char *ext;
    const UINT8 *data;
    unsigned int dataLen;
};

/* Authentication type. */
enum SPxWebInterfaceAuthType
{
    SPX_WEB_IF_AUTH_TYPE_NONE = 0,
    SPX_WEB_IF_AUTH_TYPE_USERPASS = 1,
    SPX_WEB_IF_AUTH_TYPE_EXT = 2

};

/* SPxWebInterface */
class SPxWebInterface : public SPxObj, public SPxMessageReporterIface
{
public:
    typedef int (*ProtocolHandler)(lws *wsi,
				   int reason,
				   void *user, void *in,
				   size_t len);
    struct AuthInfo;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    SPxWebInterface(void);
    SPxErrorCode AddProtocol(const char *name, ProtocolHandler fn,
			     unsigned int perSessionDataSize,
			     void *userArg);
    SPxErrorCode RemoveProtocol(const char *name);
    virtual SPxErrorCode Create(unsigned int port=8080, const char *iface=NULL);
    virtual ~SPxWebInterface(void);

    // cppcheck-suppress virtualCallInConstructor
    virtual void Shutdown(void);

    /* Set/get application name. */
    virtual SPxErrorCode SetApplicationName(const char *name);
    virtual const char *GetApplicationName(void) { return m_appName ? m_appName : ""; }

    /* Set/get server interface to handle command messages. */
    virtual SPxErrorCode SetServerInterface(SPxRemoteControlIface *svrIf);
    virtual SPxRemoteControlIface *GetServerInterface(void) { return m_svrIf; }

    /* Set/get web server port. */
    virtual SPxErrorCode SetPortNumber(unsigned int port);
    virtual unsigned int GetPortNumber(void) const;
    virtual unsigned int GetPortNumberUsed(void) const;

    /* UI settings. */
    SPxErrorCode SetUISettingsFile(const char *filename);
    SPxErrorCode GetUISettingsFile(char *filename, int bufLen) const;

    /* Authentication. */
    SPxErrorCode SetAuthType(SPxWebInterfaceAuthType authType);
    SPxWebInterfaceAuthType GetAuthType(void) const;
    SPxErrorCode SetUsername(const char *username);
    SPxErrorCode GetUsername(char *username, int bufLen) const;
    SPxErrorCode SetPassword(const char *password);
    SPxErrorCode SetExtAuthUrl(const char *extAuthUrl);
    SPxErrorCode GetExtAuthUrl(char *extAuthUrl, int bufLen) const;
    SPxErrorCode SetExtAuthTimeoutSecs(int extAuthTimeoutSecs);
    int GetExtAuthTimeoutSecs(void) const;
    SPxErrorCode SetExtAuthPasswordHash(int enableExtAuthPasswordHash);
    int GetExtAuthPasswordHash(void) const;

    /* Set location of certificate and key. */
    SPxErrorCode SetCertificatePath(const char *certPath);
    const char *GetCertificatePath(void) const;
    SPxErrorCode SetKeyPath(const char *keyPath);
    const char *GetKeyPath(void) const;
    SPxErrorCode SetKeyPassword(const char *keyPass);
    const char *GetKeyPassword(void) const;

    /* Reply buffer size. */
    SPxErrorCode SetReplyBufferSize(size_t size) { m_replyBufSize = size; return SPX_NO_ERROR; }
    size_t GetReplyBufferSize(void) { return m_replyBufSize; }

    /* This function must be virtual, otherwise it will add a 
     * dependency on libwebsockets to applications that send
     * extended heartbeats (as it's called by SPxExtHeartbeatSender).
     */
    virtual int IsSecure(void) const;

    /* Asynchronous output through websocket. */
    unsigned int AddAsyncOutput(const char *name,
				unsigned int numStoredHistoricMessages=0);
    SPxErrorCode RemoveAsyncOutput(const char *name);
    SPxErrorCode OutputAsyncBuffer(unsigned int key,
				   const unsigned char *data,
				   unsigned int dataLen,
				   int isBinary=TRUE,
				   const char *clientID=NULL);
    SPxErrorCode OutputAsyncString(unsigned int key,
				   const char *str,
				   unsigned int len=0,
				   const char *clientID=NULL);

    /* Install resource provider. */
    SPxErrorCode AddProvider(SPxWebProviderIface *provider);
    SPxErrorCode RemoveProvider(SPxWebProviderIface *provider);

    /* Install resources to serve. */
    SPxErrorCode AddResources(const SPxWebResource *resources,
			      unsigned int numResources);
    template<unsigned int Len>
    SPxErrorCode AddResources(const SPxWebResource(&resources)[Len])
    {
	return AddResources(resources, Len);
    }
    SPxErrorCode SetResourcePage(const char *oldPageName,
				 const char *newPageName);

    /* Developer folders to search. */
    SPxErrorCode AddDeveloperFolder(const char *folder);

    /* Set help location. */
    virtual SPxErrorCode SetHelpDir(const char *path);
    int IsHelpAvailable(void) const;

    /* Unbounded GET response. */
    SPxErrorCode SendHttpUnbounded(void* session);

    const char *GetIface(void);

    SPxErrorCode SetDeveloperMode(int developerMode);

protected:
    /*
     * Protected variables.
     */
    /* Info about source of request */
    struct sockaddr_in m_fromAddr;	/* Address of far end */


    /*
     * Protected functions.
     */

    /* Optionally override these functions in a derived class
     * to add functionality to the web server.
     */

    /* Called to handle an HTTP request. */
    virtual int httpHandler(lws *wsi,
			    int reason,
			    void *user, void *in,
			    size_t len);

    /* Called to get part of an unbounded response or access to URL args. */
    virtual int getHttpExt(const char *urlExt,
			   const char * /*urlExtWithArgs*/,
			   const char * /*sessionCookie*/,
			   char *buf,
			   unsigned int bufSize,
			   char *mime,
			   unsigned int mimeSize,
			   char *encoding,
			   unsigned int encodingSize,
			   void * /*sessionPtr*/,
			   unsigned int *errCode)
    {
	/* When unbounded HTTP is not implemented by derived class,
	 * fallback to bounded HTTP GET.
	 */
	return static_cast<int>(getHttp(urlExt, buf, bufSize,
					mime, mimeSize,
					encoding, encodingSize,
					errCode));
    }

    /* Called to get an HTTP item, e.g. a webpage or image. */
    virtual unsigned int getHttp(const char *urlExt,
                                 char *buf,
                                 unsigned int bufSize,
                                 char *mime,
                                 unsigned int mimeSize,
                                 char *encoding,
                                 unsigned int encodingSize,
                                 unsigned int *errCode);

    /* Called to get the URL of the main logo. */
    virtual const char *getLogoUrl(void) const;

    /* Called to get the error page. */
    virtual unsigned int getErrorPage(const char *urlExt,
                                      char *buf,
                                      unsigned int bufSize,
                                      char *mime,
                                      unsigned int mimeSize);

    /* Called to get the login page. */
    virtual unsigned int getLoginPage(const char *urlExt,
                                      const char *urlWithArgs,
                                      char *buf,
                                      unsigned int bufSize,
                                      char *mime,
                                      unsigned int mimeSize);

    /* Called from getHttp() to get text. */
    unsigned int getText(const char *text,
                         const char *fileExt,
                         char *buf,
                         unsigned int bufSize,
                         char *mime,
                         unsigned int mimeSize);

    /* Called from getHttp() to get binary data, e.g. an image. */
    unsigned int getData(const UINT8 *data,
                         unsigned int dataSize,
                         const char *fileExt,
                         char *buf,
                         unsigned int bufSize,
                         char *mime,
                         unsigned int mimeSize);

    /* Called from getHttp() to get contents of a file. */
    unsigned int getFile(const char *urlExt,
                         char *buf,
                         unsigned int bufSize,
                         char *mime,
                         unsigned int mimeSize,
                         int isAbsolutePath=FALSE);

    /* Called to get an HTTP POST response */
    virtual unsigned int getHttpPost(const char *urlExt,
                                     const char *body,
                                     char *buf,
                                     unsigned int bufSize,
                                     char *mime,
                                     unsigned int mimeSize,
                                     unsigned int *errCode);

    virtual void handleHttpPostData(const char * /*urlExt*/,
				    const char * /*urlArgs*/,
				    const char * /*sessionCookie*/,
				    const char * /*data*/,
				    unsigned int /*dataLen*/) {};

    virtual void closeHttp(const char* /*urlExt*/,
			   const char* /*urlExtWithArgs*/,
			   const char* /*sessionCookie*/) {};

    /* Called to get an HTTP POST file upload response */
    virtual unsigned int getHttpFile(const char *urlExt,
				     const char *filename,
				     const char *filebuf,
				     size_t filelen,
				     char *buf,
				     unsigned int bufSize,
				     char *mime,
				     unsigned int mimeSize,
				     unsigned int *errCode);

    /* Use this function to get the MIME type. */
    unsigned int getMime(const char *fileExt,
                         char *mime,
                         unsigned int mimeSize);

    /* Called from getHttp() to get the default web page. */
    unsigned int getDefaultPage(const char *urlExt,
                                char *buf,
                                unsigned int bufSize,
                                char *mime,
                                unsigned int mimeSize);

    /* Called to handle an SPx command message. */
    virtual int commandHandler(lws *wsi,
                               int reason,
                               void *user, void *in,
                               size_t len);

    /* Called to handle a single command message. */
    virtual int handleCommand(const char *cmd, 
                              char *replyBuf, 
                              unsigned int replyBufSize,
                              SOCKET_TYPE sock,
                              UINT32 fromAddr,
                              UINT32 id,
                              const char *clientID);
    virtual void handleCommandFinished(SOCKET_TYPE sock, UINT32 id);

    /* Called to handle async output. */
    virtual int asyncHandler(lws *wsi,
			     int reason,
			     void *user, void *in,
			     size_t len);

    virtual int uploadHandler(void *data,
	const char *name,
	const char *filename,
	char *buf, int len,
	int stateInt);

    /* SPxMessageReporterIface function. */
    virtual int SendMessage(const char *buf, int len = 0);

    /* Parameter set/get. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private types.
     */
    struct impl;

    /*
     * Private variables.
     */

    /* General. */
    SPxAutoPtr<impl> m_p;
    SPxThread *m_thread;                    /* Web server thread. */
    unsigned int m_pollTimeoutMsecs;        /* Timeout when polling for messages. */
    unsigned int m_port;                    /* Port number to listen on. */
    unsigned int m_portUsed;                /* Actual port number used. */
    unsigned int m_cmdBufSize;              /* Command buffer size. */
    size_t m_replyBufSize;		    /* Reply buffer size. */
    SPxAutoPtr<char[]> m_cmdBuf;            /* Command buffer. */
    UINT32 m_nextCmdConnID;                 /* Next command connection ID. */
    SPxRemoteControlIface *m_svrIf;         /* Server interface object. */
    char *m_appName;                        /* Application name. */
    mutable SPxCriticalSection m_mutex;     /* Object mutex. */
    mutable SPxCriticalSection m_sslMutex;  /* Mutex for SSL strings. */
    mutable SPxCriticalSection m_asyncMutex; /* Mutex for async handling. */
    SPxAtomic<int> m_stateChanged;
    unsigned int m_spxMessagesKey;          /* Key for async message output. */

    /*
     * Private functions.
     */
    SPxErrorCode setResourcePage(const char *oldPageName,
				 const char *newPageName,
				 int addToMap);
    lws *getNextCallbackOnWritableTask(void);
    void eraseFromCallbackOnWritable(lws *wsi);
    SPxErrorCode outputAsyncBuffer(unsigned int key,
				   const unsigned char *data,
				   unsigned int dataLen,
				   int isBinary=TRUE,
				   const char *clientID=NULL);
    SPxErrorCode outputAsyncString(unsigned int key,
				   const char *str,
				   unsigned int len=0,
				   const char *clientID=NULL);
    int isUserAuthenticated(lws *wsi,
			    const char *username=NULL,
			    const char *password=NULL,
			    int remember=FALSE,
			    char *authCookieBuf=NULL,
			    unsigned int authCookieBufLen=0);
    int revalidateAuth(const char *username,
		       AuthInfo& authInfo);
    int validateExtAuth(const char *username,
			const char *password,
			const char *passwordHash) const;
    void genAuthHash(const char *salt,
		     char *buf,
		     unsigned int bufLen);
    void genPassHash(const char *password,
		     char *buf,
		     unsigned int bufLen);

    /*
     * Private static functions.
     */

    static void *webServerHandler(SPxThread *thread);
    static int httpHandlerWrapper(lws *wsi,
                                  int reason,
                                  void *user, void *in,
                                  size_t len);
    static int commandHandlerWrapper(lws *wsi,
                                     int reason,
                                     void *user, void *in,
                                     size_t len);
    static int asyncHandlerWrapper(lws *wsi,
				   int reason,
				   void *user, void *in,
				   size_t len);
    static int uploadHandlerWrapper(void *data,
				    const char *name,
				    const char *filename,
				    char *buf, int len,
				    int stateInt);
    static SPxErrorCode ensureUTF8(char *buffer,
				   unsigned int bufLen);
    static SPxErrorCode getUrlArg(const char *urlWithArgs,
				  const char *argName,
				  char *argBuf,
				  unsigned int argBufLen);
    static int isSubDelim(char c);

};

#endif /* _SPX_WEB_INTERFACE_H */

/* Global web interface object. */
extern SPxWebInterface *SPxGlobalWebInterface;

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
