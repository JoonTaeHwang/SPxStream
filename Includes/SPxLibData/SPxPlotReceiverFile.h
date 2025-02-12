/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxPlotReceiverFile class, for replaying plots
*	from a file.
*
*
* Revision Control:
*   25/06/19 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_PLOT_RECEIVER_FILE_H
#define _SPX_PLOT_RECEIVER_FILE_H

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
class SPxRadarReplay;
class SPxPacketDecoder;
class SPxAsterixDecoder;
struct sockaddr_in;
struct SPxTime_tag;

/*
 * Define our class, derived from the generic base class.
 */
class SPxPlotReceiverFile : public SPxPlotReceiver
{
public:
    /* Constructor and destructor. */
    explicit SPxPlotReceiverFile(SPxRadarReplay *srcFile);
    virtual ~SPxPlotReceiverFile(void);

private:
    /*
     * Private variables.
     */
    SPxPacketDecoder *m_pd;		/* Generic decoder object */

    /*
     * Private functions.
     */
    /* Callbacks. */
    static void packetHandlerStatic(SPxPacketDecoder *decoder,
					void *userArg,
					UINT32 packetType,
					struct sockaddr_in *fromAddr,
					struct SPxTime_tag *timestamp,
					const unsigned char *payload,
					unsigned int numBytes);
    void packetHandler(UINT32 packetType,
		       struct sockaddr_in *fromAddr,
		       struct SPxTime_tag *timestamp,
		       const unsigned char *payload,
		       unsigned int numBytes);

}; /* SPxPlotReceiverFile */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_PLOT_RECEIVER_FILE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
