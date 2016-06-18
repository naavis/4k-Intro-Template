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
#define NUMFUNCTIONS 7
#else
#define NUMFUNCTIONS 5
#endif

extern void *myglfunc[NUMFUNCTIONS];


#define oglCreateShaderProgramv         ((PFNGLCREATESHADERPROGRAMVPROC)myglfunc[0])
#define oglGenProgramPipelines          ((PFNGLGENPROGRAMPIPELINESPROC)myglfunc[1])
#define oglBindProgramPipeline          ((PFNGLBINDPROGRAMPIPELINEPROC)myglfunc[2])
#define oglUseProgramStages             ((PFNGLUSEPROGRAMSTAGESPROC)myglfunc[3])
#define oglProgramUniform4fv            ((PFNGLPROGRAMUNIFORM4FVPROC)myglfunc[4])

#ifdef DEBUG
#define oglGetProgramiv          ((PFNGLGETPROGRAMIVPROC)myglfunc[5])
#define oglGetProgramInfoLog     ((PFNGLGETPROGRAMINFOLOGPROC)myglfunc[6])
#endif

// init
int EXT_Init( void );

#endif
