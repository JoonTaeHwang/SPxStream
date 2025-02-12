/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for popup version of dialog for record and replay of
*	camera positions.
*
*
* Revision Control:
*   18/02/22 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/
#pragma once

/* Include the base class header file */
#include "SPxCamRecordDlgWin.h"

class SPxCamRecordPopupDlgWin : public SPxCamRecordDlgWin
{
public:
    /* Constructor and destructor. */
    explicit SPxCamRecordPopupDlgWin(SPxRecord *camRecorder,
				     SPxPacketDecoderFile *camReplay);
    virtual ~SPxCamRecordPopupDlgWin(void);

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

public:
    /* Nothing at present */

};

/*********************************************************************
*
* End of file
*
**********************************************************************/
