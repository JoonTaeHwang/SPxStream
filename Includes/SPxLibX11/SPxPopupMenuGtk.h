/*********************************************************************
*
* (c) Copyright 2012 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxPopupMenuGtk.h,v $
* ID: $Id: SPxPopupMenuGtk.h,v 1.6 2017/01/13 15:22:48 rew Exp $
*
* Purpose:
*   Header for SPxPopupMenuGtk  class which supports creating a pop-up
*   under GTK+.
*
*
* Revision Control:
*   13/01/17 v1.6    AGC	Add CreateMenu().
*
* Previous Changes:
*   11/01/17 1.5    AGC	Support sub-menus.
*   16/05/14 1.4    AGC	Change return values for Append functions.
*   07/10/13 1.3    AGC	Forward declare GTK structures.
*   21/12/12 1.2    REW	Fix missing newline on last line.
*   21/12/12 1.1    AGC	Initial version.
**********************************************************************/

#ifndef _SPX_POPUP_MENU_GTK_H
#define _SPX_POPUP_MENU_GTK_H

#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxPopupMenu.h"

/* Forward declarations. */
struct _GtkWidget;
struct _GtkMenuItem;
struct _GtkMenuShell;
union _GdkEvent;

/* Define our class. */
class SPxPopupMenuGtk : public SPxPopupMenu
{
public:
    /* Public functions. */
    SPxPopupMenuGtk(void);
    virtual ~SPxPopupMenuGtk(void);

    virtual int AppendItem(int enabled, int id,
			   const char *label,
			   SPxPopupMenuFn fn, void *userArg,
			   SPxMouseInterface *iface=NULL,
			   const SPxPopupMenu *subMenu=NULL);
    virtual void AppendSeparator(SPxMouseInterface *iface=NULL);
    virtual SPxErrorCode ShowMenu(void);

private:
    /* Private variables. */
    struct _GtkWidget *m_menu;
    int m_activated;

    /* Private functions. */
    static void onMenuItemActivate(struct _GtkMenuItem *menuItem, void *user_data);
    static void onMenuItemPress(struct _GtkWidget *menuItem, _GdkEvent *event, void *user_data);
    static void onSelectionDone(struct _GtkMenuShell *menuShell, void *user_data);

    virtual SPxPopupMenu *CreateMenu(void);

}; /* SPxPopupMenuGtk */

#endif /* _SPX_POPUP_MENU_GTK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
