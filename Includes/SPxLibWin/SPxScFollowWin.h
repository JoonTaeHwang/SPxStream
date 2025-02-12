/*********************************************************************
*
* (c) Copyright 2007 - 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxScFollowWin.h,v $
* ID: $Id: SPxScFollowWin.h,v 1.14 2013/10/04 15:31:08 rew Exp $
*
* Purpose:
*   Header for class to track window geometries.
*
* Revision Control:
*   04/10/13 v1.14   AGC	Simplify headers.
*
* Previous Changes:
*   13/01/12 1.13   AGC	Move most functionality to new non-MFC superclass.
*   15/09/10 1.12   REW	Make destructor virtual.
*   11/08/10 1.11   REW	Separate SPX_MAX_NUM_SLAVE_WINS from the
*			base class SPX_FOLLOW_WIN_MAX usage.
*   28/07/10 1.10   SP 	Protect against multiple inclusion.
*			Add SetWin() function.
*   22/03/09 1.9    DGJ	Enhance comments. Rework priority layering.
*			Array of SPxScDestDisplayWin pointers.
*   21/10/08 1.8    DGJ	Remove requestToComplete
*   23/01/08 1.7    DGJ	Support multiple slave windows and ensure
*			that slave's geometry is set properly.
*   07/01/08 1.6    DGJ	Added slave window.
*			Added new constructor option
*   26/08/07 1.5    DGJ	Support multiple radar sources. Inherit from
*			SPxScFollow. Use SPxThread. Support GetThread().
*   29/06/07 1.4    DGJ	Derive class from SPxObj.
*   19/06/07 1.3    REW	Fix CurrentY/W/H() access functions.
*   15/06/07 1.2    DGJ	Add visibility and polling fields.
*   13/05/07 1.1    DGJ	Initial Version
**********************************************************************/

#ifndef _SPX_SC_FOLLOW_WIN_H
#define _SPX_SC_FOLLOW_WIN_H

#include "SPxLibWin/SPxScFollowWinRaw.h"

/* Forward declarations. */
class SPxScDestDisplayWin;
class SPxSc;

/* Class used to track a MS application window that will contain scan
 * converted radar video 
 */

class SPxScFollowWin :public SPxScFollowWinRaw
{
public:
    SPxScFollowWin(SPxSc *sc, CWnd *win);
    SPxScFollowWin(SPxSc *sc, CWnd *win, SPxScDestDisplayWin *destDisplay);
    virtual ~SPxScFollowWin(void);
    CWnd *GetWin(void) {return CWnd::FromHandle(GetHwnd()); }
    void SetWin(CWnd *win) { SPxScFollowWinRaw::SetWin(win ? win->m_hWnd : NULL); }
    void SetSlaveWindow(int index, CWnd *win) { SPxScFollowWinRaw::SetSlaveWindow(index, win ? win->m_hWnd : NULL); }
    CWnd *GetSlaveWindow(int i) { return CWnd::FromHandle(GetSlaveHwnd(i)); }
    SPxScDestDisplayWin *GetDD(int i) { return (SPxScDestDisplayWin*)GetDDRaw(i); }
};

#endif /* _SPX_SC_FOLLOW_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
