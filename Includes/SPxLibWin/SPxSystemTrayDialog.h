/*********************************************************************
 *
 * (c) Copyright 2016, 2017, 2019, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxSystemTrayDialog.
 *
 * Revision Control:
 *   14/03/19 v1.3   SP 	Open dialog with single left click.
 *
 * Previous Changes:
 *   18/01/17 1.2   AGC	Show context menu on right button up not down.
 *   16/09/16 1.1   SP 	Initial version.
 *
 **********************************************************************/

#pragma once
#include "afxcmn.h"

/* Standard Windows and GDI+ headers */
#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

/* Library headers. */
#include "SPxResource.h"
#include "SPxLibUtils/SPxObj.h"

/* Constants. */
#define WM_TRAY_ICON_NOTIFY_MESSAGE (WM_USER + 1)

/* SPxSystemTrayDialog dialog */
class SPxSystemTrayDialog : public CDialog
{
    DECLARE_DYNAMIC(SPxSystemTrayDialog)

public:

    /*
     * Public types.
     */

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor. */
    SPxSystemTrayDialog(UINT resourceIDTemplate, CWnd* pParent=NULL);
    virtual ~SPxSystemTrayDialog(void);
    
    /* Configuration. */
    virtual BOOL SetTrayIconMenu(UINT resourceID);
    virtual BOOL SetTrayIconMenu(HMENU hMenu);
    virtual BOOL SetTrayIconMenu(const char * name);
    virtual void SetTrayIconToolTip(const char * text);
    virtual void SetTrayIcon(HICON hIcon);
    virtual void SetTrayIcon(UINT resourceID);
    virtual void SetTrayIcon(const char * resourceName);
    virtual void SetDefaultToTrayEnabled(BOOL isEnabled);
    virtual void SetMinimiseToTrayEnabled(BOOL isEnabled);
    virtual void SetCloseToTrayEnabled(BOOL isEnabled);
    virtual void SetAutoHideTrayIconEnabled(BOOL isEnabled);
    virtual BOOL IsTrayIconVisible(void);

    /* Overrides. */
    virtual BOOL ShowWindow(int nCmdShow);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual void OnCancel(void);

    /* GUI callback functions. */
    virtual afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    virtual afx_msg void OnDestroy(void);
    virtual afx_msg void OnSysCommand(UINT resourceID, LPARAM lParam);
    virtual afx_msg void OnWindowPosChanging(WINDOWPOS *lpwndpos);
    virtual afx_msg LRESULT OnTrayIconNotify(WPARAM wParam, LPARAM lParam);

    /* Override to handle mouse events of system tray icon. */
    virtual void OnTrayIconLButtonDown(CPoint pt);
    virtual void OnTrayIconRButtonDown(CPoint pt);
    virtual void OnTrayIconLButtonUp(CPoint pt);
    virtual void OnTrayIconRButtonUp(CPoint pt);
    virtual void OnTrayIconLButtonDblClk(CPoint pt);
    virtual void OnTrayIconRButtonDblClk(CPoint pt);
    virtual void OnTrayMouseMove(CPoint pt);

private:

    /*
     * Private variables.
     */

    BOOL m_isDefaultToTrayEnabled;
    BOOL m_isMinimiseToTrayEnabled;
    BOOL m_isCloseToTrayEnabled;
    BOOL m_isAutoHideTrayIconEnabled;
    BOOL m_isTrayIconVisible;
    BOOL m_forceDialogHidden;
    NOTIFYICONDATA m_iconData;
    CMenu m_systemTrayMenu;

    /*
     * Private functions.
     */

    BOOL ShowTrayIcon(void);
    BOOL HideTrayIcon(void);
    BOOL UpdateTrayIcon(void);
    void ShowDialog(void);
};

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/
