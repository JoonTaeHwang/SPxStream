/*********************************************************************
*
* (c) Copyright 2015 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPx Radar Simulator network source dialog.
*
*
* Revision Control:
*   25/06/18 v1.6    AGC	Correctly suppress cppcheck warnings.
*
* Previous Changes:
*   23/01/18 1.5    AGC	Support multiple objects.
*   03/03/16 1.4    AGC	Fix clang warnings.
*   27/04/15 1.3    AGC	Override OnCancel().
*			Move SetStandardAddress() implementation to source file.
*   24/04/15 1.2    REW	Add SetStandardAddress().
*   07/01/15 1.1    AGC	Initial version.
**********************************************************************/
#pragma once

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base class. */
#include "SPxLibWin/SPxTabbedPopupDlgWin.h"

/*
 * Types
 */

/* Forward declarations. */
class SPxNetworkReceive;
class SPxNetCtrlDlgWin;

/* SPxRadarSimSrcNetDlg dialog. */
class SPxSrcNetCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxSrcNetCtrlDlgWin)

public:
    /* Public functions. */
    explicit SPxSrcNetCtrlDlgWin(CWnd* pParent, SPxNetworkReceive *srcNet);
    explicit SPxSrcNetCtrlDlgWin(CWnd* pParent,
				 unsigned int numObjects,
				 SPxNetworkReceive *srcNet[]);
    virtual ~SPxSrcNetCtrlDlgWin(void);
    BOOL Create(void);

    // Dialog Data
    enum { IDD = IDD_SPX_SRC_NET_CTRL_DIALOG };

    /* Virtual functions defined by SPxTabbedItemDlg. */
    virtual void SyncFromObj(void);
    virtual int GetIDD(void) const { return IDD; }
    virtual CWnd *GetGroupBox(void) { return NULL; }
    virtual int ShowCheck(void) { return TRUE; }
    virtual void Enable(int enable);
    virtual int GetEnable(void);

    /* Convenience function. */
    void SetStandardAddress(const char *addr, int port);

    // cppcheck-suppress virtualCallInConstructor
    virtual void SetObjIndex(int index);

protected:
    /* Generated message map functions. */
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual void OnOK(void) {};
    virtual void OnCancel(void) {};
    virtual BOOL OnInitDialog(void);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    CWnd *m_parent;
    SPxNetworkReceive *m_obj;
    SPxAutoPtr<SPxNetCtrlDlgWin> m_netDlg;

    using SPxTabbedItemDlg::Create;

}; /* SPxSrcNetCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
