/*********************************************************************
 *
 * (c) Copyright 2020, 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for dialog for managing track filters.
 *
 * Revision Control:
 *  26/04/22 v1.5    BTB	Allow threat related conditions to be hidden.
 *
 * Previous Changes:
 *  27/01/22 1.4    SP 	Allow gate conditions to be hidden.
 *  24/01/22 1.3    SP 	Allow AIS/ADS-B conditions to be hidden.
 *  21/09/20 1.2    AGC	Fix build warnings.
 *  18/09/20 1.1    AGC	Initial version.
 *
 *********************************************************************/

#pragma once

#include "SPxResource.h"
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxAutoPtr.h"

/* Forward declarations. */
class SPxUniTrackDatabase;
class SPxTrackGroupEditPopupDlgWin;
class SPxTrackFiltersPanelDlgWin;
class SPxTrackFilterCondDlgWin;
class SPxUniTrackFilter;

class SPxTrackFiltersDlgWin : public CDialog
{
public:

    /* Dialog Data. */
    enum { IDD = IDD_SPX_TRACK_FILTERS_DIALOG };

    /* Construction and destruction. */
    explicit SPxTrackFiltersDlgWin(SPxUniTrackDatabase *trackDB,
                                   SPxTrackGroupEditPopupDlgWin *trackGroupEditDlg=NULL);
    virtual ~SPxTrackFiltersDlgWin(void);
    BOOL Create(CWnd *parent);
    SPxErrorCode AllowAISConditions(int allow);
    SPxErrorCode AllowADSBConditions(int allow);
    SPxErrorCode AllowGateConditions(int allow);
    SPxErrorCode AllowPointConditions(int allow);
    SPxErrorCode AllowRefPointConditions(int allow);
    SPxErrorCode AllowThreatConditions(int allow);

    void SyncFromObj(void);

protected:
    DECLARE_MESSAGE_MAP()
    virtual BOOL OnInitDialog(void);
    void OnSize(UINT nType, int width, int height);
    void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

private:
    SPxAutoPtr<SPxTrackFiltersPanelDlgWin> m_filtersDlg;
    SPxAutoPtr<SPxTrackFilterCondDlgWin> m_condDlg;
    CRect m_minSize;

    void filterEdit(SPxUniTrackFilter *filter);
    void resize(int width, int height);

    using CDialog::Create;

};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
