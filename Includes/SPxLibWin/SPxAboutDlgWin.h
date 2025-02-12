/*********************************************************************
*
* (c) Copyright 2013 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for generic SPx server about dialog.
*
*
* Revision Control:
*   12/09/19 v1.8    AGC	Support display of SRT licenses.
*
* Previous Changes:
*   09/04/19 1.7    AGC	Improve per-monitor high DPI support.
*   28/11/17 1.6    AGC	Relayout on DPI triggered size change.
*   03/03/16 1.5    AGC	Fix clang warnings.
*   04/07/14 1.4    AGC	Remove unused SPxCriticalSection header.
*   26/06/14 1.3    AGC	Use appBit instead of SPxServerBase.
*			Support start year.
*   20/06/14 1.2    AGC	Support showing OSM copyright.
*   21/10/13 1.1    AGC	Initial version.
**********************************************************************/
#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* Forward declarations. */

/* Define our class. */
class SPxAboutDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxAboutDlgWin)

public:
    enum Licenses
    {
	LIC_NONE = 0x00,
	LIC_OSM = 0x01,
	LIC_SRT = 0x02
    };

    /* Construction and destruction */
    SPxAboutDlgWin(CWnd* pParentWin,
	UINT32 appBit,
	const char *appName,
	const char *titleExtra=NULL,
	int showCP=TRUE,
	UINT32 licenses=LIC_OSM,
	int startYear=0);
    virtual ~SPxAboutDlgWin(void);
    virtual BOOL Create(void);

    void SetAppIcon(int iconID);

    /* Dialog data. */
    enum { IDD = IDD_SPX_ABOUT_DIALOG };

protected:
    /* Protected functions */
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    void OnSize(UINT nType, int cx, int cy);
    void OnDpiChanged(WPARAM, LPARAM);
    void OnBnClickedEmail(void);
    void OnBnClickedWebsite(void);
    void OnLink(NMHDR *nmhdr, LRESULT *lresult);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    void updateAppIcon(void);

    using CDialog::Create;

}; /* SPxAboutDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
