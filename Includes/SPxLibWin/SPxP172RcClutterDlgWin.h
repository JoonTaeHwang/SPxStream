/*********************************************************************
*
* (c) Copyright 2017 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for the P172 clutter map control child dialog.
*
* Revision Control:
*   16/08/18 v1.3    SP 	Don't store clutter map state internally.
*
* Previous Changes:
*   31/07/18 1.2    SP 	Move clutter map control into interface 
*                      	class to support ACS.
*   13/09/17 1.1    SP 	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* Include the classes we need. */
#include "SPxLibWin/SPxP172RcBaseDlgWin.h"

/*
 * SPxP172RcClutterDlgWin dialog.
 */
class SPxP172RcClutterDlgWin : public SPxP172RcBaseDlgWin
{
    DECLARE_DYNAMIC(SPxP172RcClutterDlgWin)

public:

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxP172RcClutterDlgWin(SPxP172RcInterfaceWin *rdrIf, BOOL isXBand);
    virtual ~SPxP172RcClutterDlgWin(void);
    virtual BOOL Create(CWnd* pParent);
    virtual BOOL Update(void);
    virtual void SendConfigSettings(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_P172_RC_CLUTTER_DIALOG };

protected:

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual afx_msg void OnDestroy(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen); 

private:

    /* 
     * Private types. 
     */

    /* 
     * Private functions. 
     */
    void SyncGUI(void);

    /* GUI callback functions */
    afx_msg void OnBnClickedClutterEnableCheck(void);
    afx_msg void OnCbnSelchangeClutterScansCombo(void);
    afx_msg void OnBnClickedClutterClearButton(void);

    /* 
     * Private static functions. 
     */

    /* 
     * Private variables. 
     */

    /* Misc. */
    BOOL m_isXBand;

    /* GUI objects. */
    CButton *m_enableCheck;
    CComboBox *m_scansCombo;
    CStatic *m_scansLabel;
    CButton *m_clearBn;

    /* GUI data exchange variables. */

}; /* class SPxP172RcClutterDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
