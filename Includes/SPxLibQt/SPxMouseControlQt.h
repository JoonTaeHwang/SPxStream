/*********************************************************************
*
* (c) Copyright 2015, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxMouseControlQt.h,v $
* ID: $Id: SPxMouseControlQt.h,v 1.3 2015/02/09 14:09:09 rew Exp $
*
* Purpose:
*   Header file for SPxMouseControlQt class.
*
* Revision Control:
*   09/02/15 v1.3    Warnings fixed.
*
* Previous Changes:
*   05/02/15 1.2    Set cursor appearence, keyboard modifiers and proper
*                    action to the mouse events.
*   26/01/15 1.1    JP Initial Version.
*
**********************************************************************/

#ifndef _SPX_MOUSE_CONTROL_QT_H
#define _SPX_MOUSE_CONTROL_QT_H

/*
 * Other headers required.
 */

/* For base class. */
#include "SPxLibUtils/SPxMouseControl.h"

/* Qt headers. */
#include <QObject>

/* Forward declare other classes required. */
class SPxViewControl;
class QMouseEvent;
class QWidget;
class QCursor;

/*********************************************************************
*
*   Class definitions
*
**********************************************************************/

/*
 * Define our class, derived from the SPx base object.
 */
class SPxMouseControlQt : private QObject, public SPxMouseControl
{
    Q_OBJECT

public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    explicit SPxMouseControlQt(SPxViewControl* viewCtrl=NULL);
    virtual ~SPxMouseControlQt(void);

    SPxErrorCode SetWindow(QWidget* widget);
    
private:
    /*
     * Private variables.
     */
    QWidget* m_widget;
    QCursor* m_cursorStd;
    QCursor* m_cursorImg;

    /*
     * Private functions.
     */
    virtual int HandleSetCursor(void);
    virtual SPxPopupMenu* createPopupMenu(void) const;

    bool eventFilter(QObject *obj, QEvent *qtEvent);

}; /* SPxMouseControlWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_MOUSE_CONTROL_QT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
