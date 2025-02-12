/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the ADS-B monitor status child dialog.
*
*
* Revision Control:
*   04/01/21 v1.1    JGB Moved input source serial, type and version to SPxADSBIoDlgWin
*
* Previous Changes:
*   27/01/21 v1.0    JGB Initial Version
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* Forward declarations. */
class SPxADSBDecoder;

/*
 * SPxADSBStatusDlgWin dialog
 */
class SPxADSBStatusDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxADSBStatusDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxADSBStatusDlgWin(CWnd* pParent, SPxADSBDecoder *obj);
    virtual ~SPxADSBStatusDlgWin(void);
    virtual BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_ADSB_STATUS_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration retrieval functions. */
    SPxADSBDecoder *GetObj(void) { return m_obj; }

    void SetStandalone(int enabled) { m_standalone = enabled; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    virtual HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    DECLARE_MESSAGE_MAP()

private:
    /*
     * Private variables.
     */
    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxADSBDecoder *m_obj;

    /* MFC controls. */
    SPxWinSpinCtrl m_lostTimeoutSpin;
    SPxWinSpinCtrl m_autoDeleteTimeoutSpin;
    SPxWinSpinCtrl m_trailPointsSpin;
    SPxWinSpinCtrl m_trackHistorySpin;

    /* Colours for display. */
    COLORREF m_colGood;
    COLORREF m_colBad;
    COLORREF m_colUnknown;
    COLORREF m_colLastMsg;

    /* Other member variables. */
    BOOL m_standalone;

    /*
     * Private functions
     */
    void OnBnClickedDeleteAll(void);

    /* Avoid warnings about hiding overloaded virtual functions. */
    using CDialog::Create;
}; /* class SPxADSBStatusDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
