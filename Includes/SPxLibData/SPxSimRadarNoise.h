/*********************************************************************
*
* (c) Copyright 2016, 2018, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxSimRadarNoise object which generates noise for
*	a a simulated radar for use with the SPxRadarSimulator.
*
*
* Revision Control:
*   06/07/22 v1.5    AGC	Add GetPeakNoiseAtRange() function.
*
* Previous Changes:
*   07/02/18 1.4    AGC	Fix mutex issues.
*   02/02/18 1.3    AGC	Support fixed number of spokes.
*   03/03/16 1.2    AGC	Generate 16-bit noise.
*   11/01/16 1.1    AGC	Initial version.
**********************************************************************/
#ifndef SPX_SIM_RADAR_NOISE_H_
#define SPX_SIM_RADAR_NOISE_H_

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/* For SPxPackingChannel. */
#include "SPxLibData/SPxRib.h"

/*
 * Types
 */

/* Forward declarations. */
class SPxSimRadar;

/* Define our class. */
class SPxSimRadarNoise : public SPxObj
{
public:
    /* Public functions. */
    explicit SPxSimRadarNoise(SPxSimRadar *radar,
	unsigned int numSamples,
	unsigned int numSpokes, int supportNearNoise);
    virtual ~SPxSimRadarNoise(void);

    SPxErrorCode SetBackgroundNoiseLevel(double noiseLevel);
    SPxErrorCode GetBackgroundNoiseLevel(double *noiseLevel) const;
    SPxErrorCode EnableNearNoise(int enableNearNoise);
    int IsNearNoiseEnabled(void) const;
    SPxErrorCode SetNearNoiseGain(double nearNoiseGain);
    SPxErrorCode GetNearNoiseGain(double *nearNoiseGain) const;
    SPxErrorCode SetNearNoiseRangeMetres(double nearNoiseRangeMetres);
    SPxErrorCode GetNearNoiseRangeMetres(double *nearNoiseRangeMetres) const;
    SPxErrorCode SetAlwaysNearNoise(int alwaysNearNoise);
    int IsAlwaysNearNoise(void) const;
    SPxErrorCode SetSeaClutterDensity(double seaClutterDensity);
    SPxErrorCode GetSeaClutterDensity(double *seaClutterDensity) const;
    SPxErrorCode SetNearNoiseVariation(double nearNoiseVariation);
    SPxErrorCode GetNearNoiseVariation(double *nearNoiseVariation) const;

    SPxErrorCode GetPeakNoiseAtRange(double rangeMetres, double *noiseLevel) const;

    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Functions called by SPxSimRadar. */
    SPxErrorCode SetNumSamples(unsigned int numSamples);
    SPxErrorCode SetNumSpokes(unsigned int numSpokes);
    SPxErrorCode ClearNearNoise(void);

    SPxErrorCode BuildNoise(unsigned char *buffer,
			    unsigned int numSamples,
			    UCHAR packing,
			    SPxPackingChannel channel);
    SPxErrorCode BuildNoise(unsigned int numSamples);
    SPxErrorCode BuildNearNoise(unsigned int numSamples,
	unsigned int bytesPerSample,
	double startRangeMetres, double endRangeMetres,
	unsigned int spoke, double prf, double beamwidthSpokes,
	double pulseLengthUsecs);
    const UINT16 *GetNoise(void) const;
    const UINT16 *GetNearNoise(void) const;
    unsigned int GetNearNoiseSamplesUsed(void) const;

    /* Private functions. */
    SPxErrorCode updateNearNoiseBlobs(unsigned int nearNoiseSamples,
	unsigned int blobWidth);
    SPxErrorCode clearNearNoise(void);
    SPxErrorCode buildNoise(unsigned char *buffer,
			    unsigned int numSamples,
			    UCHAR packing,
			    SPxPackingChannel channel);

    friend class SPxSimRadar;

}; /* SPxSimRadarNoise. */

#endif /* SPX_SIM_RADAR_NOISE_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/
