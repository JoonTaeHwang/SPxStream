/*********************************************************************
 *
 * (c) Copyright 2014 - 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPxChannelNetRadar class header.
 *
 * Revision Control:
 *   07/03/18 v1.11  SP 	Support MEDIA_NET_P217.
 *
 * Previous Changes:
 *   15/01/18 1.10  SP 	Support TPG source in base class.
 *   05/07/17 1.9   SP 	Override SPxNetAddr functions.
 *   22/01/16 1.8   SP 	Add fileID constructor arg.
 *   01/10/15 1.7   SP 	Rename from SPxChannelNetVideo.
 *   26/03/15 1.6   SP  Support output selection.
 *   05/02/15 1.5   SP  Add ID argument to constructor.
 *   23/10/14 1.4   SP  Remove m_status.
 *   14/10/14 1.3   SP  Fix ICC warnings.
 *   14/10/14 1.2   SP  Further development.
 *   24/09/14 1.1   SP  Initial version.
 *
 *********************************************************************/

#ifndef _SPX_CHANNEL_NET_RADAR_H
#define _SPX_CHANNEL_NET_RADAR_H

/* Other headers required. */
#include "SPxLibData/SPxChannelRadar.h"

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

/* Forward declare any classes required. */
class SPxNetworkReceive;
class SPxNetworkReceiveP226;
class SPxNetworkReceiveP217;
class SPxNetworkSend;
class SPxROC;

class SPxChannelNetRadar : public SPxChannelRadar
{
public:

    /*
     * Public types.
     */

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructors and destructor. */
    SPxChannelNetRadar(SPxChannelDatabase *database, UINT8 id, UINT8 fileID);
    SPxErrorCode Create(Media_t inputMedia, 
                        Media_t outputMedias,
                        int rocIndex=0);
    virtual ~SPxChannelNetRadar(void);

    /* Access. */
    virtual SPxNetworkReceive *GetNetRecv(void) const { return m_netRecv; }
    virtual SPxNetworkReceiveP226 *GetNetRecvP226(void) const { return m_netRecvP226; }

    /* Live input config functions. */
    virtual void GetLiveInputInfo(char *buf, unsigned int bufSizeBytes) const;
    virtual int IsLiveInputAvail(void) const;

    /* SPxNetAddr interface functions. */
    virtual SPxErrorCode SetAddress(const char *address, int port=0, 
                                    const char *ifAddr=NULL);
    virtual SPxErrorCode SetAddress(UINT32 address, int port=0, 
                                    const char *ifAddr=NULL);
    virtual UINT32 GetAddress(void) const;
    virtual const char *GetAddressString(void) const;
    virtual int GetPort(void) const;
    virtual UINT32 GetIfAddress(void) const;
    virtual const char *GetIfAddressString(void) const;

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Live input config functions. */
    virtual SPxErrorCode SetLiveInputEnabled(int isEnabled);
    virtual int IsLiveInputEnabled(void) const;

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private variables.
     */

    /* Input source objects. */
    SPxNetworkReceive *m_netRecv;
    SPxNetworkReceiveP226 *m_netRecvP226;

    /* General. */

    /*
     * Private static variables.
     */

    /*
     * Private functions.
     */

    /* General. */

}; /* SPxChannelNetRadar */

#endif /* _SPX_CHANNEL_NET_RADAR_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
