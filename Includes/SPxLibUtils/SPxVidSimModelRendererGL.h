/*********************************************************************
 *
 * (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx Video Simulator SPxVidSimModelRenderGL class header.
 *
 * Revision Control:
 *   22/07/22 v1.16  SP 	Include SPxAssimp.h.
 *
 * Previous Changes:
 *   22/07/22 1.15  SP 	Remove unused assimp headers.
 *   22/07/22 1.14  SP 	Changes to simplify creation and deletion.
 *   04/12/17 1.13  AGC	Avoid new clang Windows warnings.
 *   24/06/16 1.12  AGC	Fix clang on Windows warnings.
 *   11/01/16 1.11  AGC	Cull correct faces are changes to view inversion.
 *   15/12/15 1.10  AGC	Cull correct faces based on view inversion.
 *   04/12/15 1.9   AGC	Improve shader log verification.
 *   03/11/15 1.8   SP 	Add fog rendering.
 *   14/10/15 1.7   SP 	Changes to RenderGL() args.
 *   06/10/15 1.6   SP 	Support brightness and colour mode.
 *   10/09/15 1.5   SP 	Various rendering optimisations.
 *                     	Add extra args to RenderGL().
 *   26/08/15 1.4   SP 	Add new variables to Mesh_t.
 *   10/07/15 1.3   SP 	Fix node transformation issue.
 *   20/06/14 1.2   AGC	Move C++ headers to source file.
 *   19/07/13 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_VID_SIM_MODEL_RENDERER_GL_H
#define _SPX_VID_SIM_MODEL_RENDERER_GL_H

/* Other headers required. */
#include "SPxLibExternals/SPxAssimp.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxGL.h"
#include "SPxLibUtils/SPxVidSimMovable.h"

/*********************************************************************
 *
 *  Constants
 *
 *********************************************************************/

/* Number of elements in OpenGL transform matrices. */
#define SPX_GL_MATRIX_ELEMENTS 16

/* Size of OpenGL transform matrix in bytes. */
#define SPX_GL_MATRIX_SIZE_BYTES (SPX_GL_MATRIX_ELEMENTS * sizeof(GLfloat))

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
class SPxVidSimModel;

class SPxVidSimModelRendererGL : public SPxVidSimObj
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
    SPxVidSimModelRendererGL(SPxVidSimModel *model);
    virtual ~SPxVidSimModelRendererGL(void);

    /* Configuration. */
    SPxErrorCode InitialiseGL(void);
    void RenderGL(double xMetres=0.0,
                  double yMetres=0.0,
                  double zMetres=0.0,
                  double pitchDegs=0.0,
                  double rollDegs=0.0,
                  double yawDegs=0.0,
                  ColourMode_t colourMode=COLOUR_MODE_COLOUR,
                  double brightness=1.0);

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

    /* Transform matrices. Take care with alignement as they
     * are mapped to std140 layout specification.
     */
    typedef struct
    {
        GLfloat modelview[SPX_GL_MATRIX_ELEMENTS];
        GLfloat projection[SPX_GL_MATRIX_ELEMENTS];

    } Matrices_t;

    /* Information to render each assimp node. */
    typedef struct
    {
        GLuint vao;
        GLuint texIndex;
        GLuint uniformBlockIndex;
        int numFaces;
        GLuint faceBuffer;
        GLuint vposBuffer;
        GLuint vnormBuffer;
        GLuint texCoordBuffer;

    } Mesh_t;

    /* Information to render materials. */
    typedef struct
    {
        GLfloat diffuse[4];
        GLfloat ambient[4];
        GLfloat specular[4];
        GLfloat emissive[4];
        GLfloat shininess;
        int texCount;

    } Material_t;

    /* Information to render lights. */
    typedef struct
    {
        GLfloat diffuseDir[4];
        GLfloat diffuseBright[4];
        GLfloat ambientBright[4];

    } Light_t;

    /* Information to render fog. */
    typedef struct
    {
        GLfloat fogColour[4];
        GLfloat fogDensity;
        int isFogEnabled;

    } Fog_t;

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* OpenGL stuff. */
    const aiScene *m_aiModel;       /* Model read from file. */
    Matrices_t m_matrices;          /* Our copy of the transform matrices. */
    GLuint m_matricesUniBuffer;     /* Uniform buffer for matrices. */
    GLuint m_lightUniBuffer;        /* Uniform buffer for lights. */
    GLuint m_fogUniBuffer;          /* Uniform buffer for fog. */
    GLuint m_shaderProgram;         /* Shader program. */
    GLuint m_vertexShader;          /* Vertex shader. */
    GLuint m_fragShader;            /* Fragment shader. */
    GLint m_texUnitLoc;             /* Texture unit uniform location. */
    GLint m_brightnessLoc;          /* Model brightness uniform location. */
    GLint m_colourModeLoc;          /* Model colour mode uniform location. */

    /*
     * Private functions.
     */

    SPxErrorCode InitBuffers(void);
    void CheckShaderInfoLog(GLuint obj);
    void CheckProgramInfoLog(GLuint obj, int statusToCheck);
    SPxErrorCode InitShaderProgram(void);
    void RecursiveRender(const aiNode* node);
    void PushModelViewMatrix(void);
    void PopModelViewMatrix(void);
    void SetModelViewMatrix(void);
    SPxErrorCode LoadTextures(void);
    void SetGLfloat4(GLfloat f[4], GLfloat a, GLfloat b, GLfloat c, GLfloat d);
    void Colour4ToGLfloat4(const aiColor4D *c, GLfloat f[4]);

}; /* SPxVidSimModelRendererGL */

#endif /* _SPX_VID_SIM_MODEL_RENDERER_GL_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/
