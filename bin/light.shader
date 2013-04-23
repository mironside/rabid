<GLSL>
[VertexShader]

// light info
varying vec3  lightPos;
varying vec3  lightDir;
varying vec4  lightColor;
varying float lightIntensity;

varying vec3 pos;
varying vec3 N;

void main(void)
{
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  N = gl_Normal;
  lightPos = gl_LightSource[0].position.xyz;
  lightDir = gl_LightSource[0].spotDirection.xyz;
  lightIntensity = gl_LightSource[0].constantAttenuation;
  pos = gl_Vertex.xyz;
  gl_FrontColor = gl_Color;
  gl_BackColor = gl_Color;
}



[FragmentShader]
varying vec3 pos;
varying vec3 N;
varying vec3 lightPos;
varying vec3 lightDir;
varying vec4 lightColor;
varying float lightIntensity;

void main(void)
{
  vec3 L = normalize(lightPos - pos);
  float atten = sqrt(length(lightPos - pos)) * 3.14159;

  float ang1 = dot(L, N);
  ang1 = max(ang1, 0.0);

  float ang2 = dot(-L, lightDir);
  ang2 = max(ang2, 0.0);

  float I = clamp(ang1 * ang2 * lightIntensity / atten, 0.0, 1.0);
  vec3 color = clamp(I*gl_Color.rgb, 0.0, 1.0);
  gl_FragColor = vec4(color.rgb, 1.0);
}

