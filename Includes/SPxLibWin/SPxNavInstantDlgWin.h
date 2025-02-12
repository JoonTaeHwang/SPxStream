/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Nav Data Instantaneous Values child dialog.
*
*
* Revision Control:
*   06/12/18 v1.2    REW	Avoid compiler warning.  Implement contents.
*
* Previous Changes:
*   05/12/18 1.1    REW	Initial Version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibData/SPxNavData.h"

/* Forward declarations. */
class SPxNavData;
class SPxNetCtrlDlgWin;
class SPxSerialCtrlDlgWin;

/*
 * SPxNavInstantDlgWin dialog
 */
class SPxNavInstantDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxNavInstantDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxNavInstantDlgWin(CWnd* pParent, SPxNavData *obj);
    virtual ~SPxNavInstantDlgWin(void);
    virtual BOOL Create(void);
    virtual void SyncFromObj(void);

    void SetDisplayUnits(SPxUnitsDist_t unitsDist,
			 SPxUnitsSpeed_t unitsSpeed);

    /* Dialog Data */
    enum { IDD = IDD_SPX_NAV_INSTANT_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration retrieval functions. */
    SPxNavData *GetObj(void) { return m_obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    virtual HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    DECLARE_MESSAGE_MAP()

private:
    /*
     * Private variables.
     */
    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxNavData *m_obj;

    /* Options. */
    SPxUnitsDist_t m_unitsDist;
    SPxUnitsSpeed_t m_unitsSpeed;

    /* MFC controls. */

    /* Colours for display. */
    COLORREF m_colGood;
    COLORREF m_colBad;
    COLORREF m_colUnknown;
    COLORREF m_colPosition;
    COLORREF m_colSpeed;
    COLORREF m_colCourse;
    COLORREF m_colHeading;
    COLORREF m_colROT;
    COLORREF m_colAltitude;
    COLORREF m_colRoll;
    COLORREF m_colPitch;

    /*
     * Private functions
     */


    /* Avoid warnings about hiding overloaded virtual functions. */
    using CDialog::Create;
}; /* class SPxNavInstantDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
