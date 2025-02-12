/*********************************************************************
 *
 * (c) Copyright 2019 - 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   RDR remote control channel child dialog.
 *
 * Revision Control:
 *   20/11/20 v1.3    BTB	Rename class.
 *
 * Previous Changes:
 *   14/03/19 1.2    SP	Fix build warnings.
 *   13/03/19 1.1    SP	Initial version.
 *
 *********************************************************************/

#pragma once

/* SPx headers. */
#define SPX_AFXWIN
#include "SPxResource.h"
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibWin/SPxScrollDlgWin.h"

/* Forward declare any SPx classes we need. */
class SPxRemoteRDR;

/* Constants. */

/* SPxRemoteRDRChanDlgWin dialog */
class SPxRemoteRDRChanDlgWin : public CDialog
{
public:
    /*
     * Public variables.
     */

    /* Dialog Data. */
    enum { IDD = IDD_SPX_REMOTE_RDR_CHAN_DLG };

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    SPxRemoteRDRChanDlgWin(SPxRemoteRDR *rdrConn, BOOL isRecord);
    virtual ~SPxRemoteRDRChanDlgWin(void);
    virtual BOOL Create(CWnd *parent);

    /* General. */
    virtual void SetChannelID(int channelID) { m_channelID = channelID; }
    virtual int GetChannelID(void) { return m_channelID; }
    virtual void Update(void);

protected:
    /* 
     * Protected variables. 
     */

    /* 
     * Protected functions. 
     */

    DECLARE_MESSAGE_MAP()
    virtual void OnOK(void) {};     /* Disable OK button. */
    virtual void OnCancel(void) {}; /* Disable Cancel button. */
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);

private:
    /*
     * Private variables.
     */

    /* General. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    BOOL m_isRecord;            /* Record or replay? */
    SPxRemoteRDR *m_rdrConn;    /* Connection to RDR. */
    int  m_channelID;           /* Associated channel ID. */

    /* Dialog items. */
    CStatic *m_statusIcon;
    CStatic *m_nameText;
    CStatic *m_dataRateText;

    /*
     * Private functions.
     */

    void resize(void);
    const char *getChannelName(void);
    SPxStatus_t getChannelStatus(void);
    unsigned int getChannelDataRate(void);

    /* GUI callback functions. */
    afx_msg void OnSize(UINT nType, int width, int height);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    using CDialog::Create;
};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
