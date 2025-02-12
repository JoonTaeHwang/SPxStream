/*********************************************************************
*
* (c) Copyright 2014 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for re-usable range rings configuration dialog.
*
*
* Revision Control:
*   22/10/21 v1.6    AGC	Suppress spurious cppcheck warning.
*
* Previous Changes:
*   21/10/21 1.5    AGC	Rename and move from simulator to library.
*   23/01/18 1.4    AGC	Support multiple objects.
*   31/01/17 1.3    AGC	Support units configuration.
*   20/04/16 1.2    AGC	Fix clang warnings.
*   02/06/14 1.1    AGC	Initial Version.
**********************************************************************/
#pragma once

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* For base class. */
#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxGraphicsRenderer;

/*
 * SPxRangeRingsCfgDlgWin dialog
 */
class SPxRangeRingsCfgDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxRangeRingsCfgDlgWin)

public:
    explicit SPxRangeRingsCfgDlgWin(CWnd* pParent,
				 unsigned int numObjects,
				 SPxGraphicsRenderer *obj[]);
    virtual ~SPxRangeRingsCfgDlgWin();
    BOOL Create(void);
    int GetIDD(void) const { return IDD; }

    /* Sync functions. */
    void SyncFromObj(void);

    void SetDisplayUnits(SPxUnitsDist_t unitsDist, int index=-1);

    // cppcheck-suppress virtualCallInConstructor
    virtual void SetObjIndex(int index);

    /* Dialog data */
    enum { IDD = IDD_SPX_RANGE_RINGS_CFG_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    void OnOK(void) {};
    DECLARE_MESSAGE_MAP()

private:
    /* The underlying objects. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    CWnd *m_parentWin;
    SPxGraphicsRenderer *m_obj;
    SPxUnitsDist_t m_distUnits;

    /* MFC Controls. */
    SPxWinSpinCtrl m_rrSpinNum;
    SPxWinSpinCtrl m_rrSpinStart;
    SPxWinSpinCtrl m_rrSpinSpacing;
    CButton m_rrCheckShowLabels;
    CStatic m_rrLblStart;
    CStatic m_rrLblSpacing;

    void OnRRSpinNum(NMHDR*, LRESULT*);
    void OnRRSpinStart(NMHDR*, LRESULT*);
    void OnRRSpinSpacing(NMHDR*, LRESULT*);
    void OnRRCheckShowLbl(void);
    void setRangeRings(void);

    using CDialog::Create;

}; /* SPxRangeRingsCfgDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
