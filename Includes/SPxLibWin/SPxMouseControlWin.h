/*********************************************************************
*
* (c) Copyright 2012 - 2019, Cambridge Pixel Ltd.
*
*
* Purpose:
*   Header file for SPxMouseControlWin class.
*
*
* Revision Control:
*   20/02/19 v1.7    BTB	Support enabling/disabling use of most custom cursors.
*
* Previous Changes:
*   21/09/15 1.6    AGC	Support parameter control.
*   23/06/15 1.5    AGC	Support custom touch interaction.
*   07/01/15 1.4    SP 	Support improved double-click handling.
*   16/05/14 1.3    AGC	Support automatic context menus.
*   04/11/13 1.2    AGC	Support adding the same window to multiple
*			SPxMouseControlWin objects.
*   23/11/12 1.1    AGC	Initial Version.
*
**********************************************************************/

#ifndef _SPX_MOUSE_CONTROL_WIN_H
#define _SPX_MOUSE_CONTROL_WIN_H

/*
 * Other headers required.
 */

/* For base class. */
#include "SPxLibUtils/SPxMouseControl.h"

/* Forward declare other classes required. */
class SPxViewControl;
class SPxWinMenu;

/*********************************************************************
*
*   Class definitions
*
**********************************************************************/

/*
 * Define our class, derived from the SPx base object.
 */
class SPxMouseControlWin : public SPxMouseControl
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    explicit SPxMouseControlWin(SPxViewControl *viewControl=NULL);
    virtual ~SPxMouseControlWin(void);

    SPxErrorCode SetWindow(HWND hwnd, int enableTouch=FALSE);
    SPxErrorCode SetMenu(SPxWinMenu *menu);
    SPxErrorCode SetDoubleClickFilter(int isEnabled);
    SPxErrorCode SetCustomCursorsEnabled(int isEnabled);

private:
    /*
     * Private variables.
     */
    HWND m_hwnd;
    SPxWinMenu *m_menu;
    int m_trackingMouse;
    int m_doubleClickFilter;
    UINT32 m_lastLeftDownMsecs;
    int m_ignoreNextLeftUp;
    int m_enableTouch;
    int m_numTouches;
    int m_touchX[10];
    int m_touchY[10];
    POINT m_pos;
    int m_useCustomCursors;

    /*
     * Private functions.
     */
    virtual int HandleSetCursor(void);
    static LRESULT APIENTRY winProcFnWrapper(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT winProcFn(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    int useMessage(void) const;
    UINT32 getModKeyState(void) const;
    int onLButtonDown(WPARAM wParam, LPARAM lParam);
    int onLButtonUp(WPARAM wParam, LPARAM lParam);
    int onTouch(WPARAM wParam, LPARAM lParam);
    virtual SPxPopupMenu *createPopupMenu(void) const;
    virtual int PostMousePress(UINT32 flags, int x, int y);
    virtual int PostMouseRelease(UINT32 flags, int x, int y);
    virtual int PostDoubleClick(UINT32 flags, int x, int y);
    virtual int PostMouseMove(UINT32 flags, int x, int y);
    virtual int PostMouseLeave(void);
    virtual int PostMouseWheel(UINT32 flags, int x, int y, double delta);

}; /* SPxMouseControlWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_MOUSE_CONTROL_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
