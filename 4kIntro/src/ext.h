#ifndef _EXTENSIONS_H_
#define _EXTENSIONS_H_

#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#endif
#include <GL/gl.h>
#include "glext.h"


#ifdef DEBUG
#define NUMFUNCTIONS 12
#else
#define NUMFUNCTIONS 10
#endif

extern void *myglfunc[NUMFUNCTIONS];


#define glCreateShaderProgramv         ((PFNGLCREATESHADERPROGRAMVPROC)myglfunc[0])
#define glGenProgramPipelines          ((PFNGLGENPROGRAMPIPELINESPROC)myglfunc[1])
#define glBindProgramPipeline          ((PFNGLBINDPROGRAMPIPELINEPROC)myglfunc[2])
#define glUseProgramStages             ((PFNGLUSEPROGRAMSTAGESPROC)myglfunc[3])
#define glProgramUniform4fv            ((PFNGLPROGRAMUNIFORM4FVPROC)myglfunc[4])
#define glGenFramebuffers              ((PFNGLGENFRAMEBUFFERSPROC)myglfunc[5])
#define glBindFramebuffer              ((PFNGLBINDFRAMEBUFFERPROC)myglfunc[6])
#define glTexStorage2D                 ((PFNGLTEXSTORAGE2DPROC)myglfunc[7])
#define glDrawBuffers                  ((PFNGLDRAWBUFFERSPROC)myglfunc[8])
#define glFramebufferTexture           ((PFNGLFRAMEBUFFERTEXTUREPROC)myglfunc[9])

#ifdef DEBUG
#define glGetProgramiv          ((PFNGLGETPROGRAMIVPROC)myglfunc[10])
#define glGetProgramInfoLog     ((PFNGLGETPROGRAMINFOLOGPROC)myglfunc[11])
#endif

// init
int EXT_Init( void );

#endif
