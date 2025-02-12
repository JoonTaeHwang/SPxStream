/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for shared feature control dialog.
*
*
* Revision Control:
*   10/12/18 v1.2    BTB	Make reload functionality optional.
*
* Previous Changes:
*   09/11/18 1.1    BTB	Initial version.
*
**********************************************************************/

#pragma once

/* Standard headers. */

/* Library headers. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN

/*
 * SPxSharedFeatureDlgWin dialog
 */
class SPxSharedFeatureDlgWin : public CDialog
{
public:
    /* Handler for gaining or releasing master control. */
    typedef void (*MasterChangeFn_t)(unsigned int id, int isMaster, void *userArg);
    /* Handler for requesting reload. */
    typedef void (*RequestReloadFn_t)(unsigned int id, void *userArg);

    /* Constructor and destructor. */
    explicit SPxSharedFeatureDlgWin(CWnd* parentWin,
				    unsigned int id,
				    unsigned int userID,
				    const char *userName,
				    int reloadEnabled = TRUE);
    virtual ~SPxSharedFeatureDlgWin(void);
    virtual BOOL Create(void);

    void AddFeature(const char *fileName);

    /* Master change handler. */
    void SetMasterChangeHandler(MasterChangeFn_t fn, void *userArg);
    /* Request reload handler. */
    void SetRequestReloadHandler(RequestReloadFn_t fn, void *userArg);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SHARED_FEATURE_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ }
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ }
    DECLARE_MESSAGE_MAP()

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* The parent window. */
    CWnd *m_parentWin;

    unsigned int m_id;
    int m_isMaster;
    int m_reloadEnabled;

    /* Controls. */
    CStatic m_masterLabel;
    CStatic m_masterText;
    CButton m_masterBn;
    CButton m_reloadBn;

    /* Master change function. */
    MasterChangeFn_t m_masterChangeFn;
    void *m_masterChangeUserArg;

    /* Request reload function. */
    RequestReloadFn_t m_requestReloadFn;
    void *m_requestReloadUserArg;

    /* Private functions. */
    void updateControls(void);

    /* GUI callback functions. */
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnOnBnClickedMasterButton(void);
    afx_msg void OnOnBnClickedReloadButton(void);

    using CDialog::Create;

};

/*********************************************************************
*
* End of file
*
**********************************************************************/
