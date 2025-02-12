/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for the P435 control child dialog.
*
*
* Revision Control:
*   15/07/19 v1.2    AGC	Simplify address configuration.
*				Add frequency band control.
*
* Previous Changes:
*   25/06/19 1.1    AGC	Initial Version.
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxNetworkReceiveP435;

/*
 * SPxP435CtrlDlgWin dialog
 */
class SPxP435CtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxP435CtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxP435CtrlDlgWin(CWnd* pParent,
                      SPxNetworkReceiveP435 *obj);
    virtual ~SPxP435CtrlDlgWin();
    BOOL Create(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SRC_P435_CTRL_DIALOG };
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

    void OnWaveformChange(void);
    void OnNarrowFreqSpin(NMHDR*, LRESULT*);
    void OnWideFreqSpin(NMHDR*, LRESULT*);

    using SPxTabbedItemDlg::Create;

}; /* class SPxP435CtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
