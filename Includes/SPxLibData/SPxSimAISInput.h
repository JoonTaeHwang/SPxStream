/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxSimAISInput object which receives AIS
*	assignment commands and relays them to simulated platforms
*	and targets.
*
*
* Revision Control:
*   16/01/18 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/
#ifndef SPX_SIM_AIS_INPUT_H_
#define SPX_SIM_AIS_INPUT_H_

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxAISStationType enum. */
#include "SPxLibNet/SPxAISDecoder.h"

/* For base classes. */
#include "SPxLibUtils/SPxObj.h"

/*
 * Types
 */

/* Forward declarations. */
class SPxRadarSimulator;
struct SPxAISAssignmentData;
class SPxSimPlatform;
class SPxSimTarget;
class SPxSimAISStation;

/* Define our class. */
class SPxSimAISInput : public SPxObj
{
public:
    /* Public functions. */
    explicit SPxSimAISInput(SPxRadarSimulator *radarSim,
			    SPxAISDecoder *aisDecoder=NULL);
    virtual ~SPxSimAISInput(void);

    /* Retrieve internal objects. */
    SPxErrorCode GetAISDecoder(SPxAISDecoder **aisDecoder);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    static void assignmentFn(SPxAISAssignmentData *data,
			     void *userObj);
    static SPxErrorCode platformFn(unsigned int index,
				   SPxSimPlatform *platform,
				   void *userArg);
    static SPxErrorCode targetFn(unsigned int index,
				 SPxSimTarget *target,
				 void *userArg);
    static UINT32 getReportIntervalMSFromOffset(UINT16 reportsPer10Min);
    static UINT32 getReportIntervalMSFromInterval(SPxAISStationType stationType,
						  UINT8 reportInterval,
						  UINT32 targetReportIntervalMS);
    static int checkStationType(SPxAISStationType stationType,
				int inland,
				UINT8 reqStationType);
    static int checkShipCargoType(UINT8 shipCargoType,
				  UINT8 reqShipCargoType);
    SPxErrorCode assignStation(SPxSimAISStation *station,
			       const SPxAISAssignmentData *data);
    SPxErrorCode assignMmsiStation(UINT32 mmsi, UINT16 offset,
				   UINT32 stationMmsi, SPxSimAISStation *station);

}; /* SPxSimAISInput. */

#endif /* SPX_SIM_AIS_INPUT_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
