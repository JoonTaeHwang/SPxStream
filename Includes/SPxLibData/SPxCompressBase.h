/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Generic decompression definitions, these have been separated
*	from the compression functions to reduce dependencies.
*
*
* Revision Control:
*   08/08/22 v1.1    BTB	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_COMPRESS_BASE_H
#define _SPX_COMPRESS_BASE_H

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

/* Maximum number of memory items managed by the ZLIB compression class. */
#define SPX_COMPRESS_NUM_MEM_ITEMS  (5)

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Structure for managing ZLIB memory. */
struct SPxCompressMem
{
    int inUse;
    unsigned int size;
    void *ptr;
};

/* ZLIB decompression class. */
class SPxCompressBase
{
public:
    /* Public functions. */
    SPxCompressBase(void);
    virtual ~SPxCompressBase(void);

    int Uncompress(unsigned char *dest, unsigned long *destLen,
                   const unsigned char *source, unsigned long sourceLen);

protected:
    /* Protected functions. */
    static void *SPxAlloc(void *opaque, unsigned int items, unsigned int size);
    static void SPxFree(void *opaque, void *address);

private:
    /* Private variables. */
    SPxCompressMem m_memory[SPX_COMPRESS_NUM_MEM_ITEMS];

}; /* SPxCompressBase */

#endif /* _SPX_COMPRESS_BASE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
