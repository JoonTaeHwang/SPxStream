/*********************************************************************
*
* (c) Copyright 2018, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxSimAISStation object which represents
*	simulated AIS mobile or base station.
*
*
* Revision Control:
*   01/10/21 v1.4    AGC	Add draught to AIS info.
*
* Previous Changes:
*   10/02/21 1.3    AGC	Fix typo in Update() param name.
*   02/02/18 1.2    AGC	Support legacy Class B static messages.
*			Support configurable RAIM and Class B flags.
*   10/01/18 1.1    AGC	Initial version.
**********************************************************************/
#ifndef SPX_SIM_AIS_STATION_H_
#define SPX_SIM_AIS_STATION_H_

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base classes. */
#include "SPxLibUtils/SPxObj.h"

/*
 * Types
 */

/* Forward declarations. */
struct SPxAISInfo;
struct SPxSimState;
class SPxRadarSimulator;

/* Define our class. */
class SPxSimAISStation : public SPxObj
{
public:
    /* Public functions. */
    explicit SPxSimAISStation(SPxRadarSimulator *radarSim);
    virtual ~SPxSimAISStation(void);

    SPxErrorCode SetAISName(const char *aisName);
    SPxErrorCode GetAISName(char *aisName, unsigned int bufLen) const;
    SPxErrorCode SetDestinationFallback(const char *destinationFallback);

    SPxErrorCode SetUseLegacyStatic(int useLegacyStatic);
    int IsUsingLegacyStatic(void) const;

    SPxErrorCode SetAISInfo(const SPxAISInfo *aisInfo);
    SPxErrorCode GetAISInfo(SPxAISInfo *aisInfo) const;
    SPxErrorCode SetAISIntervalMS(UINT32 intervalMS, UINT8 quietTime, UINT8 transmitMode,
				  UINT32 validMS);
    SPxErrorCode GetAISIntervalMS(UINT32 *intervalMS) const;
    SPxErrorCode GetCurrentAISIntervalMS(UINT32 *intervalMS) const;
    SPxErrorCode GetTransmitMode(UINT32 *transmitMode) const;
    int IsQuietTime(void) const;
    SPxErrorCode GetCurrentState(SPxSimState *state) const;

    SPxErrorCode Update(UINT32 mmsiFallback,
			int ownShip,
			const SPxSimState *state,
			double headingOffsetDegs,
			double lengthMetres,
			double breadthMetres,
			int isAIS, int changed);
    const char *GetDesc(void) const;

    unsigned int BuildPos(char *sentence, unsigned int sizeBytes);
    int IsStaticARequired(void) const;
    int IsStaticBRequired(void) const;
    unsigned int BuildStaticA(char *sentence, unsigned int sizeBytes);
    unsigned int BuildStaticB(char *sentence, unsigned int sizeBytes);

    SPxErrorCode SendBaseMessage(char *sentence, unsigned int sizeBytes);

    static bool CompareAISInfo(const SPxAISInfo& lhs, const SPxAISInfo& rhs);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    SPxErrorCode getMMSI(UINT32 *mmsi) const;
    SPxErrorCode getDestination(char *destination, unsigned int bufLen) const;
    SPxErrorCode getAISIntervalMS(UINT32 *intervalMS) const;
    SPxErrorCode getCurrentAISIntervalMS(UINT32 *intervalMS) const;
    SPxErrorCode setAISPosTime(void);
    SPxErrorCode setAISStaticTime(void);
    char nextAISPosChannel(void);
    char nextAISStaticChannel(void);
    int nextAISMessageID(void);
    UINT32 toggleChannel(UINT32 &channel);

}; /* SPxSimAISStation. */

#endif /* SPX_SIM_AIS_STATION_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
