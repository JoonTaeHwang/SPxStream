/*********************************************************************
*
* (c) Copyright 2012 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Base class for any dialog that includes controls
*	used to interface with the radar.
*
* Revision Control:
*   08/06/20 v1.10   SP 	Support syncing of controls.
*
* Previous Changes:
*   03/03/16 1.9    AGC	Fix clang warnings.
*   26/03/14 1.8    SP 	Remove TEMP_EXCLUDE_SXV.
*   17/02/14 1.7    SP 	Add TEMP_EXCLUDE_SXV.
*   15/10/13 1.6    SP 	Add GetInterface().
*   27/09/13 1.5    AGC	Add forward declaration for CSliderCtrl.
*   06/06/13 1.4    SP 	Move GRACE_PERIOD_MS here and rename.
*   08/04/13 1.3    SP 	Add m_mesgDest.
*   22/10/12 1.2    SP 	Further development.
*   18/10/12 1.1    SP 	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* Include the classes we need. */
#include "SPxLibWin/SPxP172RcInterfaceWin.h"
#include "SPxLibWin/SPxP172RcTrafficDlgWin.h"

/*
 * Constants.
 */

/* Grace period in milliseconds. The grace period is started
 * after a change in a control or associated status value. During
 * this time an error may be pending due to a mismatch between the 
 * control and status value, in which case the status indicator 
 * is displayed with a grey background. If an error is still 
 * pending at the end of the grace period then an error is
 * displayed and an alarm is reported.
 *
 * Up until V1.55.1, this value was 3s. However some commands
 * sent to the radar can take several seconds to complete. 
 * Therefore KH suggested that this value is increased to 10s.
 */
#define SPX_P172_RC_GRACE_PERIOD_MS 10000

/* Forward declarations. */
class CSliderCtrl;

/*
 * SPxP172RcBaseDlgWin dialog.
 */
class SPxP172RcBaseDlgWin : public CDialog, public SPxObj
{
    DECLARE_DYNAMIC(SPxP172RcBaseDlgWin)

public:
    /* Constructor and destructor. */
    SPxP172RcBaseDlgWin(UINT nIDTemplate);
    virtual ~SPxP172RcBaseDlgWin();
    virtual BOOL Create(CWnd* pParent);
    virtual SPxP172RcInterfaceWin *GetInterface(void) { return m_interface; }

protected:
    DECLARE_MESSAGE_MAP()

    /* 
     * Protected types. 
     */

    /* The following structure is used to set and control
     * the behaviour of a Control and Status pair. This is a
     * control (combobox, checkbox, etc) that has an associated
     * status indicator (a text field). A mismatch between the 
     * control setting and the status value is highlighted by the 
     * colour of the status indicator. Control and Status pairs 
     * are held in a linked list so that they can be easily
     * processed.
     */
    typedef struct _Control_t
    {
        /* The status indicator handle. */
        CStatic *status;

        /* Combobox control and string value. */
        CComboBox *combo;
        char valueStr[SPX_P172_MAX_FIELD_LEN];

        /* Checkbox control and boolean value. */
        CButton *check;
        BOOL valueBool;

        /* Slider control and integer value. */
        CSliderCtrl *slider;
        CStatic *sliderText;
        int valueInt;

        /* Edit control and floating-point value. */
        CEdit *edit;
        double valueReal;

        /* Is control and status available? */
        BOOL isAvailable;

        /* Is status only, i.e. control disabled? */
        BOOL isReadOnly;

        /* Does the control currently show an error? */
        BOOL isError;

        /* Is there an error pending during grace period? */
        BOOL isErrorPending;

        /* Start of grace period timestamp. */
        UINT32 gracePeriodStartTimeMs;

        /* Has an alarm been reported? */
        BOOL isAlarmReported;

        /* If TRUE then the control was changed by the user. 
         * If FALSE then the status value associated with the
         * control was changed by the radar.
         */
        BOOL isControlChanged;

        /* Next in linked list. */
        _Control_t *nextPtr;

    } Control_t;

    /* Band selection.*/
    typedef enum
    {
        BAND_NOT_SET = 0,    /* Not a transceiver control. */
        BAND_X,              /* X-band transceiver control. */
        BAND_S               /* S-band transceiver control. */
    
    } Band_t;

    /* 
     * Protected variables. 
     */

    /* Colours to use for status indicators. */
    COLORREF m_statusOkColour;
    COLORREF m_statusErrorColour;
    COLORREF m_statusPendingColour;
    COLORREF m_statusDisabledColour;

    /* Brushes used to paint status indicators. */
    CBrush *m_statusOkBrush;
    CBrush *m_statusErrorBrush;
    CBrush *m_statusPendingBrush;
    CBrush *m_statusDisabledBrush;

    /* The interface to the radar. */
    SPxP172RcInterfaceWin *m_interface;

    /* Linked list of controls. */
    Control_t *m_ctrlListHead;

    /* Tranceiver band. */
    Band_t m_band;

    /* Message destination. */
    SPxP172RcTransportWin::MesgDest_t m_mesgDest;

    /* 
     * Protected functions. 
     */

    virtual void SetControlChanged(Control_t *ctrl);

    virtual BOOL UpdateControl(Control_t *ctrl, 
                               const SPxP172RcInterfaceWin::StatusField_t *field);

    virtual BOOL UpdateControl(Control_t *ctrl,
                               const SPxP172RcInterfaceWin::StatusField_t *field,
                               BOOL isError);

    virtual void SyncControl(Control_t *ctrl, 
                             const SPxP172RcInterfaceWin::StatusField_t *field);

    virtual afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:

    /* 
     * Private variables. 
     */

    UINT m_nIDTemplate;

    using CDialog::Create;

}; /* class SPxP172RcBaseDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
