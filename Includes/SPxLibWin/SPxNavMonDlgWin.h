/*********************************************************************
*
* (c) Copyright 2018 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Nav Data Monitoring dialog.
*
*
* Revision Control:
*   01/02/19 v1.3    AGC	Fix N/C #661 - add option to show errors.
*
* Previous Changes:
*   02/01/19 1.2    REW	Support feature restrictions.
*   05/12/18 1.1    REW	Initial Version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibData/SPxNavData.h"
#include "SPxLibWin/SPxTableDlgWin.h"

/* Forward declarations. */
class SPxNavData;
class SPxNavIoDlgWin;
class SPxNavInstantDlgWin;
class SPxNavSentencesDlgWin;
class SPxNavHistoryDlgWin;

/*
 * SPxNavMonDlgWin dialog
 */
class SPxNavMonDlgWin : public SPxTableDlgWin
{
    DECLARE_DYNAMIC(SPxNavMonDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxNavMonDlgWin(CWnd* pParent, SPxNavData *obj,
			     int showErrors);
    virtual ~SPxNavMonDlgWin(void);
    virtual BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Options. */
    void SetDisplayUnits(SPxUnitsDist_t unitsDist,
			 SPxUnitsSpeed_t unitsSpeed);

    /* Dialog Data */
    enum { IDD = IDD_SPX_NAV_MON_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration retrieval functions. */
    SPxNavData *GetObj(void) { return m_obj; }

    /* Child dialog access. */
    SPxNavIoDlgWin *GetChildIo(void)	{ return(m_ioDlg); }
    SPxNavInstantDlgWin *GetChildInstant(void) { return(m_instantDlg); }
    SPxNavSentencesDlgWin *GetChildSentences(void) { return(m_sentencesDlg); }
    SPxNavHistoryDlgWin *GetChildHistory(void) { return(m_historyDlg); }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    void OnTimer(UINT_PTR nIDEvent);
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

    /* Sub dialogs. */
    SPxAutoPtr<SPxNavIoDlgWin> m_ioDlg;
    SPxAutoPtr<SPxNavInstantDlgWin> m_instantDlg;
    SPxAutoPtr<SPxNavSentencesDlgWin> m_sentencesDlg;
    SPxAutoPtr<SPxNavHistoryDlgWin> m_historyDlg;

    /*
     * Private functions
     */


}; /* class SPxNavMonDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
