/*********************************************************************
*
* (c) Copyright 2021, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulated Weather control child dialog.
*
*
* Revision Control:
*   09/09/22 v1.2   AGC	Improve behaviour when dialog hidden.
*
* Previous Changes:
*   02/03/21 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/* For SPxObjPtr. */
#include "SPxLibUtils/SPxObjPtr.h"

/* For SPxSimWeather. */
#include "SPxLibData/SPxSimWeather.h"

/* For SPxWinButton. */
#include "SPxLibWin/SPxWinButton.h"

/* For SPxWinEditCtrl. */
#include "SPxLibWin/SPxWinEditCtrl.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* Forward declarations. */
class SPxRadarSimServer;
class SPxRadarSimulator;
class SPxSimMotion;
class SPxSimWeather;
class SPxWeatherRenderer;
class SPxAreaEditorMouse;
class SPxViewControl;

/*
 * SPxSimWeatherCtrlDlgWin dialog
 */
class SPxSimWeatherCtrlDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxSimWeatherCtrlDlgWin)

public:
    typedef void (*ShowMotion)(SPxSimMotion *motion, void *userArg);

    /* Constructor and destructor. */
    explicit SPxSimWeatherCtrlDlgWin(CWnd* pParent,
				     SPxRadarSimServer *server,
				     SPxWeatherRenderer *renderer=NULL,
				     SPxAreaEditorMouse *editor=NULL);
    virtual ~SPxSimWeatherCtrlDlgWin(void);
    BOOL Create(void);
    void SyncFromObj(int updateAll=TRUE, int updatePointList=FALSE);
    SPxErrorCode SetCurrWeather(SPxSimWeather *weather);
    void AddWeather(void);
    int IsRepaintDue(void);

    void SetShowMotionFn(ShowMotion fn, void *userArg);
    
    void SetDisplayUnits(SPxUnitsDist_t unitsDist,
			 SPxUnitsSpeed_t unitsSpeed);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_WEATHER_CTRL_DIALOG };

    /* Configuration retrieval functions. */
    SPxRadarSimulator *GetObj(void) { return m_obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    void OnTimer(UINT_PTR nIDEvent);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    void OnShowWindow(BOOL bShow, UINT nStatus);
    virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
    BOOL OnNeedToolTip(UINT id, NMHDR *pTTTStruct, LRESULT *pResult);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* The parent window */
    CWnd *m_parentWin;

    /* Callback variables. */
    ShowMotion m_showMotionFn;
    void *m_showMotionUserArg;

    /* Units */
    SPxUnitsDist_t m_unitsDist;
    SPxUnitsSpeed_t m_unitsSpeed;

    /* The object we are controlling */
    SPxRadarSimServer *m_server;
    SPxRadarSimulator *m_obj;
    SPxObjPtr<SPxSimWeather> m_currWeather;
    SPxWeatherRenderer *m_renderer;
    SPxAreaEditorMouse *m_editor;

    /* Controls. */
    CButton m_btnAdd;
    CButton m_btnDelete;
    CButton m_btnDeleteAll;
    CComboBox m_comboWeather;
    SPxWinEditCtrl m_editName;
    SPxWinSpinCtrl m_spinLat;
    SPxWinSpinCtrl m_spinLong;
    SPxWinSpinCtrl m_spinHeight;
    CButton m_radioHeightSea;
    CButton m_radioHeightGround;
    CComboBox m_comboDynSource;
    SPxWinSpinCtrl m_spinSpeed;
    SPxWinSpinCtrl m_spinCourse;
    CStatic m_lblHeadingOffset;
    SPxWinSpinCtrl m_spinHeadingOffset;
    CComboBox m_comboMotions;
    SPxWinButton m_btnShowMotion;
    CStatic m_staticTime;
    SPxWinSpinCtrl m_spinMotionOffTime;
    SPxWinSpinCtrl m_spinMotionOffX;
    SPxWinSpinCtrl m_spinMotionOffY;
    SPxWinSpinCtrl m_spinMotionOffH;
    CStatic m_lblLat;
    CStatic m_lblLong;
    CStatic m_lblHeight;
    CStatic m_lblSpeed;
    CStatic m_lblCourse;
    CStatic m_lblOffsetX;
    CStatic m_lblOffsetY;
    CStatic m_lblOffsetH;
    CButton m_checkFollow;
    SPxWinSpinCtrl m_spinIntensity;
    CButton m_btnPointAdd;
    CButton m_btnPointRemove;
    CListBox m_listPoints;
    CStatic m_lblPointX;
    CStatic m_lblPointY;
    SPxWinSpinCtrl m_spinPointX;
    SPxWinSpinCtrl m_spinPointY;

    /* Other variables. */
    SPxAtomic<int> m_repaintDue;
    SPxAtomic<int> m_syncRequired;
    double m_prevCentreLatDegs;
    double m_prevCentreLongDegs;
    double m_prevWidthMetres;
    int m_viewChangeFromTimer;
    int m_removeCallback;

    /* Private functions */
    void followWeather(int setZoom);
    static SPxErrorCode addWeatherToCombo(unsigned int index, SPxSimWeather *target, void *userArg);
    static SPxErrorCode addMotionToCombo(unsigned int index, SPxSimMotion *motion, void *userArg);
    static SPxErrorCode eventFnWrapper(UINT32 flags, const SPxRadarSimEventParams *params, void *userArg);
    SPxErrorCode eventFn(UINT32 flags);
    static int deleteRendererFn(void *invokingObject, void *userObject, void *arg);
    static int viewChangeFnWrapper(void *invokingObject, void *userObject, void *arg);
    void viewChangeFn(SPxViewControl *viewCtrl, UINT32 flags);

    /*
     * Handler functions.
     */
    void OnDelete(void);
    void OnDeleteAll(void);
    void OnChangeWeather(void);
    void OnSetName(NMHDR*, LRESULT*);
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    void OnRadioHeightSea(void);
    void OnRadioHeightGround(void);
    void OnCheckFollow(void);
    void OnChangeDynSource(void);
    void OnChangeMotion(void);
    void OnShowMotion(void);
    void OnAddPoint(void);
    void OnRemovePoint(void);
    void OnListPointsSelChange(void);

    using CDialog::Create;

}; /* class SPxSimWeatherCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
