/*********************************************************************
*
* (c) Copyright 2013 - 2016, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxGL - Cross-platform classes and functions
*	for dealing with OpenGL.
*
* Revision Control:
*   06/10/21 v1.11   SP 	Fix last change.
*
* Previous Changes:
*   06/10/21 1.10   SP 	Change GLsizeiptr and GLintptr definitions to match Qt header.
*   07/06/21 1.9    AGC	Support extension queries using CoreProfile.
*   27/05/21 1.8    AGC	Avoid shadow of X11 global.
*   03/03/16 1.7    AGC	Remove strange empty line.
*   04/12/15 1.6    AGC	Add GL_VALIDATE_STATUS.
*   26/08/15 1.5    AGC	Support multisampling.
*   01/07/14 1.4    AGC	Rename gl namespace to SPxGL.
*   19/07/13 1.3    SP 	Add vertex array functions.
*   26/04/13 1.2    SP 	Add functions to check OpenGL version, etc.
*   25/04/13 1.1    SP 	Initial version copied from SPxAVGL.h.
*                       Additional OpenGL functions added.
*
**********************************************************************/
#ifndef SPX_GL_H
#define SPX_GL_H

/*
 * Includes
 */

/* SPx library includes. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxError.h"

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#include <GL/gl.h>
#pragma comment(lib, "OpenGL32")
#else
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glext.h>
#endif

/*
 * Constants
 */
#ifndef GL_VERSION_1_2
#define GL_CLAMP_TO_EDGE                0x812F
#define GL_MULTISAMPLE                  0x809D
#define GL_BGR                          0x80E0
#define GL_BGRA                         0x80E1
#define GL_UNSIGNED_INT_8_8_8_8_REV	0x8367

#ifndef GL_ARB_imaging
#define GL_FUNC_ADD		0x8006
#endif

#ifndef GL_ARB_uniform_buffer_object
#define GL_UNIFORM_BUFFER 0x8A11
#endif

#endif
#ifndef GL_VERSION_1_3
#define GL_TEXTURE0		0x84C0
#endif
#ifndef GL_VERSION_1_4
#define GL_DEPTH_COMPONENT32	0x81A7
#endif
#ifndef GL_VERSION_1_5
#ifdef _WIN64
typedef signed long long int GLintptr;
typedef signed long long int GLsizeiptr;
#else
typedef signed long int GLintptr;
typedef signed long int GLsizeiptr;
#endif
#define GL_ARRAY_BUFFER		0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_READ_ONLY		0x88B8
#define GL_WRITE_ONLY		0x88B9
#define GL_READ_WRITE		0x88BA
#define GL_BUFFER_ACCESS	0x88BB
#define GL_BUFFER_MAPPED	0x88BC
#define GL_BUFFER_MAP_POINTER	0x88BD
#define GL_STREAM_DRAW		0x88E0
#define GL_STREAM_READ		0x88E1
#define GL_STREAM_COPY		0x88E2
#define GL_STATIC_DRAW		0x88E4
#define GL_STATIC_READ		0x88E5
#define GL_STATIC_COPY		0x88E6
#define GL_DYNAMIC_DRAW         0x88E8
#endif
#ifndef GL_VERSION_2_0
typedef char GLchar;
#define GL_VERTEX_SHADER	0x8B31
#define GL_FRAGMENT_SHADER	0x8B30
#define GL_COMPILE_STATUS	0x8B81
#define GL_LINK_STATUS		0x8B82
#define GL_VALIDATE_STATUS	0x8B83
#define GL_INFO_LOG_LENGTH      0x8B84
#endif
#ifndef GL_VERSION_2_1
#define GL_PIXEL_UNPACK_BUFFER	0x88EC
#endif
#ifndef GL_VERSION_3_2
#define GL_TEXTURE_2D_MULTISAMPLE 0x9100
#endif
#ifndef GL_ARB_framebuffer_object
#define GL_COLOR_ATTACHMENT0            0x8CE0
#define GL_DEPTH_ATTACHMENT             0x8D00
#define GL_FRAMEBUFFER                  0x8D40
#define GL_FRAMEBUFFER_EXT              0x8D40
#define GL_FRAMEBUFFER_COMPLETE_EXT     0x8CD5
#define GL_FRAMEBUFFER_UNSUPPORTED_EXT  0x8CDD
#define GL_RENDERBUFFER                 0x8D41
#define GL_UNIFORM_BUFFER_EXT           0x8DEE
#endif
#ifndef GL_EXT_framebuffer_blit
#define GL_READ_FRAMEBUFFER_EXT		0x8CA8
#define GL_DRAW_FRAMEBUFFER_EXT		0x8CA9
#endif

#define GL_CONTEXT_MAJOR_VERSION_ARB		    0x2091
#define GL_CONTEXT_MINOR_VERSION_ARB		    0x2092
#define GL_CONTEXT_LAYER_PLANE_ARB		    0x2093
#define GL_CONTEXT_FLAGS_ARB			    0x2094
#define GL_CONTEXT_PROFILE_MASK_ARB		    0x9126
#define GL_CONTEXT_DEBUG_BIT_ARB		    0x0001
#define GL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB	    0x0002
#define GL_CONTEXT_CORE_PROFILE_BIT_ARB		    0x00000001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB    0x00000002

/*
 * Types
 */

/* Forward declarations. */
struct SPxGLCtxPriv;
struct SPxGLShaderPriv;
struct SPxGLFramebufferPriv;
struct SPxGLTexPriv;
struct SPxGLBufferPriv;
struct SPxGLVertexAttribPriv;
class SPxGLTex;

/* Class for encapsulating a cross-platform Window. */
class SPxGLWin
{
public:
#ifdef WIN32
    explicit SPxGLWin(HWND hwnd=NULL);
    HWND hwnd;
#else
    explicit SPxGLWin(Display *display=NULL, int screen=0, Window window=0);
    Display *display;
    int screen;
    Window window;
#endif
    ~SPxGLWin(void);
    SPxErrorCode Create(void);
    void Lock(void);
    void Unlock(void);
    int Exists(void);
    int createdWindow;
};

/* Class for auto locking/unlocking a GL window. */
class SPxAutoLockWin
{
public:
    SPxAutoLockWin(SPxGLWin &win)
    : m_win(win)
    {
	m_win.Lock();
    }
    ~SPxAutoLockWin(void)
    {
	m_win.Unlock();
    }
private:
    SPxGLWin m_win;
};

/* Class for encapsulating an OpenGL context. */
class SPxGLCtx
{
public:
    SPxGLCtx(void);
    ~SPxGLCtx(void);
    SPxErrorCode SetWindow(const SPxGLWin& window, int enableMultisampling=TRUE);
    void Bind(void);
    void Unbind(void);
    int IsValid(void);
    int WinExists(void);
    int CheckExtension(const char *name);
#ifdef WIN32
    HGLRC GetNative(void) const;
    HDC GetNativeDC(void) const;
#else
    GLXContext GetNative(void) const;
    Display *GetNativeDisplay(void) const;
#endif
private:
    SPxAutoPtr<SPxGLCtxPriv> m_p;
};

/* Class for encapsulating an OpenGL shader program. */
class SPxGLShader
{
public:
    explicit SPxGLShader(const char *vertexSource=NULL, 
	        const char *fragSource=NULL, 
	        SPxErrorCode *errRtn=NULL);
    ~SPxGLShader(void);
    void Bind(void);
    void Unbind(void);
    GLuint GetAttribLocation(const char *attribName);
    GLint GetUniformLocation(const char *uniformName);
private:
    SPxAutoPtr<SPxGLShaderPriv> m_p;
};

/* Class for encapsulating an OpenGL off-screen buffer. */
class SPxGLFramebuffer
{
public:
    explicit SPxGLFramebuffer(int multisample=FALSE);
    ~SPxGLFramebuffer(void);
    SPxErrorCode SetSize(UINT16 width, UINT16 height);
    void Bind(void);
    void Unbind(void);
    void BindRead(void);
    void BindWrite(void);
    SPxGLTex* GetTexture(void);
private:
    SPxAutoPtr<SPxGLFramebufferPriv> m_p;
};

/* Class for encapsulating OpenGL texture. */
class SPxGLTex
{
public:
    explicit SPxGLTex(int target,
		      int minFilter=GL_LINEAR_MIPMAP_LINEAR,
		      int magFilter=GL_LINEAR);
    ~SPxGLTex(void);
    void Config2D(int internalFormat, 
		int width, int height, 
		int border, int format, 
		unsigned int type,
		void *pixels=NULL);
    void Config2DMultisample(int internalFormat, 
		int width, int height);
    void Bind(void);
    void Unbind(void);
    GLuint GetID(void);
private:
    SPxAutoPtr<SPxGLTexPriv> m_p;
};

/* Class for encapsulating OpenGL buffer. */
class SPxGLBuffer
{
public:
    SPxGLBuffer(int target);
    ~SPxGLBuffer(void);
    void SetData(unsigned int size, 
		 const void *data, 
		 int usage);
    void SetSubData(unsigned int offset,
                    unsigned int size, 
                    const void *data);
    void CopyTo(unsigned int size,
	        const void *data,
	        int usage);
    void Bind(void);
    void Unbind(void);
    GLuint GetID(void);
private:
    SPxAutoPtr<SPxGLBufferPriv> m_p;
};

class SPxGLVertexAttrib
{
public:
    SPxGLVertexAttrib(SPxGLShader *shader,
		      const char *name,
		      int size,
		      unsigned int type,
		      unsigned char normalized,
		      unsigned int stride,
		      const void *vertexPointer=NULL);
    ~SPxGLVertexAttrib(void);
private:
    SPxAutoPtr<SPxGLVertexAttribPriv> m_p;
};

/* Class for pushing/popping attributes. */
class SPxGLAttrib
{
public:
    explicit SPxGLAttrib(unsigned int attribs)
    {
	glPushAttrib(attribs);
    }
    ~SPxGLAttrib(void)
    {
	glPopAttrib();
    }
};

/* Class for autobinding/unbinding framebuffers. */
template<typename T>
class SPxGLBind
{
public:
    explicit SPxGLBind(T *gl)
	: m_gl(gl)
    {
	if( m_gl )
	{
	    m_gl->Bind();
	}
    }
    ~SPxGLBind(void)
    {
	if( m_gl )
	{
	    m_gl->Unbind();
	}
    }
private:
    T *m_gl;
};

/*
 * Functions
 */

/* Cross platform functions. */
void glSwapBuffers(const SPxGLWin& window);
int glXCheckVersion(int major, int minor);
void glEnableErrorChecks(int enable=TRUE);
void glCheckError(const char *string=NULL);

/* OpenGL V1.0/1.1 function pointer types. */
typedef void (APIENTRY * PFNGLENABLE)(GLenum);
typedef void (APIENTRY * PFNGLMATRIXMODE)(GLenum);
typedef void (APIENTRY * PFNGLCLEAR)(GLbitfield);
typedef void (APIENTRY * PFNGLPUSHMATRIX)(void);
typedef void (APIENTRY * PFNGLPOPMATRIX)(void);
typedef void (APIENTRY * PFNGLLOADIDENTITY)(void);
typedef void (APIENTRY * PFNGLORTHO)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);
typedef void (APIENTRY * PFNGLVIEWPORT)(GLint, GLint, GLsizei, GLsizei);

/* Post OpenGL V1.1 function pointer types. */
typedef void (APIENTRY * PFNGLBLENDEQUATION)(GLenum);
typedef void (APIENTRY * PFNGLACTIVETEXTURE)(GLenum);
typedef void (APIENTRY * PFNGLBINDBUFFER)(GLenum, GLuint);
typedef void (APIENTRY * PFNGLBINDBUFFERRANGE)(GLenum, GLuint, GLuint, GLintptr, GLsizeiptr);
typedef void (APIENTRY * PFNGLDELETEBUFFERS)(GLsizei, const GLuint*);
typedef void (APIENTRY * PFNGLGENBUFFERS)(GLsizei, GLuint *);
typedef void (APIENTRY * PFNGLBUFFERDATA)(GLenum, GLsizeiptr, const GLvoid*, GLenum);
typedef void (APIENTRY * PFNGLBUFFERSUBDATA)(GLenum, GLintptr, GLsizeiptr, const GLvoid*);
typedef GLvoid* (APIENTRY *PFNGLMAPBUFFER)(GLenum, GLenum);
typedef GLvoid* (APIENTRY *PFNGLMAPBUFFERRANGE)(GLenum, GLintptr, 
                                                GLsizeiptr, GLbitfield);
typedef GLboolean (APIENTRY *PFNGLUNMAPBUFFER)(GLenum);
typedef GLuint (APIENTRY * PFNGLCREATEPROGRAM)(void);
typedef GLuint (APIENTRY * PFNGLCREATESHADER)(GLenum);
typedef void (APIENTRY * PFNGLSHADERSOURCE)(GLuint, GLsizei, const GLchar**, const GLint*);
typedef void (APIENTRY * PFNGLCOMPILESHADER)(GLuint);
typedef void (APIENTRY * PFNGLGETSHADERIV)(GLuint, GLenum, GLint*);
typedef void (APIENTRY * PFNGLGETSHADERINFOLOG)(GLuint, GLsizei, GLsizei*, GLchar*);
typedef void (APIENTRY * PFNGLATTACHSHADER)(GLuint, GLuint);
typedef void (APIENTRY * PFNGLLINKPROGRAM)(GLuint);
typedef void (APIENTRY * PFNGLVALIDATEPROGRAM)(GLuint);
typedef void (APIENTRY * PFNGLGETPROGRAMIV)(GLuint, GLenum, GLint*);
typedef void (APIENTRY * PFNGLUSEPROGRAM)(GLuint);
typedef void (APIENTRY * PFNGLDETACHSHADER)(GLuint, GLuint);
typedef void (APIENTRY * PFNGLDELETESHADER)(GLuint);
typedef void (APIENTRY * PFNGLDELETEPROGRAM)(GLuint);
typedef void (APIENTRY * PFNGLGETPROGRAMINFOLOG)(GLuint, GLsizei, GLsizei*, GLchar*);
typedef void (APIENTRY * PFNGLVERTEXATTRIBPOINTER)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);
typedef void (APIENTRY * PFNGLUNIFORM1I)(GLint, GLint);
typedef void (APIENTRY * PFNGLUNIFORM1F)(GLint, GLfloat);
typedef GLint (APIENTRY * PFNGLGETATTRIBLOCATION)(GLuint, const GLchar*);
typedef GLint (APIENTRY * PFNGLGETUNIFORMLOCATION)(GLuint, const GLchar*);
typedef GLuint  (APIENTRY * PFNGLGETUNIFORMBLOCKINDEX)(GLuint, const GLchar*);
typedef void (APIENTRY * PFNGLUNIFORMBLOCKBINDING)(GLuint,  GLuint,  GLuint);
typedef void (APIENTRY * PFNGLENABLEVERTEXATTRIBARRAY)(GLuint);
typedef void (APIENTRY * PFNGLDISABLEVERTEXATTRIBARRAY)(GLuint);
typedef void (APIENTRY * PFNGLVERTEXATTRIB2F)(GLuint, GLfloat, GLfloat);
typedef void (APIENTRY * PFNGLVERTEXATTRIB4FV)(GLuint, const GLfloat*);
typedef void (APIENTRY * PFNGLGENVERTEXARRAYS)(GLsizei, GLuint*);
typedef void (APIENTRY * PFNGLBINDVERTEXARRAY)(GLuint);
typedef void (APIENTRY * PFNGLDELETEVERTEXARRAYS)(GLsizei, const GLuint*);
typedef void (APIENTRY * PFNGLBINDRENDERBUFFER)(GLenum, GLuint);
typedef void (APIENTRY * PFNGLDELETERENDERBUFFERS)(GLenum, const GLuint*);
typedef void (APIENTRY * PFNGLGENRENDERBUFFERS)(GLsizei, GLuint*);
typedef void (APIENTRY * PFNGLRENDERBUFFERSTORAGE)(GLenum, GLenum, GLsizei, GLsizei);
typedef void (APIENTRY * PFNGLBINDFRAMEBUFFER)(GLenum, GLuint);
typedef void (APIENTRY * PFNGLDELETEFRAMEBUFFERS)(GLenum, const GLuint*);
typedef void (APIENTRY * PFNGLGENFRAMEBUFFERS)(GLsizei, GLuint*);
typedef void (APIENTRY * PFNGLFRAMEBUFFERTEXTURE2D)(GLenum, GLenum, GLenum, GLuint, GLint);
typedef void (APIENTRY * PFNGLFRAMEBUFFERRENDERBUFFER)(GLenum, GLenum, GLenum, GLuint);
typedef GLenum (APIENTRY * PFNGLCHECKFRAMEBUFFERSTATUS)(GLenum);
typedef void (APIENTRY * PFNGLGENERATEMIPMAP)(GLenum);
typedef void (APIENTRY * PFNGLPUSHCLIENTATTRIB)(GLbitfield);
typedef void (APIENTRY * PFNGLPOPCLIENTATTRIB)(void);
typedef void (APIENTRY * PFNGLBINDATTRIBLOCATION)(GLuint,  GLuint,  const GLchar*);
typedef void (APIENTRY * PFNGLBINDFRAGDATALOCATION)(GLuint, GLuint,  const char*);
typedef void (APIENTRY * PFNGLTEXIMAGE2DMULTISAMPLE)(GLenum, GLsizei, GLint, GLsizei, GLsizei, GLboolean);
typedef void (APIENTRY * PFNGLRENDERBUFFERSTORAGEMULTISAMPLE)(GLenum, GLsizei, GLenum, GLsizei, GLsizei);
typedef void (APIENTRY * PFNGLBLITFRAMEBUFFEREXT)(GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum);
typedef void (APIENTRY *PFNGLGETINTEGERV)(GLenum, GLint *);
typedef const GLubyte * (APIENTRY *PFNGLGETSTRINGI)(GLenum, GLuint);

/* GL Functions. */
namespace SPxGL
{

/* Utility functions. */
extern void Initialise(void);
extern SPxErrorCode GetVersion(unsigned int *majorVerRtn, 
                               unsigned int *minorVerRtn);

extern SPxErrorCode CheckVersion(unsigned int reqMajorVer, 
                                 unsigned int reqMinorVer,
                                 int reportErr=FALSE,
                                 const char *errMsgPrefix=NULL);

extern SPxErrorCode CheckExtension(const char *extensionName,
                                   int reportErr=FALSE,
                                   const char *errMsgPrefix=NULL);

/* OpenGL V1.0/1.1 functions. */
extern PFNGLENABLE Enable;
extern PFNGLMATRIXMODE MatrixMode;
extern PFNGLCLEAR Clear;
extern PFNGLPUSHMATRIX PushMatrix;
extern PFNGLPOPMATRIX PopMatrix;
extern PFNGLLOADIDENTITY LoadIdentity;
extern PFNGLORTHO Ortho;
extern PFNGLVIEWPORT Viewport;

/* Post OpenGL V1.1 functions. */
extern PFNGLBLENDEQUATION BlendEquation;
extern PFNGLACTIVETEXTURE ActiveTexture;
extern PFNGLBINDBUFFER BindBuffer;
extern PFNGLBINDBUFFERRANGE BindBufferRange;
extern PFNGLDELETEBUFFERS DeleteBuffers;
extern PFNGLGENBUFFERS GenBuffers;
extern PFNGLBUFFERDATA BufferData;
extern PFNGLBUFFERSUBDATA BufferSubData;
extern PFNGLMAPBUFFER MapBuffer;
extern PFNGLMAPBUFFERRANGE MapBufferRange;
extern PFNGLUNMAPBUFFER UnmapBuffer;
extern PFNGLCREATEPROGRAM CreateProgram;
extern PFNGLCREATESHADER CreateShader;
extern PFNGLSHADERSOURCE ShaderSource;
extern PFNGLCOMPILESHADER CompileShader;
extern PFNGLGETSHADERIV GetShaderiv;
extern PFNGLGETSHADERINFOLOG GetShaderInfoLog;
extern PFNGLATTACHSHADER AttachShader;
extern PFNGLLINKPROGRAM LinkProgram;
extern PFNGLVALIDATEPROGRAM ValidateProgram;
extern PFNGLGETPROGRAMIV GetProgramiv;
extern PFNGLUSEPROGRAM UseProgram;
extern PFNGLDETACHSHADER DetachShader;
extern PFNGLDELETESHADER DeleteShader;
extern PFNGLDELETEPROGRAM DeleteProgram;
extern PFNGLGETPROGRAMINFOLOG GetProgramInfoLog;
extern PFNGLVERTEXATTRIBPOINTER VertexAttribPointer;
extern PFNGLUNIFORM1I Uniform1i;
extern PFNGLUNIFORM1F Uniform1f;
extern PFNGLGETATTRIBLOCATION GetAttribLocation;
extern PFNGLGETUNIFORMLOCATION GetUniformLocation;
extern PFNGLGETUNIFORMBLOCKINDEX GetUniformBlockIndex;
extern PFNGLUNIFORMBLOCKBINDING UniformBlockBinding;
extern PFNGLENABLEVERTEXATTRIBARRAY EnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAY DisableVertexAttribArray;
extern PFNGLVERTEXATTRIB2F VertexAttrib2f;
extern PFNGLVERTEXATTRIB4FV VertexAttrib4fv;
extern PFNGLGENVERTEXARRAYS GenVertexArrays;
extern PFNGLBINDVERTEXARRAY BindVertexArray;
extern PFNGLDELETEVERTEXARRAYS DeleteVertexArrays;
extern PFNGLBINDRENDERBUFFER BindRenderbuffer;
extern PFNGLDELETERENDERBUFFERS DeleteRenderbuffers;
extern PFNGLGENRENDERBUFFERS GenRenderbuffers;
extern PFNGLRENDERBUFFERSTORAGE RenderbufferStorage;
extern PFNGLBINDFRAMEBUFFER BindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERS DeleteFramebuffers;
extern PFNGLGENFRAMEBUFFERS GenFramebuffers;
extern PFNGLFRAMEBUFFERTEXTURE2D FramebufferTexture2D;
extern PFNGLFRAMEBUFFERRENDERBUFFER FramebufferRenderbuffer;
extern PFNGLCHECKFRAMEBUFFERSTATUS CheckFramebufferStatus;
extern PFNGLGENERATEMIPMAP GenerateMipmap;
extern PFNGLPUSHCLIENTATTRIB PushClientAttrib;
extern PFNGLPOPCLIENTATTRIB PopClientAttrib;
extern PFNGLBINDATTRIBLOCATION BindAttribLocation;
extern PFNGLBINDFRAGDATALOCATION BindFragDataLocation;
extern PFNGLTEXIMAGE2DMULTISAMPLE TexImage2DMultisample;
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLE RenderbufferStorageMultisample;
extern PFNGLBLITFRAMEBUFFEREXT BlitFramebufferEXT;
extern PFNGLGETINTEGERV GetIntegerv;
extern PFNGLGETSTRINGI GetStringi;
}

#endif /* SPX_GL_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/
