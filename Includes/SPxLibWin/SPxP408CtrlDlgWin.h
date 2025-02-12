/*********************************************************************
*
* (c) Copyright 2018, 2020, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for the P408 control child dialog.
*
*
* Revision Control:
*   17/03/20 v1.2    AJH    Fix typo.
*
* Previous Changes:
*   19/10/18 1.1    AGC	Initial Version.
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxNetworkReceiveP408;

/*
 * SPxP408CtrlDlgWin dialog
 */
class SPxP408CtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxP408CtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxP408CtrlDlgWin(CWnd* pParent,
                      SPxNetworkReceiveP408 *obj);
    virtual ~SPxP408CtrlDlgWin();
    BOOL Create(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SRC_P408_CTRL_DIALOG };
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

    void OnAziResChange(void);
    void OnFrameRateChange(NMHDR*, LRESULT*);
    void OnMaxRangeChange(NMHDR*, LRESULT*);
    void OnMaxVelocityChange(NMHDR*, LRESULT*);
    void OnClutterChange(void);
    void OnCfarThresholdChange(NMHDR*, LRESULT*);
    void OnDopplerThresholdChange(NMHDR*, LRESULT*);

    SPxErrorCode setAziRes(void);
    SPxErrorCode setFrameRate(void);
    SPxErrorCode setMaxRange(void);
    SPxErrorCode setMaxVelocity(void);
    SPxErrorCode setClutter(void);
    SPxErrorCode setCfarThreshold(void);
    SPxErrorCode setDopplerThreshold(void);
    SPxErrorCode setAziResComplete(void);
    SPxErrorCode setFrameRateComplete(void);
    SPxErrorCode setMaxRangeComplete(void);
    SPxErrorCode setMaxVelocityComplete(void);
    SPxErrorCode setClutterComplete(void);
    SPxErrorCode setCfarThresholdComplete(void);
    SPxErrorCode setDopplerThresholdComplete(void);

    using SPxTabbedItemDlg::Create;

}; /* class SPxP408CtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
