/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Interface for plot sources.
*
* Revision Control:
*   27/06/19 v1.2    AGC	Fix warnings.
*
* Previous Changes:
*   25/06/19 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_PLOT_SOURCE_IFACE_H
#define _SPX_PLOT_SOURCE_IFACE_H

#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"

class SPxPlotSourceIface
{
public:
    SPxPlotSourceIface(void) {}
    virtual ~SPxPlotSourceIface() {}

    /* Does this plot source add the plots to the plot database? */
    virtual int AddsPlotsToDatabase(void) const { return FALSE; }

    virtual SPxErrorCode AddPlotCallback(SPxCallbackListFn_t /*fn*/, void * /*userObj*/) = 0;
    virtual SPxErrorCode RemovePlotCallback(SPxCallbackListFn_t /*fn*/, void * /*userObj*/) = 0;

    virtual SPxErrorCode AddStatusCallback(SPxCallbackListFn_t /*fn*/, void * /*userObj*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode RemoveStatusCallback(SPxCallbackListFn_t /*fn*/, void * /*userObj*/) { return SPX_ERR_NOT_SUPPORTED; }

};

#endif /* _SPX_PLOT_SOURCE_IFACE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
