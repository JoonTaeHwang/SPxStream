/*********************************************************************
*
* (c) Copyright 2007 - 2013, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxCompressORC.h,v $
* ID: $Id: SPxCompressORC.h,v 1.11 2013/11/05 09:43:41 rew Exp $
*
* Purpose:
*   Implementation of ORC Compression / Decompression
*
*
* Revision Control:
*   04/11/13 v1.11   AGC	Fix #273 - Decompression errors.
*
* Previous Changes:
*   04/10/13 1.10   AGC	Simplify headers.
*   25/04/13 1.9    AGC	Remove ZLIB definitions.
*   20/03/13 1.8    REW	Make SPxDecompressORC() more robust.
*   18/10/12 1.7    AGC	Do not include zlib.h on ARM.
*   19/10/08 1.6    DGJ	Add destinationSize to SPxDecompressORC().
*   13/02/08 1.5    REW	ZLib is no longer conditionally compiled.
*   15/08/07 1.4    REW	Need extern "C" for previous change.
*   15/08/07 1.3    REW	Add zlib externs as well.
*   26/07/07 1.2    REW	Make source pointers consts.
*			Simplify NumBytesUsed. Split BitSequence class.
*   25/07/07 1.1    DGJ	Initial Version
**********************************************************************/

#ifndef _SPX_COMPRESSORC_H
#define _SPX_COMPRESSORC_H

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Magic number */
#define SPX_MAGIC_ORC_HEADER 0x6759

/* A simple marker code to provide a check point for the start of a window. */
#define SPX_ORC_START_WINDOW_MARKER 0x78


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

/* Forward declarations. */
struct SPxReturn_tag;

/* Classes to help pack and unpack bit sequences. The class manages an
 * area of memory that the caller can insert a bit sequence into. The
 * class packs the bits tightly.
 */
class BitSequence
{
protected:
    int m_bitPos;
    int m_bytePos;
    int m_totalBits;
    int m_bufferLength;
    int m_reachedBufferEnd;
    int m_status;
public:
    BitSequence();
    int NumBytesUsed(void)
    {
	return( (m_totalBits + 7) / 8 );
    }
    int GetStatus(void)
    {
	return(m_status);
    }
    int ReachedEnd(void)
    {
	return m_reachedBufferEnd;
    }
};

class BitSequenceInput :public BitSequence
{
    const unsigned char *m_buffer;
public:
    BitSequenceInput(const unsigned char *memoryToUse, int length=0);
    int GetBits(int numBits);
};

class BitSequenceOutput :public BitSequence
{
    unsigned char *m_buffer;
public:
    BitSequenceOutput(unsigned char *memoryToUse, int length=0);
    void AddBits(unsigned char val, unsigned char numBits);
};

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/*
 * Raw compress/decompress functions.
 */

/* The dataLenBytes argument was previously optional.
 * It is required as of 04/11/13 because without it a buffer
 * overrun is possible if the window size does not evenly divide
 * into the uncompressed data size.
 */
extern int SPxDecompressORC(const unsigned char *data,
				unsigned char *destination,
				int destinationSize,
				int *optionalDataSpaceUsedPtr,
				int dataLenBytes);
extern int SPxCompressORC(const unsigned char *data, int length,
				int windowSize, 
				unsigned char *destinationBuffer,
				int destinationSize,
				int *bytesWrittenToBuffer);

/*
 * Return-based compress/decompress functions.
 */
extern int SPxCompressReturn(SPxReturn_tag *r,
				unsigned char *outputMemory,
				int outputMemorySize,
				int *bytesWrittenOutput);
extern int SPxCompressDecompress(SPxReturn_tag *r,
				int *compressedSizePtr,
				double *compressionRatioPtr);

/*
 * Test function.
 */
extern int SPxTestCompress(void);



#endif /* _SPX_COMPRESSORC_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
