/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for applications wishing to use GTK+ with warnings
*	triggered by the GTK+ headers automatically disabled.
*
*
* Revision Control:
*   23/01/20 v1.2    AGC	Remove glade header.
*
* Previous Changes:
*   22/01/20 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_GTK_INCLUDES_H
#define _SPX_GTK_INCLUDES_H

/* Disable shift into sign bit warning from GTK. */
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshift-sign-overflow"
#endif

/* Disable unnecessary parentheses warning from GTK. */
#if defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#if (__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
#pragma GCC diagnostic push
#endif
#if (__GNUC__ >= 8)
#pragma GCC diagnostic ignored "-Wparentheses"
#endif
#if (__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 2))
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#endif

/* Include the GTK+ headers. */
#include <gtk/gtk.h>		/* Standard definitions */
#include <gdk/gdkx.h>		/* X-specific definitions */

#if defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#if (__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
#pragma GCC diagnostic pop
#endif
#endif
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _SPX_GTK_INCLUDES_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
