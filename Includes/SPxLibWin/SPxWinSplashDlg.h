/*********************************************************************
 * (c) Copyright 2014, 2016, 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header file for re-usable splash dialog class.
 *
 * Revision Control:
 *  05/04/22 v1.4    SP 	Support transparent labels.
 *
 * Previous Changes:
 *  04/07/18 1.3    BTB	Add SetImageFilename().
 *  03/03/16 1.2    AGC	Fix clang warnings.
 *  20/06/14 1.1    AGC	Initial version based on
 *			SPxRadarSimulator splash screen.
 **********************************************************************/

#pragma once

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

#include "SPxResource.h"

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibWin/SPxWinImageCtrl.h"

class SPxWinSplashDlg : public CDialog
{
    DECLARE_DYNAMIC(SPxWinSplashDlg)

public:
    // Methods
    explicit SPxWinSplashDlg(CWnd* pParent=NULL);
    virtual ~SPxWinSplashDlg(void);
    BOOL Create(void);
    SPxErrorCode SetImageFilename(const char *filename);
    void SetBitmap(int id);
    void SetBorder(int border);
    void SetBackgroundColour(COLORREF col);
    void SetTextColour(COLORREF col);
    void SetProgressTextColour(COLORREF col);
    void SetBackgroundTextColour(COLORREF col);
    void SetBackgroundTextTransparent(int enable);
    void SetName(const char *name, ...);
    void SetLicense(const char *license, ...);
    void SetText(const char *txt, ...);
    void SetProgress(int val);
    void IncProgress(int delta);

    // Variables
    enum { IDD = IDD_SPX_SPLASH_DIALOG };

protected:
    // Methods
    virtual BOOL OnInitDialog();
    HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    void DoDataExchange(CDataExchange *pDX);
    DECLARE_MESSAGE_MAP()

private:
    CWnd *m_parentWin;
    SPxAutoPtr<CBrush> m_brush;
    int m_textBgTransparent;
    COLORREF m_textCol;
    COLORREF m_progressTextCol;
    SPxWinImageCtrl m_imgCtrl;
    CStatic m_lblName;
    CStatic m_lblLicense;
    CStatic m_lblProgress;
    CProgressCtrl m_progress;

    using CDialog::Create;

}; // SPxRadarSimSplashDlg

/*********************************************************************
 * End of File
 **********************************************************************/
