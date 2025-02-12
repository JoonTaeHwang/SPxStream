/*********************************************************************
*
* (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxSimTargetTemplate object which represents 
*	template for a simulated target within the SPxRadarSimulator.
*
*
* Revision Control:
*   06/07/22 v1.8    AGC	Support intensity above background noise.
*
* Previous Changes:
*   23/08/16 1.7    AGC	Improve ordering of target templates.
*   23/05/16 1.6    AGC	Modify SPX_SIM_TGT_TEMPLATE_FLAGS_ALL value.
*   16/05/14 1.5    AGC	Support symmetric shapes.
*			Support remote control.
*			Add SetFromTarget() function.
*   06/06/13 1.4    AGC	Support multiple simulation files.
*   24/05/13 1.3    AGC	Support multilateration targets.
*   03/05/13 1.2    AGC	Support ADS-B.
*   25/04/13 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_SIM_TARGET_TEMPLATE_H
#define _SPX_SIM_TARGET_TEMPLATE_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/* For SPxSimTargetIntensity. */
#include "SPxLibData/SPxSimTarget.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/*
 * Types
 */

/* Forward declarations. */
class SPxRadarSimulator;
class SPxSimTarget;

/* Flags specifying template validity. */
enum SPxSimTargetTemplateFlags
{
    SPX_SIM_TGT_TEMPLATE_FLAGS_NONE	    = 0x00000000,
    SPX_SIM_TGT_TEMPLATE_FLAGS_PRIMARY	    = 0x00000001,
    SPX_SIM_TGT_TEMPLATE_FLAGS_TRACK	    = 0x00000002,
    SPX_SIM_TGT_TEMPLATE_FLAGS_AIS	    = 0x00000004,
    SPX_SIM_TGT_TEMPLATE_FLAGS_ADSB	    = 0x00000008,
    SPX_SIM_TGT_TEMPLATE_FLAGS_LENGTH	    = 0x00000010,
    SPX_SIM_TGT_TEMPLATE_FLAGS_BREADTH	    = 0x00000020,
    SPX_SIM_TGT_TEMPLATE_FLAGS_SHAPE	    = 0x00000040,
    SPX_SIM_TGT_TEMPLATE_FLAGS_INTENSITY    = 0x00000080,
    SPX_SIM_TGT_TEMPLATE_FLAGS_MULTILAT	    = 0x00000100,

    SPX_SIM_TGT_TEMPLATE_FLAGS_ALL	    = 0x7FFFFFFF
};

/* Define our class. */
class SPxSimTargetTemplate : public SPxObj
{
public:
    /* Public functions. */
    explicit SPxSimTargetTemplate(SPxRadarSimulator *radarSim, const char *name=NULL);
    virtual ~SPxSimTargetTemplate(void);

    /* Template name. */
    SPxErrorCode SetName(const char *name);
    SPxErrorCode GetName(char *buffer, unsigned int bufLen) const;

    UINT32 SetFlags(UINT32 flags);
    UINT32 GetFlags(void) const;
    UINT32 AddFlags(UINT32 flags);
    UINT32 RemoveFlags(UINT32 flags);

    /* Target properties. */
    SPxErrorCode EnablePrimary(int enable);
    int IsPrimary(void) const;
    SPxErrorCode EnableTrack(int enable);
    int IsTrack(void) const;
    SPxErrorCode EnableMultilat(int enable);
    int IsMultilat(void) const;
    SPxErrorCode EnableAIS(int enable);
    int IsAIS(void) const;
    SPxErrorCode EnableADSB(int enable);
    int IsADSB(void) const;
    SPxErrorCode SetLengthMetres(double lengthMetres);
    SPxErrorCode GetLengthMetres(double *lengthMetres) const;
    SPxErrorCode SetBreadthMetres(double breadthMetres);
    SPxErrorCode GetBreadthMetres(double *breadthMetres) const;
    SPxErrorCode SetShape(const SPxPoint *points, unsigned int numPoints, int symmetric=FALSE);
    SPxErrorCode GetShape(SPxPoint *points, unsigned int *pointsLen, int *symmetric=NULL) const;
    SPxErrorCode SetIntensityType(SPxSimTargetIntensity intensityType);
    SPxErrorCode GetIntensityType(SPxSimTargetIntensity *intensityType) const;
    SPxErrorCode SetRcs(double rcs);
    SPxErrorCode GetRcs(double *rcs) const;
    SPxErrorCode SetIntensityAboveBackground(double intensityAboveBackground);
    SPxErrorCode GetIntensityAboveBackground(double *intensityAboveBackground) const;

    SPxErrorCode SetFromTarget(SPxSimTarget *target);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Called by SPxRadarSimulator. */
    SPxErrorCode RemoveRadarSim(void);
    void ApplyDesc(const char *buffer);
    const char *GetDesc(void) const;
    void SetFile(const char *filename);
    int WasFromFile(const char *filename);

    /* Private functions. */
    SPxSimTargetTemplate *findTemplate(const char *name);
    SPxErrorCode setName(const char *name, int genEvent = TRUE);

    friend class SPxRadarSimulator;

}; /* SPxSimTargetTemplate. */

#endif /* _SPX_SIM_TARGET_TEMPLATE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
