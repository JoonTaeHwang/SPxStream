/*********************************************************************
 *
 * (c) Copyright 2016, 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   A modal pop-up dialog used to build SQL queries.
 *
 * Revision Control:
 *   15/06/20 v1.4   SP 	Add header required for CDateTimeCtrl.
 *
 * Previous Changes:
 *   12/06/20 1.3   SP 	Add date/time picker for timestamps.
 *   28/07/16 1.2   SP 	Remove m_selectSQLPart.
 *   14/07/16 1.1   SP 	Initial version.
 *
 **********************************************************************/

#pragma once

#define SPX_AFXWIN
#include "SPxResource.h"
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibWin/SPxWinEditCtrl.h"

/* Include required header for CDateTimeCtrl. */
#include "afxdtctl.h"

/* Forward declare any classes required. */
class SPxSQLDatabase;
class SPxThread;

class SPxSQLBuilderDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxSQLBuilderDlgWin)

public:

    /*
     * Public variables.
     */

    /* Dialog Data */
    enum { IDD = IDD_SPX_SQL_BUILDER_DIALOG };

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxSQLBuilderDlgWin(CWnd* pParent, SPxSQLDatabase *db, const char *sql);
    virtual ~SPxSQLBuilderDlgWin();

    /* Get SQL from dialog. */
    virtual const char *GetSQL(void) { return m_fullSQL.GetBuffer(); }

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual void OnSize(UINT nType, int cWidth, int cHeight);
    virtual void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    SPxSQLDatabase *m_db;
    unsigned int m_minWidth;
    unsigned int m_minHeight;
    CStringA m_inputSQL;
    CStringA m_fullSQL;
    CStringA m_filterSQLPart;
    CStringA m_sortSQLPart;
    unsigned int m_numColumns;
    SPxTime_t m_timestamp;

    /* Dialog items. */
    CComboBox *m_tablesCombo;
    CListBox *m_columnsListBox;
    CButton *m_columnsSelectAllBn;
    CComboBox *m_filterColumnCombo;
    CComboBox *m_filterOpCombo;
    CSPxWinEditCtrl m_filterValueEdit;
    CDateTimeCtrl *m_filterDatePicker;
    CDateTimeCtrl *m_filterTimePicker;
    CButton *m_filterAddBn;
    CButton *m_filterClearAllBn;
    CComboBox *m_sortColumnCombo;
    CComboBox *m_sortOpCombo;
    CButton *m_sortAddBn;
    CButton *m_sortClearAllBn;
    CButton *m_enableEditingCheck;
    CSPxWinEditCtrl m_sqlEdit;

    /*
     * Private functions.
     */

    /* General. */
    void resize(int cWidth, int cHeight);
    void syncGUI(BOOL isTableChanged);
    void syncGUIFromSQL(const char *sql);

    /* GUI callback functions. */
    afx_msg void OnCbnSelchangeTablesCombo(void);
    afx_msg void OnLbnSelchangeColumnsList(void);
    afx_msg void OnBnClickedSelectAllColumnsButton(void);
    afx_msg void OnCbnSelchangeFilterColumnCombo(void);
    afx_msg void OnBnClickedFilterAddButton(void);
    afx_msg void OnBnClickedFilterClearAllButton(void);
    afx_msg void OnDtnDatetimeFilterChangedPicker(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedSortAddButton(void);
    afx_msg void OnBnClickedSortClearAllButton(void);
    afx_msg void OnBnClickedEnableEditingCheck(void);
    afx_msg LRESULT OnEditEnter(WPARAM wParam, LPARAM lParam);
    afx_msg void OnKillFocusSqlEdit(void);
    afx_msg void OnKillFocusFilterValueEdit(void);

    /*
     * Private static functions.
     */
};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
