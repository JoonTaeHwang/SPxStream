/*********************************************************************
 *
 * (c) Copyright 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Basic wrapper around a websocket to implement client functions.
 *   Could be extended to server functions in the future if needed.
 *
 * Revision Control:
 *   25/10/18 v1.3   SP 	Support SPxNetAddr.
 *
 * Previous Changes:
 *   22/10/18 1.2   AGC	Suppress erroneous cppcheck warning.
 *   15/10/18 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_WEBSOCKET_H
#define _SPX_WEBSOCKET_H

/*
 * Other headers required.
 */

/* Other classes that we need. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibNet/SPxNetAddr.h"

/* Forward declare any classes required. */

/* Forward declare data types required so that we do not need to
 * include the libwebsockets.h header file here. Note that we cannot
 * forward declare libwebsocket_callback_reasons here as it is an
 * enum. Instead we use int for this argument and cast it within
 * the function.
 */
struct lws_context;
struct lws;

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

/* Forward declare the class so it can be used below. */
class SPxWebsocket;

/* Define the type of the callback data handler functions. */
typedef void (* SPxWebsocketFn_t)(SPxWebsocket *ws,
                                  void *userArg,
                                  const SPxTime_t *timestamp,
                                  const unsigned char *payload,
                                  unsigned int numBytes);

/*
 * Define our class.
 */
class SPxWebsocket : public SPxObj, public SPxNetAddr
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
    SPxWebsocket(void);
    virtual ~SPxWebsocket(void);

    /* Required to implement SPxNetAddr. */
    virtual SPxErrorCode SetAddress(const char *address, int port=0, const char *ifAddr=NULL);
    virtual SPxErrorCode SetAddress(UINT32 address, int port=0, const char *ifAddr=NULL);
    virtual UINT32 GetAddress(void) const;
    virtual const char *GetAddressString(void) const;
    virtual int GetPort(void) const;
    virtual UINT32 GetIfAddress(void) const;
    virtual const char *GetIfAddressString(void) const;

    /* Connection. */
    virtual SPxErrorCode Connect(const char *protocol=NULL);
    virtual int IsConnectionStarted(void);
    virtual int IsConnected(void);

    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode Disconnect(void);

    /* Send and receive. */
    virtual SPxErrorCode SendText(const char *text);
    virtual SPxErrorCode SetReceiveHandler(SPxWebsocketFn_t fn, void *userArg);

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
    SPxAutoPtr<impl> m_p;
    SPxThread *m_thread;                /* Web socket thread. */
    int m_port;                         /* Port to connect to. */
    int m_isConnected;                  /* Is websocket connected? */
    SPxCriticalSection m_sendMutex;     /* Send mutex. */
    SPxCriticalSection m_recvMutex;     /* Receive mutex. */
    SPxWebsocketFn_t m_recvFn;          /* Receive handler. */
    void *m_recvFnUserArg;              /* Receive handler user arg. */

    /*
     * Private functions.
     */

    /*
     * Private static functions.
     */

    static void *threadHandler(SPxThread *thread);

    static int protocolHandler(lws *wsi, 
                               int reasonInt,
                               void *user, 
                               void *in, 
                               size_t len);

}; /* SPxWebsocket */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_WEBSOCKET_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
