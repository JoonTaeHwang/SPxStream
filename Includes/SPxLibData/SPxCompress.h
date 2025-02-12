/*********************************************************************
*
* (c) Copyright 2007 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Generic compression definitions, note that the decompression
*	functions and memory management have been moved to a base class.
*
* Revision Control:
*   08/08/22 v1.5    BTB	Move decompression and memory management to base class.
*
* Previous Changes:
*   03/11/15 1.4    AGC	Uncompress() now takes source data as const.
*   25/04/13 1.3    AGC	Create class with custom allocater for ZLIB.
*   18/10/12 1.2    AGC	Do not include zlib.h on ARM.
*   10/12/07 1.1    REW	Initial Version, from SPxCompressORC.h.
**********************************************************************/

#ifndef _SPX_COMPRESS_H
#define _SPX_COMPRESS_H

/* Other headers. */
#include "SPxLibData/SPxCompressBase.h"
#include "SPxLibData/SPxRib.h"

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

/* ZLIB compression class. */
class SPxCompress : public SPxCompressBase
{
public:
    /* Public functions. */
    SPxCompress(void);
    virtual ~SPxCompress(void);

    int Compress(unsigned char *dest, unsigned long *destLen,
		const unsigned char *source, unsigned long sourceLen);

    unsigned char *CompressGen(const SPxReturn *rtn,
			       unsigned char *destBuf,
			       unsigned char *destBuf2,
			       unsigned int destBufSizeBytes,
			       unsigned int *encodedSizeBytesPtr,
			       unsigned int *formatReqPtr,
			       unsigned int *formatUsedPtr,
			       FILE *logFile, int verbose);

}; /* SPxCompress */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_COMPRESS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
