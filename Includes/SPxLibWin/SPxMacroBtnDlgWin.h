/*********************************************************************
*
* (c) Copyright 2020, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for macro button container dialog.
*
*
* Revision Control:
*   07/10/22 v1.6    AGC	Add CheckState().
*
* Previous Changes:
*   27/05/20 1.5    BTB	Support different text colour for disabled buttons.
*   27/05/20 1.4    BTB	Support configuring font name and size.
*   13/05/20 1.3    REW	Support GetNeededSize().
*   03/04/20 1.2    AGC	Derive from SPxWinCustomDlg.
*			Add border size and max cells options.
*   17/03/20 1.1    AGC	Initial version.
**********************************************************************/
#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/* For base class. */
#include "SPxLibWin/SPxWinCustomDlg.h"

/* Forward declarations. */

/* Define our class. */
class SPxMacroBtnDlgWin : public SPxWinCustomDlg
{
    DECLARE_DYNAMIC(SPxMacroBtnDlgWin)

public:
    enum Orientation
    {
        ORIENT_HORZ = 0,
        ORIENT_VERT = 1
    };

    /* Construction and destruction */
    explicit SPxMacroBtnDlgWin(CWnd* pParentWin,
                               const char *nameFormat);
    virtual ~SPxMacroBtnDlgWin(void);
    virtual BOOL Create(void);

    SPxErrorCode SetOrientation(Orientation orient);
    Orientation GetOrientation(void) const;
    void SetBorderSize(int borderSize);
    void SetMaxSize(int maxSize);
    void SetMaxNumCells(int maxNumCells);
    void SetSaveUpdateOnly(int saveUpdateOnly);

    void EnableCustomColours(int enable);
    void SetColourNormal(COLORREF colour);
    void SetColourDisabled(COLORREF colour);
    void SetColourPressed(COLORREF colour);
    void SetColourHot(COLORREF colour);
    void SetColourBorder(COLORREF colour);
    void SetColourText(COLORREF colour);
    void SetColourTextDisabled(COLORREF colour);
    void ReplaceBitmapColour(COLORREF oldColour, COLORREF newColour);

    void SetCustomFont(const char *fontName, int fontSize);

    virtual void SyncFromObj(void) { Sync(); }
    void Sync(void);
    void CheckState(void);

    void Layout(void);
    int GetNeededSize(void) const;

    /* Dialog data. */
    enum { IDD = IDD_SPX_MACRO_BTN_DIALOG };

protected:
    /* Protected functions */
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void) {}
    virtual void OnCancel(void) {}
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    using CDialog::Create;

}; /* SPxMacroBtnDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
