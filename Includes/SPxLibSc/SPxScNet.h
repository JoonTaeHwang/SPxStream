/*********************************************************************
*
* (c) Copyright 2007 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header file for common definitions between SPxScSourceNet
*	and SPxScDestNet classes and the other network-using classes
*	such as those in SPxLibNet.
*
* Revision Control:
*   31/08/22 v1.65   SP 	Add SPxScNetIsLoopback().
*
* Previous Changes:
*   06/05/21 1.64   AJH	Remove GET_OBJ_COMMAND.
*   28/03/21 1.63   SP 	Add SPX_SCNET_HB_ID_ALARMS.
*   16/03/21 1.62   AGC	Move SPX_SOCK_INVALID to SPxCommon.h.
*   02/03/21 1.61   JGB	Add GET_OBJ_COMMAND.
*   11/12/20 1.60   SP 	Add SPX_SCNET_DEFAULT_PORT_TEWA.
*   19/11/20 1.59   SP 	Add SPxScGetLocalHostname() and SPxScGetHostname().
*   21/08/20 1.58   SP 	Add SPX_SCNET_DEFAULT_PORT_MONITOR.
*   24/04/20 1.57   AGC	Add UINT32 overload of SPxScNetIsALocalAddr().
*   09/03/20 1.56   SP 	Add CAN_SET_COMMAND.
*   21/01/20 1.55   BTB	Add SPX_SCNET_HB_ID_TRACK_CONVERSION.
*   16/01/20 1.54   SP 	Add process ID to heartbeat.
*   10/01/20 1.53   SP 	Add SPX_SCNET_HB_ID_MONITOR.
*   12/11/19 1.52   SP 	Revert last change.
*   08/11/19 1.51   SP 	Add SPX_SCNET_DEFAULT_ADDR_EXT_HEARTBEAT and
*			SPX_SCNET_DEFAULT_PORT_EXT_HEARTBEAT.
*   07/11/19 1.50   SP 	Add SPxScNetIsAddrString().
*   14/05/19 1.49   SP 	Add SPX_SCNET_DEFAULT_PORT_WEB_INTERFACE.
*   06/02/19 1.48   AGC	Make firewall check more specific.
*   15/06/18 1.47   AGC	Add SPxScNetSockToAddr().
*   30/04/18 1.46   AGC	Add SPxScNetMulticastBind().
*   18/10/17 1.45   AGC	Extend SPxScNetGetIfAddrs() with netmask/gateway/mac.
*   12/04/17 1.44   AGC	Add SPxScNetCheckFirewall().
*   21/09/16 1.43   SP 	Rename SPX_SCNET_HB_ID_RDR_AGENT to SPX_SCNET_HB_ID_RDR.
*   13/09/16 1.42   SP 	Add numChannels to heartbeat.
*   09/09/16 1.41   SP 	Change RDR agent ID.
*                       Add SPX_SCNET_DEFAULT_PORT_RDR_AGENT.
*   02/09/16 1.40   SP 	Add SPX_SCNET_HB_ID_RDR_AGENT.
*   14/10/15 1.39   AGC	Add SPxScNetIsMulticast() taking UINT32 address.
*   26/01/15 1.38   SP 	Add SPxScNetStringToAddr().
*   26/09/14 1.37   REW	Add SPX_SCNET_DEFAULT_PORT_SAFETY_NET etc.
*   10/09/14 1.36   REW	Add SPX_SCNET_HB_ID_SAFETY_NET.
*   22/01/14 1.35   REW	Add SPxScNetIsBroadcast().
*   15/11/13 1.34   SP 	Add BIN_GET_COMMAND.
*   04/10/13 1.33   AGC	Simplify headers.
*   17/06/13 1.32   REW	Add sourceType to heartbeat structure.
*   24/05/13 1.31   AGC	Skip non-multicastable interfaces.
*   15/05/13 1.30   AGC	Modify ASTERIX default string to match format.
*   19/04/13 1.29   REW	Add ASTERIX and NMEA defaults.
*   13/02/13 1.28   REW	Add numBytes option to send/recv buf functions.
*   04/02/13 1.27   REW	Add funcs to drop multicast group memberships.
*   22/11/12 1.26   REW	Add PAN_VIEW_COMMAND for PanView().
*   07/09/12 1.25   AGC	Add tag names for structures.
*			Use const for SPxScNetSetupAddr() host param.
*   31/08/12 1.24   AGC	Add loopback to SPxScNetIfStruct.
*   21/08/12 1.23   AGC	Add SPxScNetGetIfAddrs().
*   02/08/12 1.22   AGC	Move SOCKET_TYPE and SOCKLEN_TYPE to SPxCommon.h.
*   29/05/12 1.21   AGC	Add SPxScNetSetTTL().
*   14/11/11 1.20   AGC	Add SPxScNetSetKeepAlive().
*   27/06/11 1.19   AGC	Add SPX_SCNET_DEFAULT_PORT_VIDEO,
*			and SPX_SCNET_DEFAULT_ADDR_VIDEO.
*   09/06/11 1.18   AGC	Add SPX_SCNET_HB_ID_AV_SERVER.
*   21/10/10 1.17   REW	Add SPX_SCNET_HB_ID_DIST_IMAGES.
*   22/02/10 1.16   REW	Add SPX_SCNET_HB_ID_FUSION.
*   30/10/09 1.15   REW	Add SPxScNetMakeBlocking().
*   21/08/09 1.14   REW	Add SET_SWEEPLINE_COMMAND.
*   23/07/09 1.13   REW	Add SPxScNetIsALocalAddr().
*   22/07/09 1.12   REW	Add SPX_SCNET_HB_ID_SCREEN_REC.
*   09/02/09 1.11   REW	Add health and sourceStatus to heartbeats.
*   25/09/08 1.10   REW	Add SPX_SOCK_INVALID.
*   19/09/08 1.9    REW	Set SPX_SCNET_DEFAULT_PORT_REPORTS to 5079.
*   16/09/08 1.8    REW	Add SPX_SCNET_DEFAULT_PORT_REPORTS.
*   27/08/08 1.7    REW	Add SPxScNetFormatAddr().
*   11/02/08 1.6    REW	Add SPX_SCNET_PATCH_ACK_MAGIC.
*   07/09/07 1.5    REW	Add GET_COMMAND and SET_COMMAND.
*   25/07/07 1.4    REW	More utility functions, radar defaults etc.
*   09/07/07 1.3    REW	Add SPX_SCNET_HB_STATE_ definitions.
*   06/07/07 1.2    REW	Change defaults.  Change heartbeats.  Add
*			more definitions and utility functions.
*   04/07/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_SC_NET_H
#define _SPX_SC_NET_H

/*
 * Other headers required.
 */

#include <stdio.h>

/* Common types etc. */
#include "SPxLibUtils/SPxCommon.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Default port numbers for client/server connections and heartbeats.
 *
 * The Internet Assigned Numbers Authority (IANA) has assigned port
 * numbers 4377 and 4378 to the Cambridge Pixel SPx Server and Display
 * applications respectively.
 *
 * For more information, see http://www.iana.org/assignments/port-numbers.
 *
 * The TCP ports with those numbers are used for the server connections
 * (which perform handshaking with clients to exchange dynamically
 * assigned data ports).  One of the UDP ports is used as the default
 * for heartbeats and the other as the default for radar distribution.
 *
 * In addition, port 5079 has been assigned for UDP use only for Cambridge
 * Pixel SPx Reports (tracks etc.).
 */
#define	SPX_SCNET_DEFAULT_PORT_SERVER		4377	/* TCP Scan Conv */
#define	SPX_SCNET_DEFAULT_PORT_DISPLAY		4378	/* TCP Display */
#define SPX_SCNET_DEFAULT_PORT_RDR_AGENT	4379	/* TCP RDR Agent */
#define SPX_SCNET_DEFAULT_PORT_MONITOR		4380	/* TCP SPx Monitor */
#define SPX_SCNET_DEFAULT_PORT_TEWA		4381	/* TCP TEWA applications. */
#define	SPX_SCNET_DEFAULT_PORT_HEARTBEAT	4377	/* UDP */
#define	SPX_SCNET_DEFAULT_PORT_RADAR		4378	/* UDP */
#define SPX_SCNET_DEFAULT_PORT_VIDEO		4379	/* UDP */
#define	SPX_SCNET_DEFAULT_PORT_REPORTS		5079	/* UDP */
#define	SPX_SCNET_DEFAULT_PORT_ASTERIX		8600	/* UDP */
#define	SPX_SCNET_DEFAULT_PORT_SAFETY_NET	8650	/* UDP */
#define	SPX_SCNET_DEFAULT_PORT_WEB_INTERFACE	8080	/* TCP Web Server */
#define	SPX_SCNET_DEFAULT_PORT_NMEA		10110	/* UDP */

/* Define values for the heartbeat serverID field.  These values can
 * be OR'ed together to indicate to listeners what services the sender
 * can provide.
 *
 * For example, a server providing display functionality would use
 *	SPX_SCNET_HB_ID_SERVER | SPX_SCNET_HB_ID_DISPLAY.
 *
 * A server providing scan-conversion functionality would use
 *	SPX_SCNET_HB_ID_SERVER | SPX_SCNET_HB_ID_SCANCONV.
 *
 * The "SPxHeartbeats" test program decodes and reports the various
 * combinations of these for each server type.
 */
#define	SPX_SCNET_HB_ID_SERVER			0x1000	/* Radar Server */
#define	SPX_SCNET_HB_ID_AV_SERVER		0x2000	/* Video Server */
#define	SPX_SCNET_HB_ID_RDR			0x4000	/* RDR */
#define	SPX_SCNET_HB_ID_MONITOR			0x8000	/* Monitor */
#define	SPX_SCNET_HB_ID_DISPLAY			0x0001
#define	SPX_SCNET_HB_ID_SCANCONV		0x0002
#define	SPX_SCNET_HB_ID_DISTRIBUTION		0x0004	/* Radar video */
#define	SPX_SCNET_HB_ID_RECORDING		0x0008
#define	SPX_SCNET_HB_ID_TRACKING		0x0010
#define	SPX_SCNET_HB_ID_FUSION			0x0020
#define	SPX_SCNET_HB_ID_DIST_IMAGES		0x0040	/* Images */
#define	SPX_SCNET_HB_ID_SAFETY_NET		0x0080
#define	SPX_SCNET_HB_ID_SCREEN_REC		0x0100
#define	SPX_SCNET_HB_ID_TRACK_CONVERSION	0x0200
#define	SPX_SCNET_HB_ID_ALARMS			0x0400

/* Define some of the various states that can be reported in heartbeats,
 * although some applications may report more.
 */
#define	SPX_SCNET_HB_STATE_NOT_SET		0
#define	SPX_SCNET_HB_STATE_INITIALISED		1
#define	SPX_SCNET_HB_STATE_CONNECTED		2
#define	SPX_SCNET_HB_STATE_DISCONNECTED		3


/*
 * Define the default multicast addresses.
 *
 * We use addresses from the "IPv4 Organization Local Scope" (239.192.0.0/14)
 * as described in RFC 2365, "Administratively Scoped IP Multicast".  Local
 * system administrators or network specialists should be able to configure
 * organisational networks to support these, or they can normally be changed
 * with run-time options if necessary.
 */
#define	SPX_SCNET_DEFAULT_ADDR_HEARTBEAT	"239.192.43.77"
#define	SPX_SCNET_DEFAULT_ADDR_RADAR		"239.192.43.78"
#define SPX_SCNET_DEFAULT_ADDR_VIDEO		"239.192.43.79"
#define	SPX_SCNET_DEFAULT_ADDR_REPORTS		"239.192.50.79"
#define	SPX_SCNET_DEFAULT_ADDR_ASTERIX		"239.192.86.0"
#define	SPX_SCNET_DEFAULT_ADDR_SAFETY_NET	"239.192.86.50"
#define	SPX_SCNET_DEFAULT_ADDR_NMEA		"239.192.101.10"


/*
 * Define magic numbers for patch and heartbeats.
 */
#define SPX_SCNET_PATCH_MAGIC			0xC0DE3174
#define SPX_SCNET_PATCH_ACK_MAGIC		0xC0DE3175
#define SPX_SCNET_HEARTBEAT_MAGIC		0xC0DE7241


/*
 * Various maximum sizes etc.
 */
/* Maximum size of the buffer (in units of UINT32) to hold patch commands. */
#define SPX_SCNET_MAX_SIZE_PATCH_BUF_UINT32	1024

/* Maximum size (in bytes) of a buffer to hold the command input.
 * The biggest request is the LUT set (256 x 4 + command string bytes)
 */
#define SPX_SCNET_MAX_SIZE_COMMAND_BUF_BYTES	2048

/*
 * Debug flags.
 */
#define	SPX_SCNET_DEBUG_CONNECTS		0x00000001
#define	SPX_SCNET_DEBUG_SOCKETS			0x00000002
#define	SPX_SCNET_DEBUG_HANDSHAKES		0x00000004
#define	SPX_SCNET_DEBUG_COMMANDS		0x00000010
#define	SPX_SCNET_DEBUG_PATCHES			0x00000020
#define	SPX_SCNET_DEBUG_MESSAGES		0x00000040
#define	SPX_SCNET_DEBUG_USEREVENT		0x00000080
#define	SPX_SCNET_DEBUG_BACKGROUND		0x00000100
#define	SPX_SCNET_DEBUG_VERBOSE			0x80000000


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

/*
 * Structure for heartbeat messages.
 */
typedef struct SPxScNetHeartbeatStruct_tag
{
    UINT32 magic;		/* Magic number, SPX_SCNET_HEARTBEAT_MAGIC */
    UINT16 appId;		/* ID of server - see SPX_SCNET_HB_ID defs */
    UINT8 health;		/* Health of server (see SPxSystemHealth.h) */
    UINT8 state;		/* State of server, see SPX_SCNET_HB_STATE...*/
    UINT32 clientAddr;		/* Address of the client (0 if no connect) */
    UINT16 listeningPort;	/* Port that server accepts connections on */
    UINT16 sourceStatus;	/* Source status information */
    UINT8 sourceType;		/* Type of source, SPX_RIB_SOURCETYPE_... */
    UINT8 numChannels;		/* Number of channels, sources, screens, etc. */
    UINT16 processId;		/* Last two bytes of the application's process ID. */
} SPxScNetHeartbeatStruct;

/*
 * Structure for interface information.
 */
typedef struct SPxScNetIfStruct_tag
{
    UINT32 addr;		/* Interface address in host byte order. */
    UINT8 loopback;		/* Is this a loopback interface? */
    UINT8 multicast;		/* Can this interface multicast? */
    UINT8 enabled;		/* Is interface enabled? */
    UINT8 connected;		/* Is interface connected? */
    UINT8 multicastDefault;	/* Is interface default for multicast? */
    char name[256];		/* Interface name. */
    UINT32 netmask;		/* Interface mask. */
    UINT32 gateway;		/* Interface gateway. */
    UINT64 mac;			/* MAC address. */

} SPxScNetIfStruct;

/*********************************************************************
*
*   Variables
*
**********************************************************************/

/* Commands sent across the command interface. */
extern const char * HANDSHAKE_COMMAND;
extern const char * NEW_WIN_COMMAND;
extern const char * DEL_WIN_COMMAND;
extern const char * SET_WIN_PARAM_COMMAND;
extern const char * SET_WIN_POS_COMMAND;
extern const char * SET_WIN_GEOM_COMMAND;
extern const char * SET_FADE_COMMAND;
extern const char * SET_VIEW_COMMAND;
extern const char * PAN_VIEW_COMMAND;
extern const char * SET_COLOUR_COMMAND;
extern const char * SET_BRIGHT_COMMAND;
extern const char * SET_LUT_COMMAND;
extern const char * SHOW_RADAR_COMMAND;
extern const char * SET_SOURCE_COMMAND;
extern const char * SET_RADAR_POS_COMMAND;
extern const char * SET_SWEEPLINE_COMMAND;
extern const char * CAN_SET_COMMAND;
extern const char * SET_COMMAND;
extern const char * GET_COMMAND;

/* Commands sent across the binary interface. */
extern const char * BIN_GET_COMMAND;

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* Extern the utility functions used by various modules. */
extern int SPxScNetCloseSocket(SOCKET_TYPE sock);
extern int SPxScNetSetupAddr(struct sockaddr_in *addr,
			     const char *host,
			     int port,
			     FILE *logFile);
extern int SPxScNetFormatAddr(UINT32 addr, char *buf, int bufSizeBytes);
extern int SPxScNetIsAddrString(const char *addr);
extern UINT32 SPxScNetStringToAddr(const char *addr);
extern UINT32 SPxScNetSockToAddr(struct sockaddr_in *addr);
extern int SPxScNetIsALocalAddr(SOCKET_TYPE sock, UINT32 addr);
extern int SPxScNetIsALocalAddr(UINT32 addr);
extern int SPxScNetGetIfAddrs(SPxScNetIfStruct *addrs, unsigned int *numAddr);
extern int SPxScNetMakeNonBlocking(SOCKET_TYPE sock, FILE *logFile);
extern int SPxScNetMakeBlocking(SOCKET_TYPE sock, FILE *logFile);
extern int SPxScNetSetReuseaddr(SOCKET_TYPE sock, FILE *logFile);
extern int SPxScNetSetKeepAlive(SOCKET_TYPE sock, 
				UINT32 timeoutMsecs,
				FILE *logFile);
extern int SPxScNetSetTTL(SOCKET_TYPE sock, int ttl, FILE *logFile);
extern int SPxScNetIncreaseRecvBuf(SOCKET_TYPE sock, FILE *logFile,
				unsigned int numBytes=0);
extern int SPxScNetIncreaseSendBuf(SOCKET_TYPE sock, FILE *logFile,
				unsigned int numBytes=0);
extern int SPxScNetIsBroadcast(struct sockaddr_in *addr, FILE *logFile);
extern int SPxScNetIsMulticast(struct sockaddr_in *addr, FILE *logFile);
extern int SPxScNetIsMulticast(UINT32 addr, FILE *logFile);
extern int SPxScNetIsLoopback(UINT32 addr);
extern int SPxScNetJoinMulticastOnAllInterfaces(SOCKET_TYPE sock,
				struct in_addr *mcastAddr,
				FILE *logFile);
extern int SPxScNetJoinMulticastOnInterface(SOCKET_TYPE sock,
				struct in_addr *mcastAddr,
				struct in_addr *ifAddr,
				FILE *logFile);
extern int SPxScNetJoinOrDropMcastOnAllInterfaces(SOCKET_TYPE sock,
				struct in_addr *mcastAddr,
				int joinNotDrop,
				FILE *logFile);
extern int SPxScNetJoinOrDropMcastOnInterface(SOCKET_TYPE sock,
				struct in_addr *mcastAddr,
				struct in_addr *ifAddr,
				int joinNotDrop,
				FILE *logFile);
extern int SPxScNetCanMulticastBind(void);
extern int SPxScNetCheckFirewall(int port, int isTCP);
extern int SPxScGetLocalHostname(char *buf, unsigned int bufLen);
extern int SPxScGetHostname(UINT32 addr, char *buf, unsigned int bufLen);

#endif /* _SPX_SC_NET_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
