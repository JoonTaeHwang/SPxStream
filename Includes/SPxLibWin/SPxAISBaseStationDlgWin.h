/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for AIS Base Station control dialog.
*
*
* Revision Control:
*   25/06/18 v1.3    AGC	Correctly suppress cppcheck warning.
*
* Previous Changes:
*   23/01/18 1.2    AGC	Support multiple stations.
*   18/01/18 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* For base class. */
#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxSimAISStation;
class SPxRadarSimulator;

/*
 * SPxAISBaseStationDlgWin dialog
 */
class SPxAISBaseStationDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxAISBaseStationDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxAISBaseStationDlgWin(CWnd* pParent,
				     unsigned int numObjects,
				     SPxSimAISStation *obj[],
				     SPxRadarSimulator *radarSim);
    virtual ~SPxAISBaseStationDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);
    // cppcheck-suppress virtualCallInConstructor
    virtual void SetObjIndex(int index);

    int HasObj(void) const { return(m_obj != NULL); }

    /* Dialog Data */
    enum { IDD = IDD_SPX_AIS_BASE_STATION_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    void OnTimer(UINT_PTR nIDEvent);
    virtual void OnOK(void) {}
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxSimAISStation *m_obj;
    SPxRadarSimulator *m_radarSim;

    /* Controls. */
    SPxWinSpinCtrl m_spinMmsi1;
    SPxWinSpinCtrl m_spinInterval1;
    SPxWinSpinCtrl m_spinMmsi2;
    SPxWinSpinCtrl m_spinInterval2;
    CStatic m_lblAssignStatus;
    CButton m_btnAssignSend;

    CStatic m_lblLat1;
    CStatic m_lblLon1;
    CStatic m_lblLat2;
    CStatic m_lblLon2;
    SPxWinSpinCtrl m_spinLat1;
    SPxWinSpinCtrl m_spinLon1;
    SPxWinSpinCtrl m_spinLat2;
    SPxWinSpinCtrl m_spinLon2;
    CComboBox m_comboStation;
    SPxWinSpinCtrl m_spinShip;
    CComboBox m_comboTransmit;
    CComboBox m_comboInterval;
    SPxWinSpinCtrl m_spinQuiet;
    CStatic m_lblGroupStatus;
    CButton m_btnGroupSend;

    /* Other variables. */
    UINT32 m_assignSendTime;
    UINT32 m_groupSendTime;

    /* Private functions. */
    void OnChangeMmsi1(NMHDR*, LRESULT*);
    void OnSendAssign(void);
    void OnSendGroup(void);

    using CDialog::Create;

}; /* class SPxAISBaseStationDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
