/*********************************************************************
*
* (c) Copyright 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxNetRecvAsterixPlots.h,v $
* ID: $Id: SPxNetRecvAsterixPlots.h,v 1.4 2012/12/12 17:11:22 rew Exp $
*
* Purpose:
*   Header for Asterix plots network receiving class.
*
* Revision Control:
*   12/12/12 v1.4    AGC	Add new BufferDegs parameter.
*
* Previous Changes:
*   12/06/12 1.3    AGC	Move CAT48 plots after antenna.
*   06/06/12 1.2    AGC	Make SPxNetworkReceive functions public.
*			Add IsThreadEnabled() function for testing.
*   29/05/12 1.1    AGC	Initial Version.
*********************************************************************/

#ifndef _SPX_NET_RECV_ASTERIX_PLOTS_H
#define _SPX_NET_RECV_ASTERIX_PLOTS_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* We need the base class header (which includes several others as well). */
#include "SPxLibNet/SPxNetRecv.h"


/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Macros
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/* Forward declarations. */
struct SPxNetworkReceiveAsterixPlotsPriv;
class SPxAsterixDecoder;
class SPxAsterixMsg;

/*
 * Define our network receiver class for Asterix plots,
 * derived from the standard one.
 */
class SPxNetworkReceiveAsterixPlots : public SPxNetworkReceive
{
public:
    /*
     * Public functions.
     */

    /* Constructor, destructor etc. */
    SPxNetworkReceiveAsterixPlots(SPxRIB *buffer, int useTcp=TRUE);
    virtual ~SPxNetworkReceiveAsterixPlots();

    /* Overwride the base class functions we want to handle ourselves. */
    virtual SPxErrorCode Create(const char *address=NULL, int port=0, 
				const char *ifAddr=NULL);

    /* Set/Get parameters. */
    SPxErrorCode SetRandomData(int *randomDataBuf, unsigned int randomDataCount);
    SPxErrorCode SetNumSamples(unsigned int numSamples);
    unsigned int GetNumSamples(void) const;
    SPxErrorCode SetNumAzimuths(unsigned int numAzimuths);
    unsigned int GetNumAzimuths(void) const;
    SPxErrorCode SetEndRangeMetres(double endRangeMetres);
    double GetEndRangeMetres(void) const;
    SPxErrorCode SetPlotSize(double aziDegs, double rangeMetres);
    SPxErrorCode GetPlotSize(double *aziDegs, double *rangeMetres) const;
    SPxErrorCode UsePlotSizeFromMessage(int usePlotSizeFromMessage);
    int IsPlotSizeFromMessage(void) const;
    SPxErrorCode SetPlotIntensity(unsigned char intensity);
    unsigned char GetPlotIntensity(void) const;
    SPxErrorCode UsePlotIntensityFromMessage(int usePlotIntensityFromMessage);
    int IsPlotIntensityFromMessage(void) const;
    SPxErrorCode SetDetectionProbability(double detectionProbability);
    double GetDetectionProbability(void) const;
    SPxErrorCode SetMaxWeaveDegs(double maxWeaveDegs);
    double GetMaxWeaveDegs(void) const;
    SPxErrorCode SetBufferDegs(double bufferDegs);
    double GetBufferDegs(void) const;
    SPxErrorCode SetClutterArea(const char *name, 
	double startAziDegs, double endAziDegs, 
	double startRangeMetres, double endRangeMetres,
	unsigned int plotCount);
    SPxErrorCode GetClutterArea(const char *name,
	double *startAziDegs, double *endAziDegs, 
	double *startRangeMetres, double *endRangeMetres,
	unsigned int *plotCount) const;
    SPxErrorCode RemoveClutterArea(const char *name);

    /* Functions from SPxNetworkReceive. */
    virtual int ThreadStateChangedFn(void);
    virtual int ThreadHandleDataFn(unsigned char *data,
				   unsigned int dataSizeBytes);
    int IsThreadEnabled(void) { return m_threadThinksEnabled; }

    /* Generic parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private types.
     */
    SPxAutoPtr<SPxNetworkReceiveAsterixPlotsPriv> m_p;

    /*
     * Private functions.
     */
    /* Functions from SPxObj. */
    virtual SPxErrorCode SetConfigFromState(void);

    /* Internal functions. */
    static void processCat34Wrapper(const SPxAsterixDecoder *decoder, 
	void *userArg, UINT8 category, SPxAsterixMsg *msg);
    void processCat34(const SPxAsterixDecoder *decoder, 
	UINT8 category, SPxAsterixMsg *msg);
    static void processCat48Wrapper(const SPxAsterixDecoder *decoder, 
	void *userArg, UINT8 category, SPxAsterixMsg *msg);
    void processCat48(const SPxAsterixDecoder *decoder, 
	UINT8 category, SPxAsterixMsg *msg);
    SPxErrorCode renderPlot(double posAziDegs, double posRangeMetres,
			    double sizeAziDegs, double sizeRangeMetres,
			    UINT8 intensity,
			    int movePlotsAfterAntenna);
    SPxErrorCode writeReturnToRib(unsigned int azimuthNumber, UINT8 *data);

    /* Utility functions */
    inline unsigned int aziAdd(unsigned int a1, unsigned int a2);
    inline unsigned int aziSub(unsigned int a1, unsigned int a2);
    inline unsigned int aziDiff(unsigned int a1, unsigned int a2);
    inline int aziOverlap(unsigned int a1, unsigned int a2,
	unsigned int b1, unsigned int b2);
    inline double aziToDegs(unsigned int azi);
    inline unsigned int degsToAzi(double degs);
    inline double sampleToMetres(unsigned int sample);
    inline unsigned int metresToSample(double metres);

}; /* SPxNetworkReceiveAsterixPlots class */


/*********************************************************************
 *
 *   Function prototypes
 *
 **********************************************************************/

#endif /* _SPX_NET_RECV_ASTERIX_PLOTS_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/
