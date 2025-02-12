/*********************************************************************
*
* (c) Copyright 2020 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the info DB main input dialog.
*
*
* Revision Control:
*   15/09/22 v1.13   BTB	Rework how enum definition names are configured.
*
* Previous Changes:
*   17/03/21 1.12   BTB	Support generic string-based units for fields.
*   23/09/20 1.11   BTB	Support specifying rows/columns for fields.
*                      	Support read only fields.
*   28/08/20 1.10   BTB	Use icons for load/save buttons.
*   27/08/20 1.9    BTB	Add tooltips to buttons.
*   27/08/20 1.8    BTB	Add load and save buttons.
*   03/07/20 1.7    BTB	Support configuring number of DP for double-based fields.
*   12/06/20 1.6    BTB	Support distance and speed fields.
*   26/05/20 1.5    BTB	Support configuration of type for fields.
*   18/05/20 1.4    BTB	Update to work with changes to info DB library code.
*   13/05/20 1.3    BTB	Do all updates from the GUI thread.
*   06/05/20 1.2    BTB	Support operation without editing capability.
*   29/04/20 1.1    BTB	Initial version.
*
**********************************************************************/

#pragma once

/* Library headers. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibWin/SPxWinButton.h"
#include "SPxLibWin/SPxWinListCtrl.h"

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
class SPxInfoDBItemEditPopupDlgWin;
class SPxInfoDBNode;

/*
 * SPxInfoDBInputDlgWin class.
 */
class SPxInfoDBInputDlgWin : public CDialog, public SPxObj
{
public:
    /* Constructor and destructor. */
    SPxInfoDBInputDlgWin(SPxInfoDBNode *dbNode, CWnd *parentWin = NULL, int editDisabled = FALSE);
    virtual ~SPxInfoDBInputDlgWin();
    BOOL Create(void);

    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Configuration of the item type and fields being managed by the dialog. */
    SPxErrorCode SetItemType(const char *type);
    const char *GetItemType(void);
    SPxErrorCode SetItemFieldName(unsigned int fieldIndex, const char *fieldName);
    const char *GetItemFieldName(unsigned int fieldIndex);
    SPxErrorCode SetItemFieldType(unsigned int fieldIndex, int fieldType);
    int GetItemFieldType(unsigned int fieldIndex);
    SPxErrorCode SetItemFieldEnumName(unsigned int fieldIndex, const char* enumName);
    const char* GetItemFieldEnumName(unsigned int fieldIndex);
    SPxErrorCode SetItemFieldReadOnly(unsigned int fieldIndex, int readOnly);
    int GetItemFieldReadOnly(unsigned int fieldIndex);
    SPxErrorCode SetItemFieldUnits(unsigned int fieldIndex, int fieldUnits);
    int GetItemFieldUnits(unsigned int fieldUnits);
    SPxErrorCode SetItemFieldUnitString(unsigned int fieldIndex, const char *fieldUnits);
    const char *GetItemFieldUnitString(unsigned int fieldIndex);
    SPxErrorCode SetItemFieldNumDP(unsigned int fieldIndex, int numDP);
    int GetItemFieldNumDP(unsigned int fieldIndex);
    SPxErrorCode SetItemFieldEditRow(unsigned int fieldIndex, unsigned int row);
    unsigned int GetItemFieldEditRow(unsigned int fieldIndex);
    SPxErrorCode SetItemFieldEditCol(unsigned int fieldIndex, unsigned int col);
    unsigned int GetItemFieldEditCol(unsigned int fieldIndex);

    /* Synchronisation. */
    void SyncGUI(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_INFODB_INPUT_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG *pMsg);
    virtual void OnOK(void) { /* Don't call the CDialog handler */ };
    virtual void OnCancel(void) { /* Don't call the CDialog handler */ };
    virtual afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnButtonCreate(void);
    afx_msg void OnButtonEdit(void);
    afx_msg void OnButtonDelete(void);
    afx_msg void OnButtonLoad(void);
    afx_msg void OnButtonSave(void);
    DECLARE_MESSAGE_MAP()

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* The parent window. */
    CWnd *m_parentWin;

    /* Item management node. */
    SPxInfoDBNode *m_dbNode;

    /* Used to implement tool tips. */
    CToolTipCtrl m_toolTip;

    /* Buttons for item operations. */
    CButton m_createBn;
    CButton m_editBn;
    CButton m_deleteBn;

    /* Buttons for loading from/saving to file. */
    SPxWinButton m_loadBn;
    SPxWinButton m_saveBn;

    /* Item list. */
    SPxWinListCtrl m_itemList;

    /* Item edit child dialog. */
    SPxInfoDBItemEditPopupDlgWin *m_itemEditDlg;

    void syncGUIFromTimer(void);
    void updateBuffer(void);
    void updateRowSelection(void);
    void addItemToBuffer(SPxInfoDBItem *item);
    void addFieldToBuffer(SPxInfoDBField *field);
    void onRowSelect(int row,  SPxWinListCtrl::SelectState_t state);
    void addMissingFields(const char *itemUUID);

    static int addItemToBufferIterator(void *dbNodeVoid, 
                                       void *objVoid, 
                                       void *itemVoid);
    static int addFieldToBufferIterator(void *dbItemVoid, 
                                        void *objVoid, 
                                        void *fieldVoid);
    static void rowSelectHandler(void *userArg, 
                                 int row, 
                                 SPxWinListCtrl::SelectState_t state);

    using CDialog::Create;

}; /* SPxInfoDBInputDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
