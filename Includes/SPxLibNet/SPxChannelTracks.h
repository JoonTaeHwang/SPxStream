/*********************************************************************
 *
 * (c) Copyright 2015 - 2019, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPxChannelTracks class header.
 *
 * Revision Control:
 *   17/07/19 v1.14  BTB 	Add optional checks to warn for non-extended tracks or missing lat/long.
 *
 * Previous Changes:
 *   10/07/19 1.13  SP 	Fix N/C #712 - Lat/long not calculated for Cat-48 tracks.
 *   22/08/17 1.12  SP 	Add SetMaxNumTrailPoints().
 *   03/08/17 1.11  SP 	Override SetSaveUpdateOnly().
 *   22/01/16 1.10  SP 	Add fileID constructor arg.
 *   16/12/15 1.9   SP 	Fix output availability issues.
 *   03/11/15 1.8   AGC Packet decoder callback data now const.
 *   05/06/15 1.7   SP  Fix issues recording & replaying ASTERIX tracks.
 *   26/03/15 1.6   SP  Support output selection.
 *   26/02/15 1.5   SP  Add GetOutputInfo().
 *   23/02/15 1.4   SP  Change args to HandleReplayPacket().
 *   20/02/15 1.3   SP  Support parameter loading/saving.
 *   05/02/15 1.2   SP  Add ID argument to constructor.
 *                      Add HandleReplayPacket().
 *   26/01/15 1.1   SP  Initial version.
 *
 *********************************************************************/

#ifndef _SPX_CHANNEL_TRACKS_H
#define _SPX_CHANNEL_TRACKS_H

/* Other headers required. */
#include "SPxLibData/SPxChannel.h"

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
class SPxTrackDatabase;
class SPxRemoteServer;
class SPxAsterixDecoder;
class SPxAsterixMsg;

class SPxChannelTracks : public SPxChannel
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
    SPxChannelTracks(SPxChannelDatabase *database, UINT8 id, UINT8 fileID);
    SPxErrorCode Create(Media_t inputMedia, Media_t outputMedias);
    virtual ~SPxChannelTracks(void);

    /* Access. */
    virtual SPxTrackDatabase *GetTrackDB(void) { return m_trackDB; }
    virtual SPxRemoteServer *GetRemoteServer(void) { return m_remoteServer; }
    virtual SPxAsterixDecoder *GetAsterixDecoder(void) { return m_asterixDecoder; }

    /* Live input config functions. */
    virtual void GetLiveInputInfo(char *buf, 
                                  unsigned int bufSizeBytes) const;
    virtual int IsLiveInputAvail(void) const;

    /* Output config functions. */
    virtual int IsOutputMediaAvail(Media_t media) const;
    virtual SPxErrorCode SetOutputEnabled(unsigned int index, int isEnabled);
    virtual int IsOutputEnabled(unsigned int index) const;
    virtual void GetOutputInfo(unsigned int index,
                               char *buf, 
                               unsigned int bufSizeBytes) const;
    virtual SPxNetAddr *GetOutputNetCtrlIf(unsigned int index);

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
    
    /* Config load/save. */
    virtual SPxErrorCode SetStateFromConfig(void);
    virtual SPxErrorCode SetConfigFromState(void);
    virtual void SetSaveUpdateOnly(int updateOnly);

    /* Track checking options. */
    virtual void EnableExtendedTrackCheck(int enable) { m_checkForExtendedTracks = enable; }
    virtual void EnableLatLongCheck(int enable) { m_checkForLatLong = enable; }

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

    /* Replay packet handler. */
    virtual void HandleReplayPacket(UINT16 packetType,
                                    const unsigned char *data,
                                    unsigned int dataSizeBytes);

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private variables.
     */

    /* Input/output objects. */
    SPxRemoteServer *m_remoteServer;     /* Used to receive SPx tracks. */
    SPxAsterixDecoder *m_asterixDecoder; /* Used to receive & send ASTERIX tracks. */

    /* General. */
    SPxTrackDatabase *m_trackDB;         /* Track database. */
    int m_isAsterixOutputAvail;          /* ASTERIX output available. */

    /* Variables for checking track type/contents. */
    int m_checkForExtendedTracks;
    int m_nonExtendedTrack;
    int m_foundNonExtendedTrack;
    int m_checkForLatLong;
    int m_missingLatLong;
    int m_foundMissingLatLong;

    /* The following are only used with SPxRemoteServer. */
    mutable char m_srcAddrString[16];    /* Current remote server address. */
    mutable char m_srcIfAddrString[16];  /* Current remote server ifaddrs. */
    mutable int m_srcPort;               /* Current remote server port. */
    SPxPacketSender *m_packetSender;     /* Packet sender for remote server.*/
    
    /*
     * Private static variables.
     */

    /*
     * Private functions.
     */

    void ReportTrackMesg(UINT8 asterixCategory,
                         struct SPxPacketTrackMinimal_tag *pMinRpt,
                         struct SPxPacketTrackNormal_tag *pNormRpt,
                         struct SPxPacketTrackExtended_tag *pExtRpt);

    void ReportTrackerStatusMesg(UINT8 asterixCategory,
                                 struct SPxPacketTrackerStatus_tag *status);

    /*
     * Private static functions.
     */

    static void spxPacketHandler(SPxPacketDecoder *decoder,
                                 void *userArg,
                                 UINT32 packetType,
                                 struct sockaddr_in *from,
                                 SPxTime_t *timestamp,
                                 const unsigned char *payload,
                                 unsigned int numBytes);

    static void spxTrackHandler(SPxRemoteServer *svr,
                                void *userArg,
                                struct sockaddr_in *from,
                                SPxTime_t *timestamp,
                                struct SPxPacketTrackMinimal_tag *pMinRpt,
                                struct SPxPacketTrackNormal_tag *pNormRpt,
                                struct SPxPacketTrackExtended_tag *pExtRpt);

    static void spxTrackerStatusHandler(SPxRemoteServer *svr,
                                void *userArg,
                                struct sockaddr_in *from,
                                SPxTime_t *timestamp,
                                struct SPxPacketTrackerStatus_tag *status);

    static void asterixPacketHandler(SPxPacketDecoder *decoder,
                                     void *userArg,
                                     UINT32 packetType,
                                     struct sockaddr_in *from,
                                     SPxTime_t *timestamp,
                                     const unsigned char *payload,
                                     unsigned int numBytes);

    static void asterixSPxTrackHandler(const SPxAsterixDecoder *decoder,
                                       void *userArg,
                                       struct SPxPacketTrackExtended_tag *rpt,
                                       const SPxAsterixMsg *asterixMsg);

    static void asterixSPxTrackerStatusHandler(const SPxAsterixDecoder *decoder,
                                               void *userArg,
                                               struct SPxPacketTrackerStatus_tag *rpt,
                                               const SPxAsterixMsg *asterixMsg);

}; /* SPxChannelTracks */

#endif /* _SPX_CHANNEL_TRACKS_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
