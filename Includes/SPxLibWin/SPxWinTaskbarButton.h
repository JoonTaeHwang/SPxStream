/*********************************************************************
*
* (c) Copyright 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxWinTaskbarButton.h,v $
* ID: $Id: SPxWinTaskbarButton.h,v 1.1 2014/04/23 08:49:26 rew Exp $
*
* Purpose:
*   Header for Windows taskbar button interaction.
*
*
* Revision Control:
*   23/04/14 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/
#ifndef _SPX_WIN_TASKBAR_BUTTON_H
#define _SPX_WIN_TASKBAR_BUTTON_H

/* For HWND. */
#include "SPxLibUtils/SPxCommon.h"

/* For SPxAutoPtr class. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/* Macro for responding to thumb button clicks with message map. */
#define ON_SPX_TB_THUMB(id, memberFxn) \
	{ WM_COMMAND, (WORD)0x1800, (WORD)id, (WORD)id, AfxSigCmd_v, \
		(static_cast< AFX_PMSG > (memberFxn)) },

class SPxWinTaskbarButton : public SPxObj
{
public:
    explicit SPxWinTaskbarButton(HWND hwnd);
    virtual ~SPxWinTaskbarButton(void);

    SPxErrorCode AddThumbButton(UINT id, int iconID, const char *tooltip);
    SPxErrorCode EnableThumbButton(UINT id, int enabled);
    SPxErrorCode ShowThumbButton(UINT id, int show);

    SPxErrorCode SetProgress(float percent);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    static LRESULT CALLBACK hookFn(int code, WPARAM wParam, LPARAM lParam);
    void create(void);

}; /* SPxWinTaskbarButton */

#endif /* _SPX_WIN_TASKBAR_BUTTON_H */
/*********************************************************************
*
* End of file
*
**********************************************************************/

