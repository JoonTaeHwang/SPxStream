/*********************************************************************
*
* (c) Copyright 2013 - 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxConfigObjDlgWin.h,v $
* ID: $Id: SPxConfigObjDlgWin.h,v 1.8 2016/03/03 10:59:08 rew Exp $
*
* Purpose:
*   A class for displaying one or more configuration dialogs.
*
* Revision Control:
*   03/03/16 v1.7    AGC	Fix clang warnings.
*
* Previous Changes:
*   05/02/15 1.6    AGC	Replace SPxConfigObjPopupDlg with SPxTabbedItemDlg.
*   07/01/15 1.5    AGC	Support push buttons.
*   26/03/14 1.4    AGC	Add new form for constructor.
*   22/11/13 1.3    AGC	Require popup dialogs to derive from SPxConfigObjPopupDlg.
*   18/11/13 1.2    AGC	Support format string for SetLabel().
*   21/10/13 1.1    AGC	Initial version.
*
**********************************************************************/

#pragma once

#define SPX_AFXWIN
#include "SPxResource.h"
#include "SPxLibUtils/SPxCommon.h"

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxError.h"

/* Forward declarations. */
class SPxServerDlgWin;
class SPxObj;
class SPxTabbedItemDlg;

/* Define our class. */
class SPxConfigObjDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxConfigObjDlgWin)

public:
    /* Public functions. */

    /* Construction and destruction. */
    SPxConfigObjDlgWin(SPxServerDlgWin *parent, const char *title=NULL);
    SPxConfigObjDlgWin(CWnd *parent=NULL, const char *title=NULL);
    virtual ~SPxConfigObjDlgWin(void);
    BOOL Create(void);

    void Sync(void);
    SPxErrorCode Add(unsigned int index, const char *name, SPxObj *checkObj=NULL,
	SPxTabbedItemDlg *dialog=NULL, const char *cfgText=NULL,
	int pushNotCheck=FALSE);
    SPxErrorCode SetLabel(unsigned int index, const char *format, ...);

    enum { IDD = IDD_SPX_CONFIG_OBJ_DIALOG };

protected:
    /* Protected functions. */
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    void OnTimer(UINT_PTR nIDEvent);
    void OnSize(UINT nType, int cx, int cy);
    virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
    BOOL OnNeedToolTip(UINT id, NMHDR *pTTTStruct, LRESULT *pResult);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    void OnBnClickedCheck(UINT nID);
    void OnBnClickedBtn(UINT nID);
    void layout(void);

    using CDialog::Create;

}; /* SPxConfigObjDlgWin. */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
