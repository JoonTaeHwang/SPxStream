/*********************************************************************
 *
 * (c) Copyright 2016, 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   A pop-up dialog used to perform language selection.
 *
 * Revision Control:
 *   03/07/18 v1.3    BTB	Support csv files.
 *				Support UTF-8 or UTF-16 character encoding.
 *
 * Previous Changes:
 *   03/05/18 1.2   AGC	Add IsLoaded() function.
 *   29/04/16 1.1   SP 	Initial version..
 *
 **********************************************************************/

#pragma once

/* Library headers */
#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

/* Library headers. */
#include "SPxResource.h"
#include "SPxLibUtils/SPxObj.h"

enum SPxLangCharEncoding_t
{
    SPX_LANG_ENCODING_AUTO = 0,
    SPX_LANG_ENCODING_UTF8 = 1,
    SPX_LANG_ENCODING_UTF16 = 2

};

/*
 * SPxLanguageDlgWin dialog.
 */
class SPxLanguageDlgWin : public CDialog, public SPxObj
{
    DECLARE_DYNAMIC(SPxLanguageDlgWin)

public:

    /*
     * Public variables.
     */

    /* Dialog Data */
    enum { IDD = IDD_SPX_LANGUAGE_DIALOG };

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxLanguageDlgWin(CWnd *parentWin, HICON hIcon);
    virtual ~SPxLanguageDlgWin();

    /* General. */
    virtual SPxErrorCode LoadLanguages(const char *filename,
                                       const char *languageName,
				       SPxLangCharEncoding_t encoding = SPX_LANG_ENCODING_AUTO);
    virtual BOOL IsLoaded(void) const;
    virtual BOOL IsLanguageChanged(void) { return m_isLanguageChanged; }
    virtual const char *GetLanguageName(void);
    virtual void OnOK(void);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    /* General. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    HICON m_hIcon;
    BOOL m_isLanguageLoaded;
    BOOL m_isLanguageChanged;

    /* Dialog items. */
    CComboBox *m_combo;

    /*
     * Private functions.
     */

    SPxErrorCode parseFile(const char *filename, FILE *fd);
    SPxErrorCode setParams(const char *filename);

    /* GUI callback functions. */

}; /* Class SPxLanguageDlgWin */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
