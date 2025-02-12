/*********************************************************************
 *
 * (c) Copyright 2015 - 2017, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxChannelNavData.h,v $
 * ID: $Id: SPxChannelNavData.h,v 1.13 2017/08/03 13:41:45 rew Exp $
 *
 * Purpose:
 *   SPxChannelNavData class header.
 *
 * Revision Control:
 *   03/08/17 v1.11  SP 	Override SetSaveUpdateOnly().
 *
 * Previous Changes:
 *   23/08/16 1.10  SP 	Override SetInputEnabled() to enable/disable channel.
 *   03/11/15 1.9   AGC	Packet decoder callback data now const.
 *   10/07/15 1.8   SP  Move serial sender to base class.
 *   22/04/15 1.7   SP  Use packet sender for net output.
 *   26/03/15 1.6   SP  Support output selection.
 *                      Support SPxSerial interface.
 *   06/03/15 1.5   SP  Changes to doBackgroundProcessing().
 *   26/02/15 1.4   SP  Add GetOutputInfo().
 *   23/02/15 1.3   SP  Change args to HandleReplayPacket().
 *   05/02/15 1.2   SP 	Add ID argument to constructor.
 *   26/01/15 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_CHANNEL_NAV_DATA_H
#define _SPX_CHANNEL_NAV_DATA_H

/* Other headers required. */
#include "SPxLibData/SPxChannel.h"
#include "SPxLibData/SPxSerial.h"
#include "SPxLibNet/SPxNetAddr.h"

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
class SPxNavData;
class SPxSerialPort;

class SPxChannelNavData : public SPxChannel
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

    /* Constructor and destructor. */
    SPxChannelNavData(SPxChannelDatabase *database, UINT8 id, UINT8 fileID);
    SPxErrorCode Create(Media_t inputMedia, Media_t outputMedias);
    virtual ~SPxChannelNavData(void);

    /* Access to underlaying nav data object. */
    virtual SPxNavData *GetNavData(void) const { return m_navData; }

    /* Input config functions. */
    virtual SPxErrorCode SetInputEnabled(int isEnabled);

    /* Live input config functions. */
    virtual void GetLiveInputInfo(char *buf, unsigned int bufSizeBytes) const;
    virtual int IsLiveInputAvail(void) const;

    /* Output config functions. */
    virtual int IsOutputMediaAvail(Media_t media) const;
    virtual SPxErrorCode SetOutputEnabled(unsigned int index, int isEnabled);
    virtual int IsOutputEnabled(unsigned int index) const;
    virtual void GetOutputInfo(unsigned int index,
                               char *buf, 
                               unsigned int bufSizeBytes) const;
    virtual SPxNetAddr *GetOutputNetCtrlIf(unsigned int index);
    virtual SPxSerial *GetOutputSerCtrlIf(unsigned int index);
    
    /* Source SPxNetAddr interface functions. */
    virtual SPxErrorCode SetAddress(const char *address, int port=0, 
                                    const char *ifAddr=NULL);
    virtual SPxErrorCode SetAddress(UINT32 address, int port=0, 
                                    const char *ifAddr=NULL);
    virtual UINT32 GetAddress(void) const;
    virtual const char *GetAddressString(void) const;
    virtual int GetPort(void) const;
    virtual UINT32 GetIfAddress(void) const;
    virtual const char *GetIfAddressString(void) const;

    /* Source SPxSerial interface functions. */
    virtual SPxErrorCode SetSerialName(const char *name);
    virtual SPxErrorCode SetSerialBaud(unsigned int baud);
    virtual const char *GetSerialName(void) const;
    virtual unsigned int GetSerialBaud(void) const;
   
    /* Config load/save. */
    virtual SPxErrorCode SetStateFromConfig(void);
    virtual SPxErrorCode SetConfigFromState(void);
    virtual void SetSaveUpdateOnly(int updateOnly);

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

    /* Check and get channel status. */
    virtual SPxStatus_t CheckStatus(char *mesgBuf, unsigned int mesgBufLen);

    /* Replay packet handler - not used in this class. */
    virtual void HandleReplayPacket(UINT16 packetType,
                                    const unsigned char *data,
                                    unsigned int dataSizeBytes);

    /* Background processing function called from thread. */
    virtual UINT32 doBackgroundProcessing(void);

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private variables.
     */

    /* General. */
    SPxNavData *m_navData;                  /* Nav data object. */
    mutable char m_srcAddrString[16];       /* Copy of source address. */
    mutable char m_srcIfAddrString[16];     /* Copy of interface address. */
    SPxPacketSender *m_packetSender;        /* Network output. */
    int m_isNetOutHeaderless;               /* Headerless network output? */

    /*
     * Private functions.
     */

    /*
     * Private static functions.
     */

    static int mesgHandler(void *invokingObj,
                           void *userArg,
                           void *msgPtr);

}; /* SPxChannelNavData */

#endif /* _SPX_CHANNEL_NAV_DATA_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
