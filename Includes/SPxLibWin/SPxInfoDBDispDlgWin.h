/*********************************************************************
*
* (c) Copyright 2020 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the info DB display dialog.
*
*
* Revision Control:
*   15/09/22 v1.9    BTB	Rework how enum definition names are configured.
*
* Previous Changes:
*   03/12/21 1.8    BTB	Support configuring column width.
*   22/11/21 1.7    BTB	Support configuring background and text colour.
*   29/10/21 1.6    BTB	Support configuring row height.
*   17/03/21 1.5    BTB	Support generic string-based units for fields.
*   06/07/20 1.4    AGC	Use consistent naming for dialog IDs.
*   03/07/20 1.3    BTB	Support configuration of displayed number of DP for fields.
*   12/06/20 1.2    BTB	Support configuration of display units for fields.
*   11/06/20 1.1    BTB	Initial version.
*
**********************************************************************/

#pragma once

/* Library headers. */
#include "SPxLibUtils/SPxAutoPtr.h"
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
class SPxInfoDBNode;

/*
 * SPxInfoDBDispDlgWin class.
 */
class SPxInfoDBDispDlgWin : public CDialog, public SPxObj
{
public:
    /* Constructor and destructor. */
    SPxInfoDBDispDlgWin(SPxInfoDBNode *dbNode, CWnd *parentWin = NULL);
    virtual ~SPxInfoDBDispDlgWin();
    BOOL Create(void);

    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Configuration of the item type and fields being managed by the dialog. */
    SPxErrorCode SetItemType(const char *type);
    const char *GetItemType(void);
    SPxErrorCode SetItemFieldName(unsigned int fieldIndex, const char *fieldName);
    const char *GetItemFieldName(unsigned int fieldIndex);
    SPxErrorCode SetItemFieldEnumName(unsigned int fieldIndex, const char *enumName);
    const char *GetItemFieldEnumName(unsigned int fieldIndex);
    SPxErrorCode SetItemFieldUnits(unsigned int fieldIndex, int fieldUnits);
    int GetItemFieldUnits(unsigned int fieldIndex);
    SPxErrorCode SetItemFieldUnitString(unsigned int fieldIndex, const char *fieldUnits);
    const char *GetItemFieldUnitString(unsigned int fieldIndex);
    SPxErrorCode SetItemFieldNumDP(unsigned int fieldIndex, int numDP);
    int GetItemFieldNumDP(unsigned int fieldIndex);
    SPxErrorCode SetItemFieldColWidth(unsigned int fieldIndex, int widthPx);
    int GetItemFieldColWidth(unsigned int fieldIndex);

    /* Configuration of table font. */
    SPxErrorCode SetFontName(const char *fontName);
    const char *GetFontName(void);
    SPxErrorCode SetFontSize(int fontSize);
    int GetFontSize(void);

    /* Configuration of table row height. */
    SPxErrorCode SetLinesPerRow(unsigned int lines);
    unsigned int GetLinesPerRow(void);

    /* Configuration of colour. */
    SPxErrorCode SetBackgroundColour(unsigned int bgr);
    unsigned int GetBackgroundColour(void);
    SPxErrorCode SetTextColour(unsigned int bgr);
    unsigned int GetTextColour(void);

    /* Synchronisation. */
    void SyncGUI(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_INFODB_DISP_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual void OnOK(void) { /* Don't call the CDialog handler */ };
    virtual void OnCancel(void) { /* Don't call the CDialog handler */ };
    virtual afx_msg void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* The parent window. */
    CWnd *m_parentWin;

    /* Item management node. */
    SPxInfoDBNode *m_dbNode;

    /* Item list. */
    SPxWinListCtrl m_itemList;

    void syncGUIFromTimer(void);
    void updateBuffer(void);
    void addItemToBuffer(SPxInfoDBItem *item);
    void addFieldToBuffer(SPxInfoDBField *field);

    static int addItemToBufferIterator(void *dbNodeVoid, 
                                       void *objVoid, 
                                       void *itemVoid);
    static int addFieldToBufferIterator(void *dbItemVoid, 
                                        void *objVoid, 
                                        void *fieldVoid);

    using CDialog::Create;

}; /* SPxInfoDBInputDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
