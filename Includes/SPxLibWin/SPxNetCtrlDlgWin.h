/*********************************************************************
*
* (c) Copyright 2012 - 2019, 2021, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for a reuseable network control dialog class.
*
* Revision Control:
*   21/06/22 v1.22   AGC	Option to enable address and port when disabled.
*				Option to enable/disable TCP checkbox.
*
* Previous Changes:
*   20/05/21 1.21   AGC	Always show address for network receipt.
*   19/05/21 1.20   REW	Add TCP checkbox.
*   20/08/19 1.19   SP 	Rename SPxOutputMonitor to SPxOutputManager.
*   19/08/19 1.18   SP 	Support output monitor.
*   14/02/19 1.17   BTB	Support forcing display of the address control.
*   18/01/19 1.16   REW	Support button to use default addr/port.
*   21/12/18 1.15   REW	Support ChangeSetButton().
*   28/11/17 1.14   AGC	Respond to DPI changes.
*   12/04/17 1.13   AGC	Improve button update on port change.
*   31/10/16 1.12   AGC	Improve background updates.
*   13/09/16 1.11   AGC	Disable set button until changes made.
*   03/03/16 1.10   AGC	Fix clang warnings.
*   01/02/16 1.9    AGC	Improve background interface list updates.
*   22/01/16 1.8    AGC	Update interface list in background.
*   07/01/15 1.7    AGC	Rename dialog IDs.
*   22/11/13 1.6    AGC	Add SPxCommon header.
*   21/10/13 1.5    AGC	Use table layout.
*   15/10/13 1.4    AGC	Reposition controls if dialog is resized.
*   25/01/13 1.3    AGC	Control of interface address now optional.
*   31/08/12 1.2    AGC	Allow differences for send/recv.
*			Include "Any" interface for receivers.
*   21/08/12 1.1    AGC	Initial version.
*
**********************************************************************/

#pragma once

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

/* Standard headers. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* Include the SPx resource header */
#include "SPxResource.h"

/* Forward declarations. */
class SPxNetAddr;
class SPxTableLayoutWin;
class SPxOutputManager;

/*
 * SPxNetCtrlDlgWin dialog
 */
class SPxNetCtrlDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxNetCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxNetCtrlDlgWin(CWnd* pParent,
                     SPxNetAddr *netObj,
		     int netReceipt=TRUE,
		     int showIfAddr=TRUE);
    virtual ~SPxNetCtrlDlgWin();
    BOOL Create(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_NET_CTRL_DIALOG };

    void SetStandardAddress(const char *addr, int port, int labelOnly=FALSE);
    void ChangeSetButton(const char *text);
    void EnableAddrPort(int enable);
    void AllowTCP(int allow);
    SPxTableLayoutWin *GetLayout(void);
    void SetObj(SPxNetAddr *netObj);
    SPxNetAddr *GetObj(void);
    void SetOutputManager(SPxOutputManager *outputMgr);
    SPxOutputManager *GetOutputManager(void);

    /* Sync functions. */
    void SyncFromObj(int syncIfPendingChange=TRUE);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    void OnEnable(BOOL bEnable);
    void OnTimer(UINT_PTR nIDEvent);
    void OnDpiChangedAfterParent();
    virtual void OnSetFont(CFont *pFont);
    void OnCbnSelchangeSrcctrlNetIf();
    void OnCbnDropdownSrcctrlNetIf();
    void OnSetAddrPort();
    void OnUseDefaults();
    void OnAddrChange();
    void OnPortChange(NMHDR*, LRESULT*);
    void OnPortUpdate();
    void OnTcpClicked();
    DECLARE_MESSAGE_MAP()

private:
    /* Private functions */
    SPxErrorCode updateInterfaceAddresses(void);
    SPxErrorCode updateInterfaceCombo(void);
    SPxErrorCode syncCompleteSet(void);
    SPxErrorCode syncCompleteNoSet(void);
    SPxErrorCode syncComplete(int syncIfPendingChange);
    void updateWarning(void);
    void updateFont(void);
    void updateSetButton(int stateChanged=-1);

    using CDialog::Create;

    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* class SPxNetCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
