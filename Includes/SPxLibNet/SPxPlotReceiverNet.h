/*********************************************************************
*
* (c) Copyright 2019, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxPlotReceiverNet class, for receiving external
*	plots on the network.
*
*
* Revision Control:
*   05/10/22 v1.6    REW	Support UAP code.
*
* Previous Changes:
*   25/06/19 1.5    AGC	Rename to SPxPlotReceiverNet.
*			Derive from new SPxPlotReceiver base.
*   05/02/19 1.4    REW	Make latency configurable.
*   05/02/19 1.3    REW	Support Asterix plot receipt.
*   04/02/19 1.2    REW	Check type of plot before use.
*   04/02/19 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_PLOT_RECEIVER_NET_H
#define _SPX_PLOT_RECEIVER_NET_H

/*
 * Other headers required.
 */
/* We need some library headers for common types, functions, errors etc. */
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibData/SPxPlotReceiver.h"
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

/* Forward declare classes we need. */
class SPxPacketDecoder;
class SPxPacketDecoderNet;
class SPxAsterixDecoder;
struct sockaddr_in;
struct SPxTime_tag;

/*
 * Define our class, derived from the generic base class.
 */
class SPxPlotReceiverNet : public SPxPlotReceiver, public SPxNetAddr
{
public:
    /*
     * Public members.
     */
    /* Constructor and destructor. */
    explicit SPxPlotReceiverNet(void);
    virtual ~SPxPlotReceiverNet(void);

    virtual SPxErrorCode SetType(unsigned int type);

    /* SPxNetAddr interface. */
    virtual SPxErrorCode SetAddress(const char *address, int port=0,
					const char *ifAddr=NULL);
    virtual SPxErrorCode SetAddress(UINT32 address, int port=0,
					const char *ifAddr=NULL);
    virtual UINT32 GetAddress(void) const;
    virtual const char *GetAddressString(void) const;
    virtual int GetPort(void) const;
    virtual UINT32 GetIfAddress(void) const;
    virtual const char *GetIfAddressString(void) const;
    virtual int IsTCPSupported(void) const;
    virtual SPxErrorCode SetTCP(int isTCP);
    virtual int IsTCP(void) const;

    /* Asterix options. */
    SPxErrorCode SetExpectedSAC(UINT8 sac);
    SPxErrorCode SetExpectedSIC(UINT8 sic);
    SPxErrorCode SetUAPCode(unsigned int uapCode);
    UINT8 GetExpectedSAC(void);
    UINT8 GetExpectedSIC(void);
    unsigned int GetUAPCode(void) const;

    /* Generic parameter assignment. */
    int SetParameter(char *name, char *value);
    int GetParameter(char *name, char *valueBuf, int bufLen);

protected:
    /*
     * Protected members.
     */


private:
    /*
     * Private variables.
     */
    mutable SPxCriticalSection m_mutex;	/* Mutex to protect internals */

    /* Objects we use internally. */
    SPxPacketDecoderNet *m_pdn;		/* Generic decoder object */
    SPxAsterixDecoder *m_asterixDecoder;/* Asterix decoder object */

    int m_typeIsAsterix;		/* Convenience flag */

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

}; /* SPxPlotReceiverNet */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_PLOT_RECEIVER_NET_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
