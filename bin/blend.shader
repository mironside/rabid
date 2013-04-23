<GLSL>
[VertexShader]
varying float frac;
void main()
{
  gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
  gl_TexCoord[0] = gl_MultiTexCoord0;
  frac = gl_LightSource[0].linearAttenuation;
}


[FragmentShader]
uniform sampler2D Tex0;
uniform sampler2D Tex1;
varying float frac;
void main()
{
  vec4 l = texture2D(Tex0, gl_TexCoord[0].st);
  vec4 a = texture2D(Tex1, gl_TexCoord[0].st);

  // unpack
  float A;
  A = a[3];
  A = A / 255.0;
  A = A + a[2];
  A = A / 255.0;
  A = A + a[1];
  A = A / 255.0;
  A = A + a[0];

  float I = (l[0]) * frac;
  I = A + I;

  // pack
  vec4 f;
  f[0] = I - mod(I, 1.0 / 255.0);
  I = (I - f[0]) * 255.0;
  f[1] = I - mod(I, 1.0 / 255.0);
  I = (I - f[1]) * 255.0;
  f[2] = I - mod(I, 1.0 / 255.0);
  I = (I - f[2]) * 255.0;
  f[3] = I - mod(I, 1.0 / 255.0);

  gl_FragColor = clamp(f, 0.0, 1.0);
//  gl_FragColor = clamp(l+a, 0.0, 1.0);
}
