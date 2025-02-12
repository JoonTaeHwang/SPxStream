/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header of SPxOnvifProbe class - An SPx library class that
*	manages the sending and receiving of Onvif probe messages.
*
* Revision Control:
*   26/11/19 v1.2    AGC        Improve testability.
*
* Previous Changes:
*   20/11/19 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef SPX_ONVIF_PROBE_H_
#define SPX_ONVIF_PROBE_H_

/* Standard headers. */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxEventHandler. */
#include "SPxLibUtils/SPxEventHandler.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
struct sockaddr_in;
class SPxPacketDecoder;
struct SPxTime_tag;

/*
 * Define our class.
 */
class SPxOnvifProbe : public SPxObj
{
public:
    typedef void(*ProbeFn)(struct sockaddr_in *from,
			   const unsigned char *buffer,
			   unsigned int bufLen);
    typedef void(*DiscoverFn)(struct sockaddr_in *from,
			      const unsigned char *buffer,
			      unsigned int bufLen);

    static SPxOnvifProbe *GetInstance();

    SPxErrorCode SendDiscovery(const void *buffer, unsigned int bufLen,
                               UINT32 discoveryPeriodMS=500);
    SPxErrorCode SendProbeResponse(UINT32 address,
				   int port,
				   const void *buffer,
				   unsigned int bufLen);

    template<typename F, typename O>
    SPxErrorCode AddProbeCallback(F fn, O obj)
    {
	SPxErrorCode err = m_probeFns.Add(fn, obj);
	checkProbeThreads();
	return err;
    }
    template<typename F, typename O>
    SPxErrorCode RemoveProbeCallback(F fn, O obj)
    {
	SPxErrorCode err = m_probeFns.Remove(fn, obj);
	checkProbeThreads();
	return err;
    }
    template<typename F, typename O>
    SPxErrorCode AddDiscoverCallback(F fn, O obj)
    {
	SPxErrorCode err = m_discoverFns.Add(fn, obj);
	checkDiscoverThreads();
	return err;
    }
    template<typename F, typename O>
    SPxErrorCode RemoveDiscoverCallback(F fn, O obj)
    {
	SPxErrorCode err = m_discoverFns.Remove(fn, obj);
	checkDiscoverThreads();
	return err;
    }
 
private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxEventHandler<ProbeFn> m_probeFns;
    SPxEventHandler<DiscoverFn> m_discoverFns;

    /* Construction and destruction. */
    explicit SPxOnvifProbe(void);
    virtual ~SPxOnvifProbe(void);

    void checkProbeThreads(void);
    void checkDiscoverThreads(void);
    static void probeHandlerWrapper(SPxPacketDecoder *pktDecoder,
				    void *userArg,
				    UINT32 packetType,
				    struct sockaddr_in *from,
				    struct SPxTime_tag *time,
				    const unsigned char *payload,
				    unsigned int numBytes);
    void probeHandler(struct sockaddr_in *from,
		      const unsigned char *payload,
		      unsigned int numBytes);
    static void discoverHandlerWrapper(SPxPacketDecoder *pktDecoder,
				       void *userArg,
				       UINT32 packetType,
				       struct sockaddr_in *from,
				       struct SPxTime_tag *time,
				       const unsigned char *payload,
				       unsigned int numBytes);
    void discoverHandler(struct sockaddr_in *from,
			 const unsigned char *payload,
			 unsigned int numBytes);

}; /* SPxOnvifProbe */

#endif /* SPX_ONVIF_PROBE_H_ */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
