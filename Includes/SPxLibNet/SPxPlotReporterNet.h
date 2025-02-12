/*********************************************************************
*
* (c) Copyright 2009, 2018 - 2020, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxPlotReporterNet class, used for sending plot reports
*	to the network.
*
*
* Revision Control:
*   21/06/22 v1.10   AGC	Add silent option to GetSenderPort().
*
* Previous Changes:
*   07/02/20 1.9    SP 	Add GetSenderPort().
*   25/06/19 1.8    AGC	Rename to SPxPlotReporterNet.
*			Derive from new SPxPlotReporter base.
*   11/03/19 1.7    REW	Support reporting of merged plots.
*   11/01/19 1.6    AGC	Support SPx plot status messages.
*   17/04/18 1.5    REW	Estimate period for Asterix north messages.
*   12/04/18 1.4    REW	Handle sector messages for Asterix reporting.
*   23/03/18 1.3    REW	Add support for Asterix reports.
*			Set/GetSenderID() changed to UINT16.
*   29/06/09 1.2    REW	Remove blob detector arg. Make handler public.
*   25/06/09 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_PLOT_REPORTER_NET_H
#define _SPX_PLOT_REPORTER_NET_H

/*
 * Other headers required.
 */
/* We need the plot reporter base class we are derived from. */
#include "SPxLibData/SPxPlotReporter.h"
#include "SPxLibNet/SPxNetAddr.h"


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

/* Forward-define classes in case we're included in the wrong order. */
class SPxWorldMap;
class SPxPacketSender;

/*
 * Define our class.
 */
class SPxPlotReporterNet : public SPxPlotReporter, public SPxNetAddr
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxPlotReporterNet(SPxWorldMap *worldMap = NULL);
    virtual ~SPxPlotReporterNet();

    virtual SPxErrorCode SetAddress(const char *address, int port = 0, const char *ifAddr = NULL);
    virtual SPxErrorCode SetAddress(UINT32 address, int port = 0, const char *ifAddr = NULL);
    virtual UINT32 GetAddress(void) const;
    virtual const char *GetAddressString(void) const;
    virtual int GetPort(void) const;
    virtual UINT32 GetIfAddress(void) const;
    virtual const char *GetIfAddressString(void) const;
    virtual int IsTCPSupported(void) const;
    virtual SPxErrorCode SetTCP(int isTCP);
    virtual int IsTCP(void) const;
    virtual SPxErrorCode SetTTL(int ttl);
    virtual int GetTTL(void) const;
    virtual int GetSenderPort(int silent=FALSE) const;

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /*
     * Private variables.
     */
    SPxPacketSender *m_packetSender;

    /*
     * Private functions.
     */
    virtual SPxErrorCode OutputPlot(const SPxTime_tag *time,
				    const unsigned char *data,
				    unsigned int dataSize);
    virtual SPxErrorCode OutputStatus(const unsigned char *data,
				      unsigned int dataSize);

}; /* SPxPlotReporterNet */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_PLOT_REPORTER_NET_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
