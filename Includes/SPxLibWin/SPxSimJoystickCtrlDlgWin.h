/*********************************************************************
*
* (c) Copyright 2015 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxSimJoystickCtrlDlgWin.h,v $
* ID: $Id: SPxSimJoystickCtrlDlgWin.h,v 1.4 2017/01/31 14:46:17 rew Exp $
*
* Purpose:
*	Header for dialog used to configure the joystick control.
*
* Revision Control:
*   31/01/17 v1.4    AGC	Support units configuration.
*
* Previous Changes:
*   07/10/16 1.3    AGC	Support 2D/3D control.
*   03/03/16 1.2    AGC	Fix clang warnings.
*   22/10/15 1.1    AGC	Initial version.
**********************************************************************/
#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibWin/SPxWinSpinCtrl.h"

/* Forward declarations. */
class SPxJoystickSim;

class SPxSimJoystickCtrlDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxSimJoystickCtrlDlgWin)

public:
    /* Public functions. */
    explicit SPxSimJoystickCtrlDlgWin(CWnd* pParent);
    virtual ~SPxSimJoystickCtrlDlgWin(void);
    BOOL Create(void);
    void SyncFromObj(void);

    void SetJoystick(SPxJoystickSim *joystick);

    void SetDisplayUnits(SPxUnitsSpeed_t unitsSpeed);

    // Dialog Data
    enum { IDD = IDD_SPX_SIM_JOYSTICK_CTRL_DIALOG };

protected:
    /* Protected functions. */
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    virtual void OnTimer(UINT_PTR nIDEvent);
    virtual void OnOK(void) {};
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    CWnd *m_parent;
    SPxJoystickSim *m_joystick;
    SPxUnitsSpeed_t m_unitsSpeed;

    /* Controls. */
    CComboBox m_comboSource;
    CComboBox m_comboAxis;
    CStatic m_labelTurnRange;
    SPxWinSpinCtrl m_spinMinTurn;
    SPxWinSpinCtrl m_spinMaxTurn;
    CSliderCtrl m_sliderTurn;
    CStatic m_labelTurn;
    CButton m_checkTurn;
    CStatic m_labelMinVrate;
    CStatic m_labelMaxVrate;
    SPxWinSpinCtrl m_spinMinVrate;
    SPxWinSpinCtrl m_spinMaxVrate;
    CSliderCtrl m_sliderVrate;
    CStatic m_labelVrate;
    CButton m_checkVrate;
    CStatic m_labelMinSpeed;
    CStatic m_labelMaxSpeed;
    CStatic m_labelMinAccel;
    CStatic m_labelMaxAccel;
    CComboBox m_comboType;
    SPxWinSpinCtrl m_spinMinSpeed;
    SPxWinSpinCtrl m_spinMaxSpeed;
    SPxWinSpinCtrl m_spinMinAccel;
    SPxWinSpinCtrl m_spinMaxAccel;
    CSliderCtrl m_sliderSpeed;
    CStatic m_labelSpeed;
    CButton m_checkSpeed;

    /* Private functions. */
    void OnSourceChange(void);
    void OnAxisChange(void);
    void OnCheckTurn(void);
    void OnCheckVrate(void);
    void OnTypeChange(void);
    void OnCheckSpeed(void);
    void OnSpinMinTurn(NMHDR*, LRESULT*);
    void OnSpinMaxTurn(NMHDR*, LRESULT*);
    void OnSpinMinVrate(NMHDR*, LRESULT*);
    void OnSpinMaxVrate(NMHDR*, LRESULT*);
    void OnSpinMinSpeed(NMHDR*, LRESULT*);
    void OnSpinMaxSpeed(NMHDR*, LRESULT*);
    void OnSpinMinAccel(NMHDR*, LRESULT*);
    void OnSpinMaxAccel(NMHDR*, LRESULT*);

    static SPxErrorCode addJoystickToCombo(void *userArg, const char *name, 
	const void *dev);

    using CDialog::Create;

}; /* SPxSimJoystickCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
