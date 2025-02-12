/*********************************************************************
*
* (c) Copyright 2021 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   A pop-up dialog that applications can create to add system tray support.
*
* Revision Control:
*   21/01/22 v1.15   AJH	Fix compiler warnings.
*
* Previous Changes:
*   20/01/22 1.14   AJH	Web interface may have optional path.
*   30/11/21 1.13   REW	Add button to open web interface.
*   20/10/21 1.12   REW	Support https option for web interface.
*   19/10/21 1.11   REW	Add menu option to open web interface.
*   30/09/21 1.10   AGC	Restore console window on app exit.
*   18/05/21 1.9    AGC	Add context menu.
*   12/05/21 1.8    REW	Remove auto-generated content in last change.
*   10/05/21 1.7    AJH	Support freeze.
*   07/05/21 1.6    REW	Add CountApps().
*   06/05/21 1.5    SP 	Add RestoreStdio().
*   27/04/21 1.4    REW	Rework icon handling to also work with thread.
*   27/04/21 1.3    REW	Rework Create() to include optional thread.
*   27/04/21 1.2    REW	Avoid compiler warning.
*   26/04/21 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_SYSTRAYAPP_H_
#define _SPX_SYSTRAYAPP_H_

#define SPX_AFXWIN

/*
 * Other headers required.
 */
/* Include SPx headers we need. */
#include "SPxResource.h"
#include "SPxLibWin/SPxSystemTrayDialog.h"
#include "SPxLibWin/SPxWinMessageCtrl.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/


/*********************************************************************
*
*   Macros
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxThread;

/*
 * Define our class, derived from the system tray base class.
 */
class SPxSystemTrayApp : public SPxSystemTrayDialog
{
    DECLARE_DYNAMIC(SPxSystemTrayApp)

public:

    /*
     * Public variables.
     */
    /* Dialog Data */
    enum { IDD = IDD_SPX_SYSTRAYAPP_DIALOG };

    /*
     * Public functions.
     */
    /* Constructor and destructor.  Remember to Create() and ShowWindow().*/
    SPxSystemTrayApp(const char *appName, CWnd *pParent=NULL);
    virtual ~SPxSystemTrayApp();
    virtual BOOL Create(UINT iconID=0, int showWin=FALSE,
			int runMsgThread=FALSE, CWinApp *app=NULL,
			int hideConsoleWindow=TRUE);

    /* Other initialisation functions if desired. */
    SPxErrorCode InitialiseAfx(void);
    SPxErrorCode RedirectStdio(void);
    SPxErrorCode RestoreStdio(void);
    SPxErrorCode SetQuitFlagPtr(int *quitFlagPtr);
    SPxErrorCode SetWebPort(unsigned int portNum, int secure=FALSE,
	const char *path=NULL);

    /* Convenience function to check how many of our app are running. */
    unsigned int CountApps(const char *exeName32, const char *exeName64);

    /* Options. */
    SPxErrorCode SetLogToFile(int enable);
    SPxErrorCode SetFreeze(int enable);

    /* Simple message reporting (can also GetMsgCtrl() for complex options). */
    void ReportMesg(const char *text);

    /* Access to internals. */
    SPxWinMessageCtrl *GetMsgCtrl(void) { return(&m_msgWnd); }


protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnSize(UINT nType, int cWidth, int cHeight);
    virtual void OnGetMinMaxInfo(MINMAXINFO *lpMMI);

private:

    /*
     * Private variables.
     */
    /* Parent, if any. */
    CWnd *m_parent;			/* From constructor */

    /* Copy of application name. */
    char *m_appName;			/* NULL if not given */

    /* Context menu. */
    CMenu m_menu;
    CMenu m_subMenu;

    /* Underlying message control. */
    SPxWinMessageCtrl m_msgWnd;		/* Window for messages */

    /* GUI data exchange. */
    int m_logToFile;			/* Flag */
    int m_freeze;			/* Flag */

    /* Minimum dialog size. */
    unsigned int m_minWidth;
    unsigned int m_minHeight;

    /* Message loop handling. */
    SPxThread *m_msgThread;		/* Thread handle */
    CWinApp *m_app;			/* App handle */
    int m_showAfterCreate;		/* Flag to say we should show */
    UINT m_iconToUse;			/* Icon, or 0 for default */
    int m_hideConsoleWindow;		/* Has the console window been hidden? */
    UINT m_consoleWindowShowCmd;	/* Show state for console window */

    /* Location of flag to set if quitting. */
    int *m_quitFlagPtr;			/* NULL if not wanted */

    /* Stdio handling. */
    SPxThread *m_stdioThread;		/* Thread handle */
    int m_pfds[2];			/* File descriptors */
    int m_origStdoutFd;			/* Original stdout file descriptor. */
    int m_origStderrFd;			/* Original stderr file descriptor. */
    int m_origStdoutDup;
    int m_origStderrDup;

    /* Port that application provides web interface on. */
    unsigned int m_webPort;		/* Port number, or 0 for none */
    int m_webSecure;			/* Use https for web interface? */
    char *m_webPath;			/* URL for web UI. */

    /*
     * Private functions.
     */
    /* Thread functions. */
    static void *msgThreadFuncStatic(SPxThread *thread);
    void *msgThreadFunc(SPxThread *thread);
    static void *stdioThreadFuncStatic(SPxThread *thread);
    void *stdioThreadFunc(SPxThread *thread);

    /* GUI callback functions. */
    void OnClickedShow(void);
    void OnClickedWebIf(void);
    void OnClickedQuit();
    void OnClickedHide();
    void OnClickedClear();
    void OnClickedOpenWeb();
    void OnClickedLogToFile();
    void OnClickedFreeze();

    /* Avoid warnings about hiding overloaded virtual functions. */
    using CDialog::Create;
};

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_SYSTRAYAPP_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
