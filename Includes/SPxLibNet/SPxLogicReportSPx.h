/*********************************************************************
*
* (c) Copyright 2013, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxLogicReportSPx.h,v $
* ID: $Id: SPxLogicReportSPx.h,v 1.2 2013/11/22 16:49:38 rew Exp $
*
* Purpose:
*	Header for SPxLogicReportSPx class, used for sending SPx 
*	tracks reports received on the network to a logic database.
*
*
* Revision Control:
*   22/11/13 v1.2    AGC	Make get functions const.
*
* Previous Changes:
*   04/03/13 1.1    AGC	Initial version.
**********************************************************************/

#ifndef SPX_LOGIC_REPORT_SPX_H
#define SPX_LOGIC_REPORT_SPX_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObjPtr.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibNet/SPxNetAddr.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxLogicDB;
class SPxRemoteServer;

/*
 * Define our class.
 */
class SPxLogicReportSPx : public SPxObj, public SPxNetAddr
{
public:
    /* Public functions. */
    SPxLogicReportSPx(SPxLogicDB *logicDB, UINT32 id=0);
    virtual ~SPxLogicReportSPx(void);

    /* Network address/port. */
    virtual SPxErrorCode SetAddress(const char *addr, int port, const char *ifAddr=NULL);
    virtual SPxErrorCode SetAddress(UINT32 addr, int port, const char *ifAddr=NULL);
    virtual UINT32 GetAddress(void) const;
    virtual const char *GetAddressString(void) const;
    virtual int GetPort(void) const;
    virtual UINT32 GetIfAddress(void) const;
    virtual const char *GetIfAddressString(void) const;

    /* Server ID. */
    SPxErrorCode SetServerID(UINT8 id);
    UINT8 GetServerID(void);

private:
    /* Private variables. */
    SPxObjPtr<SPxLogicDB> m_logicDB;
    UINT32 m_id;
    SPxAutoPtr<SPxRemoteServer> m_remoteSvr;
    UINT32 m_addr;
    SPxAutoPtr<char, void(*)(void*)> m_addrString;
    int m_port;
    UINT32 m_ifAddr;
    SPxAutoPtr<char, void(*)(void*)> m_ifAddrString;
    UINT8 m_serverID;

    /* Private functions. */
    static void trackFnWrapper(SPxRemoteServer *svr, void *arg,
			       struct sockaddr_in *from,
			       SPxTime_t *timestamp,
			       struct SPxPacketTrackMinimal_tag *minRpt,
			       struct SPxPacketTrackNormal_tag *normRpt,
			       struct SPxPacketTrackExtended_tag *extRpt);
    void trackFn(struct sockaddr_in *from, SPxTime_t *timestamp,
		 struct SPxPacketTrackMinimal_tag *minRpt,
		 struct SPxPacketTrackNormal_tag *normRpt,
		 struct SPxPacketTrackExtended_tag *extRpt);

}; /* SPxLogicReportSPx */

#endif /* SPX_LOGIC_REPORT_SPX_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
