/*********************************************************************
 *
 * (c) Copyright 2016 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx Video Simulator SPxVidSimModelRenderGL class header.
 *
 * Revision Control:
 *   22/07/22 v1.4   SP 	Changes to simplify creation and deletion.
 *
 * Previous Changes:
 *   03/06/21 1.3   AGC	Correct SPxRunProcess forward declaration.
 *   31/10/16 1.2   SP 	Support tiled map overlay.
 *   13/10/16 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_VID_SIM_TERRAIN_RENDERER_GL_H
#define _SPX_VID_SIM_TERRAIN_RENDERER_GL_H

/* Other headers required. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxGL.h"
#include "SPxLibUtils/SPxVidSimObj.h"

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
class SPxVidSimDispGL;
class SPxVidSimScene;
class SPxVidSimRadar;
class SPxTerrainDatabase;
class SPxScSourceLocal;
class SPxScDestTexture;
class SPxRunProcess;
class SPxViewControl;

class SPxVidSimTerrainRendererGL : public SPxVidSimObj
{
public:

    /*
     * Public types.
     */

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxVidSimTerrainRendererGL(SPxVidSimDispGL *disp);
    virtual ~SPxVidSimTerrainRendererGL(void);

    /* Configuration. */
    SPxErrorCode InitialiseGL(void);
    SPxErrorCode RenderGL(void);

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

    /* Event handling. */
    virtual void HandleEvent(const EventInfo_t *eventInfo);

private:

    /*
     * Private types.
     */

    /* Terrain point. */
    typedef struct 
    {
        GLfloat vertex[3];          /* Vertex X, Y and Z. */
        GLfloat normal[3];          /* Normal X, Y and Z. */
        GLfloat mapTexUV[2];    /* Terrain texture U and V coordinates. */
        GLfloat radarTexUV[2];      /* Radar texture U and V coordinates. */

    } TerrainPoint_t;

    /* Triangle strip point. Do not change order! */
    typedef struct 
    {
        GLfloat color[3];           /* Render colour R, G and B. */
        GLfloat normal[3];          /* Point normal X, Y and Z. */
        GLfloat vertex[3];          /* Point vertex X, Y and Z. */
        GLfloat mapTexUV[2];        /* Terrain texture U and V coordinates. */
        GLfloat radarTexUV[2];      /* Radar texture U and V coordinates. */
        unsigned int x;             /* Point X index. */
        unsigned int y;             /* Point Y index. */

    } TriStripPoint_t;

    /*
     * Private variables.
     */

    SPxGLBuffer *m_glBuffer;                /* Terrain data in video memory. */
    SPxGLTex *m_glTexture;                  /* Tiled map texture. */
    TriStripPoint_t *m_triStripPts;         /* Triangle strip points in host memory. */
    unsigned int m_maxNumTriStripPts;       /* Number of allocated points in m_triStripPts. */
    unsigned int m_numTriStripPts;          /* Number of points being used in m_triStripPts. */
    int m_terrainMeshChanged;               /* Flag set to regenerate terrain points. */
    int m_terrainMapChanged;                /* Flag to update terrain map. */
    SPxScDestTexture *m_radarDestTexture;   /* Radar dest texture. */
    SPxScSourceLocal *m_radarScanConv;      /* Scan connverter. */
    SPxRunProcess *m_radarScanConvPro;      /* Scan conversion run process. */
    ColourMode_t m_colourMode;              /* Last display colour mode. */
    SPxVidSimRadar *m_radar;                /* Radar to render. */

    /*
     * Private functions.
     */

    void GenerateTerrain(void);
    void UpdateTexture(void);
    SPxErrorCode RenderTerrain(void);
    SPxErrorCode RenderRadar(void);

    /*
     * Private static functions.
     */

}; /* SPxVidSimTerrainRendererGL */

#endif /* _SPX_VID_SIM_TERRAIN_RENDERER_GL_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
