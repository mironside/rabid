<GLSL>
[VertexShader]
void main()
{
  gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
  gl_TexCoord[0] = gl_MultiTexCoord0;
}


[FragmentShader]
// Converts from high precision to standard color
uniform sampler2D Tex0;
uniform sampler2D Tex1;
void main()
{
  vec4 a = texture2D(Tex0, gl_TexCoord[0].st);

  // only take one channel
  float A;
  A = a[3];
  A = A / 255.0;
  A = A + a[2];
  A = A / 255.0;
  A = A + a[1];
  A = A / 255.0;
  A = A + a[0];

  gl_FragColor = clamp(vec4(A), 0.0, 1.0);
//  gl_FragColor = clamp(a, 0.0, 1.0);
}

