/*********************************************************************
*
* (c) Copyright 2016 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxRestartApp.h,v $
* ID: $Id: SPxRestartApp.h,v 1.4 2017/10/18 13:38:35 rew Exp $
*
* Purpose:
*	Header for SPx class to restart an application with
*	the previously used or new configuration file.
*
* Revision Control:
*   18/10/17 v1.4    AGC	Add Linux support.
*				Rename SPxWinRestartApp to SPxRestartApp.
*
* Previous Changes:
*   11/01/17 1.3    SP 	Add closeAllResources(). 
*   23/08/16 1.2    AGC	Add functions for testing.
*   14/07/16 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef SPX_RESTART_APP_H_
#define SPX_RESTART_APP_H_

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxWideString.h"

/* Class used to start tasks. */
class SPxRestartApp
{
public:
    /* Public functions. */
    explicit SPxRestartApp(void);
    virtual ~SPxRestartApp(void);

    void SetArgs(int argc, SPX_CHAR *argv[]);
    void SetDefaultConfigFilename(const char *defaultConfigFilename);

    int HaveCmdLineConfigFilename(void) const;
    const char *GetConfigFilename(void) const;
    const char *GetExePath(void) const;
    void Restart(const char *extraCmdLineParams=NULL,
		 const char *configDomain=NULL);

    void BuildCmdLine(char *buffer, unsigned int bufLen,
		      const char *extraCmdLineParams=NULL,
		      const char *configDomain=NULL) const;

protected:
    /* Functions for testing. */
    virtual const char *GetConfigShortPath(const char *configDomain) const;

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    void closeAllResources(void);
    void buildCmdArgs(const char *extraCmdLineParams,
		      const char *configDomain) const;
    void buildCmdLineFromArgs(char *buffer, unsigned int bufLen) const;

}; /* SPxRestartApp */

#endif /* SPX_RESTART_APP_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
