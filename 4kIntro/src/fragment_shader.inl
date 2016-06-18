/* File generated with Shader Minifier 1.1.4
 * http://www.ctrl-alt-test.fr
 */
#ifndef FRAGMENT_SHADER_INL_
# define FRAGMENT_SHADER_INL_

const char *fragment_shader_glsl =
 "#version 430\n"
 "layout(location=0)uniform vec4 fpar[4];"
 "layout(location=0)out vec4 color;"
 "in vec2 p;"
 "void main()"
 "{"
   "color=vec4(.5*p.x+.5,.5*p.y+.5,0.,1.);"
 "}";

#endif // FRAGMENT_SHADER_INL_
