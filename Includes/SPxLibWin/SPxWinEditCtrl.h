/*********************************************************************
*
* (c) Copyright 2010 - 2020 Cambridge Pixel Ltd.
*
* Purpose:
*	Header for an edit control that sends message to parent
*	when enter key pressed.
*
*
* Revision Control:
*   26/08/20 v1.10   AGC	Add HasFocus() function.
*
* Previous Changes:
*   03/03/16 1.9    AGC	Fix clang warnings.
*   18/07/14 1.8    AGC	Add SetValidChars() function.
*   02/06/14 1.7    AGC	Include required headers.
*   22/01/14 1.6    AGC	Fix macro for NM_TEXTCHANGE.
*   21/01/14 1.5    AGC	Add macro for improved message handling.
*   13/12/13 1.4    AGC	Only handle key down enter messages.
*   22/11/13 1.3    AGC	Rename to SPxWinEditCtrl.
*   08/12/10 1.2    AGC	Move from Windows Server to SPx library.
*   03/12/10 1.1    AGC	Initial Version.
**********************************************************************/

#pragma once

#define WM_ENTER	(WM_USER + 1)	/* Message that "Enter" key was pressed. */
#define NM_TEXTCHANGE	(WM_APP + 6)
#define ON_EN_RETURN(id, memberFxn) \
	{ WM_NOTIFY, (WORD)(int)NM_RETURN, (WORD)id, (WORD)id, AfxSigNotify_v, \
		(AFX_PMSG) \
		(static_cast< void (AFX_MSG_CALL CCmdTarget::*)(NMHDR*, LRESULT*) > \
		(memberFxn)) },
#define ON_EN_TEXTCHANGE(id, memberFxn) \
	{ WM_NOTIFY, (WORD)(int)NM_TEXTCHANGE, (WORD)id, (WORD)id, AfxSigNotify_v, \
		(AFX_PMSG) \
		(static_cast< void (AFX_MSG_CALL CCmdTarget::*)(NMHDR*, LRESULT*) > \
		(memberFxn)) },

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

#include "SPxLibUtils/SPxAutoPtr.h"

/*
 * SPxWinEditCtrl control
 */
class SPxWinEditCtrl : public CEdit
{
    DECLARE_DYNAMIC(SPxWinEditCtrl)

public:
    /* Construction and desctruction. */
    SPxWinEditCtrl();
    virtual ~SPxWinEditCtrl();

    /* Overridden functions. */
    virtual BOOL Create(DWORD dwStyle, const RECT &rect, 
			CWnd *pParentWnd, UINT nID);
    virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, 
			  LPCTSTR lpszWindowName, DWORD dwStyle,
			  const RECT &rect, CWnd *pParentWnd, UINT nID, LPVOID lpParam=0);
    virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, 
			  LPCTSTR lpszWindowName, DWORD dwStyle,
			  int x, int y, int w, int h, 
			  HWND hWndParent, HMENU nIDorHMenu, 
			  LPVOID lpParam=0);

    int HasFocus(void) const;
    void SetValidChars(const char *validChars);

protected:
    /* Protected functions. */
    virtual BOOL PreTranslateMessage(MSG *pMsg);
    virtual void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    virtual void OnKillFocus(CWnd *pNewWnd);

    DECLARE_MESSAGE_MAP()

private:
    struct impl;
    SPxAutoPtr<impl> m_p;
    
}; /* class SPxWinEditCtrl */

/* Old class name for backwards compatibility. */
typedef SPxWinEditCtrl CSPxWinEditCtrl;

/*********************************************************************
*
* End of file
*
**********************************************************************/
