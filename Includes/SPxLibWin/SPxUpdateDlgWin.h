/*********************************************************************
*
* (c) Copyright 2014 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for generic SPx server check for updates dialog.
*
*
* Revision Control:
*   02/02/18 v1.6    AGC	Override OnOK().
*
* Previous Changes:
*   09/05/17 1.5    AGC	Perform initial check when window first shown.
*   03/03/16 1.4    AGC	Fix clang warnings.
*   05/02/15 1.3    AGC	Warn when closing dialog if download in progress.
*   15/09/14 1.2    AGC	Support Visual Studio version selection.
*   04/07/14 1.1    AGC	Initial version.
**********************************************************************/
#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxProduct enumeration. */
#include "SPxLibUtils/SPxInstall.h"

/* Forward declarations. */

/* Define our class. */
class SPxUpdateDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxUpdateDlgWin)

public:
    /* Construction and destruction */
    explicit SPxUpdateDlgWin(CWnd *pParentWin,
	SPxInstall::Product product,
	const char *name,
	int useLatestVersion=TRUE);
    virtual ~SPxUpdateDlgWin(void);
    virtual BOOL Create(void);

    int IsDownloadInProgress(void) const;

    /* Dialog data. */
    enum { IDD = IDD_SPX_UPDATE_DIALOG };

protected:
    /* Protected functions */
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual void OnOK(void) {};
    virtual void OnCancel(void);
    virtual BOOL OnInitDialog(void);
    void OnBnClickedShowAll(void);
    void OnBnClickedCheck(void);
    void OnBnClickedDownload(void);
    void OnBnClickedCancel(void);
    void OnBnClickedOpenFolder(void);
    void OnTimer(UINT_PTR nIDEvent);
    void OnShowWindow(BOOL bShow, UINT nStatus);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxErrorCode checkForUpdate(void);
    SPxErrorCode checkForUpdateComplete(void);
    SPxErrorCode downloadUpdate(void);
    SPxErrorCode downloadUpdateComplete(void);
    void setVersionLabel(void);
    void populateComboVS(int showAll);

    using CDialog::Create;

}; /* SPxUpdateDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
