/*********************************************************************
*
* (c) Copyright 2012 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*   Header file for SPxMouseControl class.
*
*
* Revision Control:
*   15/10/20 v1.34   AGC	Support rubber-band zoom oneshot.
*				Support rubber-band zoom button selection.
*
* Previous Changes:
*   01/10/20 1.33   AGC	Support rubber-band zoom.
*   17/03/20 1.32   AGC	Support access to mouse control from interface.
*   04/03/20 1.31   AGC	Improve SPxAVDisp/MouseControl deadlock fix.
*   02/03/20 1.30   AGC	Fix potential SPxAVDisp/MouseControl deadlock.
*   19/12/19 1.29   AGC	Support enable/disable of mouse interfaces.
*   18/02/18 1.28   BTB	Allow SPxAVDisp to handle context functionality.
*   02/02/18 1.27   AGC	Add arrow cursor.
*   31/10/17 1.26   AGC	Add cross cursor.
*   06/10/17 1.25   SP 	Add drag and drop support.
*   18/07/17 1.24   SP 	Add SetViewCtrlPanEnabled() and SetViewCtrlZoomEnabled().
*   22/06/17 1.23   AGC	Add Tilt and Pan/Tilt cursors.
*   12/04/17 1.22   AGC	Enforce minimum time for click and drag.
*   31/01/17 1.21   AGC	Support separators in sub-menus.
*   11/01/17 1.20   AGC	Support sub-menus in context menus.
*   21/09/15 1.19   AGC	Support parameter control.
*   10/09/15 1.18   AGC	Ignore mouse events when showing menu.
*   14/04/15 1.17   SP 	Allow view ctrl to be enabled / disabled.
*   07/01/15 1.16   SP 	Support default cursor.
*   11/12/14 1.15   SP 	Add SPX_CURSOR_VSIZE & SPX_CURSOR_HSIZE.
*   02/06/14 1.14   AGC	Generate mouse move message on modifier key change.
*   16/05/14 1.13   AGC	Support automatic context menus.
*   05/03/14 1.12   SP 	Report mouse release as handled 
*                       when view is panned.
*   13/12/13 1.11   SP 	Add SetZoomOnDoubleClick().
*   02/12/13 1.10   SP 	Add SetClickSoundEnabled().
*   08/11/13 1.9    AGC	Fix cppcheck warnings.
*   04/11/13 1.8    AGC	Add Set/GetObj() to SPxMouseInterface.
*   15/05/13 1.7    AGC	Only respond to mouse move events where mouse has moved.
*   25/04/13 1.6    AGC	Fix panning when double-clicking title bar.
*   04/03/13 1.5    AGC	Support high-precision mice when scrolling.
*   25/01/13 1.4    AGC	Support adding interfaces to front of list.
*   12/12/12 1.3    AGC	Allow recursion of handlers.
*   23/11/12 1.2    REW	Add virtual destructor.
*   23/11/12 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_MOUSE_CONTROL_H
#define _SPX_MOUSE_CONTROL_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxPopupMenu.h"

/* Forward declare other classes required. */
class SPxViewControl;
class SPxRenderer;
class SPxPopupMenuLater;
class SPxMouseControl;

/* Cursor modes. */
enum SPxCursor_t
{
    SPX_CURSOR_NONE, /* Do not set cursor. */
    SPX_CURSOR_STD,
    SPX_CURSOR_VIEW,
    SPX_CURSOR_SELECT,
    SPX_CURSOR_VSIZE,
    SPX_CURSOR_HSIZE,
    SPX_CURSOR_TILT,
    SPX_CURSOR_PAN_TILT,
    SPX_CURSOR_CROSS,
    SPX_CURSOR_ARROW
};

/* Zoom modes/ */
enum SPxZoomMode_t
{
    SPX_ZOOM_DRAG_DROP = 0,
    SPX_ZOOM_RUBBER_BAND = 1,
    SPX_ZOOM_RUBBER_BAND_ONESHOT = 2
};

/* Interface that classes can derive from. */
class SPxMouseInterface
{
public:
    typedef void SubMenu;

    SPxMouseInterface(void)
	: m_obj(NULL), m_mouseCtrl(NULL), m_ctxMenu(NULL),
	m_menuEnabled(TRUE), m_menuPriority(0)
    {}
    virtual ~SPxMouseInterface(void) {}

    /* Mouse move and click handler functions. */
    virtual int HandleMousePress(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseRelease(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleDoubleClick(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseMove(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseLeave(void);
    virtual int HandleMouseWheel(UINT32 flags, int x, int y, double delta);

     /* Drag and drop handler functions. */
    virtual SPxObj *HandleDragDropQueryStart(UINT32 flags, int x, int y);
    virtual void HandleDragDropStart(UINT32 flags, int x, int y, SPxObj *dragObj);
    virtual void HandleDragDropMove(UINT32 flags, int x, int y, SPxObj *dragObj, SPxObj *dropObj);
    virtual SPxObj *HandleDragDropQueryEnd(UINT32 flags, int x, int y, SPxObj *dragObj);
    virtual void HandleDragDropEnd(UINT32 flags, int x, int y, SPxObj *dragObj, SPxObj *dropObj);

    int AppendMenuItem(int enabled, const char *label,
	SPxPopupMenuFn fn, void *userArg, SPxMouseInterface *iface=NULL);
    void AppendMenuSeparator(SPxMouseInterface *iface=NULL);

    SubMenu *AppendSubMenu(const char *label,
			   SPxMouseInterface *iface=NULL);
    int AppendSubMenuItem(SubMenu *subMenu,
			  int enabled, const char *label,
			  SPxPopupMenuFn fn, void *userArg);
    void AppendMenuSeparator(SubMenu *subMenu);

    void SetObj(SPxObj *obj) { m_obj = obj; }
    SPxObj *GetObj(void) const { return m_obj; }

    void SetMouseControl(SPxMouseControl *mouseCtrl) { m_mouseCtrl = mouseCtrl; }
    SPxMouseControl *GetMouseControl(void) const { return m_mouseCtrl; }

    void EnableMenu(int enable) { m_menuEnabled = enable; }
    void SetMenuPriority(unsigned int priority) { m_menuPriority = priority; }
    unsigned int GetMenuPriority(void) const { return m_menuPriority; }
    int IsMenuEnabled(void) const { return m_menuEnabled; }
    int HasContextMenu(void) const { return (m_ctxMenu != NULL); }
    SPxPopupMenuLater * GetContextMenu(void) { return m_ctxMenu; }

private:
    SPxObj *m_obj;
    SPxMouseControl *m_mouseCtrl;
    SPxPopupMenuLater *m_ctxMenu;
    int m_menuEnabled;
    unsigned int m_menuPriority;
 
    void SetContextMenu(SPxPopupMenuLater *menu) { m_ctxMenu = menu; }
    friend class SPxMouseControl;
    friend class SPxAVDisp;
};

/*
 * Define our class, derived from the SPx base object.
 */
class SPxMouseControl : public SPxObj, public SPxLock
{
public:
    /* Public functions. */

    /* Constructor and destructor. */
    explicit SPxMouseControl(SPxViewControl *viewCtrl=NULL);
    virtual ~SPxMouseControl(void);

    /* Renderers. */
    SPxErrorCode AddInterface(SPxMouseInterface *iface, int addToFront=FALSE);
    SPxErrorCode RemoveInterface(SPxMouseInterface *iface);
    SPxErrorCode EnableInterface(SPxMouseInterface *iface, int enabled);

    /* Enable / disable click sound. */
    void SetClickSoundEnabled(int state) { m_isClickSoundEnabled = state; }

    /* Enable / disable zoom on double click. */
    void SetZoomOnDoubleClick(int state) { m_zoomOnDoubleClick = state; }

    /* Enable / disable common right-click context menu. */
    void SetContextMenuEnabled(int state) { m_isContextMenuEnabled = state; }

    /* Set default cursor. */
    SPxErrorCode SetDefaultCursor(SPxCursor_t cursor);
    SPxCursor_t GetDefaultCursor(void) const { return m_defaultCursor; }

    /* Enable / disable view control. */
    void SetViewCtrlEnabled(int isEnabled);
    int IsViewCtrlEnabled(void) const;
    void SetViewCtrlPanEnabled(int isEnabled) {m_isViewCtrlPanEnabled = isEnabled;}
    int IsViewCtrlPanEnabled(void) const {return (m_viewCtrl && m_isViewCtrlPanEnabled);}
    void SetViewCtrlZoomEnabled(int isEnabled) {m_isViewCtrlZoomEnabled = isEnabled;}
    int IsViewCtrlZoomEnabled(void) const {return (m_viewCtrl && m_isViewCtrlZoomEnabled);}
    void SetViewCtrlZoomMode(SPxZoomMode_t zoomMode) { m_zoomMode = zoomMode; }
    SPxZoomMode_t GetViewCtrlZoomMode(void) const { return m_zoomMode; }
    void SetViewZoomRubberBandButton(UINT32 button) { m_viewZoomRBButton = button; }
    UINT32 GetViewZoomRubberBandButton(void) const { return m_viewZoomRBButton; }
    int IsViewZoomRubberBandInProgress(void) const { return m_isViewZoomRBInProgress; }
    void GetViewZoomRubberBandArea(int *x1, int *y1, int *x2, int *y2) const;

    /* Parameter setting/getting */
    virtual int SetParameter(char *name, char *value);

    virtual SPxErrorCode Enter(void) { return m_mutex.Enter(); }
    virtual SPxErrorCode Leave(void) { return m_mutex.Leave(); }

protected:
    /* Functions for handling mouse interaction. */
    int HandleMousePress(UINT32 flags, int x, int y);
    int HandleMouseRelease(UINT32 flags, int x, int y);
    int HandleDoubleClick(UINT32 flags, int x, int y);
    int HandleMouseMove(UINT32 flags, int x, int y);
    int HandleMouseLeave(void);
    int HandleMouseWheel(UINT32 flags, int x, int y, double delta);
    int HandleModKeyChange(UINT32 flags);
    virtual int HandleSetCursor(void)=0;
    SPxCursor_t GetSPxCursor(void) const { return m_cursor; }

private:
    /* Private types. */

    struct InterfaceInfo_t
    {
        InterfaceInfo_t *next;
	SPxMouseInterface *iface;
        int enabled;
    };

    /* Private variables. */

    /* View control. */
    SPxViewControl *m_viewCtrl;
    int m_zoomOnDoubleClick;            /* Allow zoom on double click. */

    /* Mouse. */
    int m_lastPosValid;			/* Last mouse position is valid? */
    int m_lastX;			/* Last x mouse position. */
    int m_lastY;			/* Last y mouse position. */
    int m_currPosValid;			/* Current mouse position is valid? */
    int m_currX;			/* Current x mouse position. */
    int m_currY;			/* Current y mouse position. */
    UINT32 m_mousePressLeftTime;	/* Ticker time of last mouse press. */
    InterfaceInfo_t *m_ifaces;		/* List of mouse interfaces. */
    SPxCursor_t m_cursor;
    int m_inHandler;			/* Already in handler? */
    int m_inShowMenu;			/* Already showing menu? */
    int m_isViewPanInProgress;          /* Is view being panned? */
    UINT32 m_viewZoomRBButton;		/* Button to trigger rubber-band zoom. */
    int m_isViewZoomRBInProgress;	/* Is view being zoomed with rubber band zoom? */
    int m_isViewCtrlPanEnabled;         /* View control pan enabled? */
    int m_isViewCtrlZoomEnabled;        /* View control zoom enabled? */
    SPxZoomMode_t m_zoomMode;		/* View control zoom mode. */
    
    /* Mutex. */
    SPxCriticalSection m_mutex;         /* Mutex to support multi-thread. */

    /* Play click when button pressed? */
    int m_isClickSoundEnabled;

    /* Show context menu generated by mouse interfaces. */
    int m_isContextMenuEnabled;

    /* Default cursor. */
    SPxCursor_t m_defaultCursor;

    /* Drag and drop. */
    int m_isDragDropInProgress;         /* Is drag drop in progress? */
    int m_dragStartX;                   /* Drag start X position. */
    int m_dragStartY;                   /* Drag start Y position. */
    SPxObj *m_dragObj;                  /* Object about to be/being dragged. */
    SPxMouseInterface *m_dragIface;     /* Interface for drag object. */
    SPxObj *m_dropObj;                  /* Object on to which dragged object may be dropped. */
    SPxMouseInterface *m_dropIface;     /* Interface for drop object. */

    /* Private functions. */
    virtual SPxPopupMenu *createPopupMenu(void) const=0;
    virtual int PostMousePress(UINT32 flags, int x, int y) { return HandleMousePress(flags, x, y); }
    virtual int PostMouseRelease(UINT32 flags, int x, int y) { return HandleMouseRelease(flags, x, y); }
    virtual int PostDoubleClick(UINT32 flags, int x, int y) { return HandleDoubleClick(flags, x, y); }
    virtual int PostMouseMove(UINT32 flags, int x, int y) { return HandleMouseMove(flags, x, y); }
    virtual int PostMouseLeave(void) { return HandleMouseLeave(); }
    virtual int PostMouseWheel(UINT32 flags, int x, int y, double delta) { return HandleMouseWheel(flags, x, y, delta); }
    int handleDragDropStart(UINT32 flags, int x, int y);
    int handleDragDropMove(UINT32 flags, int x, int y);
    int handleDragDropEnd(UINT32 flags, int x, int y);

    /* Private static functions. */

}; /* SPxMouseControl */

#endif /* _SPX_MOUSE_CONTROL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
