/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for popup version of dialog for managing track groups.
 *
 * Revision Control:
 *  21/09/20 v1.2    AGC	Fix build warnings.
 *
 * Previous Changes:
 *  18/09/20 1.1    AGC	Initial version.
 *
 *********************************************************************/

#pragma once

#include "SPxResource.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCommon.h"

 /* Forward declarations. */
class SPxUniTrackDatabase;
class SPxTrackGroupEditDlgWin;

class SPxTrackGroupEditPopupDlgWin : public CDialog
{
public:

    /* Dialog Data. */
    enum { IDD = IDD_SPX_TRACK_GROUP_EDIT_POPUP_DIALOG };

    /* Construction and destruction. */
    explicit SPxTrackGroupEditPopupDlgWin(SPxUniTrackDatabase *trackDB);
    virtual ~SPxTrackGroupEditPopupDlgWin(void);
    BOOL Create(CWnd *parent);

    void SetTrackGroupID(UINT32 trackGroupID);

    virtual void SyncFromObj(void);

protected:
    DECLARE_MESSAGE_MAP()
    virtual BOOL OnInitDialog(void);
    void OnSize(UINT nType, int width, int height);
    void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    virtual void OnOK(void) {};

private:
    SPxAutoPtr<SPxTrackGroupEditDlgWin> m_dlg;
    CRect m_minSize;

    void resize(int width, int height);

    using CDialog::Create;

};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
