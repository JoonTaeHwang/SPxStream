/*********************************************************************
 *
 * (c) Copyright 2015 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPxChannelADSB class header.
 *
 * Revision Control:
 *   01/02/22 v1.9   BTB	Add SetMaxNumTrailPoints().
 *
 * Previous Changes:
 *   03/08/17 1.8   SP 	Override SetSaveUpdateOnly().
 *   04/08/16 1.7   SP 	Use SetLiveSrcEnabled() on ADSB decoder
 *                      to fix replay issue.
 *   22/01/16 1.6   SP 	Add fileID constructor arg.
 *   11/01/16 1.5   SP	Release TCP output port when not in use.
 *   07/01/16 1.4   SP	Add Set/GetManufacturer().
 *   03/11/15 1.3   AGC Packet decoder callback data now const.
 *   10/07/15 1.2   SP  Move serial sender to base class.
 *   26/03/15 1.1   SP  Initial version.
 *
 *********************************************************************/

#ifndef _SPX_CHANNEL_ADSB_H
#define _SPX_CHANNEL_ADSB_H

/* Other headers required. */
#include "SPxLibData/SPxChannel.h"
#include "SPxLibData/SPxSerial.h"
#include "SPxLibNet/SPxNetAddr.h"
#include "SPxLibNet/SPxDecoderRepeater.h"

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
class SPxADSB;
class SPxSerialPort;
class SPxADSBDecoder;

class SPxChannelADSB : public SPxChannel
{
public:

    /*
     * Public types.
     */

    /*
     * Public variables.
     */

    /*
     * Public static functions.
     */

    static void SetMaxNumTrailPoints(unsigned int numPts);

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxChannelADSB(SPxChannelDatabase *database, UINT8 id, UINT8 fileID);
    SPxErrorCode Create(Media_t inputMedia, Media_t outputMedias);
    virtual ~SPxChannelADSB(void);

    /* Access to underlaying decoder object. */
    virtual SPxADSBDecoder *GetADSBDecoder(void) const { return m_adsbDecoder; }
    /* Manufacturer config. */
    virtual SPxErrorCode SetManufacturer(SPxDeviceManufacturer_t manu);
    virtual SPxDeviceManufacturer_t GetManufacturer(void);

    /* Antenna position. */
    virtual SPxErrorCode SetLatLong(double latDegs, double longDegs);

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
    SPxADSBDecoder *m_adsbDecoder;      /* ADSB decoder object. */
    mutable char m_srcAddrString[16];   /* Copy of source address. */
    mutable char m_srcIfAddrString[16]; /* Copy of interface address. */
    SPxMesgIOType_t m_destNetType;      /* Decoder dest network type. */
    int m_isTcpOutputEnabled;           /* Has user enabled the TCP output? */

    /*
     * Private functions.
     */

    /*
     * Private static variables.
     */

    static void packetHandler(SPxPacketDecoder *decoder,
                              void *userArg,
                              UINT32 packetType,
                              struct sockaddr_in *from,
                              SPxTime_t *timestamp,
                              const unsigned char *payload,
                              unsigned int numBytes);

}; /* SPxChannelADSB */

#endif /* _SPX_CHANNEL_ADSB_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
