/*********************************************************************
*
* (c) Copyright 2015, 2016, 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulated Radar blind sector control child dialog.
*
*
* Revision Control:
*   23/07/18 v1.7    AGC	Add enable checkbox.
*
* Previous Changes:
*   25/06/18 1.6    AGC	Correctly suppress cppcheck warning.
*   23/01/18 1.5    AGC	Support multiple objects.
*   15/12/16 1.4    AGC	Add option to clear aux video in blind sectors.
*   03/03/16 1.3    AGC	Fix clang warnings.
*   01/12/15 1.2    AGC	Use project code to disable some controls.
*   26/11/15 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxBlindSectorRef. */
#include "SPxLibData/SPxSimRadar.h"

/* For SPxWinEditCtrl. */
#include "SPxLibWin/SPxWinEditCtrl.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxSimRadar;

/*
 * SPxSimRadarBlindSectorCtrlDlgWin dialog
 */
class SPxSimRadarBlindSectorCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxSimRadarBlindSectorCtrlDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxSimRadarBlindSectorCtrlDlgWin(CWnd* pParent,
					      unsigned int numObjects,
					      SPxSimRadar *obj[],
					      unsigned int projectCode=110);
    virtual ~SPxSimRadarBlindSectorCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void) { SyncFromObj(NULL); }
    void SyncFromObj(const char *newBlindSectorName);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_RADAR_BLIND_SECTOR_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration retrieval functions. */
    SPxSimRadar *GetObj(void) { return m_obj; }
    // cppcheck-suppress virtualCallInConstructor
    virtual void SetObjIndex(int index);

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
    void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    void OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    CWnd *m_parentWin;
    SPxSimRadar *m_obj;
    const unsigned int m_projectCode;
    int m_syncRequired;

    CButton m_checkBlindSectorsEnabled;
    CStringA m_currBlindSectorName;
    CString m_currBlindSectorDesc;
    int m_maxBlindSectorItemWidth;

    CSliderCtrl m_sliderBlindSectorNoise;
    CStatic m_lblBlindSectorNoise;
    CButton m_checkBlindSectorBlankAux;
    CButton m_btnBlindSectorAdd;
    CButton m_btnBlindSectorRemove;
    CListBox m_listBlindSectors;
    SPxWinEditCtrl m_editBlindSectorName;
    CStatic m_lblBlindSectorStart;
    CStatic m_lblBlindSectorEnd;
    SPxWinSpinCtrl m_spinBlindSectorStart;
    SPxWinSpinCtrl m_spinBlindSectorEnd;
    CButton m_radioBlindSectorNorth;
    CButton m_radioBlindSectorHeading;

    /* Private functions */
    void OnCheckEnable(void);
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    void OnCheckBlankAux(void);
    void OnAddBlindSector(void);
    void OnRemoveBlindSector(void);
    void OnListBlindSectorsSelChange(void);
    void OnEditBlindSectorName(NMHDR*, LRESULT*);
    void OnBlindSectorNorth(void);
    void OnBlindSectorHeading(void);
    static SPxErrorCode blindSectorFnWrapper(void *userArg, const char *name,
	double startDegs, double endDegs, SPxBlindSectorRef reference);
    SPxErrorCode blindSectorFn(const char *name,
	double startDegs, double endDegs, SPxBlindSectorRef reference);
    static SPxErrorCode changeFn(SPxSimRadar *radar,
				 UINT32 flags,
				 void *userArg);

    using SPxTabbedItemDlg::Create;

}; /* class SPxSimRadarBlindSectorCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
