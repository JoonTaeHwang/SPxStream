/*********************************************************************
 *
 * (c) Copyright 2020, 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Base class from which tab dialog classes are derived that
 *   interact with an SPxCondition.
 *
 * Revision Control:
 *  26/04/22 v1.13   BTB	Allow threat related conditions to be hidden.
 *
 * Previous Changes:
 *  27/01/22 1.12   SP 	Allow gate and other conditions to be hidden.
 *  24/01/22 1.11   SP 	Allow AIS/ADS-B conditions to be hidden.
 *  16/04/21 1.10   SP 	More renamed operator functions and types.
 *  13/04/21 1.9    SP 	Use renamed operator functions and types.
 *  10/02/21 1.8    AGC	Shift logic here from derived classes.
 *  08/10/20 1.7    SP 	Support restrictions.
 *  29/09/20 1.6    AGC	Sort tracks in track combo by name.
 *  25/09/20 1.5    AGC	Support use with alarm conditions.
 *  24/09/20 1.4    AGC	Get units from track database.
 *  24/09/20 1.3    AGC	Get picture database from track database.
 *  21/09/20 1.2    AGC	Fix build warnings.
 *  18/09/20 1.1    AGC	Initial version extracted from SPxRwCondTabDlg.
 *
 *********************************************************************/
#pragma once

#include "SPxLibUtils/SPxAlarmCondition.h"
#include "SPxLibUtils/SPxCondition.h"
#include "SPxWinEditCtrl.h"
#include "SPxWinListCtrl.h"
#include "SPxWinSpinCtrl.h"
#include "SPxTrackClassComboBox.h"
#include "SPxTabbedItemDlg.h"
#include "SPxLibWin/SPxWinTask.h"

/* Forward declarations. */
class SPxUniTrackDatabase;
class SPxAlarmDatabase;
class SPxConditionGroup;
class SPxCondition;
class SPxUniTrack;
class SPxTrackClassMngr;
class SPxTrackGroupEditPopupDlgWin;

/* SPxCondDlgWin dialog */
class SPxCondDlgWin : public SPxTabbedItemDlg
{
public:
    /*
     * Public types.
     */

    /*
     * Public variables.
     */

    /* Dialog Data. */
    enum { IDD = IDD_SPX_COND_DIALOG };

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    explicit SPxCondDlgWin(SPxUniTrackDatabase *trackDB,
                           SPxAlarmDatabase *alarmDB,
                           SPxTrackClassMngr *trackClassMngr=NULL,
                           SPxTrackGroupEditPopupDlgWin *trackGroupEditDlg=NULL);
    virtual ~SPxCondDlgWin(void);
    virtual BOOL Create(CWnd *parent);

    /* General. */
    SPxErrorCode SetGroup(SPxConditionGroup *group);
    virtual void SyncGUI(void);
    SPxErrorCode AllowAISConditions(int allow) { m_allowAISConditions = allow; return SPX_NO_ERROR; }
    SPxErrorCode AllowADSBConditions(int allow) { m_allowADSBConditions = allow; return SPX_NO_ERROR; }
    SPxErrorCode AllowGateConditions(int allow) { m_allowGateConditions = allow; return SPX_NO_ERROR; }
    SPxErrorCode AllowPointConditions(int allow) { m_allowPointConditions = allow; return SPX_NO_ERROR; }
    SPxErrorCode AllowRefPointConditions(int allow) { m_allowRefPointConditions = allow; return SPX_NO_ERROR; }
    SPxErrorCode AllowThreatConditions(int allow) { m_allowThreatConditions = allow; return SPX_NO_ERROR; }

    /* Master. */
    virtual void SetIsMaster(int isMaster) { m_isMaster = isMaster; }
    int GetIsMaster(void) { return m_isMaster; }

    /* SPxTabbedItemDlg interface. */
    virtual void SyncFromObj(void) { SyncGUI(); }
    virtual int GetIDD(void) const { return IDD; }
    virtual SPxErrorCode GetTabName(char *buf, int bufSizeBytes) const;

protected:
    /* 
     * Protected variables. 
     */

    /* 
     * Protected functions. 
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    /* Implement these functions in a derived class. */
    virtual const char *getNameEditToolTip(void) const = 0;
    virtual BOOL canTrackBeAddedToCombo(SPxUniTrack *track)=0;

    /* GUI callback functions. */
    virtual afx_msg void OnDestroy(void);
    afx_msg void OnSize(UINT nType, int width, int height);
    virtual afx_msg void OnTimer(UINT_PTR nIDEvent);
    virtual void OnOK(void) {}
    virtual void OnCancel(void) {}

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Child controls. */
    SPxWinListCtrl m_list;
    SPxWinEditCtrl m_nameEdit;
    CComboBox m_argOpCombo;
    SPxWinSpinCtrl m_argSpin;
    CStatic m_argUnitsLabel;
    CComboBox m_argCombo;
    SPxTrackClassComboBox m_argClassCombo;
    CButton m_trackGroupBtn;
    CStatic m_withinLabel;
    SPxWinSpinCtrl m_timeLimitSpin;
    CStatic m_timeLimitLabel;
    SPxWinSpinCtrl m_rangeLimitSpin;
    CStatic m_rangeLimitLabel;
    CStatic m_nameLabel;

    /* General. */
    CStringA m_name;                          /* Current display name. */
    BOOL m_isArgComboStr;                     /* Is combo storing strings or values? */
    SPxUniTrackDatabase *m_trackDB;           /* Unified track database. */
    SPxAlarmDatabase *m_alarmDB;              /* Alarm database. */
    SPxTrackClassMngr *m_trackClassMngr;      /* Track class manager. */
    SPxTrackGroupEditPopupDlgWin *m_trackGroupEditDlg; /* Track group edit dialog. */
    SPxCondition::Type_t m_shapeType;
    int m_isMaster;
    SPxWinTask m_task;
    const char *m_tabName;
    CToolTipCtrl m_toolTip;                   /* Used to implement tool tips. */
    CStringA m_listToolTipText;               /* Tool tip for conditions list. */
    SPxConditionGroup *m_group;               /* Current condition group. */
    SPxCondition *m_selectedCondition;        /* Selected condition. */
    SPxConditionGroup *m_pmGroup;             /* Popup menu condition group. */
    SPxCondition *m_pmCondition;              /* Popup menu condition. */
    BOOL m_isChangePending;                   /* Change pending to conditions list. */
    BOOL m_allowAISConditions;                /* Allow AIS related conditions? */
    BOOL m_allowADSBConditions;               /* Allow ADS-B related conditions? */
    BOOL m_allowGateConditions;               /* Allow gate (shape) related conditions? */
    BOOL m_allowPointConditions;              /* Allow point (shape) related conditions? */
    BOOL m_allowRefPointConditions;           /* Allow ref point related conditions? */
    BOOL m_allowThreatConditions;             /* Allow threat related conditions? */

    /*
     * Private functions.
     */

    /* General. */
    void syncGUIFromTimer(void);
    BOOL canConditionBeCreated(SPxConditionGroup *condGroup, SPxCondition::Type_t type);
    void onRowSelect(int row, SPxWinListCtrl::SelectState_t state);
    void addConditionToList(SPxCondition *condition);
    SPxErrorCode processTrackDatabaseEvent(void);
    SPxErrorCode processAlarmDatabaseEvent(void);
    void getArgComboSize(CSize *size, int showTrackGroupBtn) const;
    void addPopupMenuConditionItems(CMenu *menu,
                                    SPxConditionGroup *condGroup,
                                    SPxCondition *condition);
    void onPopupMenuSelect(int row, int selection);

    /* Event handling functions. */
    void onConditionCreate(SPxCondition *condition);
    void onConditionChange(SPxCondition *condition,
                           SPxCondition::ChangeFlags_t flags);
    void onConditionDelete(SPxCondition *condition);
    void onConditionGroupCreate(UINT32 conditionGroupID);
    void onConditionGroupChange(UINT32 conditionGroupID) {};
    void onConditionGroupDelete(UINT32 conditionGroupID);

    /* GUI callback functions. */
    afx_msg LRESULT OnEnterEdit(WPARAM wParam, LPARAM lParam);
    afx_msg void OnKillFocusNameEdit(void);
    afx_msg LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    afx_msg void OnCbnSelchangeArgOperatorCombo(void);
    afx_msg void OnCbnDropdownArgOperatorCombo(void);
    afx_msg void OnCbnSelchangeArgCombo(void);
    afx_msg void OnCbnDropdownArgCombo(void);
    afx_msg void OnCbnSelchangeArgClassCombo(void);
    afx_msg void OnCbnDropdownArgClassCombo(void);
    afx_msg void OnEditTrackGroup(void);

    /*
     * Private static functions.
     */

    static void rowSelectHandler(void *userArg, 
                                 int row, 
                                 SPxWinListCtrl::SelectState_t state);

    static void popupMenuCreateHandler(void *userArg, int row, CMenu *menu);

    static void popupMenuSelectHandler(void *userArg, int row, int selection);

    static int conditionIterator(void *groupVoid, 
                                 void *objVoid, 
                                 void *conditionVoid);

    static int addTrackToVectorIterator(void *trackDBVoid,
                                        void *objVoid, 
                                        void *trackVoid);

    static int addRefPointToComboIterator(void *trackDBVoid, 
                                          void *objVoid, 
                                          void *refPointVoid);

    static SPxErrorCode shapeIterator(SPxAreaObj *shape, void *userArg);

    static int trackDatabaseEventHandler(void *invokingObjPtr,
                                         void *userObjPtr,
                                         void *eventPtr);
    static int alarmDatabaseEventHandler(void *invokingObjPtr,
                                         void *userObjPtr,
                                         void *eventPtr);

    using CDialog::Create;

};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
