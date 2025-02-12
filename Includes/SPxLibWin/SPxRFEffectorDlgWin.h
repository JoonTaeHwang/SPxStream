/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for RF Effector control dialog.
*
*
* Revision Control:
*   31/05/08 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibWin/SPxWinCustomDlg.h"
#include "SPxLibUtils/SPxObj.h"

/* Forward declarations. */
class SPxRFJammer;

/*
 * SPxRFEffectorDlgWin dialog
 */
class SPxRFEffectorDlgWin : public SPxWinCustomDlg, public SPxObj
{
    DECLARE_DYNAMIC(SPxRFEffectorDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxRFEffectorDlgWin(CWnd* pParent,
				 SPxRFJammer *effector);
    virtual ~SPxRFEffectorDlgWin(void);
    BOOL Create(void);
    void SyncFromObj(void);

    SPxErrorCode SetButtonText(unsigned int num, const char *text);
    SPxErrorCode GetButtonText(unsigned int num, char *buf, unsigned int bufLen) const;
    SPxErrorCode SetButtonCommand(unsigned int num, unsigned int command);
    SPxErrorCode GetButtonCommand(unsigned int num, unsigned int *command) const;
    SPxErrorCode SetButtonIsBitmask(unsigned int num, int isBitmask);
    SPxErrorCode GetButtonIsBitmask(unsigned int num, int *isBitmask) const;
    SPxErrorCode SetButtonIsStatus(unsigned int num, int isStatus);
    SPxErrorCode GetButtonIsStatus(unsigned int num, int *isStatus) const;

    /* Dialog Data */
    enum { IDD = IDD_SPX_RF_EFFECTOR_CTRL_DIALOG };

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void) {}
    void OnSize(UINT nType, int cx, int cy);
    HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
    void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* The parent window */
    CWnd *m_parentWin;

    /* Background brush for labels. */
    CBrush m_brushBack;

    /* Private functions. */
    void OnBnClicked(UINT id);

    using CDialog::Create;

}; /* class SPxRFEffectorDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
