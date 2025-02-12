/*********************************************************************
*
* (c) Copyright 2007 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Header file for string utilities.
*
*
* Revision Control:
*   15/01/21 v1.35   AGC	Add SPxStrStarts/EndsWith().
*
* Previous Changes:
*   23/06/20 1.34   AJH	Add SPxCharIsNumber().
*   08/11/19 1.33   AGC	Add SPxCharIs/To...() functions.
*   28/08/19 1.32   AGC	Add SPxStrToUpper/Lower().
*   27/06/19 1.31   SP 	Add SPxStrToValues().
*   14/03/18 1.30   AGC	Support format warnings with clang on Windows.
*   06/03/18 1.29   AGC	Support SPxSnprintf() format warnings.
*   28/11/17 1.28   AGC	Fix SPxStrCpy() for 8/16 conversion in non-Unicode builds.
*   26/07/17 1.27   AGC	Add SPxStrTruncate() for safe ASCII/UTF-8 truncation.
*   23/05/16 1.26   AGC	Fix warnings from Clang on Windows build.
*   11/12/14 1.25   AGC	Allow wide string SPxStrlen in non-unicode build.
*   01/09/14 1.24   AGC	Remove SPxSnprintf for wide strings.
*			Add SPxStrcpy for all strings.
*   10/03/14 1.23   AGC	Only expose wide string functions in Unicode builds.
*   24/02/14 1.22   AGC	Add wide string SPxSnprintf with automatic count.
*   24/02/14 1.21   SP 	Add functions for wide strings on windows.
*   27/01/14 1.20   AGC	Add SPxStrEscapeCtrlChars() function.
*   06/12/13 1.19   AGC	Fix VS 64-bit warning.
*   04/12/13 1.18   AGC	Add SPxStrReplace() function.
*			Add SPxStrReplaceCtrlChars() function.
*   10/10/13 1.17   AGC	Use va_list from cstdarg.
*   08/10/13 1.16   AGC	Fix warning when compiling .NET library.
*   07/10/13 1.15   AGC	Add templated form of SPxSnprintf().
*   04/10/13 1.14   AGC	Simplify headers.
*   26/11/12 1.13   AGC	Add SPxIsspace() function.
*   01/10/12 1.12   AGC	Added SPxVsnprintf().
*   19/09/11 1.11   REW	Need stdio.h.
*   09/06/11 1.10   AGC	Add SPxStrNoCaseCompLen1/2().
*   03/12/10 1.9    AGC	Use _snprintf_s to guarantee NULL termination.
*   26/11/10 1.8    SP  Add SPxStrRemoveWhiteSpace().
*   08/11/10 1.7    AGC	Move SPxDirname() to SPxSysUtils.h
*   28/10/10 1.6    AGC	Add SPxDirname().
*   30/09/10 1.5    SP 	Support Borland build.
*   09/09/08 1.4    REW	Added SPxStrCaseNoCompN().
*   23/01/08 1.3    REW	Added SPxStrdup().
*   25/10/07 1.2    REW	Renamed from SPxStringComp to SPxStrings.
*			Added SPxSnprintf().
*   12/10/07 1.1    DGJ	Initial version
**********************************************************************/

#ifndef _SPX_STRINGS_H
#define _SPX_STRINGS_H

/* Need cstdarg for va_start/va_list/va_end. */
#include <cstdarg>

/* Need stdio.h for snprintf() on Solaris builds. */
#include <stdio.h>

/* Need string.h for strdup on Linux builds. */
#include <string.h>

/* Need SPX_T8 and SPX_T16. */
#include "SPxLibUtils/SPxWideString.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/*********************************************************************
*
*   Macros
*
**********************************************************************/

/*
 * Define SPxSnprintf() and SPxStrdup() according to the OS.
 */
#if defined(_WIN32) && !defined(__BORLANDC__)
/* The safe version of snprintf must be used on Windows because
 * otherwise a NULL terminator will not be appended when
 * the source string is longer than or equal in length to
 * the destination buffer.
 * _snprintf_s returns -1 when truncation occurs unlike the
 * linux snprintf.
 */
inline int SPxVsnprintf(char *str, size_t count, const char *format, std::va_list ap)
{
    return _vsnprintf_s(str, count, _TRUNCATE, format, ap);
}

/* SPxStrlen for wide character strings on Windows. */
inline size_t SPxStrlen(const wchar_t *str)
{
    return wcslen(str);
}

#define	SPxStrdup	_strdup
#else
inline int SPxVsnprintf(char *str, size_t count, const char *format, std::va_list ap)
{
    return vsnprintf(str, count, format, ap);
}
#define	SPxStrdup	strdup
#endif

#ifdef _WIN32
#ifndef __clang__
/* Disable managed code generation for varargs function.
 * This is required to prevent a warning when compiling the .NET library.
 */
#pragma managed(push, off)
#endif
#endif

/* Cross-platform non-templated count form of SPxSnprintf. */
inline int
#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 3, 4)))
#endif
SPxSnprintf(char *str, size_t count, const char *format, ...)
{
    int ret = 0;
    std::va_list ap;
    va_start(ap, format);
    ret = SPxVsnprintf(str, count, format, ap);
    va_end(ap);
    return ret;
}

/* Cross-platform templated count form of SPxVsnprintf. */
template<size_t count>
inline int SPxVsnprintf(char (&str)[count], const char *format, std::va_list ap)
{
    return SPxVsnprintf(str, count, format, ap);
}

/* Cross-platform templated count form of SPxSnprintf. */
template<size_t count>
inline int
#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 2, 3)))
#endif
SPxSnprintf(char (&str)[count], const char *format, ...)
{
    int ret = 0;
    std::va_list ap;
    va_start(ap, format);
    ret = SPxVsnprintf(str, count, format, ap);
    va_end(ap);
    return ret;
}

/* Cross-platform form of SPxStrlen. */
inline size_t SPxStrlen(const char *str)
{
    return strlen(str);
}

/* Cross platform versions of SPxStrcpy. */
inline char *SPxStrcpy(char *dst, size_t count, const char *src)
{
    SPxSnprintf(dst, count, "%s", src);
    return dst;
}

#ifdef _WIN32

inline char *SPxStrcpy(char *dst, size_t count, const wchar_t *src)
{
    return SPxStrcpy(dst, count, SPX_TA(src));
}

inline wchar_t *SPxStrcpy(wchar_t *dst, size_t count, const wchar_t *src)
{
#if (_MSC_VER >= 1900)
    _snwprintf_s(dst, count, _TRUNCATE, L"%ls", src);
#else
    _snwprintf_s(dst, count, _TRUNCATE, L"%s", src);
#endif
    return dst;
}

inline wchar_t *SPxStrcpy(wchar_t *dst, size_t count, const char *src)
{
    return SPxStrcpy(dst, count, SPX_TW(src));
}

#endif

template<typename C1, typename C2, size_t count>
inline C1 *SPxStrcpy(C1 (&dst)[count], const C2 *src)
{
    return SPxStrcpy(dst, count, src);
}

unsigned int SPxStrToValues(const char *str, 
                            double *val1, 
                            double *val2=NULL, 
                            double *val3=NULL);

unsigned int SPxStrToValues(const char *str, 
                            unsigned int *val1, 
                            unsigned int *val2=NULL, 
                            unsigned int *val3=NULL);

#ifdef _WIN32
#ifndef __clang__
/* Restore the managed code generation state. */
#pragma managed(pop)
#endif
#endif

/*********************************************************************
*
*   Variable definitions
*
**********************************************************************/

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* Extern the functions we provide. */
extern int SPxCharIsAlnum(int ch);
extern int SPxCharIsDigit(int ch);
extern int SPxCharIsSpace(int ch);
extern int SPxCharToLower(int ch);
extern int SPxCharToUpper(int ch);
extern int SPxIsspace(int ch);
extern int SPxCharIsNumber(int ch);
extern int SPxStrNoCaseComp(const char *str1, const char *str2);
extern int SPxStrNoCaseCompN(const char *str1, const char *str2, int n);
extern int SPxStrNoCaseCompLen1(const char *str1, const char *str2);
extern int SPxStrNoCaseCompLen2(const char *str1, const char *str2);
extern void SPxStrRemoveWhiteSpace(const char *srcStr, char *dstStr, 
                                   int keepNewLine=0);
extern int SPxStrReplaceCtrlChars(char *str);
extern int SPxStrEscapeCtrlChars(char *buf, int bufLen, const char *str);
extern int SPxStrReplace(char *buf, unsigned int bufLen,
			 const char *str, const char *oldStr,
			 const char *newStr);

extern void SPxStrTruncate(char *dst, size_t count, const char *src);
template<size_t count>
inline void SPxStrTruncate(char(&dst)[count], const char *src)
{
    return SPxStrTruncate(dst, count, src);
}
extern void SPxStrToUpper(char *str);
extern void SPxStrToLower(char *str);
extern int SPxStrStartsWith(const char *str, const char *prefix);
extern int SPxStrEndsWith(const char *str, const char *suffix);

#endif /* _SPX_STRINGS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
