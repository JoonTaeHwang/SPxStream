/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the P322 radar client dialog.
*
* Revision Control:
*   20/07/18 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/
#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxAutoPtr.h"

/* Forward declarations. */
class SPxNetworkClientP322;
struct SPxNetP322MsgDiscovery;

/*
 * SPxP322ClientDlgWin dialog.
 */
class SPxP322ClientDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxP322ClientDlgWin)

public:
    /* Constructor and destructor. */
    SPxP322ClientDlgWin(CWnd* pParent,
			SPxNetworkClientP322 *client);
    virtual ~SPxP322ClientDlgWin(void);
    virtual BOOL Create(void);
    void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_P322_CLIENT_DIALOG };

protected:
    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual void OnTimer(UINT_PTR nTimerID);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    void OnDiscoSelChange(void);
    void OnConnect(void);
    void OnSaveSetup(void);
    void OnSaveIPSetup(void);
    void OnSaveSTC(void);
    void OnIfAddress1Change(NMHDR*, LRESULT*);
    void OnNetmask1Change(NMHDR*, LRESULT*);
    void OnGateway1Change(NMHDR*, LRESULT*);
    void OnIfAddress2Change(NMHDR*, LRESULT*);
    void OnNetmask2Change(NMHDR*, LRESULT*);
    void OnGateway2Change(NMHDR*, LRESULT*);
    void OnStatusAddress1Change(NMHDR*, LRESULT*);
    void OnStatusAddress2Change(NMHDR*, LRESULT*);
    void OnStatusPortChange(NMHDR*, LRESULT*);
    void OnVideoAddress1Change(NMHDR*, LRESULT*);
    void OnVideoAddress2Change(NMHDR*, LRESULT*);
    void OnVideoPortChange(NMHDR*, LRESULT*);
    void OnUseVideoAddress1(void);
    void OnUseVideoAddress2(void);
    void OnControlPortChange(NMHDR*, LRESULT*);
    void OnClearIPSetup(void);
    void OnApplyIPSetup(void);
    void OnAutoTuneChange(void);
    void OnTransmitChange(void);
    void OnTransmitModeChange(void);
    void OnSectorBlankChange(void);
    void OnSartModeChange(void);
    void OnInteferRejectChange(void);
    void OnIfBandwidthChange(void);
    void OnMuteChange(void);
    void OnInhibitTransmitChange(void);
    void OnAutoBiaGainChange(void);
    void OnPrjJitterChange(void);
    void OnNoiseSrcModeChange(void);
    void OnSleepModeChange(void);
    void OnNumSamplesChange(void);
    void OnNumAzimuthsChange(void);
    void OnAntennaRpmChange(NMHDR*, LRESULT*);
    void OnManTuneVoltageChange(NMHDR*, LRESULT*);
    void OnClearTransmit(void);
    void OnApplyTransmit(void);
    void OnRadarTypeChange(void);
    void OnAntennaHeightChange(NMHDR*, LRESULT*);
    void OnTriggerDelayChange(NMHDR*, LRESULT*);
    void OnZeroRangeChange(NMHDR*, LRESULT*);
    void OnAntennaOffsetChange(NMHDR*, LRESULT*);
    void OnStcChange(void);
    void OnTestVideoChange(void);
    void OnSimulationEncoderChange(void);
    void OnSerialNumChange(NMHDR*, LRESULT*);
    void OnSectorSelChange(void);
    void OnSectorStartChange(NMHDR*, LRESULT*);
    void OnSectorEndChange(NMHDR*, LRESULT*);
    void OnSectorRefChange(void);
    void OnClearSetup(void);
    void OnApplySetup(void);

    static void addDiscovered(UINT32 fromAddress,
			      const SPxNetP322MsgDiscovery *msg,
			      void *userArg);

    UINT32 getCurrAddress(void);

    using CDialog::Create;
};

/*********************************************************************
*
* End of file
*
**********************************************************************/
