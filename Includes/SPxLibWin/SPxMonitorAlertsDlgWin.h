/*********************************************************************
 *
 * (c) Copyright 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for the SPx Monitor alerts child dialog.
 *
 * Revision Control:
 *   04/05/22 v1.4   SP 	Remove OnSize().
 *
 * Previous Changes:
 *   28/04/22 1.3   SP 	Support alert alarm sound.
 *   27/04/22 1.2   SP 	Show monitor status.
 *   26/04/22 1.1   SP 	Initial version.
 *
 **********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* Include the classes we need. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibNet/SPxRemoteMonitor.h"
#include "SPxLibWin/SPxWinListCtrl.h"

/*
 * SPxMonitorAlertsDlgWin dialog.
 */
class SPxMonitorAlertsDlgWin : public CDialog, public SPxObj
{
    DECLARE_DYNAMIC(SPxMonitorAlertsDlgWin)
    using CDialog::Create;

public:

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxMonitorAlertsDlgWin(CWnd *parent, SPxRemoteMonitor *monitor);
    virtual ~SPxMonitorAlertsDlgWin(void);
    BOOL Create(void);

    /* Configuration. */
    void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_MONITOR_ALERTS_DIALOG };

protected:

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    virtual afx_msg void OnTimer(UINT_PTR nIDEvent);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

        SPxWinListCtrl m_alertsListCtrl;

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    CWnd *m_parent;                 /* Parent window. */
    SPxRemoteMonitor *m_monitor;    /* Monitor interface. */
    int m_nextRow;                  /* Used when writing to list. */

    /* Dialog items. */
    CButton m_ackAllBtn;
    CButton m_alarmSoundCheck;
    CStatic m_statusIcon;
    CStatic m_statusLabel;

    /*
     * Private functions.
     */

    /* General. */
    void syncGUI(void);
    void syncAlertsList(void);
    void createMfcPopupMenu(CMenu *menu);

    /* GUI callback functions */
    afx_msg void OnBnClickedAckAllButton(void);
    afx_msg void OnBnClickedAlarmCheck(void);
    void onPopupMenuCreate(int row, CMenu *menu);
    void onPopupMenuSelect(int row, int id);
    void onAlertAckCheckboxClicked(int row, int id);

    /*
     * Private static functions.
     */

    static void alertIterator(void *userArg, const SPxRemoteMonitor::Alert_t *alert);
    static void popupMenuCreateHandler(void *userArg, int row, CMenu *menu);
    static void popupMenuSelectHandler(void *userArg, int row, int selection);
    static void alertAckCheckboxClickedHandler(void *userArg, int row, int state);

}; /* class SPxMonitorAlertsDlgWin */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
