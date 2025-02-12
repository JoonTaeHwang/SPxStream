/*********************************************************************
*
* (c) Copyright 2015, 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxP172RcCommandDlgWin.h,v $
* ID: $Id: SPxP172RcCommandDlgWin.h,v 1.3 2016/03/03 10:59:08 rew Exp $
*
* Purpose:
*   A pop-up dialog used to send advanced commands to the radar.
*
* Revision Control:
*   03/03/16 v1.3    AGC	Fix clang warnings.
*
* Previous Changes:
*   27/11/15 1.2    SP 	Handle OnOK() to send.
*   26/11/15 1.1    SP 	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* Include the classes we need. */
#include "SPxLibUtils/SPxCommon.h"

/* Forward declarations. */
class SPxP172RcInterfaceWin;

/*
 * SPxP172RcCommandDlgWin dialog.
 */
class SPxP172RcCommandDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxP172RcCommandDlgWin)

public:
    /* Constructor and destructor. */
    SPxP172RcCommandDlgWin(SPxP172RcInterfaceWin *rdrIf);
    virtual ~SPxP172RcCommandDlgWin(void);
    virtual BOOL Create(CWnd* pParent);

    /* Dialog Data */
    enum { IDD = IDD_SPX_P172_RC_COMMAND_DIALOG };

protected:
    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void);
    virtual afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
    /* Private types. */

    /* Request types. */
    typedef enum
    {
        REQ_TYPE_NONE = 0,
        REQ_TYPE_STATUS,
        REQ_TYPE_REL_SECTOR,
        REQ_TYPE_TRUE_SECTOR,
        REQ_TYPE_PROC_SECTOR,
        REQ_TYPE_TUNE,

    } RequestType_t;

    /* Private functions. */
    void HandleMesg(const SPxP172RcInterfaceWin::MesgInfo_t *mesg);

    /* Callback functions. */
    afx_msg void OnBnClickedSendButton();

    /* Private variables. */
    SPxP172RcInterfaceWin *m_interface;
    BOOL m_isInitialised;
    RequestType_t m_lastReqType;
    unsigned int m_lastReqID;

    /* Dialog items. */
    CWnd *m_cmdEdit;
    CWnd *m_replyEdit;
    CComboBox *m_destCombo;
    CButton *m_sendBn;

    /* Private static functions. */
    static int rxMesgTextHandler(void *invArg, 
                                 void *userArg, 
                                 void *mesgArg);

    using CDialog::Create;

}; /* class SPxP172RcCommandDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
