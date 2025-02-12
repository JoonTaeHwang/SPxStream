/*********************************************************************
*
* (c) Copyright 2018 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Nav Data History child dialog.
*
*
* Revision Control:
*   01/02/19 v1.6    AGC	Fix N/C #661 - add option to show errors.
*
* Previous Changes:
*   02/01/19 1.5    REW	Support logging to file.
*   12/12/18 1.4    REW	Support freezing the history window.
*   11/12/18 1.3    REW	Show received messages in window.
*   06/12/18 1.2    REW	Avoid compiler warning.
*   05/12/18 1.1    REW	Initial Version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibData/SPxNavData.h"
#include "SPxLibWin/SPxWinMessageCtrl.h"

/* Forward declarations. */
class SPxNavData;
class SPxWinMessageCtrl;

/*
 * SPxNavHistoryDlgWin dialog
 */
class SPxNavHistoryDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxNavHistoryDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxNavHistoryDlgWin(CWnd* pParent, SPxNavData *obj,
				 int showErrors);
    virtual ~SPxNavHistoryDlgWin(void);
    virtual BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_NAV_HISTORY_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration retrieval functions. */
    SPxNavData *GetObj(void) { return m_obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    DECLARE_MESSAGE_MAP()

private:
    /*
     * Private variables.
     */
    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxNavData *m_obj;

    /* MFC controls. */
    SPxWinMessageCtrl m_msgWnd;		/* Window for messages */

    /* GUI data exchange. */
    int m_freezeWin;
    int m_logToFile;

    /*
     * Private functions
     */
    /* GUI functions. */
    void onFreeze();
    void onLogToFile();

    /* Callback handler. */
    static int staticMsgHandler(void *invokingObject, void *userObject,
					void *arg);
    int msgHandler(const char *msg);

    /* Avoid warnings about hiding overloaded virtual functions. */
    using CDialog::Create;
}; /* class SPxNavHistoryDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
