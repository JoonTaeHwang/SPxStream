/*********************************************************************
*
* (c) Copyright 2015 Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxButtonBoxQt.h,v $
* ID: $Id: SPxButtonBoxQt.h,v 1.5 2015/02/09 14:08:23 rew Exp $
*
* Purpose:
*	Header file for a Button Box for GUI control.
*
* Revision Control:
*   09/02/15 v1.5    JP     Warnings fixed.
*
* Previous Changes:
*   05/02/15 1.4    JP     Add option to set font and size of the button's text.
*                          Correct some warnings.
*   26/01/15 1.3    JP 	Implementation of qt events.
*   13/01/15 1.2    JP 	Implementation of colours (background, labels, frames).
*   07/01/15 1.1    JP 	Initial version.
**********************************************************************/
#ifndef _SPX_BUTTON_BOX_QT_H
#define _SPX_BUTTON_BOX_QT_H

/* Qt headers. */
#include <QWidget>
#include <QPushButton>

/* Header for base class. */
#include "SPxLibUtils/SPxButtonBox.h"

/* SPx headers. */
#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxError.h"

/* Qt forward declarations. */
class QGridLayout;
class QStyleOption;
class QPainter;
class QMouseEvent;

/* Our forward declarations. */
class SPxQtStyle;
class SPxButtonBoxButtonQt;

/* Declaration of the SPxButtonBoxQt class. */
class SPxButtonBoxQt : protected QWidget, public SPxButtonBox
{
    Q_OBJECT

public:
    /*
     * Public funtions.
     */

    /* Constructor. */
    explicit SPxButtonBoxQt(QLayout *parentLayout);

    /* Destructor. */
    virtual ~SPxButtonBoxQt();

    /* Create the buttons. */
    SPxErrorCode CreateGridButtons(int nHButtons, int nVButtons);

    /* Set the font to be used for the buttons. */
    SPxErrorCode SetFont(const char *family, int textSize);

    /*
     * Public variables.
     */

private:
    /*
     * Private funtions.
     */

    /* Private constructor. */
    SPxButtonBoxQt();

    /* Avoid objs copies of this class. */
    SPxButtonBoxQt(const SPxButtonBoxQt& buttonBox);
    SPxButtonBoxQt& operator=( const SPxButtonBoxQt& buttonBox);

    /* Redraw the button box. */
    SPxErrorCode Redraw(void);

    /* Creator of SPxButtonBoxButtonQt objects. */
    SPxButtonBoxButton* CreateButton(void);

    /* Override of the paint event. */
    void paintEvent(QPaintEvent* paintEvent);

    /* Override of the mouse move event. */
    void mouseMoveEvent(QMouseEvent* mouseEvent);

    /* Override of the mouse press button event. */
    void mousePressEvent(QMouseEvent* mouseEvent);

    /* Override of the mouse release button event. */
    void mouseReleaseEvent(QMouseEvent* mouseEvent);

    /*
     * Private variables.
     */

    /* Timer. */
    QTimer* m_timer;

    /* Qt style. */
    SPxQtStyle* m_buttonBoxStyle;

    /* Friend classes. */
    friend class SPxButtonBoxButtonQt;

private slots:
    void TimerSlot();

}; /* SPxButtonBoxButtonQt */

/* Declaration of the SPxButtonBoxButtonQt class. */
class SPxButtonBoxButtonQt : protected QPushButton, public SPxButtonBoxButton
{
	Q_OBJECT

public:
    /*
     * Public funtions.
     */

     /* Constructor. */
    explicit SPxButtonBoxButtonQt(SPxButtonBoxQt *buttonBox);

    /* Destructor. */
    virtual ~SPxButtonBoxButtonQt();

    /* Set button label. */
    SPxButtonBoxButton* SetLabelForState(int state, const char *str);

    /* Set colour on. */
    SPxButtonBoxButton* SetColourOn(UINT32 colour, int autoSetOthers=TRUE);
    
    /* Set colour off. */
    SPxButtonBoxButton* SetColourOff(UINT32 colour);

    /* Set visibility. */
    SPxButtonBoxButton* SetVisible(int state);

    /*
     * Public variables.
     */

private:

    /*
     * Private funtions.
     */
    /* Avoid objs copies of objs of this class. */
    SPxButtonBoxButtonQt(const SPxButtonBoxButtonQt& buttonBoxbutton);
    SPxButtonBoxButtonQt& operator=( const SPxButtonBoxButtonQt& buttonBoxbutton);

    /* Draw button. */
    SPxErrorCode Draw(void *arg);

    /* Set button position. */
    SPxButtonBoxButton* SetPosition(int xPos, int yPos, int w, int h);

    /*
     * Private variables.
     */

    /* Button box pointer*/
    SPxButtonBoxQt* m_qtParent;

    /* Qt style. */
    SPxQtStyle* m_buttonStyle;

    /* Friend classes. */
    friend class SPxButtonBoxQt;

};

#endif /* _SPX_BUTTON_BOX_QT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
