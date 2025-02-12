/*********************************************************************
*
* (c) Copyright 2016, 2019 -  2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Dialog class for the Target Table window.
*
* Revision Control:
*   05/04/22 v1.32   AGC	Add optional search box.
*
* Previous Changes:
*   25/11/21 1.31   AGC	Support setting default sort order.
*   31/03/21 1.30   AGC	Add comments to OnCancel().
*   09/11/20 1.29   AGC	Add height mode option.
*   30/10/20 1.28   AGC	Add legacy format option.
*   15/10/20 1.27   SP 	Support unified track filters.
*   29/09/20 1.26   AJH	Add column visible.
*   24/09/20 1.25   AJH	Add OnCancel().
*   23/09/20 1.24   AGC	Remove RefreshTable forceRedraw option.
*   18/09/20 1.23   AJH	Add column padding/align.
*			Support track inclusion callback.
*   11/09/20 1.22   SP 	Support display of lost tracks.
*			Add track colour callback for SPxUniTrack.
*   10/09/20 1.21   AJH	Support column auto-resizing.
*   10/09/20 1.20   AGC	Update header font on DPI changed.
*   09/09/20 1.19   AJH	Allow row indicators and label above table to be suppressed.
*   13/08/20 1.18   SP 	Support forced refresh.
*   05/08/20 1.17   SP 	Support unified track database.
*   06/05/20 1.16   AGC Add missing context menu support changes.
*   06/05/20 1.15   AGC Support context menu.
*   30/04/20 1.14   AGC Set num columns based on parameters.
*   29/04/20 1.13   AGC Add DeselectRows() function.
*   08/04/20 1.12   AGC Support disabled colour.
*   03/04/20 1.11   AGC Support multiple columns.
*   27/03/20 1.10   AGC Fix warning.
*   24/03/20 1.9    AGC Convert to reusable library dialog.
*   23/03/20 1.8    AGC Default to child dialog.
*                       Support font size configuration.
*   26/09/19 1.7    AGC	Move track label formatting to main dialog.
*   10/10/16 1.6    REW	Rename to Track Table (not Target Table).
*			Catch show/hide and update PPI ctrl button.
*   07/10/16 1.5    AGC	Reduce flicker.
*			Show horizontal scroll bars when appropriate.
*			Preserve display position on refresh.
*   26/09/16 1.4    REW	Show number of tracks as status label.
*			Show colour indicator for each track.
*   23/09/16 1.3    REW	Set window icon.
*   20/09/16 1.2    REW	Complete initial implementation.
*   16/09/16 1.1    REW	Initial Version.
**********************************************************************/

#pragma once
#include "afxcmn.h"

/*
 * Includes.
 */
#include "SPxResource.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibWin/SPxWinButton.h"
#include "SPxLibWin/SPxWinListCtrl.h"
#include "SPxLibWin/SPxWinCustomDlg.h"

/*
 * Constants.
 */

/*
 * Classes.
 */

/* Forward declarations. */
class SPxTrackTableDlgWin;
class SPxThreatDatabase;
class SPxThreat;
class SPxUniTrackDatabase;
class SPxUniTrack;
class SPxBitmapWin;

/* SPxTrackTableDlgWin dialog */
class SPxTrackTableDlgWin : public SPxWinCustomDlg, public SPxObj
{
    DECLARE_DYNAMIC(SPxTrackTableDlgWin)

public:

    /*
     * Public types.
     */
    typedef SPxErrorCode(*GetRefPointCallback)(double *refLatDegs,
                                               double *refLongDegs,
                                               void *userArg);
    typedef SPxErrorCode(*TrackSelectCallback)(UINT32 trackID,
                                               int doubleClick,
                                               void *userArg);
    typedef UINT32(*GetTrackColourCallback)(SPxUniTrack *track,
                                             void *userArg);
    typedef UINT32(*GetThreatColourCallback)(SPxThreat *threat,
                                             void *userArg);
    typedef int(*IsThreatAlarmCallback)(SPxThreat *threat,
                                        void *userArg);
    typedef void (*ContextMenuCreateCallback)(SPxTrackTableDlgWin *dlg,
                                              int row,
                                              CMenu *menu,
                                              void *userArg);
    typedef void (*ContextMenuSelectCallback)(SPxTrackTableDlgWin *dlg,
                                              int row,
                                              int selection,
                                              void *userArg);
    typedef int(*GetTrackIncludeCallback)(SPxUniTrack *track,
					      void *userArg);

    /*
     * Public variables.
     */

    /* Dialog Data. */
    enum { IDD = IDD_SPX_TRACK_TABLE_DIALOG };

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    explicit SPxTrackTableDlgWin(SPxThreatDatabase *threatDB);
    explicit SPxTrackTableDlgWin(SPxUniTrackDatabase *uniTrackDB);
    virtual ~SPxTrackTableDlgWin(void);
    virtual BOOL Create(CWnd* pParent);

    void SetWinMenu(SPxWinMenu *menu);

    /* Options. */
    SPxErrorCode SetUseLegacyFormat(int useLegacyFormat);
    int GetUseLegacyFormat(void) const;
    SPxErrorCode SetHeader(const char *str, unsigned int col);
    const char *GetHeader(unsigned int col) const;
    SPxErrorCode SetFormatString(const char *str, unsigned int col=0);
    const char *GetFormatString(unsigned int col = 0) const;
    SPxErrorCode SetFontSize(unsigned int fontSize);
    unsigned int GetFontSize(void) const;
    SPxErrorCode SetFormatLatLong(SPxFormatLatLong_t formatLatLong);
    SPxErrorCode SetUnitsDist(SPxUnitsDist_t unitsDist);
    SPxErrorCode SetUnitsHeight(SPxUnitsHeight_t unitsHeight);
    SPxErrorCode SetUnitsSpeed(SPxUnitsSpeed_t unitsSpeed);
    unsigned int GetNumCols(void) const;
    SPxErrorCode SetRowIndicatorEnabled(int enabled);
    SPxErrorCode SetLabelEnabled(int enabled);
    SPxErrorCode SetSearchEnabled(int enabled);
    SPxErrorCode SetAutoResizeEnabled(int enabled);
    SPxErrorCode SetColumnPadding(int column, int padding);
    SPxErrorCode SetColumnAlign(int column, SPxWinListCtrl::Align_t align);
    SPxErrorCode SetColumnVisible(int column, BOOL isVisible);
    SPxErrorCode SetDefaultSortOrder(int column, SPxWinListCtrl::SortOrder_t order);
    SPxErrorCode SetTrackFiltersEnabled(int isEnabled);
    int AreTrackFiltersEnabled(void) const;
    SPxErrorCode SetHeightMode(SPxHeightMode_t heightMode);
    SPxHeightMode_t GetHeightMode(void) const;

    /* Callbacks. */
    SPxErrorCode SetRefPointCallback(GetRefPointCallback getRefPointFn, void *userArg);
    SPxErrorCode SetTrackSelectCallback(TrackSelectCallback trackSelectFn, void *userArg);
    SPxErrorCode SetTrackColourCallback(GetTrackColourCallback getTrackColourFn, void *userArg);
    SPxErrorCode SetThreatColourCallback(GetThreatColourCallback getThreatColourFn, void *userArg);
    SPxErrorCode SetIsThreatAlarmCallback(IsThreatAlarmCallback isThreatAlarmFn, void *userArg);
    SPxErrorCode SetContextMenuCreateCallback(ContextMenuCreateCallback contextMenuCreateFn, void *userArg);
    SPxErrorCode SetContextMenuSelectCallback(ContextMenuSelectCallback contextMenuSelectFn, void *userArg);
    SPxErrorCode SetTrackIncludeCallback(GetTrackIncludeCallback getTrackIncludeFn, void *userArg);

    /* Update the GUI. */
    SPxErrorCode RefreshTable(void);

    /* Custom colours. */
    void EnableCustomColours(int enable);
    void SetColourNormal(COLORREF colour);
    void SetColourDisabled(COLORREF colour);
    void SetColourPressed(COLORREF colour);
    void SetColourBorder(COLORREF colour);
    void SetColourText(COLORREF colour);
    void SetColourLostTrackText(COLORREF colour);
    COLORREF GetColourNormal(void) const;
    COLORREF GetColourDisabled(void) const;
    COLORREF GetColourPressed(void) const;
    COLORREF GetColourBorder(void) const;
    COLORREF GetColourText(void) const;
    COLORREF GetColourLostTrackText(void) const;

    UINT32 GetTrackID(int row) const;
    void DeselectRows(void);

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
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC *pDC);
    afx_msg void OnPaint(void);
    void OnDpiChangedAfterParent(void);
    afx_msg void OnFilter(void);
    afx_msg void OnSearch(void);
    afx_msg void OnListBoxSelChanged(void);
    afx_msg void OnListBoxDoubleClick(void);
    virtual void OnCancel(void);

    /* Lay out for current size etc. */
    virtual void ReLayout(void);

    /* Parameter settings. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /*
     * Private functions.
     */

    void initCommon(void);

    /* Build string for target information. */
    SPxErrorCode buildTrackInfo(char *buf, int bufLen,
				const SPxUniTrack *track,
                                const char *formatString);

    /* Callback to iterate database and add tracks to list. */
    static int staticAddTrackToList(void *invokingObj, void *userObj,
					void *arg);
    int addTrackToList(SPxUniTrack *track, SPxThreat *threat);
    static void staticRowSelect(void *arg, int row,
                                SPxWinListCtrl::SelectState_t state);
    void showHideHeader(void);

    static void popupMenuCreate(void *arg, int row, CMenu *menu);
    static void popupMenuSelect(void *arg, int row, int selection);

    using CDialog::Create;

}; /* Class SPxTrackTableDlgWin. */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
