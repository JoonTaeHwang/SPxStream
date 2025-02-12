/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for generic ASTERIX version handling.
*
*
* Revision Control:
*   16/01/20 v1.1    REW	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_ASTERIX_VERSION_H
#define _SPX_ASTERIX_VERSION_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxTypes.h"

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

/*
 * Define our class.
 */
class SPxAsterixVersion
{
public:
    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxAsterixVersion(UINT8 major=0, UINT8 minor=0);
    virtual ~SPxAsterixVersion(void);

    /* Setting functions. */
    SPxErrorCode SetVersion(const SPxAsterixVersion *version);
    SPxErrorCode SetVersion(const char *versString);
    SPxErrorCode SetVersion(UINT8 major, UINT8 minor);

    /* Retrieval functions. */
    int FormatVersionString(char *buf, int bufLen);
    UINT8 GetVersionMajor(void) const	{ return(m_major); }
    UINT8 GetVersionMinor(void) const	{ return(m_minor); }
    int IsZero(void) const	{ return((m_major == 0) && (m_minor == 0)); }
    int IsNonZero(void) const	{ return(!IsZero()); }

private:
    /*
     * Private variables.
     */
    /* Version. */
    UINT8 m_major;
    UINT8 m_minor;
}; /* SPxAsterixVersion */

inline bool operator<(const SPxAsterixVersion& a, const SPxAsterixVersion &b)
{
    if( a.GetVersionMajor() < b.GetVersionMajor() )
    {
	return(TRUE);
    }
    if( (a.GetVersionMajor() == b.GetVersionMajor())
		&& (a.GetVersionMinor() < b.GetVersionMinor()) )
    {
	return(TRUE);
    }
    return(FALSE);
}

inline bool operator<=(const SPxAsterixVersion& a, const SPxAsterixVersion &b)
{
    if( a.GetVersionMajor() < b.GetVersionMajor() )
    {
	return(TRUE);
    }
    if( (a.GetVersionMajor() == b.GetVersionMajor())
		&& (a.GetVersionMinor() <= b.GetVersionMinor()) )
    {
	return(TRUE);
    }
    return(FALSE);
}

inline bool operator==(const SPxAsterixVersion& a, const SPxAsterixVersion &b)
{
    if( (a.GetVersionMajor() == b.GetVersionMajor())
	&& (a.GetVersionMinor() == b.GetVersionMinor()) )
    {
	return(TRUE);
    }
    return(FALSE);
}

inline bool operator>(const SPxAsterixVersion& a, const SPxAsterixVersion &b)
{
    if( a.GetVersionMajor() > b.GetVersionMajor() )
    {
	return(TRUE);
    }
    if( (a.GetVersionMajor() == b.GetVersionMajor())
		&& (a.GetVersionMinor() > b.GetVersionMinor()) )
    {
	return(TRUE);
    }
    return(FALSE);
}

inline bool operator>=(const SPxAsterixVersion& a, const SPxAsterixVersion &b)
{
    if( a.GetVersionMajor() > b.GetVersionMajor() )
    {
	return(TRUE);
    }
    if( (a.GetVersionMajor() == b.GetVersionMajor())
		&& (a.GetVersionMinor() >= b.GetVersionMinor()) )
    {
	return(TRUE);
    }
    return(FALSE);
}


#endif /* _SPX_ASTERIX_VERSION_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
