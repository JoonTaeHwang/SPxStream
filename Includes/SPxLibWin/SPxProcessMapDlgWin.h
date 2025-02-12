/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the SPxProcessMap child dialog.
*
*
* Revision Control:
*   07/11/18 v1.3    AGC	High DPI improvements.
*
* Previous Changes:
*   08/02/18 1.2    AGC	Fix Unicode issues.
*   07/02/18 1.1    REW	Initial Version.
**********************************************************************/

#pragma once

/* Library headers. */
#include "SPxLibUtils/SPxError.h"

/* Include the SPx resource header */
#include "SPxResource.h"

/* Forward declarations. */
class SPxObj;
class SPxRIB;
class SPxPIM;
class SPxRunProcess;

/*
 * SPxProcessMapDlgWin class.
 */
class SPxProcessMapDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxProcessMapDlgWin)

public:
    /* Constructor and destructor. */
    SPxProcessMapDlgWin(CWnd* pParent);
    virtual ~SPxProcessMapDlgWin();
    BOOL Create(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_PROCESS_MAP_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual void OnTimer(UINT_PTR nIDEvent);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    virtual void OnSize(UINT nType, int cx, int cy);
    virtual void OnDpiChanged(WPARAM wParam, LPARAM lParam);
    virtual void OnPaint(void);
    virtual void OnLButtonDown(UINT nFlags, CPoint point);
    virtual void OnRButtonDown(UINT nFlags, CPoint point);
    virtual void OnBnClickedDoneProcessMap(void);
    virtual void OnBnClickedMakeInactive(void);
    virtual void OnBnClickedMakeActive(void);
    DECLARE_MESSAGE_MAP()

private:
    /* A displayed box has a geometry and an SPx object that it represents. */
    struct DisplayBox
    {
	SPxObj *obj;
	int x;
	int y;
	int w;
	int h;
    };

    /* The parent window */
    CWnd *m_parentWin;

    /* Control fields. */
    SPxRIB **m_ribList;
    int m_numRibs;
    int m_boxW;
    int m_boxH;
    int m_spaceV;
    int m_spaceH;
    DisplayBox *m_boxList;
    int m_numAllocatedBoxes;
    int m_numBoxes;
    SPxObj *m_highlightSPxObjectL;
    SPxObj *m_highlightSPxObjectR;
    SPxRunProcess *m_highlightRunProcess;
    int m_highlightRunProcessX;
    int m_highlightRunProcessY;

    /* Drawing objects etc. */
    CBrush *m_backgroundBrush;
    CBrush *m_processBrush;
    CBrush *m_processBrushParameters;
    CBrush *m_pimBrush;
    CBrush *m_ribBrush;
    CBrush *m_ribBufferBrush1;
    CBrush *m_ribBufferBrush2;
    CBrush *m_ribBufferBrush3;
    CPen *m_processPen;
    CPen *m_penInActive;
    CPen *m_processPenHighlight;
    CPen *m_blackPen;
    CPen *m_connectPen;
    CFont m_labelFont;
    CPen *m_labelPen;

    /* Private functions */
    void redraw(void);
    void drawRIB(CDC *dc, SPxRIB *rib, int x, int y, int *w);
    void drawPIM(CDC *dc, SPxPIM *pim, int x, int y, int *w);
    void drawProcessList(CDC *dc, SPxRunProcess *rp, int x, int y, int *height);
    void drawProcess(CDC *dc, SPxRunProcess *rp, int x, int y, int *w);
    void drawConnect(CDC *dc, int x1, int y1, int x2, int y2);
    void addBox(int x, int y, int w, int h, SPxObj* obj);
    void drawLabel(CDC *dc, const char *str, int x, int y);
    void showParameterPopup(CDC *dc, SPxRunProcess *rp);
    int dpiScale(int val);

    using CDialog::Create;
}; /* class SPxProcessMapDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
