/*********************************************************************
*
* (c) Copyright 2014, 2016, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for generic SPx parameter control dialog.
*
*
* Revision Control:
*   05/12/19 v1.4    BTB	Fix build error.
*
* Previous Changes:
*   04/12/19 1.3    BTB	Exclude language parameters from list.
*   03/03/16 1.2    AGC	Fix clang warnings.
*   22/10/14 1.1    AGC	Initial version.
**********************************************************************/
#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibWin/SPxWinEditCtrl.h"

/* Forward declarations. */
struct SPxStringList;

/* Define our class. */
class SPxParamDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxParamDlgWin)

public:
    /* Construction and destruction */
    explicit SPxParamDlgWin(CWnd* pParentWin);
    virtual ~SPxParamDlgWin(void);
    virtual BOOL Create(void);

    /* Dialog data. */
    enum { IDD = IDD_SPX_PARAM_DIALOG };

protected:
    /* Protected functions */
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void) {};
    HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    void OnComboParamTextChange(void);
    void OnBtnClickedSet(void);
    void OnBtnClickedGet(void);

    void populateParameters(void);

    void checkAddParameter(SPxStringList *param);

    using CDialog::Create;

}; /* SPxParamDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
