/*********************************************************************
 *
 * (c) Copyright 2012 - 2020, Cambridge Pixel Ltd.
 *
 *
 * Purpose:
 *  Header for soft button base class.
 *
 * Revision Control:
 *  15/10/20 v1.18   AGC	Add SetIsToggled().
 *
 * Previous Changes:
 *  01/10/20 1.17   AGC	Support toggle buttons.
 *  19/11/18 1.16   AGC	High DPI improvements.
 *  18/10/18 1.15   AGC	Improve handling of double-clicks.
 *  06/06/18 1.14   BTB	Make sure button always flashes.
 *  04/12/15 1.13   REW	Support mouseOver bitmaps and overlay text.
 *  02/12/15 1.12   REW	Support long clicks with optional handler.
 *  31/07/15 1.11   AGC	Support movable buttons.
 *  27/05/15 1.10   REW	Support SetSensitive().
 *  22/05/15 1.9    AGC	Support long clicks (display).
 *  20/02/15 1.8    AGC	Derive from SPxObj and replace Enable().
 *  16/02/15 1.7    AGC	Add HandleMouseLeave() function.
 *  09/02/15 1.6    AGC	Support pressed/released event.
 *  26/01/15 1.5    AGC	Remove SetDestination() declaration.
 *  15/10/13 1.4    SP 	Implement SPxMouseInterface functions.
 *  14/08/13 1.3    SP 	Pass brightness to paint(), not alpha.
 *  23/11/12 1.2    AGC	Make compatible with SPxMouseControl.
 *  09/11/12 1.1    AGC	Initial version.
 *
 **********************************************************************/
#ifndef _SPX_SOFT_BUTTON_H
#define _SPX_SOFT_BUTTON_H

/*
 * Other headers required.
 */

/* For common types etc. */
#include "SPxLibUtils/SPxCommon.h"

/* For SPxMouseInterface. */
#include "SPxLibUtils/SPxMouseControl.h"

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/*
 * Constants.
 */

/*
 * Types.
 */

/* Forward declarations. */
class SPxSoftButton;
class SPxRenderer;

/* Button clicked callback */
typedef void (*SPxSoftButtonClickedFn_t)(SPxSoftButton *bn, void *userData);
typedef void (*SPxSoftButtonPressedFn_t)(SPxSoftButton *bn, int isPressed, void *userData);
typedef void (*SPxSoftButtonMovedFn_t)(SPxSoftButton *bn, int x, int y, void *userData);

/* Define our class. */
class SPxSoftButton : public SPxObj, public SPxMouseInterface
{
public:
    /* Public functions */
    SPxSoftButton(void);
    virtual ~SPxSoftButton(void);
    SPxErrorCode Render(SPxRenderer *renderer=NULL);
    void SetClickedHandler(SPxSoftButtonClickedFn_t fn, void *userPtr);
    void SetLongClickedHandler(SPxSoftButtonClickedFn_t fn, void *userPtr);
    void SetPressedHandler(SPxSoftButtonPressedFn_t fn, void *userPtr);
    void SetMovedHandler(SPxSoftButtonMovedFn_t fn, void *userPtr);
    int HandleMousePress(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    int HandleMouseRelease(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    int HandleDoubleClick(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    int HandleMouseMove(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    int HandleMouseLeave(void);
    int HandleMouseOver(int x, int y, int *renderRtn);
    virtual void SetPos(int x, int y, int isDpiScaled=TRUE);
    int GetX(void) { return m_x + m_shiftX; }
    int GetY(void) { return m_y + m_shiftY; }
    int GetOrigX(void) { return m_x; }
    int GetOrigY(void) { return m_y; }
    int IsPosDpiScaled(void) { return m_isPosDpiScaled; }
    void SetHighlightEnabled(int state) { m_isHighlightEnabled = state; }
    void SetLongClickEnabled(int state) { m_isLongClickEnabled = state; }
    void SetToggleEnabled(int state)	{ m_isToggleEnabled = state; }
    void SetIsToggled(int state)	{ m_isToggled = state; }
    int GetIsToggled(void) const	{ return m_isToggled; }
    void SetSensitive(int sensitive)	{ m_isSensitive = sensitive; }
    int GetSensitive(void)		{ return(m_isSensitive); }
    void SetMovable(int movable)	{ m_isMovable = movable; }
    void SetMaxShift(double maxShift)	{ m_maxShift = maxShift; }

    /* Text to render. */
    void SetText(const char *text, int x, int y, UINT32 argb);
    void SetMouseOverText(const char *text, int x, int y, UINT32 argb);

protected:
    /* Protected functions. */
    virtual SPxErrorCode paint(double brightness) = 0;
    virtual int isMouseOver(int x, int y) = 0;

    int getIsCursorOver(void) const { return(m_isCursorOver); }

private:
    /* Private variables */
    int m_x;                        /* Top-left X */
    int m_y;                        /* Top-left Y */
    int m_isPosDpiScaled;	    /* Is position DPI scaled? */
    int m_pressX;		    /* Position mouse was pressed. */
    int m_pressY;
    int m_shiftX;		    /* Current X shift */
    int m_shiftY;		    /* Current Y shift */
    int m_isPressed;                /* Set to TRUE when pressed */
    int m_isCursorOver;             /* Set to TRUE when cursor over button */
    SPxSoftButtonClickedFn_t m_clickedFn; /* Button clicked handler */
    void *m_clickedFnUserPtr;       /* Button clicked handler user data */
    SPxSoftButtonClickedFn_t m_longClickedFn; /* Button long-clicked handler */
    void *m_longClickedFnUserPtr;   /* Button long-clicked handler user data */
    SPxSoftButtonPressedFn_t m_pressedFn; /* Button pressed handler */
    void *m_pressedFnUserPtr;       /* Button pressed handler user data */
    SPxSoftButtonMovedFn_t m_movedFn; /* Button moved handler */
    void *m_movedFnUserPtr;         /* Button moved handler user data */
    UINT32 m_lastPressedTime;       /* Timestamp when last pressed */
    int m_flashStartTime;	    /* Time in ms after press that button flash started*/
    int m_isHighlightEnabled;       /* Enable roll-over highlight. */
    int m_isLongClickEnabled;       /* Enable highlighting long clicks. */
    int m_isToggleEnabled;	    /* Behave like a toggle button, rather than a push button? */
    int m_isToggled;		    /* Is the button currently toggled (pressed down)? */
    int m_isSensitive;		    /* FALSE if insensitive */
    int m_isMovable;		    /* Is the soft button movable */
    double m_maxShift;		    /* Maximum shift for movable button */

    /* Overlay text. */
    char *m_text;		    /* Text to overlay, or NULL */
    int m_textX;		    /* Offset in X direction */
    int m_textY;		    /* Offset in Y direction */
    UINT32 m_textARGB;		    /* Text colour */
    char *m_moText;		    /* Text for mouseOver situation, if any */
    int m_moTextX;		    /* Offset in X direction */
    int m_moTextY;		    /* Offset in Y direction */
    UINT32 m_moTextARGB;	    /* Text colour */

}; /* SPxSoftButton */

#endif /* _SPX_SOFT_BUTTON_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
