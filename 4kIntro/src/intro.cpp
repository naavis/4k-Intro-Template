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

static GLuint fragmentShader;
static GLuint framebuffer;
static GLuint texture;
static GLuint programPipeline;

int intro_init(void)
{
	if (!EXT_Init())
		return 0;

	GLuint vertexShader = oglCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vertex_shader_glsl);
	fragmentShader = oglCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fragment_shader_glsl);

	oglGenProgramPipelines(1, &programPipeline);
	oglBindProgramPipeline(programPipeline);
	oglUseProgramStages(programPipeline, GL_VERTEX_SHADER_BIT, vertexShader);
	oglUseProgramStages(programPipeline, GL_FRAGMENT_SHADER_BIT, fragmentShader);

#ifdef DEBUG
	int result;
	char info[1536];
	oglGetProgramiv(vertexShader, GL_LINK_STATUS, &result);
	oglGetProgramInfoLog(vertexShader, 1024, NULL, (char *)info);
	if (!result) {
		MessageBox(0, info, "Error!", MB_OK | MB_ICONEXCLAMATION);
	}
	oglGetProgramiv(fragmentShader, GL_LINK_STATUS, &result);
	oglGetProgramInfoLog(fragmentShader, 1024, NULL, (char *)info);
	if (!result) {
		MessageBox(0, info, "Error!", MB_OK | MB_ICONEXCLAMATION);
	}
#endif

	oglGenFramebuffers(1, &framebuffer);
	oglBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	oglTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, XRES, YRES);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	oglFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

	static const GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	oglDrawBuffers(1, drawBuffers);

	return 1;
}

static float fparams[4 * 4];

void intro_do(long time)
{
	oglBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	oglBindProgramPipeline(programPipeline);
	// Set fparams to give input to shaders

	// Render
	oglProgramUniform4fv(fragmentShader, 0, 4, fparams);
	glRects(-1, -1, 1, 1); // Deprecated. Still seems to work though.

	oglBindFramebuffer(GL_FRAMEBUFFER, 0);
}