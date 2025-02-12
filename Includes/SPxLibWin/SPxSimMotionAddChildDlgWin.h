/*********************************************************************
*
* (c) Copyright 2014, 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxSimMotionAddChildDlgWin.h,v $
* ID: $Id: SPxSimMotionAddChildDlgWin.h,v 1.4 2016/08/23 14:48:12 rew Exp $
*
* Purpose:
*	Header for the dialog to add a child simulated motion to a motion.
*
*
* Revision Control:
*   23/08/16 v1.4    AGC	Event function prototype updated.
*
* Previous Changes:
*   03/03/16 1.3    AGC	Fix clang warnings.
*   07/11/14 1.2    AGC	Update radar sim event handler.
*   02/06/14 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* Forward declarations. */
class SPxSimMotionCtrlDlgWin;
class SPxRadarSimulator;
class SPxSimMotionRenderer;
class SPxSimMotion;
struct SPxRadarSimEventParams;

/*
 * SPxSimMotionAddChildDlgWin dialog
 */
class SPxSimMotionAddChildDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxSimMotionAddChildDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxSimMotionAddChildDlgWin(SPxSimMotionCtrlDlgWin *parent,
	SPxRadarSimulator *obj, SPxSimMotionRenderer *renderer=NULL);
    virtual ~SPxSimMotionAddChildDlgWin(void);
    BOOL Create(void);
    void SyncFromObj(void);
    SPxErrorCode SetCurrMotion(SPxSimMotion *motion, SPxSimSegment *segment);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_MOTION_ADD_CHILD_DIALOG };

protected:
    virtual void OnOK(void){};
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()

private:
    /* The object we are controlling */
    SPxSimMotionCtrlDlgWin *m_parentWin;
    SPxRadarSimulator *m_obj;
    SPxSimMotionRenderer *m_renderer;
    SPxSimMotion *m_currMotion;
    SPxSimSegment *m_currSegment;
    int m_motionExists;

    /* Controls. */
    CComboBox m_comboMotions;
    CButton m_btnInsert;
    CButton m_btnAppend;

    /* Other variables. */
    int m_syncRequired;

    /*
     * Handler functions.
     */
    void OnInsert(void);
    void OnAppend(void);
    static SPxErrorCode motionFn(unsigned int index, SPxSimMotion *motion, void *userArg);
    static SPxErrorCode addMotionToCombo(unsigned int index, SPxSimMotion *motion, void *userArg);
    static SPxErrorCode eventFnWrapper(UINT32 flags, const SPxRadarSimEventParams *params, void *userArg);
    SPxErrorCode eventFn(UINT32 flags);

    using CDialog::Create;

}; /* SPxSimMotionAddChildDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
