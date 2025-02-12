/*********************************************************************
*
* (c) Copyright 2012 - 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxGtkGuiCtrl.h,v $
* ID: $Id: SPxGtkGuiCtrl.h,v 1.2 2014/05/21 11:26:06 rew Exp $
*
* Purpose:
*	Header for base class for re-usable GTK dialogs.
*
*
* Revision Control:
*   21/05/14 v1.2    SP 	Add SetAsPopup().
*
* Previous Changes:
*   31/08/12 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_GTK_GUI_CTRL_H
#define _SPX_GTK_GUI_CTRL_H

/*
* Other headers required.
*/

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
struct _GObject;
struct SPxGtkGuiCtrlPriv;

class SPxGtkGuiCtrl : public SPxObj
{
public:
    /* Public functions. */
    explicit SPxGtkGuiCtrl(const char *xml);
    virtual ~SPxGtkGuiCtrl(void);

    virtual SPxErrorCode SetAsPopup(const char *name,
                                    _GObject *parent,
                                    int noDelete=TRUE);
    _GObject *GetObj(const char *name) const;
    SPxErrorCode SyncFromObj(void);

private:
    /* Private variables. */
    SPxAutoPtr<SPxGtkGuiCtrlPriv> m_p;

    /* Private functions. */
    virtual SPxErrorCode Sync(void)=0;
    
}; /* SPxGtkGuiCtrl */

#endif /* _SPX_GTK_GUI_CTRL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
