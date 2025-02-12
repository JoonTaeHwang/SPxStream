/*********************************************************************
*
* (c) Copyright 2010 - 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxPimCtrlDlgWin.h,v $
* ID: $Id: SPxPimCtrlDlgWin.h,v 1.7 2016/03/03 10:59:08 rew Exp $
*
* Purpose:
*	Header for PIM control dialog.
*
*
* Revision Control:
*   03/03/16 v1.7    AGC	Fix clang warnings.
*
* Previous Changes:
*   16/11/15 1.6    AGC	Place spin control to using resource file.
*   09/11/15 1.5    REW	Fix last change.
*   06/11/15 1.4    REW	Convert to re-usable tab-able dialog in library
*			from SPxServerWin.
*   08/02/12 1.3    AGC	Add Interference Suppression controls.
*   08/12/10 1.2    AGC	Remove spin control header as now part of library.
*   03/12/10 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Include the SPx resource header. */
#include "SPxResource.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* Include base class header. */
#include "SPxLibWin/SPxTabbedPopupDlgWin.h"

/* Forward declarations. */
class SPxPIM;

/*
 *  SPxPimCtrlDlgWin dialog
 */
class SPxPimCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxPimCtrlDlgWin)

public:
    /* Construction and destruction. */
    SPxPimCtrlDlgWin(CWnd* pParent, SPxPIM *pim);
    virtual ~SPxPimCtrlDlgWin();
    BOOL Create(void);

    /* Sync GUI to match object state. */
    virtual void SyncFromObj(void);

    /* Controls/options. */
    virtual int GetIDD(void) const { return IDD; }

    /* Dialog Data. */
    enum { IDD = IDD_SPX_PIM_CTRL_DIALOG };

protected:
    /* DDX/DDV support */
    virtual void DoDataExchange(CDataExchange* pDX);

    /*
     * Message handler functions.
     */
    virtual void OnOK(void){};
    virtual void OnCancel(void){};
    virtual BOOL OnInitDialog(void);
    afx_msg void OnCbnSelchangePimRangeModeCombo(void);
    afx_msg void OnCbnSelchangePimAziModeCombo(void);
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    afx_msg void OnCbnSelchangePimRefModeCombo(void);
    afx_msg void OnCbnSelchangePimIfLevelCombo();

    DECLARE_MESSAGE_MAP()

private:
    /* Parent window. */
    CWnd *m_parentWin;

    /* PIM to control. */
    SPxPIM *m_pim;

    /* Is the dialog initialised? */
    int m_init;

    /* MFC Controls. */
    CStatic m_aziLbl;
    CStatic m_samplesLbl;
    CComboBox m_ifLevelCombo;
    CComboBox m_rangeModeCombo;
    CComboBox m_aziModeCombo;
    CStatic m_aziOffsetLabel;
    SPxWinSpinCtrl m_aziOffsetSpin;
    CComboBox m_refModeCombo;

    using CDialog::Create;

}; /* SPxPimCtrlDlgWin class. */

/*********************************************************************
*
* End of file
*
**********************************************************************/
