/*********************************************************************
*
* (c) Copyright 2009 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header file for a Button Box for GUI control.
*
* Revision Control:
*   24/10/18 v1.25   SP 	Sync label flashing of buttons in the same box.
*
* Previous Changes:
*   10/10/18 1.24   REW	Support showing antenna angles in buttons.
*   16/05/18 1.23   REW	Support SetButtonOnWidth().
*   10/05/18 1.22   REW	Add SetExplicitState().
*   03/05/18 1.21   AGC	Support right-click events.
*   01/05/18 1.20   REW	Add ClearRadio().
*   24/01/18 1.19   SP 	Support label flashing.
*   17/01/18 1.18   SP 	Add SetAllSensitive().
*   22/06/17 1.17   AGC	Allow insensitive pen to be set per-button.
*   08/04/16 1.16   AGC	Add const user data overload for SetHandler().
*   13/03/15 1.15   REW	Add allowNone to SetRadio().
*   04/07/14 1.14   AGC	Move strings to source file.
*   20/06/14 1.13   AGC	Move C++ headers to source file.
*   20/02/14 1.12   REW	Add SetAllInsensitive().
*   17/02/14 1.11   SP	Redraw when button sensitivity changed.
*			Add SetInactiveAvailable().
*   21/01/14 1.10   SP 	Support multi-state toggle.
*   15/01/14 1.9    SP 	Make GetParent() public.
*   02/12/13 1.8    SP 	Add SetClickSoundEnabled().
*   18/11/13 1.7    AGC	Fix clang warning.
*   17/09/13 1.6    SP 	Add m_redrawEnabled flag to fix #261.
*   14/08/13 1.5    SP 	Add GetButtonByLabel().
*                       Add GetButtonByValue().
*                       Add GetButtonNoLock() to fix mutex locking issue.
*                       Add SetLabelOff();
*   01/08/13 1.4    SP 	Support square buttons.
*                       Allow pens to set be per-button.
*                       Allow highlight to be disabled.
*                       Support button box resizing.
*                       Support centre and left label alignment.
*   14/03/12 1.3    AGC	Make SetVisible() virtual.
*   13/02/12 1.2    AGC	Re-add AddButton() support.
*   09/12/11 1.1    AGC	Initial Version based on SPxLibWin file v1.3
**********************************************************************/

#ifndef _SPX_BUTTON_BOX_H
#define _SPX_BUTTON_BOX_H

/* For common definitions */
#include "SPxLibUtils/SPxCommon.h"

/* For SPxAutoPtr class. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxCriticalSection class. */
#include "SPxLibUtils/SPxCriticalSection.h"

/* For SPxImage. */
#include "SPxLibUtils/SPxImage.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Type of button */
#define SPX_BB_BUTTON_TYPE_PUSH	          (0)
#define SPX_BB_BUTTON_TYPE_TOGGLE         (1)
#define SPX_BB_BUTTON_TYPE_TOGGLE_3STATES (2)
#define SPX_BB_BUTTON_TYPE_TOGGLE_4STATES (3)


/* State of button. */
#define SPX_BB_BUTTON_STATE_INACTIVE    (0)
#define SPX_BB_BUTTON_STATE_ACTIVE_1    (1)
#define SPX_BB_BUTTON_STATE_ACTIVE_2    (2)
#define SPX_BB_BUTTON_STATE_ACTIVE_3    (3)

/* For backward compatibility. */
#define SPX_BB_BUTTON_STATE_ACTIVE      SPX_BB_BUTTON_STATE_ACTIVE_1

/* Maximum number of states. */
#define SPX_BB_BUTTON_MAX_NUM_STATES 4

/* Label alignment */
#define SPX_BB_BUTTON_LABEL_ALIGN_LEFT     (0)
#define SPX_BB_BUTTON_LABEL_ALIGN_CENTRE   (1)

/* Flash states. */
#define SPX_BB_BUTTON_FLASH_OFF                 (0)
#define SPX_BB_BUTTON_FLASH_STATE               (1)
#define SPX_BB_BUTTON_FLASH_LABEL               (2)
#define SPX_BB_BUTTON_FLASH_LABEL_WHEN_ACTIVE   (3)

/* Default timer interval. */
#define SPX_BB_TIMER_INTERVAL_MS    (250)

/* Maximum number of antenna angles that can be shown. */
#define	SPX_BB_BUTTON_MAX_ANGLES	4

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxButtonBoxButton;

class SPxBBPosF
{
public:
    SPxBBPosF(float x=0, float y=0)
	: X(x), Y(y) {}
    float X;
    float Y;
};

enum SPxBBButtons
{
    SPX_BB_LEFT_BUTTON = 0x1,
    SPX_BB_RIGHT_BUTTON = 0x2
};

/* Button box button handler function type. */
typedef void (*SPxButtonBoxButtonHandler)(SPxButtonBoxButton *button, void *userData);
typedef void (*SPxButtonBoxButtonHandlerConst)(SPxButtonBoxButton *button, const void *userData);

/* Button box class. */
class SPxButtonBox : public SPxObj
{
public:
    SPxButtonBox(void);
    virtual ~SPxButtonBox(void);

    /* Create a set of grid buttons */
    SPxErrorCode CreateGridButtons(int numButW, int numButH);

    virtual SPxErrorCode Resize(int w, int h);

    /* Set a region of buttons to have radio behaviour. */
    SPxErrorCode SetRadio(int startX, int startY, int endX, int endY, 
	SPxButtonBoxButtonHandler fn = NULL, 
	void *userData = NULL, int state = TRUE,
	int allowNone = FALSE);
    SPxErrorCode RadioBlock(int startX, int startY, int endX, int endY,
	SPxButtonBoxButtonHandler fn = NULL,
	void *userData = NULL, int state = TRUE)
    { return SetRadio(startX, startY, endX, endY, fn, userData, state); }

    /* Clear a region of buttons to remove radio behaviour (also clears
     * from other buttons in the same groups but outside the region).
     */
    SPxErrorCode ClearRadio(int startX, int startY, int endX, int endY);

    /* Set spacing for buttons. */
    void SetGridSpacing(int spacingX, int spacingY);

    /* Return the number of buttons. */
    int GetNumButtons(void);

    /* Get the first button in the linked list. */
    SPxButtonBoxButton *GetFirstButton(void);

    /* Get the next button in the linked list. */
    SPxButtonBoxButton *GetNextButton(SPxButtonBoxButton *button);

    /* Set the font to be used for the buttons. */
    SPxErrorCode SetFont(const char *family, int size);

    /* Enable or disable rounded corners. */
    SPxErrorCode SetCornerRadius(int pixels);

    /* Enable or disable roll-over highlight. */
    SPxErrorCode SetHighlightEnabled(int state);

    /* Set the colours. These assignments for default colours must occur
     * after the constructor and before the Create() call is made.
     */
    void SetButtonTextColour(UINT32 textColour) { SetColourLabelOn(textColour); }
    void SetColourLabelOn(UINT32 colour) { m_colourLabelOn = colour; }
    void SetColourLabelOff(UINT32 colour) { m_colourLabelOff = colour; }
    void SetBackgroundColour(UINT32 backColour) {m_backgroundColour = backColour;}
    void SetButtonOnColour(UINT32 buttonOnColour) {m_buttonOnColour = buttonOnColour;}
    void SetButtonOffColour(UINT32 buttonOffColour) {m_buttonOffColour = buttonOffColour;}
    void SetButtonInsensitiveColour(UINT32 insensitiveColour) {m_buttonInsensitiveColour = insensitiveColour;}
    void SetButtonPreselectColour(UINT32 preselectColour) {m_buttonPreselectColour = preselectColour;}
    void SetButtonOffPenColour(UINT32 buttonOffPen) {m_buttonOffPenColour = buttonOffPen;}
    void SetButtonOnPenColour(UINT32 buttonOnPen) {m_buttonOnPenColour = buttonOnPen;}
    void SetButtonOnPenWidth(int width) {m_buttonOnPenWidth = width;}
    void SetButtonHighlightPenColour(UINT32 highlightPen) {m_buttonHighlightPenColour = highlightPen;}
    void SetButtonInsensitivePenColour(UINT32 insensitivePen) {m_buttonInsensitivePenColour = insensitivePen;}
    void SetBackgroundPenColour(UINT32 backPen) {m_backPenColour = backPen;}

    /* Return the button at a specified grid location (if buttons were created
     * with CreateGridButtons api.
     */
    SPxButtonBoxButton *GetButton(int x, int y);
    SPxButtonBoxButton *GetButtonAt(int x, int y) { return GetButton(x, y); }
    SPxButtonBoxButton *GetButtonByLabel(const char *label);
    SPxButtonBoxButton *GetButtonByValue(UINT32 value);
    
    /* Add a new previously created button. */
    void AddButton(SPxButtonBoxButton *newButton);

    SPxButtonBoxButton *GetButtonSelectPending(void) { return m_selectPending; }

    /* Get dimensions. */
    int GetWidth(void) { return m_w; }
    int GetHeight(void) { return m_h; }

    /* Change visibility state */
    void SetVisible(int state);
    int GetVisible(void) { return m_visible; }

    /* Set all child buttons sensitive/insensitive in one go. */
    void SetAllSensitive(int isSensitive);
    void SetAllInsensitive(void);

    /* Enable / disable click sound. */
    void SetClickSoundEnabled(int state) { m_isClickSoundEnabled = state; }

    virtual SPxErrorCode Redraw(void)=0;

protected:
    /* Protected functions. */
    SPxErrorCode Create(int w, int h);

    const char *GetButtonFontFamily(void);
    int GetButtonFontSize(void);

    UINT32 GetButtonTextColour(void) { return GetColourLabelOn(); }
    UINT32 GetColourLabelOn(void) { return m_colourLabelOn; }
    UINT32 GetColourLabelOff(void) { return m_colourLabelOff; }
    UINT32 GetBackgroundColour(void) { return m_backgroundColour; }
    UINT32 GetButtonOnColour(void) { return m_buttonOnColour; }
    UINT32 GetButtonOffColour(void) { return m_buttonOffColour; }
    UINT32 GetButtonInsensitiveColour(void) { return m_buttonInsensitiveColour; }
    UINT32 GetButtonPreselectColour(void) { return m_buttonPreselectColour; }
    UINT32 GetButtonOffPenColour(void) { return m_buttonOffPenColour; }
    UINT32 GetButtonOnPenColour(void) { return m_buttonOnPenColour; }
    int GetButtonOnPenWidth(void) { return m_buttonOnPenWidth; }
    UINT32 GetButtonHighlightPenColour(void) { return m_buttonHighlightPenColour; }
    UINT32 GetButtonInsensitivePenColour(void) { return m_buttonInsensitivePenColour; }
    UINT32 GetBackgroundPenColour(void) { return m_backPenColour; }
    int GetCornerRadius(void) { return m_cornerRadius; }

    SPxErrorCode MouseMove(int buttons, SPxBBPosF point);
    SPxErrorCode LButtonUp(SPxBBPosF point);
    SPxErrorCode LButtonDown(SPxBBPosF point);
    SPxErrorCode RButtonUp(SPxBBPosF point);
    SPxErrorCode RButtonDown(SPxBBPosF point);
    SPxErrorCode TimerFunc(void);
    int IsRedrawEnabled(void) { return m_isRedrawEnabled; }

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxCriticalSection m_mutex;			/* Mutex protection. */
    int m_flashOn;

    /* State */
    int m_visible;

    /* Geometry of button box */
    int m_w, m_h;

    SPxButtonBoxButton *m_lastHighlightButton;

    int m_counter;
    int m_spacingX, m_spacingY;
    int m_cornerRadius;
    int m_highlightEnabled;

    /* Play click when button pressed? */
    int m_isClickSoundEnabled;

    /* Grid size if initialised with CreateGridButtons() */
    int m_numGridButtonsAcross;
    int m_numGridButtonsDown;

    /* Which child button has a select pending? */
    SPxButtonBoxButton* m_selectPending;

    /* Which child button has right-click select pending? */
    SPxButtonBoxButton* m_selectPendingRight;

    /* Flag used internally to enable/disable redraws. */
    int m_isRedrawEnabled;

    /* Colours */
    UINT32 m_backgroundColour;
    UINT32 m_buttonOnColour;
    UINT32 m_buttonOffColour;
    UINT32 m_buttonInsensitiveColour;
    UINT32 m_buttonPreselectColour;
    UINT32 m_buttonOffPenColour, m_buttonOnPenColour;
    UINT32 m_buttonHighlightPenColour;
    UINT32 m_buttonInsensitivePenColour;
    UINT32 m_backPenColour;
    UINT32 m_colourLabelOn;
    UINT32 m_colourLabelOff;

    /* Widths. */
    int m_buttonOnPenWidth;

    /* Private functions. */
    SPxButtonBoxButton *FindButtonForPoint(SPxBBPosF point);
    SPxErrorCode ResizeGridButtons(void);
    SPxButtonBoxButton *GetButtonNoLock(int x, int y);
    virtual SPxButtonBoxButton *CreateButton(void)=0;

    friend class SPxButtonBoxButton;

}; /* SPxButtonBox */

/* Class for an individual button in the button box. */
class SPxButtonBoxButton
{
public:
    /* Set the geometry of the button. */
    virtual SPxButtonBoxButton* SetPosition(int x, int y, int w, int h) 
			{m_x = x; m_y = y; m_w = w; m_h = h; return this;}
    int GetX(void) { return m_x; }
    int GetY(void) { return m_y; }
    int GetWidth(void) { return m_w; }
    int GetHeight(void) { return m_h; }

    /* Set colours */
    SPxButtonBoxButton* SetColour(UINT32 colour, int autoSetOthers=TRUE) { return SetColourOn(colour, autoSetOthers); }
    virtual SPxButtonBoxButton* SetColourOn(UINT32 colour, int autoSetOthers=TRUE);
    virtual SPxButtonBoxButton* SetColourOff(UINT32 colour);
    virtual SPxButtonBoxButton* SetColourLabelOn(UINT32 colour);
    virtual SPxButtonBoxButton* SetColourLabelOff(UINT32 colour);
    virtual SPxButtonBoxButton* SetColourPreselect(UINT32 colour);
    virtual SPxButtonBoxButton* SetPenColourOn(UINT32 colour, int autoSetOthers=TRUE);
    virtual SPxButtonBoxButton* SetPenColourOff(UINT32 colour);
    virtual SPxButtonBoxButton* SetPenColourPreselect(UINT32 colour);
    virtual SPxButtonBoxButton* SetPenColourHighlight(UINT32 colour);
    virtual SPxButtonBoxButton* SetPenColourInsensitive(UINT32 colour);

    /* Get/Set state */
    int GetState(void) { return m_state; }
    SPxButtonBoxButton* SetState(int state, int invokeHandler=FALSE);
    SPxButtonBoxButton* SetExplicitState(int state);
    int GetActive(void) { return GetState(); }
    SPxButtonBoxButton* SetActive(int active) { return SetState(active, TRUE); }
    void SetInactiveAvailable(int isAvail) { m_isInactiveAvail = isAvail; }
    int IsInactiveAvailable(void) { return m_isInactiveAvail; }

    /* Set the grid reference */
    SPxButtonBoxButton* SetGridRef(int x, int y) { m_gridX = x; m_gridY = y; return this; }

    /* Get/Set visibility */
    int GetVisible(void) {return m_visible;}
    virtual SPxButtonBoxButton* SetVisible(int state) {m_visible = state; return this;}

    /* Clear radio group. */
    SPxErrorCode ClearRadioGroup(void);

    /* Get the next button in the chain. */
    SPxButtonBoxButton* GetNext(void) { m_parent->GetNextButton(this); return this; }

    SPxButtonBoxButton* GetNextRadio(void) { return m_radioNext; }

    /* Get/Set flash state. */
    SPxButtonBoxButton* SetFlashState(int state) {m_flashState = state; return this;}
    int GetFlashState(void) {return m_flashState;}
    SPxButtonBoxButton* SetFlashFrequency(int freq) {m_flashFrequency = freq; return this;}
    int GetFlashFrequency(void) {return m_flashFrequency;}

    SPxButtonBoxButton* SetUserData(void *userData) {m_userData = userData ; return this;}
    void *GetUserData(void) { return m_userData; }
    SPxButtonBoxButton* SetClientData(void *userData) { return SetUserData(userData); }
    void *GetClientData(void) { return GetUserData(); }
    SPxButtonBoxButton* SetGridPosition(int x, int y) {m_gridX = x, m_gridY = y; return this;}
    virtual SPxButtonBoxButton* SetLabelForState(int state, const char *str);
    virtual SPxButtonBoxButton* SetLabel(const char *str);
    virtual SPxButtonBoxButton* SetLabelOff(const char *str) { return SetLabelForState(SPX_BB_BUTTON_STATE_INACTIVE, str); }
    const char *GetLabelForState(int state); 
    const char *GetLabel(void) { return GetLabelForState(SPX_BB_BUTTON_STATE_ACTIVE); }
    const char *GetLabelOff(void) { return GetLabelForState(SPX_BB_BUTTON_STATE_INACTIVE); }
    const char *GetLabelString(void) { return GetLabel(); }
    SPxButtonBoxButton* SetLabelMarkup(const char *str) { return SetLabel(str); }
    SPxButtonBoxButton* SetHandler(SPxButtonBoxButtonHandler fn, void* userData = 0, int leftClick = TRUE);
    SPxButtonBoxButton* SetHandler(SPxButtonBoxButtonHandlerConst fn, const void* userData, int leftClick = TRUE);
    SPxButtonBoxButton* SetLabelXAlignment(int alignment) { m_labelXAlignment = alignment; return this;}
    SPxButtonBoxButton* SetLabelVisible(int isVisible) { m_labelVisible = isVisible; return this; }
    int GetLabelVisible(void) { return m_labelVisible; }
    SPxButtonBoxButton* SetValue(UINT32 val) {m_value = val ; return this;}
    UINT32 GetValue(void) { return m_value; }

    /* Get/Set type */
    int GetType(void) {return m_type;}
    SPxButtonBoxButton* SetType(int type) {m_type = type; return this;}

    /* Associate an integer with the state of the button. The integer gets
     * polled by the button box and the state of the button is either on or
     * off depending on the value (non zero or zero) of the variable.
     */
    SPxButtonBoxButton* AssociateVar (int *var) {m_associatedVar = var; return this;}

    /* Activate the button. */
    void Activate(int left = TRUE);

    /* Get/Set sensitivity state. */
    int GetSensitive(void) {return m_sensitive;}
    virtual SPxButtonBoxButton* SetSensitive(int sensitive);

    int GetGridX(void) { return m_gridX; }
    int GetGridY(void) { return m_gridY; }

    virtual SPxErrorCode Draw(void *arg)=0;

    /* Get parent button box. */
    virtual SPxButtonBox *GetParent(void) { return m_parent; }

    /* Antenna status support (negative angle disables that display). */
    SPxErrorCode SetAntennaAngle(double degs, unsigned int idx=0);
    SPxErrorCode SetAntennaAngles(double degs0, double degs1=-1.0,
				double degs2=-1.0, double degs3=-1.0);
    double GetAntennaAngle(unsigned int idx=0) const;

protected:
    SPxButtonBoxButton(SPxButtonBox* parent);
    virtual ~SPxButtonBoxButton(void);
    virtual void setStateInternal(int state) { m_state = state; }
    int GetLabelXAlignment(void) { return m_labelXAlignment; }

    UINT32 GetColourOn(void) { return m_colourOn; }
    UINT32 GetColourOff(void) { return m_colourOff; }
    UINT32 GetColourPreselect(void) { return m_colourPreselect; }
    UINT32 GetPenColourOn(void) { return m_penColourOn; }
    UINT32 GetPenColourOff(void) { return m_penColourOff; }
    UINT32 GetPenColourPreselect(void) { return m_penColourPreselect; }
    UINT32 GetPenColourHighlight(void) { return m_penColourHighlight; }
    UINT32 GetPenColourInsensitive(void) { return m_penColourInsensitive; }
    UINT32 GetColourLabelOn(void) { return m_textColourOn; }
    UINT32 GetColourLabelOff(void) { return m_textColourOff; }
    int GetHighlightOnPointer(void) { return m_highlightOnPointer; }
    int GetTextColourSet(void) { return m_textColourSet; }
    int GetColourOnSet(void) { return m_colourOnSet; }
    int GetColourOffSet(void) { return m_colourOffSet; }
    int GetColourPreselectSet(void) { return m_colourPreselectSet; }
    int GetPenColourOnSet(void) { return m_penColourOnSet; }
    int GetPenColourOffSet(void) { return m_penColourOffSet; }
    int GetPenColourPreselectSet(void) { return m_penColourPreselectSet; }
    int GetPenColourHighlightSet(void) { return m_penColourHighlightSet; }
    int GetPenColourInsensitiveSet(void) { return m_penColourInsensitiveSet; }

private:
    /* State context. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private variables. */
    SPxButtonBox *m_parent;		/* Our parent button box */
    
    /* Geometry of button. */
    int m_x, m_y, m_w, m_h;
    int m_highlightOnPointer;
    int m_visible;			/* Visible at all ? */
    int m_state;			/* SPX_BB_BUTTON_STATE* */
    int m_type;				/* SPX_BB_BUTTON_TYPE_PUSH = Push, SPX_BB_BUTTON_TYPE_TOGGLE = toggle */
    int m_flashState;			/* SPX_BB_BUTTON_FLASH_STATE* */
    int m_sensitive;			/* Can be changed through GUI ? */
    int m_flashFrequency;		/* Flashing frequency (1 = interval of timer, 
					2 = half interval etc) */
    SPxButtonBoxButtonHandler m_handler;    /* The event handler for activation */
    void *m_userData;			/* User data for evant handler. */
    SPxButtonBoxButtonHandlerConst m_handlerConst;    /* The event handler for activation */
    const void *m_userDataConst;	/* Const user data for event handler. */
    SPxButtonBoxButtonHandler m_handlerRight;    /* The right-click event handler */
    void *m_userDataRight;		/* User data for right-click event handler. */
    SPxButtonBoxButtonHandlerConst m_handlerRightConst;    /* The right-click event handler */
    const void *m_userDataRightConst;	/* Const user data for right-click event handler. */
    UINT32 m_value;			/* Value assigned to button. */
    int m_isInactiveAvail;	/* Is the inactive toggle state available? */
    SPxButtonBoxButton *m_radioNext;	/* If radio, this points to next button. */
    int m_radioAllowNone;		/* If radio, can we deselect all? */
    UINT32 m_colourOn;			/* Button ON colour. */
    UINT32 m_colourOff;			/* Button OFF colour. */
    UINT32 m_colourPreselect;		/* Button PRESELECT colour. */
    UINT32 m_penColourOn;		/* Button ON pen colour. */
    UINT32 m_penColourOff;		/* Button OFF pen colour. */
    UINT32 m_penColourPreselect;	/* Button PRESELECT pen colour. */
    UINT32 m_penColourHighlight;	/* Button HIGHLIGHT pen colour. */
    UINT32 m_penColourInsensitive;	/* Button INSENSITIVE pen colour. */
    UINT32 m_textColourOn;		/* Text ON colour */
    UINT32 m_textColourOff;		/* Text OFF colour */
    int m_textColourSet;		/* Is there a colour set for this button ? */
    int m_colourOnSet;
    int m_colourOffSet;
    int m_colourPreselectSet;
    int m_penColourOnSet;
    int m_penColourOffSet;
    int m_penColourPreselectSet;
    int m_penColourHighlightSet;
    int m_penColourInsensitiveSet;

    int *m_associatedVar;		/* An associated variable whose boolean state
					is mirrored by the button's toggle state. */
    int m_gridX;			/* Grid location (x,y) */
    int m_gridY;
    int m_labelXAlignment;
    int m_labelVisible;

    /* Antenna status support. */
    double m_antennaAngles[SPX_BB_BUTTON_MAX_ANGLES];

    /* Private functions. */
    void ToggleState(void);
    void SetSelectPending(int left = TRUE);
    int EnclosesPoint(SPxBBPosF point);
    void SetHighlightForPointer(int state) {m_highlightOnPointer = state;}
    
    friend class SPxButtonBox;

}; /* SPxButtonBoxButton */

#endif /* _SPX_BUTTON_BOX_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
