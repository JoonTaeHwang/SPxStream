/*********************************************************************
*
* (c) Copyright 2012 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	A child dialog used to display a single status field.
*
* Revision Control:
*   08/06/20 v1.9    SP 	Show field as disabled in dual-redundant mode.
*
* Previous Changes:
*   22/03/19 1.8    SP 	Automatically start new column when needed.
*   03/03/16 1.7    AGC	Fix clang warnings.
*   22/10/15 1.6    SP 	Store field in a linked list.
*   14/10/15 1.5    SP 	Support linked status fields.
*   26/01/15 1.4    AGC	Remove unused HICON and parent window.
*   01/09/14 1.3    AGC	VS2015 format string changes.
*   26/10/12 1.2    SP 	Further development.
*   18/10/12 1.1    SP 	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* Include the classes we need. */
#include "SPxLibWin/SPxP172RcInterfaceWin.h"
#include "SPxLibWin/SPxP172RcHelpDlgWin.h"

/* Forward declare classes. */
class SPxP172RcDlgWin;

/*
 * SPxP172RcStatusFieldDlgWin dialog.
 */
class SPxP172RcStatusFieldDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxP172RcStatusFieldDlgWin)

public:

    /* 
     * Public types. 
     */

    /* 
     * Public functions. 
     */

    /* Constructor and destructor. */
    SPxP172RcStatusFieldDlgWin(SPxP172RcDlgWin *mainDlg,
                               const SPxP172RcInterfaceWin::StatusField_t *field,
                               const char *groupName,
                               const char *unitName,
                               SPxP172RcHelpDlgWin *helpDlg);
    virtual ~SPxP172RcStatusFieldDlgWin(void);
    virtual BOOL Create(CWnd* pParent);

    /* Update and get status. */
    virtual BOOL Update(void);
    virtual const SPxP172RcInterfaceWin::StatusField_t *GetInterfaceField(void) { return m_field; }
    virtual const char *GetGroupName(void);
    virtual const char *GetUnitName(void);
    virtual SPxP172RcStatusFieldDlgWin *GetNextField(void) { return m_nextField; }

    /* 
     * Public static functions. 
     */
    static SPxP172RcStatusFieldDlgWin *GetFirstField(void) { return FirstField; }

    /* 
     * Public variables. 
     */

    /* Dialog Data */
    enum { IDD = IDD_SPX_P172_RC_STATUS_FIELD_DIALOG };

    /* 
     * Public static variables. 
     */

    static SPxP172RcStatusFieldDlgWin *FirstField;
    static SPxP172RcStatusFieldDlgWin *LastField;

protected:

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };

private:

    /* 
     * Private functions. 
     */

    void SetNextField(SPxP172RcStatusFieldDlgWin *field) { m_nextField = field; }

    /* GUI callback functions. */
    afx_msg void OnBnClickedStatusHelpButton();

    using CDialog::Create;

    /* 
     * Private variables. 
     */

    /* Brushes. */
    CBrush *m_statusOkBrush;
    CBrush *m_statusErrorBrush;
    CBrush *m_statusDisabledBrush;

    /* GUI items. */
    CStatic *m_nameWin;     /* Field name. */
    CStatic *m_valueWin;    /* Field value. */
    CButton *m_helpBn;      /* Help button. */

    /* The main dialog. */
    SPxP172RcDlgWin *m_mainDlg;

    /* The interface. */
    SPxP172RcInterfaceWin *m_interface;

    /* The field to display. */
    const SPxP172RcInterfaceWin::StatusField_t *m_field;

    /* Item name to pass to help dialog. */
    CStringA m_helpItemName;

    /* Is field value OK or showing an error? */
    BOOL m_isOk;

    /* Dialog used to display help info. */
    SPxP172RcHelpDlgWin *m_helpDlg;

    /* Next status field in linked list. */
    SPxP172RcStatusFieldDlgWin *m_nextField;

    /* Associated status name, e.g. "R8R9TxRxS0", etc. */
    CStringA m_groupName;

    /* Associated unit name, e.g. "Active Transceiver", etc. */
    CStringA m_unitName;

}; /* class SPxP172RcStatusFieldDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
