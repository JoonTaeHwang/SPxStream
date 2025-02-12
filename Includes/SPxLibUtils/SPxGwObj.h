/*********************************************************************
 *
 * (c) Copyright 2013, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxGwObj.h,v $
 * ID: $Id: SPxGwObj.h,v 1.3 2013/10/07 13:38:45 rew Exp $
 *
 * Purpose:
 *   SPx graphics widget library base class.
 *
 * Revision Control:
 *   07/10/13 v1.3    AGC	Move to SPxLibUtils.
 *
 * Previous Changes:
 *   04/10/13 1.2    AGC	Simplify headers.
 *   11/01/13 1.1    SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_GW_OBJ_H
#define _SPX_GW_OBJ_H

/* We need SPxLibUtils for common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxObj.h"

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

/*********************************************************************
 *
 *   Class definitions
 *
 *********************************************************************/

class SPxGwObj : public SPxObj
{
public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxGwObj(void);
    virtual ~SPxGwObj(void);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /*
     * Protected static functions.
     */

private:

    /*
     * Private variables.
     */

    /*
     * Private functions.
     */

}; /* SPxGwObj */

#endif /* _SPX_GW_OBJ_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/

