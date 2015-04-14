#ifdef OPENGL3

in vec4 vColor;
out vec4 color;

void main()
{
    color = vColor;
};

#else // #ifdef OPENGL3
#if IS_PLATFORM_IOS
precision mediump float;
#endif

varying vec4 vColor;

void main()
{
    gl_FragColor = vColor;
};
#endif // #else // #ifdef OPENGL3
