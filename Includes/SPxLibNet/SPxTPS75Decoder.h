/*********************************************************************
*
* (c) Copyright 2012 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxTPS75Decoder class.
*
*	This class can be used to decode a TPS-75 message buffer and
*	build an SPx Extended Track message from the information.
*
*	TPS-75 message content comes from a specification
*	(confidential) obtained as part of project P478.
*
*
* Revision Control:
*   14/10/20 v1.2    BTB	Fix header and formatting.
*
* Previous Changes:
*   12/10/20 1.1    BTB	Initial Version.
**********************************************************************/

#ifndef _SPX_TPS75_DECODER_H
#define _SPX_TPS75_DECODER_H

/*
 * Other headers required.
 */
#include "SPxLibData/SPxPackets.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxCallbackList.h"

/* We need our base class. */
#include "SPxLibUtils/SPxObj.h"

class SPxTPS75Decoder : public SPxObj
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxTPS75Decoder(void);
    virtual ~SPxTPS75Decoder(void);

    /* Functions to configure sensor information. */
    SPxErrorCode SetSensorID(unsigned int sensorID);
    unsigned int GetSensorID(void) { return m_sensorID; }

    SPxErrorCode SetSensorLatLongAvailable(int available);
    int IsSensorLatLongAvailable(void) { return m_sensorLatLongAvailable; }

    SPxErrorCode SetSensorLatLong(double latDegs, double longDegs);
    double GetSensorLatDegs(void) { return m_sensorLatDegs; }
    double GetSensorLongDegs(void) { return m_sensorLongDegs; }

    /* Callback handling functions. */
    SPxErrorCode AddExtendedTrackCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode RemoveExtendedTrackCallback(SPxCallbackListFn_t fn, void *userObj);

    /* Decode function. */
    SPxErrorCode Decode(const UINT16 *tpsMsgBuf,
                        unsigned int bufSizeWords,
                        int extendedMsg) const;

    /* Static conversion function. */
    static SPxErrorCode BuildSPxTrack(const UINT16 *tpsMsgBuf,
                                      unsigned int bufSizeWords,
                                      SPxPacketTrackExtended *track,
                                      int extendedMsg,
                                      int sensorID = 0,
                                      int sensorLatLongValid=FALSE,
                                      double sensorLatDegs=0.0,
                                      double sensorLongDegs=0.0);

private:
    /*
     * Private variables.
     */

    /* Sensor information. */
    unsigned int m_sensorID;
    int m_sensorLatLongAvailable;
    double m_sensorLatDegs;
    double m_sensorLongDegs;

    /* Callback list for passing on extended track messages. */
    SPxCallbackList *m_extTrackCBList;

}; /* SPxTPS75Decoder */

#endif /* _SPX_TPS75_DECODER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
