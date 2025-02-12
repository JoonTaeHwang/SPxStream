/*********************************************************************
 *
 * (c) Copyright 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for the SPx Monitor alerts popup dialog.
 *
 * Revision Control:
 *   11/06/19 v1.1   SP 	Initial version.
 *
 * Previous Changes:
 *
 **********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxLibWin/SPxMonitorAlertsDlgWin.h"

/*
 * SPxMonitorAlertsPopupDlgWin dialog
 */
class SPxMonitorAlertsPopupDlgWin : public SPxMonitorAlertsDlgWin
{
    DECLARE_DYNAMIC(SPxMonitorAlertsPopupDlgWin)

public:

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxMonitorAlertsPopupDlgWin(CWnd* pParent, SPxRemoteMonitor *monitor);
    virtual ~SPxMonitorAlertsPopupDlgWin();

protected:

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

    /* GUI callback functions. */
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

private:

    /*
     * Private variables.
     */

    unsigned int m_minWidth;
    unsigned int m_minHeight;
};

/*********************************************************************
*
* End of file
*
**********************************************************************/
