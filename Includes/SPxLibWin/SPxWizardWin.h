/*********************************************************************
*
* (c) Copyright 2010 - 2015, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header file for SPxWizard functionality on Windows.
*
*
* Revision Control:
*   19/06/20 v1.12   AJH	Support password entry.
*
* Previous Changes:
*   26/01/15 1.11   AGC	Use const char * for addControlButton() text.
*   21/10/13 1.10   REW	Support multiple instances of same page.
*   04/10/13 1.9    AGC	Simplify headers.
*   24/05/13 1.8    AGC	Fix WINVER undefined warning.
*   01/10/12 1.7    AGC	Move control map to source file.
*   13/07/12 1.6    AGC	Add accelerator keys and set focus.
*   23/12/10 1.5    AGC	Free control buttons on delete.
*   17/12/10 1.4    AGC	Save dialog position between pages.
*   13/12/10 1.3    AGC	Complete implementation - callbacks for all controls,
*			progress indication, better layout, support slider
*			controls.
*   08/12/10 1.2    AGC	Implement most functionality - not complete.
*   26/11/10 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_WIZARDWIN_H
#define _SPX_WIZARDWIN_H

/*
 * Other headers required.
 */
#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxProcParam types. */
#include "SPxLibData/SPxProcParam.h"

/* We need our base class. */
#include "SPxLibUtils/SPxWizard.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Types of control that may be added programatically. */
enum SPxWizCtrlType_t
{
    SPX_WIZ_CTRL_LABEL,
    SPX_WIZ_CTRL_BUTTON,
    SPX_WIZ_CTRL_EDIT,
    SPX_WIZ_CTRL_SPIN,
    SPX_WIZ_CTRL_MENU,
    SPX_WIZ_CTRL_SLIDER,
    SPX_WIZ_CTRL_PASSWORD
};

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
struct SPxWizardWinPriv;

/*
 * Define the class.
 */
class SPxWizardWin :public CDialog, public SPxWizard
{
public:
    /*
     * Public functions.
     */
    /* Constructor/destructor. */
    SPxWizardWin(CWnd *parent=NULL, HICON hIcon=NULL);
    virtual ~SPxWizardWin(void);

protected:
    /*
     * Protected functions.
     */
    /* Functions to build the wizard, which we need to overwrite from
     * the base class to actually implement the wizard functionality
     * on different platforms.
     */
    virtual SPxErrorCode startPage(const char *label,
				    unsigned int pageIdx,
				    unsigned int numPages,
				    unsigned int pageInstance,
				    unsigned int numInstances);
    virtual SPxErrorCode addNote(unsigned int indent,
				    const char *note);
    virtual SPxErrorCode addButton(unsigned int indent, void *callbackArg,
				    int isRadio,
				    const char *label,
				    int initialState);
    virtual SPxErrorCode addControl(unsigned int indent, void *callbackArg,
				    struct SPxProcParamCtrlInfo_tag *info,
				    const char *initialValue);
    virtual SPxWizardPageResult_t runPage(void);


    /* Message map functions. */
    virtual BOOL OnInitDialog();
    void OnCmd(UINT id);
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    LRESULT OnEditEnter(WPARAM wParam, LPARAM lParam);
    virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO *pTI) const;
    BOOL OnNeedToolTip(UINT id, NMHDR *pTTTStruct, LRESULT *pResult);
    void OnMove(int x, int y);
    DECLARE_MESSAGE_MAP()

private:
    /*
     * Private functions.
     */
    void addControlButton(CButton **btn, const char *text, CRect rect, 
			  int id, int enable, int defaultBtn=FALSE);
    void createEditCtrl(unsigned int indent, void *callbackArg,
			SPxProcParamCtrlInfo_t *info,
			const char *initialValue);
    void createPasswordCtrl(unsigned int indent, void *callbackArg,
			SPxProcParamCtrlInfo_t *info,
			const char *initialValue);
    void createSpinCtrl(unsigned int indent, void *callbackArg,
			SPxProcParamCtrlInfo_t *info,
			const char *initialValue);
    void createCheckCtrl(unsigned int indent, void *callbackArg,
		 	 SPxProcParamCtrlInfo_t *info,
			 const char *initialValue);
    void createRadioCtrl(unsigned int indent, void *callbackArg,
			 SPxProcParamCtrlInfo_t *info,
			 const char *initialValue);
    void createMenuCtrl(unsigned int indent, void *callbackArg,
			SPxProcParamCtrlInfo_t *info,
			const char *initialValue);
    void createSliderCtrl(unsigned int indent, void *callbackArg,
			  SPxProcParamCtrlInfo_t *info,
			  const char *initialValue);

    /*
     * Private variables.
     */
    DLGTEMPLATE *m_winTemplate;		    /* Dialog template. */
    CWnd *m_parent;			    /* Parent control. */
    HICON m_hIcon;			    /* Icon for dialog. */
    SPxAutoPtr<SPxWizardWinPriv> m_p;	    /* Private structure. */
    int m_nextCtrlID;			    /* ID for next control to create. */
    int m_currY;			    /* Y position of next control. */
    int m_isGroupStarted;		    /* Has a radio button group been started. */
    CButton *m_mainGroup;		    /* Main group box. */
    CButton *m_prevBtn;			    /* Previous button. */
    CButton *m_nextBtn;			    /* Next button. */
    CButton *m_finishBtn;		    /* Finish button. */
    CStatic *m_progressLabel;		    /* Progress indicator. */
    unsigned int m_pageIdx;		    /* Index of  current page. */
    unsigned int m_numPages;		    /* Total number of pages. */
    unsigned int m_pageInstance;	    /* Instance of current page */
    unsigned int m_numInstances;	    /* Number of instances of page */
    CString m_pageLabel;		    /* Text for main group box. */
    CFont *m_noteFont;			    /* Font for notes. */
    CPoint m_dialogPosition;		    /* Dialog location on screen in pixels. */

    /*
     * Static variables for class.
     */

    /* Layout variables - ALL in dialog units. */
    static const int m_lineSpacing = 2;	    /* Spacing between lines. */
    static const int m_noteSpace = 3;	    /* Extra space under a note. */
    static const int m_ctrlH = 14;	    /* Default height of controls. */
    static const int m_buttonH = 12;	    /* Default height of buttons (check/radio). */
    static const int m_borderIn = 8;	    /* Border inside main group box. */
    static const int m_borderOut = 5;	    /* Border outside main group box. */
    static const int m_indent = 25;	    /* Indent for each indent level. */
    static const int m_btnW = 35;	    /* Width of prev/next/finish buttons. */
    static const int m_minWidth = 185;	    /* Minimum width of dialog. */

}; /* SPxWizard */


/*********************************************************************
*
*   Macros
*
**********************************************************************/

/*********************************************************************
*
*   Variable definitions
*
**********************************************************************/

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_WIZARDWIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
