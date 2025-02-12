/*********************************************************************
*
* (c) Copyright 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxAutoDC.h,v $
* ID: $Id: SPxAutoDC.h,v 1.1 2012/04/04 16:25:05 rew Exp $
*
* Purpose:
*	SPxAutoDC class for automatically releasing a device context.
*
* Revision Control:
*   04/04/12 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/
#ifndef _SPX_AUTO_DC_H
#define _SPX_AUTO_DC_H

/* For HWND and HDC types. */
#include "SPxLibUtils/SPxCommon.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/*
 * Define our class.
 */
class SPxAutoDC
{
public:
    SPxAutoDC(HWND hwnd)
    {
	m_hwnd = hwnd;
	m_dc = GetDC(m_hwnd);
    }
    ~SPxAutoDC(void)
    {
	if( m_dc )
	{
	    ReleaseDC(m_hwnd, m_dc);
	}
    }
    operator HDC()
    {
	return m_dc;
    }
    int IsNull(void)
    {
	return (m_dc == NULL);
    }
private:
    HWND m_hwnd;
    HDC m_dc;

}; /* SPxAutoDC */

#endif /* _SPX_AUTO_DC_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
