/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxICD100Decoder class which supports the receiving
 *	of track messages from a Ranger R series radar and translating
 *	them into SPx extended track messages.
 *
 *	ICD-100 track message content comes from a specification
 *	(confidential) obtained as part of project P429.
 *
 *
 * Revision Control:
 *   03/11/20 v1.3   BTB 	Handle different units for distances/speeds/angles.
 *
 * Previous Changes:
 *   06/10/20 1.2   BTB 	Make constructor explicit.
 *   06/02/20 1.1   BTB 	Initial Version.
 **********************************************************************/

#ifndef _SPX_ICD100_DECODER_H
#define _SPX_ICD100_DECODER_H

/*
 * Other headers required.
 */

/* We need the SPxTargetDecoder base class. */
#include "SPxLibNet/SPxTargetDecoder.h"

class SPxICD100Decoder : public SPxTargetDecoder
{
public:
   /*
    * Public functions.
    */

    /* Constructor and destructor. */
    explicit SPxICD100Decoder(int initiallyDisabled);
    virtual ~SPxICD100Decoder(void);

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;
    /* We can't send tracker status messages until we have received
     * at least one status report and a device configuration from the radar.
     */
    int m_receivedInitialStatus;
    int m_receivedInitialConfig;
    /* Count used to check if we are connected to anything over TCP. */
    unsigned int m_connectionCount;

    /*
     * Private functions.
     */
    void handlePacket(const unsigned char *payload, unsigned int numBytes);
    void translateXMLMessage(const char *messageStr, const char *messageType);

    double getMetresForDistString(const char *distStr, const char *unitsStr);
    double getMpsForSpeedString(const char *speedStr, const char *unitsStr);
    double getDegsForAngleString(const char *angleStr, const char *unitsStr);

    void handleTCPConnect(void);
    void handleTCPDisconnect(void);

    /*
     * Private static functions.
     */
    static void handlePacketWrapper(SPxPacketDecoder *packetDecoder,
                                    void *userArg,
                                    UINT32 packetType,
                                    struct sockaddr_in *from,
                                    SPxTime_t *timestamp,
                                    const unsigned char *payload,
                                    unsigned int numBytes);

    static int tcpConnectHandler(void *invokingObj,
                                 void *userArg,
                                 void *tcpConn);

    static int tcpDisconnectHandler(void *invokingObj,
                                    void *userArg,
                                    void *tcpConn);

}; /* SPxICD100Decoder */

#endif /* _SPX_ICD100_DECODER_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
