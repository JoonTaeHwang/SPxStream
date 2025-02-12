/*********************************************************************
*
* (c) Copyright 2019, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for project-435 network receiving module.
*
*
* Revision Control:
*   24/03/20 v1.5    AGC        Support fixed azimuth offset.
*
* Previous Changes:
*   24/03/20 1.4    AGC Correct received plots to North-Up.
*   15/07/19 1.3    AGC	Simplify address configuration.
*			Complete control implementation.
*   25/06/19 1.2    AGC	Derive from SPxPlotSourceIface.
*			Add control configuration.
*   20/06/19 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_NETRECVP435_H
#define _SPX_NETRECVP435_H

/*
 * Other headers required.
 */
 /* We need the base class header (which includes several others as well). */
#include "SPxLibNet/SPxNetRecv.h"
#include "SPxLibData/SPxPlotSourceIface.h"

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

/* Forward declare any classes we use. */
class SPxRIB;
class SPxNavData;
class SPxCallbackList;

/*
 * Define our specific network receiver class, derived from the
 * standard one.
 */
class SPxNetworkReceiveP435 :public SPxNetworkReceive, public SPxPlotSourceIface
{
public:
    enum WaveformType
    {
	WAVEFORM_WIDE = 0,
	WAVEFORM_NARROW = 1
    };

    /* Constructor, destructor etc. */
    explicit SPxNetworkReceiveP435(SPxRIB *buffer, SPxNavData *navData=NULL);
    virtual ~SPxNetworkReceiveP435(void);
    virtual UINT32 GetMsecsSinceLastReturn(void) { return 0; }
    virtual UINT32 GetMsecsSinceLastAziChange(void) { return 0; }

    /* Configuration. */
    SPxErrorCode SetWaveformType(WaveformType waveformType);
    SPxErrorCode GetWaveformType(WaveformType *waveformType) const;
    SPxErrorCode SetNarrowFrequencyBand(unsigned int index);
    SPxErrorCode GetNarrowFrequencyBand(unsigned int *index) const;
    SPxErrorCode SetWideFrequencyBand(unsigned int index);
    SPxErrorCode GetWideFrequencyBand(unsigned int *index) const;
    SPxErrorCode SetAzimuthOffset(double aziOffsetDegs);
    SPxErrorCode GetAzimuthOffset(double *aziOffsetDegs) const;

    /* Callback handling. */
    virtual SPxErrorCode AddPlotCallback(SPxCallbackListFn_t fn, void *userObj);
    virtual SPxErrorCode RemovePlotCallback(SPxCallbackListFn_t fn, void *userObj);

    /* Generic parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    /*
     * Private functions.
     */

    /* Overwrite the base class functions we want to handle ourselves. */
    virtual int ThreadHandleDataFn(unsigned char *data,
				   unsigned int dataSizeBytes);
    SPxErrorCode processFrame(const unsigned char *data);
    SPxErrorCode outputSpoke(UINT16 azi16,
			     int includeTarget,
			     double rangeMetres);
    SPxErrorCode sendCommand(UINT16 parNum, UINT32 value);

}; /* SPxNetworkReceiveP435 class */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NETRECVP435_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
