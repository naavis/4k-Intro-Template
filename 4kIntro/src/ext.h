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


#define oglCreateShaderProgramv         ((PFNGLCREATESHADERPROGRAMVPROC)myglfunc[0])
#define oglGenProgramPipelines          ((PFNGLGENPROGRAMPIPELINESPROC)myglfunc[1])
#define oglBindProgramPipeline          ((PFNGLBINDPROGRAMPIPELINEPROC)myglfunc[2])
#define oglUseProgramStages             ((PFNGLUSEPROGRAMSTAGESPROC)myglfunc[3])
#define oglProgramUniform4fv            ((PFNGLPROGRAMUNIFORM4FVPROC)myglfunc[4])
#define oglGenFramebuffers              ((PFNGLGENFRAMEBUFFERSPROC)myglfunc[5])
#define oglBindFramebuffer              ((PFNGLBINDFRAMEBUFFERPROC)myglfunc[6])
#define oglTexStorage2D                 ((PFNGLTEXSTORAGE2DPROC)myglfunc[7])
#define oglDrawBuffers                  ((PFNGLDRAWBUFFERSPROC)myglfunc[8])
#define oglFramebufferTexture           ((PFNGLFRAMEBUFFERTEXTUREPROC)myglfunc[9])

#ifdef DEBUG
#define oglGetProgramiv          ((PFNGLGETPROGRAMIVPROC)myglfunc[10])
#define oglGetProgramInfoLog     ((PFNGLGETPROGRAMINFOLOGPROC)myglfunc[11])
#endif

// init
int EXT_Init( void );

#endif
