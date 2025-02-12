/*********************************************************************
*
* (c) Copyright 2015 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulated Radar control IFF child dialog.
*
*
* Revision Control:
*   11/05/21 v1.8    AGC	Support IFF Mode 6.
*
* Previous Changes:
*   26/04/19 1.7    AGC	Add single mode IFF tags option.
*   24/04/19 1.6    AGC	Improve multi-channel support.
*   25/06/18 1.5    AGC	Correctly suppress cppcheck warning.
*   23/01/18 1.4    AGC	Support multiple objects.
*   18/07/17 1.3    AGC	Support new IFF encode in video mode.
*   03/03/16 1.2    AGC	Fix clang warnings.
*   26/11/15 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibWin/SPxWinEditCtrl.h"
#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxSimRadar;

/*
 * SPxSimRadarIffCtrlDlgWin dialog
 */
class SPxSimRadarIffCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxSimRadarIffCtrlDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxSimRadarIffCtrlDlgWin(CWnd* pParent,
				      unsigned int numObjects,
				      SPxSimRadar *obj[],
				      unsigned int projectCode);
    virtual ~SPxSimRadarIffCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_RADAR_IFF_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration retrieval functions. */
    SPxSimRadar *GetObj(void) { return m_obj; }
    // cppcheck-suppress virtualCallInConstructor
    virtual void SetObjIndex(int index);

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    CWnd *m_parentWin;
    SPxSimRadar *m_obj;
    unsigned int m_projectCode;

    /* Controls. */
    SPxWinEditCtrl m_editIffInterlace;
    CComboBox m_comboIffEncode;
    CButton m_checkSinglePattern;

    /* Private functions */

    /*
     * Handler functions.
     */
    void OnEditIffInterlace(NMHDR*, LRESULT*);
    void OnChangeIffEncode(void);
    void OnCheckSinglePattern(void);
 
    using SPxTabbedItemDlg::Create;

}; /* class SPxSimRadarIffCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
