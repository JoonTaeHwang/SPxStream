/*********************************************************************
*
* (c) Copyright 2010 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for a spin control.
*
*
* Revision Control:
*   02/09/20 v1.22   AGC	Suppress CSPxWinButton cppcheck unitialised member warnings.
*
* Previous Changes:
*   02/09/20 1.21   AGC	Workaround CSPxWinButton cppcheck unitialised member warnings.
*   29/07/22 1.20   AGC	Fix acceleration with floating point values.
*   04/01/21 1.19   BTB	Use correct max value.
*   04/01/21 1.18   BTB	Add define for max usable value.
*   02/02/17 1.17   AGC	Improve acceleration settings.
*   15/12/16 1.16   AGC	GetValue...() functions now const.
*   03/03/16 1.15   AGC	Fix clang warnings.
*   04/12/15 1.14   AGC	Add setWhenFocused argument to SetValue().
*   06/10/15 1.13   AGC	Add HasFocus() function.
*   10/07/15 1.12   AGC	Add GetNumEdit/GetSpinButton() functions.
*   21/01/14 1.11   AGC	Add macro for improved message handling.
*			Support arbitrary numeric base.
*			Support minimum digits.
*   15/01/14 1.10   AGC	Align look of automatic/manually created spin controls.
*   22/11/13 1.9    AGC	Add more SetValue/GetValue overloads.
*   07/10/13 1.8    AGC	Support use with or without dialog editor.
*			Rename class to SPxWinSpinCtrl.
*   24/05/13 1.7    AGC	Fix WINVER undefined warning.
*   20/02/12 1.6    AGC	By default, disable mouse-wheel changing value.
*   04/03/11 1.5    AGC	Add SetFocus() override to set focus to num edit control
*   22/12/10 1.4    AGC	Display spin button correctly on Windows Vista.
*   13/12/10 1.3    AGC	Add optional redraw parameter to SetValue().
*   08/12/10 1.2    AGC	Move from Windows Server to SPx library.
*   18/11/10 1.1    AGC	Initial Version.
**********************************************************************/
#pragma once

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

#include "SPxWinNumEditCtrl.h"

/* Max usable value for spin control range, higher values stop the buttons on
 * the control from working properly.
 */
#define SPX_WIN_SPIN_MAX_VALUE 2147483647.0

/* Window message for any change to the value associated with the spin control. */
#define WM_SPINCHANGE	(WM_USER + 4)
#define NM_SPINCHANGE	(WM_APP + 4)
#define ON_SP_CHANGE(id, memberFxn) \
	{ WM_NOTIFY, (WORD)(int)NM_SPINCHANGE, (WORD)id, (WORD)id, AfxSigNotify_v, \
		(AFX_PMSG) \
		(static_cast< void (AFX_MSG_CALL CCmdTarget::*)(NMHDR*, LRESULT*) > \
		(memberFxn)) },

/* Forward declarations. */
struct SPxWinSpinCtrlData;

/* Define our class. */
class SPxWinSpinCtrl : public CWnd
{
    DECLARE_DYNAMIC(SPxWinSpinCtrl)

public:
    /* Constructor and destructor */
    SPxWinSpinCtrl(void);
    virtual ~SPxWinSpinCtrl(void);

    /* Set and get the value associated with the spin control. */
    void SetValue(double val, int redraw=FALSE, int setWhenFocused=FALSE);
    void SetValue(float val, int redraw=FALSE, int setWhenFocused=FALSE);
    void SetValue(int val, int redraw=FALSE, int setWhenFocused=FALSE);
    void SetValue(unsigned int val, int redraw=FALSE, int setWhenFocused=FALSE);
    double GetValueAsDouble(void) const { return m_value; }
    float GetValueAsFloat(void) const { return static_cast<float>(m_value); }
    int GetValueAsInt(void) const { return static_cast<int>(m_value); }
    unsigned int GetValueAsUint(void) const { return static_cast<unsigned int>(m_value); }

    /* Set the range and precision of the spin control. */
    void SetRange(double min, double max);
    void SetRange(int min, int max);
    void SetDecimalPlaces(unsigned int numDigits);
    unsigned int GetDecimalPlaces(void);
    void SetIncrement(double increment);
    void SetIncrement(int increment);
    void SetBase(int base);
    void SetMinDigits(int minDigits);

    /* Enable/disable mouse wheel interaction. */
    void SetHandleMouseWheel(int handleMouseWheel) { m_handleMouseWheel = handleMouseWheel; }

    int HasFocus(void) const;
    SPxWinNumEditCtrl *GetNumEdit(void);
    CSpinButtonCtrl *GetSpinButton(void);

protected:

    /* Overridden CWnd functions. */
    virtual void OnEnable(BOOL bEnable);
    virtual void OnSize(UINT nType, int cx, int cy);
      
    /* Message map handlers. */
    void OnSpinCtrlChange(NMHDR *pNMHDR, LRESULT *pResult);
    void OnSpinEditChange(NMHDR *, LRESULT *);

    /* DDX/DDV support */
    virtual void DoDataExchange(CDataExchange* pDX);
    BOOL PreTranslateMessage(MSG *pMsg);
    DECLARE_MESSAGE_MAP()

private:
    /* GUI objects. */
    CFont m_font;  /* Control font. */

    /* Current value. */
    double m_value;

    /* Min and max values. */
    double m_minVal;
    double m_maxVal;

    double m_increment;	/* Amount increment by for each up/down arrow. */

    int m_handleMouseWheel; /* Change spin button when mouse scrolled? */

    /* Private functions. */
    SPxWinSpinCtrlData *GetP(void);
    const SPxWinSpinCtrlData *GetP(void) const;
    static LRESULT WINAPI windowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
    int convertValForSpin(double val) const;
    void resetAcceleration(void);

}; /* class SPxWinSpinCtrl */

/* Class with old name for backwards compatibility. */
class CSPxWinSpinCtrl : public SPxWinSpinCtrl
{
public:
    // cppcheck-suppress uninitDerivedMemberVar
    CSPxWinSpinCtrl(CWnd * /*parent*/=NULL) { };
    virtual ~CSPxWinSpinCtrl(void) {};
    BOOL Create(int flags, CRect size, CWnd *parent, int ID);
private:
    using SPxWinSpinCtrl::Create;
};

/*********************************************************************
*
* End of file
*
**********************************************************************/
