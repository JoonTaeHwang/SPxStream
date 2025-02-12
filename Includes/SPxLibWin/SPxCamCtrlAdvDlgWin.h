/*********************************************************************
*
* (c) Copyright 2014 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Camera controller child dialog.
*
*
* Revision Control:
*   08/01/21 v1.6    BTB	Support setting primary and secondary Pelco addresses at the same time.
*
* Previous Changes:
*   02/07/18 1.5    AGC	Add Onvif support.
*   20/04/17 1.4    AGC	Add IsSupported callback.
*   31/10/16 1.3    AGC	Support slave controllers.
*   08/04/16 1.2    AGC	Support network control.
*   17/02/14 1.1    AGC	Initial version.
*
**********************************************************************/

#pragma once

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxCameraType enumeration. */
#include "SPxLibData/SPxCamCtrlBase.h"

/* Include the base class header. */
#include "SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxCamCtrlBase;
class SPxCamCtrlDlgWin;

/*
 * SPxCamCtrlAdvDlgWin dialog
 */
class SPxCamCtrlAdvDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxCamCtrlAdvDlgWin)

public:
    typedef SPxCamCtrlBase* (*CreateCamCtrl)(SPxCameraType camType,
					     unsigned int index,
					     void *userArg);
    typedef int (*IsSupportedCamCtrl)(SPxCameraType camType,
				      void *userArg);

    /* Constructor and destructor. */
    explicit SPxCamCtrlAdvDlgWin(SPxCamCtrlDlgWin *pParent, SPxCamCtrlBase *camCtrl,
				 unsigned int index=0,
				 CreateCamCtrl createFn=NULL, void *createUserArg=NULL,
				 IsSupportedCamCtrl isSupportedFn=NULL, void *isSupportedUserArg=NULL,
				 int showSlaveOption=FALSE, int isDualMode=FALSE);
    virtual ~SPxCamCtrlAdvDlgWin(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_CAM_CONTROLLERS_DIALOG };

    void SetObj(SPxCamCtrlBase *camCtrl);
    SPxCamCtrlBase *GetObj(void);

    /* Sync functions. */
    virtual void SyncFromObj(void);
    virtual int GetIDD(void) const { return IDD; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK(void) {}
    virtual void OnCancel(void) {}
    DECLARE_MESSAGE_MAP()

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    void OnChangeType(void);
    void OnChangeMesgType(void);
    void OnChangeAddr(NMHDR*, LRESULT*);
    void OnChangeSecAddr(NMHDR*, LRESULT*);
    void OnChangeIfaceType(void);

    SPxErrorCode createCamCtrl(void);
    SPxErrorCode createCamCtrlComplete(void);
    void repopulateCtrlTypeCombo(void);
    static int camCtrlDeleted(void *invokingObject,
			      void *userObject,
			      void *arg);

}; /* class SPxCamCtrlAdvDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/
