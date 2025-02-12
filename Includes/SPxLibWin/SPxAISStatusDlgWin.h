/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the AIS monitor status child dialog.
*
*
* Revision Control:
*   22/09/20 v1.4    AJH	Add support for standalone.
*
* Previous Changes:
*   18/09/20 1.3    AJH	Fix headers.
*   09/09/20 1.2    AJH	Add support for setting timeouts/interpolation.
*   07/09/20 1.1    AJH	Initial Version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* Forward declarations. */
class SPxAISDecoder;

/*
 * SPxAISStatusDlgWin dialog
 */
class SPxAISStatusDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxAISStatusDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxAISStatusDlgWin(CWnd* pParent, SPxAISDecoder *obj);
    virtual ~SPxAISStatusDlgWin(void);
    virtual BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_AIS_STATUS_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration retrieval functions. */
    SPxAISDecoder *GetObj(void) { return m_obj; }

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
    SPxAISDecoder *m_obj;

    /* MFC controls. */
    SPxWinSpinCtrl m_lostTimeoutSpin;
    SPxWinSpinCtrl m_autoDeleteTimeoutSpin;
    SPxWinSpinCtrl m_statusTimeoutSpin;
    SPxWinSpinCtrl m_extrapolatePeriodSpin;

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
}; /* class SPxAISStatusDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
