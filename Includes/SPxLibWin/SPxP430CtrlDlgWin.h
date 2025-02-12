/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for the P430 control child dialog.
*
*
* Revision Control:
*   13/06/19 v1.1    AJH	Initial version.
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
class SPxNetworkReceiveP430;

/*
 * SPxP430CtrlDlgWin dialog
 */
class SPxP430CtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxP430CtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxP430CtrlDlgWin(CWnd* pParent, 
                      SPxNetworkReceiveP430 *obj);
    virtual ~SPxP430CtrlDlgWin();
    BOOL Create(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SRC_P430_CTRL_DIALOG };
    int GetIDD(void) const { return IDD; }

    /* Sync functions. */
    void SyncFromObj(void);
    void UpdateStatusVals(void);

    /* Configuration retrieval functions. */
    SPxNetworkReceiveP430 *GetObj(void) { return m_obj; }

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
    afx_msg void OnSeaClutterAuto(void);
    afx_msg void OnRainAuto(void);
    afx_msg void OnSector(void);
    afx_msg void OnSectorSet(void);

    /* GUI control variables. */
    unsigned int m_rangeScale;		/* Requested range scale */
    CButton m_transmitting;		/* Radar transmit on/off */
    CComboBox m_comboSectorID;		/* Blanking sector ID */
    CSliderCtrl m_gainSlider;		/* Gain */
    CButton m_gainAuto;			/* Auto-gain button */
    CSliderCtrl m_seaClutterSlider;	/* Sea clutter */
    CButton m_seaClutterAuto;		/* Auto-seaclutter button */
    CSliderCtrl m_rainSlider;		/* Rain */
    CButton m_rainAuto;			/* Auto-rain button */
    unsigned int m_sector;		/* Currently selected sector */
    unsigned int m_sectorStart;		/* Blanking sector start angle */
    unsigned int m_sectorWidth;		/* Blanking sector width */

    /* General. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxNetworkReceiveP430 *m_obj;

    using SPxTabbedItemDlg::Create;

}; /* class SPxP430CtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
