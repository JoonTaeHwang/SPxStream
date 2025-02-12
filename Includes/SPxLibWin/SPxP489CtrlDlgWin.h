/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for the P489 control child dialog.
*
*
* Revision Control:
*   05/01/21 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxNetworkReceiveP489;

/*
 * SPxP408CtrlDlgWin dialog
 */
class SPxP489CtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxP489CtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxP489CtrlDlgWin(CWnd* pParent,
                      SPxNetworkReceiveP489 *obj);
    virtual ~SPxP489CtrlDlgWin();
    BOOL Create(void);

    void SetProToRibRaw(SPxRunProcess *proToRibRaw);
    void SetProToRibProc(SPxRunProcess *proToRibProc);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SRC_P489_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Sync functions. */
    virtual void SyncFromObj(void);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK(void) { /* Don't call the CDialog handler */ };
    virtual void OnCancel(void) { /* Don't call the CDialog handler */ };
    DECLARE_MESSAGE_MAP()

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    void OnEndRangeChange(NMHDR*, LRESULT*);
    void OnGainChange(NMHDR*, LRESULT*);
    void OnPeriodMultChange(NMHDR*, LRESULT*);

    using SPxTabbedItemDlg::Create;

}; /* class SPxP489CtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
