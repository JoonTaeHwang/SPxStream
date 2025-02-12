/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Base class from which child dialog classes are derived that
 *   interact with an SPxConditionGroup.
 *
 * Revision Control:
 *  21/09/20 v1.2    AGC	Fix build warnings.
 *
 * Previous Changes:
 *  18/09/20 1.1    AGC	Initial version extracted from SPxRwCondGroupsPanelDlgWin.
 *
 *********************************************************************/
#pragma once

/* Required headers. */
#include "SPxResource.h"
#include "SPxLibUtils/SPxTimer.h"
#include "SPxLibNet/SPxUniTrackFilter.h"
#include "SPxLibWin/SPxWinListCtrl.h"
#include "SPxLibWin/SPxWinTask.h"

/* Forward declarations. */
class SPxConditionGroup;

/* SPxCondGroupsPanelDlgWin dialog */
class SPxCondGroupsPanelDlgWin : public CDialog
{
public:
    /*
     * Public variables.
     */

    /* Dialog Data. */
    enum { IDD = IDD_SPX_COND_GROUPS_PANEL_DIALOG };

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    explicit SPxCondGroupsPanelDlgWin(SPxUniTrackDatabase *trackDB,
                                      int sharedMode=FALSE);
    virtual ~SPxCondGroupsPanelDlgWin(void);
    BOOL Create(CWnd *parent);

    void SyncFromObj(void) { syncGUI(); }

    /* General. */
    int IsSharedMode(void) const { return m_isSharedMode; }

protected:
    /* 
     * Protected variables. 
     */

    /* GUI controls. */
    SPxWinListCtrl m_list;                   /* Condition groups list. */

    /* General. */
    BOOL m_isChangePending;                 /* Change pending to list. */
    SPxConditionGroup *m_selectedGroup;     /* Group currently selected. */

    /* 
     * Protected functions. 
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);

    /* General. */
    virtual void resize(int width, int height);
    virtual void syncGUI(void);
    virtual void syncGUIFromTimer(void);

    /* Implement these functions in a derived class. */
    virtual void updateBuffer(void)=0;
    virtual void reportGroupEdit(SPxConditionGroup *group)=0;
    virtual void onPopupMenuCreate(int row, CMenu *menu)=0;
    virtual void onPopupMenuSelect(int row, int selection)=0;
    virtual void onTrackFilterCreate(SPxUniTrackFilter *filter) {}
    virtual void onTrackFilterChange(SPxUniTrackFilter *filter,
                                     SPxUniTrackFilter::ChangeFlags_t flags) {}
    virtual void onTrackFilterDelete(SPxUniTrackFilter *filter) {}
    virtual void onTrackFilterConditionCreate(SPxUniTrackCondition *condition) {}
    virtual void onTrackFilterConditionChange(SPxUniTrackCondition *condition,
                                              SPxUniTrackCondition::ChangeFlags_t flags) {}
    virtual void onTrackFilterConditionDelete(SPxUniTrackCondition *condition) {}
    virtual void onTrackGroupCreate(UINT32 trackGroupID) {}
    virtual void onTrackGroupChange(UINT32 trackGroupID) {}
    virtual void onTrackGroupDelete(UINT32 trackGroupID) {}

    /* GUI callback functions. */
    virtual void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR *lpncsp);
    virtual void OnNcPaint(void);
    afx_msg void OnSize(UINT nType, int width, int height);
    virtual afx_msg void OnTimer(UINT_PTR nIDEvent);
    virtual void OnOK(void) {}
    virtual void OnCancel(void) {}

    /*
     * Protected static functions.
     */

    static int addGroupToBufferIterator(void *databaseVoid, 
                                        void *objVoid, 
                                        void *alarmVoid);

private:
    /*
     * Private variables.
     */

    /* General. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxUniTrackDatabase *m_trackDB;
    SPxTimer m_timer;                          /* Update timer. */
    SPxConditionGroup *m_prevSelectedGroup;    /* Previously selected context. */
    int m_isSharedMode;                        /* In shared mode? */
    CBrush *m_borderBrush;
    SPxWinTask m_task;

    /*
     * Private functions.
     */

    /* General. */
    void addGroupToBuffer(SPxConditionGroup *group);
    void onRowSelect(int row,  SPxWinListCtrl::SelectState_t state);
    void onRowCheckboxClicked(int row, int state);
    SPxErrorCode processTrackDatabaseEvent(void);

    /*
     * Private static functions.
     */

    static void updateBufferTimer(void *userArg);

    static void rowSelectHandler(void *userArg, 
                                 int row, 
                                 SPxWinListCtrl::SelectState_t state);

    static void rowCheckboxClickedHandler(void *userArg, 
                                          int row, 
                                          int state);

    static void popupMenuCreateHandler(void *userArg, int row, CMenu *menu);
    static void popupMenuSelectHandler(void *userArg, int row, int id);

    static int trackDatabaseEventHandler(void *invokingObjPtr,
                                         void *userObjPtr,
                                         void *eventPtr);

    using CDialog::Create;
};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
