/*********************************************************************
*
* (c) Copyright 2017 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header of SPxOnvifClient class - An SPx library class that
*	manages communications with an Onvif server.
*
* Revision Control:
*   21/04/22 v1.26   AGC	Return early on discovery failures.
*
* Previous Changes:
*   21/09/21 1.25   AGC	Get zoom limits from camera.
*   23/06/21 1.24   AGC	Avoid unnecessary messages to server.
*   05/03/21 1.23   AGC	Add functions for testing.
*   06/01/20 1.22   AGC	Support RTSP over HTTP.
*   20/11/19 1.21   AGC	Move discovery to SPxOnvifProbe.
*   19/06/19 1.20   AGC	Add SetSynchronizationPoint().
*   27/03/19 1.19   AGC	Add IsMoveStatusSupported().
*   06/02/19 1.18   AGC	Improve custom server behaviour.
*   31/01/19 1.17   AGC	Adjust service addresses for NAT.
*   28/01/19 1.16   AGC	Improve error reporting.
*   16/01/19 1.15   AGC	Improve unauthorised checks.
*   16/01/19 1.14   AGC	Support PTZ timeout.
*   15/01/19 1.13   AGC	Use Onvif RTSP session timeout.
*   15/01/19 1.12   AGC	Clean up server UUID.
*   14/01/19 1.11   AGC	Improve unauthorised detection.
*   14/01/19 1.10   AGC	Get absolute tilt degrees limits from camera.
*   14/01/19 1.9    AGC	Improve generic pan/tilt space support.
*   21/12/18 1.8    AGC	Support custom servers.
*   12/12/18 1.7    AGC	Support use of service address during discovery.
*   01/11/18 1.6    AGC	Add basic event support.
*   14/09/18 1.5    AGC	Support querying for spherical pan/tilt space.
*   02/07/18 1.4    AGC	Support more support check functions.
*   20/04/18 1.3    AGC	Move from SPxLibAV to SPxLibNet.
*   14/11/17 1.2    AGC	Remove unused parameter functions.
*   13/11/17 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef SPX_ONVIF_CLIENT_H_
#define SPX_ONVIF_CLIENT_H_

/* Standard headers. */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxNetTCP;
class SPxPacketDecoder;
struct sockaddr_in;
struct SPxTime_tag;
struct mxml_node_s;

/*
 * Define our class.
 */
class SPxOnvifClient : public SPxObj
{
public:
    enum Capability
    {
	CAP_NONE = 0x0,
	CAP_MEDIA = 0x1,
	CAP_PTZ = 0x2
    };
    enum Mode
    {
	MODE_NONE = 0,
	MODE_UDP_MULTICAST = 1,
	MODE_UDP_UNICAST = 2,
	MODE_TCP_INTERLEAVED = 3,
	MODE_HTTP = 4
    };

    struct EventItem
    {
	char name[512];
	char value[512];
    };

    struct Event
    {
	char topic[512];
	unsigned int numSources;
	unsigned int numData;
	EventItem sources[4];
	EventItem data[4];
    };

    /* Construction and destruction. */
    explicit SPxOnvifClient(SPxNetTCP *extNetTCP=NULL);
    virtual ~SPxOnvifClient(void);
    virtual SPxErrorCode Create(void);

    SPxErrorCode SetUsername(const char *username);
    SPxErrorCode GetUsername(char *buf, unsigned int bufLen) const;
    SPxErrorCode SetPassword(const char *password);
    SPxErrorCode GetPassword(char *buf, unsigned int bufLen) const;

    SPxErrorCode Discover(UINT32 capabilities,
			  const char *serviceAddress=NULL,
			  const char *serverID=NULL);

    unsigned int GetNumServers(void) const;
    SPxErrorCode GetServerID(unsigned int index, char *buf, unsigned int bufLen) const;
    SPxErrorCode GetServerName(const char *serverID, char *buf, unsigned int bufLen) const;
    SPxErrorCode GetServerAddr(const char *serverID, char *buf, unsigned int bufLen) const;
    SPxErrorCode GetServiceAddress(const char *serverID, char *buf, unsigned int bufLen) const;
    SPxErrorCode GetServerID(const char *serverAddress, char *buf, unsigned int bufLen) const;
    int IsModeSupported(const char *serverID, Mode mode) const;

    SPxErrorCode GetNumProfiles(const char *serverID,
				unsigned int *numProfiles,
				int update=TRUE);
    SPxErrorCode GetProfileID(const char *serverID,
			      unsigned int index,
			      char *buf, unsigned int bufLen) const;
    SPxErrorCode GetProfileName(const char *serverID,
				const char *profileID,
				char *buf, unsigned int bufLen) const;
    SPxErrorCode GetProfileSource(const char *serverID,
				  const char *profileID,
				  char *buf, unsigned int bufLen) const;
    SPxErrorCode GetProfileStreamUri(const char *serverID,
				     const char *profileID,
				     Mode mode,
				     char *buf, unsigned int bufLen) const;
    SPxErrorCode GetProfileSessionTimeout(const char *serverID,
					  const char *profileID,
					  double *timeoutSecs) const;
    int IsRelPanTiltSupported(const char *serverID,
			      const char *profileID) const;
    SPxErrorCode GetRelPTZTimeoutSecs(const char *serverID,
				      const char *profileID,
				      double *timeoutSecs) const;
    int IsRelZoomSupported(const char *serverID,
			   const char *profileID) const;
    int IsRelFocusSupported(const char *serverID,
			    const char *profileID) const;
    int IsAbsPanTiltSupported(const char *serverID,
			      const char *profileID) const;
    int IsAbsPanTiltSphereSupported(const char *serverID,
				    const char *profileID) const;
    int IsAbsPanTiltSphereDefault(const char *serverID,
				  const char *profileID) const;
    SPxErrorCode GetAbsTiltLimits(const char *serverID,
				  const char *profileID,
				  double *absTiltMin,
				  double *absTiltMax) const;
    SPxErrorCode GetAbsTiltLimitsDegs(const char *serverID,
				      const char *profileID,
				      double *absTiltMinDegs,
				      double *absTiltMaxDegs) const;
    SPxErrorCode GetAbsZoomLimits(const char *serverID,
				  const char *profileID,
				  double *absZoomMin,
				  double *absZoomMax) const;
    int IsAbsZoomSupported(const char *serverID,
			   const char *profileID) const;
    int IsAbsFocusSupported(const char *serverID,
			    const char *profileID) const;
    int IsAutoFocusSupported(const char *serverID,
			     const char *profileID) const;
    int IsMoveStatusSupported(const char *serverID,
			      const char *profileID) const;

    SPxErrorCode SetSynchronizationPoint(const char *serverID,
					 const char *profileID);

    mxml_node_s *PostPTZ(const char *serverID,
			 mxml_node_s *doc) const;
    mxml_node_s *PostImaging(const char *serverID,
			     mxml_node_s *doc) const;

    int IsEventPullSupported(const char *serverID) const;
    unsigned int GetNumEventTypes(const char *serverID) const;
    SPxErrorCode GetEventTypeName(const char *serverID,
				  unsigned int eventTypeIndex,
				  char *buf, unsigned int bufLen) const;
    SPxErrorCode SubscribeForEvents(const char *serverID,
				    const char *filter,
				    char *buf, unsigned int bufLen);
    SPxErrorCode UnsubscribeForEvents(const char *serverID,
				      const char *pullPoint);
    SPxErrorCode PollForEvents(const char *serverID,
			       const char *pullPoint,
			       Event *events,
			       unsigned int *numEvents);

    SPxErrorCode UpdateAddressForNAT(const char* serverID,
				     const char* address,
				     char *buf, unsigned int bufLen) const;

    static void CleanServerID(char *buf, unsigned int bufLen,
			      const char *serverID);

private:
    /* Private variables. */
    struct TopicInfo;
    struct ProfileInfo;
    struct ServerInfo;
    struct impl;
    SPxAutoPtr<impl> m_p;

    void recvDiscoverResp(struct sockaddr_in *from,
			  const unsigned char *payload,
			  unsigned int numBytes);
    SPxErrorCode discoverSingle(UINT32 capabilities,
				const char *serviceAddress,
				const char *serverID);
    SPxErrorCode getProfiles(ServerInfo& sInfo,
			     UINT32 capabilities);
    SPX_ATTRIB_NODISCARD
    mxml_node_s *postOnvif(ServerInfo *sInfo,
			   const char *dest,
			   mxml_node_s *doc,
			   int requireAuth=TRUE,
			   int retrying=FALSE,
			   SPxErrorCode *errRet=NULL) const;
    SPxErrorCode getServerProps(ServerInfo& sInfo,
				UINT32 reqCapabilities) const;
    SPxErrorCode getSystemDateTime(ServerInfo& sInfo) const;
    int isUnauthorised(mxml_node_s *doc) const;
    SPxErrorCode downloadFile(const char *url,
			      unsigned char *buffer,
			      unsigned int *bufferSizeBytes,
			      unsigned int timeoutMsecs,
			      const char *headers=NULL,
			      const char *post=NULL,
			      unsigned int *statusCodeRet=NULL) const;

}; /* SPxOnvifClient */

#endif /* SPX_ONVIF_CLIENT_H_ */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
