/*********************************************************************
 *
 * (c) Copyright 2019, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for ASTERIX Cat-253 network object, used to send
 *   and receive Cat-253 UDP messages.
 *
 * Revision Control:
 *   20/06/19 v1.2   SP 	Use SPxPacketSenderNet for output.
 *
 * Previous Changes:
 *   21/05/19 1.1   SP 	Initial version.
 *
 **********************************************************************/

#ifndef _SPX_NET_CAT_253_UDP_H
#define _SPX_NET_CAT_253_UDP_H

/*
 * Other headers required.
 */

/* We need the base class headers. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibNet/SPxNetCat253.h"
#include "SPxLibNet/SPxNetAddr.h"

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

/* Forward declare any classes required. */
class SPxPacketSender;
class SPxPacketDecoder;
class SPxPacketDecoderNet;

class SPxNetCat253UDP : public SPxNetCat253, public SPxObj, public SPxNetAddr
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */

    /* Constructor, destructor etc. */
    SPxNetCat253UDP(void);
    virtual ~SPxNetCat253UDP();

    /* General. */
    void SetDebug(UINT32 debug) { m_debug = debug; }
    UINT32 GetDebug(void) { return m_debug; }
    void SetLogFile(FILE *file) { m_logFile = file; }
    FILE *GetLogFile(void) { return m_logFile; }

    /* Address/port control. */
    virtual SPxErrorCode SetAddress(const char *address, int port=0, 
                                    const char *ifAddr=NULL);
    virtual SPxErrorCode SetAddress(UINT32 address, int port=0, 
                                    const char *ifAddr=NULL);
    virtual SPxErrorCode SetPort(int port) { return SetPortNumber(port); }
    /* The SetPort function calls the SetPortNumber because the winspool
     * header defines SetPort to be either SetPortW or SetPortA which causes
     * issues when linking to a function called SetPort. This inline function
     * does not suffer from this problem.
     */
    SPxErrorCode SetPortNumber(int port);
    virtual UINT32 GetAddress(void) const;
    virtual const char *GetAddressString(void) const;
    virtual int GetPort(void) const;
    virtual UINT32 GetIfAddress(void) const;
    virtual const char *GetIfAddressString(void) const;

    /* Message handling. */
    SPxErrorCode SendMesg(UINT8 *appData, unsigned int numDataBytes);
    SPxErrorCode PollForMesgs(void);

    /* Generic parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:

    /*
     * Protected functions.
     */

    /*
     * Protected fields.
     */

private:

    /*
     * Private fields.
     */

    /* General. */
    FILE *m_logFile;
    UINT32 m_debug;
    SPxPacketDecoderNet *m_packetDecoder;
    SPxPacketSender *m_packetSender;

    /*
     * Private functions.
     */

    /*
     * Private static functions.
     */

    static void packetHandler(SPxPacketDecoder *decoder,
                              void *userArg,
                              UINT32 packetType,
                              struct sockaddr_in *from,
                              SPxTime_t *timestamp,
                              const unsigned char *payload,
                              unsigned int numBytes);

}; /* SPxNetCat253UDP class */


/*********************************************************************
 *
 *   Function prototypes
 *
 **********************************************************************/

#endif /* _SPX_NET_CAT_253_UDP_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
