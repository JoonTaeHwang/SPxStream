/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for the P437 control child dialog.
*
*
* Revision Control:
*   01/11/19 v1.1    AJH	Initial version.
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
class SPxNetworkReceiveP437;

/*
 * SPxP437CtrlDlgWin dialog
 */
class SPxP437CtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxP437CtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxP437CtrlDlgWin(CWnd* pParent, 
                      SPxNetworkReceiveP437 *obj);
    virtual ~SPxP437CtrlDlgWin();
    BOOL Create(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SRC_P437_CTRL_DIALOG };
    int GetIDD(void) const { return IDD; }

    /* Sync functions. */
    void SyncFromObj(void);
    void UpdateStatusVals(void);

    /* Configuration retrieval functions. */
    SPxNetworkReceiveP437 *GetObj(void) { return m_obj; }

    virtual HICON SetIcon(HICON icon, BOOL bigIcon);

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual void OnTimer(UINT_PTR nIDEvent);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    DECLARE_MESSAGE_MAP()

private:
    /* Private functions */

    void UpdateControlEnables(void);

    /*
     * Handler functions.
     */

    afx_msg void OnEnable(BOOL bEnable);
    afx_msg void OnRangeSet(void);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    afx_msg void OnTransmitting(void);
    afx_msg void OnGainAuto(void);
    afx_msg void OnSeaAuto(void);
    afx_msg void OnRainOn(void);

    /* GUI control variables. */
    unsigned int m_rangeScale;		/* Requested range scale */
    CButton m_transmitting;		/* Radar transmit on/off */
    CSliderCtrl m_gainSlider;		/* Gain */
    CButton m_gainAuto;			/* Auto-gain button */
    CSliderCtrl m_seaSlider;		/* Sea */
    CButton m_seaAuto;			/* Auto-sea button */
    CSliderCtrl m_rainSlider;		/* Rain */
    CButton m_rainOn;			/* Rain-on button */
    CSliderCtrl m_interfSlider;		/* Interference */

    /* General. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxNetworkReceiveP437 *m_obj;

    using SPxTabbedItemDlg::Create;

}; /* class SPxP437CtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
