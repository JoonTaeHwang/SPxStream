/*********************************************************************
*
* (c) Copyright 2015 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulated Radar position control child dialog.
*
*
* Revision Control:
*   25/06/18 v1.5    AGC	Correctly suppress cppcheck warning.
*
* Previous Changes:
*   23/01/18 1.4    AGC	Support multiple objects.
*   31/01/17 1.3    AGC	Support units configuration.
*			Support platform name.
*   03/03/16 1.2    AGC	Fix clang warnings.
*   26/11/15 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxSimRadar;
class SPxSimPlatform;
struct SPxRadarSimEventParams;

/*
 * SPxSimRadarPositionCtrlDlgWin dialog
 */
class SPxSimRadarPositionCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxSimRadarPositionCtrlDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxSimRadarPositionCtrlDlgWin(CWnd* pParent,
					   unsigned int numObjects,
					   SPxSimRadar *obj[]);
    virtual ~SPxSimRadarPositionCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    void SetDisplayUnits(SPxUnitsDist_t unitsDist);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_RADAR_POSITION_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration retrieval functions. */
    SPxSimRadar *GetObj(void) { return m_obj; }
    // cppcheck-suppress virtualCallInConstructor
    virtual void SetObjIndex(int index);

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnTimer(UINT_PTR nIDEvent);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    CWnd *m_parentWin;
    SPxSimRadar *m_obj;
    SPxUnitsDist_t m_unitsDist;
    int m_platformNameChanged;

    CButton m_radioPlatRel;
    CButton m_radioStatic;
    CComboBox m_comboPlatform;
    CStatic m_lblPosA;
    CStatic m_lblPosB;
    CStatic m_lblPosHeight;
    CStatic m_lblPosHeightAbove;
    SPxWinSpinCtrl m_spinPosA;
    SPxWinSpinCtrl m_spinPosB;
    SPxWinSpinCtrl m_spinPosHeight;
    CButton m_radioHeightSea;
    CButton m_radioHeightGround;

    /* Private functions */
    void OnRadioPosPlatRel(void);
    void OnRadioPosStatic(void);
    void OnComboChangePlatform(void);
    void OnSpinChangePosA(NMHDR *, LRESULT *);
    void OnSpinChangePosB(NMHDR *, LRESULT *);
    void OnSpinChangePosHeight(NMHDR *, LRESULT *);
    void OnRadioHeightSea(void);
    void OnRadioHeightGround(void);
    SPxErrorCode eventHandler(UINT32 flags, const SPxRadarSimEventParams *params);
    static SPxErrorCode addPlatformToCombo(unsigned int index,
					   SPxSimPlatform *platform,
					   void *userArg);

    using SPxTabbedItemDlg::Create;

}; /* class SPxSimRadarPositionCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
