/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the manual tracker sensor info control child dialog.
*
*
* Revision Control:
*   03/03/20 v1.1    BTB	Initial Version.
*
* Previous Changes:
**********************************************************************/

#pragma once

/* Library headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* Forward declarations. */
class SPxAsterixEncoder;

/*
 * SPxManualTrackerSensorInfoDlgWin class.
 */
class SPxManualTrackerSensorInfoDlgWin : public CDialog
{
public:
    /* Constructor and destructor. */
    SPxManualTrackerSensorInfoDlgWin(CWnd* pParent,
                                     SPxAsterixEncoder *trackEncoder);
    virtual ~SPxManualTrackerSensorInfoDlgWin();
    BOOL Create(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_MANUAL_TRACKER_SENSOR_INFO_DIALOG };

    /* Sync functions. */
    virtual void SyncGUI(void);

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    void OnShowWindow(BOOL bShow, UINT nStatus);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    afx_msg void OnButtonLatLongDMSRadio(void);
    afx_msg void OnButtonLatLongDecimalRadio(void);
    afx_msg void OnButtonLatNorthRadio(void);
    afx_msg void OnButtonLatSouthRadio(void);
    afx_msg void OnButtonLongEastRadio(void);
    afx_msg void OnButtonLongWestRadio(void);
    DECLARE_MESSAGE_MAP()

private:
    /*
     * Private members.
     */
    /* The parent window */
    CWnd *m_parentWin;

    /* Controls. */
    SPxWinSpinCtrl m_statusIntervalSpin;
    CButton m_dmsLatLongRadio;
    CButton m_decLatLongRadio;
    SPxWinSpinCtrl m_dmsLatDegsSpin;
    SPxWinSpinCtrl m_dmsLatMinsSpin;
    SPxWinSpinCtrl m_dmsLatSecsSpin;
    CButton m_dmsLatNorthRadio;
    CButton m_dmsLatSouthRadio;
    SPxWinSpinCtrl m_dmsLongDegsSpin;
    SPxWinSpinCtrl m_dmsLongMinsSpin;
    SPxWinSpinCtrl m_dmsLongSecsSpin;
    CButton m_dmsLongEastRadio;
    CButton m_dmsLongWestRadio;
    SPxWinSpinCtrl m_decLatSpin;
    SPxWinSpinCtrl m_decLongSpin;

    /* Encoder for creating ASTERIX messages. */
    SPxAsterixEncoder *m_trackEncoder;

    /*
     * Private functions
     */
    void syncPositionControls(void);
    void syncStatusControls(void);
    void updateSensorPosition(void);

    using CDialog::Create;

}; /* class SPxManualTrackerDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
