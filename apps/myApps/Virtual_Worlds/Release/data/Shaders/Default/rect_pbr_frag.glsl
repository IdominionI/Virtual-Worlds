#version 450

#define IN in
#define OUT out
#define TEXTURE texture
#define FRAG_COLOR fragColor
out vec4 fragColor;
#define SAMPLER sampler2DRect
#define MAX_LIGHTS 2
#define HAS_TEXTURE 1
#define HAS_COLOR 0
#define PBR_QUALITY_LEVEL_HIGH 1
#define HAS_SHADOWS 1
#define SHADOWS_USE_CUBE_MAP_ARRAY 1
#define SHADOWS_USE_TEXTURE_ARRAY 1

IN vec3 v_worldPosition;
IN vec3 v_worldNormal;
IN vec2 v_texcoord; // receive the texCoord if needed

#if HAS_COLOR
IN vec4 v_color;
#endif

uniform vec3 uCameraPos;

#if !defined(PBR_QUALITY_LEVEL_LOW) && !defined(PBR_QUALITY_LEVEL_HIGH)
// set high level as default, no middle ground :0
#define PBR_QUALITY_LEVEL_HIGH
#endif




struct shadowData {
        float enabled;
        vec3 lightWorldPos;
        vec3 lightUp;
        vec3 lightRight;
        float near;
        float far;
        float shadowType; // 0 = hard, 1 = pcf 2x2, 2 = pcf 9 samples, 3 = pcf 20 samples
        int texIndex;
        float strength;
        float bias;
        float normalBias;
        float sampleRadius;
        mat4 shadowMatrix;
};

#if defined(MAX_LIGHTS) && MAX_LIGHTS
uniform shadowData shadows[MAX_LIGHTS];
#endif

#ifndef FLT_EPS
#define FLT_EPS            0.0006103515625
#endif

#ifndef ONE_LESS_FLT_EPS
#define ONE_LESS_FLT_EPS            1.0-FLT_EPS
#endif

#ifdef SHADOWS_USE_CUBE_MAP_ARRAY
uniform samplerCubeArray uShadowCubeMap;
#else
uniform samplerCube uShadowCubeMap;
#endif

#ifdef SHADOWS_USE_TEXTURE_ARRAY
uniform sampler2DArrayShadow uShadowMapDirectional;
uniform sampler2DArrayShadow uShadowMapSpot;
uniform sampler2DArray uShadowMapArea;
#else
uniform sampler2DShadow uShadowMapDirectional;
uniform sampler2DShadow uShadowMapSpot;
uniform sampler2D uShadowMapArea;
#endif

mediump vec2 cardinalSampleDisk4_v2[4] = vec2[](vec2(-1, 0), vec2(1, 0), vec2(0, -1), vec2(0, 1));
mediump vec2 cornerSampleDisk4_v2[4] = vec2[](vec2(-1, -1), vec2(1, -1), vec2(1, 1), vec2(-1, 1));
mediump vec2 poissonDisk16_v2[16] = vec2[](
                                                                                 vec2( -0.94201624, -0.39906216 ),vec2( 0.94558609, -0.76890725 ),
                                                                                 vec2( -0.094184101, -0.92938870 ),vec2( 0.34495938, 0.29387760 ),
                                                                                 vec2( -0.91588581, 0.45771432 ),vec2( -0.81544232, -0.87912464 ),
                                                                                 vec2( -0.38277543, 0.27676845 ),vec2( 0.97484398, 0.75648379 ),
                                                                                 vec2( 0.44323325, -0.97511554 ),vec2( 0.53742981, -0.47373420 ),
                                                                                 vec2( -0.26496911, -0.41893023 ),vec2( 0.79197514, 0.19090188 ),
                                                                                 vec2( -0.24188840, 0.99706507 ),vec2( -0.81409955, 0.91437590 ),
                                                                                 vec2( 0.19984126, 0.78641367 ),vec2( 0.14383161, -0.14100790 ));

// vec 3 arrays of ofset direction for sampling
mediump vec3 cardinalSamplingDisk6_v3[6] = vec3[](vec3(0, 0, -1), vec3(0, 0, 1),vec3(-1, 0, 0), vec3(-1, 0, 0),vec3(0, -1, 0), vec3(0, 1, 0));
mediump vec3 cornerSamplingDisk8_v3[8] = vec3[](vec3(-1, -1, -1), vec3(1, -1, -1), vec3(1, 1, -1), vec3(-1, 1, -1),vec3(-1, -1, 1), vec3(1, -1, 1), vec3(1, 1, 1), vec3(-1, 1, 1));
// array of offset direction for sampling
mediump vec3 gridSamplingDisk20_v3[20] = vec3[](
 vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
 vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
 vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
 vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
 vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1));

const vec2 shadowCoordsMin = vec2(FLT_EPS);
const vec2 shadowCoordsMax = vec2(ONE_LESS_FLT_EPS);

#ifdef SHADOWS_USE_CUBE_MAP_ARRAY
void AddShadowAmountCubeMap( in samplerCubeArray aShadowMap, in float acurrentDepth, in vec3 st3, in int aTexIndex, inout float ashadow ) {
#else
void AddShadowAmountCubeMap( in samplerCube aShadowMap, in float acurrentDepth, in vec3 st3, in int aTexIndex, inout float ashadow ) {
#endif
#ifdef SHADOWS_USE_CUBE_MAP_ARRAY
        float closestDepth = texture(aShadowMap, vec4(st3, aTexIndex) ).r;
#else
        float closestDepth = texture(aShadowMap, st3 ).r;
#endif
        if(closestDepth < 1.0 && acurrentDepth > closestDepth) {
                ashadow += 1.0;
        }
}

#ifdef SHADOWS_USE_CUBE_MAP_ARRAY
float SampleShadowCube(in shadowData aShadowData, in samplerCubeArray aShadowMap, vec3 fragToLight, float acurrentDepth, float aclosestDepth, float offset, float abias ) {
#else
float SampleShadowCube(in shadowData aShadowData, in samplerCube aShadowMap, vec3 fragToLight, float acurrentDepth, float aclosestDepth, float offset, float abias ) {
#endif
        float shadow = 0.0;
        float currentDepth = acurrentDepth;
        float closestDepth = aclosestDepth;
        if(closestDepth < 1.0 && currentDepth - abias > closestDepth) {
                shadow = 1.f;
        }
        // adjust for bias here //
        currentDepth = currentDepth - abias;
        vec3 st3 = fragToLight;
        if( aShadowData.shadowType < 1.0 ) {
                // already set shadow above
        } else if( aShadowData.shadowType < 2.0 ) {
                for( int i = 0; i < 8; i++ ) {
                        st3 = fragToLight + (cornerSamplingDisk8_v3[i] * offset);
                        AddShadowAmountCubeMap( aShadowMap, currentDepth, st3, aShadowData.texIndex, shadow );
                }
                // dividing by 9 because shadow was set initially and added a grid of 8
                shadow /= 9.0;
        } else if( aShadowData.shadowType < 3.0 ) {
                int samples = 8;
                for(int i = 0; i < samples; ++i) {
                        st3 = fragToLight + (cornerSamplingDisk8_v3[i] * offset);
                        AddShadowAmountCubeMap( aShadowMap, currentDepth, st3, aShadowData.texIndex, shadow );
                }
                if( shadow > 0.0 ) {
                        int samples = 6;
                        for(int i = 0; i < samples; ++i) {
                                st3 = fragToLight + (cardinalSamplingDisk6_v3[i] * offset);
                                AddShadowAmountCubeMap( aShadowMap, currentDepth, st3, aShadowData.texIndex, shadow );
                        }
                        shadow /= 15.0;
                } else {
                        shadow /= 9.0;
                }
        } else {
                int samples = 20;
                for(int i = 0; i < samples; ++i) {
                        st3 = fragToLight + (gridSamplingDisk20_v3[i] * offset);
                        AddShadowAmountCubeMap( aShadowMap, currentDepth, st3, aShadowData.texIndex, shadow );
                }
                shadow /= float(samples+1);
        }
        return shadow;
}

float PointLightShadow( in shadowData aShadowData, in vec3 aWorldFragPos, in vec3 aWorldNormal ) {
        // aWorldNormal should be normalized
        vec3 normalOffset = aWorldNormal*(aShadowData.normalBias);
        vec3 lightDiff = (aWorldFragPos+normalOffset) - aShadowData.lightWorldPos;
        float cosTheta = dot( aWorldNormal, normalize(lightDiff));
        float bias = max(aShadowData.bias * ( 1.0-abs(cosTheta )), aShadowData.bias * 0.05);
        float currentDepth = length(lightDiff) / aShadowData.far;
#ifdef SHADOWS_USE_CUBE_MAP_ARRAY
        float closestDepth = texture(uShadowCubeMap, vec4(lightDiff, aShadowData.texIndex) ).r;
#else
        float closestDepth = texture(uShadowCubeMap, lightDiff ).r;
#endif
        float diskRadius = (abs(currentDepth-closestDepth)/(aShadowData.far*0.5)) * aShadowData.sampleRadius + aShadowData.sampleRadius;
        float shadow = SampleShadowCube(aShadowData, uShadowCubeMap, lightDiff, currentDepth, closestDepth, diskRadius, bias);
        return shadow;
}

#ifdef SHADOWS_USE_TEXTURE_ARRAY
void AddShadowVisibility( in sampler2DArrayShadow aShadowMap, in vec3 acoords, in int aTexIndex, inout float visibility ) {
#else
void AddShadowVisibility( in sampler2DShadow aShadowMap, in vec3 acoords, in int aTexIndex, inout float visibility ) {
#endif
        acoords.xy = clamp( acoords.xy, shadowCoordsMin, shadowCoordsMax );
#ifdef SHADOWS_USE_TEXTURE_ARRAY
        visibility += texture(aShadowMap, vec4(acoords.xy, aTexIndex, acoords.z ));
#else
        visibility += texture(aShadowMap, vec3(acoords.xy, acoords.z ));
#endif
}

#ifdef SHADOWS_USE_TEXTURE_ARRAY
float SampleShadow(in shadowData aShadowData, in sampler2DArrayShadow aShadowMap, vec4 fragPosLightSpace, float offset, float abias ) {
#else
float SampleShadow(in shadowData aShadowData, in sampler2DShadow aShadowMap, vec4 fragPosLightSpace, float offset, float abias ) {
#endif
        // perform perspective divide
        vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
        if (projCoords.x >= 1.0 || projCoords.x <= 0.0 ||
                projCoords.y >= 1.0 || projCoords.y <= 0.0 ||
                projCoords.z >= 1.0 || projCoords.z <= 0.0) {
                return 1.0;
        }
        float bias = abias;
        int shadowTexIndex = aShadowData.texIndex;
        float visibility = 1.0;
        projCoords.z = projCoords.z-bias;
#ifdef SHADOWS_USE_TEXTURE_ARRAY
        visibility = texture(aShadowMap, vec4(projCoords.xy, shadowTexIndex, projCoords.z ));
#else
        visibility = texture(aShadowMap, projCoords );
#endif
        vec3 st3 = projCoords.xyz;
        if( aShadowData.shadowType < 1.0 ) {
                //hard shadow, visibility already set above
        } else if( aShadowData.shadowType < 2.0 ) {
                float radius = offset;
                // sample from the corners
                for( int i = 0; i < 4; i++ ) {
                        st3.xy = projCoords.xy+cornerSampleDisk4_v2[i]*radius;
                        AddShadowVisibility( aShadowMap, st3, shadowTexIndex, visibility );
                }
                visibility /= 5.0;
        } else if( aShadowData.shadowType < 3.0 ) {
                float radius = offset;
                // sample from the corners
                for( int i = 0; i < 4; i++ ) {
                        st3.xy = projCoords.xy+cornerSampleDisk4_v2[i]*radius;
                        AddShadowVisibility( aShadowMap, st3, shadowTexIndex, visibility );
                }
                // sample from the up, down, left, right
                for( int i = 0; i < 4; i++ ) {
                        st3.xy = projCoords.xy+cardinalSampleDisk4_v2[i]*radius;
                        AddShadowVisibility( aShadowMap, st3, shadowTexIndex, visibility );
                }
                visibility /= 9.0;
        } else {
                float radius = offset;
                for( int i = 0; i < 16; i++ ) {
                        st3.xy = projCoords.xy+poissonDisk16_v2[i]*radius;
                        AddShadowVisibility( aShadowMap, st3, shadowTexIndex, visibility );
                }
                visibility /= 17.0;
        }
        return visibility;
}

#ifdef SHADOWS_USE_TEXTURE_ARRAY
void AddShadowAmountSampler2D( in sampler2DArray aShadowMap, in vec3 st3, in int aTexIndex, inout float ashadow ) {
#else
void AddShadowAmountSampler2D( in sampler2D aShadowMap, in vec3 st3, in int aTexIndex, inout float ashadow ) {
#endif
        st3.xy = clamp( st3.xy, shadowCoordsMin, shadowCoordsMax );
#ifdef SHADOWS_USE_TEXTURE_ARRAY
        float closestDepth = texture(aShadowMap, vec3(st3.xy, aTexIndex )).r;
#else
        float closestDepth = texture(aShadowMap, st3.xy ).r;
#endif
        if(closestDepth < 1.0 && st3.z > closestDepth) {
                ashadow += 1.f;
        }
}

#ifdef SHADOWS_USE_TEXTURE_ARRAY
float SampleShadow2D(in shadowData aShadowData, in sampler2DArray aShadowMap, vec3 aprojCoords, float aclosestDepth, float offset, float abias ) {
#else
float SampleShadow2D(in shadowData aShadowData, in sampler2D aShadowMap, vec3 aprojCoords, float aclosestDepth, float offset, float abias ) {
#endif
        float shadow = 0.0;
        float bias = abias;
        int shadowTexIndex = aShadowData.texIndex;
        vec3 projCoords = aprojCoords;
        projCoords.z = projCoords.z-bias;

        vec3 st3 = projCoords.xyz;
//      float currentDepth = projCoords.z;
        float closestDepth = aclosestDepth;

        if(closestDepth < 1.0 && st3.z > closestDepth) {
                shadow = 1.0;
        }

        if( aShadowData.shadowType < 1.0 ) {
                // hard shadow
        } else if( aShadowData.shadowType < 2.0 ) {
                float radius = offset;
                // sample from the corners
                for( int i = 0; i < 4; i++ ) {
                        st3.xy = projCoords.xy+cornerSampleDisk4_v2[i]*radius;
                        AddShadowAmountSampler2D( aShadowMap, st3, shadowTexIndex, shadow );
                }
                shadow /= 5.0;
        } else if( aShadowData.shadowType < 3.0 ) {
                float radius = offset;
                // sample from the corners
                for( int i = 0; i < 4; i++ ) {
                        st3.xy = projCoords.xy+cornerSampleDisk4_v2[i]*radius;
                        AddShadowAmountSampler2D( aShadowMap, st3, shadowTexIndex, shadow );
                }
                if( shadow < 5.0 ) {
                        // sample from the up, down, left, right
                        for( int i = 0; i < 4; i++ ) {
                                st3.xy = projCoords.xy+cardinalSampleDisk4_v2[i]*radius;
                                AddShadowAmountSampler2D( aShadowMap, st3, shadowTexIndex, shadow );
                        }
                        shadow /= 9.0;
                } else {
                        shadow /= 5.0;
                }
        } else {
                float radius = offset;
                int filterSize = 12;
                radius = offset / (float(filterSize));
                for (int i = -filterSize; i <= filterSize; i++) {
                        for (int j = -filterSize; j <= filterSize; j++) {
                                st3.xy = projCoords.xy + vec2(float(i),float(j)) * radius;
                                AddShadowAmountSampler2D( aShadowMap, st3, shadowTexIndex, shadow );
                        }
                }
                shadow /= (1.0+((2.0 * float(filterSize) + 1.0) * (2.0 * float(filterSize) + 1.0)));
        }
        return shadow;
}

float getShadowBias(in shadowData aShadowData, in vec3 aWorldFragPos, in vec3 aWorldNormal) {
        vec3 lightDiff = aWorldFragPos - aShadowData.lightWorldPos;
        float cosTheta = dot(aWorldNormal, normalize(lightDiff));
        cosTheta = clamp(cosTheta, 0.0, 1.0);
        return clamp(aShadowData.bias * 0.1 * (1.0-(cosTheta)), aShadowData.bias * 0.01, aShadowData.bias);
}

float DirectionalShadow( in shadowData aShadowData, in vec3 aWorldFragPos, in vec3 aWorldNormal) {
        float bias = getShadowBias(aShadowData, aWorldFragPos, aWorldNormal);
        vec3 normalOffset = aWorldNormal*(aShadowData.normalBias);
        vec4 fragPosLightSpace = aShadowData.shadowMatrix * vec4(aWorldFragPos+normalOffset, 1.0);
        float visibility = SampleShadow( aShadowData, uShadowMapDirectional, fragPosLightSpace, aShadowData.sampleRadius, bias );
        return clamp(1.0-visibility, 0.0, 1.0);
}

float SpotShadow( in shadowData aShadowData, in vec3 aWorldFragPos, in vec3 aWorldNormal) {
        float bias = getShadowBias(aShadowData, aWorldFragPos, aWorldNormal);
        vec3 normalOffset = aWorldNormal*(aShadowData.normalBias);
        vec4 fragPosLightSpace = aShadowData.shadowMatrix * vec4(aWorldFragPos+normalOffset, 1.0);
        float visibility = SampleShadow( aShadowData, uShadowMapSpot, fragPosLightSpace, aShadowData.sampleRadius, bias );
        return clamp(1.0-visibility, 0.0, 1.0);
}

//https://www.gamedev.net/tutorials/programming/graphics/effect-area-light-shadows-part-1-pcss-r4971/
#ifdef SHADOWS_USE_TEXTURE_ARRAY
vec2 PCSS_BlockerDistance(in sampler2DArray atex, int aSampleIndex, in vec3 projCoord, in float searchUV, in vec2 rotationTrig) {
#else
vec2 PCSS_BlockerDistance(in sampler2D atex, int aSampleIndex, in vec3 projCoord, in float searchUV, in vec2 rotationTrig) {
#endif
        int PCSS_SampleCount = 6;
        // Perform N samples with pre-defined offset and random rotation, scale by input search size
        float blockers = 0.0;
        float avgBlocker = 0.0;
        int filterSize = PCSS_SampleCount;
        for (int i = -filterSize; i <= filterSize; i++) {
                for (int j = -filterSize; j <= filterSize; j++) {
                        vec2 offset = vec2(float(i),float(j)) * searchUV;
#ifdef SHADOWS_USE_TEXTURE_ARRAY
                        float z = texture(atex, vec3(projCoord.xy + offset, aSampleIndex) ).x;
#else
                        float z = texture(atex, projCoord.xy + offset).x;
#endif
                        if (z < projCoord.z) {
                                blockers += 1.0;
                                avgBlocker += z;
                        }
                }
        }
        // Calculate average blocker depth
        if(blockers > 0.0) {
                avgBlocker /= blockers;
        }
        // To solve cases where there are no blockers - we output 2 values - average blocker depth and no. of blockers
        return vec2(avgBlocker, blockers);
}

float AreaShadow( in shadowData aShadowData, in vec3 aWorldFragPos, in vec3 aWorldNormal, in vec2 lightSize) {
        float bias = getShadowBias(aShadowData, aWorldFragPos, aWorldNormal);
        vec3 normalOffset = aWorldNormal*(aShadowData.normalBias);
        vec4 fragPosLightSpace = aShadowData.shadowMatrix * vec4(aWorldFragPos, 1.0);
        vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

        vec3 claMin = vec3(FLT_EPS);
        vec3 claMax = vec3(ONE_LESS_FLT_EPS);
        projCoords = clamp(projCoords, claMin, claMax);

#ifdef SHADOWS_USE_TEXTURE_ARRAY
        float closestDepth = texture(uShadowMapArea, vec3(projCoords.xy, aShadowData.texIndex) ).r;
#else
        float closestDepth = texture(uShadowMapArea, projCoords.xy ).r;
#endif
        float tsize = float(textureSize( uShadowMapArea, aShadowData.texIndex ).x);
        vec2 averageBlocker = PCSS_BlockerDistance(uShadowMapArea, aShadowData.texIndex, projCoords, (lightSize.x*0.5)/tsize, vec2(1.0,1.0));
        // If there isn't any average blocker distance - it means that there is no blocker at all
        if (averageBlocker.y < 1.0 ) {
                return 0.0f;
        }
        float averageBlockerDepth = averageBlocker.x;
        float receiverDepth = projCoords.z;
        float pvalue = (receiverDepth - averageBlockerDepth) / averageBlockerDepth;
        float penumbraSize = (clamp(abs(pvalue) * 4.0, 0.0, 1.0)) * aShadowData.sampleRadius + aShadowData.sampleRadius * 0.01;
        fragPosLightSpace = aShadowData.shadowMatrix * vec4(aWorldFragPos+normalOffset, 1.0);
        projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

        float shadow = SampleShadow2D(aShadowData, uShadowMapArea, projCoords, closestDepth, penumbraSize, bias );
        return shadow;
}



#ifndef TWO_PI
#define TWO_PI 6.2831853072
#endif

#ifndef PI
#define PI 3.14159265359
#endif

#ifndef HALF_PI
#define HALF_PI 1.57079632679
#endif

#ifndef PHI
#define PHI 0.5*(1.0+sqrt(5.0))
#endif

#ifndef ONE_OVER_PI
#define ONE_OVER_PI 1.0/PI
#endif



#ifndef FUNCTIONS_UTILS
#define FUNCTIONS_UTILS

#ifndef MEDIUMP_FLT_MAX
#define MEDIUMP_FLT_MAX    65504.0
#endif

#ifndef saturateMediump
#define saturateMediump(x) min(x, MEDIUMP_FLT_MAX)
#endif

#ifndef FLT_EPS
#define FLT_EPS            0.00006103515625
#endif

#ifndef saturate
#define saturate(x) clamp(x, 0.0, 1.0)
#endif



// pow functions //
float pow5( in float a ) {
        float aSq = a * a;
        return aSq * aSq * a;
}

vec2 pow5( in vec2 a ) {
        vec2 aSq = a * a;
        return aSq * aSq * a;
}

vec3 pow5( in vec3 a ) {
        vec3 aSq = a * a;
        return aSq * aSq * a;
}

vec4 pow5( in vec4 a ) {
        vec4 aSq = a * a;
        return aSq * aSq * a;
}

#endif


#ifndef FUNCTIONS_COLOR_SPACES
#define FUNCTIONS_COLOR_SPACES

// https://github.com/patriciogonzalezvivo/lygia/blob/main/color/space/gamma2linear.glsl
#if !defined(GAMMA_CONST) && !defined(PBR_QUALITY_LEVEL_LOW)
#define GAMMA_CONST 2.2
#define ONE_OVER_GAMMA 1.0/GAMMA_CONST
#endif

float gamma2Linear(in float v) {
#ifdef GAMMA_CONST
        return pow(v, GAMMA_CONST);
#else
        // assume gamma 2.0
        return v * v;
#endif
}

vec3 gamma2Linear( in vec3 v ) {
#ifdef GAMMA_CONST
        return pow(v, vec3(GAMMA_CONST));
#else
        // assume gamma 2.0
        return v * v;
#endif
}

vec4 gamma2Linear(in vec4 v) {
        return vec4(gamma2Linear(v.rgb), v.a);
}

float linear2Gamma(in float v) {
#ifdef GAMMA_CONST
        return pow(v, ONE_OVER_GAMMA);
#else
        return sqrt(v);
#endif
}

vec3 linear2Gamma(in vec3 v) {
#ifdef GAMMA_CONST
        return pow(v, vec3(ONE_OVER_GAMMA));
#else
        return sqrt(v);
#endif
}

vec4 linear2Gamma(in vec4 v) {
        return vec4(linear2Gamma(v.rgb), v.a);
}

#endif


#ifndef FUNCTIONS_NORMALS
#define FUNCTIONS_NORMALS
//-- PerturbNormal normal map code from ------------------------------------ //
// http://www.geeks3d.com/20130122/normal-mapping-without-precomputed-tangent-space-vectors/
// http://www.thetenthplanet.de/archives/1180
mat3 CotangentFrame(in vec3 N, in vec3 p, in vec2 uv) {
        // get edge vectors of the pixel triangle
        vec3 dp1 = dFdx(p);
        vec3 dp2 = dFdy(p);
        vec2 duv1 = dFdx(uv);
        vec2 duv2 = dFdy(uv);

        // solve the linear system
        vec3 dp2perp = cross(dp2, N);
        vec3 dp1perp = cross(N, dp1);
        vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
        vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

        // construct a scale-invariant frame
        float invmax = inversesqrt(max(dot(T, T), dot(B, B)));
        return mat3(T * invmax, B * invmax, N);
}

vec3 PerturbNormal(in vec3 normalMap, in vec3 N, in vec3 V, in vec2 texcoord) {
        // assume N, the interpolated vertex normal and
        // V, the view vector (vertex to eye)
        vec3 map = normalMap * 255. / 127. - 128. / 127.;
        mat3 TBN = CotangentFrame(N, -V, texcoord);
        return normalize(TBN * map);
}


// https://github.com/mrdoob/three.js/blob/dev/src/renderers/shaders/ShaderChunk/bumpmap_pars_fragment.glsl.js
// Bump Mapping Unparametrized Surfaces on the GPU by Morten S. Mikkelsen
// https://mmikk.github.io/papers3d/mm_sfgrad_bump.pdf

// Evaluate the derivative of the height w.r.t. screen-space using forward differencing (listing 2)
vec2 dHdxy_fwd(in SAMPLER normalMap, in vec2 uv, in float hStrength) {

        vec2 dSTdx = dFdx( v_texcoord ).xy;
        vec2 dSTdy = dFdy( v_texcoord ).xy;

        float Hll = hStrength * TEXTURE(normalMap, uv ).x;
        float dBx = hStrength * TEXTURE(normalMap, uv + dSTdx ).x - Hll;
        float dBy = hStrength * TEXTURE(normalMap, uv + dSTdy ).x - Hll;

        return vec2( dBx, dBy );

}

vec3 perturbNormalArb( vec3 apos, vec3 anorm, vec2 dHdxy, float faceDirection ) {
        vec3 vSigmaX = dFdx( apos.xyz );
        vec3 vSigmaY = dFdy( apos.xyz );
        vec3 vN = anorm; // normalized

        vec3 R1 = cross( vSigmaY, vN );
        vec3 R2 = cross( vN, vSigmaX );

        float fDet = dot( vSigmaX, R1 ) * faceDirection;

        vec3 vGrad = sign( fDet ) * ( dHdxy.x * R1 + dHdxy.y * R2 );
        return normalize( abs( fDet ) * anorm - vGrad );

}
#endif



//#ifndef SAMPLE_TEXTURE
//#define SAMPLE_TEXTURE(TEX,UV) texture(TEX, UV )
//#endif

#ifndef MATERIAL_STRUCT
#define MATERIAL_STRUCT
struct Material {
        // material properties
        vec4 albedo;// = vec4( vec3(0.5), 1.0);
        vec4 emissive;// = vec4(0.0);
        float ao;// = 1.0;
        float roughness;// = 0.3;
        float metallic;// = 0.0;
        float reflectance;// = 0.5;
        float clearCoat; // 0
        float clearCoatRoughness;// 0.1

        vec3 f0;// = vec3(0.04);
        vec3 f90;// = vec3(1.0);

        vec3 worldNormal;// = (1.0, 0.0, 0.0);
        vec3 normalWorldGeometry;
        vec3 clearcoatNormal;

        vec2 uv;
        float iblExposure;
};
#endif

#ifndef FUNCTIONS_MATERIALS
#define FUNCTIONS_MATERIALS

//#pragma include "shaderUtils.glsl"

// material parameters
uniform vec4 mat_diffuse;
uniform vec4 mat_emissive;
uniform float mat_roughness;
uniform float mat_metallic;
uniform float mat_reflectance;
uniform float mat_ibl_exposure;
#if defined(HAS_CLEAR_COAT)
uniform vec2 mat_clearcoat;
#endif
#if defined(HAS_TEX_NORMAL) || defined(HAS_TEX_DISPLACEMENT)
uniform float mat_normal_mix;
#endif
#if defined(HAS_TEX_DISPLACEMENT)
uniform float mat_displacement_strength;
uniform float mat_displacement_normals_strength;
#endif

#if HAS_TEXTURE
uniform SAMPLER tex0;
#endif

vec4 getMaterialAlbedo(in vec2 aUv) {
        vec4 albedo = mat_diffuse;
        #if defined(HAS_TEXTURE)
                #if HAS_TEXTURE
                        albedo = TEXTURE(tex0, aUv);
                #endif
        #endif

        #if defined(HAS_COLOR)
                #if HAS_COLOR
                        albedo *= v_color;
                #endif
        #endif
        return gamma2Linear(albedo);
}

// apply emmisive texture
//#if defined(HAS_TEX_EMISSIVE)
//uniform SAMPLER tex_emissive;
//#endif

vec4 getMaterialEmissive(in vec2 aUv) {
        vec4 emission = mat_emissive;
#if defined(HAS_TEX_EMISSIVE)
        emission = TEXTURE(tex_emissive, aUv);
#endif
        return gamma2Linear(emission);
}

//#if defined(HAS_TEX_OCCLUSION)
//uniform SAMPLER tex_occlusion;
//#endif

//#if defined(HAS_TEX_AO_ROUGHNESS_METALLIC)
//uniform SAMPLER tex_ao_roughness_metallic;
//#endif

float getMaterialAO(in vec2 aUv) {
        float ao = 1.0;
#if defined(HAS_TEX_OCCLUSION)
        ao = TEXTURE(tex_occlusion, aUv).r;
#elif defined(HAS_TEX_AO_ROUGHNESS_METALLIC)
        ao = TEXTURE(tex_ao_roughness_metallic, aUv).r;
#endif
        return ao;
}

//#if defined(HAS_TEX_ROUGHNESS_METALLIC)
//uniform SAMPLER tex_roughness_metallic;
//#endif


//#if defined(HAS_TEX_ROUGHNESS)
//uniform SAMPLER tex_roughness;
//#endif

float getMaterialRoughness(in vec2 aUv) {
        float roughness = mat_roughness;
#if defined(HAS_TEX_AO_ROUGHNESS_METALLIC)
        roughness = TEXTURE(tex_ao_roughness_metallic, aUv).g;
#elif defined(HAS_TEX_ROUGHNESS_METALLIC)
        roughness = TEXTURE(tex_roughness_metallic, aUv).g;
#elif defined(HAS_TEX_ROUGHNESS)
        roughness = TEXTURE(tex_roughness, aUv).g;
#endif
        return max(roughness, 0.001);
}


//#if defined(HAS_TEX_METALLIC)
//uniform SAMPLER tex_metallic;
//#endif

float getMaterialMetallic(in vec2 aUv) {
        float metalness = mat_metallic;
        #if defined(HAS_TEX_AO_ROUGHNESS_METALLIC)
        metalness = TEXTURE(tex_ao_roughness_metallic, aUv).b;
        #elif defined(HAS_TEX_ROUGHNESS_METALLIC)
        metalness = TEXTURE(tex_roughness_metallic, aUv).b;
        #elif defined(HAS_TEX_METALLIC)
        metalness = TEXTURE(tex_metallic, aUv).b;
        #endif
        return metalness;
}

vec3 getAoRoughnessMetallic(in vec2 aUv) {
        vec3 arm = vec3(1.0, mat_roughness, mat_metallic );
        #if defined(HAS_TEX_AO_ROUGHNESS_METALLIC)
                arm.rgb = TEXTURE(tex_ao_roughness_metallic, aUv).rgb;
        #else
                #if defined(HAS_TEX_OCCLUSION)
                        arm.r = TEXTURE(tex_occlusion, aUv).r;
                #endif

                #if defined(HAS_TEX_ROUGHNESS_METALLIC)
                        arm.gb = TEXTURE(tex_roughness_metallic, aUv).gb;
                #else
                        #if defined(HAS_TEX_ROUGHNESS)
                                arm.g = TEXTURE(tex_roughness, aUv).g;
                        #endif
                        #if defined(HAS_TEX_METALLIC)
                                arm.b = TEXTURE(tex_metallic, aUv).b;
                        #endif
                #endif
        #endif
        arm.g = max(arm.g, 0.001);
        return arm;
}

//#if defined(HAS_TEX_NORMAL)
//uniform SAMPLER tex_normal;
//#endif

vec3 getMaterialNormal(in vec2 aUv, in vec3 aNormal, in vec3 aPos ) {
        vec3 tnormal = v_worldNormal;
        #if defined(HAS_TEX_NORMAL)
        tnormal = PerturbNormal(TEXTURE(tex_normal, aUv).rgb, aNormal, aPos, aUv);
        tnormal = mix(v_worldNormal, tnormal, mat_normal_mix );
        #endif
        return normalize(tnormal);
}

float getMaterialReflectance(in vec2 aUv) {
        //TODO: add a reflectance map
        return mat_reflectance;
}

//#if defined(HAS_TEX_CLEARCOAT_INTENSITY)
//uniform SAMPLER tex_clearcoat_intensity;
//#endif

//#if defined(HAS_TEX_CLEARCOAT_ROUGHNESS)
//uniform SAMPLER tex_clearcoat_roughness;
//#endif

//#if defined(HAS_TEX_CLEARCOAT_INTENSITY_ROUGHNESS)
//uniform SAMPLER tex_clearcoat_intensity_roughness;
//#endif



// https://github.com/KhronosGroup/glTF/tree/main/extensions/2.0/Khronos/KHR_materials_clearcoat
vec2 getMaterialClearcoat(in vec2 aUv) {
        // intensity, roughness
        vec2 clearCoat = vec2(0.0, 0.1);
        #if defined(HAS_CLEAR_COAT)
                clearCoat = mat_clearcoat;
                #if defined(HAS_TEX_CLEARCOAT_INTENSITY_ROUGHNESS)
                        vec3 tc = TEXTURE(tex_clearcoat_intensity_roughness, aUv).rgb;
                        clearCoat.x *= tc.r;
                        clearCoat.y *= tc.g;
                #else
                        #if defined(HAS_TEX_CLEARCOAT_INTENSITY)
                                clearCoat.x *= TEXTURE(tex_clearcoat_intensity, aUv).r;
                        #endif
                        #if defined(HAS_TEX_CLEARCOAT_ROUGHNESS)
                                clearCoat.y *= TEXTURE(tex_clearcoat_roughness, aUv).g;
                        #endif
                #endif
        #endif
        return clearCoat;
}

//#if defined(HAS_TEX_CLEARCOAT_NORMAL)
//uniform SAMPLER tex_clearcoat_normal;
//#endif

vec3 getMaterialClearCoatNormal(in vec2 aUv, in vec3 aNormal) {
        vec3 ccn = aNormal;
        #if defined(HAS_CLEAR_COAT)
                #if defined(HAS_TEX_CLEARCOAT_NORMAL)
                        ccn = PerturbNormal(TEXTURE(tex_clearcoat_normal, aUv).rgb, aNormal, aPos, aUv);
                #endif
        #endif
        return ccn;
}

void setupMaterial( inout Material amat ) {
        amat.albedo = getMaterialAlbedo(amat.uv);
        amat.emissive = getMaterialEmissive(amat.uv);
//      amat.ao = getMaterialAO();
//      amat.roughness = getMaterialRoughness();
//      amat.metallic = getMaterialMetallic();
        vec3 arm = getAoRoughnessMetallic(amat.uv);
        amat.ao = arm.r;
        amat.roughness = arm.g;
        amat.metallic = arm.b;
        amat.reflectance = getMaterialReflectance(amat.uv);
#if defined(HAS_CLEAR_COAT)
        vec2 cc = getMaterialClearcoat(amat.uv);
        amat.clearCoat = cc.x; // 0
        amat.clearCoatRoughness = cc.y;// 0.1
#else
        amat.clearCoat = 0.0;
        amat.clearCoatRoughness = 0.1;
#endif
        amat.clearCoatRoughness = saturate( amat.clearCoatRoughness + 0.01 );

        // set some minimums, otherwise things can render black ....
        amat.metallic = saturate( amat.metallic + FLT_EPS);
        amat.roughness = saturate( amat.roughness + 0.01 );
        // reflectance only applies to dielectrics, ie, non metals
        amat.reflectance = saturate( amat.reflectance + FLT_EPS);

        amat.iblExposure = mat_ibl_exposure;

        amat.f0 = vec3(0.04);
        amat.f90 = vec3(1.0);
        amat.worldNormal = getMaterialNormal(amat.uv, v_worldNormal, -v_worldPosition );
        amat.normalWorldGeometry = v_worldNormal;
        #if defined(HAS_CLEAR_COAT)
        amat.clearcoatNormal = getMaterialClearCoatNormal(amat.uv, amat.normalWorldGeometry);
        #else
        amat.clearcoatNormal = amat.normalWorldGeometry;
        #endif

        #if defined(HAS_TEX_DISPLACEMENT)
        float dstrength = mat_displacement_normals_strength * mat_displacement_strength;
        vec3 nnormal = perturbNormalArb( -v_worldPosition, v_worldNormal, dHdxy_fwd(tex_displacement, amat.uv, dstrength), -1.0 );
        amat.worldNormal = mix(amat.worldNormal, nnormal, mat_normal_mix );
        #endif

}

#endif



#ifndef FUNCTIONS_LIGHTING
#define FUNCTIONS_LIGHTING

//#define MIN_N_DOT_V 1e-4

//---------------------------------------------------
float clampNoV(in float NoV) {
        // Neubelt and Pettineo 2013, "Crafting a Next-gen Material Pipeline for The Order: 1886"
        return max(NoV, FLT_EPS);
}

//---------------------------------------------------
// https://github.com/google/filament/blob/main/shaders/src/brdf.fs
// Bruce Walter et al. 2007. Microfacet Models for Refraction through Rough Surfaces.
float D_GGX(float NoH, float roughness) {
        float a = NoH * roughness;
        float k = roughness / (1.0 - NoH * NoH + a * a);
        return saturateMediump(k * k * ONE_OVER_PI);//(1.0 / PI);
}

// Bruce Walter et al. 2007. Microfacet Models for Refraction through Rough Surfaces.
float D_GGX(float roughness, float NoH, const vec3 n, const vec3 h) {
        vec3 NxH = cross(n, h);
        float a = NoH * roughness;
        float k = roughness / (dot(NxH, NxH) + a * a);
        float d = k * k * ONE_OVER_PI;//(1.0 / PI);
        return saturateMediump(d);
}

// https://github.com/google/filament/blob/e1ba37b910617d0ea68bf17071c17264653e3dd0/shaders/src/brdf.fs#L54
float D_GGX(float roughness, float NoH, const vec3 h) {
    // Walter et al. 2007, "Microfacet Models for Refraction through Rough Surfaces"

    // In mediump, there are two problems computing 1.0 - NoH^2
    // 1) 1.0 - NoH^2 suffers floating point cancellation when NoH^2 is close to 1 (highlights)
    // 2) NoH doesn't have enough precision around 1.0
    // Both problem can be fixed by computing 1-NoH^2 in highp and providing NoH in highp as well

    // However, we can do better using Lagrange's identity:
    //      ||a x b||^2 = ||a||^2 ||b||^2 - (a . b)^2
    // since N and H are unit vectors: ||N x H||^2 = 1.0 - NoH^2
    // This computes 1.0 - NoH^2 directly (which is close to zero in the highlights and has
    // enough precision).
    // Overall this yields better performance, keeping all computations in mediump
//#if defined(TARGET_MOBILE)
    //vec3 NxH = cross(shading_normal, h);
    //float oneMinusNoHSquared = dot(NxH, NxH);
//#else
    float oneMinusNoHSquared = 1.0 - NoH * NoH;
//#endif

    float a = NoH * roughness;
    float k = roughness / (oneMinusNoHSquared + a * a);
    float d = k * k * (1.0 / PI);
    return saturateMediump(d);
}

//Eric Heitz. 2014. Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs.
//Journal of Computer Graphics Techniques, 3 (2).
float V_SmithGGXCorrelated(in float NoV, in float NoL, in float aroughness) {
        float a2 = aroughness;// * aroughness;
        float GGXV = NoL * sqrt((NoV - a2 * NoV) * NoV + a2);
        float GGXL = NoV * sqrt((NoL - a2 * NoL) * NoL + a2);
        float v = 0.5 / (GGXV + GGXL);
        return saturateMediump(v);
}

// Hammon 2017, "PBR Diffuse Lighting for GGX+Smith Microsurfaces"
float V_SmithGGXCorrelatedFast(in float NoV, in float NoL, in float roughness) {
        float v = 0.5 / mix(2.0 * NoL * NoV, NoL + NoV, roughness);
        return saturateMediump(v);
}

float V_Kelemen(float LoH) {
        // Kelemen 2001, "A Microfacet Based Coupled Specular-Matte BRDF Model with Importance Sampling"
        return saturateMediump(0.25 / (LoH * LoH));
}

float V_Neubelt(float NoV, float NoL) {
        // Neubelt and Pettineo 2013, "Crafting a Next-gen Material Pipeline for The Order: 1886"
        return saturateMediump(1.0 / (4.0 * (NoL + NoV - NoL * NoV)));
}

// Christophe Schlick. 1994. An Inexpensive BRDF Model for Physically-Based Rendering. Computer Graphics Forum, 13 (3), 233ÔÇô246.
//vec3 F_Schlick(in float u, in vec3 f0, in float f90) {
//      return f0 + (vec3(f90) - f0) * pow(1.0 - u, 5.0);
//}
//
//vec3 F_Schlick(in float u, in vec3 f0) {
//      float f = pow(1.0 - u, 5.0);
//      return f + f0 * (1.0 - f);
//}

vec3 F_Schlick(const vec3 f0, float f90, float VoH) {
        // Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"
        return f0 + (f90 - f0) * pow5(1.0 - VoH);
}

vec3 F_Schlick(const vec3 f0, float VoH) {
        float f = pow(1.0 - VoH, 5.0);
        return f + f0 * (1.0 - f);
}

float F_Schlick(float f0, float f90, float VoH) {
        return f0 + (f90 - f0) * pow5(1.0 - VoH);
}

float Fd_Lambert() {
        return ONE_OVER_PI;//1.0 / PI;
}



// Brent Burley. 2012. Physically Based Shading at Disney.
// Physically Based Shading in Film and Game Production, ACM SIGGRAPH 2012 Courses.
float Fd_Burley(float roughness, float NoV, float NoL, float LoH) {
        // Burley 2012, "Physically-Based Shading at Disney"
        float f90 = 0.5 + 2.0 * roughness * LoH * LoH;
        float lightScatter = F_Schlick(1.0, f90, NoL);
        float viewScatter  = F_Schlick(1.0, f90, NoV);
        return lightScatter * viewScatter * ONE_OVER_PI;
}


//https://www.unrealengine.com/en-US/blog/physically-based-shading-on-mobile
vec3 EnvBRDFApprox( in vec3 SpecularColor, in float Roughness, in float NoV ) {
        const vec4 c0 = vec4( -1.0, -0.0275, -0.572, 0.022 );
        const vec4 c1 = vec4( 1.0, 0.0425, 1.04, -0.04 );
        vec4 r = Roughness * c0 + c1;
        float a004 = min( r.x * r.x, exp2( -9.28 * NoV ) ) * r.x + r.y;
        vec2 AB = vec2( -1.04, 1.04 ) * a004 + r.zw;
        return SpecularColor * AB.x + AB.y;
}
//https://www.unrealengine.com/en-US/blog/physically-based-shading-on-mobile
vec2 EnvBRDFApproxDFG( in float Roughness, in float NoV ) {
        const vec4 c0 = vec4( -1.0, -0.0275, -0.572, 0.022 );
        const vec4 c1 = vec4( 1.0, 0.0425, 1.04, -0.04 );
        vec4 r = Roughness * c0 + c1;
        float a004 = min( r.x * r.x, exp2( -9.28 * NoV ) ) * r.x + r.y;
        vec2 AB = vec2( -1.04, 1.04 ) * a004 + r.zw;
        return vec2(AB.x, AB.y);
}

// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
        return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}



// ----------------------------------------------------------------------------
// https://google.github.io/filament/Filament.md.html#toc9.5
float computeSpecularAO(float NoV, float ao, float roughness) {
#ifdef PBR_QUALITY_LEVEL_HIGH
        return clamp(pow(NoV + ao, exp2(-16.0 * roughness - 1.0)) - 1.0 + ao, 0.0, 1.0);
#else
        return 1.0;
#endif
}

vec3 getPrefilteredDFG(in vec3 acolor, in float aNoV, in float aroughness ) {
        vec3 dfg = vec3(0.0);
#if defined(PBR_QUALITY_LEVEL_HIGH) && defined(HAS_TEX_ENV_BRDF_LUT)
        //      vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), aroughness)).rg;
        dfg = texture(tex_brdfLUT, vec2(aNoV, aroughness)).rgb;
        //      specularReflectance = (adata.f0 * brdf.x + adata.f90 * brdf.y);
        // vec3 specularColor = f0 * brdf.x + f90 * brdf.y;
#else
//      dfg.xy = EnvBRDFApproxDFG( aroughness*aroughness, aNoV );
        dfg = EnvBRDFApprox(acolor, aroughness, aNoV );
        //      specularReflectance = (adata.f0 * brdf.x + adata.f90 * brdf.y);
        //      specularReflectance = mix(brdf.xxx, brdf.yyy, adata.f0);
        //      specularReflectance = EnvBRDFApprox( adata.f0, aroughness, adata.NoV );
        //return SpecularColor * AB.x + AB.y;
#endif
        return dfg;
}

#endif



#ifndef STRUCT_PBR_DATA
#define STRUCT_PBR_DATA


struct PbrData {
        vec3 directDiffuse;
        vec3 indirectDiffuse;

        vec3 directSpecular;
        vec3 indirectSpecular;

        vec3 diffuse; // multiplied by (1.0 - amat.metallic);
        float NoV;
        vec3 normalWorld;
        vec3 normalWorldGeometry;
        vec3 viewDirectionWorld;
        vec3 reflectionWorld;
        vec3 positionWorld;

        float iblLuminance;

        vec3 f0;
        float f90;
        vec3 dfg;
        vec3 energyCompensation;
};

void setupPbrData( inout PbrData adata, in Material amat, in vec3 aPosWorld, in vec3 aCamPos ) {
        vec3 N = amat.worldNormal;//normalize(v_worldNormal);
        vec3 V = normalize(aCamPos - aPosWorld);
        vec3 R = reflect(-V, N);

        adata.positionWorld = aPosWorld;
        adata.normalWorldGeometry = amat.normalWorldGeometry;

        // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
        // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
        //      vec3 F0 = vec3(0.04);
        vec3 f0 = mix(amat.f0, amat.albedo.rgb, amat.metallic);
        //      f0 = 0.16 * mat.reflectance.rgb * mat.reflectance.rgb * (1.0 - mat.metallic) + mat.albedo.rgb * mat.metallic;
        f0 = 0.16 * amat.reflectance * amat.reflectance * (1.0 - amat.metallic) + amat.albedo.rgb * amat.metallic;

        adata.normalWorld = N;
        adata.reflectionWorld = R;
        adata.viewDirectionWorld = V;
        adata.NoV = max(dot(N, V), 0.0001);
        //      pbrData.f0 = mix(0.04, mat.albedo, mat.metallic);
        adata.f0 = f0;
#ifdef PBR_QUALITY_LEVEL_HIGH
        // cheap luminance approximation
        //      pbrData.f90 = vec3(clamp(dot(pbrData.f0.g, 50.0 * 0.33), 0.0, 1.0));
        adata.f90 = saturate(dot(f0, vec3(50.0 * 0.33)));
#else
        adata.f90 = 1.0;
#endif
        adata.diffuse = amat.albedo.rgb * (1.0 - amat.metallic);
        adata.iblLuminance = amat.iblExposure; // convert to luminance?

        adata.directDiffuse = vec3(0.0);
        adata.indirectDiffuse = vec3(0.0);

        adata.directSpecular = vec3(0.0);
        adata.indirectSpecular = vec3(0.0);

        adata.dfg = getPrefilteredDFG(adata.f0, adata.NoV, amat.roughness );
        adata.dfg.y = clamp( adata.dfg.y, 0.001, 10.0);

        adata.energyCompensation = 1.0 + adata.f0 * (1.0 / (adata.dfg.y) - 1.0);
}

#endif

#ifndef STRUCT_LIGHT_DATA
#define STRUCT_LIGHT_DATA

struct PbrLightData {
        float enabled;
        // 0 = pointlight 1 = directionlight, 2 = spotlight, 3 = area light
        float type;
        // where are we in world space
        vec4 position;
        vec3 direction;
        // light color, intensity is stored in .w
        vec4 diffuse;
        float radius;// = 0.0;
        // attenuation
        float constantAttenuation;
        float linearAttenuation;
        float quadraticAttenuation;
        //      // only for spot
        float spotCutoff;
        float spotCosCutoff;
        float spotExponent;
        //      // only for area
        float width;
        float height;
        vec3 right;
        vec3 up;
};

#if defined(MAX_LIGHTS) && MAX_LIGHTS
uniform PbrLightData lights[MAX_LIGHTS];
#endif

#endif



#ifndef FUNCTIONS_PBR_LIGHT_TYPES
#define FUNCTIONS_PBR_LIGHT_TYPES

// https://google.github.io/filament/Filament.md.html#lighting/directlighting/directionallights
vec3 SurfaceSpecular(in PbrData adata, const vec3 h, float NoL, float NoH, float LoH, in float aroughness) {

        // perceptually linear roughness to roughness (see parameterization)
        float roughness = aroughness * aroughness;

        //      float D_GGX(float roughness, float NoH, const vec3 n, const vec3 h)
        float D = D_GGX( roughness, NoH, adata.normalWorld, h );

#ifdef PBR_QUALITY_LEVEL_HIGH
        //      float f90 = saturate(dot(f0, vec3(50.0 * 0.33)));
        vec3  F = F_Schlick(adata.f0, adata.f90, LoH);
        float V = V_SmithGGXCorrelated(adata.NoV, NoL, roughness);
#else
        vec3  F = F_Schlick(adata.f0, LoH);
        float V = V_SmithGGXCorrelatedFast(adata.NoV, NoL, roughness);
#endif

        // specular BRDF
        vec3 Fr = (D * V) * F;
        return Fr;
}

float SurfaceDiffuse(float roughness, float NoV, float NoL, float LoH) {
#ifdef PBR_QUALITY_LEVEL_LOW
        // lambertian diffuse BRDF
        return Fd_Lambert();
#else
        // TODO: oren nayar?
        //      vec3 Fd = adata.diffuse * Fd_Lambert();
        return Fd_Burley(roughness, NoV, NoL, LoH);
#endif
}

// https://google.github.io/filament/Filament.md.html#materialsystem/clearcoatmodel
#if defined(HAS_CLEAR_COAT)
vec2 SurfaceClearCoat(in PbrData adata, in Material amat, const vec3 h, float NoH, float LoH ) {
// vec2 SurfaceClearCoat(in PbrData adata, in Material amat, float NoH, float LoH ) {
        #ifdef PBR_QUALITY_LEVEL_HIGH
        float D = D_GGX(amat.clearCoatRoughness, NoH, h);
        #else
        float D = D_GGX(NoH,amat.clearCoatRoughness);
        #endif
        float V = V_Kelemen(LoH);
        float F = F_Schlick(0.04, 1.0, LoH) * amat.clearCoat;//amat.clearCoat; // fix IOR to 1.5

        return vec2((D * V) * F, F);
}
#endif


float GetDistanceAttenuation( float distSq, float aLightRad) {
        float rad = max( FLT_EPS, aLightRad);
        float att = saturate(1.0 - distSq / (rad*rad));
        return att * att;
}


//void calcLight(in int aLightIndex, inout PbrData adata, in Material amat ) {
void calcLight(in PbrLightData light, in int aLightIndex, inout PbrData adata, in Material amat ) {

        vec3 lPos = light.position.xyz;
        //vec3 L = normalize(light.position.xyz);
        vec3 L = normalize(lPos);
        //      vec3 h = normalize(aV + aL);
        vec3 halfV = normalize( adata.viewDirectionWorld + L );

        //      float NoV = adata.NoV;//abs(dot(aN, aV)) + FLT_EPS;
        float NoL = saturate(dot(adata.normalWorld, L));
        float NoH = saturate(dot(adata.normalWorld, halfV));
        float LoH = saturate(dot(L, halfV));

        //      vec3 SurfaceSpecular(in PbrData adata, const vec3 h, float NoL, float NoH, float LoH, in float aroughness)
        vec3 Fr = SurfaceSpecular( adata, halfV, NoL, NoH, LoH, amat.roughness );
        Fr *= adata.energyCompensation;
        // diffuseColor = _mat.albedo.rgb * (vec3(1.0) - _mat.f0) * (1.0 - _mat.metallic);
        float sd = SurfaceDiffuse( amat.roughness*amat.roughness, adata.NoV, NoL, LoH );
        // PbrData.diffuse is multiplied by (1.0-metallic).
//      vec3 Fd = adata.diffuse * (vec3(1.0)-adata.f0) * sd;
        vec3 Fd = adata.diffuse * sd;

        float attenuation = 0.0;
        float shadow = 0.0;

        if(light.type<0.5){
                // point light
                vec3 posToLight = adata.positionWorld - light.position.xyz;

                if( light.radius > 0.0 ) {
                        float distanceSquare = dot(posToLight, posToLight);
                        attenuation = GetDistanceAttenuation( distanceSquare, light.radius );
                } else {
                        // from phong.frag
                        float d = length(posToLight);
                        attenuation = saturate(1.0 / (light.constantAttenuation + light.linearAttenuation * d + light.quadraticAttenuation * d * d));
                }
                //              attenuation = saturate(1.0 / (alight.constantAttenuation + alight.linearAttenuation * d + alight.quadraticAttenuation * d * d));
                //              attenuation = 1.0;
                attenuation *= NoL;

#ifdef HAS_SHADOWS
                if( shadows[aLightIndex].enabled > 0.5 ) {
                        shadow = PointLightShadow(shadows[aLightIndex], adata.positionWorld, adata.normalWorld );
                        shadow *= shadows[aLightIndex].strength;
                }
#endif
        }else if(light.type<1.5){
                // directional
                attenuation = NoL;
#ifdef HAS_SHADOWS
                if( shadows[aLightIndex].enabled > 0.5 ) {
                        shadow = DirectionalShadow(shadows[aLightIndex], adata.positionWorld, adata.normalWorld );
                        shadow *= shadows[aLightIndex].strength;
                }
#endif
        }else if(light.type<2.5){
                // spot light
                // Compute vector from surface to light position
                vec3 posToLight = light.position.xyz - adata.positionWorld;
                float spotEffect = dot(light.direction, -normalize(posToLight));

                if (spotEffect > light.spotCosCutoff) {

                        float spotCut = radians(clamp(light.spotCutoff, 0.0, 89.9));
                        float spotInner = clamp(spotCut - ((light.spotExponent/128.0) * HALF_PI), 0.0, spotCut-0.001);

                        attenuation = smoothstep(spotCut, spotInner, acos(spotEffect));
                        if( light.radius > 0.0 ) {
                                float distanceSquare = dot(posToLight, posToLight);
                                attenuation *= GetDistanceAttenuation( distanceSquare, light.radius );
                        } else {
                                // from phong.frag
                                float d = length(posToLight);
                                attenuation *= spotEffect / (light.constantAttenuation + light.linearAttenuation * d + light.quadraticAttenuation * d * d);
                        }
                        attenuation = saturate(NoL * attenuation);

#ifdef HAS_SHADOWS
                        if( shadows[aLightIndex].enabled > 0.5 ) {
                                shadow = SpotShadow( shadows[aLightIndex], adata.positionWorld, adata.normalWorld );
//                              shadow *= attenuation;
                                shadow *= shadows[aLightIndex].strength;
                        }
#endif

                } else {
                        attenuation = 0.0;
                }
        }else{
                // area light
                // TODO: Implement
                attenuation = NoL;

                #ifdef HAS_SHADOWS
                        if( shadows[aLightIndex].enabled > 0.5 ) {
                                shadow = AreaShadow( shadows[aLightIndex], adata.positionWorld, adata.normalWorld, vec2(light.width, light.height) );
                                shadow *= shadows[aLightIndex].strength;
                        }
                #endif
        }

        vec3 radiance = gamma2Linear(light.diffuse.rgb) * ( light.diffuse.w * amat.ao * (1.0-shadow));

#if defined(HAS_CLEAR_COAT)
        if (amat.clearCoat > 0.0) {

                vec3 h = normalize( adata.viewDirectionWorld + light.position.xyz );
                float clearNoH = NoH;
                vec3 cnormal = adata.normalWorldGeometry;
                #if defined(HAS_TEX_CLEARCOAT_NORMAL)
                //float clearCoatNoV = clampNoV(dot(adata.clearcoatNormal, adata.viewDirectionWorld));
//              // the noh needs to recalc because it is different than the geometry
                cnormal = adata.clearcoatNormal;
                #endif

                clearNoH = saturate(dot(cnormal, h));

                float clearCoatNoL = saturate(dot(cnormal, light.position.xyz));

                // SurfaceClearCoat(in PbrData adata, in Material amat, const vec3 h, float NoH, float LoH )
                vec2 clearCoatFactor = SurfaceClearCoat(adata, amat, h, clearNoH, LoH );
                // vec2 clearCoatFactor = SurfaceClearCoat(adata, amat, clearNoH, LoH );
                float Fcc = clearCoatFactor.y;
                float clearCoat = clearCoatFactor.x;

                // account for energy loss in the base layer
                float cattenuation = 1.0 - Fcc;
                // Fd = ((Fd * cattenuation) + clearSpec);
                Fd *= cattenuation;// * attenuation;
                Fr += clearCoat * clearCoatNoL;
                // Fd *= cattenuation * attenuation;
                // Fr += (Fd + (Fr * cattenuation)*cattenuation) + (clearSpec);// * (specAO*Fcc);
        }
#endif
        radiance *= attenuation;
        adata.directDiffuse += Fd * radiance;
        adata.directSpecular += Fr * radiance;
}


#ifndef HAS_SHADOWS
void calcFakeDirectionalLight(in vec3 adir, inout PbrData adata, in Material amat ) {
        PbrLightData dlight;
        dlight.enabled = 1.0;
        dlight.type = 1.0;
        dlight.position = vec4(adir,1.0);
        dlight.diffuse = vec4(1.0);
        calcLight(dlight, 0, adata, amat );
}
#endif



#endif




#ifndef FUNCTIONS_ENV_LIGHT
#define FUNCTIONS_ENV_LIGHT

//#ifndef ENV_MAP_MAX_MIPS
//#define ENV_MAP_MAX_MIPS 5.0
//#endif

// Image Based Lighting
#ifdef HAS_TEX_ENV_IRRADIANCE
uniform samplerCube tex_irradianceMap;
#endif

#ifdef HAS_TEX_ENV_PRE_FILTER
uniform samplerCube tex_prefilterEnvMap;
#endif

#ifdef HAS_TEX_ENV_BRDF_LUT
uniform sampler2D tex_brdfLUT;
#endif

uniform float uCubeMapEnabled;
uniform float uEnvMapMaxMips; // 5.0


// ----------------------------------------------------------------------------
vec3 fakeEnvLightingDiffuse( in vec3 aRefl ) {
        vec3 R = abs(aRefl);//abs(adata.reflectionWorld);
        float cc = max( 0.0, abs( dot(R, vec3(0.0,0.5,0.5) )));
        return vec3(clamp(cc*0.25+0.6,0.0,1.0));
}

// ----------------------------------------------------------------------------
vec3 fakeEnvLightingSpecular( in vec3 aRefl, in float aroughness ) {
        //      vec3 R = abs(adata.reflectionWorld);
        vec3 R = abs(aRefl);
        float mm = (aroughness * aroughness);
        float ss = (1.0-mm*0.95);
        ss *= ss;
        ss *= ss;
        ss = ss * 122.0;
        float cc = max( 0.0, abs( dot(R, vec3(0.0,0.5,0.5)*1.43 )));
        cc = pow(cc+0.005, ss+0.05);
        return vec3(clamp(cc,0.0,1.0));
}


vec3 getIndirectPrefilteredReflection( in vec3 aR, in vec3 aN, in float aroughness ) {
        vec3 indirectSpecularRadiance = vec3(0.0);
        //      float lod = pow(aroughness, 2.0) * (ENV_MAP_MAX_MIPS-1);
#if defined(HAS_TEX_ENV_PRE_FILTER)
        float lod = aroughness * (uEnvMapMaxMips-1.0);
        //      indirectSpecularRadiance = textureLod(tex_prefilterEnvMap, aR, aroughness * (ENV_MAP_MAX_MIPS-1) ).rgb;
        vec3 ai = textureLod(tex_prefilterEnvMap, aR, floor(lod) ).rgb;
        vec3 ab = textureLod(tex_prefilterEnvMap, aR, clamp(ceil(lod), floor(lod), uEnvMapMaxMips-1.0) ).rgb;
        indirectSpecularRadiance = mix(ai, ab, lod-floor(lod) );
        //indirectSpecularRadiance = indirectSpecularRadiance / (indirectSpecularRadiance+vec3(1.0));
//      float horizon = min(1.0 + dot(adata.reflectionWorld, adata.normalWorld), 1.0);
        float horizon = min(1.0 + dot(aR, aN), 1.0);
        indirectSpecularRadiance *= horizon * horizon;
#else
        #ifdef HAS_CUBE_MAP
                indirectSpecularRadiance = fakeEnvLightingSpecular(aR, aroughness);
        #endif
#endif
        return indirectSpecularRadiance;
}

// https://google.github.io/filament/Filament.md.html#materialsystem/clearcoatmodel
void evaluateClearCoatIBL(const PbrData adata, const Material amat, inout vec3 Fd, inout vec3 Fr) {
#if defined(HAS_CLEAR_COAT)
        if (amat.clearCoat > 0.0) {
                // We want to use the geometric normal for the clear coat layer
                #if defined(HAS_TEX_CLEARCOAT_NORMAL)
                // the noh needs to recalc because it is different than the geometry
                vec3 clearNormal = adata.clearcoatNormal;
                #else
                vec3 clearNormal = adata.normalWorldGeometry;
                #endif

                float clearCoatNoV = clampNoV(dot(clearNormal, adata.viewDirectionWorld));
                vec3 clearCoatR = reflect(-adata.viewDirectionWorld, clearNormal);

                // The clear coat layer assumes an IOR of 1.5 (4% reflectance)
                float Fc = F_Schlick(0.04, 1.0, clearCoatNoV) * amat.clearCoat;
                float attenuation = 1.0 - Fc;
                Fd *= attenuation;
//              Fr *= (attenuation);

//              float specAO = horizon * horizon * computeSpecularAO(clearCoatNoV, amat.ao, amat.clearCoatRoughness );
                float specAO = computeSpecularAO(clearCoatNoV, amat.ao, amat.clearCoatRoughness );
                vec3 indirectSpecular = getIndirectPrefilteredReflection(clearCoatR, clearNormal, amat.clearCoatRoughness*amat.clearCoatRoughness);
                //Fr += getIndirectPrefilteredReflection(clearCoatR, clearNormal, amat.clearCoatRoughness) * (specAO * Fc);
                #if defined(HAS_TEX_ENV_PRE_FILTER)
                        indirectSpecular = indirectSpecular / (indirectSpecular+vec3(1.0));
                #endif
                Fr += indirectSpecular * adata.energyCompensation * (Fc * specAO);
        }
#endif
}

// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
        return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void calcEnvironmentIndirect( inout PbrData adata, in Material amat ) {
#ifdef HAS_CUBE_MAP
        if( uCubeMapEnabled < 0.5 ) {
                return;
        }
#endif
        // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
        vec3 indirectSpecularRadiance = getIndirectPrefilteredReflection( adata.reflectionWorld, adata.normalWorld, amat.roughness * amat.roughness );

#if defined(HAS_TEX_ENV_PRE_FILTER)
        // HDR Tone mapping
        // TODO: account for non-hdr images
        indirectSpecularRadiance = indirectSpecularRadiance / (indirectSpecularRadiance+vec3(1.0));
#endif

        // adata.dfg and adata.energyCompensation are set in pbrData.glsl :: setupPbrData()
        vec3 Fr = indirectSpecularRadiance * adata.dfg * adata.energyCompensation;
        float specAO = computeSpecularAO(adata.NoV, amat.ao, amat.roughness);
        Fr *= specAO;

        vec3 Fd = adata.diffuse;// amat.albedo.rgb * (1.0-amat.metallic);//vec3(0.0);
        vec3 irradiance = vec3(0.0);
        // TODO: implement spherical harmonics if tex_irradianceMap not present
#ifdef HAS_TEX_ENV_IRRADIANCE
        irradiance = texture(tex_irradianceMap, adata.normalWorld).rgb;
        // HDR Tone mapping
        irradiance = irradiance / (irradiance + vec3(1.0));
        Fd *= irradiance;// * (1.0);
//      vec3 Fd = adata.diffuse * irradiance * (1.0 - E) * diffuseBRDF;
#else
        irradiance = fakeEnvLightingDiffuse(adata.reflectionWorld);
        Fd *= irradiance;
#endif
        Fd *= amat.ao;
        Fd *= (1.0-adata.dfg);

        evaluateClearCoatIBL(adata, amat, Fd, Fr);

        adata.indirectDiffuse += (Fd * adata.iblLuminance);
        adata.indirectSpecular += (Fr * adata.iblLuminance);
}

#endif



vec4 postFragment(vec4 localColor){ return localColor; }

vec4 ofApplyPbr(in Material amat, in PbrData apbrData ) {
        // calculate the IBL
        #ifdef HAS_CUBE_MAP
        calcEnvironmentIndirect( apbrData, amat );
        #endif

        #if defined(MAX_LIGHTS) && MAX_LIGHTS
        // now lets add in all of the lights //
        for( int i = 0; i < MAX_LIGHTS; i++ ){
                if(lights[i].enabled<0.5) continue;
                // function calculates all the surface lighting for each light type, including shadows
                calcLight(lights[i], i, apbrData, amat);
        }
        #endif

        #ifndef HAS_CUBE_MAP
                #if defined(MAX_LIGHTS) && !MAX_LIGHTS
                        // we have no lighting in the scene //
                        // lets add some so we can see what's going on //
                        #ifndef HAS_SHADOWS
                        calcFakeDirectionalLight(uCameraPos, apbrData, amat );
                        #endif
                #endif
        #endif

        vec3 totalDiffuse = apbrData.directDiffuse + apbrData.indirectDiffuse;
        vec3 totalSpecular = apbrData.directSpecular + apbrData.indirectSpecular;

        // now lets add in the global lighting //
        vec4 color  = vec4(0.0);
        color.rgb += totalDiffuse;
        color.rgb += totalSpecular;
        color.a = saturate(amat.albedo.a);

        color.rgb += saturate( amat.emissive.rgb * (amat.emissive.a * color.a));

        return linear2Gamma(color);
}

vec4 ofApplyPbr(in Material amat) {
        // make sure to setup material first //
        PbrData pbrData;
        setupPbrData( pbrData, amat, v_worldPosition, uCameraPos );
        return ofApplyPbr( amat, pbrData);
}

vec4 ofApplyPbr() {
        Material mat;
        mat.uv = v_texcoord;
        setupMaterial( mat );
        return ofApplyPbr(mat);
}


void main() {
        vec4 color = ofApplyPbr();
        FRAG_COLOR = saturate(postFragment(color));
}