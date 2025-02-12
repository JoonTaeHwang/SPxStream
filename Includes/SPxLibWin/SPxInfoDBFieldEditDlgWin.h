/*********************************************************************
*
* (c) Copyright 2020 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for info DB field editing dialog.
*
*
* Revision Control:
*   26/04/21 v1.12   BTB	Support editing Boolean fields.
*
* Previous Changes:
*   07/04/21 1.11   BTB	Remove functionality now handled better inside database.
*   23/09/20 1.10   BTB	Support creating dummy dialog with no field object.
*   15/09/20 1.9    BTB	Support editing IFF code fields.
*   08/09/20 1.8    BTB	Support editing date fields.
*   08/09/20 1.7    BTB	Support inactive (read only) fields.
*   13/08/20 1.6    BTB	Support editing enumeration fields.
*   06/07/20 1.5    BTB	Support editing time fields.
*   12/06/20 1.4    BTB	Support editing distance and speed fields.
*   26/05/20 1.3    BTB	Support editing int and double fields.
*   05/05/20 1.2    BTB	Add HasFieldValueChanged().
*   29/04/20 1.1    BTB	Initial version.
*
**********************************************************************/

#pragma once

/* Standard headers. */

/* Library headers. */
#include "SPxLibNet/SPxInfoDBNode.h"
#include "SPxLibUtils/SPxAutoPtr.h"

/* For various SPx controls */
#include "SPxLibWin/SPxWinEditCtrl.h"
#include "SPxLibWin/SPxWinComboBox.h"
#include "SPxLibWin/SPxWinNumEditCtrl.h"
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN

/* Include required header for CDateTimeCtrl. */
#include "afxdtctl.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */

/*
 * SPxInfoDBFieldEditDlgWin dialog
 */
class SPxInfoDBFieldEditDlgWin : public CDialog
{
public:
    /* Constructor and destructor. */
    SPxInfoDBFieldEditDlgWin(SPxInfoDBNode *dbNode, const char *itemUUID,
                             SPxInfoDBField *field, CWnd* parentWin = NULL,
                             int isEditable = TRUE, const char *fieldName = NULL);
    virtual ~SPxInfoDBFieldEditDlgWin(void);
    virtual BOOL Create(void);

    /* General. */
    virtual void ApplyValue(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_INFODB_FIELD_EDIT_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ }
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ }
    DECLARE_MESSAGE_MAP()

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* The parent window. */
    CWnd *m_parentWin;

    /* Controls. */
    CStatic m_nameLabel;
    SPxWinEditCtrl m_valueTextEdit;
    SPxWinSpinCtrl m_valueSpin;
    CStatic m_unitsLabel;
    CDateTimeCtrl m_valueTimePicker;
    CDateTimeCtrl m_valueDatePicker;
    CComboBox m_valueCombo;
    CStatic m_valueLabel;
    SPxWinNumEditCtrl m_valueNumber;
    CButton m_valueCheckBn;

    afx_msg void OnValueTextChange(NMHDR *nmhdr, LRESULT*);
    afx_msg LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    afx_msg void OnTimeChange(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDateChange(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnCbnSelchangeValueCombo(void);
    afx_msg void OnNumEditChange(NMHDR *nmhdr, LRESULT*);
    afx_msg void OnBoolChange(void);

    using CDialog::Create;

}; /* class SPxInfoDBFieldEditDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/