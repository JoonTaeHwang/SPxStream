/*********************************************************************
*
* (c) Copyright 2012 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxPopupMenu.h,v $
* ID: $Id: SPxPopupMenu.h,v 1.8 2017/01/13 15:22:02 rew Exp $
*
* Purpose:
*   Header for SPxPopupMenu base class which supports creating a pop-up
*   under Windows or GTK+ using the derived class.
*
*
* Revision Control:
*   13/01/17 v1.8    AGC	Add CreateMenu().
*
* Previous Changes:
*   11/01/17 1.7    AGC	Support sub-menus.
*   16/05/14 1.6    AGC	Add IsEmpty() and GetNumItems().
*			Change return values for Append functions.
*   02/12/13 1.5    SP 	Add SetClickSoundEnabled().
*   08/10/13 1.4    AGC	Remove static Create() function.
*   07/10/13 1.3    AGC	Add static Create() function.
*   21/12/12 1.2    REW	Fix missing newline at end of file.
*   21/12/12 1.1    AGC	Initial version.
**********************************************************************/

#ifndef _SPX_POPUP_MENU_H
#define _SPX_POPUP_MENU_H

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"

/* Forward declarations. */
class SPxPopupMenu;
class SPxMouseInterface;

/* Callback function type. */
typedef void (*SPxPopupMenuFn)(SPxPopupMenu *menu, int id, void *userArg);

/* Define our class. */
class SPxPopupMenu : public SPxObj
{
public:
    /* Public functions. */
    SPxPopupMenu(void);
    virtual ~SPxPopupMenu(void);

    virtual int AppendItem(int enabled, int id,
			   const char *label,
			   SPxPopupMenuFn fn, void *userArg,
			   SPxMouseInterface *iface=NULL,
			   const SPxPopupMenu *subMenu=NULL)=0;
    virtual void AppendSeparator(SPxMouseInterface *iface=NULL)=0;
    virtual SPxErrorCode ShowMenu(void)=0;
    virtual SPxPopupMenu *CreateMenu(void)=0;

    int IsEmpty(void) const;
    unsigned int GetNumItems(void) const;

    /* Enable / disable click sound. */
    void SetClickSoundEnabled(int state) { m_isClickSoundEnabled = state; }

protected:
    /* Protected functions. */
    int addItem(int id, SPxPopupMenuFn fn, void *userArg);
    void menuItemClicked(int id);
    SPxPopupMenuFn getItemFn(int id) const;
    void *getItemUserArg(int id) const;

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    int m_isClickSoundEnabled; 

}; /* SPxPopupMenu */

#endif /* _SPX_POPUP_MENU_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
