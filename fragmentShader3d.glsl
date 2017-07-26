#version 400
in vec3 outColor;
in vec3 textureCoords;
 
out vec4 frag_colour;

uniform sampler3D myTextureSampler;
uniform bool useTexture;

void main() {
  if(useTexture) 
  {
	float sampledScalar = texture( myTextureSampler, textureCoords)[0];
    float ww = 500.0;
	float wl = 1200.0;
	float w0 = wl - ww/2;
	float dI = 1.0/ww;
	sampledScalar = (sampledScalar-w0)*dI;
    frag_colour = vec4(sampledScalar, sampledScalar, sampledScalar, 1.0);
  }
  else
  {
     frag_colour = vec4(outColor, 1.0);
  }
 
}