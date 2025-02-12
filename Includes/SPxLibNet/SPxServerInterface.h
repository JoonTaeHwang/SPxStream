/*********************************************************************
*
* (c) Copyright 2010 - 2017, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Declares the SPxServerInterface class, which allows
 *	a class to add server functionality such as heartbeats
 *	and client connections.
*
* Revision Control:
*   20/02/20 v1.6    SP 	Add SendHeartbeat() overload.
*
* Previous Changes:
*   21/02/17 1.5    AGC	Add option to disable control.
*   04/10/13 1.4    AGC	Simplify headers.
*   07/09/12 1.3    AGC	Use const for string constructor params.
*   11/10/10 1.2    AGC Changes for addition of cpp file.
*   04/10/10 1.1    AGC Initial version. 
**********************************************************************/
#ifndef _SPX_SERVER_INTERFACE_H
#define _SPX_SERVER_INTERFACE_H

/*
 * Other headers required.
 */
/* We need the SPxScSourceNet base class header. */
#include "SPxLibSc/SPxScSourceNet.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*********************************************************************
*
*   Macros
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/**
 * Define our class, derived from the SPxScSourceNet class.
 */
class SPxServerInterface :public SPxScSourceNet
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    explicit SPxServerInterface(int serverPort,
				FILE *log=NULL,
				int heartbeatPort=0,
				const char *heartbeatAddress=NULL,
				const char *serverAddress=NULL,
				int controlAvailable=TRUE);
    virtual ~SPxServerInterface();

    /* Function to check constructor was okay and sockets are created.
     * Returns TRUE if everything is okay, FALSE otherwise.
     */
    int SocketsOK(void)	{ return SPxScSourceNet::SocketsOK();}

    /* Function to try to accept a client connection (non-blocking). */
    SOCKET_TYPE TryAccept(void){ return SPxScSourceNet::TryAccept();}
    const struct sockaddr_in *GetClientAddr(void)
    {
	/* Return NULL if no client connected, or address struct otherwise. */
	return SPxScSourceNet::GetClientAddr();
    }

    /* Functions to run the command loop after accepting a connection,
     * and other functions to manipulate that loop.
     */
    void CommandLoop(void)			{ SPxScSourceNet::CommandLoop();}
    void StopCommandLoop(void)			{ SPxScSourceNet::StopCommandLoop();}
    void SetEventHandler(void (*fn)(void))	{ SPxScSourceNet::SetEventHandler(fn);}
    void SetEventSource(int fd)			{ SPxScSourceNet::SetEventSource(fd);}
    void SetTimeout(unsigned int second, unsigned int microseconds)
    {
	SPxScSourceNet::SetTimeout(second, microseconds);
    }

    void SetBackgroundProc(void (*fn)(void*), void* userData)
    {
	SPxScSourceNet::SetBackgroundProc(fn, userData);
    }

    /* Heartbeat control. */
    void SendHeartbeat(void)		{ SPxScSourceNet::SendHeartbeat();}
    void SendHeartbeat(UINT8 health, UINT16 sourceStatus, 
                       UINT8 sourceType, UINT8 numChannels) 
    { 
        SPxScSourceNet::SendHeartbeat(health, sourceStatus, 
                                      sourceType, numChannels);
    }
    void EnableHeartbeats(int state)	{ SPxScSourceNet::EnableHeartbeats(state);}
    void SetState(int state)		{ SPxScSourceNet::SetState(state);}
    int GetState(void)			{ return SPxScSourceNet::GetState();}
    void SetID(int id)			{ SPxScSourceNet::SetID(id);}
    int GetID(void)			{return SPxScSourceNet::GetID();}

}; /* SPxServerInterface class */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_SERVER_INTERFACE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
