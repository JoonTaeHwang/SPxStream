/*********************************************************************
*
* (c) Copyright 2012 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxPopupMenuWin.h,v $
* ID: $Id: SPxPopupMenuWin.h,v 1.7 2017/01/13 15:22:23 rew Exp $
*
* Purpose:
*   Header for SPxPopupMenuWin class which supports creating a pop-up
*   under Windows.
*
*
* Revision Control:
*   13/01/17 v1.7    AGC	Add CreateMenu().
*
* Previous Changes:
*   11/01/17 1.6    AGC	Support sub-menus.
*   10/09/15 1.5    AGC	Require window handle.
*   16/05/14 1.4    AGC	Change return values for Append functions.
*   21/10/13 1.3    AGC	Support custom drawn menus using SPxWinMenu.
*   07/10/13 1.2    AGC	Add required header.
*   21/12/12 1.1    AGC	Initial version.
*
**********************************************************************/

#ifndef _SPX_POPUP_MENU_WIN_H
#define _SPX_POPUP_MENU_WIN_H

#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxPopupMenu.h"
#include "SPxLibUtils/SPxAutoPtr.h"

/* Forward declarations. */
class SPxWinMenu;
class SPxMouseInterface;

/* Define our class. */
class SPxPopupMenuWin : public SPxPopupMenu
{
public:
    /* Public functions. */
    explicit SPxPopupMenuWin(HWND hwnd=NULL, SPxWinMenu *menu=NULL);
    virtual ~SPxPopupMenuWin(void);

    virtual int AppendItem(int enabled, int id,
			   const char *label,
			   SPxPopupMenuFn fn, void *userArg,
			   SPxMouseInterface *iface=NULL,
			   const SPxPopupMenu *subMenu=NULL);
    virtual void AppendSeparator(SPxMouseInterface *iface=NULL);
    virtual SPxErrorCode ShowMenu(void);
    
private:
    /* Private variables. */
    HMENU m_menu;
    SPxWinMenu *m_winMenu;
    WNDPROC m_origWinProc;
    HWND m_hwnd;
    int m_inWinProc;

    /* Private functions. */
    static LRESULT APIENTRY winProcFnWrapper(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT winProcFn(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    virtual SPxPopupMenu *CreateMenu(void);

}; /* SPxPopupMenuWin */

#endif /* _SPX_POPUP_MENU_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
