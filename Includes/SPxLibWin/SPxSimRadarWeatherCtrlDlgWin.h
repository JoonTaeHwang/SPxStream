/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulated Radar control Weather child dialog.
*
*
* Revision Control:
*   01/09/22 v1.1    AGC	Initial version.
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
 * SPxSimRadarWeatherCtrlDlgWin dialog
 */
class SPxSimRadarWeatherCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxSimRadarWeatherCtrlDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxSimRadarWeatherCtrlDlgWin(CWnd* pParent,
					  unsigned int numObjects,
					  SPxSimRadar *obj[]);
    virtual ~SPxSimRadarWeatherCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_RADAR_WEATHER_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration retrieval functions. */
    SPxSimRadar *GetObj(void) { return m_obj; }
    // cppcheck-suppress virtualCallInConstructor
    virtual void SetObjIndex(int index);

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    CWnd *m_parentWin;
    SPxSimRadar *m_obj;

    /* Controls. */
    SPxWinSpinCtrl m_spinIntensity1Value;
    SPxWinSpinCtrl m_spinIntensity7Value;

    /* Private functions */
    void OnChangeIntensity1Value(NMHDR*, LRESULT*);
    void OnChangeIntensity7Value(NMHDR*, LRESULT*);

    using SPxTabbedItemDlg::Create;

}; /* class SPxSimRadarWeatherCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
