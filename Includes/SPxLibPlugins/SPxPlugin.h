/*********************************************************************
*
* (c) Copyright 2009, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxPlugin.h,v $
* ID: $Id: SPxPlugin.h,v 1.2 2009/07/31 13:02:15 rew Exp $
*
* Purpose:
*   Abstract class that is used as the base class for all
*   SPx plugin classes.
*
* Revision Control:
*   31/07/09 v1.2    SP 	Move non-class items to SPxPluginUtils.h.
*
* Previous Changes:
*   21/07/09 1.1    SP 	Initial version.
**********************************************************************/

#ifndef _SPX_PLUGIN_H
#define _SPX_PLUGIN_H

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

/*
 * Define the class itself.
 */
class SPxPlugin
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    SPxPlugin(void) { }
    virtual ~SPxPlugin(void) { }

}; /* SPxPlugin */

#endif /* _SPX_PLUGIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
