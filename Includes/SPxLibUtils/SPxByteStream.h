/*********************************************************************
*
* (c) Copyright 2012 - 2014, 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header of SPxByteStream class for extracting byte-swapped
*	data from a byte stream.
*
*
*	The byte stream is created from a pointer and length:
*
*	    SPxByteStream bs(pointer, length, byteswap);
*
*	Byte-swapping data is extracted using the >> operator.
*	The success or failure of the extraction can be checked
*	as follows:
*
*	    unsigned int val = 0;
*	    if( bs >> val )
*	    {
*		Success!
*	    }
*
*	Multiple extractions can be chained with overall success check:
*
*	    unsigned int val1 = 0;
*	    char val2 = 0;
*	    if( bs >> val1 >> val2 )
*	    {
*		Success!
*	    }
*
*	Bytes can be skipped with the Ignore() function.
*
*
* Revision Control:
*   04/11/19 v1.5    AGC	Do not derive from SPxObj.
*
* Previous Changes:
*   13/03/14 1.4    AGC	Use new form of SPxByteSwapInSitu.
*   22/11/13 1.3    AGC	Move SPxByteStreamSwapInSitu() to SPxByteSwap.
*   03/08/12 1.2    AGC	Fix icc warning with SPxByteStreamSwapInSitu().
*   02/08/12 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_BYTE_STREAM_H
#define _SPX_BYTE_STREAM_H

/*
 * Other headers required.
 */

/* Standard headers. */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxIsHostBigEndian(), SPxByteSwapInSitu(). */
#include "SPxLibUtils/SPxByteSwap.h"

/* For SPxError(). */
#include "SPxLibUtils/SPxError.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */

/*
 * Define our class.
 */

/* Not derived from SPxObj for performance reasons. */
class SPxByteStream
{
public:
    /* Public functions. */
    explicit SPxByteStream(const unsigned char *data, 
	unsigned int dataBytes,
	int byteSwap=!SPxIsNetworkEndian());
    virtual ~SPxByteStream(void);

    SPxByteStream& Ignore(unsigned int numBytes=1);
    int Get(void);
    int Peek(void);
    unsigned int Size(void) const;
    int ByteSwap(void) const;
    unsigned int ReadBytes(void) const;
    unsigned int UnreadBytes(void) const;
    int Good(void) const;
    int Eof(void) const;
    const unsigned char *DataPtr(void) const;
    const unsigned char *CurrPtr(void) const;
    const unsigned char *EndPtr(void) const;
    operator void*(void) const;

    /* Read from the current position in the stream into
     * a variable, doing required byte-swapping.
     * This is a templated function and must therefore be implemented
     * within the header.
     */
    template<class T>
    SPxByteStream& operator>>(T& val)
    {
	if( !Eof() )
	{
	    /* Check there are enough bytes. */
	    if( UnreadBytes() >= sizeof(T) )
	    {
		/* Read value from pointer - byte-swapping if required. */
		const T *valPtr = reinterpret_cast<const T*>(CurrPtr());
		val = *valPtr;
		if( ByteSwap() )
		{
		    SPxByteSwapInSitu(&val);
		}

		/* Move current pointer past read value. */
		advance(sizeof(T));
	    }
	    else
	    {
		/* Mark end of stream. */
		advance(UnreadBytes());
		setEof(TRUE);
	    }
	}

	return *this;
    }

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;	/* Private structure. */

    /* Private functions. */
    void advance(unsigned int numBytes);
    void setEof(int eof);

}; /* SPxByteStream */

#endif /* _SPX_BYTE_STREAM_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
