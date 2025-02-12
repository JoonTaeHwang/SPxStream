/*********************************************************************
 *
 * (c) Copyright 2019, 2020 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Wrapper around a CListCtrl to support a separate data buffer,
 *   hidden columns and other useful things.
 *
 * Revision Control:
 *   28/06/22 v1.19  SP 	Fix auto resize of last column after sort order change.
 *                      	Tidy up tabs and spaces.
 *
 * Previous Changes:
 *   14/06/22 1.18  AGC	Avoid mutex lock errors when dragging rows.
 *   04/05/22 1.17  SP 	Stop size oscillation.
 *   01/12/21 1.16  SP 	Add SetColumnWidth().
 *   24/09/20 1.15  AGC	Fix column sizing for per-monitor high DPI.
 *   23/09/20 1.14  AGC	Remove selection in progress flag.
 *   18/09/20 1.13  AJH	Add column alignment.
 *   08/07/20 1.12  AGC	Add drag/drop support.
 *   18/05/20 1.11  SP 	Support default sort order.
 *   06/05/20 1.10  AGC	Support menu customisation.
 *   04/05/20 1.9   AGC	Fix checkbox clicking.
 *   04/05/20 1.8   AGC	Add clear selection option to ClearRows().
 *   04/05/20 1.7   AGC	Add CLICKED selection state.
 *   29/04/20 1.6   BTB	Support strings as row data.
 *   08/04/20 1.5   AGC	Support disabled colour.
 *   03/04/20 1.4   AGC	Support customisable colours.
 *                     	 Support multiline rows.
 *                     	Support coloured indicators.
 *   01/07/19 1.3   SP 	Add SetTableAutoSize().
 *   11/06/19 1.2   SP 	Correct comment.
 *   07/06/19 1.1   SP 	Initial version based on SPxRwListCtrl.
 *
 *********************************************************************/

#pragma once

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxSharedPtr.h"

/* Forward declarations. */
class SPxWinMenu;

/* SPxWinListCtrl dialog */
class SPxWinListCtrl : public CListCtrl
{
    DECLARE_DYNAMIC(SPxWinListCtrl)

public:
    /*
     * Public types.
     */
    struct Row_t;
    struct Cell_t;

    /* Selection state. */
    typedef enum
    {
        DESELECTED = 0,
        SELECTED = 1,
        DOUBLE_CLICKED = 2,
        CLICKED = 3
    
    } SelectState_t;

    /* Sort order. */
    typedef enum
    {
        SORT_NONE = 0,
        SORT_ASCENDING = 1,
        SORT_DESCENDING = 2
    
    } SortOrder_t;

    /* Alignment. */
    typedef enum
    {
        ALIGN_LEFT = 0,
        ALIGN_CENTRE = 1,
        ALIGN_RIGHT = 2

    } Align_t;

    /* Checkbox display in first column. */
    typedef enum
    {
        CHECKBOX_MODE_NONE,
        CHECKBOX_MODE_BISTATE,
        CHECKBOX_MODE_TRISTATE

    } CheckboxMode_t;

    /* Row selection callback function. */
    typedef void (* RowSelectFn_t)(void *arg, int row, SelectState_t state);

    /* Row checkbox clicked callback function. */
    typedef void (* RowCheckboxFn_t)(void *arg, int row, int state);

    /* Row drag callback function. */
    typedef void(*RowDragDropFn_t)(void *arg, int row, int insertedBeforeRow);

    /* Popup menu callback function. */
    typedef void (* PopupMenuCreateFn_t)(void *arg, int row, CMenu *menu);
    typedef void (* PopupMenuSelectFn_t)(void *arg, int row, int selection);

    /*
     * Public variables.
     */

    /* Dialog Data. */

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    SPxWinListCtrl(void);
    virtual ~SPxWinListCtrl(void);

    /* Popup menu customisation. */
    void SetWinMenu(SPxWinMenu *menu);

    /*********************************************************************
     * Buffer function that can be called from any thread.
     *********************************************************************/

    /* Buffer lock and unlock. */
    virtual void LockBuffer(void);
    virtual void UnlockBuffer(void);

    /* Checkbox configuration. */
    virtual SPxErrorCode SetCheckboxMode(CheckboxMode_t mode);
    virtual CheckboxMode_t GetCheckboxMode(void) const { return m_checkboxMode; }

    /* Column functions. */
    virtual int GetNumColumns(void) const;
    virtual SPxErrorCode SetColumns(int numColumns, const char *names[]=NULL);
    virtual int AddColumn(const char *name=NULL);
    virtual SPxErrorCode SetColumnName(int column, const char *name=NULL);
    virtual const char *GetColumnName(int column) const;
    virtual SPxErrorCode SetColumnVisible(int column, BOOL isVisible);
    virtual BOOL IsColumnVisible(int column) const;
    virtual SPxErrorCode SetDefaultSortOrder(int column, SortOrder_t order);
    virtual int GetDefaultSortColumn(void) const { return m_defaultSortColumn; }
    virtual SortOrder_t GetDefaultSortOrder(void) const { return m_defaultSortOrder; }
    virtual SPxErrorCode SetColumnSortOrder(int column, SortOrder_t order);
    virtual SPxWinListCtrl::SortOrder_t GetColumnSortOrder(int column) const;
    virtual SPxErrorCode SetColumnPadding(int column, int padPixels);
    virtual SPxErrorCode SetColumnAlign(int column, Align_t align);
    virtual BOOL SetColumnWidth(int column, int width);
    virtual SPxErrorCode AutoResizeColumn(int column, BOOL allowShrink);
    virtual SPxErrorCode SetTableAutoSize(BOOL isEnabled);
    virtual BOOL IsTableAutoSize(void) const { return m_isTableAutoSize; }

    /* Row functions. */
    virtual int GetNumRows(void) const { return m_numRowsInUse; }
    virtual int InsertRow(int pos);
    virtual SPxErrorCode SetRowUserData(int row, void *data);
    virtual void *GetRowUserData(int row) const;
    virtual SPxErrorCode SetRowUserValue(int row, UINT32 value);
    virtual UINT32 GetRowUserValue(int row) const;
    virtual SPxErrorCode SetRowUserString(int row, const char *rowString);
    virtual const char *GetRowUserString(int row) const;
    virtual SPxErrorCode SetRowBackgroundColour(int row, COLORREF bgr);
    virtual SPxErrorCode GetRowBackgroundColour(int row, COLORREF *bgrRtn) const;
    virtual SPxErrorCode ResetRowBackgroundColour(int row);
    virtual SPxErrorCode SetRowTextColour(int row, COLORREF bgr, BOOL flash=FALSE);
    virtual SPxErrorCode GetRowTextColour(int row, COLORREF *bgrRtn) const;
    virtual SPxErrorCode ResetRowTextColour(int row);
    virtual SPxErrorCode SetRowIndicatorColour(int row, COLORREF bgr);
    virtual SPxErrorCode GetRowIndicatorColour(int row, COLORREF *bgrRtn) const;
    virtual SPxErrorCode ResetRowIndicatorColour(int row);
    virtual SPxErrorCode SetRowCheckboxEnabled(int row, BOOL isEnabled);
    virtual BOOL IsRowCheckboxEnabled(int row) const;
    virtual SPxErrorCode SetRowCheckboxState(int row, int state);
    virtual int GetRowCheckboxState(int row) const;
    virtual SPxErrorCode SetRowLineHeight(int numLines);
    virtual int GetRowLineHeight(void) const;
    virtual SPxErrorCode SetAllowRowDragDrop(int allowRowDragDrop);
    virtual int IsRowDragDropAllowed(void) const { return m_isRowDragDropAllowed; }
    virtual SPxErrorCode SortRows(void);
    virtual BOOL IsSortPending(void) const;
    virtual SPxErrorCode ClearRows(int clearSelectedRow=FALSE);

    /* Cell functions. */
    virtual SPxErrorCode SetCell(int row, 
                                 int column, 
                                 const char *text,
                                 BOOL isNumber=FALSE,
                                 double value1=0.0,
                                 double value2=0.0);
    virtual const char *GetCellText(int row, int column) const;
    virtual SPxErrorCode GetCellValue(int row,
                                      int column,
                                      double *value1Rtn,
                                      double *value2Rtn) const;

    /* Misc. */
    virtual SPxErrorCode Clear(void);

    /* Custom colours. */
    void EnableCustomColours(int enable);
    void SetColourNormal(COLORREF colour);
    void SetColourDisabled(COLORREF colour);
    void SetColourPressed(COLORREF colour);
    void SetColourBorder(COLORREF colour);
    void SetColourText(COLORREF colour);
    COLORREF GetColourNormal(void) const;
    COLORREF GetColourDisabled(void) const;
    COLORREF GetColourPressed(void) const;
    COLORREF GetColourBorder(void) const;
    COLORREF GetColourText(void) const;

    /*********************************************************************
     * List functions that can only be called from the GUI thread.
     *********************************************************************/

    /* List functions. */
    virtual BOOL IsSyncPending(void) const;
    virtual SPxErrorCode SyncToBuffer(void);
    virtual SPxErrorCode SetSelectedRow(int row, BOOL singleSelection=TRUE);
    virtual int GetSelectedRow(void) const;
    virtual int IsRowSelected(int row) const;
    virtual SPxErrorCode SetRowSelectionEnabled(BOOL isEnabled);
    virtual BOOL IsRowSelectionEnabled(void) const { return m_isRowSelectionEnabled; }

    /* Callbacks. */
    virtual SPxErrorCode SetRowSelectFn(RowSelectFn_t fn, void *arg);
    virtual SPxErrorCode SetRowCheckboxFn(RowCheckboxFn_t fn, void *arg);
    virtual SPxErrorCode SetRowDragDropFn(RowDragDropFn_t fn, void *arg);
    virtual SPxErrorCode SetPopupMenuFn(PopupMenuCreateFn_t createFn,
                                        PopupMenuSelectFn_t selectFn,
                                        void *arg);

protected:
    /* 
     * Protected variables. 
     */

    /* 
     * Protected functions. 
     */
    DECLARE_MESSAGE_MAP()
    virtual void PreSubclassWindow(void);
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    /* GUI callback functions. */
    virtual afx_msg void OnSize(UINT nType, int width, int height);
    virtual afx_msg LRESULT OnPostSize(WPARAM wparam, LPARAM lparam);
    virtual afx_msg void OnContextMenu(CWnd *wnd, CPoint point);
    virtual afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMIS);
    virtual afx_msg void OnColumnClicked(NMHDR *pNMHDR, LRESULT *pResult);
    virtual afx_msg void OnItemClicked(NMHDR *pNMHDR, LRESULT *pResult);
    virtual afx_msg void OnItemDoubleClicked(NMHDR *pNMHDR, LRESULT *pResult);
    virtual afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    virtual afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    virtual afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    virtual afx_msg void OnMouseLeave(void);
    virtual afx_msg void OnBeginDrag(NMHDR *pNMHDR, LRESULT *pResult);
    virtual afx_msg void OnGetData(NMHDR *pNMHDR, LRESULT *pResult);
    virtual afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    virtual afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

    int HitTest(CPoint pt, UINT* pFlags=NULL) const;

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
    mutable SPxCriticalSection m_mutex;     /* Mutex for the buffer. */
    BOOL m_isBufferLocked;                  /* Set when buffer already locked externally. */
    BOOL m_isRowSelectionEnabled;           /* Is row selection enabled? */
    CheckboxMode_t m_checkboxMode;          /* Checkbox mode. */
    CImageList m_imageList;                 /* Checkbox images. */
    BOOL m_isImageListSet;                  /* Is image list set? */
    BOOL m_isColumnChangePending;           /* Is a column change panding? */
    BOOL m_isColumnResizePending;           /* Is a column resize pending? */
    BOOL m_isRowChangePending;              /* Is a row change pending? */
    int m_firstUpdateRow;                   /* First row that has been updated. */
    int m_lastUpdateRow;                    /* Last row that has been updated. */
    BOOL m_isSyncToBufferInProgress;        /* Flag while SyncToBuffer() is being called. */
    BOOL m_isSortPending;                   /* Is a sort pending? */
    int m_numRowsInUse;                     /* Number of rows being used. */
    COLORREF m_selectedRowBGR;              /* Background colour of selected row. */
    COLORREF m_disabledTextBGR;             /* Disabled text colour. */
    BOOL m_isFlashStateOn;                  /* Current flash state. */
    BOOL m_isTableAutoSize;                 /* Auto size table to fill window? */
    int m_rowLineHeight;                    /* Line height of each row in number of lines. */
    int m_isRowDragDropAllowed;             /* Is row drag/drop allowed? */
    int m_dragIndex;                        /* Row index of row being dragged. */
    CImageList *m_dragImage;                /* Row drag image. */
    int m_isTrackingMouse;                  /* Is mouse tracking active? */
    int m_defaultSortColumn;                /* Default column to sort. */
    SortOrder_t m_defaultSortOrder;         /* Default sort order. */
    UINT32 m_lastResizeTime;                /* Time of last resize. */
    int m_prevListWidths[2];                /* Previous list widths. */
    int m_prevListHeights[2];               /* Previous list heights. */

    /* Callbacks. */
    RowSelectFn_t m_rowSelectFn;                /* Row selection function. */
    void *m_rowSelectFnArg;                     /* Row selection function user arg. */
    RowCheckboxFn_t m_rowCheckboxFn;            /* Row checkbox changed function. */
    void *m_rowCheckboxFnArg;                   /* Row checkbox changed function arg. */
    RowDragDropFn_t m_rowDragDropFn;            /* Row drag/drop function. */
    void *m_rowDragDropFnArg;                   /* Row drag/drop function arg. */
    PopupMenuCreateFn_t m_popupMenuCreateFn;    /* Popup menu create function. */
    PopupMenuSelectFn_t m_popupMenuSelectFn;    /* Popup menu selection function. */
    void *m_popupMenuFnArg;                     /* Popup menu user arg. */

    /* 
     * Private functions. 
     */

    int addColumnInternal(const char *name);
    SPxErrorCode setColumnSortOrderInternal(int column, SortOrder_t order);
    SPxErrorCode setSelectedRowInternal(int row, BOOL singleSelection, BOOL invokeCallbacks);
    int getSelectedRowInternal(void) const;
    int isRowSelectedInternal(int row) const;
    void getColumnDisplayName(int column, CString *str) const;
    int getAutoColumnWidth(const char *text, int isHeader=FALSE);
    void autoSizeLastColumn(void);
    int listToBufferColumn(int listColumn) const;
    void setRowChanged(int row, BOOL isDataChanged);
    void installImageList(void);
    void endDrag(CPoint point, int doMove);

    /* 
     * Private static functions. 
     */

    static bool compareRows(const SPxSharedPtr<Row_t>& r1,
                            const SPxSharedPtr<Row_t>& r2);
    static int compareCells(SortOrder_t sortOrder,
                            const Cell_t& r1Cell,
                            const Cell_t& r2Cell);
};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
