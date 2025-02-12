/*********************************************************************
*
* (c) Copyright 2014, 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the AIS control pop-up dialog.
*
*
* Revision Control:
*   25/06/18 v1.5    AGC	Correctly suppress cppcheck warning.
*
* Previous Changes:
*   24/01/18 1.4    AGC	Fix close button behaviour.
*   23/01/18 1.3    AGC	Support multiple objects.
*   18/01/18 1.2    AGC	Support Inland AIS configuration.
*			Support Base Station control.
*   02/06/14 1.1 AGC	Initial version.
*
**********************************************************************/

#pragma once

/* For base class. */
#include "SPxLibWin/SPxTableDlgWin.h"

/* Forward declarations. */
class SPxAISIface;
class SPxAISDlgWin;
class SPxAISInlandDlgWin;
class SPxAISBaseStationDlgWin;
class SPxSimAISStation;
class SPxRadarSimulator;

/*
 * SPxAISPopupDlgWin dialog
 */
class SPxAISPopupDlgWin : public SPxTableDlgWin
{
    DECLARE_DYNAMIC(SPxAISPopupDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxAISPopupDlgWin(CWnd* pParent,
			       unsigned int numObjects,
			       SPxAISIface *obj[],
			       SPxSimAISStation *station[]=NULL,
			       SPxRadarSimulator *radarSim=NULL);
    virtual ~SPxAISPopupDlgWin();
    virtual BOOL Create(void);

    /* Sync functions. */
    void SyncFromObj(void);
    // cppcheck-suppress virtualCallInConstructor
    virtual void SetObjIndex(int index);

    // Dialog Data
    enum { IDD = IDD_SPX_AIS_POPUP_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

protected:
    /* Protected functions */
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    void OnOK(void) {}
    virtual void OnCancel(void) { CDialog::OnCancel(); }
    DECLARE_MESSAGE_MAP()

private:
    CWnd *m_parentWin;
    SPxAutoPtr<SPxAISDlgWin> m_aisDlg;
    SPxAutoPtr<SPxAISInlandDlgWin> m_aisInlandDlg;
    SPxAutoPtr<SPxAISBaseStationDlgWin> m_aisBaseStationDlg;

    void OnSync(NMHDR*, LRESULT*);

    using CDialog::Create;

}; /* class SPxAISPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
