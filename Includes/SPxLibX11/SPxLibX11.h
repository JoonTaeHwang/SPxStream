/*********************************************************************
*
* (c) Copyright 2007 - 2018, 2020, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Top-level header for SPxLibX11 library.
*
*   This should be the only header that needs to be included in
*   code that uses this library.  It includes all the other public
*   headers for the library.
*
*
* Revision Control:
*   02/09/21 v1.26   AGC	Use SPX_GTK_SUPPORTED flag.
*
* Previous Changes:
*   13/10/20 1.25   REW	Add SPxAeroRendererGtk.h
*   22/02/18 1.24   REW	Add SPxDFRendererGtk.h
*   13/11/15 1.23   REW	Add SPxPlotRendererGtk.h
*   15/05/15 1.22   JP	Add SPxRasterMapRendererGtk.h
*   22/09/14 1.22   REW	Add SPxSafetyNetRendererGtk.h
*   07/10/13 1.21   AGC	Sort headers.
*   04/10/13 1.20   AGC	Move functions to SPxX11Init.h.
*   03/01/13 1.19   SP 	Add SPxGwPicture.h
*   23/11/12 1.18   AGC	Add SPxMouseControlGtk.h
*   09/11/12 1.17   AGC	Add SPxSoftButtonGtk.h
*			Add SPxViewControlRendererGtk.h
*   21/06/12 1.16   SP 	Add SPxWorldMapRendererGtk.h
*                       Add SPxOwnShipRendererGtk.h
*   06/06/12 1.15   SP 	Add SPxTiledMapRendererGtk.h
*   08/02/12 1.14   AGC	Add SPxAreaRendererGtk.h
*   23/12/11 1.13   REW	Add SPxADSBRendererGtk.h
*   26/04/11 1.12   SP 	Add SPxGraphicsRendererGtk.h
*   12/04/11 1.11   SP  Add SPxTrackRendererGtk.h
*   25/03/11 1.10   SP 	Exclude OpenGL headers in Solaris build.
*   01/03/11 1.9    REW	Add SPxAISRendererGtk.h
*   28/01/11 1.8    AGC	Add SPxPPIwindowGlx.h, SPxPPIwindowTsi.h,
*			SPxGlxBitmapTex.h and SPxGlxFunctions.h
*   22/03/10 1.7    SP 	Add SPxGlxMixer.h and SPxPPIwindowGLX.h
*   27/11/08 1.6    REW	Add SPxScreenRecord.h and SPxScreenReplay.h
*   13/02/08 1.5    DGJ	Add SPxPPI.h	
*   09/07/07 1.4    REW	Args to SPxLibX11Init().
*   12/06/07 1.3    REW	Add SPxScFollowX11.h
*   07/06/07 1.2    REW	Add headers.
*   24/05/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_LIBX11_H_
#define	_SPX_LIBX11_H_

/* Include all the public headers from this library, assuming they
 * are in a subdirectory of one of the directories in the include
 * path (i.e. so you don't need to add each subdirectory to the include
 * path, just the parent of them all).
 */
#include "SPxLibUtils/SPxCommon.h"
#if SPX_GTK_SUPPORTED
#include "SPxLibX11/SPxADSBRendererGtk.h"
#include "SPxLibX11/SPxAeroRendererGtk.h"
#include "SPxLibX11/SPxAISRendererGtk.h"
#include "SPxLibX11/SPxAreaRendererGtk.h"
#include "SPxLibX11/SPxDFRendererGtk.h"
#include "SPxLibX11/SPxGraphicsRendererGtk.h"
#include "SPxLibX11/SPxGwPicture.h"
#include "SPxLibX11/SPxMouseControlGtk.h"
#include "SPxLibX11/SPxOwnShipRendererGtk.h"
#include "SPxLibX11/SPxPlotRendererGtk.h"
#include "SPxLibX11/SPxPPI.h"
#include "SPxLibX11/SPxRasterMapRendererGtk.h"
#include "SPxLibX11/SPxSafetyNetRendererGtk.h"
#include "SPxLibX11/SPxScDestDisplayX11.h"
#include "SPxLibX11/SPxScFollowX11.h"
#include "SPxLibX11/SPxScreenRecord.h"
#include "SPxLibX11/SPxScreenReplay.h"
#include "SPxLibX11/SPxSoftButtonGtk.h"
#include "SPxLibX11/SPxTiledMapRendererGtk.h"
#include "SPxLibX11/SPxTrackRendererGtk.h"
#include "SPxLibX11/SPxViewControlRendererGtk.h"
#include "SPxLibX11/SPxWorldMapRendererGtk.h"
#include "SPxLibX11/SPxX11Init.h"
#endif

/* The following headers are excluded from the Solaris build. */
#ifndef __sun
#include "SPxLibX11/SPxGlxBitmapTex.h"
#include "SPxLibX11/SPxGlxFunctions.h"
#include "SPxLibX11/SPxGlxMixer.h"
#include "SPxLibX11/SPxPPIwindowGlx.h"
#include "SPxLibX11/SPxPPIwindowTsi.h"
#endif

#endif /* _SPX_LIBX11_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
