#ifdef GL_ES
precision mediump float;
#endif

// Phong related variables
uniform sampler2D uSampler;
uniform vec3 uKd;
uniform vec3 uKs;
uniform vec3 uLightPos;
uniform vec3 uCameraPos;
uniform vec3 uLightIntensity;

varying highp vec2 vTextureCoord;
varying highp vec3 vFragPos;
varying highp vec3 vNormal;

// Shadow map related variables
#define NUM_SAMPLES 50
#define BLOCKER_SEARCH_NUM_SAMPLES NUM_SAMPLES
#define PCF_NUM_SAMPLES NUM_SAMPLES
#define NUM_RINGS 10

#define EPS 1e-3
#define PI 3.141592653589793
#define PI2 6.283185307179586

uniform sampler2D uShadowMap;

varying vec4 vPositionFromLight;

highp float rand_1to1(highp float x ) { 
  // -1 -1
  return fract(sin(x)*10000.0);
}

highp float rand_2to1(vec2 uv ) { 
  // 0 - 1
	const highp float a = 12.9898, b = 78.233, c = 43758.5453;
	highp float dt = dot( uv.xy, vec2( a,b ) ), sn = mod( dt, PI );
	return fract(sin(sn) * c);
}

float unpack(vec4 rgbaDepth) {
    const vec4 bitShift = vec4(1.0, 1.0/256.0, 1.0/(256.0*256.0), 1.0/(256.0*256.0*256.0));
    return dot(rgbaDepth, bitShift);
}

vec2 poissonDisk[NUM_SAMPLES];


float getBIAS(){
  float m = 0.05;
  float bias = max(m , m* (1.0 - dot(normalize(vNormal), normalize(uLightPos - vFragPos)))) * 0.1;
  return bias;
}
void poissonDiskSamples( const in vec2 randomSeed ) {

  float ANGLE_STEP = PI2 * float( NUM_RINGS ) / float( NUM_SAMPLES );
  float INV_NUM_SAMPLES = 1.0 / float( NUM_SAMPLES );

  float angle = rand_2to1( randomSeed ) * PI2;
  float radius = INV_NUM_SAMPLES;
  float radiusStep = radius;

  for( int i = 0; i < NUM_SAMPLES; i ++ ) {
    poissonDisk[i] = vec2( cos( angle ), sin( angle ) ) * pow( radius, 0.75 );
    radius += radiusStep;
    angle += ANGLE_STEP;
  }
}

void uniformDiskSamples( const in vec2 randomSeed ) {

  float randNum = rand_2to1(randomSeed);
  float sampleX = rand_1to1( randNum ) ;
  float sampleY = rand_1to1( sampleX ) ;

  float angle = sampleX * PI2;
  float radius = sqrt(sampleY);

  for( int i = 0; i < NUM_SAMPLES; i ++ ) {
    poissonDisk[i] = vec2( radius * cos(angle) , radius * sin(angle)  );

    sampleX = rand_1to1( sampleY ) ;
    sampleY = rand_1to1( sampleX ) ;

    angle = sampleX * PI2;
    radius = sqrt(sampleY);
  }
}

#define SEARCHRADIUS 0.0015
float findBlocker( sampler2D shadowMap,  vec2 uv, float zReceiver ) {
  // vPositionFromLight.z
  poissonDiskSamples(uv);
  float blockDepth = 0.0;
  int cnt = 0 ;
  for(int i = 0; i < BLOCKER_SEARCH_NUM_SAMPLES;i++)
  {
    vec2 randCoords = uv + SEARCHRADIUS * poissonDisk[i];
    float depthOnShadownMap = unpack(texture2D(shadowMap,randCoords));
    if(zReceiver > depthOnShadownMap){
      blockDepth += depthOnShadownMap;
      cnt++;
    }
  }
  if(blockDepth < EPS)
    return -1.0;

	return blockDepth / float(cnt);
}

float useShadowMap(sampler2D shadowMap, vec4 shadowCoord){
  vec4 depthRGBA = texture2D(shadowMap,shadowCoord.xy);
  float depthUnpack = unpack(depthRGBA);
  float curDepth = shadowCoord.z;

  return curDepth - getBIAS() <= depthUnpack + EPS ? 1.0: 0.0;
}

float PCF(sampler2D shadowMap, vec4 coords ,float filterSize) {
  float curDepth = coords.z;
  float sum = 0.0;
  poissonDiskSamples(coords.xy);
  for(int i = 0 ; i < PCF_NUM_SAMPLES; i++)
  {
    //vec4 randCoords = vec4(coords.xy + filterSize * poissonDisk[i] ,coords.zw);
    //float depth = unpack( texture2D(shadowMap ,randCoords.xy));
    //float vis = curDepth - getBIAS() <= depth ? 1.0 : 0.0;
    //sum += vis;
    vec4  rand = vec4(coords.xy + SEARCHRADIUS * poissonDisk[i] ,coords.zw);
    sum += useShadowMap(shadowMap, rand);
  }
 
  return sum / float(PCF_NUM_SAMPLES);
}

float PCSS(sampler2D shadowMap, vec4 coords){
  float zReceiver = coords.z;
  // STEP 1: avgblocker depth
  float averBlockDepth = findBlocker(shadowMap,coords.xy, zReceiver);
  if(averBlockDepth < EPS)
    return 1.0;  
  // STEP 2: penumbra size
  float filterSize = (zReceiver - averBlockDepth) * SEARCHRADIUS / averBlockDepth;
  // STEP 3: filtering
  
  return PCF(shadowMap,coords,filterSize);
}



vec3 blinnPhong() {
  vec3 color = texture2D(uSampler, vTextureCoord).rgb;
  color = pow(color, vec3(2.2));

  vec3 ambient = 0.05 * color;

  vec3 lightDir = normalize(uLightPos);
  vec3 normal = normalize(vNormal);
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 light_atten_coff =
      uLightIntensity / pow(length(uLightPos - vFragPos), 2.0);
  vec3 diffuse = diff * light_atten_coff * color;

  vec3 viewDir = normalize(uCameraPos - vFragPos);
  vec3 halfDir = normalize((lightDir + viewDir));
  float spec = pow(max(dot(halfDir, normal), 0.0), 32.0);
  vec3 specular = uKs * light_atten_coff * spec;

  vec3 radiance = (ambient + diffuse + specular);
  vec3 phongColor = pow(radiance, vec3(1.0 / 2.2));
  return phongColor;
}

void main(void) {

  float visibility;
  vec3 shadowCoord = vPositionFromLight.xyz / vPositionFromLight.w * 0.5 + 0.5;
  //visibility = useShadowMap(uShadowMap, vec4(shadowCoord, 1.0));
  //visibility = PCF(uShadowMap, vec4(shadowCoord, 1.0), 0.0005);
  visibility = PCSS(uShadowMap, vec4(shadowCoord, 1.0));

  vec3 phongColor = blinnPhong();

  gl_FragColor = vec4(phongColor * visibility, 1.0);
  //gl_FragColor = vec4(phongColor, 1.0);
}