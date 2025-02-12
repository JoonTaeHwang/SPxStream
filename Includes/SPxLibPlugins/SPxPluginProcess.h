/*********************************************************************
*
* (c) Copyright 2009, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxPluginProcess.h,v $
* ID: $Id: SPxPluginProcess.h,v 1.5 2013/11/22 16:52:45 rew Exp $
*
* Purpose:
*   Base class used for all SPx plugin process classes.
*
* Revision Control:
*   22/11/13 v1.5    AGC	Fix cppcheck warning.
*
* Previous Changes:
*   15/10/13 1.4    AGC	Fix warnings from SPxLibAVMfc.
*   04/10/13 1.3    AGC	Simplify headers.
*   13/08/09 1.2    SP 	Pass instance name to init function.
*   31/07/09 1.1    SP 	Initial version.
*
**********************************************************************/

#ifndef _SPX_PLUGIN_PROCESS_H
#define _SPX_PLUGIN_PROCESS_H

/* Other headers */
#include "SPxLibPlugins/SPxPlugin.h"

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

/* Forward declarations. */
class SPxProcess;

/*
 * Define the class itself.
 */
class SPxPluginProcess : public SPxPlugin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    SPxPluginProcess(void) : m_process(NULL) { }
    virtual ~SPxPluginProcess(void) { }

    /* Function to get SPx process handle */
    virtual SPxProcess *GetProcess(void) { return m_process; }

    /* Function to initialise object (must be overridden in derived class) */
    virtual SPxErrorCode Init(const char * /*name*/) { return SPX_ERR_ASSERT; }

protected:
    /*
     * Protected variables.
     */

    /* SPx process handle (must be initialised in derived class) */
    SPxProcess *m_process;

    /*
     * Protected functions.
     */    

private:
    /*
     * Private variables.
     */

    /*
     * Private functions.
     */

}; /* SPxPluginProcess */


/*********************************************************************
*
*   Function Prototypes
*
**********************************************************************/

#endif /* _SPX_PLUGIN_PROCESS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
