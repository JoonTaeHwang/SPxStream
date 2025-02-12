/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulated Radar control Doppler child dialog.
*
*
* Revision Control:
*   19/02/21 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibWin/SPxTabbedDlgWin.h"

#include "SPxLibWin/SPxWinSpinCtrl.h"

/* Forward declarations. */
class SPxSimRadar;

/*
 * SPxSimRadarDopplerCtrlDlgWin dialog
 */
class SPxSimRadarDopplerCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxSimRadarDopplerCtrlDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxSimRadarDopplerCtrlDlgWin(CWnd* pParent,
					  unsigned int numObjects,
					  SPxSimRadar *obj[]);
    virtual ~SPxSimRadarDopplerCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    void SetDisplayUnits(SPxUnitsSpeed_t unitsSpeed);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_RADAR_DOPPLER_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration retrieval functions. */
    SPxSimRadar *GetObj(void) { return m_obj; }
    // cppcheck-suppress virtualCallInConstructor
    virtual void SetObjIndex(int index);

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    CWnd *m_parentWin;
    SPxSimRadar *m_obj;
    SPxSimRadarNoise *m_noise;
    SPxUnitsSpeed_t m_unitsSpeed;

    /* Controls. */
    CStatic m_lblMinSpeed;
    CStatic m_lblMaxSpeed;
    SPxWinSpinCtrl m_spinMinSpeed;
    SPxWinSpinCtrl m_spinMaxSpeed;
    CButton m_radioPosTowards;
    CButton m_radioPosAway;
    CSliderCtrl m_sliderNoise;
    CStatic m_lblNoise;

    /* Private functions */
    void OnChangeMinSpeed(NMHDR*, LRESULT*);
    void OnChangeMaxSpeed(NMHDR*, LRESULT*);
    void OnChangeSign(void);

    using SPxTabbedItemDlg::Create;

}; /* class SPxSimRadarDopplerCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
