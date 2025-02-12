/*********************************************************************
*
* (c) Copyright 2018, 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for a dialog for configuring an Onvif client.
*
*
* Revision Control:
*   06/02/19 v1.3    AGC	Always disable controls when updating.
*
* Previous Changes:
*   14/01/19 1.2    AGC	Improve unauthorised indication.
*   02/07/18 1.1    AGC	Initial version.
**********************************************************************/
#pragma once

/* Standard headers. */
#define SPX_AFXWIN
#include "SPxResource.h"
#include "SPxLibUtils/SPxCommon.h"

/* For SPxErrorCode */
#include "SPxLibUtils/SPxError.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* Forward declarations. */
class SPxCamCtrlOnvif;
class SPxTableLayoutWin;

/*
 * SPxOnvifCtrlDlgWin dialog
 */
class SPxOnvifCtrlDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxOnvifCtrlDlgWin)
public:
    /* Constructor and destructor. */
    explicit SPxOnvifCtrlDlgWin(CWnd* pParent, SPxCamCtrlOnvif *camCtrl);
    virtual ~SPxOnvifCtrlDlgWin(void);
    BOOL Create(void);

    void SetObj(SPxCamCtrlOnvif *camCtrl);

    virtual void SyncFromObj(void);
    virtual int GetIDD(void) const { return IDD; }
    virtual SPxTableLayoutWin *GetLayout(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_ONVIF_CTRL_DIALOG };

protected:
    void DoDataExchange(CDataExchange *pDX);
    int OnInitDialog(void);
    void OnSize(UINT nType, int cx, int cy);
    void OnEnable(BOOL bEnable);
    HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
    void OnTimer(UINT_PTR nIDEvent);
    void OnDestroy(void);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    void OnDiscover(void);
    void OnTextChange(NMHDR *nmhdr, LRESULT*);
    void OnComboServer(void);
    void OnComboProfile(void);
    SPxErrorCode discover(void);
    SPxErrorCode discoverComplete(void);
    SPxErrorCode getProfiles(void);
    SPxErrorCode getProfilesComplete(void);
    void updateServers(void);
    void updateProfiles(void);
    void setStatus(void);

    using CDialog::Create;

}; /* SPxAVSrcRtspDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
