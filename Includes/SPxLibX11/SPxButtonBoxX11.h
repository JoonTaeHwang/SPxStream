/*********************************************************************
*
* (c) Copyright 2008 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	SPx GTK/Linux Application Framework. Button Class
*
* Revision Control:
*   22/01/20 v1.10   AGC	Use new SPxGtkIncludes header.
*
* Previous Changes:
*   07/01/20 1.9    AGC	Silence deprecated warnings.
*   06/08/18 1.8    AGC	Suppress gcc 8 warnings from GTK headers.
*   03/05/18 1.7    AGC	Support right-click events.
*   16/02/15 1.6    AGC	Use correct signature for button press callback.
*   18/07/14 1.5    AGC	Avoid warnings in glib headers.
*   04/07/14 1.4    AGC	Remove unused strings.
*   18/11/13 1.3    AGC	Fix clang warning.
*   13/02/12 1.2    AGC	Add AddButton() support.
*   09/12/11 1.1    AGC	Initial library version, moved from SPxFrameX.
*			Refactor to derive from SPxButtonBox.
**********************************************************************/
#ifndef _SPX_BUTTON_BOX_X11_H
#define _SPX_BUTTON_BOX_X11_H

/* Header for GTK+ headers. */
#include "SPxLibX11/SPxGtkIncludes.h"

/* Header for base class. */
#include "SPxLibUtils/SPxButtonBox.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/*
 * Implementation of a Button in a Button Box
 */
class SPxButtonBoxX11 : public SPxButtonBox
{
public:
    SPxButtonBoxX11(void);
    virtual ~SPxButtonBoxX11(void);
    
    /* Create the button box with a specified geometry */
    GtkWidget* Create(int w, int h);

    /* Legacy creation function. */
    GtkWidget* Create(int numButW, int numButH, int w, int h);

    SPxErrorCode Redraw(void) { return SPX_NO_ERROR; }

    GtkWidget* GetWidget(void) { return m_widget; }

private:
    /* Private variables. */
    GtkWidget *m_widget;

    /* Timer id. */
    unsigned int m_timerId;

    /* Private functions. */
    virtual SPxButtonBoxButton *CreateButton(void);
    static void SetColour(GdkColor *colour, UINT32 intColour);

    static gboolean timerFuncWrapper(gpointer data);
    void OnLButtonUp(SPxBBPosF pos) { LButtonUp(pos); }
    void OnLButtonDown(SPxBBPosF pos) { LButtonDown(pos); }
    void OnRButtonUp(SPxBBPosF pos) { RButtonUp(pos); }
    void OnRButtonDown(SPxBBPosF pos) { RButtonDown(pos); }

    friend class SPxButtonBoxButtonX11;

}; /* SPxButtonBoxX11 */

class SPxButtonBoxButtonX11 : public SPxButtonBoxButton
{
public:
    /* Public functions. */
    SPxButtonBoxButtonX11(SPxButtonBoxX11 *parent);
    virtual ~SPxButtonBoxButtonX11(void) {}

    GtkWidget *GetWidget(void) { return m_buttonWidget; }
    GtkWidget *GetLabelWidget(void) { return m_buttonLabelWidget; }

    virtual SPxButtonBoxButton* SetPosition(int x, int y, int w, int h);
    virtual SPxButtonBoxButton* SetLabel(const char *str);
    virtual SPxErrorCode Draw(void *arg) { return SPX_NO_ERROR; }

    virtual SPxButtonBoxButton* SetColourOn(UINT32 colour, int autoSetOthers=TRUE);
    virtual SPxButtonBoxButton* SetColourOff(UINT32 colour);
    virtual SPxButtonBoxButton* SetColourPreselect(UINT32 colour);

    virtual SPxButtonBoxButton* SetSensitive(int sensitive);

private:
    /* Private variables. */
    SPxButtonBoxX11 *m_x11Parent;
    GtkWidget *m_buttonWidget;
    GtkWidget *m_buttonLabelWidget;

    /* Private functions. */
    virtual void setStateInternal(int state);

    /* Button toggle/press callbacks. */
    static void toggleButtonWrapper(GtkToggleButton *toggle, gpointer userData);
    void toggleButton(GtkToggleButton *toggle, gpointer userData);
    static gboolean buttonPressWrapper(GtkWidget *widget, GdkEventButton *gdkEvent, gpointer userData);
    void buttonPress(GtkWidget *widget, GdkEventButton *gdkEvent, gpointer userData);
    static gboolean buttonReleaseWrapper(GtkWidget *widget, GdkEventButton *gdkEvent, gpointer userData);
    void buttonRelease(GtkWidget *widget, GdkEventButton *gdkEvent, gpointer userData);

    friend class SPxButtonBoxX11;

}; /* SPxButtonBoxButtonX11 */

#endif /* _SPX_BUTTON_BOX_X11_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
