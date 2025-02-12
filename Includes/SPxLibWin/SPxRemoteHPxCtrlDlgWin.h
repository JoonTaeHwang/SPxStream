/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for remote HPx control dialog.
*
*
* Revision Control:
*   02/12/20 v1.2    BTB	Show/hide channel B controls depending on card type.
*
* Previous Changes:
*   27/11/20 1.1    BTB	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header. */
#include "SPxResource.h"

/* For SPxRemoteHPxCtrl. */
#include "SPxLibNet/SPxRemoteHPxCtrl.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* Include base class header. */
#include "SPxLibWin/SPxTabbedPopupDlgWin.h"

/*
 * SPxRemoteHPxCtrlDlgWin dialog
 */
class SPxRemoteHPxCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxRemoteHPxCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxRemoteHPxCtrlDlgWin(CWnd* pParent, SPxRemoteHPxCtrl *hpxConn);
    virtual ~SPxRemoteHPxCtrlDlgWin(void);
    BOOL Create(void);

    virtual void SyncFromObj(void);

    virtual int GetIDD(void) const { return IDD; }

    /* Dialog Data */
    enum { IDD = IDD_SPX_REMOTE_HPX_CTRL_DIALOG };

protected:
    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){};
    virtual void OnCancel(void){};

    /* GUI callback functions. */
    virtual void OnTimer(UINT_PTR nIDEvent);

private:
    /*
     * Private variables.
     */

    /* Parent window. */
    CWnd *m_parentWin;

    /* Remote control connection. */
    SPxRemoteHPxCtrl *m_hpxConn;

    /* Whether to sync controls to the HPx state on updating. */
    int m_hpxSyncRequired;

    /* Whether or not we have a channel B on the card. */
    int m_haveChannelB;

    /* Amount to move controls when channel B controls disappear/appear. */
    int m_channelBAdjust;

    /* Child controls. */
    SPxWinSpinCtrl m_gainASpin;
    SPxWinSpinCtrl m_offsetASpin;
    SPxWinSpinCtrl m_gainBSpin;
    SPxWinSpinCtrl m_offsetBSpin;
    SPxWinSpinCtrl m_rangeStartSpin;
    SPxWinSpinCtrl m_rangeEndSpin;
    SPxWinSpinCtrl m_rangeCorrectionSpin;
    CStatic m_gainAStatus;
    CStatic m_offsetAStatus;
    CStatic m_gainBStatus;
    CStatic m_offsetBStatus;
    CStatic m_rangeStartStatus;
    CStatic m_rangeEndStatus;
    CStatic m_rangeCorrectionStatus;
    CStatic m_gainBLabel;
    CStatic m_offsetBLabel;
    CStatic m_rangeStartLabel;
    CStatic m_rangeEndLabel;
    CStatic m_rangeCorrectionLabel;
    CButton m_chanBGroup;
    CButton m_rangeGroup;

    /*
     * Private functions.
     */

     /* General. */
    void syncToHPxState(void);
    void syncGUI(void);
    void setHaveChannelB(int haveChannelB);

    /* GUI callback functions. */
    afx_msg LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam); 

    using CDialog::Create;

};  /* class SPxRemoteHPxCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
