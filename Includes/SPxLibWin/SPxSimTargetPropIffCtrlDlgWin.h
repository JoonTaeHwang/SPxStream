/*********************************************************************
*
* (c) Copyright 2017, 2018, 2021, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for the Simulated Target IFF properties dialog.
*
*
* Revision Control:
*   11/05/21 v1.3    AGC	Support IFF Mode 6.
*
* Previous Changes:
*   19/01/18 1.2    AGC	Support IFF military emergency.
*   01/12/17 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxWinNumEditCtrl. */
#include "SPxLibWin/SPxWinNumEditCtrl.h"

/* For base class. */
#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxSimTarget;

/*
 * SPxSimTargetPropCtrlDlgWin dialog
 */
class SPxSimTargetPropIffCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxSimTargetPropIffCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxSimTargetPropIffCtrlDlgWin(CWnd* pParent,
				  SPxSimTarget *obj,
				  unsigned int projectCode);
    virtual ~SPxSimTargetPropIffCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_TARGET_PROP_IFF_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration functions. */
    void SetObj(SPxSimTarget *obj);
    SPxSimTarget *GetObj(void) { return m_obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */

    /* The parent window */
    CWnd *m_parentWin;

    /* The objects we are controlling */
    SPxSimTarget *m_obj;
    unsigned int m_projectCode;
    int m_spiTimerRunning;

    /* IFF controls. */
    CButton m_checkMode1;
    CButton m_checkMode2;
    CButton m_checkMode3A;
    SPxWinNumEditCtrl m_editMode1;
    SPxWinNumEditCtrl m_editMode2;
    SPxWinNumEditCtrl m_editMode3A;
    SPxWinNumEditCtrl m_editMode1Conf;
    SPxWinNumEditCtrl m_editMode2Conf;
    SPxWinNumEditCtrl m_editMode3AConf;
    SPxWinNumEditCtrl m_editModeCConf;
    CButton m_checkMode1Ext;
    CStatic m_lblModeC;
    CButton m_checkSpi;
    CButton m_checkX;
    CButton m_checkME;
    CButton m_checkMode6;

    /* Private functions */
    void OnCheckMode1(void);
    void OnCheckMode2(void);
    void OnCheckMode3A(void);
    void OnSetMode1(NMHDR *, LRESULT *);
    void OnSetMode2(NMHDR *, LRESULT *);
    void OnSetMode3A(NMHDR *, LRESULT *);
    void OnSetMode1Conf(NMHDR *, LRESULT *);
    void OnSetMode2Conf(NMHDR *, LRESULT *);
    void OnSetMode3AConf(NMHDR *, LRESULT *);
    void OnSetModeCConf(NMHDR *, LRESULT *);
    void OnCheckMode1Ext(void);
    void OnCheckSpi(void);
    void OnCheckX(void);
    void OnCheckME(void);
    void OnCheckMode6(void);
    void syncModeC(void);
    void syncSpi(UINT8 iffFlags);

    using CDialog::Create;

}; /* class SPxSimTargetPropIffCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
