/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for camera FOV calibration preview dialog.
*
*
* Revision Control:
*   07/08/19 v1.1   BTB	Initial version.
*
* Previous Changes:
*
**********************************************************************/

#pragma once

/* Standard headers. */

/* Library headers. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN

/* Forward declarations. */
class SPxCamera;
class SPxCamFOVCalDlgWin;

/*
 * SPxCamFOVCalPrevDlgWin dialog
 */
class SPxCamFOVCalPrevDlgWin : public CDialog
{
public:
    /* Constructor and destructor. */
    SPxCamFOVCalPrevDlgWin(SPxCamFOVCalDlgWin* parentWin);
    virtual ~SPxCamFOVCalPrevDlgWin(void);
    virtual BOOL Create(void);

    void SetToInitialState(void);

    void SetLUTFilename(const char *filename);
    const char *GetLUTFilename(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_CAM_FOV_CALIBRATION_PREVIEW_DIALOG };

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
    SPxCamFOVCalDlgWin *m_parentWin;

    /* Controls. */
    CButton m_previewCurrRadio;
    CButton m_previewNewRadio;
    CStatic m_lutFileLabel;
    CWnd m_lutFileEdit;

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
