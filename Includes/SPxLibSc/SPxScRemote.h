/*********************************************************************
*
* (c) Copyright 2007, 2009, 2010, 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxScRemote.h,v $
* ID: $Id: SPxScRemote.h,v 1.10 2014/02/17 17:18:15 rew Exp $
*
* Purpose:
*  Header file for SPxScRemote, an interface to a remote scan-converter.
*
*  Applications wishing to use the scan-conversion functionality of a
*  server running remotely (possibly on another machine) should use this
*  class to access and control it.
*
*  This class provides the network interface setup and control, with the
*  API for the scan conversion itself coming from the SPxSc base class.
*
*
* Revision Control:
*   17/02/14 v1.10   AGC	Use const where appropriate.
*
* Previous Changes:
*   15/09/10 1.9    REW	Make destructor virtual.
*   18/11/09 1.8    REW	Add timeoutMsecs to Connect().
*   23/06/09 1.7    REW	Add other forms of Set/GetRemoteParameter().
*   07/09/07 1.6    REW	Add GetRemoteParameter().
*   15/08/07 1.5    REW	Remove deprecated API.
*   16/07/07 1.4    DGJ	Add SetRemoteParameter()
*   09/07/07 1.3    REW	New API etc. Comments.
*   29/06/07 1.2    DGJ	Constructor now in .cpp file.
*   08/06/07 1.1    DGJ	Initial Version.
**********************************************************************/

#ifndef _SPxScRemote_h
#define _SPxScRemote_h

/*
 * Other headers required.
 */
#include "SPxScDestNet.h"


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/*
 * Define the class itself, derived from SPxScDestNet (which in turn is
 * derived from SPxSc, the provider of most of the scan conversion control
 * functions).
 */
class SPxScRemote :public SPxScDestNet
{
public:
    /*
     * Public functions.
     */
    /* Constructor/destructor. */
    SPxScRemote(void);
    virtual ~SPxScRemote(void) {;}

    /* Network handling. */
    static int MakeNetwork(const char *serverAddress = NULL,
				int serverPort = 0,
				int heartbeatPort = 0,
				const char *heartbeatAddress = NULL);
    static int Connect(unsigned int timeoutMsecs=0);
    static int Disconnect(void);

    /* Set/get parameters on a named object */
    int SetRemoteParameter(const char *objectName, const char *parameterName,
						const char *parameterValue);
    int SetRemoteParameter(const char *objectName, const char *parameterName,
						int parameterValue);
    int SetRemoteParameter(const char *objectName, const char *parameterName,
						unsigned int parameterValue);
    int SetRemoteParameter(const char *objectName, const char *parameterName,
						REAL32 parameterValue);
    int GetRemoteParameter(const char *objectName, const char *parameterName,
						int *valuePtr);
    int GetRemoteParameter(const char *objectName, const char *parameterName,
						unsigned int *valuePtr);
    int GetRemoteParameter(const char *objectName, const char *parameterName,
						REAL32 *valuePtr);
    int GetRemoteParameter(const char *objectName, const char *parameterName,
						char *valueBuf, int bufLen);

    /* See the underlying SPxSc base class for most of the scan conversion
     * control functions.
     */

    /* Log reporting. */
    static void SetLogFile(FILE *logfile)
    {
	/* Set the log file through the parent's static class. */   
	SPxScDestNet::SetLogFile(logfile);
    }
    static void SetDebug(UINT32 debug)
    {
	/* Set debug on the underlying class. */
	SPxScDestNet::SetDebug(debug);
    }

private:
    /*
     * Currently no private functions etc.
     */
}; /* SPxScRemote */
         

#endif /* _SPxScRemote_h */
 
/*********************************************************************
*
* End of file
*
**********************************************************************/
