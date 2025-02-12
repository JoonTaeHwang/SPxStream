/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for dialog for managing track groups.
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
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibNet/SPxUniTrack.h"
#include "SPxLibWin/SPxTabbedItemDlg.h"
#include "SPxLibWin/SPxWinEditCtrl.h"

/* Forward declarations. */
class SPxUniTrackDatabase;
class SPxUniTrack;

class SPxTrackGroupEditDlgWin : public SPxTabbedItemDlg
{
public:

    /* Dialog Data. */
    enum { IDD = IDD_SPX_TRACK_GROUP_EDIT_DIALOG };

    /* Construction and destruction. */
    explicit SPxTrackGroupEditDlgWin(SPxUniTrackDatabase *trackDB);
    virtual ~SPxTrackGroupEditDlgWin(void);
    BOOL Create(CWnd *parent);

    void SetTrackGroupID(UINT32 trackGroupID);

    virtual void SyncFromObj(void);
    virtual SPxErrorCode GetTabName(char *buf, int bufSizeBytes) const;

    virtual int GetIDD(void) const { return IDD; }

protected:
    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    void OnTimer(UINT_PTR nIDEvent);
    virtual void OnOK(void) {}
    virtual void OnCancel(void) {}

private:
    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxUniTrackDatabase *m_trackDB;
    UINT32 m_trackGroupID;
    UINT32 m_trackGroupDesc;
    int m_isFused;
    SPxUniTrack::TrackType_t m_srcType;
    int m_srcIndex;
    SPxUniTrack::FieldType_t m_fieldType;
    UINT32 m_field;

    CButton m_addGroupBtn;
    CButton m_saveGroupBtn;
    CButton m_delGroupBtn;
    CComboBox m_groupCombo;
    CStatic m_nameLbl;
    SPxWinEditCtrl m_nameEdit;
    CButton m_addDescBtn;
    CButton m_remDescBtn;
    CListBox m_descList;
    CButton m_fusedCheck;
    CStatic m_sourceLbl;
    CComboBox m_sourceCombo;
    CStatic m_fieldTypeLbl;
    CComboBox m_fieldTypeCombo;
    CStatic m_fieldLbl;
    CComboBox m_fieldCombo;

    void OnAddGroup();
    void OnSaveGroup();
    void OnDelGroup();
    void OnGroupChange();
    void OnGroupNameChange(NMHDR*, LRESULT*);
    void OnAddDesc();
    void OnRemDesc();
    void OnDescChange();
    void OnDescStrChange();
    void OnFieldDropDown();

    void repopulateFieldCombo();
    void addToFieldCombo(SPxUniTrack *uniTrack);

    static int trackIteratorFn(void *invokingObject,
			       void *userObject,
			       void *arg);

    using CDialog::Create;

};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
