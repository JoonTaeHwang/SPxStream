/*********************************************************************
*
* (c) Copyright 2018 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Nav Data I/O child dialog.
*
*
* Revision Control:
*   09/01/19 v1.8    AGC	Support mode with only net input OR output.
*
* Previous Changes:
*   08/01/19 1.7    AGC	Use source wrappers to preserve source info.
*   04/01/19 1.6    REW	Improve handling of serial port names etc.
*   03/01/19 1.5    REW	Invert Headerless option to SPx Headers.
*   02/01/19 1.4    REW	Support free mode options.
*   21/12/18 1.3    REW	Restore source network addr/port after serial.
*   06/12/18 1.2    REW	Avoid compiler warning.
*   05/12/18 1.1    REW	Initial Version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibData/SPxNavData.h"

/* Forward declarations. */
class SPxNavData;
class SPxNetCtrlDlgWin;
class SPxSerialCtrlDlgWin;

/*
 * SPxNavIoDlgWin dialog
 */
class SPxNavIoDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxNavIoDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxNavIoDlgWin(CWnd* pParent, SPxNavData *obj);
    virtual ~SPxNavIoDlgWin(void);
    virtual BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Options. */
    void SetAllowRedistribution(int allow);
    void SetAllowNetInputAndOutput(int allow);
    void SetShowStandardAddressMsg(int show);

    /* Dialog Data */
    enum { IDD = IDD_SPX_NAV_IO_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration retrieval functions. */
    SPxNavData *GetObj(void) { return m_obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    DECLARE_MESSAGE_MAP()

private:
    /*
     * Private variables.
     */
    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxNavData *m_obj;

    /* Options. */
    int m_allowRedistribution;
    int m_allowNetInputAndOutput;
    int m_showStandardMsgs;

    /* Sub dialogs. */
    SPxAutoPtr<SPxNetCtrlDlgWin> m_srcNetDlg;
    SPxAutoPtr<SPxSerialCtrlDlgWin> m_srcSerialDlg;
    SPxAutoPtr<SPxNetCtrlDlgWin> m_destNetDlg;

    /* MFC controls. */
    CButton m_radioSrcNet;
    CButton m_radioSrcSerial;
    CButton m_buttonSPxHeaders;
    CButton m_buttonDestEnabled;
    CStatic m_groupSrc;
    CStatic m_groupDest;

    /*
     * Private functions
     */
    void OnBnClickedSrcType(void);
    void OnBnClickedSPxHeaders(void);
    void OnBnClickedDestEnabled(void);

    /* Avoid warnings about hiding overloaded virtual functions. */
    using CDialog::Create;
}; /* class SPxNavIoDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
