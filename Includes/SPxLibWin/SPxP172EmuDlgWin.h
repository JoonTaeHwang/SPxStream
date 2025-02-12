/*********************************************************************
*
* (c) Copyright 2015, 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxP172EmuDlgWin.h,v $
* ID: $Id: SPxP172EmuDlgWin.h,v 1.4 2016/03/03 10:59:08 rew Exp $
*
* Purpose:
*	Header for the P172 radar emulator dialog.
*
* Revision Control:
*   03/03/16 v1.4    AGC	Fix clang warnings.
*
* Previous Changes:
*   15/12/15 1.3    AJH	Removed OnDestroy().
*   10/12/15 1.2    AJH	Added resize functions.
*   17/11/15 1.1    AJH	Initial version.
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* Include the classes we need. */
#include "SPxLibData/SPxP172Emulator.h"
#include "SPxLibWin/SPxP172EmuRadarDlgWin.h"

/*
 * SPxP172EmuDlgWin dialog.
 */
class SPxP172EmuDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxP172EmuDlgWin)

public:

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxP172EmuDlgWin(CWnd* pParent,
		     SPxP172Emulator *obj[],
		     unsigned int numRadar = 1,
		     BOOL isStandAlone = TRUE);
    virtual ~SPxP172EmuDlgWin(void);
    virtual BOOL Create(void);
    void SyncFromObj(void);

    /* Log file. */
    void WriteToLogFile(const char *text);

    /* Dialog Data */
    enum { IDD = IDD_SPX_P172_EMU_DIALOG };

protected:

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual afx_msg void OnTimer(UINT_PTR nIDEvent);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };

private:
    /* Private variables. */

    /* The parent window */
    CWnd *m_parentWin;

    /* The objects we are controlling, and the number of objects. */
    SPxP172Emulator **m_emulatorObj;
    const unsigned int m_numRadar;

    /* Stand-alone application? */
    const BOOL m_isStandAlone;
    
    /* Handles to dialog widgets. */
    SPxAutoPtr<SPxTabbedDlgWin> m_radarDlg;
    SPxTabbedItemDlg **m_radarDlgTabs;
    SPxTabbedItemName **m_radarDlgTitleObjects;

    /* Private functions. */
    static void syncFnWrapper(void *arg);
    void syncFn(void);
    void resize(void);

    using CDialog::Create;

    /* 
     * Private static functions. 
     */
    static void spxErrorHandler(SPxErrorType errType, SPxErrorCode errCode,
                                int arg1, int arg2, 
                                const char *arg3, const char *arg4);

    /* Log file. */
    CStringA m_logFilePrefix;    /* Filename prefix. */
    FILE *m_logFile;             /* File handle. */

}; /* class SPxP172EmuDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
