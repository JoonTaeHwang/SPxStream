/*********************************************************************
*
* (c) Copyright 2019, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulated Radar control IFF child dialog.
*
*
* Revision Control:
*   11/05/21 v1.2    AGC	Support IFF Mode 6.
*
* Previous Changes:
*   24/04/19 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibWin/SPxTableDlgWin.h"

/* Forward declarations. */
class SPxSimRadarCtrlDlgWin;
class SPxSimRadar;

/*
 * SPxSimRadarVidTypeCtrlDlgWin dialog
 */
class SPxSimRadarVidTypeCtrlDlgWin : public SPxTableDlgWin
{
    DECLARE_DYNAMIC(SPxSimRadarVidTypeCtrlDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxSimRadarVidTypeCtrlDlgWin(SPxSimRadarCtrlDlgWin* pParent,
					  unsigned int numObjects,
					  SPxSimRadar *obj[],
					  unsigned int projectCode);
    virtual ~SPxSimRadarVidTypeCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_RADAR_VID_TYPE_CTRL_DIALOG };
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
    SPxSimRadarCtrlDlgWin *m_parentWin;
    SPxSimRadar *m_obj;
    unsigned int m_projectCode;

    /* Controls. */
    CComboBox m_comboVidTypeA;
    CComboBox m_comboVidTypeB;
    CButton m_checkDigBitsParallel;
    CComboBox m_comboVidTypeD0;
    CComboBox m_comboVidTypeD1;
    CComboBox m_comboVidTypeD2;
    CComboBox m_comboVidTypeD3;
    CComboBox m_comboVidTypeD4;
    CComboBox m_comboVidTypeD5;
    CComboBox m_comboVidTypeD6;
    CComboBox m_comboVidTypeD7;

    /* Private functions */
    void OnCheckParallelDig(void);
    void OnChangeVidTypeA(void);
    void OnChangeVidTypeB(void);
    void OnChangeVidTypeD0(void);
    void OnChangeVidTypeD1(void);
    void OnChangeVidTypeD2(void);
    void OnChangeVidTypeD3(void);
    void OnChangeVidTypeD4(void);
    void OnChangeVidTypeD5(void);
    void OnChangeVidTypeD6(void);
    void OnChangeVidTypeD7(void);

    /*
     * Handler functions.
     */

    using SPxTabbedItemDlg::Create;

}; /* class SPxSimRadarVidTypeCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
