/*********************************************************************
*
* (c) Copyright 2011, Cambridge Pixel Ltd.
*
* File:  $RCSfile: SPxGlxFunctions.h,v $
* ID: $Id: SPxGlxFunctions.h,v 1.4 2011/02/23 10:56:03 rew Exp $
*
* Purpose:
*   Header for class to provide OpenGL function pointers not always
*   defined in OpenGL link-libraries.
*   Simply include this class as an member object (not pointer) in 
*   a class to make the GL/GLX functions available.
*
* Revision Control:
*   23/02/11 v1.4    AGC	Add glBlendEquation().
*
* Previous Changes:
*   11/02/11 1.3    AGC	Add glXWaitVideoSync().
*   04/02/11 1.2    AGC	Add glBlendFuncSeparate().
*			Move functions into SPxGlx namespace.
*   28/01/11 1.1    AGC	Initial Version
*
**********************************************************************/

#ifndef _SPX_GLX_FUNCTIONS_H
#define	_SPX_GLX_FUNCTIONS_H

/* Standard GL/GLX headers. */
#include "GL/gl.h"
#include "GL/glx.h"
#include "GL/glxext.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* GL/GLX function pointers. */
namespace SPxGlx
{
    extern void (*glGenFramebuffers)(GLsizei, GLuint*);
    extern void (*glGenRenderbuffers)(GLsizei, GLuint*);
    extern void (*glBindFramebuffer)(GLenum, GLuint);
    extern void (*glBindRenderbuffer)(GLenum, GLuint);
    extern GLenum (*glCheckFramebufferStatus)(GLenum);
    extern void (*glDeleteFramebuffers)(GLsizei, const GLuint*);
    extern void (*glDeleteRenderbuffers)(GLsizei, const GLuint*);
    extern void (*glFramebufferTexture2D)(GLenum, GLenum, GLenum, GLuint, GLint);
    extern void (*glRenderbufferStorage)(GLenum, GLenum, GLsizei, GLsizei);
    extern void (*glFramebufferRenderbuffer)(GLenum, GLenum, GLenum, GLuint);
    extern void (*glGenBuffers)(GLsizei, GLuint*);
    extern void (*glDeleteBuffers)(GLsizei, const GLuint*);
    extern void (*glBindBuffer)(GLenum, GLuint);
    extern void (*glBufferData)(GLenum, GLsizeiptr, const GLvoid*, GLenum);
    extern void (*glBufferSubData)(GLenum, GLintptr, GLsizeiptr, const GLvoid*); 
    extern GLvoid* (*glMapBuffer)(GLenum, GLenum);
    extern GLboolean (*glUnmapBuffer)(GLenum);
    extern void (*glXBindTexImage)(Display*, GLXDrawable, int, const int *);
    extern void (*glXReleaseTexImage)(Display*, GLXDrawable, int);
    extern void (*glBlendEquation)(GLenum);
    extern void (*glBlendFuncSeparate)(GLenum, GLenum, GLenum, GLenum);
    extern int (*glXSwapInterval)(int);
    extern int (*glXWaitVideoSync)(int, int, unsigned int*);

}

/* GL_NVX_gpu_memory_info constants - check for extension before use. */
#define GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX          0x9047
#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX    0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX  0x9049
#define GPU_MEMORY_INFO_EVICTION_COUNT_NVX            0x904A
#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX            0x904B

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

/*
 * SPxGlxFunctions class.
 */
class SPxGlxFunctions
{
public:
    /* Public functions. */

    /* Construction and destruction. */
    SPxGlxFunctions(void);
    virtual ~SPxGlxFunctions(void);

    int IsInitialised(void) { return m_initialised; }

private:
    static int m_initialised;	/* Has the class been successfully initialised? */

}; /* SPxGlxFunctions class */

#endif /* _SPX_GLX_FUNCTIONS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/
