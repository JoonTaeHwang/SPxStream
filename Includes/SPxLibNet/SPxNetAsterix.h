/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for ASTERIX Cat-240 network base class.
*
* Revision Control:
*   24/04/19 v1.1    SP 	Initial version.
*
* Previous Changes:
*
**********************************************************************/

#ifndef _SPX_NET_ASTERIX_H
#define _SPX_NET_ASTERIX_H

/*
 * Other headers required.
 */
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

/* Forward declare any classes we need. */

class SPxNetAsterix
{
public:

    /*
     * Public types.
     */

    /*
     * Public fields.
     */

    /*
     * Public functions.
     */

    /* Constructor, destructor etc. */
    explicit SPxNetAsterix(void);
    virtual ~SPxNetAsterix(void);

protected:

    /*
     * Protected types.
     */

    /*
     * Protected functions.
     */

    unsigned int cellsToBytes(int cellRes, unsigned int numCells);
    unsigned int bytesToCells(int cellRes, unsigned int numBytes);

    /*
     * Protected fields.
     */

    /* Cell resolution codes. */
    static const UINT8 CELL_SIZE_1BIT = 1;
    static const UINT8 CELL_SIZE_2BITS = 2;
    static const UINT8 CELL_SIZE_4BITS = 3;
    static const UINT8 CELL_SIZE_8BITS = 4;
    static const UINT8 CELL_SIZE_16BITS = 5;
    static const UINT8 CELL_SIZE_32BITS = 6;

private:

    /*
     * Private types.
     */

    /*
     * Private fields.
     */

    /*
     * Private functions.
     */

}; /* SPxNetAsterix class */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NET_P427_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
