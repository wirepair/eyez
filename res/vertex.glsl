#ifdef GL_ES
precision mediump float;
#endif

attribute vec4 u_position;
uniform float u_time;

void main() 
{
	gl_Position = u_position;
}