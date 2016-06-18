#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <GL/gl.h>
#include <math.h>
#include "config.h"
#include "ext.h"
#include "vertex_shader.inl"
#include "fragment_shader.inl"
#include "fp.h"

static int   fsid;

int intro_init( void )
{
    if( !EXT_Init() )
        return 0;

    int vsid = oglCreateShaderProgramv(GL_VERTEX_SHADER,   1, &vsh);
        fsid = oglCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fragment_shader_glsl);
 
    unsigned int pid;
    oglGenProgramPipelines(1, &pid);
    oglBindProgramPipeline(pid);
    oglUseProgramStages(pid, GL_VERTEX_SHADER_BIT, vsid);
    oglUseProgramStages(pid, GL_FRAGMENT_SHADER_BIT, fsid);

    #ifdef DEBUG
        int		result;
        char    info[1536];
        oglGetProgramiv(vsid, GL_LINK_STATUS, &result);
		oglGetProgramInfoLog(vsid, 1024, NULL, (char *)info);
		if (!result) {
			MessageBox(0, info, "Error!", MB_OK | MB_ICONEXCLAMATION);
		}
		oglGetProgramiv(fsid, GL_LINK_STATUS, &result);
		oglGetProgramInfoLog(fsid, 1024, NULL, (char *)info);
		if (!result) {
			MessageBox(0, info, "Error!", MB_OK | MB_ICONEXCLAMATION);
		}
    #endif

    return 1;
}

static float fparams[4*4];

void intro_do(long time)
{
	// Set fparams to give input to shaders

	// Render
    oglProgramUniform4fv(fsid, 0, 4, fparams);
    glRects(-1, -1, 1, 1); // Deprecated. Still seems to work though.
}