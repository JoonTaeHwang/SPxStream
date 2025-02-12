/*********************************************************************
*
* (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header containing classes for converting to/from wide strings
*   on Windows.
*
* Revision Control:
*   08/08/22 v1.17   BTB	Move SPxGetCharacter to SPxCharUtils.
*
* Previous Changes:
*   07/12/20 1.16   AGC	Add empty SPX_TW/SPX_TA macros for Linux.
*   28/08/20 1.15   AGC	Move some functions to source file.
*   06/07/20 1.14   SP 	Add up and down arrows.
*   23/05/19 1.13   BTB	Add L-R and R-L markers.
*   18/07/17 1.12   AGC	Fix conversion when truncation required.
*   09/05/17 1.11   AGC	Fix cppcheck warnings.
*   07/11/16 1.10   AGC	Allow UTF-8 or ACP strings internally.
*			Simplify SPxTBase and improve layout.
*   21/05/15 1.9    AGC	Add pound character.
*   20/02/15 1.8    AGC	Add SPxT8toT16 and SPxT16toT8 classes for non-Unicode builds.
*   01/09/14 1.7    AGC	VS2015 format string changes.
*   04/07/14 1.6    AGC	Remove need for m_short flag.
*   15/01/14 1.5    AGC	Add optionally included CString constructors.
*			Move SPxGetCharacter() here from SPxSysUtils.h.
*			Fix broken truncated UTF-8 strings when converting.
*   08/11/13 1.4    AGC	Fix cppcheck warnings.
*   27/09/13 1.3    AGC	Add SPX_CHAR type.
*   01/08/13 1.2    AGC	Test for Unicode using UNICODE or _UNICODE.
*   30/07/13 1.1    AGC	Initial version.
*
**********************************************************************/
#ifndef SPX_WIDE_STRING_H
#define SPX_WIDE_STRING_H

/*
 * Other headers required.
 */

#ifdef _WIN32
#include "tchar.h"
#endif

#include <cstring>  /* For std::memset, std::strlen. */
#include "SPxLibUtils/SPxTypes.h" /* For SPX_FUNCTION_DELETE. */

/* SPxGetCharacter has been moved from here to SPxCharUtils,
 * so the header is included here for backward compatibility.
 */
#include "SPxLibUtils/SPxCharUtils.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

#ifdef _WIN32
typedef TCHAR SPX_CHAR;
#else
typedef char SPX_CHAR;
#endif

/* SPxTAtoT16 and SPxT16toTA should only be available under Windows. */
#ifdef _WIN32

#define SPX_CP_UTF8 65001

/* Base class for string conversions. */
template<typename I, typename O>
class SPxTBase
{
public:
    /* Public functions. */
    SPxTBase(void) : m_ostr(NULL)
    {
	std::memset(m_shortOstr, 0, sizeof(m_shortOstr));
    }
    SPxTBase(const O *str) : m_ostr(str)
    {
	std::memset(m_shortOstr, 0, sizeof(m_shortOstr));

	/* Mark that m_ostr should not be freed. */
	m_shortOstr[0] = TRUE;
    }
    virtual ~SPxTBase(void)
    {
	if(m_ostr && !m_shortOstr[0])
	{
	    delete[] m_ostr;
	}
	m_ostr = NULL;
    }
    operator const O*(void) const
    {	
	return m_ostr ? m_ostr : m_shortOstr;
    }

protected:
    /* Protected functions. */
    void DoConversion(const I *str, int codePage);

private:
    /* Private variables. */
    O m_shortOstr[32 - (sizeof(O*) / sizeof(O))];	/* Short output string. */
    const O *m_ostr;				/* Long output string. */

    /* Private functions (disable copying). */
    SPxTBase(const SPxTBase&) SPX_FUNCTION_DELETE;
    SPxTBase& operator=(const SPxTBase&) SPX_FUNCTION_DELETE;

    /* Function to do conversion. */
    virtual int Convert(int codePage, const I *str, O *ostr,
			std::size_t numChars, int numInChars, int flags)=0;
    virtual int TruncateConvert(int /*codePage*/, const I * /*str*/, int /*flags*/, std::size_t& /*len*/)
    {
	return -1;
    }

}; /* SPxTBase */

/* Translate ACP/UTF-8 -> UTF-16. */
class SPxTAtoT16 : public SPxTBase<char, wchar_t>
{
public:
    /* Public functions. */
    SPxTAtoT16(const char *str, int codePage=SPX_CP_UTF8) { DoConversion(str, codePage); }
#ifdef __CSTRINGT_H__
    SPxTAtoT16(const CStringA &str, int codePage=SPX_CP_UTF8) { DoConversion(static_cast<LPCSTR>(str), codePage); }
#endif
    SPxTAtoT16(const wchar_t *str) : SPxTBase(str) {}
#ifdef __CSTRINGT_H__
    SPxTAtoT16(const CStringW &str) : SPxTBase(static_cast<LPCWSTR>(str)) {}
#endif
    virtual ~SPxTAtoT16(){}

private:
    virtual int Convert(int codePage, const char *str, wchar_t *ostr,
			std::size_t numChars, int numInChars, int errFlag) SPX_VIRTUAL_OVERRIDE;

    virtual int TruncateConvert(int codePage, const char *str, int errFlag, std::size_t& len) SPX_VIRTUAL_OVERRIDE;

}; /* SPxTAtoT16 */

/* Translate UTF-16 -> ACP/UTF-8. */
class SPxT16toTA : public SPxTBase<wchar_t, char>
{
public:
    /* Public functions. */
    SPxT16toTA(const wchar_t *str, int codePage=SPX_CP_UTF8) { DoConversion(str, codePage); }
#ifdef __CSTRINGT_H__
    SPxT16toTA(const CStringW &str, int codePage=SPX_CP_UTF8) { DoConversion(static_cast<LPCWSTR>(str), codePage); }
#endif
    SPxT16toTA(const char *str) : SPxTBase(str) {}
#ifdef __CSTRINGT_H__
    SPxT16toTA(const CStringA &str) : SPxTBase(static_cast<LPCSTR>(str)) {}
#endif
    virtual ~SPxT16toTA(){}

private:
    virtual int Convert(int codePage, const wchar_t *str, char *ostr,
			std::size_t numChars, int numInChars, int errFlag) SPX_VIRTUAL_OVERRIDE;

}; /* SPxTAtoT16 */

/* Convenient name for classes above. */
#define SPX_TW(val) ((const wchar_t*)SPxTAtoT16(val))
#define SPX_TA(val) ((const char*)SPxT16toTA(val))

#endif

/* Text UTF-8 <--> UTF-16 transformations. */
#if defined(__BORLANDC__) || defined(_UNICODE) || defined(UNICODE)

/* Translate UTF-8 -> UTF-16. */
class SPxT8toT16 : public SPxTAtoT16
{
public:
    /* Public functions. */
    SPxT8toT16(const char *str) : SPxTAtoT16(str) {}
    SPxT8toT16(const wchar_t *wstr) : SPxTAtoT16(wstr) {}
    virtual ~SPxT8toT16(){}

}; /* SPxT8toT16 */

/* Translate UTF-16 -> UTF-8 */
class SPxT16toT8 : public SPxT16toTA
{
public:
    SPxT16toT8(const wchar_t *wstr) : SPxT16toTA(wstr) {}
    SPxT16toT8(const char *str) : SPxT16toTA(str) {}
    virtual ~SPxT16toT8(){}

}; /* SPxT16toT8 */

/* Convenient names for classes above. */
#define SPX_T16(val) ((const TCHAR*)SPxT8toT16(val))
#define SPX_T8(val) ((const char*)SPxT16toT8(val))
#else

#ifdef _WIN32
/* Define SPxT8toT16/SPxT16toT8 classes that can be 
 * used in non-unicode builds so that generically transformed
 * strings can be stored without having to use CString.
 */
typedef SPxT16toTA SPxT8toT16;
typedef SPxTAtoT16 SPxT16toT8;

/* Convert to UTF-16 and back again, so that UTF-8 and local code page strings
 * area all converted to local code page as best as is possible.
 */
#define SPX_T16(val) ((const TCHAR*)SPxT16toTA(SPxTAtoT16(val), CP_ACP))
#define SPX_T8(val) val /* Do nothing. */

#else

#define SPX_T16(val) val /* Do nothing. */
#define SPX_T8(val) val /* Do nothing. */
#define SPX_TW(val) val /* Do nothing. */
#define SPX_TA(val) val /* Do nothing. */

#endif

#endif

#endif /* SPX_WIDE_STRING_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
