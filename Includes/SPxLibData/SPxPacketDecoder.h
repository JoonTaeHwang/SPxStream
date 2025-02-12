/*********************************************************************
*
* (c) Copyright 2011 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxPacketDecoder class.
*
*	This is a generic class that may be used to decode an SPx
*	packet stream. The SPxPacketDecoderNet and SPxPacketDecoderFile
*	classes implement decoding of network streams and file streams.
*
*	This class parses the messages given to it and invokes 
*	callback functions according to the packet types found.
*	The file is heavily based on parts of the old SPxPacketDecoder
*	class from SPxLibNet (that is now SPxPacketDecoderNet).
*
*
* Revision Control:
*   01/02/21 v1.31   SP 	Add ModifyChannelPacket().
*
* Previous Changes:
*   04/08/20 1.30   BTB	Add SPX_DEV_MANUF_FR24.
*   10/07/19 1.29   SP 	Add peekMetadataPacket(), peekCameraPosPacket(),
*			and peekCameraCommandPacket().
*   14/05/19 1.28   REW	Add SPX_DEV_MANUF_AIRNAV.
*   09/04/19 1.27   REW	Rename DUMP1090 to SPX_DEV_MANUF_AVR and
*			add SPX_DEV_MANUF_BEAST.
*   09/04/19 1.26   REW	Add SPX_DEV_MANUF_DUMP1090.
*   31/01/19 1.25   REW	Add ASTERIX manufacturer support.
*   21/06/18 1.24   AGC	Support setting bad message error interval.
*   02/03/18 1.23   REW	Support Kinetic SBS-3 raw KAL packets.
*   13/07/17 1.22   AGC	Improvements to handler removal.
*   03/10/16 1.21   AGC	Add channel filter to NMEA decode.
*   27/09/16 1.20   AGC	Add channel filter.
*   16/09/16 1.19   AGC	Add fromAddr argument to DecodeBuffer().
*   03/11/15 1.18   AGC	Packet decoder callback data now const.
*   03/11/15 1.17   SP 	Add peekNetPacket().
*   29/06/15 1.16   AGC	Add GetChannelIndices().
*   23/06/15 1.15   AGC	Add device manufacturer.
*			Support serial data decoding.
*   05/06/15 1.14   SP 	Add peekRecordPacket().
*   11/05/15 1.13   SP 	Update current channel index from packets
*                       that have a channel index field.
*   27/04/15 1.12   AGC	Fix handler removal.
*   26/03/15 1.11   AGC	Support mutiple handlers for each packet type.
*   20/02/15 1.10   SP 	Rename handleChannelPacket() to handleChanSelectPacket().
*   16/02/15 1.9    SP 	Add handleChannelPacket().
*   05/02/15 1.8    SP 	Add initial support for channel index.
*   13/03/14 1.7    AGC	Support processing packets from within handlers.
*   20/12/13 1.6    REW	Change SetDirectHandler() to InstallDirectHandler()
*   15/11/13 1.5    SP 	Add DecodeBuffer() and GetPacketDeficit().
*   04/10/13 1.4    AGC	Move C++ headers to source.
*   15/05/13 1.3    AGC	Report bad message errors every 10 seconds.
*   15/03/12 1.2    REW	Support direct access to packets.
*   06/04/11 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_PACKET_DECODER_H
#define _SPX_PACKET_DECODER_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCriticalSection.h"


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

enum SPxDeviceManufacturer_t
{
    SPX_DEV_MANUF_NONE = 0,
    SPX_DEV_MANUF_SPX = 1,		/* Cambridge Pixel SPx format */
    SPX_DEV_MANUF_RG = 2,		/* RadarGadget USB input */
    SPX_DEV_MANUF_KAL = 3,		/* Kinetic SBS-3 Basestation format */
    SPX_DEV_MANUF_ASTERIX = 4,		/* Eurocontrol ASTERIX */
    SPX_DEV_MANUF_AVR = 5,		/* E.g. Dump1090 TCP port 30002 */
    SPX_DEV_MANUF_BEAST = 6,		/* E.g. Dump1090 TCP port 30005 */
    SPX_DEV_MANUF_AIRNAV = 7,		/* AirNav RadarBox, subset of KAL */
    SPX_DEV_MANUF_FR24 = 8		/* FR24 JSON data */
};

/* Selected packet IDs for the KAL (Kinetic SBS-3) device. */
enum SPxKALPktID_t
{
    SPX_KAL_PKT_ADSB = 0x01,
    SPX_KAL_PKT_SERIAL_ENQ = 0x11,
    SPX_KAL_PKT_LOGIN_REQ = 0x17,
    SPX_KAL_PKT_STATUS = 0x20,
    SPX_KAL_PKT_SERIAL_REPLY = 0x21,
    SPX_KAL_PKT_LOGIN_INFO= 0x26,
    SPX_KAL_PKT_RADIO_STATUS = 0x38,
    SPX_KAL_PKT_AUDIO_DATA = 0x3b,
    SPX_KAL_PKT_REG_DATA = 0x45,
    SPX_KAL_PKT_REG_ACCESS = 0x54,
    SPX_KAL_PKT_AIS_DATA = 0x57
};

/* Need to forward-declare the class for use in the handler function type. */
class SPxPacketDecoder;

/* Define the type of the callback packet handler functions. */
typedef void (*SPxPacketDecoderFn_t)(SPxPacketDecoder *,	/* this */
				     void *,			/* userArg */
				     UINT32,			/* packetType*/
				     struct sockaddr_in *,	/* from/NULL */
				     struct SPxTime_tag *,	/* time/NULL */
				     const unsigned char *,	/* payload */
				     unsigned int);		/* numBytes */

/*
 * Define our class.
 */
class SPxPacketDecoder : public SPxObj
{
public:
    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxPacketDecoder(void);
    virtual ~SPxPacketDecoder(void);

    /* Configuration functions. */
    SPxErrorCode SetManufacturer(SPxDeviceManufacturer_t manufacturer);
    SPxDeviceManufacturer_t GetManufacturer(void) const { return m_manufacturer; }

    /* Handler functions. */
    SPxErrorCode InstallHandler(SPxPacketDecoderFn_t fn,
				void *userArg,
				UINT32 packetType);
    SPxErrorCode RemoveHandler(SPxPacketDecoderFn_t fn,
			       void *userArg,
			       UINT32 packetType,
			       int waitIfInvoking = TRUE);
    SPxErrorCode RemoveHandler(SPxPacketDecoderFn_t fn,
			       void *userArg,
			       int waitIfInvoking = TRUE);

    /* Direct access handlers for pre-decoded data.
     * Packet type will be zero and there will be no timestamp when invoked.
     */
    SPxErrorCode InstallDirectHandler(SPxPacketDecoderFn_t fn, void *userArg);
    SPxErrorCode RemoveDirectHandler(SPxPacketDecoderFn_t fn, void *userArg,
				     int waitIfInvoking = TRUE);

    /* Decode data in a user-supplied buffer. */
    SPxErrorCode DecodeBuffer(const unsigned char *buf, unsigned int numBytes,
			      struct sockaddr_in *fromAddr=NULL)
    {
        return decodeAndInvoke(buf, numBytes, fromAddr);
    }

    /* Get channel index for the current packet. */
    virtual UINT8 GetChannelIndex(void) const { return m_channelIndex; }

    /* Get list of channel indices. */
    SPxErrorCode GetChannelIndices(unsigned char *indices,
	unsigned int *numIndices) const;

    /* Set bad message error interval. */
    SPxErrorCode SetBadMsgErrIntervalMsecs(UINT32 badMsgErrIntervalMsecs);

    /*
     * Public static functions.
     */

    static SPxErrorCode GetPacketDeficit(unsigned char *buf, 
                                         unsigned int numBytes,
                                         int *deficitBytes);

    static const UINT8 *ModifyPacketChannel(UINT16 packetType,
                                            UINT8 channelIndex,
                                            const UINT8 *payload,
                                            unsigned int payloadBytes,
                                            int payloadNetEndian,
                                            UINT8 *buffer,
                                            unsigned int bufferBytes);

protected:
    /*
     * Protected functions.
     */
    SPxErrorCode decodeAndInvoke(const unsigned char *buf,
                                 unsigned int numBytes,
                                 struct sockaddr_in *fromAddr,
                                 int mutexAlreadyLocked=FALSE,
				 int channelFilter=-1);

    SPxErrorCode decodePacketType(UINT32 packetType, 
				  struct sockaddr_in *fromAddr,
				  struct SPxTime_tag *timePtr, 
				  const unsigned char *buf,
				  unsigned int numBytes, 
				  int mutexAlreadyLocked,
				  int channelFilter=-1);

    /* Set channel index for the next packet. */
    virtual void setChannelIndex(UINT8 index);

    void resetChannelIndices(void);

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;
    mutable SPxCriticalSection m_mutex;
    SPxDeviceManufacturer_t m_manufacturer;
    UINT32 m_lastBadMsgTime;		/* Last time a bad message error was reported. */
    UINT32 m_badMsgErrIntervalMsecs;	/* Minimum interval between bad message errors. */
    UINT8 m_channelIndex;		/* Channel index for next packet. */
    int m_channelIndices[256];		/* Channel indices. */

    /* KAL decoding fields. */
    unsigned char m_kalBuf[1024];
    unsigned int m_kalBufPresent;
    unsigned int m_kalBufDLEIndex;

    /*
     * Private functions.
     */
    int removeHandler(SPxPacketDecoderFn_t fn,
		      void *userArg,
		      UINT32 packetType);

    void handleChanSelectPacket(const unsigned char *payload,
                                unsigned int numBytes);
    
    void peekRadarConfigPacket(const unsigned char *payload,
                               unsigned int numBytes);

    void peekRadarReturnPacket(const unsigned char *payload,
                               unsigned int numBytes);

    void peekImageChunkPacket(const unsigned char *payload,
                              unsigned int numBytes);

    void peekRecordPacket(const unsigned char *payload,
                          unsigned int numBytes);

    void peekNetPacket(const unsigned char *payload,
                       unsigned int numBytes);

    void peekMetadataPacket(const unsigned char *payload,
                            unsigned int numBytes);

    void peekCameraPosPacket(const unsigned char *payload,
                             unsigned int numBytes);

    void peekCameraCommandPacket(const unsigned char *payload,
                                 unsigned int numBytes);

    /* Manufacturer decoding/unframing. */
    unsigned int decodeSPx(const unsigned char *buf,
                           unsigned int numBytes,
                           struct sockaddr_in *fromAddr,
                           int mutexAlreadyLocked,
			   int channelFilter);
    unsigned int decodeRG(const unsigned char *buf,
			  unsigned int numBytes,
			  struct sockaddr_in *fromAddr,
                          int mutexAlreadyLocked);
    unsigned int decodeKAL(const unsigned char *buf,
                           unsigned int numBytes,
                           struct sockaddr_in *fromAddr,
                           int mutexAlreadyLocked);
    unsigned int decodeASTERIX(const unsigned char *buf,
                           unsigned int numBytes,
                           struct sockaddr_in *fromAddr,
                           int mutexAlreadyLocked);
    unsigned int decodeAVR(const unsigned char *buf,
			  unsigned int numBytes,
			  struct sockaddr_in *fromAddr,
                          int mutexAlreadyLocked);
    unsigned int decodeBEAST(const unsigned char *buf,
			  unsigned int numBytes,
			  struct sockaddr_in *fromAddr,
                          int mutexAlreadyLocked);
    unsigned int decodeFR24(const unsigned char *buf,
                            unsigned int numBytes,
                            struct sockaddr_in *fromAddr,
                            int mutexAlreadyLocked);

    /* Protocol deserialisation. */
    unsigned int decodeAISorNMEA(UINT32 packetType, 
			struct sockaddr_in *fromAddr,
			struct SPxTime_tag *timePtr,
			const unsigned char *buf, 
			unsigned int numBytes,
			int mutexAlreadyLocked,
			int channelFilter);

    /* Callback invocation. */
    int invokeDirect(struct sockaddr_in *fromAddr,
		     struct SPxTime_tag *timePtr,
		     const unsigned char *buf, 
		     unsigned int numBytes,
		     int mutexAlreadyLocked);
    SPxErrorCode invoke(UINT32 packetType, 
			struct sockaddr_in *fromAddr,
			struct SPxTime_tag *timePtr,
			const unsigned char *buf, 
			unsigned int numBytes,
			int mutexAlreadyLocked,
			int channelFilter=-1);

}; /* SPxPacketDecoder */

#endif /* _SPX_PACKET_DECODER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
