/*********************************************************************
*
* (c) Copyright 2009, 2010, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxPluginHandler.h,v $
* ID: $Id: SPxPluginHandler.h,v 1.5 2013/10/04 15:31:08 rew Exp $
*
* Purpose:
*   Header for the plugin handler class.
*
* Revision Control:
*   04/10/13 v1.5    AGC	Simplify headers.
*
* Previous Changes:
*   15/09/10 1.4    REW	Make destructor virtual.
*   13/08/09 1.3    SP 	Make ObjectInfo_t private.
*   31/07/09 1.2    SP 	Add init function, object handling and version.
*   21/07/09 1.1    SP 	Initial version.
**********************************************************************/

#ifndef _SPX_PLUGIN_HANDLER_H
#define _SPX_PLUGIN_HANDLER_H

/* Other headers */
#include "SPxLibPlugins/SPxPluginUtils.h"
#include "SPxLibUtils/SPxObj.h"

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
class SPxPlugin;

/*
 * Define the class itself.
 */
class SPxPluginHandler : public SPxObj
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    SPxPluginHandler(void);
    virtual ~SPxPluginHandler(void);
    SPxErrorCode Open(const char *name);
    SPxErrorCode Close(void);
    int TestSymbol(const char *sym);
    void *LoadSymbol(const char *sym, SPxErrorCode *errRtn);
    SPxPlugin *CreateObj(const char *className, SPxErrorCode *errRtn);
    SPxErrorCode DestroyObj(SPxPlugin *obj);

private:
    /*
     * Private variables.
     */

    /* Name of the plugin */
    char m_pluginName[256];

    /* Name of plugin file including path and file extension */
    char *m_filename;

    /* Handle for plugin file */
#ifdef _WIN32
    HMODULE m_handle;
#else
    void *m_handle;
#endif

    /* Plugin interface function pointers */
    SPxPluginOpenFn_t m_openFn;
    SPxPluginCloseFn_t m_closeFn;

    /* Structure used to store object information in a linked list */
    typedef struct _ObjectInfo
    {
        /* Object handle */
        SPxPlugin *handle;

        /* Previous object in the list or NULL if first entry */
        _ObjectInfo *prev;    

        /* Next object in list or NULL if last entry */
        _ObjectInfo *next;

    } ObjectInfo_t;

    /* Linked list of objects created using the plugin */
    ObjectInfo_t *m_objList;

    /*
     * Private functions.
     */

}; /* SPxPluginHandler */

/*********************************************************************
*
*   Public Function Prototypes
*
**********************************************************************/

extern SPxErrorCode SPxPluginInit(const SPxLibInfo_t *libInfo);

#endif /* _SPX_PLUGIN_HANDLER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
