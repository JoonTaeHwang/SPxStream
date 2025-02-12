/*********************************************************************
*
* (c) Copyright 2013 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Area Editor dialog - dialog for editing
*	SPxGw... objects and SPxActiveRegion objects.
*
*
* Revision Control:
*   03/07/18 v1.14   AGC	Hide window in StopAndClose9).
*
* Previous Changes:
*   03/07/18 1.13   REW	Add StopAndClose().
*   31/10/17 1.12   AGC	Remove SetCurrentObj() create arg.
*   23/10/17 1.11   AGC	Support improved object creation.
*   03/03/16 1.10   AGC	Fix clang warnings.
*   01/09/14 1.9    AGC	VS2015 format string changes.
*   15/07/14 1.8    SP 	Add SetFilenameExtension().
*   29/01/14 1.7    AGC	Add image path tooltip.
*   22/01/14 1.6    AGC	Support optional symbol scaling.
*   06/12/13 1.5    AGC	Use SPxWinEditCtrl for text/font name.
*   04/12/13 1.4    AGC	Support inheritable options.
*			Use SPxWinEditCtrl for area name.
*   22/11/13 1.3    AGC	Disable area editing when dialog hidden.
*   21/10/13 1.2    AGC	Add SetMenu() function.
*   07/10/13 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/* For SPxAreaEditorMouse. */
#include "SPxLibUtils/SPxAreaEditorMouse.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* For SPxWinButton. */
#include "SPxLibWin/SPxWinButton.h"

/* For SPxWinEditCtrl. */
#include "SPxLibWin/SPxWinEditCtrl.h"

/* Forward declarations. */
class SPxAreaObj;

/*
 * SPxAreaEditorDlgWin dialog
 */
class SPxAreaEditorDlgWin : public CDialog, public SPxAreaEditorMouse
{
    DECLARE_DYNAMIC(SPxAreaEditorDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxAreaEditorDlgWin(CWnd* pParent, SPxAreaObj *obj, SPxRenderer *renderer);
    virtual ~SPxAreaEditorDlgWin(void);
    BOOL Create(void);
    virtual SPxErrorCode SetFilenameExtension(const char *ext);
    virtual void Repopulate(void);
    virtual void SyncFromObj(void);
    virtual void SetCurrentObj(SPxAreaObj *obj, int selectNext=FALSE);
    virtual void SetCurrentPoint(unsigned int index);
    SPxErrorCode SetMenu(SPxWinMenu *menu) { return SPxAreaEditorMouse::SetMenu(menu); }


    virtual void StopAndClose(void) { ShowWindow(SW_HIDE); OnCancel(); }

    /* Dialog Data */
    enum { IDD = IDD_SPX_AREA_EDITOR_DIALOG };

    /* Configuration retrieval functions. */

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ }
    virtual void OnCancel(void);
    void OnShowWindow(BOOL bShow, UINT nStatus);
    virtual BOOL PreTranslateMessage(MSG *pMsg);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */

    /* The parent window */
    CWnd *m_parentWin;
    
    SPxAreaObj *m_obj;			/* The top-level object we are controlling. */
    SPxAreaObj *m_currentArea;		/* The currently selected area. */
    HTREEITEM m_currentItem;		/* The currently selected item in the tree. */
    unsigned int m_currPoint;		/* The currently selected point. */
    TCHAR m_tooltipBuf[2048];		/* Buffer for creating image filename tooltip. */
    char m_extBuf[5];			/* Buffer for filename extension. */

    /* Controls. */
    CButton m_btnLoad;
    CButton m_btnSave;
    CButton m_checkMouseEdit;
    CComboBox m_comboCreate;
    CButton m_btnCreate;
    CTreeCtrl m_tree;
    HTREEITEM m_rootItem;
    CButton m_btnUndo;
    CStatic m_lblUndo;
    SPxWinEditCtrl m_editName;
    CStatic m_lblPosX;
    CStatic m_lblPosY;
    SPxWinSpinCtrl m_spinPosX;
    SPxWinSpinCtrl m_spinPosY;
    SPxWinSpinCtrl m_spinOffX;
    SPxWinSpinCtrl m_spinOffY;
    CButton m_checkColLine;
    CButton m_checkColFill;
    SPxWinButton m_btnColLine;
    SPxWinButton m_btnColFill;
    SPxWinSpinCtrl m_spinAlphaLine;
    SPxWinSpinCtrl m_spinAlphaFill;
    CButton m_checkLineWidth;
    SPxWinSpinCtrl m_spinLineWidth;
    CButton m_checkDash;
    CComboBox m_comboDash;
    SPxWinEditCtrl m_editText;
    SPxWinEditCtrl m_editFont;
    SPxWinSpinCtrl m_spinFontSize;
    CComboBox m_comboAlign;
    CComboBox m_comboSymbol;
    CStatic m_lblImage;
    CButton m_btnImage;
    CButton m_btnAdd;
    CButton m_btnDel;
    CListBox m_listPoints;
    CStatic m_lblPtPosX;
    CStatic m_lblPtPosY;
    SPxWinSpinCtrl m_spinPtPosX;
    SPxWinSpinCtrl m_spinPtPosY;
    CButton m_checkAutoClose;
    CButton m_checkSymbolScaling;
    CButton m_btnDelArea;
    CToolTipCtrl m_toolTip;

    /* Private functions */
    static SPxErrorCode areaFnWrapper(SPxAreaObj *obj, void *userArg);
    SPxErrorCode areaFn(SPxAreaObj *obj, HTREEITEM parent);
    HTREEITEM FindItem(HTREEITEM item, SPxAreaObj *obj);
    static SPxErrorCode undoIterateFn(void *userArg, unsigned int id, const char *name,
				     void *userObj, void *userObj2, void *undoUserArg);

    void OnBtnClickedLoad(void);
    void OnBtnClickedSave(void);
    void OnBtnClickedMouseEdit(void);
    void OnBtnClickedCreate(void);
    void OnTreeSelChanged(NMHDR *pNMHDR, LRESULT *pResult);
    void OnBtnClickedUndo(void);
    void OnListPointsSelChanged(void);
    LRESULT OnEnterEdit(WPARAM wParam, LPARAM lParam);
    void OnKillFocusSetName(void);
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    void OnBtnClickedCheckColLine(void);
    void OnBtnClickedCheckColFill(void);
    void OnBtnClickedColLine(void);
    void OnBtnClickedColFill(void);
    void OnBtnClickedCheckLineWidth(void);
    void OnBtnClickedCheckDash(void);
    void OnComboDashSelChanged(void);
    void OnKillFocusSetText(void);
    void OnKillFocusSetFont(void);
    void OnComboAlignSelChanged(void);
    void OnComboSymbolSelChanged(void);
    void OnBtnClickedImage(void);
    void OnBtnClickedAdd(void);
    void OnBtnClickedDel(void);
    void OnBtnClickedAutoClose(void);
    void OnBtnClickedSymScale(void);
    void OnBtnClickedDelArea(void);
    BOOL OnNeedToolTip(UINT id, NMHDR *pTTTStruct, LRESULT *pResult);

    using CDialog::Create;
    using CDialog::SetMenu;

}; /* class SPxAreaEditorDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
