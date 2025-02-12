/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for interface for receiving data from an Ascan process.
*
* Revision Control:
*   24/06/22 v1.1    BTB	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_ASCAN_IFACE_H
#define _SPX_ASCAN_IFACE_H

#include "SPxLibData/SPxRib.h"

class SPxAscanIface
{
public:
    SPxAscanIface(void) {}
    virtual ~SPxAscanIface(void) {}

    virtual void NewReturn(SPxReturn *newReturn)=0;
    virtual void NewAux(SPxReturn *newReturn, unsigned int index=0)=0;
};

#endif /* _SPX_ASCAN_IFACE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
