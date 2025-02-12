/*********************************************************************
*
* (c) Copyright 2012 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the P172 SBS-800/900 and PCAN series transceiver 
*	control child dialog.
*
* Revision Control:
*   28/09/22 v1.16   SP 	Add support for sleep mode.
*
* Previous Changes:
*   27/09/22 1.15   SP 	Add initial SPEXER 600 support.
*   04/11/21 1.14   SP 	Support Mk5 X-Band.
*   08/06/20 1.13   SP 	Support syncing of controls.
*   22/03/19 1.12   SP 	Add support for pulse override.
*   13/09/17 1.11   SP 	Support clutter map control.
*   04/12/15 1.10   SP 	Changes to use Mk11 TWS mode status field.
*   26/11/15 1.9    SP 	Changes to support Mk11 radar.
*   23/09/15 1.8    SP 	Make status fields const.
*   26/08/15 1.7    SP 	Support linking of transceivers.
*   26/01/15 1.6    AGC	Remove unused HICON.
*   14/10/14 1.5    SP 	Add params to disable gain/sea/rain/IR (TODO 117-3).
*   26/03/14 1.4    SP 	Further changes for SxV support.
*   04/10/13 1.3    AGC	Simplify headers.
*   26/10/12 1.2    SP 	Further development.
*   18/10/12 1.1    SP 	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* Include the classes we need. */
#include "SPxLibWin/SPxP172RcBaseDlgWin.h"

/* Foward declare classes we use. */
class SPxP172RcDlgWin;
class SPxP172RcSectorDlgWin;
class SPxP172RcClutterDlgWin;

/*
 * SPxP172RcR8R9TxRxDlgWin dialog.
 */
class SPxP172RcR8R9TxRxDlgWin : public SPxP172RcBaseDlgWin
{
    DECLARE_DYNAMIC(SPxP172RcR8R9TxRxDlgWin)

public:

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxP172RcR8R9TxRxDlgWin(SPxP172RcInterfaceWin *rdrIf,
                            BOOL isXBand);
    virtual ~SPxP172RcR8R9TxRxDlgWin(void);
    virtual BOOL Create(CWnd* pParent);
    virtual void SetTxRxLink(SPxP172RcR8R9TxRxDlgWin *txRx);
    virtual BOOL Update(void);
    virtual void Sync(void);
    virtual void SendConfigSettings(void);
    virtual void SyncAfterEmergencyStop(void);
    virtual void SetRangeControl(const char *valueStr);
    virtual SPxErrorCode SetStateFromConfig(void);
    virtual SPxErrorCode SetConfigFromState(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_P172_RC_R8R9_TXRX_DIALOG };

protected:

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual afx_msg void OnHScroll(UINT nSBCode, UINT nPos, 
                                   CScrollBar *pScrollBar);
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

    BOOL UpdateRunModeControl(BOOL isAvailable, 
                              const SPxP172RcInterfaceWin::StatusField_t *runModeField,
                              const SPxP172RcInterfaceWin::StatusField_t *sleepModeField);
    BOOL UpdateRangeControl(BOOL isAvailable, 
                            const SPxP172RcInterfaceWin::StatusField_t *field);
    BOOL UpdatePowerControl(BOOL isAvailable, 
                            const SPxP172RcInterfaceWin::StatusField_t *powerField,
                            const SPxP172RcInterfaceWin::StatusField_t *rpiField,
                            const SPxP172RcInterfaceWin::StatusField_t *sartField,
                            const SPxP172RcInterfaceWin::StatusField_t *twsField);
    void SyncPowerControl(const SPxP172RcInterfaceWin::StatusField_t *powerField,
                          const SPxP172RcInterfaceWin::StatusField_t *rpiField,
                          const SPxP172RcInterfaceWin::StatusField_t *sartField,
                          const SPxP172RcInterfaceWin::StatusField_t *twsField);
    BOOL UpdateFreqNumControl(BOOL isAvailable,
                              const SPxP172RcInterfaceWin::StatusField_t *field);
    BOOL UpdatePulseOvrControl(BOOL isAvailable,
                               const SPxP172RcInterfaceWin::StatusField_t *field);
    void SendFreqNumChirpCommand(void);
    void SendFilterCommand(void);
    BOOL IsRangeInNM(const char *range);

    /* GUI callback functions */
    afx_msg void OnCbnSelchangeModeCombo(void);
    afx_msg void OnBnClickedSwapButton(void);
    afx_msg void OnCbnSelchangeRangeCombo(void);
    afx_msg void OnCbnSelchangePowerCombo(void);
    afx_msg void OnCbnSelchangeFreqNumCombo(void);
    afx_msg void OnCbnSelchangeChirpCombo(void);
    afx_msg void OnCbnSelchangeIrCombo(void);
    afx_msg void OnCbnSelchangePulseOvrCombo(void);

    /* 
     * Private static functions. 
     */

    /* Swap transceiver button. */
    CButton *m_txRxSwapBn;

    /* Controls and status information. Note that these
     * structures are linked together to form a linked-list.
     */
    Control_t m_runMode;
    Control_t m_range;
    Control_t m_power;
    Control_t m_freqNum;
    Control_t m_chirp;
    Control_t m_gain;
    Control_t m_seaFilter;
    Control_t m_rainFilter;
    Control_t m_ir;
    Control_t m_pulseOvr;
    CStatic *m_pulseOvrLabel;

    /* User-defined availability of filter controls. */
    BOOL m_isGainAvail;
    BOOL m_isSeaFilterAvail;
    BOOL m_isRainFilterAvail;
    BOOL m_isIRAvail;

    /* Sector blanking dialog. */
    SPxP172RcSectorDlgWin *m_sectBlankDlg;
    SPxP172RcClutterDlgWin *m_clutterMapDlg;

    /* Link to second transceiver. */
    SPxP172RcR8R9TxRxDlgWin *m_txRxLink;

}; /* class SPxP172RcR8R9TxRxDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
