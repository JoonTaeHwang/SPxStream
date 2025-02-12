/*********************************************************************
 *
 * (c) Copyright 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxP524Decoder class which supports the receiving
 *	of track messages from a P524 radar and translating
 *	them into SPx extended track messages.
 *
 *
 * Revision Control:
 *   25/03/22 v1.5   AGC	Support setting pitch/roll from radar.
 *
 * Previous Changes:
 *   17/03/22 1.4   AGC	Move WriteDebug() to base class.
 *   14/03/22 1.3   AGC	Suppress spurious cppcheck warning.
 *   11/03/22 1.2   AGC	Start/stop radar.
 *   04/03/22 1.1   AGC	Initial Version.
 **********************************************************************/

#ifndef _SPX_P524_DECODER_H
#define _SPX_P524_DECODER_H

/*
 * Other headers required.
 */

#include "SPxLibNet/SPxNetTCP.h"

/* We need the SPxTargetDecoder base class. */
#include "SPxLibNet/SPxTargetDecoder.h"

class SPxP524Decoder : public SPxTargetDecoder
{
public:
   /*
    * Public functions.
    */

    /* Constructor and destructor. */
    explicit SPxP524Decoder(int initiallyDisabled);
    virtual ~SPxP524Decoder(void);

    // cppcheck-suppress virtualCallInConstructor
    virtual void SetActive(int active=TRUE);
    
    virtual SPxErrorCode SetSrcNetAddr(const char *addr,
                                       int port=0,
                                       const char *ifAddr=NULL);

    SPxErrorCode SetComNetPort(int port);
    int GetComNetPort(void) const;
    SPxErrorCode SetStatusSrcNetPort(int port);
    int GetStatusSrcNetPort(void) const;
    SPxErrorCode SetUseRadarPitchRoll(int useRadarPitchRoll);
    int GetUseRadarPitchRoll(void) const;

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /*
     * Private functions.
     */
    void startRadar(void);
    void stopRadar(void);
    void handlePacketTrack(const unsigned char *payload,
                           unsigned int numBytes);
    void handlePacketStatus(const unsigned char *payload,
                            unsigned int numBytes);
    unsigned int decodeTrack(const unsigned char *data,
                             unsigned int dataLen);
    unsigned int decodeDetect(const unsigned char *data,
                              unsigned int dataLen);

    /*
     * Private static functions.
     */
    static void connectRadarComplete(SPxNetTCP *tcp, SPxErrorCode errorCode,
                                     SPxNetTCPAction action,
                                     void* userArg,
                                     const unsigned char* buffer,
                                     unsigned int bufLen,
                                     UINT32 fromAddr,
                                     SOCKET_TYPE sock);
    static void sendStartRadarComplete(SPxNetTCP *tcp, SPxErrorCode errorCode,
                                       SPxNetTCPAction action,
                                       void* userArg,
                                       const unsigned char* buffer,
                                       unsigned int bufLen,
                                       UINT32 fromAddr,
                                       SOCKET_TYPE sock);
    static void sendStopRadarComplete(SPxNetTCP *tcp, SPxErrorCode errorCode,
                                      SPxNetTCPAction action,
                                      void* userArg,
                                      const unsigned char* buffer,
                                      unsigned int bufLen,
                                      UINT32 fromAddr,
                                      SOCKET_TYPE sock);
    static void handlePacketTrackWrapper(SPxPacketDecoder *packetDecoder,
                                         void *userArg,
                                         UINT32 packetType,
                                         struct sockaddr_in *from,
                                         SPxTime_t *timestamp,
                                         const unsigned char *payload,
                                         unsigned int numBytes);
    static void handlePacketStatusWrapper(SPxPacketDecoder *packetDecoder,
                                          void *userArg,
                                          UINT32 packetType,
                                          struct sockaddr_in *from,
                                          SPxTime_t *timestamp,
                                          const unsigned char *payload,
                                          unsigned int numBytes);

}; /* SPxP524Decoder */

#endif /* _SPX_P524_DECODER_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
