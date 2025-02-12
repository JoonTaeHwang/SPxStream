/*********************************************************************
*
* (c) Copyright 2015 - 2016, 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for a reuseable serial port control dialog class.
*
* Revision Control:
*   07/01/19 v1.7    AGC	Improve first serial port name retrieval.
*
* Previous Changes:
*   04/01/19 1.6    REW	Support GetFirstPortName().
*   31/10/16 1.5    AGC	Add Set/GetObj() functions.
*   03/03/16 1.4    AGC	Fix clang warnings.
*   22/02/16 1.3    SP 	Disable controls when dialog disabled.
*   26/03/15 1.2    SP 	Rewritten to use combo box for port name.
*   07/01/15 1.1    AGC	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* For base class */
#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxSerial;

/*
 * SPxSerialCtrlDlgWin dialog
 */
class SPxSerialCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxSerialCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxSerialCtrlDlgWin(CWnd* pParent, SPxSerial *obj);
    virtual ~SPxSerialCtrlDlgWin();
    BOOL Create(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SERIAL_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    void SetObj(SPxSerial *obj) { m_obj = obj; }
    SPxSerial *GetObj(void) { return m_obj; }

    SPxErrorCode GetFirstPortName(char *buf, unsigned int bufLen) const;

    /* Sync functions. */
    void SyncFromObj(void);

protected:
    /* Protected functions. */
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    CWnd *m_parentWin;              /* The parent window */
    SPxSerial *m_obj;               /* The object being controlled. */

    /* Dialog items. */
    CComboBox *m_portCombo; /* Serial port control combo. */
    CComboBox *m_baudCombo; /* Serial baud control combo. */
     
    /* Callback functions. */
    afx_msg void OnEnable(BOOL bEnable);
    afx_msg void OnCbnSelchangePortCombo(void);
    afx_msg void OnCbnSelchangeBaudCombo(void);
    afx_msg void OnDestroy(void);

    using SPxTabbedItemDlg::Create;

}; /* class SPxSerialCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
