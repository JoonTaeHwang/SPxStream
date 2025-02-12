/*********************************************************************
*
* (c) Copyright 2008 - 2018, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the generic run-process child control dialog.
*
*
* Revision Control:
*   02/09/22 v1.17   AGC	Rename ParamContext_t to avoid cppcheck ODR warning.
*
* Previous Changes:
*   01/02/22 1.16   AGC	Update high DPI support.
*   07/11/18 1.15   AGC	High DPI improvements.
*   02/09/16 1.14   AGC	Improve high DPI support.
*   04/12/15 1.13   AGC	Add option to sync focused controls.
*   17/03/15 1.12   AGC	Prevent closing child dialog using Escape.
*   06/03/15 1.11   AGC	Allow use in tabbed dialog.
*			Convert to child dialog - SPxProcCtrlPopupDlgWin
*			replaces this for use as popup dialog.
*   10/04/14 1.10   AGC	Use scrollbar if dialog is too tall.
*   26/03/14 1.9    AGC	Move to library from SPxServerWin and
*			rename from SPxWinCtrlPro.h.
*   19/12/12 1.8    REW	Fix IDD.
*   17/12/10 1.7    AGC	Prevent wrapping of value label text.
*			Use correct fonts.
*   13/12/10 1.6    AGC Improve layout - line up controls with text.
*   08/12/10 1.5    AGC	Remove spin control header as now part of library.
*   03/12/10 1.4    AGC	Show icon in dialog.
*			Support not showing excluded parameters.
*   18/11/10 1.3    AGC	Revamp to match new Linux server process dialog.
*   08/11/10 1.2    AGC	Move common headers to precompiled header.
*   11/07/08 1.1    REW	Initial Version.
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* Common headers. */
#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

/* For base class. */
#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxProcCtrlDlgWin;
class SPxProcessRunParameter;
class SPxProcessParameter;
struct SPxProcParamCtrlInfo_tag;
class SPxProcParamIface;

/* Parameter context */
typedef struct
{
    SPxProcCtrlDlgWin *obj;                 /* Pointer to 'this' object */
    int paramNum;                           /* Parameter number */
    int paramIdx;			    /* Parameter index */
    SPxProcessRunParameter *runParam;       /* Process run parameter */
    SPxProcessParameter *param;             /* Underlying parameter */
    const SPxProcParamCtrlInfo_tag *ctrlInfo; /* GUI control info */
    CWnd *ctrlWidget;			    /* Main control widget */
    CWnd *extraCtrlWidget;		    /* Extra control widget (For spin buttons) */
    CStatic *mainLabelWidget;		    /* Main label widget */
    CStatic *extraLabelWidget;		    /* Extra label widget */
    CButton *enableToggle;		    /* Aux enable/disable widget */

} SPxProcCtrlParamContext_t;

/*
 * SPxProcCtrlDlgWin dialog
 */
class SPxProcCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxProcCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxProcCtrlDlgWin(CWnd *pParent, SPxProcParamIface *obj, const char *title=NULL);
    virtual ~SPxProcCtrlDlgWin();
    virtual BOOL Create(UINT nIDTemplate=IDD, CWnd *pParentWnd=NULL) { return CDialog::Create(nIDTemplate, pParentWnd); }

    /* Dialog Data */
    enum { IDD = IDD_SPX_PROC_CTRL_DIALOG };

    /* Set functions. */
    void SetRunProcess(SPxProcParamIface *obj)	{ m_runPro = obj; }

    /* Sync functions. */
    void SyncFromObj(int syncAll, int syncFocusCtrl);
    virtual void SyncFromObj(void) { SyncFromObj(TRUE, FALSE); }
    virtual int GetIDD(void) const { return IDD; }
    virtual CWnd *GetGroupBox(void) { return m_frame; }

protected:
    /* Handle of our icon. */
    HICON m_hIcon;

    /* DDX/DDV support */
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
    DECLARE_MESSAGE_MAP()

    /*
     * Handler functions.
     */
    virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    afx_msg void OnOK(void){};
    afx_msg void OnCancel(void){};
    afx_msg LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    afx_msg void OnEditLostFocus(UINT id);
    afx_msg void OnSliderCtrlChange(UINT id);
    afx_msg void OnMenuCtrlChange(UINT id);
    afx_msg void OnCheckCtrlClicked(UINT id);
    afx_msg void OnButtonCtrlClicked(UINT id);
    afx_msg void OnEnableRadioClicked(UINT id);
    afx_msg void OnDpiChangedAfterParent(void);

private:
    /* GUI control variables. */

    /* The underlying object. */
    SPxProcParamIface *m_runPro;	/* The object we are controlling */

    void InitGUI(CWnd *parent, const char *title, int firstCall);
    void SetParamValue(int paramNum, double val);
    void SetParamString(int paramNum, const char *str);
    void AddCtrl(SPxProcCtrlParamContext_t *ctxPtr);
    void AddLabelCtrl(SPxProcCtrlParamContext_t *ctxPtr);
    void AddSpinCtrl(SPxProcCtrlParamContext_t *ctxPtr);
    void AddEntryCtrl(SPxProcCtrlParamContext_t *ctxPtr);
    void AddSliderCtrl(SPxProcCtrlParamContext_t *ctxPtr);
    void AddRadioCtrl(SPxProcCtrlParamContext_t *ctxPtr);
    void AddMenuCtrl(SPxProcCtrlParamContext_t *ctxPtr);
    void AddCheckCtrl(SPxProcCtrlParamContext_t *ctxPtr);
    void AddButtonCtrl(SPxProcCtrlParamContext_t *ctxPtr);
    void AddEnableRadio(SPxProcCtrlParamContext_t *ctxPtr);
    void AddCtrlLabels(SPxProcCtrlParamContext_t *ctxPtr);
    int GetParamNum(UINT id, UINT type);
    void MapDialogRectWithPxOffset(CRect &rect, int pxOffset);
    int CountParamsInGroup(SPxProcessParameter *paramGroup);
    void UpdateSize(int firstCall);

    /* Number of parameters */
    int m_numParams;
    int m_numShownParams;

    /* Additional context for each parameter */
    SPxProcCtrlParamContext_t *m_paramCtx;

    const char *m_title;	/* Dialog title. */
    CButton *m_frame;		/* Group box around controls. */
    CStatic *m_noParamsLabel;	/* Label saying no params are available. */
    CWnd *m_parent;		/* Parent dialog. */

    /* Layout parameters - all in Dialog Units, except the labelWidth. */
    static const int m_outerBorder = 5;
    static const int m_innerBorder = 5;
    static const int m_topBorder = m_outerBorder + m_innerBorder + 7;
    static const int m_mainLabelH = 9;
    static const int m_paramH = 14;
    static const int m_paramSpace = m_paramH + m_innerBorder;
    static const int m_paramW = 100;
    static const int m_toggleW = 10;
    static const int m_toggleSpace = m_toggleW + m_innerBorder;
    static const int m_spinW = 10;
    static const int m_showValW = 22;
    static const int m_showValSpace = m_showValW + m_innerBorder;
    static const int m_checkW = 10;
    int m_labelWidthPx;

    int m_scrollPos;
    int m_maxHeightDlu;
    int m_heightPx;
    
    /* Fonts. */
    CFont m_largeFont;	    /* Large font for short description text. */
    CFont m_smallFont;	    /* Small font for long description text. */
    CFont m_boldFont;	    /* Bold font for bold labels. */

    int m_synced;

}; /* class SPxProcCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
