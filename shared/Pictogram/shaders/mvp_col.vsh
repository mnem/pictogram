#ifdef GL_ES
#define LOW_PRECISION lowp
#else
#define LOW_PRECISION
#endif

attribute vec4 position;
attribute vec4 color;

//uniform mat4 modelViewProjectionMatrix;
//uniform mat3 normalMatrix;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

varying LOW_PRECISION vec4 colorVarying;

void main()
{
//	gl_Position = modelViewProjectionMatrix * position;
	gl_Position = projectionMatrix * modelViewMatrix * position;
	colorVarying = color;
}
