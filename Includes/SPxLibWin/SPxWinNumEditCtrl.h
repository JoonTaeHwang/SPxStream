/*********************************************************************
*
* (c) Copyright 2010 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for an edit control that only allows numbers.
*
*
* Revision Control:
*   21/11/18 v1.10   AGC	Remove unused OnInitDialog() function.
*
* Previous Changes:
*   31/10/16 1.9    AGC	Make GetValueAs..() functions const.
*   16/03/16 1.8    AGC	Support retrieving min digits and range.
*   03/03/16 1.7    AGC	Fix clang warnings.
*   26/03/14 1.6    AGC	Redraw on base/min digits changes.
*   21/01/14 1.5    AGC	Add macro for improved message handling.
*			Support arbitrary numeric base.
*			Support minimum digits.
*   22/11/13 1.4    AGC	Use typedef for old name.
*   15/10/13 1.3    AGC	Rename to SPxWinNumEditCtrl.
*   08/12/10 1.2    AGC	Move from Windows Server to SPx library.
*   03/12/10 1.2    AGC	Set default font to match parent.
*			Correct spelling of GetValueAsInt() function.
*   18/11/10 1.1    AGC	Initial Version.
**********************************************************************/
#pragma once

/*
 * Constants.
 */

#define WM_ENTER	(WM_USER + 1)	/* Message that "Enter" key was pressed. */
#define WM_VALCHANGE	(WM_USER + 2)	/* Message that the value has changed. */
#define WM_UPDOWN	(WM_USER + 3)	/* Message that up or down key was pressed. */
#define NM_VALCHANGE	(WM_APP + 2)	/* Notification that the value has changed. */
#define ON_NE_CHANGE(id, memberFxn) \
	{ WM_NOTIFY, (WORD)(int)NM_VALCHANGE, (WORD)id, (WORD)id, AfxSigNotify_v, \
		(AFX_PMSG) \
		(static_cast< void (AFX_MSG_CALL CCmdTarget::*)(NMHDR*, LRESULT*) > \
		(memberFxn)) },
/*
 * SPxWinNumEditCtrl control
 */
class SPxWinNumEditCtrl : public CEdit
{
    DECLARE_DYNAMIC(SPxWinNumEditCtrl)

public:
    /* Construction and desctruction. */
    SPxWinNumEditCtrl(void);
    virtual ~SPxWinNumEditCtrl(void);

    /* Overridden functions. */
    virtual BOOL Create(DWORD dwStyle, const RECT &rect, 
			CWnd *pParentWnd, UINT nID);
    virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, 
			  LPCTSTR lpszWindowName, DWORD dwStyle,
			  const RECT &rect, CWnd *pParentWnd, UINT nID,
			  LPVOID lpParam=NULL);
    virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, 
			  LPCTSTR lpszWindowName, DWORD dwStyle,
			  int x, int y, int w, int h, 
			  HWND hWndParent, HMENU nIDorHMenu, 
			  LPVOID lpParam=NULL);

    /* Set or get current value. */
    void SetValue(double val, int redraw=FALSE);
    void SetValue(int val, int redraw=FALSE);
    double GetValueAsDouble(void) const { return m_value; }
    int GetValueAsInt(void) const { return static_cast<int>(m_value); }

    /* Set the allowable range and precision. */
    void SetRange(double min, double max);
    void SetRange(int min, int max);
    double GetRangeMinAsDouble(void) const { return m_minVal; }
    double GetRangeMaxAsDouble(void) const { return m_maxVal; }
    int GetRangeMinAsInt(void) const { return static_cast<int>(m_minVal); }
    int GetRangeMaxAsInt(void) const { return static_cast<int>(m_maxVal); }
    void SetDecimalPlaces(unsigned int numPlaces);
    unsigned int GetDecimalPlaces(void) const { return m_decimalPlaces; }
    void SetIncrement(double increment){ m_increment = increment; }
    void SetIncrement(int increment) { m_increment = static_cast<double>(increment); }
    void SetBase(int base) { m_base = base; SetValue(m_value, TRUE); }
    void SetMinDigits(int minDigits) { m_minDigits = minDigits; SetValue(m_value, TRUE); }
    int GetMinDigits(void) const { return m_minDigits; }

    void UpdateValue(void);

protected:
    /* Overridden functions. */
    virtual UINT OnGetDlgCode(void);
    virtual void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    virtual void OnKillFocus(CWnd *wnd);
    virtual BOOL PreTranslateMessage(MSG *pMsg);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    unsigned int m_decimalPlaces;    /* Number of decimal places to display. */
    double m_value;	    /* Current value. */
    double m_minVal;	    /* Min allowable value. */
    double m_maxVal;	    /* Max allowable value. */
    double m_increment;	    /* Amount to increment by. */
    int m_base;		    /* Base for displayed numbers. */
    int m_minDigits;	    /* Minimum number of digits to display (pad with zeros). */

    using CEdit::CreateEx;

}; /* class SPxWinNumEditCtrl */

/* Old class name for backwards compatibility. */
typedef SPxWinNumEditCtrl CSPxWinNumEditCtrl;

/*********************************************************************
*
* End of file
*
**********************************************************************/
