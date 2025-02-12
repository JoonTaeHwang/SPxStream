/*********************************************************************
*
* (c) Copyright 2013 - 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxServerDlgWin.h,v $
* ID: $Id: SPxServerDlgWin.h,v 1.14 2016/03/03 10:59:08 rew Exp $
*
* Purpose:
*	Header for basic re-usable dialog for SPx servers.
*
*
* Revision Control:
*   03/03/16 v1.14   AGC	Fix clang warnings.
*
* Previous Changes:
*   08/07/14 1.13   AGC	Re-add old AddRight() option.
*   04/07/14 1.12   AGC	Simplify AddRight() options.
*   20/06/14 1.11   AGC	Support using parameter for status bar.
*   08/05/14 1.10   AGC	Improved escape behaviour.
*   01/04/14 1.9    AGC	Add optional parameter for the enabled
*			object for right hand side controls.
*   26/03/14 1.8    AGC	Automatically size left side controls.
*   05/03/14 1.7    AGC	Add clear messages menu item.
*   17/02/14 1.6    AGC	Use re-usable message control.
*			Add RemoveLeft/HasLeft functions.
*   27/01/14 1.5    AGC	Replace resizable left controls with show/hide.
*   22/11/13 1.4    AGC	Display messages straight away if possible.
*   18/11/13 1.3    AGC	Support tooltips for bitmap buttons.
*   04/11/13 1.2    AGC	Add GetOverloadDesc() function.
*			Support adding bitmap buttons.
*			Support adding SPxConfigObjDlgWin controls.
*			Support coloured messages in message window.
*   21/10/13 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxCriticalSection. */
#include "SPxLibUtils/SPxCriticalSection.h"

/* Forward declarations. */
class SPxServerBase;
class SPxHealthDlgWin;
class SPxConfigObjDlgWin;
class SPxNavData;
class SPxObj;

/*
 * SPxServerDlgWin dialog
 */
class SPxServerDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxServerDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxServerDlgWin(CWnd* pParent, const char *title=NULL,
	const char *configFile=NULL);
    virtual ~SPxServerDlgWin(void);
    BOOL Create(void);
    virtual void Sync(void);

    void ReportMsg(COLORREF colour, const char *msg, ...);

    void SetServer(SPxServerBase *server);
    SPxServerBase *GetServer(void) const;
    void SetLogo(int logoID);
    virtual BOOL SetMenu(CMenu *pMenu);
    void SetMaxLines(int maxLines);
    void AddLeft(SPxHealthDlgWin *ctrl);
    void AddLeft(SPxConfigObjDlgWin *ctrl);
    void AddLeft(CWnd *ctrl);
    void RemoveLeft(CWnd *ctrl);
    int HasLeft(CWnd *ctrl) const;
    void AddMiddle(CWnd *ctrl);
    void AddRight(CWnd *ctrl=NULL);
    void AddRight(int bitmapID, const char *caption,
	SPxObj *activeObj, SPxObj *enableObj=NULL,
	const char *activeParam=NULL, const char *enableParam=NULL);
    void AddRight(int bitmapID, const char *caption,
	SPxObj *activeObj, SPxObj *enableObj,
	const char *activeParam, const char *enableParam,
	int activeValue);
    void SetShowStatusBar(int show);
    void SetShowStatusBarTime(int show);
    void SetStatusBarNavData(SPxNavData *navData);
    void AddToStatusBar(const char *label, SPxObj *obj, SPxObj *extraObj=NULL);
    void AddToStatusBar(const char *label, SPxObj *obj, const char *activeParam);
    void Layout(void);

    BOOL TranslateAccelerator(LPMSG lpMsg);

    virtual const char *GetOverloadDesc(void) const { return "Server is overloaded"; }

    /* Dialog Data */
    enum { IDD = IDD_SPX_SERVER_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void) { /* Don't call the CDialog handler */ };
    virtual void OnSysCommand(UINT nID, LPARAM lParam);
    void OnSize(UINT nType, int cx, int cy);
    void OnTimer(UINT_PTR nIDEvent);
    void OnGetMinMaxInfo(MINMAXINFO *lpMMI);
    virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
    BOOL OnNeedToolTip(UINT id, NMHDR *pTTTStruct, LRESULT *pResult);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions */
    void OnBtnShowHideClicked(void);
    void OnBtnClicked(UINT id);
    void OnSave(void);
    void OnSaveAs(void);
    void OnClearMessages(void);
    void OnQuit(void);
    void OnAbout(void);
    void resize(void);
    void layout(void);
    void displaySaveConfigErrorDlg(SPxErrorCode error, 
				   const char *filename);
    BOOL addMenu(CMenu *menu, CMenu *menuToAdd, int topLevel);
    void addRight(int bitmapID, const char *caption, SPxObj *activeObj,
		  SPxObj *enableObj, const char *activeParam,
		  const char *enableParam, int value, int useValue);
    void updateHealth(void);
    void updateConfigObjs(void);
    void updateButtons(void);
    void save(const char *path);
    UINT32 getProjectCode(void) const;

    using CDialog::Create;

}; /* SPxServerDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
