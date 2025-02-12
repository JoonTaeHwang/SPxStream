/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for LRAD searchlight control dialog.
*
*
* Revision Control:
*   18/03/19 v1.4   BTB 	Disable dialog more neatly.
*
* Previous Changes:
*   18/02/19 1.3   BTB 	Make strobe rates configurable.
*   15/02/19 1.2   BTB 	Add controls to show current state.
*   14/02/19 1.1   BTB 	Initial Version.
*
**********************************************************************/

#pragma once

/* Standard headers. */

/* Library headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN

/* Forward declarations. */
class SPxLRADSearchlightCtrl;

/*
* SPxLRADSearchlightDlgWin dialog
*/
class SPxLRADSearchlightDlgWin : public CDialog
{
public:
    /* Constructor and destructor. */
    SPxLRADSearchlightDlgWin(CWnd *parentWin = NULL,
			     unsigned int strobeRate1 = 1,
			     unsigned int strobeRate2 = 10,
			     unsigned int strobeRate3 = 50);
    virtual ~SPxLRADSearchlightDlgWin(void);
    virtual BOOL Create(CWnd *parent);

    void SetSearchlightController(SPxLRADSearchlightCtrl *controller);

    /* Dialog Data */
    enum { IDD = IDD_SPX_LRAD_SEARCHLIGHT_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ }
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ }
    DECLARE_MESSAGE_MAP()

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Searchlight controller object. */
    SPxLRADSearchlightCtrl *m_searchlightCtrl;

    /* Controls. */
    CButton m_beamEnableBn;
    CStatic m_beamEnableStatusLabel;
    CButton m_beamStrobeEnableBn;
    CStatic m_beamStrobeEnableStatusLabel;
    CStatic m_beamLvlGroup;
    CButton m_beamLowRadio;
    CButton m_beamNormalRadio;
    CButton m_beamHighRadio;
    CStatic m_beamLvlStatusLabel;
    CStatic m_strobeRateGroup;
    CButton m_strobeRadio1;
    CButton m_strobeRadio2;
    CButton m_strobeRadio3;
    CStatic m_strobeRateStatusLabel;
    CStatic m_beamWidthLabel;
    CStatic m_beamWidthStatusLabel;
    CSliderCtrl m_beamWidthSlider;
    CStatic m_floodLabel;
    CStatic m_spotLabel;

    /* Colour of text for status fields. */
    COLORREF m_beamEnableColour;
    COLORREF m_beamStrobeEnableColour;
    COLORREF m_beamLvlColour;
    COLORREF m_strobeRateColour;
    COLORREF m_beamWidthColour;

    /* Private functions. */
    void syncControls(void);

    /* GUI callback functions. */
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    afx_msg void OnOnBnClickedBeamEnableButton(void);
    afx_msg void OnOnBnClickedBeamStrobeEnableButton(void);
    afx_msg void OnOnBnClickedBeamLowButton(void);
    afx_msg void OnOnBnClickedBeamNormalButton(void);
    afx_msg void OnOnBnClickedBeamHighButton(void);
    afx_msg void OnOnBnClickedStrobeButton1(void);
    afx_msg void OnOnBnClickedStrobeButton2(void);
    afx_msg void OnOnBnClickedStrobeButton3(void);

    using CDialog::Create;

};

/*********************************************************************
*
* End of file
*
**********************************************************************/
