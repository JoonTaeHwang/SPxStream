/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Popup multi-radar coverage map dialog.
 *
 * Revision Control:
 *  06/05/20 v1.3    AGC        Add SetTabTitle() functions.
 *
 * Previous Changes:
 *  30/04/20 1.2    AGC Add missing include.
 *  24/04/20 1.1    AGC	Initial version.
 *
 **********************************************************************/

#pragma once

/* Local headers. */
#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxTiledMapSrcCoverage.h"

/* Forward declarations. */
class SPxTiledMapDatabase;
class SPxTiledMapSrcCoverage;
class SPxTiledMapRenderer;

/* SPxRwCoverageMapDlg dialog. */
class SPxCoveragePopupDlgWin : public CDialog
{
public:
    explicit SPxCoveragePopupDlgWin(CWnd *parent,
                                    SPxTiledMapDatabase *db,
                                    SPxTiledMapSrcCoverage *src);
    virtual ~SPxCoveragePopupDlgWin();
    BOOL Create(void);

    virtual void SyncFromObj(void);

    void SetTabTitle(const char *tabTitle);
    void SetTabTitleObjects(SPxTabbedItemName *objects[]);

    SPxErrorCode AddRenderer(SPxTiledMapRenderer *renderer);

protected:
    DECLARE_MESSAGE_MAP()
    BOOL OnInitDialog();
    virtual void OnOK() {}

    /* Dialog Data. */
    enum { IDD = IDD_SPX_COVERAGE_POPUP_CTRL_DIALOG };

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    void coverageChange(UINT32 flags,
                        const SPxTiledMapSrcCoverage::VisInfo *visInfo,
                        int index,
                        const SPxTiledMapSrcCoverage::RadarInfo *radarInfo);

    using CDialog::Create;
};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
