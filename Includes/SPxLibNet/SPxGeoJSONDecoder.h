/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxGeoJSONDecoder class which supports the receiving
 *	of detections from a GeoJSON-based sensor and translating
 *	them into SPx extended track messages.
 *
 * Revision Control:
 *   13/10/20 v1.4   AJH 	Fix build problem.
 *
 * Previous Changes:
 *   10/10/20 1.3   AJH 	Write user map from feature set.
 *   05/10/20 1.2   AJH 	Add actual implementation.
 *   02/10/20 1.1   AJH 	Initial Version.
 **********************************************************************/

#ifndef _SPX_GEOJSON_DECODER_H
#define _SPX_GEOJSON_DECODER_H

/* We need the SPxTargetDecoder base class. */
#include "SPxLibNet/SPxTargetDecoder.h"

class SPxGeoJSONDecoder : public SPxTargetDecoder
{
public:
   /*
    * Public functions.
    */

    /* Constructor and destructor. */
    explicit SPxGeoJSONDecoder(int initiallyDisabled);
    virtual ~SPxGeoJSONDecoder(void);

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Count used to check if we are connected to anything over TCP. */
    unsigned int m_connectionCount;

    /*
     * Private functions.
     */
    void handlePacket(const unsigned char *payload, unsigned int numBytes);
    void translateJSONMessage(const char *messageStr);
    void handleFeature(const char *messageStr);
    void handleFeatureCollection(const char *messageStr);

    void handleTCPConnect(void);
    void handleTCPDisconnect(void);

    void writeMapStart(FILE *fp, unsigned int colour);
    void writeMapNode(FILE *fp, double latDegs, double longDegs,
	const char *name, double latDegs2=0.0, double longDegs2=0.0);
    void writeMapEnd(FILE *fp);

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

}; /* SPxGeoJSONDecoder */

#endif /* _SPX_GEOJSON_DECODER_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
