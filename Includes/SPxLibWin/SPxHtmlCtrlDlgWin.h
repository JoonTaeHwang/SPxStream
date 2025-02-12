/*********************************************************************
*
* (c) Copyright 2016 - 2018, 2022, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for the HTML child dialog for displaying a web page.
*
*
* Revision Control:
*   01/09/22 v1.12   AGC	Re-use spawned help popups.
*
* Previous Changes:
*   11/07/22 1.11   AGC	Improve behaviour of spawned popups.
*   29/06/22 1.10   AGC	Disallow most popup browser windows.
*   14/06/22 1.9    AGC	Support display of loading image.
*   24/05/22 1.8    AGC	Raise min version for WebView2 to VS2019.
*   23/05/22 1.7    AGC	Fix WebView2 build issues.
*   20/05/22 1.6    AGC	Improve WebView2 visibility checks.
*   20/05/22 1.5    AGC	Use WebView2 where supported.
*   18/10/18 1.4    AGC	Use pointer for CDHtmlDialog.
*   17/10/18 1.3    BTB	Dialog is now child not popup.
*			Add option to make dialog scrollable.
*   16/11/16 1.2    AGC	Derive from SPxWinCustomDlg.
*   02/09/16 1.1    AGC	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxAutoPtr/SPxComPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibWin/SPxComInit.h"
#include "SPxLibWin/SPxComPtr.h"
#include "SPxLibWin/SPxWin32Utils.h"
#include "SPxLibWin/SPxWinImageCtrl.h"

/* For base class. */
#include "SPxLibWin/SPxWinCustomDlg.h"

/* Forward declarations. */
class CDHtmlDialog;
struct ICoreWebView2;
struct ICoreWebView2Controller;
struct ICoreWebView2Settings;
struct ICoreWebView2Environment;
struct ICoreWebView2Deferral;
struct ICoreWebView2NavigationStartingEventArgs;
struct ICoreWebView2NavigationCompletedEventArgs;
struct ICoreWebView2NewWindowRequestedEventArgs;

/* WebView2 requires a Windows 10 SDK.
 * The minimum Visual Studio version build of the
 * SPx library that targets a Window 10 SDK is VS2019.
 */
#define SPX_WEBVIEW2_MIN_VS (1920)

/* Message to indicate WebView2 setip completion. */
#define SPX_WM_WEBVIEW2_COMPLETE (WM_APP + 11)

/* Provide Event Token implementation. */
struct SPxEventRegistrationToken
{
    __int64 value;
};

/*
 * SPxHtmlCtrlDlgWin dialog
 */
class SPxHtmlCtrlDlgWin : public SPxWinCustomDlg
{
    DECLARE_DYNAMIC(SPxHtmlCtrlDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxHtmlCtrlDlgWin(CWnd *pParent, int scrollable=FALSE);
    virtual ~SPxHtmlCtrlDlgWin(void);
    BOOL Create(void);
    void Navigate(const TCHAR *address);

    void SetLoadingImage(int resourceID);
    void SetAllowNavigation(int allow) { m_allowNavigation = allow; }

#if _MSC_VER >= SPX_WEBVIEW2_MIN_VS
    void SetDeferral(ICoreWebView2NewWindowRequestedEventArgs *newWinArgs,
		     ICoreWebView2Deferral *deferral,
		     SPxHtmlCtrlDlgWin *helpParent);
#endif

    static int IsWebView2Supported(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_HTML_CTRL_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    void OnSize(UINT nType, int cx, int cy);
    void OnTimer(UINT_PTR nIDEvent);
    void OnShowWindow(BOOL bShow, UINT nStatus);
    BOOL OnEraseBkgnd(CDC *pDC);
    virtual void OnOK(void) {}
    void OnClose();
    void OnCancel();
    DECLARE_MESSAGE_MAP()

private:
    class CreateWebViewEnvComplete;
    class CreateWebViewCtrlComplete;
    class FullScreenChange;
    class NavigationStarting;
    class NavigationComplete;
    class NewWindowRequested;

    CWnd *m_parent;
    SPxWinImageCtrl m_imgLoading;
    CStatic m_lblLoading;
    int m_scrollable;
    int m_allowNavigation;
    int m_currNavigationAllowed;
    SPxComInit m_comInit;
    SPxAutoPtr<CDHtmlDialog> m_htmlDlg;
#if _MSC_VER >= SPX_WEBVIEW2_MIN_VS
    SPxComPtr<ICoreWebView2> m_webView;
    SPxComPtr<ICoreWebView2Controller> m_webController;
    SPxComPtr<ICoreWebView2Settings> m_webSettings;
    SPxComPtr<ICoreWebView2NewWindowRequestedEventArgs> m_newWinArgs;
    SPxComPtr<ICoreWebView2Deferral> m_deferral;
    SPxComPtr<CreateWebViewEnvComplete> m_webViewEnvComplete;
    SPxComPtr<CreateWebViewCtrlComplete> m_webViewCtrlComplete;
    SPxComPtr<FullScreenChange> m_webViewFullScreenChange;
    SPxComPtr<NavigationStarting> m_webViewNavigationStarting;
    SPxComPtr<NavigationComplete> m_webViewNavigationComplete;
    SPxComPtr<NewWindowRequested> m_webViewNewWindowRequested;
#endif
    CString m_address;
    SPxFullScreenState m_fullScreenState;
#if _MSC_VER >= SPX_WEBVIEW2_MIN_VS
    SPxEventRegistrationToken m_fullScreenEventToken;
    SPxEventRegistrationToken m_navStartingEventToken;
    SPxEventRegistrationToken m_navCompleteEventToken;
    SPxEventRegistrationToken m_newWinReqEventToken;
#endif
    SPxAutoPtr<SPxHtmlCtrlDlgWin> m_helpDlg;
    SPxHtmlCtrlDlgWin *m_helpParent;

#if _MSC_VER >= SPX_WEBVIEW2_MIN_VS
    HRESULT OnCreateWebViewEnvCompleted(HRESULT result,
					ICoreWebView2Environment* env);
    HRESULT OnCreateWebViewCtrlCompleted(HRESULT result,
					 ICoreWebView2Controller* controller);
    LRESULT OnInitWebView2Complete(WPARAM wparam, LPARAM lparam);
    HRESULT OnFullScreenChange(ICoreWebView2 *sender, IUnknown *args);
    HRESULT OnNavigationStart(ICoreWebView2 *sender, ICoreWebView2NavigationStartingEventArgs *args);
    HRESULT OnNavigationComplete(ICoreWebView2 *sender, ICoreWebView2NavigationCompletedEventArgs *args);
    HRESULT OnNewWindowRequested(ICoreWebView2 *sender, ICoreWebView2NewWindowRequestedEventArgs *args);
#endif
    void SetupFallback(void);
    void Resize(void);
    void CheckVisibility(int forceShow=FALSE);

    using CDialog::Create;

}; /* class SPxHtmlCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
