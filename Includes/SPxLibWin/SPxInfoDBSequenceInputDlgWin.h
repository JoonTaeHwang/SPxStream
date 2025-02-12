/*********************************************************************
*
* (c) Copyright 2020 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the info DB sequenced main input dialog.
*
*
* Revision Control:
*   15/09/22 v1.5    BTB	Rework how enum definition names are configured.
*
* Previous Changes:
*   17/03/21 1.4    BTB	Support generic string-based units for fields.
*   23/09/20 1.3    BTB	Support specifying rows/columns for fields.
*   09/09/20 1.2    BTB	Various additions to controls/functionality.
*   04/09/20 1.1    BTB	Initial version.
*
**********************************************************************/

#pragma once

/* Library headers. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"
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
 * SPxInfoDBSequenceInputDlgWin class.
 */
class SPxInfoDBSequenceInputDlgWin : public CDialog, public SPxObj
{
public:
    /* Constructor and destructor. */
    SPxInfoDBSequenceInputDlgWin(SPxInfoDBNode *dbNode, const char *nextState, const char *previousState,
                                 CWnd *parentWin = NULL, int creationDisabled = FALSE, int deletionDisabled = FALSE);
    virtual ~SPxInfoDBSequenceInputDlgWin();
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
    int GetItemFieldUnits(unsigned int fieldIndex);
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
    enum { IDD = IDD_SPX_INFODB_SEQUENCE_INPUT_DIALOG };

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
    afx_msg void OnButtonSendForward(void);
    afx_msg void OnButtonSendBack(void);
    afx_msg void OnButtonRecall(void);
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

    /* Controls for prefill item to use. */
    CStatic m_prefillLabel;
    CComboBox m_prefillCombo;

    /* Buttons for item operations. */
    CButton m_createBn;
    CButton m_editBn;
    CButton m_deleteBn;
    CButton m_sendForwardBn;
    CButton m_sendBackBn;
    CButton m_recallBn;

    /* Item lists. */
    SPxWinListCtrl m_workQueueItemList;
    SPxWinListCtrl m_inactiveItemList;

    /* Item edit child dialog. */
    SPxInfoDBItemEditPopupDlgWin *m_itemEditDlg;

    void syncGUIFromTimer(void);
    void updateBuffers(void);
    void updateRowSelection(void);

    void addItemToWorkQueueBuffer(SPxInfoDBItem *item);
    void addFieldToWorkQueueBuffer(SPxInfoDBField *field);
    void onWorkQueueRowSelect(int row,  SPxWinListCtrl::SelectState_t state);

    void addItemToInactiveBuffer(SPxInfoDBItem *item);
    void addFieldToInactiveBuffer(SPxInfoDBField *field);
    void onInactiveRowSelect(int row,  SPxWinListCtrl::SelectState_t state);

    void addMissingFields(const char *itemUUID);

    static int addItemToWorkQueueBufferIterator(void *dbNodeVoid, 
                                                void *objVoid, 
                                                void *itemVoid);
    static int addFieldToWorkQueueBufferIterator(void *dbItemVoid, 
                                                 void *objVoid, 
                                                 void *fieldVoid);
    static void workQueueRowSelectHandler(void *userArg, 
                                          int row, 
                                          SPxWinListCtrl::SelectState_t state);

    static int addItemToInactiveBufferIterator(void *dbNodeVoid, 
                                               void *objVoid, 
                                               void *itemVoid);
    static int addFieldToInactiveBufferIterator(void *dbItemVoid, 
                                                void *objVoid, 
                                                void *fieldVoid);
    static void inactiveRowSelectHandler(void *userArg, 
                                         int row, 
                                         SPxWinListCtrl::SelectState_t state);

    using CDialog::Create;

}; /* SPxInfoDBSequenceInputDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
