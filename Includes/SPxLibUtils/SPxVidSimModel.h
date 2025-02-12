/*********************************************************************
 *
 * (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx Video Simulator SPxVidSimModel class header.
 *
 * Revision Control:
 *   22/07/22 v1.16  SP 	Include SPxAssimp.h.
 *
 * Previous Changes:
 *   22/07/22 1.15  SP 	Suppress more assimp header warnings.
 *   22/07/22 1.14  SP 	Changes to simplify creation and deletion.
 *   04/12/17 1.13  AGC	Avoid new clang Windows warnings.
 *   01/12/16 1.12  SP 	Move ZRefMode_t to SPxVidSimObj.h.
 *   30/11/16 1.11  SP 	Add Get/SetModelName().
 *   31/10/16 1.10  SP 	Support Z reference mode.
 *   24/06/16 1.9   AGC	Fix clang on Windows warnings.
 *   07/06/16 1.8   AGC	Avoid clang on Windows warnings from assimp headers.
 *   10/09/15 1.7   SP 	Add extra args to RenderGL().
 *   27/08/15 1.6   SP 	Add debug support.
 *   26/08/15 1.5   SP 	Add config options and parameters.
 *   20/06/14 1.4   AGC	Move C++ headers to source file.
 *   04/10/13 1.3   AGC	Simplify headers.
 *   19/07/13 1.2   SP 	Further development.
 *   15/07/13 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_VID_SIM_MODEL_H
#define _SPX_VID_SIM_MODEL_H

/* Other headers required. */
#include "SPxLibExternals/SPxAssimp.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxGL.h"
#include "SPxLibUtils/SPxVidSimMovable.h"
#include "SPxLibUtils/SPxVidSimModelRendererGL.h"

/*********************************************************************
 *
 *  Constants
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

/*********************************************************************
 *
 *   Class definitions
 *
 *********************************************************************/

/* Forward declare any classes required. */
class SPxVidSimScene;
class SPxVidSimModelDatabase;
class SPxVidSimDispGL;
class SPxTerrainDatabase;

class SPxVidSimModel : public SPxVidSimMovable
{
    /* Grant SPxVidSimModelDatabase private access. */
    friend class SPxVidSimModelDatabase;

public:

    /*
     * Public types.
     */

    /* Model types. */
    enum ModelType_t
    {
        MODEL_TYPE_STATIC,
        MODEL_TYPE_TARGET
    };

    /*
     * Public variables.
     */

    /* Model flags. */
    static const UINT32 MODEL_MAX_QUALITY = (1U << 0);

    /* Debug flags. */
    static const UINT32 DEBUG_RENDER = (1U << 0);

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxVidSimModel(SPxVidSimModelDatabase *database);
    virtual ~SPxVidSimModel(void);

    /* Configuration. */
    SPxErrorCode LoadFile(const char *filename);
    const char *GetFilename(void);
    const aiScene *GetAndLockAIModel(void);
    void UnlockAIModel(void);
    SPxErrorCode InitialiseGL(SPxVidSimDispGL *disp);
    SPxErrorCode RenderGL(SPxVidSimDispGL *disp,
                          double xMetres=0.0,
                          double yMetres=0.0,
                          double zMetres=0.0,
                          double pitchDegs=0.0,
                          double rollDegs=0.0,
                          double yawDegs=0.0);
    SPxErrorCode SetModelType(ModelType_t type);
    ModelType_t GetModelType(void) { return m_modelType; }
    SPxErrorCode SetModelName(const char *name);
    SPxErrorCode GetModelName(char *buf, unsigned int bufLen);
    SPxErrorCode SetZRefMode(ZRefMode_t mode);
    ZRefMode_t GetZRefMode(void) { return m_zRefMode; }

    /* Scaling. */
    SPxErrorCode SetModelUnitsPerMetre(double muPerMetre);
    double GetModelUnitsPerMetre(void) { return m_modelUnitsPerMetre; }

    /* Positional correction in model units. */
    SPxErrorCode SetXCorrectionModelUnits(double modelUnits);
    double GetXCorrectionModelUnits(void);
    SPxErrorCode SetYCorrectionModelUnits(double modelUnits);
    double GetYCorrectionModelUnits(void);
    SPxErrorCode SetZCorrectionModelUnits(double modelUnits);
    double GetZCorrectionModelUnits(void);

    /* Selection criteria. */
    SPxErrorCode SetMinAltitudeMetres(double altMetres);
    double GetMinAltitudeMetres(void) { return m_minAltMetres; }
    SPxErrorCode SetMaxAltitudeMetres(double altMetres);
    double GetMaxAltitudeMetres(void) { return m_maxAltMetres; }

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private types.
     */

    /* Display information. */
    struct DispInfo_t
    {
        SPxVidSimDispGL *disp;
        SPxVidSimModelRendererGL *renderer;

    };

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    Assimp::Importer m_importer;             /* File m_importer. */
    const aiScene *m_aiModel;                /* Model read from file. */
    SPxCriticalSection m_mutex;              /* Model mutex. */
    ModelType_t m_modelType;                 /* Model type. */
    double m_modelUnitsPerMetre;             /* Model scale factor. */
    UINT32 m_modelFlags;                     /* Model flags. */
    ZRefMode_t m_zRefMode;                   /* Z reference mode. */

    /* Debugging. */
    UINT32 m_debugFlags;
    UINT32 m_lastRenderReportTime;
    UINT32 m_renderMsecs;
    UINT32 m_renderCount;

    /* Fixed positional correction. */
    double m_xCorModelUnits;
    double m_yCorModelUnits;
    double m_zCorModelUnits;

    /* Selection criteria. */
    double m_minAltMetres;
    double m_maxAltMetres;

    /*
     * Private functions.
     */

}; /* SPxVidSimModel */

#endif /* _SPX_VID_SIM_MODEL_H */

/*********************************************************************
 *
 *  End of file
 *
 *********************************************************************/
