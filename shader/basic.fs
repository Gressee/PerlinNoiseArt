#version 330 core

// Get attribue data
layout(location = 0) out vec4 color;

// Get the uniform data
uniform vec2 screenDim;
uniform vec2 offset;
uniform vec2 baseOffsetRed;
uniform vec2 baseOffsetGreen;
uniform vec2 baseOffsetBlue;


// Noise Code from: https://github.com/keijiro/NoiseShader/blob/master/Assets/GLSL/ClassicNoise2D.glsl
vec4 mod289(vec4 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
  return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r) {
  return 1.79284291400159 - 0.85373472095314 * r;
}

vec2 fade(vec2 t) {
  return t*t*t*(t*(t*6.0-15.0)+10.0);
}

float cnoise(vec2 P) {
  vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
  vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
  Pi = mod289(Pi); // To avoid truncation effects in permutation
  vec4 ix = Pi.xzxz;
  vec4 iy = Pi.yyww;
  vec4 fx = Pf.xzxz;
  vec4 fy = Pf.yyww;

  vec4 i = permute(permute(ix) + iy);

  vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0 ;
  vec4 gy = abs(gx) - 0.5 ;
  vec4 tx = floor(gx + 0.5);
  gx = gx - tx;

  vec2 g00 = vec2(gx.x,gy.x);
  vec2 g10 = vec2(gx.y,gy.y);
  vec2 g01 = vec2(gx.z,gy.z);
  vec2 g11 = vec2(gx.w,gy.w);

  vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));
  g00 *= norm.x;  
  g01 *= norm.y;  
  g10 *= norm.z;  
  g11 *= norm.w;  

  float n00 = dot(g00, vec2(fx.x, fy.x));
  float n10 = dot(g10, vec2(fx.y, fy.y));
  float n01 = dot(g01, vec2(fx.z, fy.z));
  float n11 = dot(g11, vec2(fx.w, fy.w));

  vec2 fade_xy = fade(Pf.xy);
  vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
  float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
  return 2.3 * n_xy;
}


// Produces the detailed noise
float fractalNoise(vec2 point) {
    float noiseSum = 0;
    float amplitude = 1;
    float frequency = 2;
    float amplitudeSum = 0; 

    for (int i = 0; i < 5; i++) {
        noiseSum += cnoise(point * frequency) * amplitude;

        amplitudeSum += amplitude;

        frequency *= 2;
        amplitude *= 0.5;
    }

    //noiseSum = 1 - abs(noiseSum);

    // Get the range to 0-1 from    amplitudeSum - amplitudeSum
    noiseSum = noiseSum/amplitudeSum * 0.5f + 0.5f;

    return noiseSum;
}


// Produces warped noise from a point
float warpPoint(vec2 samplePoint, vec2 baseOffset, vec2 offset, int warpNum) {
  
  // Init warp vars
  float warpX = 0;
  float warpY = 0;
  vec2 warpedPoint = samplePoint;

  for (int i = 0; i < warpNum; i++) {
    warpX = fractalNoise(warpedPoint + offset[0] + baseOffset[0]);
    warpY = fractalNoise(warpedPoint + offset[1] + baseOffset[1]);
    warpedPoint = vec2(warpX, warpY);
  }

  float noise = fractalNoise(warpedPoint + offset + baseOffset[1]);

  return noise;
}

vec4 mapColor(float val) {
  // Maps a value in range (0, 1) to a color

  float red = 2.0f*(val-0.5f);
  float green = 0.5f*val;
  float blue = abs(val-0.5f);
  vec4 color = vec4(red, green, blue, 1.0f);
  return color;
}

void main() {
    
  // Get the coords of the fragment in (-1; 1) range
  vec2 viewportCoord = vec2(((gl_FragCoord.x / screenDim[0]) - 0.5f) * 2 , ((gl_FragCoord.y / screenDim[1]) - 0.5f) * 2);
  
  float val = warpPoint(viewportCoord, vec2(0.0f, 0.0f), offset, 1);
  color = mapColor(val);
  /*
  float r = warpPoint(viewportCoord, baseOffsetRed, offset, 1);
  float g = warpPoint(viewportCoord, baseOffsetGreen, offset, 1);
  float b = warpPoint(viewportCoord, baseOffsetBlue, offset, 1);
  
  
  r = 1-r;
  g = 1-g;
  b = 1-b;
  
  r = r * 0.5f;// + 0.25f;
  g = g * 0.5f;// + 0.25f;
  b = b * 0.5f;// + 0.25f;
  
  color = vec4(r, g, b, 1.0f);
  */
}