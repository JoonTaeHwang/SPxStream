/*********************************************************************
*
* (c) Copyright 2019, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxPlotReceiverSrc class, for receiving plots
*	from a plot-based source.
*
*
* Revision Control:
*   03/04/20 v1.2    AGC        Remove prototypes for non-existent functions.
*
* Previous Changes:
*   25/06/19 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef SPX_PLOT_RECEIVER_SRC_H
#define SPX_PLOT_RECEIVER_SRC_H

/*
 * Other headers required.
 */
/* We need some library headers for common types, functions, errors etc. */
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibData/SPxPlotReceiver.h"

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

/* Forward declare classes we need. */
class SPxPlotSourceIface;
class SPxPacketDecoder;
class SPxAsterixDecoder;
struct sockaddr_in;
struct SPxTime_tag;

/*
 * Define our class, derived from the generic base class.
 */
class SPxPlotReceiverSrc : public SPxPlotReceiver
{
public:
    /* Constructor and destructor. */
    explicit SPxPlotReceiverSrc(SPxPlotSourceIface *plotSrc);
    virtual ~SPxPlotReceiverSrc(void);

private:
    /*
     * Private variables.
     */
    SPxPlotSourceIface *m_plotSrc;

    /*
     * Private functions.
     */
    /* Callbacks. */
    static int staticPlotHandler(void *invokingObj, void *userObj, void *arg);

}; /* SPxPlotReceiverSrc */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_PLOT_RECEIVER_SRC_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
