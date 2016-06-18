static const char *vertex_shader_glsl = \
"#version 430\n"

"layout (location=0)in vec2 inVer;"
"out vec2 p;"
"out gl_PerVertex"
"{"
"vec4 gl_Position;"
"};"

"void main()"
"{"
"gl_Position=vec4(inVer,0.0,1.0);"
"p=inVer;"
"}";