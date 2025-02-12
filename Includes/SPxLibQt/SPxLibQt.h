/*********************************************************************
*
* (c) Copyright 2013, 2015, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Top-level header for SPxLibQt library.
*
*   This should be the only header that needs to be included in
*   code that uses this library.  It includes all the other public
*   headers for the library.
*
* Revision Control:
*   08/10/21 v1.14   SP 	Include QOpenGLContext to fix clash in Qt6.
*
* Previous Changes:
*   06/10/21 1.13   SP 	Fix issue with maths symbol redefinitions on Qt6.
*   05/10/21 1.12   AGC	Exclude SPxQGL classes on Qt6.
*   10/05/21 1.11   AGC	Include QTextStream to avoid build errors.
*   05/05/21 1.10   AGC	Exclude SPxQOpenGLRadarWidget where not supported.
*   28/04/21 1.9    AGC	Add SPxQOpenGLRadarWidget.
*   22/04/15 1.8    JP 	Add SPxRasterMapRendererQt.
*   08/04/15 1.7    JP 	Add SPxOwnShipRendererQt, SPxAISRendererQt
*			SPxExtRendererQt, SPxWorldMapRendererQt.
*   05/02/15 1.6    JP 	Add SPxPopupMenuQt.
*   26/01/15 1.5    JP 	Add Softbutton and mousecontrol.
*   07/01/15 1.4    JP 	Add renderers.
*   06/06/13 1.3    SP 	Add SPxQGLRadarVideoItem.
*			Add SPxQGLRadarWidget.
*			Fix Qt5 issues with QVariant.
*   03/05/13 1.2    SP 	Add SPxQRadarWidget.
*   25/04/13 1.1    SP	Initial version.
*
**********************************************************************/

#ifndef _SPX_LIBQT_H_
#define	_SPX_LIBQT_H_

/* Standard headers. */
#define _USE_MATH_DEFINES  /* Avoid clash with qmath.h */
#include <math.h>

/* Including this after SPx.h, which defines bool and many other
 * common types, causes all sorts of problems. Therefore this header
 * is included first.
 */
#include <QVariant>

/* Include Qt header for version checking. */
#include <QObject>

/* Include QTextStream header to avoid build errors when this header
 * gets included later.
 */
#include <QTextStream>

/* Include OpenGL header to avoid clash with OpenGL types in SPxGL.h with Qt6. */
#include <QOpenGLContext>

/* We need the main SPx header first (for all the utility classes etc). */
#ifdef _WIN32
#include "SPxNoMFC.h"
#else
#include "SPx.h"
#endif

/* Include all the public headers from this library, assuming they
 * are in a subdirectory of one of the directories in the include
 * path (i.e. so you don't need to add each subdirectory to the include
 * path, just the parent of them all).
 */
#include "SPxLibQt/SPxAISRendererQt.h"
#include "SPxLibQt/SPxButtonBoxQt.h"
#include "SPxLibQt/SPxExtRendererQt.h"
#include "SPxLibQt/SPxGraphicsRendererQt.h"
#include "SPxLibQt/SPxMouseControlQt.h"
#include "SPxLibQt/SPxOwnShipRendererQt.h"
#include "SPxLibQt/SPxPopupMenuQt.h"
#if QT_VERSION < 0x060000
#include "SPxLibQt/SPxQGLRadarWidget.h"
#endif
#if QT_VERSION >= 0x050300
#include "SPxLibQt/SPxQOpenGLRadarWidget.h"
#endif
#include "SPxLibQt/SPxQRadarWidget.h"
#if QT_VERSION < 0x060000
#include "SPxLibQt/SPxQGLRadarVideoItem.h"
#endif
#include "SPxLibQt/SPxQRadarVideoItem.h"
#include "SPxLibQt/SPxQUtils.h"
#include "SPxLibQt/SPxRasterMapRendererQt.h"
#include "SPxLibQt/SPxRendererQt.h"
#include "SPxLibQt/SPxSoftButtonQt.h"
#include "SPxLibQt/SPxTiledMapRendererQt.h"
#include "SPxLibQt/SPxTrackRendererQt.h"
#include "SPxLibQt/SPxViewControlRendererQt.h"
#include "SPxLibQt/SPxWorldMapRendererQt.h"

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_LIBQT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
