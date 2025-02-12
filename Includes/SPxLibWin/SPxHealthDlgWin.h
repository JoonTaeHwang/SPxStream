/*********************************************************************
*
* (c) Copyright 2013 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxHealthDlgWin.h,v $
* ID: $Id: SPxHealthDlgWin.h,v 1.4 2017/04/12 09:22:18 rew Exp $
*
* Purpose:
*	Header for the re-usable health indicator control dialog.
*
*
* Revision Control:
*   12/04/17 v1.4    AGC	Add SourceAux type.
*				Support changing displayed names at runtime.
*
* Previous Changes:
*   03/03/16 1.3    AGC	Fix clang warnings.
*   01/02/16 1.2    AGC	Add Get() function.
*			Support two different border styles for swatches.
*   21/10/13 1.1    AGC	Initial version.
**********************************************************************/
#pragma once

/* Standard headers. */
#include "SPxResource.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/*
 *  SPxHealthDlgWin dialog
 */
class SPxHealthDlgWin: public CDialog
{
    DECLARE_DYNAMIC(SPxHealthDlgWin)

public:

    /* Possible healths. */
    enum Health
    {
	Disabled,
	Error,
	Warning,
	OK
    };

    enum Type
    {
	Initialisation,
	License,
	Source,
	SourceAux,
	Processing,
	NavData
    };

    enum SwatchStyle
    {
	Line,
	Sunken
    };

    /* Construction and destruction. */
    SPxHealthDlgWin(CWnd* pParent = NULL);
    virtual ~SPxHealthDlgWin(void);
    BOOL Create(void);

    SPxErrorCode SetSwatchStyle(SwatchStyle swatchStyle);

    SPxErrorCode Add(Type type, const char *name, const char *defaultErrorText=NULL);

    /* Set the health status for type of indicator. */
    SPxErrorCode Set(Type type, Health health, const char *errorText=NULL);

    SPxErrorCode SetName(Type type, const char *name);

    Health Get(Type type) const;

    /* Dialog Data */
    enum { IDD = IDD_SPX_HEALTH_DIALOG };

protected:
    /* Standard data exchange function. */
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    void OnSize(UINT nType, int cx, int cy);
    HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
    virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
    BOOL OnNeedToolTip(UINT id, NMHDR *pTTTStruct, LRESULT *pResult);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    CWnd *m_parent;
    CStatic m_group;

    /* Private functions. */
    void resize(void);

    using CDialog::Create;

}; /* SPxHealthDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

