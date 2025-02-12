/*********************************************************************
*
* (c) Copyright 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxWinMsgBox.h,v $
* ID: $Id: SPxWinMsgBox.h,v 1.1 2014/04/23 08:49:26 rew Exp $
*
* Purpose:
*   Header for Windows Message Box functions.
*
*
* Revision Control:
*   23/04/14 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/
#ifndef _SPX_WIN_MSG_BOX_H_
#define	_SPX_WIN_MSG_BOX_H_

/* For HWND. */
#include "SPxLibUtils/SPxCommon.h"

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* Display a message box. */
extern int SPxWinMsgBox(HWND hWnd, const char *title,
			const char *text, const char *subText, UINT type,
			UINT btnType1=0, const char *btnText1=NULL,
			UINT btnType2=0, const char *btnText2=NULL,
			UINT btnType3=0, const char *btnText3=NULL);


#endif /* _SPX_WIN_MSG_BOX_H_ */
/*********************************************************************
*
* End of file
*
**********************************************************************/
