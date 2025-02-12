/*********************************************************************
*
* (c) Copyright 2012 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the P172 sector blanking control child dialog.
*
* Revision Control:
*   12/10/22 v1.9    SP 	Fix build warning.
*
* Previous Changes:
*   12/10/22 1.8    SP 	Support sweep sectors on SPEXER 600.
*   08/06/20 1.7    SP 	Support syncing of controls.
*   13/09/17 1.6    SP 	Remove unused icons.
*   11/05/16 1.5    SP 	Support multi-language.
*   26/01/15 1.4    AGC	Remove unused HICON.
*   26/11/13 1.3    AGC	Remove unused m_parentWin variable.
*   26/10/12 1.2    SP 	Further development.
*   18/10/12 1.1    SP 	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* Include the classes we need. */
#include "SPxLibWin/SPxP172RcBaseDlgWin.h"
#include "SPxP172RcSectorSizeDlgWin.h"

/*
 * SPxP172RcSectorDlgWin dialog.
 */
class SPxP172RcSectorDlgWin : public SPxP172RcBaseDlgWin
{
    DECLARE_DYNAMIC(SPxP172RcSectorDlgWin)

public:

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxP172RcSectorDlgWin(SPxP172RcInterfaceWin *rdrIf,
                          BOOL isXBand);
    virtual ~SPxP172RcSectorDlgWin(void);
    virtual BOOL Create(CWnd* pParent);
    virtual BOOL Update(void);
    virtual void Sync(void);
    virtual void SendConfigSettings(void);
    SPxErrorCode SetStateFromConfig(void);
    SPxErrorCode SetConfigFromState(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_P172_RC_SECTOR_DIALOG };

protected:

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
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
    void SendSectorEnableCommand(void);
    const SPxP172RcInterfaceWin::SectorStatus_t *GetSectorStatus(unsigned int sectorNum);

    /* GUI callback functions */
    afx_msg void OnBnClickedSectorS1Check(void);
    afx_msg void OnBnClickedSectorS2Check(void);
    afx_msg void OnBnClickedSectorS3Check(void);
    afx_msg void OnBnClickedSectorS4Check(void);
    afx_msg void OnBnClickedSectorTxMuteCheck(void);
    afx_msg void OnBnClickedSectorSizeButton(void);

    /* 
     * Private static functions. 
     */

    /* 
     * Private variables. 
     */

    /* General GUI objects. */
    CButton *m_sectorSizeBn;
    CStatic *m_sectorSizeLed;

    /* Controls and status information. Note that these
     * structures are linked together to form a linked-list.
     */
    Control_t m_sectors[SPX_P172_NUM_SECTORS];
    Control_t m_txMute;

    /* Sector setup dialog. */
    SPxP172RcSectorSizeDlgWin *m_sectorSizeDlg;

    /* Labels. */
    CStringA m_transmitLabel;
    CStringA m_blankingLabel;
    CStringA m_sweepLabel;
    CStringA m_sectorLabel;
    CStringA m_controlLabel;

}; /* class SPxP172RcSectorDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
