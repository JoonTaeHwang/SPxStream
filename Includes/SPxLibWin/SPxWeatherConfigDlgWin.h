/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the weather display configuration dialog.
*
*
* Revision Control:
*   12/03/21 v1.4    BTB	Derive from SPxTabbedItemDlg.
*
* Previous Changes:
*   19/02/21 1.3    BTB	Streamline controls.
*   17/02/21 1.2    BTB	Fix build issues.
*   16/02/21 1.1    BTB	Initial version.
*
**********************************************************************/

#pragma once

#include "SPxLibUtils/SPxAutoPtr.h"

/* Include the base class header. */
#include "SPxTabbedDlgWin.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxWeatherDatabase;

/*
 * SPxWeatherConfigDlgWin class.
 */
class SPxWeatherConfigDlgWin : public SPxTabbedItemDlg
{
public:
    /*
     * Public functions.
     */

     /* Dialog Data */
    enum { IDD = IDD_SPX_WEATHER_CONFIG_DIALOG };

    /* Constructor and destructor. */
    explicit SPxWeatherConfigDlgWin(SPxWeatherDatabase *database, CWnd *parentWin = NULL);
    virtual ~SPxWeatherConfigDlgWin(void);
    virtual BOOL Create(void);

    virtual void SyncFromObj(void);
    virtual int GetIDD(void) const { return IDD; }
    virtual SPxErrorCode GetTabName(char *buf, int bufLen) const;

protected:
    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };

private:
    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

     /* Parent window. */
    CWnd *m_parentWin;

    /* Renderer to configure. */
    SPxWeatherDatabase *m_database;

    /* Child controls. */
    CSliderCtrl m_transSlider;
    CButton m_multiColRadio;
    CButton m_monoColRadio;
    CButton m_varTransRadio;
    CButton m_varGainRadio;
    CButton m_varBothRadio;
    CButton m_filledRadio;
    CButton m_outlineRadio;

    /*
     * Private functions.
     */
    void syncControlsToDb(void);
    void syncMultiColControlsToDb(void);
    void syncMonoColControlsToDb(void);
    void updateControls(void);

    /* GUI callback functions. */
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    afx_msg void OnBnClickedMultiColourRadio(void);
    afx_msg void OnBnClickedMonoColourRadio(void);
    afx_msg void OnBnClickedVaryTransparencyRadio(void);
    afx_msg void OnBnClickedVaryGainRadio(void);
    afx_msg void OnBnClickedVaryBothRadio(void);
    afx_msg void OnBnClickedFilledRadio(void);
    afx_msg void OnBnClickedOutlineRadio(void);
    afx_msg void OnBnClickedMonoColourCtrlBtn(void);
    afx_msg void OnIntensityColourChange(UINT id);

    using CDialog::Create;

}; /* class SPxWeatherConfigDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
