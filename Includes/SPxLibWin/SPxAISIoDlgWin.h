/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the AIS I/O child dialog.
*
*
* Revision Control:
*   17/02/21 v1.3   JGB Removed ais monitor enum qualifiers for older compilers.
*
* Previous Changes:
*   18/09/20 1.2    AJH	Fix headers.
*   04/09/20 1.1    AJH	Initial Version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxAutoPtr.h"

/* Forward declarations. */
class SPxAISDecoder;
class SPxNetCtrlDlgWin;
class SPxSerialCtrlDlgWin;

/*
 * SPxAISIoDlgWin dialog
 */
class SPxAISIoDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxAISIoDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxAISIoDlgWin(CWnd* pParent, SPxAISDecoder *obj);
    virtual ~SPxAISIoDlgWin(void);
    virtual BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Options. */
    void SetAllowRedistribution(int allow);
    void SetAllowNetInputAndOutput(int allow);
    void SetShowStandardAddressMsg(int show);

    /* Dialog Data */
    enum { IDD = IDD_SPX_AIS_IO_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration retrieval functions. */
    SPxAISDecoder *GetObj(void) { return m_obj; }

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
    SPxAISDecoder *m_obj;

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
    CButton m_buttonSrcTCP;
    CButton m_buttonDestEnabled;
    CButton m_buttonDstTCP;
    CStatic m_groupSrc;
    CStatic m_groupDest;

    /*
     * Private functions
     */
    void OnBnClickedSrcType(void);
    void OnBnClickedSPxHeaders(void);
    void OnBnClickedSrcTCP(void);
    void OnBnClickedDestEnabled(void);
    void OnBnClickedDstTCP(void);

    /* Avoid warnings about hiding overloaded virtual functions. */
    using CDialog::Create;
}; /* class SPxAISIoDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
