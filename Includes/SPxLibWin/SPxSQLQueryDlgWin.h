/*********************************************************************
 *
 * (c) Copyright 2016, 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   A pop-up dialog used to show results from an SQL query.
 *
 * Revision Control:
 *   14/02/18 v1.7    AGC	Fix N/C 527 - Use correct special column indices.
 *
 * Previous Changes:
 *   18/07/16 1.6   SP 	Show date/time at cursor.
 *   14/07/16 1.5   SP 	Add pop-up query builder.
 *   08/07/16 1.4   SP  Many changes to improve query speed.
 *                      Add export feature.
 *   24/06/16 1.3   SP  Add Reset().
 *   13/04/16 1.2   AGC Fix clang warnings.
 *   08/04/16 1.1   SP  Initial version.
 *
 **********************************************************************/

#pragma once

#define SPX_AFXWIN
#include "SPxResource.h"
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxError.h"

/* Forward declare any classes required. */
class SPxSQLDatabase;
class SPxThread;
class SPxSQLQueryDlgWin;

/* Results table list control. */
class SPxSQLResultsTable : public CListCtrl
{
    DECLARE_DYNAMIC(SPxSQLResultsTable)

public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxSQLResultsTable(SPxSQLQueryDlgWin *queryDlg);
    virtual ~SPxSQLResultsTable(void);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()

private:

    /*
     * Private variables.
     */

    /* General. */
    SPxSQLQueryDlgWin *m_queryDlg;
    BOOL m_isMouseTracking;

    /*
     * Private functions.
     */

    /* GUI callback functions. */
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
};

/* Main dialog class. */
class SPxSQLQueryDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxSQLQueryDlgWin)

public:

    /* Grant private access to results table. */
    friend class SPxSQLResultsTable;

    /*
     * Public variables.
     */

    /* Dialog Data */
    enum { IDD = IDD_SPX_SQL_QUERY_DIALOG };

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxSQLQueryDlgWin(SPxSQLDatabase *db, int maxNumResults=0);
    virtual ~SPxSQLQueryDlgWin(void);
    virtual BOOL Create(CWnd* pParent);

    /* Control. */
    virtual SPxErrorCode SetSQL(const char *sql, int run, int addToHistory);
    virtual SPxErrorCode Reset(int resetResults=TRUE, int resetSQL=TRUE, int resetHistory=TRUE);

    /* Access. */
    virtual CListCtrl *GetTable(void) { return m_resultsTable; }
    virtual int GetSelectedRow(void);
    virtual const char *GetValue(int row, int column);
    virtual unsigned int GetSequenceNum(int row);
    virtual UINT8 GetChannel(int row);
    virtual SPxErrorCode GetTimestamp(int row, SPxTime_t *timestamp);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual void OnOK(void);
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnTimer(UINT_PTR nIDEvent);
    virtual void OnSize(UINT nType, int cWidth, int cHeight);
    virtual void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    virtual void OnRowSelected(int row, int isSelected);
    virtual void OnItemDoubleClicked(int row, int column);

private:

    /*
     * Private types.
     */

    /* Query status. */
    typedef enum
    {
        QUERY_STATUS_OK,
        QUERY_STATUS_USER_CANCELLED,
        QUERY_STATUS_SYS_CANCELLED,
        QUERY_STATUS_ERROR
        
    } QueryStatus;

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    SPxSQLDatabase *m_db;
    int m_maxNumResults;
    unsigned int m_minWidth;
    unsigned int m_minHeight;
    unsigned int m_tableBottomOffset;
    BOOL m_isMouseTracking;
    int m_colSeqNum;
    int m_colChannel;
    int m_colTimestamp;

    /* Dialog items. */
    SPxSQLResultsTable *m_resultsTable;
    CComboBox *m_sqlCombo;
    CButton *m_openBuilderBn;
    CButton *m_submitBn;
    CButton *m_clearBn;
    CButton *m_exportBn;
    CProgressCtrl *m_progress;
    CStatic *m_statusText;
    CStatic *m_dateTimeText;

    /* Query thread. */
    SPxThread *m_queryThread;
    CString m_querySQL;
    CStringA m_queryErrMsg;
    BOOL m_isQueryFinished;
    QueryStatus m_queryStatus;

    /* Export thread. */
    SPxThread *m_exportThread;
    CStringA m_exportErrMsg;
    unsigned int m_exportPercentComplete;
    BOOL m_isExportComplete;
    CStringA m_exportFilename;

    /*
     * Private functions.
     */

    /* General. */
    void resize(int cWidth, int cHeight);
    void clearBufferAndTable(void);
    void updateTable(void);
    void syncGUI(void);
    void syncGUIFromTimer(void);
    void appendToHistory(const char *sql);
    void setDateTimeText(const SPxTime_t *time);
    void startQuery(void);
    void startExport(const char *filename);

    /* Query thread functions. */
    void appendRowToBuffer(int numColumns,
                           const char **columnValues,
                           const char **columnNames);

    /* Export thread functions. */
    void appendHeadersToFile(FILE *fPtr);
    void appendRowToFile(FILE *fPtr, int row);

    /* GUI callback functions. */
    afx_msg void OnBnClickedOpenBuilderButton(void);
    afx_msg void OnBnClickedSubmitButton(void);
    afx_msg void OnBnClickedClearButton(void);
    afx_msg void OnBnClickedExportButton(void);
    afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemDoubleClicked(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);

    /*
     * Private static functions.
     */

    static void queryResultCallback(void *userArg,
                                    SPxSQLDatabase *db,
                                    int numColumns,
                                    const char **columnValues,
                                    const char **columnNames);

    static void *queryThreadHandler(SPxThread *thread);
    static void *exportThreadHandler(SPxThread *thread);

    using CDialog::Create;
};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
