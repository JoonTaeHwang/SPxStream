/*********************************************************************
*
* (c) Copyright 2009, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxPluginUtils.h,v $
* ID: $Id: SPxPluginUtils.h,v 1.4 2013/10/04 15:31:08 rew Exp $
*
* Purpose:
*   Header for plugin utilities.
*
* Revision Control:
*   04/10/13 v1.4    AGC	Simplify headers.
*
* Previous Changes:
*   31/08/11 1.3    AGC	Use new style error handler.
*   13/08/09 1.2    SP 	Pass parameter linked-list to plugin.
*   31/07/09 1.1    SP 	Initial version.
*
**********************************************************************/

#ifndef _SPX_PLUGIN_UTILS_H
#define _SPX_PLUGIN_UTILS_H

/* Other headers */
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxParams.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Plugin interface function names */
#define SPX_PLUGIN_OPEN_FN_NAME          "SPxPluginOpen"
#define SPX_PLUGIN_CLOSE_FN_NAME         "SPxPluginClose"
#define SPX_PLUGIN_CREATE_OBJ_FN_PREFIX  "SPxPluginCreateObj_"
#define SPX_PLUGIN_DESTROY_OBJ_FN_NAME   "SPxPluginDestroyObj"

/*********************************************************************
*
*   Macros
*
**********************************************************************/

/* 
 * Include this in plugin source for each plugin object.
 * This function is called by the plugin handler to create
 * objects using the plugin.
 */
#define SPX_PLUGIN_CREATE_OBJECT_FUNCTION(className)         \
extern "C" className *SPxPluginCreateObj_##className()       \
{                                                            \
    return new className;                                    \
}

/* 
 * Include this in plugin source once only.
 * This function is called by the plugin handler to destroy
 * any object that was created using the plugin.
 */
#define SPX_PLUGIN_DESTROY_OBJECT_FUNCTION()                 \
extern "C" void SPxPluginDestroyObj(SPxPlugin *obj)          \
{                                                            \
    delete obj;                                              \
}

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxCriticalSection;
class SPxPlugin;

/* SPx library information supplied to a plugin */
typedef struct
{
    /* SPx library version, usually set to SPX_VERSION  */
    UINT32 version;

    /* Pointer to SPx error handler function */
    SPxErrorHandlerFnV2_t errFn;

    /* Start and end of parameter linked-list */
    SPxStringList *firstParam;
    SPxStringList *lastParam;
    
    /* Mutex used to protect access to parameter linked-list */
    SPxCriticalSection *paramListAccess;

} SPxLibInfo_t;

/* Plugin open and close function pointer */
typedef SPxErrorCode (*SPxPluginOpenFn_t)(const SPxLibInfo_t *spxLibInfo);
typedef SPxErrorCode (*SPxPluginCloseFn_t)(void);

/*
 * Class factory function pointers types. Class factory functions
 * are functions used to create and destroy objects from a plugin.
 * These function must be implemented in the plugin source file
 * and prefixed with 'extern "C"' to give them 'C' linkage.
 */
typedef SPxPlugin *(*SPxPluginCreateObjFn_t)(void);
typedef void (*SPxPluginDestroyObjFn_t)(SPxPlugin *obj);

/*********************************************************************
*
*   Public Function Prototypes
*
**********************************************************************/

extern SPxErrorCode SPxPluginInit(const SPxLibInfo_t *libInfo);

#endif /* _SPX_PLUGIN_UTILS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
