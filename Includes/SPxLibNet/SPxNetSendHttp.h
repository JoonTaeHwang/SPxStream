/*********************************************************************
*
* (c) Copyright 2015 - 2018 Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxNetworkSendHttp - class to send radar tracks
*	over an HTTP connection.
*
* Revision Control:
*   25/06/18 v1.8    AGC	Correctly suppress cppcheck warning.
*
* Previous Changes:
*   12/12/16 1.7    AGC	Add required headers/foward declarations.
*   30/11/16 1.6    AGC	Rename SetPort() for to avoid SetPortA issues.
*   02/09/16 1.5    AGC	Support testing.
*   03/03/16 1.4    AGC	Correct type for GetPort() return value.
*   15/05/15 1.3    AGC	Support more messages.
*   11/05/15 1.2    AGC	Support multiple connections.
*   29/04/15 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_NET_SEND_HTTP_H
#define _SPX_NET_SEND_HTTP_H

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibData/SPxNavData.h"

/* Forward declarations. */
class SPxTrackDatabase;
class SPxThread;
struct SPxPacketTrackMinimal_tag;
struct SPxPacketTrackNormal_tag;
struct SPxPacketTrackExtended_tag;

class SPxNetworkSendHttp : public SPxObj
{
public:
    explicit SPxNetworkSendHttp(SPxNavData *navData,
	unsigned int index=SPX_NAV_DATA_REF_PLATFORM,
	SPxTrackDatabase *trackDB=NULL);
    virtual ~SPxNetworkSendHttp(void);

    // cppcheck-suppress virtualCallInConstructor
    virtual void SetActive(int active);

    /* The SetPort function calls the SetPortNumber because the winspool
     * header defines SetPort to be either SetPortW or SetPortA which causes
     * issues when linking to a function called SetPort. This inline function
     * does not suffer from this problem.
     */
    SPxErrorCode SetPort(UINT16 port) { return SetPortNumber(port); }
    SPxErrorCode SetPortNumber(UINT16 port);
    UINT16 GetPort(void) const;
    SPxErrorCode SetProtocol(unsigned int protocol);
    unsigned int GetProtocol(void) const;
    SPxErrorCode SetSerial(const char *serial);
    const char *GetSerial(void) const;
    SPxErrorCode SetModel(const char *model);
    const char *GetModel(void) const;
    SPxErrorCode SetVersion(const char *version);
    const char *GetVersion(void) const;
    SPxErrorCode SetEndRangeMetres(double endRangeMetres);
    double GetEndRangeMetres(void) const;

    SPxErrorCode ReportTrack(struct SPxPacketTrackMinimal_tag *pMinRpt,
                             struct SPxPacketTrackNormal_tag *pNormRpt,
                             struct SPxPacketTrackExtended_tag *pExtRpt);

    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /* Functions that may be overridden for testing. */
    virtual SPxErrorCode createThread(int mutexAlreadyLocked);
    SPxErrorCode handleRequest(const char *buffer,
			       SOCKET_TYPE httpSocket);
    virtual SPxErrorCode sendReply(SOCKET_TYPE httpSocket,
				   const char *reply,
				   int replyLen);
    virtual UINT32 getSleepMsecs(const char *filename) const;

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    static void *threadFnWrapper(SPxThread *thread);
    void *threadFn(unsigned int index);
    SPxErrorCode createSocket(unsigned int index);
    SPxErrorCode tryAccept(unsigned int index);
    SPxErrorCode process(unsigned int index);
};

#endif /* _SPX_NET_SEND_HTTP_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
