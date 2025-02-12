/*********************************************************************
*
* (c) Copyright 2013, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxFileServer.h,v $
* ID: $Id: SPxFileServer.h,v 1.3 2014/07/04 14:47:16 rew Exp $
*
* Purpose:
*   Simple file server implementation.
*
* Revision Control:
*   04/07/14 v1.3    AGC	Move string header to source file.
*
* Previous Changes:
*   18/11/13 1.2    REW	Rename GetData() to GetBinaryData().
*   15/11/13 1.1    SP 	Initial version.
**********************************************************************/

#ifndef _SPX_FILE_SERVER_H
#define _SPX_FILE_SERVER_H

/*
 * Other headers required.
 */

/* Library headers. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxError.h"

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

/* Forward declare any other classes we need. */

/*
 * Define our class.
 */
class SPxFileServer : public SPxObj
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
    SPxFileServer(void);
    virtual ~SPxFileServer(void);

    /* Configuration. */
    SPxErrorCode SetRootDir(const char *dirName);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);
    virtual int GetBinaryData(char *command, char *dataBuf, 
                        unsigned int bufLen, unsigned int *dataLen);

private:

    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /*
     * Private functions.
     */

    SPxErrorCode GetFileList(const char *regex, char *buf, int bufLen);
    SPxErrorCode GetFile(const char *filename,
                         char *buf, 
                         unsigned int bufLen,
                         unsigned int *dataLen);
    
}; /* SPxFileServer */

#endif /* SPX_FILE_SERVER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
