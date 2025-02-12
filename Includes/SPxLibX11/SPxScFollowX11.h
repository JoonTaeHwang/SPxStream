/*********************************************************************
*
* (c) Copyright 2007 - 2010, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxScFollowX11.h,v $
* ID: $Id: SPxScFollowX11.h,v 1.11 2011/10/07 14:52:47 rew Exp $
*
* Purpose:
*   Header file for SPxScFollowX11 class
*
*
* Revision Control:
*   07/10/11 v1.11   AGC	Add support for sub-areas.
*
* Previous Changes:
*   26/11/10 1.10   AGC	Add SetRestack() to enforce stacking order
*			with no scan conversion window.
*   15/09/10 1.9    REW	Make destructor virtual.
*   29/10/08 1.8    DGJ	Fix bug with follow windows on non zero screen
*   20/05/08 1.7    DGJ	Ensure that geometry of slaved window
*		        is checked after initial addition.
*   01/02/08 1.6    DGJ	Remove AppContext from class (tidy comments)
*   24/08/07 1.5    DGJ	Added access functions for GetSC(),
*			Class inherits from SPxScFollow.
*   20/08/07 1.4    DGJ	Added access functions for window position/size
*   13/06/07 1.3    REW	Include necessary X headers.
*   11/06/07 1.2    REW	SPxLibSC changed to SPxLibSc.
*   06/06/07 1.1    DGJ	Initial Version.
**********************************************************************/
#include "stdio.h"

#include  <X11/Intrinsic.h>

#include "SPxLibSc/SPxSc.h"
#include "SPxLibUtils/SPxScFollow.h"

#define SPX_SET_WIDGET_UNDERLAY 1
#define SPX_SET_WIDGET_OVERLAY  2
#define SPX_SET_WINDOW_UNDERLAY 4
#define SPX_SET_WINDOW_OVERLAY 8     
     
class SPxScFollowX11 :public SPxScFollow
{
    Display *m_display;             // X Display connection for Window
    Window m_windowToFollow;        // The Window being followed.
    Widget m_widgetToFollow;        // The Widget being followed.   
    XtIntervalId m_timerId;         // Timer used for auto polling.
    int m_timerInterval;            // Timer interval in milliseconds
    int m_state;                    // The visibility state of the Window.
    int m_flag;                     // Represent various actions on creation.
    int m_additionalFlag;           // Flag for the additional window/widget.
    Window m_additionalWindowToManage;
    Widget m_additionalWidgetToManage;
    int m_structureChange;	    // Something changes.
    int m_screenNumber;		    // Screen number for window.
    int m_restack;		    // Restack windows?
    int m_subAreaX, m_subAreaY, m_subAreaW, m_subAreaH; // Sub-area.
    int m_subAreaEnabled;
    int m_subAreaSizeChanged;
    int m_subAreaPosChanged;

    Window FindRootWindow(Window win);

public:
    void SetWindow(Window window) {m_windowToFollow = window;}
                                // Set/Change the window being tracked.      
    void SetWidget(Widget widget) {m_widgetToFollow = widget; m_windowToFollow=0;}
                                // Set/Change the widget being tracked.
    int GetIntervalMS(void) {return m_timerInterval;}        
    SPxScFollowX11(SPxSc *slave, Display *display, Window windowToFollow, int flags=0);
                                // Create with a Window      
    SPxScFollowX11(SPxSc *slave, Widget widget, int flags=0);
                                // Create with a widget
    virtual ~SPxScFollowX11();
    void AddWindow(Window window, int flag=0);  // Add an additional window to manage
    void AddWidget(Widget widget, int flag=0);  // Add an additional widget to manage.
    int CheckWindow(void);      // Check the window geometry now.
    void CheckUnderTimer(int intervalMS);
                                // Check geometry under a timer.
    void RemoveTimer(void); 
    void SetTimerId(XtIntervalId timerId) {m_timerId = timerId;} 
    void CommonInit(void);
    Display *GetDisplay(void) {return m_display;}
    void SetRestack(int restack) { m_restack = restack; }
    void SetSubArea(int x, int y, int w, int h);
    void EnableSubArea(int enable);
    void GetSize(int *w, int *h);
};

/*********************************************************************
*
* End of file
*
**********************************************************************/
