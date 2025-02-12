/*********************************************************************
*
* (c) Copyright 2012 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the P172 SBS-700 series transceiver 
*	control child dialog.
*
* Revision Control:
*   08/06/20 v1.4    SP 	Support syncing of controls.
*
* Previous Changes:
*   26/01/15 1.3    AGC	Remove unused HICON.
*   26/10/12 1.2    SP 	Further development.
*   18/10/12 1.1    SP 	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* Include the classes we need. */
#include "SPxLibWin/SPxP172RcBaseDlgWin.h"
#include "SPxLibWin/SPxP172RcSectorDlgWin.h"

/*
 * SPxP172RcR7TxRxDlgWin dialog.
 */
class SPxP172RcR7TxRxDlgWin : public SPxP172RcBaseDlgWin
{
    DECLARE_DYNAMIC(SPxP172RcR7TxRxDlgWin)

public:

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxP172RcR7TxRxDlgWin(SPxP172RcInterfaceWin *rdrIf,
                          BOOL isXBand);
    virtual ~SPxP172RcR7TxRxDlgWin(void);
    virtual BOOL Create(CWnd* pParent);
    virtual BOOL Update(void);
    virtual void Sync(void);
    virtual void SendConfigSettings(void);
    virtual void SyncAfterEmergencyStop(void);
    virtual SPxErrorCode SetStateFromConfig(void);
    virtual SPxErrorCode SetConfigFromState(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_P172_RC_R7_TXRX_DIALOG };

protected:

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual afx_msg void OnDestroy(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen); 

private:

    /* 
     * Private types. 
     */

    /* 
     * Private functions. 
     */

    /* GUI callback functions */
    afx_msg void OnCbnSelchangeModeCombo();
    afx_msg void OnCbnSelchangePulLenCombo();
    afx_msg void OnCbnSelchangeJitterCombo();
    afx_msg void OnCbnSelchangeMonitorCombo();
    afx_msg void OnBnClickedModeSwapButton();

    /* 
     * Private static functions. 
     */

    /* 
     * Private variables. 
     */

    /* Swap transceiver button. */
    CButton *m_txRxSwapBn;

    /* Controls and status information. Note that these
     * structures are linked together to form a linked-list.
     */
    Control_t m_runMode; 
    Control_t m_pulseLen; 
    Control_t m_jitter;
    Control_t m_monitor;

    /* Sector blanking dialog. */
    SPxP172RcSectorDlgWin *m_sectBlankDlg;

}; /* class SPxP172RcR7TxRxDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
