/*********************************************************************
*
* (c) Copyright 2015 - 2017, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for a re-usable dialog that acts as a header in the
*	SPxTableDlgWin.
*
*
* Revision Control:
*   21/08/18 v1.6    BTB	Add IsDialogShown().
*
* Previous Changes:
*   06/12/17 1.5    AGC	Fix last change on VS2010.
*   06/12/17 1.4    AGC	Update header font on DPI change.
*   03/03/16 1.3    AGC	Fix clang warnings.
*   26/11/15 1.2    AGC	Support control of another dialog.
*   16/11/15 1.1    AGC	Initial version.
**********************************************************************/
#pragma once

#include "SPxResource.h"

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

#include "SPxLibWin/SPxWinButton.h"

class SPxTableDlgWin;

class SPxTableHdrDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxTableHdrDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxTableHdrDlgWin(SPxTableDlgWin* pParent, CWnd *collapseDlg=NULL);
    virtual ~SPxTableHdrDlgWin(void);
    BOOL Create(void);

    void SetText(const char *text);

    void ShowDialog(int show);
    BOOL IsDialogShown(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_TABLE_HDR_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void) {}
    virtual void OnCancel(void) {}
    void OnSize(UINT nType, int cx, int cy);
    virtual void OnSetFont(CFont *pFont);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    SPxTableDlgWin *m_parentWin;
    CWnd *m_collapseDlg;

    CFont m_labelFont;
    CStatic m_label;
    CStatic m_line;
    SPxWinButton m_btnCollapse;

    /* Private functions. */
    void OnCollapse(void);

    using CDialog::Create;

}; 

/*********************************************************************
*
* End of file
*
**********************************************************************/
