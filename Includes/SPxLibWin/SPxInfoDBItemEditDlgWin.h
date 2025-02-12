/*********************************************************************
*
* (c) Copyright 2020, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the info DB item editing dialog.
*
*
* Revision Control:
*   02/09/22 v1.10   AGC	Supress cppcheck duplicate inherited member warning.
*
* Previous Changes:
*   23/09/20 1.9    BTB	Support explicit control of row/column layout.
*                      	Support display of expected fields which don't exist yet.
*   09/09/20 1.8    BTB	Support specific expected fields being read only.
*   08/09/20 1.8    BTB	Improve support for items changing active state.
*   08/07/20 1.7    BTB	Resize dialog properly around contents.
*   07/07/20 1.6    BTB	Don't use IDOK/OnOK() directly with OK button.
*   30/06/20 1.5    BTB	Allow expected fields (and their ordering to be specified).
*   18/05/20 1.4    BTB	Update to work with changes to info DB library code.
*   05/05/20 1.3    BTB	Improve Cancel button behaviour.
*   30/04/20 1.2    BTB	Make sure controls appear correctly.
*   29/04/20 1.1    BTB	Initial version.
*
**********************************************************************/

#pragma once

/* Library headers. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxInfoDBField;
class SPxInfoDBItem;
class SPxInfoDBNode;
class SPxTableDlgWin;

/*
 * SPxInfoDBItemEditDlgWin class.
 */
class SPxInfoDBItemEditDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxInfoDBItemEditDlgWin)

public:
    /* Constructor and destructor. */
    SPxInfoDBItemEditDlgWin(SPxInfoDBNode *dbNode, CWnd *parentWin = NULL);
    virtual ~SPxInfoDBItemEditDlgWin();
    BOOL Create(void);

    void SetItemID(const char *itemID);
    const char *GetItemID(void);

    void AddExpectedField(const char *fieldName,
                          unsigned int row,
                          unsigned int col,
                          int isEditable = TRUE);

    void ForceUpdate(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_INFODB_ITEM_EDIT_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual afx_msg void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual afx_msg void OnCancel(void);
    virtual afx_msg void OnTimer(UINT_PTR nIDEvent);
    virtual afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    virtual afx_msg void OnButtonApply(void);
    virtual afx_msg void OnButtonOK(void);
    DECLARE_MESSAGE_MAP()

private:
    struct impl;
    // cppcheck-suppress duplInheritedMember
    SPxAutoPtr<impl> m_p;

    /* The parent window. */
    CWnd *m_parentWin;

    /* Item management node. */
    SPxInfoDBNode *m_dbNode;

    /* Field list placeholder control. */
    CWnd m_fieldListPh;

    /* Field list dialog. */
    SPxTableDlgWin *m_fieldListDlg;
    int m_minFieldListHeight;
    int m_minFieldListWidth;

    /* Buttons. */
    CButton m_applyBn;
    CButton m_okBn;
    CButton m_cancelBn;

    void updateFieldList(void);
    void clearFieldList(void);
    void addFieldToList(SPxInfoDBField *field);
    void addBlankFieldsToList(void);

    static int addFieldToListIterator(void *dbItemVoid,
                                      void *objVoid,
                                      void *fieldVoid);

    using CDialog::Create;

}; /* class SPxInfoDBItemEditDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
