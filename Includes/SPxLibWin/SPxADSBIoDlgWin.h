/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the ADSB I/O child dialog.
*
*
* Revision Control:
*   17/02/21 v1.4   JGB Removed adsb monitor enum qualifiers for older compilers.
*
* Previous Changes:
*   16/02/21 1.3    JGB Deselecting manufacturer ID will disable SPxADSBDecoder
*   05/01/21 1.2    JGB Removed redundant UI
*   04/01/21 1.1    JGB Added input source serial, type and version to UI.
*   27/01/21 1.0    JGB Initial Version
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxAutoPtr.h"

#include "SPxLibWin/SPxWinEditCtrl.h"

/* Forward declarations. */
class SPxADSBDecoder;
class SPxNetCtrlDlgWin;
class SPxSerialCtrlDlgWin;

/*
 * SPxADSBIoDlgWin dialog
 */
class SPxADSBIoDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxADSBIoDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxADSBIoDlgWin(CWnd* pParent, SPxADSBDecoder *obj);
    virtual ~SPxADSBIoDlgWin(void);
    virtual BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Options. */
    void SetAllowRedistribution(int allow);
    void SetAllowNetInputAndOutput(int allow);
    void SetShowStandardAddressMsg(int show);

    /* Dialog Data */
    enum { IDD = IDD_SPX_ADSB_IO_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration retrieval functions. */
    SPxADSBDecoder *GetObj(void) { return m_obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    void OnEditSourceSerial(void);
    void OnEditSourceType(void);
    void OnEditSourceVersion(NMHDR*, LRESULT*);
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
    SPxADSBDecoder *m_obj;

    /* Options. */
    int m_allowRedistribution;
    int m_allowNetInputAndOutput;
    int m_showStandardMsgs;
    int m_sourceType;

    /* Sub dialogs. */
    SPxAutoPtr<SPxNetCtrlDlgWin> m_srcNetDlg;
    SPxAutoPtr<SPxSerialCtrlDlgWin> m_srcSerialDlg;
    SPxAutoPtr<SPxNetCtrlDlgWin> m_destNetDlg;

    /* MFC controls. */
    CButton m_buttonDestEnabled;
    CButton m_buttonDstTCP;
    CStatic m_groupSrc;
    CStatic m_groupDest;

    CComboBox m_sourceSerialCombo;
    CComboBox m_sourceTypeCombo;
    SPxWinEditCtrl m_sourceVersionEdit;

    /*
     * Private functions
     */
    void OnBnClickedDestEnabled(void);
    void OnBnClickedDstTCP(void);

    /* Avoid warnings about hiding overloaded virtual functions. */
    using CDialog::Create;
}; /* class SPxADSBIoDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
