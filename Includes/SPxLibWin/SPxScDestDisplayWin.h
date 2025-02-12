/*********************************************************************
*
* (c) Copyright 2007 - 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxScDestDisplayWin.h,v $
* ID: $Id: SPxScDestDisplayWin.h,v 1.22 2012/01/13 12:04:53 rew Exp $
*
* Purpose:
*   Header for class to render scan converted data on Windows
*
* Revision Control:
*   13/01/12 v1.38   AGC	Move most functionality to new non-MFC superclass.
*
* Previous Changes:
*   28/10/10 1.37   AGC	Add include guards.
*   17/09/10 1.36   REW	Add m_fadeLevelsRemainder.
*   15/09/10 1.35   REW	Make destructor virtual.
*   09/02/10 1.34   DGJ	Comments.
*   13/01/10 1.17   SP 	Fix layered window bug on Windows 7.
*   19/03/09 1.16   DGJ	Radar Window is now CWnd not CDialog. Tidy comments
*   06/03/09 1.15   REW	Add m_numFadesSinceLastPatch counter.
*   18/02/09 1.14   REW	Support fading at intermediate 8-bit level.
*   22/10/08 1.13   DGJ	Add m_bitmapInfo into class
*   07/01/08 1.12   DGJ	Added GetOverlay and GetUnderlay functions.
*   05/12/07 1.11   DGJ	Move CDC creation to initialisation from Update
*   15/11/07 1.10   DGJ	Changes to accommodate buffered subclass
*			and synchronise fading to patch sets.
*   25/10/07 1.9    REW	Change SetSource() to use UINT32 args.
*   30/09/07 1.8    DGJ	Include afxwin.h not windows.h
*   06/09/07 1.7    DGJ	Support SetSource command to clear bitmap.
*   08/08/07 1.6    DGJ	Synchronise updates to 'update' patches.
*   04/07/07 1.5    DGJ	Make GenerateTestPattern public.
*			Removed Get + SetUpdateInterval, are defined in parent. 
*   29/06/07 1.4    DGJ	Add m_radarUpdate.
*   27/06/07 1.3    DGJ	Take out unused/commented-out code.
*   21/06/07 1.2    DGJ	Moved common code up to DestDisplay.
*   13/06/07 1.1    DGJ	Initial Version
**********************************************************************/
#ifndef _SPX_SC_DEST_DISPLAY_WIN_H
#define _SPX_SC_DEST_DISPLAY_WIN_H

#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibWin/SPxScDestDisplayWinRaw.h"

#include "stdio.h"
#include "afxwin.h"

void SPxWinUpdateThread(void *userData);

class SPxScDestDisplayWin :public SPxScDestDisplayWinRaw
{
public:
    SPxScDestDisplayWin(void);
    virtual ~SPxScDestDisplayWin(void);

    int Create(UCHAR screenID, UINT16 maxScreenW, UINT16 maxScreenH,
			CWnd *underlay = 0, CWnd *overlay = 0);

    CWnd *GetOverlay(void)  {return CWnd::FromHandle(GetOverlayHwnd());}
    CWnd *GetUnderlay(void) {return CWnd::FromHandle(GetUnderlayHwnd());}
    CWnd *GetRadarWindow(void) {return CWnd::FromHandle(GetRadarWindowHwnd());}

};

#endif /* _SPX_SC_DEST_DISPLAY_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
