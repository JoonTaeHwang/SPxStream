/*********************************************************************
 *
 * (c) Copyright 2013 - 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *  Header for alert buttons class. This class uses SPxSoftButton
 *  to display one or more buttons to altert an operator's attention
 *  to one or more events. Clicking a button hides it to confirm that
 *  the event has been noted.
 *
 * Revision Control:
 *  14/08/18 v1.11   SP 	Support alert background clicked.
 *
 * Previous Changes:
 *  22/06/18 1.10   AGC	Fix new cppcheck warning.
 *  15/02/18 1.9    AGC	Revert last change.
 *  02/02/18 1.8    AGC	Support alert removed callback.
 *  25/01/18 1.7    SP 	Add optional redraw flag to clearAlert().
 *  09/01/18 1.6    SP 	Support flashing.
 *  17/07/15 1.5    SP 	Support alert user data.
 *  16/02/15 1.4    AGC	Avoid deleting soft button that caused mouse event.
 *  19/11/14 1.3    REW	Allow alerts to be changed, removed etc.
 *  10/03/14 1.2    AGC	Rename Alert_t to SPxAlert_t.
 *  04/11/13 1.1    SP 	Initial version.
 **********************************************************************/
#ifndef _SPX_ALERT_RENDERER_H
#define _SPX_ALERT_RENDERER_H

/*
 * Other headers required.
 */

/* Library headers. */
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxMouseControl.h"
#include "SPxLibUtils/SPxRenderer.h"


/*
 * Constants.
 */

/*
 * Types.
 */

/* Forward-declare other classes we use. */
struct SPxAlert_t;
class SPxSoftButton;

/* Define our class. */
class SPxAlertRenderer : virtual public SPxRenderer
{
public:

    /* 
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxAlertRenderer(void);
    virtual ~SPxAlertRenderer(void);

    /* Configuration. */
    virtual void SetPositionPixels(int x, int y);
    virtual int GetXPixels(void) { return m_x; }
    virtual int GetYPixels(void) { return m_y; }
    virtual void SetMaxHeightPixels(unsigned int pixels) { m_maxHeightPixels = pixels; }
    virtual unsigned int GetMaxHeightPixels(void) { return m_maxHeightPixels; }
    virtual void SetMaxNumAlerts(unsigned int num) { m_maxNumAlerts = num; }
    virtual unsigned int GetMaxNumAlerts(void) { return m_maxNumAlerts; }
    virtual void SetFillColour(UINT32 argb) { m_fillColour = argb; }
    virtual UINT32 GetFillColour(void) { return m_fillColour; }
    virtual void SetOutlineColour(UINT32 argb) { m_outlineColour = argb; }
    virtual UINT32 GetOutlineColour(void) { return m_outlineColour; }
    virtual void SetTextColour(UINT32 argb) { m_textColour = argb; }
    virtual UINT32 GetTextColour(void) { return m_textColour; }
    virtual unsigned int GetNumAlerts(void);
    void ClearAllAlerts(void);

    /* Mouse handling. */
    virtual int HandleMousePress(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseRelease(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseMove(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseLeave(void);

protected:

    /* 
     * Protected functions. 
     */

    /* Add/change/remove alerts. */
    SPxErrorCode AddAlert(SPxSoftButton *button, 
                          const char *text,
                          const char *prefix=NULL,
                          UINT32 prefixColour=0,
                          SPxAlert_t **alertPtr=NULL,
                          int insertAtStart=FALSE,
                          int pinAtStart=FALSE);
    SPxErrorCode UpdateAlert(SPxAlert_t *alert, const char *text);
    SPxErrorCode RemoveAlert(SPxAlert_t *alert);

    /* Function that derived classes can override if they want to handle
     * the operator clicking on the confirm button.
     */
    virtual void HandleOperatorConfirm(SPxAlert_t *alert);

    /* Function that derived classes can override if they want to know when
     * alerts are being cleared (for whatever reason).
     */
    virtual void AlertIsBeingCleared(const SPxAlert_t *alert);

    /* Function that derived classes can override if they want to handle
     * the operator clicking on the alert itself (not the confirm button).
     */
    virtual void HandleAlertBackgroundClicked(const SPxAlert_t *alert);

    /* User data. */
    virtual SPxErrorCode  SetAlertUserData( SPxAlert_t *alert, void *data);
    virtual void *GetAlertUserData(const SPxAlert_t *alert);
    virtual SPxAlert_t *FindAlertWithUserData(void *data);

    /* Flashing control. */
    virtual SPxErrorCode SetAlertFlash(SPxAlert_t *alert, int isEnabled);
    virtual int IsAlertFlash(const SPxAlert_t *alert);

    /* Implements a function in the base class. */
    virtual SPxErrorCode renderAll(void);

    /* 
     * Protected variables. 
     */

    /* Allow alert background to be clicked? */
    int m_allowAlertBackgroundClick;

private:

    /* 
     * Private functions.
     */

    void paintAlert(SPxAlert_t *alert, int x, int y, int yMax);
    int positionAlert(SPxAlert_t *alert, int x, int y, int yMax);
    void clearAlert(SPxAlert_t *alert, int deleteAlert=FALSE, int redraw=TRUE);
    unsigned int getNumAlerts(void);
    SPxAlert_t *getAlertFromXY(int x, int y);

    /* 
     * Private static functions.
     */

    static void confirmButtonClicked(SPxSoftButton *button,
                                     void *userData);

    /* 
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;           /* Private implementation. */
    int m_x;                        /* Top-left X */
    int m_y;                        /* Top-left Y */
    UINT32 m_fillColour;
    UINT32 m_outlineColour;
    UINT32 m_textColour;
    unsigned int m_maxHeightPixels; /* Max vertical height for display. */
    unsigned int m_maxNumAlerts;    /* Max number of alerts to hold. */
    int m_flashState;               /* Current flash state (on or off). */
    UINT32 m_lastFlashTime;         /* Time flash state changed. */

}; /* SPxAlertRenderer */

#endif /* _SPX_ALERT_RENDERER_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
