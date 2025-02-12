/*********************************************************************
 *
 * (c) Copyright 2013, 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx graphics shape class, used as base class for
 *   widgets that render to an SPxGwPicture.
 *
 * Revision Control:
 *  29/07/21 v1.5    AGC	Remove unused forward declarations.
 *
 * Previous Changes:
 *  04/12/13 1.4    AGC	Move configuration functions to SPxGwWidget.
 *  07/10/13 1.3    AGC	Move to SPxLibUtils.
 *			Make cross-platform.
 *			Add SPxAreaObj support.
 *  20/03/13 1.2    SP 	Add SPxGwAlignment.
 *  25/01/13 1.1    SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_GW_SHAPE_H
#define _SPX_GW_SHAPE_H

/* Other headers required. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxRenderer.h"
#include "SPxLibUtils/SPxGwWidget.h"

/*********************************************************************
 *
 *  Constants
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Macros
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/* Polygon point. */
struct SPxGwPoint_t
{
    double x;
    double y;
};

/* Alignment. */
typedef SPxAreaAlignment SPxGwAlignment_t;

/*********************************************************************
 *
 *   Class definitions
 *
 *********************************************************************/

/* Forward declarations. */
class SPxGwWidgetGroup;

class SPxGwShape : public SPxGwWidget
{
public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxGwShape(SPxGwWidgetGroup *group);
    virtual ~SPxGwShape(void);

private:

    /*
     * Private variables.
     */

    /*
     * Private functions.
     */

    /*
     * Private static functions.
     */

}; /* SPxGwShape */

#endif /* _SPX_GW_SHAPE_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/

