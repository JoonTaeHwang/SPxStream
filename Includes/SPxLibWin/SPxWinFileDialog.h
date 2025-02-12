/*********************************************************************
*
* (c) Copyright 2014, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for Windows File Dialog class.
*
*   At the moment, this is a thin wrapper around CFileDialog
*   that forces the old-style of file dialog, as the new style
*   can cause issues with custom buttons and multi-file selection.
*
*
* Revision Control:
*   17/09/20 v1.3    AGC	Support opening a folder.
*
* Previous Changes:
*   20/06/14 1.2    AGC	Support adding check buttons.
*   06/06/14 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_WIN_FILE_DIALOG_H_
#define	_SPX_WIN_FILE_DIALOG_H_

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

#include "SPxLibUtils/SPxAutoPtr.h"

class SPxWinFileDialog : public CFileDialog
{
    DECLARE_DYNAMIC(SPxWinFileDialog)

public:
    explicit SPxWinFileDialog(BOOL bOpenFileDialog,
			      LPCTSTR lpszDefExt = NULL,
			      LPCTSTR lpszFileName = NULL,
			      DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			      LPCTSTR lpszFilter = NULL,
			      CWnd* pParentWnd = NULL,
			      DWORD dwSize = 0,
			      BOOL bVistaStyle = TRUE);
    virtual ~SPxWinFileDialog(void);

    void SetTitle(LPCTSTR title);
    void SetCheckTitle(LPCTSTR checkTitle);
    int AddCheckButton(LPCTSTR label, int checked=FALSE);
    int IsChecked(int id);

    void SetPickFolders(int pickFolders);

    virtual INT_PTR DoModal(void);

protected:
    virtual BOOL OnInitDialog(void);
    virtual void OnFileNameChange(void);
    DECLARE_MESSAGE_MAP()

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    void OnBnClicked(UINT nID);
    int isNewStyle(void);
    void customiseOldStyle(void);
    void customiseNewStyle(void);
};

#endif /* _SPX_WIN_FILE_DIALOG_H_ */
/*********************************************************************
*
* End of file
*
**********************************************************************/
