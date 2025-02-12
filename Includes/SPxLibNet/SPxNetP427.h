/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for P427 network base class.
*
* Revision Control:
*   16/04/19 v1.2    SP 	Make destructor virtual.
*
* Previous Changes:
*   15/04/19 1.1    SP 	Initial version.
*
**********************************************************************/

#ifndef _SPX_NET_P427_H
#define _SPX_NET_P427_H

/*
 * Other headers required.
 */

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

class SPxNetP427
{
public:

    /*
     * Public types.
     */

    /* Channel IDs. */
    typedef enum
    {
        AIR_1_STREAM = 0,     /* Air 1 (A1). */
        AIR_2_STREAM = 1,     /* Air 2 (A2). */
        SUR_1_STREAM = 2,     /* Surface 1 (B1). */
        SUR_2_STREAM = 3,     /* Surface 2 (B2). */
        MAX_NUM_STREAMS = 4   /* Must be last entry. */

    } StreamID_t;

    /*
     * Public fields.
     */

    /*
     * Public functions.
     */

    /* Constructor, destructor etc. */
    explicit SPxNetP427(void) {};
    virtual ~SPxNetP427(void) {};

protected:

    /*
     * Protected types.
     */

    /*
     * Protected functions.
     */

    /*
     * Protected fields.
     */

    /* 8-bit header parameters. */
    static const UINT8 HDR_PARAM_STREAM = 0;
    static const UINT8 HDR_PARAM_START_BEARING = 1;
    static const UINT8 HDR_PARAM_END_BEARING = 2;
    static const UINT8 HDR_PARAM_RANGE_SAMPLES = 4;
    static const UINT8 HDR_PARAM_NO_VALID_VIDEO = 11;
    static const UINT8 HDR_PARAM_BIN_SIZE = 12;
    static const UINT8 HDR_PARAM_RX_BEAM_ELEVATION = 13;

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

}; /* SPxNetP427 class */

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
