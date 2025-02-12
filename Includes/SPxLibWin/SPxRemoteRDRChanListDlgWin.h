/*********************************************************************
 *
 * (c) Copyright 2019 - 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   RDR remote control channel list child dialog.
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

/* SPxRemoteRDRChanListDlgWin dialog */
class SPxRemoteRDRChanListDlgWin : public SPxScrollDlgWin
{
public:
    /*
     * Public variables.
     */

    /* Dialog Data. */
    enum { IDD = IDD_SPX_REMOTE_RDR_CHAN_LIST_DLG };

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    SPxRemoteRDRChanListDlgWin(SPxRemoteRDR *rdrConn, BOOL isRecord);
    virtual ~SPxRemoteRDRChanListDlgWin(void);
    virtual BOOL Create(CWnd *parent);

    /* General. */
    virtual void Update(void);

protected:
    /* 
     * Protected variables. 
     */

    /* 
     * Protected functions. 
     */

    DECLARE_MESSAGE_MAP()
    virtual void OnOK() {};     /* Disable OK button. */
    virtual void OnCancel() {}; /* Disable Cancel button. */
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

private:
    /*
     * Private variables.
     */

    /* General. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxRemoteRDR *m_rdrConn;    /* Connection to RDR. */
    BOOL m_isRecord;            /* Record or replay? */
    int m_nextYPixels;          /* Next Y position in the dialog. */

    /* Dialog items. */
   
    /*
     * Private functions.
     */
    void resize(void);
    void updateChannels(void);
    void updateChannel(unsigned int index);
    unsigned int getNumChannels(void);
    int getChannelID(unsigned int index);

    /* GUI callback functions. */
    afx_msg void OnSize(UINT nType, int width, int height);

    /*
     * Private static functions.
     */

    using SPxScrollDlgWin::Create;
};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
