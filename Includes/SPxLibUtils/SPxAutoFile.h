/*********************************************************************
*
* (c) Copyright 2014, 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxAutoFile.h,v $
* ID: $Id: SPxAutoFile.h,v 1.3 2017/05/09 13:23:46 rew Exp $
*
* Purpose:
*   Header for SPxAutoFile class that automatically closes a file
*   on destruction.
*
* Revision Control:
*   09/05/17 v1.3    AGC	Fix cppcheck warnings.
*
* Previous Changes:
*   13/03/14 1.2    AGC	Make default constructor implicit.
*   10/03/14 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_AUTO_FILE_H
#define _SPX_AUTO_FILE_H

#include <cstdio>

/* For SPX_FUNCTION_DELETE. */
#include "SPxLibUtils/SPxTypes.h"

class SPxAutoFile
{
public:
    SPxAutoFile(std::FILE *file=NULL) : m_file(file) {}
    ~SPxAutoFile(void) { reset(); }

    std::FILE& operator*() { return *get(); }
    const std::FILE& operator*() const { return *get(); }
    std::FILE* operator->() { return get(); }
    const std::FILE* operator->() const { return get(); }
    std::FILE** operator&() { return &m_file; }
    std::FILE* const * operator&() const { return &m_file; }
    std::FILE* get() { return m_file;  }
    const std::FILE* get() const { return m_file; }
    operator std::FILE*() { return get(); }
    operator const std::FILE*() const { return get(); }
    std::FILE* release()
    {
	std::FILE* file = get();
	m_file = NULL;
	return file;
    }

    void reset(std::FILE* file=NULL)
    {
	std::FILE* tmp(m_file);
	m_file = file;
	file = tmp;
	if( file != NULL )
	{
	    std::fclose(file);
	}
    }

private:
    std::FILE *m_file;

    /* Disable copy and assignment operators. */
    SPxAutoFile(const SPxAutoFile&) SPX_FUNCTION_DELETE;
    SPxAutoFile& operator=(const SPxAutoFile&) SPX_FUNCTION_DELETE;
};

inline bool operator==(const SPxAutoFile& a, const SPxAutoFile& b)
{
    return a.get() == b.get();
}

inline bool operator!=(const SPxAutoFile& a, const SPxAutoFile& b)
{
    return a.get() != b.get();
}

inline bool operator<(const SPxAutoFile& a, const SPxAutoFile& b)
{
    return a.get() < b.get();
}

inline bool operator<=(const SPxAutoFile& a, const SPxAutoFile& b)
{
    return a.get() <= b.get();
}

inline bool operator>(const SPxAutoFile& a, const SPxAutoFile& b)
{
    return a.get() > b.get();
}

inline bool operator>=(const SPxAutoFile& a, const SPxAutoFile& b)
{
    return a.get() >= b.get();
}

#endif /* _SPX_AUTO_FILE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
