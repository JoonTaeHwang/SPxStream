/*********************************************************************
*
* (c) Copyright 2013 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulated Target properties control child dialog.
*
*
* Revision Control:
*   26/08/20 v1.18   AGC	Support P466 identity config.
*				Support IJMS config.
*				Support MIL2525D config.
*
* Previous Changes:
*   18/01/18 1.17   AGC	Move controls to various child dialogs.
*   15/12/16 1.16   AGC	Support SART targets.
*   29/11/16 1.15   AGC	Support target description.
*   08/04/16 1.14   AGC	Support IFF values of zero.
*   16/03/16 1.13   AGC	Support extended IFF mode 1.
*   03/03/16 1.12   AGC	Fix clang warnings.
*   31/07/15 1.11   AGC	Support on ground and fixed flags.
*   10/07/15 1.10   AGC	Support manually initiated flag.
*   17/03/15 1.9    AGC	Improve SPI handling.
*   22/10/14 1.8    AGC	Add display of current Mode-C code.
*   14/10/14 1.7    AGC	Support IFF SPI and X pulse.
*			Support test target bit.
*   18/07/14 1.6    AGC	Add Racon support.
*   02/06/14 1.5    AGC	Use new re-usable AIS dialog.
*   26/03/14 1.4    AGC	Support simulated target bit.
*   21/01/14 1.3    AGC	Support IFF.
*   03/05/13 1.2    AGC	Support ADS-B.
*   28/03/13 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxWinEditCtrl. */
#include "SPxLibWin/SPxWinEditCtrl.h"

/* For base class. */
#include "SPxLibWin/SPxTableDlgWin.h"

/* Forward declarations. */
class SPxSimTarget;
class SPxSimTargetPropIffCtrlDlgWin;
class SPxSimTargetPropModeSCtrlDlgWin;
class SPxSimTargetPropAdsbCtrlDlgWin;
class SPxAISDlgWin;
class SPxAISInlandDlgWin;
class SPxSimTargetPropRaconCtrlDlgWin;
class SPxSimTargetPropSartCtrlDlgWin;
class SPxSimTargetPropP466CtrlDlgWin;
class SPxSimTargetPropIjmsCtrlDlgWin;
class SPxSimTargetPropMIL2525DCtrlDlgWin;
class SPxSimTargetPropFlagsCtrlDlgWin;

/*
 * SPxSimTargetPropCtrlDlgWin dialog
 */
class SPxSimTargetPropCtrlDlgWin : public SPxTableDlgWin
{
    DECLARE_DYNAMIC(SPxSimTargetPropCtrlDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxSimTargetPropCtrlDlgWin(CWnd* pParent,
					SPxSimTarget *obj,
					unsigned int projectCode);
    virtual ~SPxSimTargetPropCtrlDlgWin(void);
    virtual BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_TARGET_PROP_CTRL_DIALOG };

    /* Configuration functions. */
    void SetObj(SPxSimTarget *obj);
    SPxSimTarget *GetObj(void) { return m_obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */

    /* The parent window */
    CWnd *m_parentWin;

    /* The objects we are controlling */
    SPxSimTarget *m_obj;

    /* Controls. */
    CStatic m_lblName;
    SPxWinEditCtrl m_editDesc;

    /* Dialogs. */
    SPxAutoPtr<SPxSimTargetPropIffCtrlDlgWin> m_iffDlg;
    SPxAutoPtr<SPxSimTargetPropModeSCtrlDlgWin> m_modeSDlg;
    SPxAutoPtr<SPxSimTargetPropAdsbCtrlDlgWin> m_adsbDlg;
    SPxAutoPtr<SPxAISDlgWin> m_aisDlg;
    SPxAutoPtr<SPxAISInlandDlgWin> m_aisInlandDlg;
    SPxAutoPtr<SPxSimTargetPropRaconCtrlDlgWin> m_raconDlg;
    SPxAutoPtr<SPxSimTargetPropSartCtrlDlgWin> m_sartDlg;
    SPxAutoPtr<SPxSimTargetPropP466CtrlDlgWin> m_p466Dlg;
    SPxAutoPtr<SPxSimTargetPropIjmsCtrlDlgWin> m_ijmsDlg;
    SPxAutoPtr<SPxSimTargetPropMIL2525DCtrlDlgWin> m_mil2525dDlg;
    SPxAutoPtr<SPxSimTargetPropFlagsCtrlDlgWin> m_flagsDlg;

    /* Private functions */
    void OnEditDesc(NMHDR*, LRESULT*);

    using CDialog::Create;

}; /* class SPxSimTargetPropCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
