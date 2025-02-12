/*********************************************************************
*
* (c) Copyright 2013, 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxRendererDlgWin.h,v $
* ID: $Id: SPxRendererDlgWin.h,v 1.2 2016/03/03 10:59:08 rew Exp $
*
* Purpose:
*   A class for displaying one or more renderers in a window.
*
* Revision Control:
*   03/03/16 v1.2    AGC	Fix clang warnings.
*
* Previous Changes:
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
class SPxRendererWin;
class SPxViewControl;

/* Define our class. */
class SPxRendererDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxRendererDlgWin)

public:
    /* Public functions. */

    /* Construction and destruction. */
    SPxRendererDlgWin(CWnd *parent=NULL, UINT32 intervalMS=1000);
    virtual ~SPxRendererDlgWin(void);
    BOOL Create(void);

    SPxErrorCode SetViewControl(SPxViewControl *viewCtrl);
    SPxErrorCode Add(SPxRendererWin *renderer);

    enum { IDD = IDD_SPX_RENDERER_DIALOG };

protected:
    /* Protected functions. */
    virtual BOOL OnInitDialog(void);
    void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    CWnd *m_parent;
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    void draw(void);
    static int viewCtrlWrapper(void *invokingObject,
			       void *userObject,
			       void *arg);
    void viewCtrl(UINT32 flags);

    using CDialog::Create;

}; /* SPxRendererDlgWin. */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
