/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for applications wishing to use Glade with warnings
*	triggered by the Glade headers automatically disabled.
*
*
* Revision Control:
*   23/01/20 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_GLADE_INCLUDES_H
#define _SPX_GLADE_INCLUDES_H

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

/* Include the glade headers. */
#include <glade/glade.h>

#if defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#if (__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
#pragma GCC diagnostic pop
#endif
#endif
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif /* _SPX_GLADE_INCLUDES_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
