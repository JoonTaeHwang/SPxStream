/*********************************************************************
*
* (c) Copyright 2017 - 2019, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for SPx help button dialog.
*
*
* Revision Control:
*   24/04/19 v1.4    AGC	Fix behaviour with min/max buttons.
*
* Previous Changes:
*   17/04/19 1.3    BTB	Allow button bitmap to be set externally.
*   28/11/17 1.2    AGC	Rescale bitmap on DPI change.
*   09/05/17 1.1    AGC	Initial version.
**********************************************************************/
#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* Forward declarations. */

/* Define our class. */
class SPxHelpBtnDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxHelpBtnDlgWin)

public:
    typedef void(*ShowHelp)(void *userArg, const char *suffix);

    /* Construction and destruction */
    explicit SPxHelpBtnDlgWin(CWnd* pParentWin,
			      ShowHelp showHelpFn,
			      void *showHelpUserArg,
			      const char *showHelpSuffix);
    virtual ~SPxHelpBtnDlgWin(void);
    virtual BOOL Create(void);

    /* Make sure to allocate bitmap object pointed to using new. */
    void SetBitmap(CBitmap *bitmap);

    /* Dialog data. */
    enum { IDD = IDD_SPX_HELP_BTN_DIALOG };

protected:
    /* Protected functions */
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void) {}
    virtual void OnCancel(void) {}
    void OnMouseMove(UINT nFlags, CPoint point);
    LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
    BOOL OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT message);
    void OnLButtonDown(UINT nFlags, CPoint point);
    void OnLButtonUp(UINT nFlags, CPoint point);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    void OnHelpClicked(void);
    void scaleBitmap(void);
    void reposition(void);
    static LRESULT CALLBACK hookFn(int code, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK hookKeyFn(int code, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK hookCbtFn(int code, WPARAM wParam, LPARAM lParam);
    int getTitleBarHeight(void) const;

    using CDialog::Create;

}; /* SPxHelpBtnDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
