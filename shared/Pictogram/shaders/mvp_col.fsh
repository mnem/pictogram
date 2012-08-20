#ifdef GL_ES
#define LOW_PRECISION lowp
#else
#define LOW_PRECISION
#endif

varying LOW_PRECISION vec4 colorVarying;

void main()
{
	gl_FragColor = colorVarying;
}
