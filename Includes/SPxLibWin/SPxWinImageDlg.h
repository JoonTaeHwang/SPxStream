/*********************************************************************
*
* (c) Copyright 2018, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for popup dialog to display a scaled image loaded from
*	a file or resource.
*
*
* Revision Control:
*   27/05/20 v1.2    REW	Add SetImage().
*
* Previous Changes:
*   05/02/18 1.1    AGC	Initial version.
**********************************************************************/
#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxWinImageCtrl. */
#include "SPxLibWin/SPxWinImageCtrl.h"

/* Forward declarations. */
class SPxImage;

/* Define our class. */
class SPxWinImageDlg : public CDialog
{
    DECLARE_DYNAMIC(SPxWinImageDlg)

public:
    /* Construction and destruction */
    explicit SPxWinImageDlg(CWnd *pParentWin);
    virtual ~SPxWinImageDlg(void);
    virtual BOOL Create(void);

    SPxErrorCode SetFilename(const char *filename);
    SPxErrorCode SetFilename(const CString &filename);
    SPxErrorCode GetFilename(char *buffer, int bufLen) const;
    SPxErrorCode GetFilename(CString &filename) const;
    SPxErrorCode SetBitmap(HBITMAP bitmap);
    SPxErrorCode SetBitmap(int resourceID);
    SPxErrorCode SetImage(SPxImage *image);
    SPxErrorCode SetBackColour(COLORREF colour);
    UINT32 GetBackColour(void) const;
    SPxErrorCode SetBorder(int border);
    int GetBorder(void) const;
    SPxErrorCode SetMaintainAspectRatio(int maintainAspectRatio);
    int IsMaintainingAspectRatio(void) const;
    SPxErrorCode SetClickable(int clickable);
    int IsClickable(void) const;

    /* Dialog data. */
    enum { IDD = IDD_SPX_WIN_IMAGE_DIALOG };

protected:
    /* Protected functions */
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    void OnSize(UINT nType, int cx, int cy);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    CWnd *m_parent;
    SPxWinImageCtrl m_imgCtrl;

    using CDialog::Create;

}; /* SPxWinImageDlg */

/*********************************************************************
*
* End of file
*
**********************************************************************/
