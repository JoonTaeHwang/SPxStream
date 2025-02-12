/*********************************************************************
*
* (c) Copyright 2013 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for OpenGL texture destination class.
*
* Revision Control:
*   27/06/18 v1.5   SP 	Support render without binding FBO.
*
* Previous Changes:
*   11/05/16 1.4    AGC	Add rotation support.
*   04/10/13 1.3    AGC	Simplify headers.
*   03/05/13 1.2    SP 	Change some variable names.
*   25/04/13 1.1    SP 	Initial version.
*
**********************************************************************/

#ifndef _SPX_SC_DEST_TEXTURE_H
#define _SPX_SC_DEST_TEXTURE_H

/*
 * Other headers required.
 */

/* For SPxGL... types. */
#include "SPxLibUtils/SPxGL.h"

/* Base class header. */
#include "SPxLibSc/SPxScDestBitmap.h"

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

/* SPxScDestTexture class definition. */
class SPxScDestTexture : public SPxScDestBitmap
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */

    /* Constructor, destructor etc. */
    SPxScDestTexture(void);
    virtual ~SPxScDestTexture();
    int Create(UINT16 maxScreenW, 
               UINT16 maxScreenH,
               GLint texInternalFormat=GL_RGBA8, 
               GLint texFormat=GL_BGRA, 
               GLint texType=GL_UNSIGNED_INT_8_8_8_8_REV); 

    /* Get the texture ID. */
    GLint GetTexture(void) { return m_texture ? m_texture->GetID() : 0; }

    /* Render to texture. */
    SPxErrorCode UpdateTexture(int bindFBO=TRUE);

    /* Get required display matrix (for heading-up true trails). */
    virtual void GetDisplayMatrix(float (&matrix)[9]);

    /* Get  size of image currently in the texture (in pixels). */
    unsigned int GetImageWidth(void) { return m_imageWidth; }
    unsigned int GetImageHeight(void) { return m_imageHeight; }
        
protected:

    /*
     * Protected fields.
     */

    /*
     * Protected functions.
     */
    
private:

    /*
     * Private types.
     */

    typedef struct
    {
        SPxGLBuffer *pbo;   /* The pixel buffer object. */
        int imageWidth;     /* Width of image in the PBO. */
        int imageHeight;    /* Height of image in the PBO. */

    } PBOInfo_t;

    /*
     * Private fields.
     */

    /* OpenGL objects. */
    int m_createdOK;         /* Object successfully created? */
    SPxGLFramebuffer *m_fbo; /* Offscreen frame buffer. */
    PBOInfo_t m_pboInfo[2];  /* Pixel buffer object info. */
    int m_currentPBO;        /* PBO currently in use. */
    SPxGLTex *m_texture;     /* Texture contained the radar video image. */

    /* Texture configuration. */
    GLint m_texInternalFormat;
    GLint m_texFormat;
    GLint m_texType;

    /* Size of image in the texture. */
    unsigned int m_imageWidth;
    unsigned int m_imageHeight;

    /*
     * Private functions.
     */

    void RenderBitmapToTexture(int bindFBO);

    /*
     * Private static functions.
     */

}; /* SPxScDestTexture class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_SC_DEST_TEXTURE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
