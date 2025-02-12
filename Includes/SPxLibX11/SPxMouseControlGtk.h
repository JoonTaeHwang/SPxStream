/*********************************************************************
*
* (c) Copyright 2012, 2014, 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxMouseControlGtk.h,v $
* ID: $Id: SPxMouseControlGtk.h,v 1.3 2017/01/11 14:37:40 rew Exp $
*
* Purpose:
*   Header file for SPxMouseControlGtk class.
*
*
* Revision Control:
*   11/01/17 v1.3    AGC	Support sub-menus.
*
* Previous Changes:
*   16/05/14 1.2    AGC	Support automatic context menus.
*   23/11/12 1.1    AGC	Initial Version.
*
**********************************************************************/

#ifndef _SPX_MOUSE_CONTROL_GTK_H
#define _SPX_MOUSE_CONTROL_GTK_H

/*
 * Other headers required.
 */

/* For base class. */
#include "SPxLibUtils/SPxMouseControl.h"

/* Forward declare other classes required. */
class SPxViewControl;
struct _GtkWidget;
struct _GdkCursor;
struct _GdkEventButton;
struct _GdkEventMotion;
struct _GdkEventCrossing;
struct _GdkEventScroll;

/*********************************************************************
*
*   Class definitions
*
**********************************************************************/

/*
 * Define our class, derived from the SPx base object.
 */
class SPxMouseControlGtk : public SPxMouseControl
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    explicit SPxMouseControlGtk(SPxViewControl *viewCtrl=NULL);
    virtual ~SPxMouseControlGtk(void);

    SPxErrorCode SetWindow(_GtkWidget *widget);
    
private:
    /*
     * Private variables.
     */
    _GtkWidget *m_widget;
    _GdkCursor *m_cursorView;
    _GdkCursor *m_cursorSelect;
    int m_buttonPressSig;
    int m_buttonReleaseSig;
    int m_motionNotifySig;
    int m_leaveNotifySig;
    int m_scrollSig;
    int m_destroySig;

    /*
     * Private functions.
     */
    virtual int HandleSetCursor(void);
    virtual SPxPopupMenu *createPopupMenu(void) const;
    static int buttonPress(_GtkWidget *widget, _GdkEventButton *event, void *user_data);
    static int buttonRelease(_GtkWidget *widget, _GdkEventButton *event, void *user_data);
    static int motionNotify(_GtkWidget *widget, _GdkEventMotion *event, void *user_data);
    static int leaveNotify(_GtkWidget *widget, _GdkEventCrossing *event, void *user_data);
    static int scroll(_GtkWidget *widget, _GdkEventScroll *event, void *user_data);
    static int destroy(_GtkWidget *widget, void *user_data);

}; /* SPxMouseControlWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_MOUSE_CONTROL_GTK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
