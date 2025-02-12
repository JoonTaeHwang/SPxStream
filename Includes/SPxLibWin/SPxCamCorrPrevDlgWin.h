/*********************************************************************
*
* (c) Copyright 2018 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for camera correction preview dialog.
*
*
* Revision Control:
*   07/08/19 v1.3    BTB	Minor tidy up.
*
* Previous Changes:
*   23/08/18 1.2    BTB	Make sure initial state gets set properly.
*   17/08/18 1.1    BTB	Initial version.
*
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN

#include "SPxLibWin/SPxWinSpinCtrl.h"

/* Forward declarations. */
class SPxCamCorrDlgWin;

/*
 * SPxCamCorrPrevDlgWin dialog
 */
class SPxCamCorrPrevDlgWin : public CDialog
{
public:
    /* Constructor and destructor. */
    SPxCamCorrPrevDlgWin(SPxCamCorrDlgWin* parentWin);
    virtual ~SPxCamCorrPrevDlgWin(void);
    virtual BOOL Create(void);
    void SetToInitialState(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_CAM_CORRECTION_PREVIEW_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ }
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ }
    DECLARE_MESSAGE_MAP()

private:
    /* The parent window. */
    SPxCamCorrDlgWin *m_parentWin;

    /* Controls. */
    CButton m_previewCurrRadio;
    CButton m_previewNewRadio;

    /* GUI callback functions. */
    afx_msg void OnBnClickedPreviewCurrentButton(void);
    afx_msg void OnBnClickedPreviewNewButton(void);

    using CDialog::Create;

};

/*********************************************************************
*
* End of file
*
**********************************************************************/
