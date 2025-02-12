/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for applications wishing to use the Cairo and
*	GdkPixbuf helper/utility functions in the SPx library.
*
*	Avoid adding GDK/GTK+ dependencies to this file!
*
* Revision Control:
*   10/08/21 v1.2    SP 	Return error code in utility functions.
*
* Previous Changes:
*   09/08/21 1.1    SP 	Initial version.
*
**********************************************************************/

#ifndef _SPX_CAIRO_H
#define _SPX_CAIRO_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxError.h"

/* Headers for cairo. */
#include <cairo/cairo.h>

/* Headers for GdkPixbuf. */
#include <gdk-pixbuf/gdk-pixbuf.h>

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*********************************************************************
*
*   Macros
*
**********************************************************************/

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/*
* Helper/utility functions.
*/

extern SPxErrorCode SPxCairoSetSourcePixbuf(cairo_t *cr,
                                            const GdkPixbuf *pixbuf,
                                            double pixbufX,
                                            double pixbufY);

extern SPxErrorCode SPxCairoSurfacePaintPixbuf(cairo_surface_t *surface,
                                               const GdkPixbuf *pixbuf);

#endif /* _SPX_CAIRO_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
