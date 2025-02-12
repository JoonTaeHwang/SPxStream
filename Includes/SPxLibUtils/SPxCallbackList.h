/*********************************************************************
*
* (c) Copyright 2008, 2010, 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxCallbackList.h,v $
* ID: $Id: SPxCallbackList.h,v 1.6 2015/02/16 14:30:45 rew Exp $
*
* Purpose:
*	Header for SPx callback list class.
*
* Revision Control:
*   16/02/15 v1.6    AGC	Move SPxCallbackListFn_t to SPxObj header.
*
* Previous Changes:
*   07/11/14 1.5    AGC	Support arbitrary number of callbacks.
*   04/10/13 1.4    AGC	Simplify headers.
*   15/09/10 1.3    REW	Make destructor virtual.
*   26/04/10 1.2    REW	Make max callbacks a constructor arg.
*   07/01/08 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_CALLBACK_LIST_H
#define _SPX_CALLBACK_LIST_H

/* Need common definitions, errors and base class. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Default maximum number of callbacks per object we currently support (no longer used). */
#define	SPX_CALLBACK_LIST_MAX_FUNCS_DEFAULT	8


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/*
 * SPx Callback List class
 */
class SPxCallbackList :public SPxObj
{
public:
    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxCallbackList(void *invokingObject,
	    unsigned int maxFuncs = SPX_CALLBACK_LIST_MAX_FUNCS_DEFAULT);
    virtual ~SPxCallbackList(void);

    /* Functions to add/remove callbacks. */
    int AddCallback(SPxCallbackListFn_t fn, void *userObject);
    int RemoveCallback(SPxCallbackListFn_t fn, void *userObject);

    /* Function to invoke all the callbacks. */
    int Invoke(void *arg);

private:
    /*
     * Private fields.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;
};

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_CALLBACK_LIST_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
