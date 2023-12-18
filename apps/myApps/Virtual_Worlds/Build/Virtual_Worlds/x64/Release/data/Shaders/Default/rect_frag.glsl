#version 150

#define IN in
#define OUT out
#define TEXTURE texture
#define FRAG_COLOR fragColor
out vec4 fragColor;
#define SAMPLER sampler2DRect
#define MAX_LIGHTS 2
#define HAS_TEXTURE 1
#define HAS_COLOR 0
#define HAS_SHADOWS 1
#define SHADOWS_USE_CUBE_MAP_ARRAY 1
#define SHADOWS_USE_TEXTURE_ARRAY 1

    IN vec2 v_texcoord; // phong receive the texCoord
    IN vec3 v_normal;
    IN vec3 v_transformedNormal;
    // Eye-coordinate position of vertex
    IN vec3 v_eyePosition;
    IN vec3 v_worldPosition;
        IN vec3 v_worldNormal;
#if HAS_COLOR
    IN vec4 v_color;
#endif
        uniform mat4 viewMatrix;


    struct lightData
    {
        float enabled;
        vec4 ambient;
        float type; // 0 = pointlight 1 = directionlight
        vec4 position; // where are we
        vec4 diffuse; // how diffuse
        vec4 specular; // what kinda specular stuff we got going on?
        // attenuation
        float constantAttenuation;
        float linearAttenuation;
        float quadraticAttenuation;
        // only for spot
        float spotCutoff;
        float spotCosCutoff;
        float spotExponent;
        // spot and area
        vec3 spotDirection;
        // only for directional
        vec3 halfVector;
        // only for area
        float width;
        float height;
        vec3 right;
        vec3 up;
    };

    uniform SAMPLER tex0;

    uniform vec4 mat_ambient;
    uniform vec4 mat_diffuse;
    uniform vec4 mat_specular;
    uniform vec4 mat_emissive;
    uniform float mat_shininess;
    uniform vec4 global_ambient;

    // these are passed in from OF programmable renderer
    uniform mat4 modelViewMatrix;
    uniform mat4 projectionMatrix;
    uniform mat4 textureMatrix;
    uniform mat4 modelViewProjectionMatrix;

#if defined(MAX_LIGHTS) && MAX_LIGHTS
    uniform lightData lights[MAX_LIGHTS];
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




        #ifdef HAS_TEX_NORMAL
        //-- normal map code from ------------------------------------ //
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
        //-- end normal map code ------------------------------------ //
        #endif



    void pointLight( in lightData light, in vec3 normal, in vec3 ecPosition3, in float shadow, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular ){
        float nDotVP;       // normal . light direction
        float nDotHV;       // normal . light half vector
        float pf;           // power factor
        float attenuation;  // computed attenuation factor
        float d;            // distance from surface to light source
        vec3  VP;           // direction from surface to light position
        vec3  halfVector;   // direction of maximum highlights
        vec3 eye = -normalize(v_eyePosition);

        // Compute vector from surface to light position
        VP = vec3 (light.position.xyz) - ecPosition3;

        // Compute distance between surface and light position
        d = length(VP);


        // Compute attenuation
        attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * d + light.quadraticAttenuation * d * d);

        // Normalize the vector from surface to light position
        VP = normalize(VP);
        halfVector = normalize(VP + eye);
        nDotHV = max(0.0, dot(normal, halfVector));
        nDotVP = max(0.0, dot(normal, VP));

        ambient += light.ambient.rgb * attenuation;
        diffuse += light.diffuse.rgb * shadow * nDotVP * attenuation;
#ifndef TARGET_OPENGLES
#define SPECULAR_REFLECTION
#endif
//#ifndef SPECULAR_REFLECTION
//        // ha! no branching :)
//        pf = mix(0.0, pow(nDotHV, mat_shininess), step(0.0000001, nDotVP));
//        specular += light.specular.rgb * shadow * pf * nDotVP * attenuation;
//#else
        // fresnel factor
        // http://en.wikibooks.org/wiki/GLSL_Programming/Unity/Specular_Highlights_at_Silhouettes
        float w = pow(1.0 - max(0.0, dot(halfVector, VP)), 5.0);
        vec3 specularReflection = attenuation * vec3(light.specular.rgb)
          * mix(vec3(mat_specular.rgb), vec3(1.0), w)
          * pow(nDotHV, mat_shininess);
        specular += shadow * mix(vec3(0.0), specularReflection, step(0.0000001, nDotVP));
//#endif
    }

        void pointLight( in lightData light, in vec3 normal, in vec3 ecPosition3, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular ){
                float tempShadow = 1.0;
                pointLight( light, normal, ecPosition3, tempShadow, ambient, diffuse, specular );
        }

    void directionalLight(in lightData light, in vec3 normal, in float shadow, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular){
        float nDotVP;         // normal . light direction
        float nDotHV;         // normal . light half vector
        float pf;             // power factor

        nDotVP = max(0.0, dot(normal, normalize(vec3(light.position))));
        nDotHV = max(0.0, dot(normal, light.halfVector));

        pf = mix(0.0, pow(nDotHV, mat_shininess), step(0.0000001, nDotVP));

        ambient += light.ambient.rgb;
        diffuse += light.diffuse.rgb * nDotVP * shadow;
        specular += light.specular.rgb * pf * nDotVP * shadow;
    }

        void directionalLight(in lightData light, in vec3 normal, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular){
                directionalLight(light, normal, 1.0, ambient, diffuse, specular);
        }

    void spotLight(in lightData light, in vec3 normal, in vec3 ecPosition3, in float shadow, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular){
        float nDotVP; // = max(dot(normal,normalize(vec3(light.position))),0.0);
        float nDotHV;       // normal . light half vector
        float pf;
        float d;            // distance from surface to light source
        vec3  VP;           // direction from surface to light position
        vec3 eye = -normalize(v_eyePosition);
        float spotEffect;
        float attenuation=1.0;
        vec3  halfVector;   // direction of maximum highlights
        // Compute vector from surface to light position
        VP = light.position.xyz - ecPosition3;
        spotEffect = dot(light.spotDirection, -normalize(VP));

        if (spotEffect > light.spotCosCutoff) {
            // Compute distance between surface and light position
            d = length(VP);
            spotEffect = pow(spotEffect, light.spotExponent);
            attenuation = spotEffect / (light.constantAttenuation + light.linearAttenuation * d + light.quadraticAttenuation * d * d);

            VP = normalize(VP);
            halfVector = normalize(VP + eye);
            nDotHV = max(0.0, dot(normal, halfVector));
            nDotVP = max(0.0, dot(normal, VP));

            pf = mix(0.0, pow(nDotHV, mat_shininess), step(0.0000001, nDotVP));

            diffuse += light.diffuse.rgb * shadow * nDotVP * attenuation;
            specular += light.specular.rgb * shadow * pf * nDotVP * attenuation;

        }

        ambient += light.ambient.rgb * attenuation;

    }

        void spotLight(in lightData light, in vec3 normal, in vec3 ecPosition3, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular) {
                spotLight(light, normal, ecPosition3, 1.0, ambient, diffuse, specular);
        }


    vec3 projectOnPlane(in vec3 point, in vec3 planeCenter, in vec3 planeNormal){
        return point - dot( point - planeCenter, planeNormal ) * planeNormal;
    }

    vec3 linePlaneIntersect(in vec3 lp, in vec3 lv, in vec3 pc, in vec3 pn){
       return lp+lv*(dot(pn,pc-lp)/dot(pn,lv));
    }

    void areaLight(in lightData light, in vec3 N, in vec3 V, float shadow, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular){
        vec3 right = light.right;
        vec3 pnormal = light.spotDirection;
        vec3 up = light.up;

        //width and height of the area light:
        float width = light.width*0.5;
        float height = light.height*0.5;
        float attenuation;

        //project onto plane and calculate direction from center to the projection.
        vec3 projection = projectOnPlane(V,light.position.xyz,pnormal);// projection in plane
        vec3 dir = projection-light.position.xyz;

        //calculate distance from area:
        vec2 diagonal = vec2(dot(dir,right),dot(dir,up));
        vec2 nearest2D = vec2(clamp( diagonal.x,-width,width  ),clamp(  diagonal.y,-height,height));
        vec3 nearestPointInside = vec3(light.position.xyz)+(right*nearest2D.x+up*nearest2D.y);
        float dist = distance(V,nearestPointInside);//real distance to area rectangle

        vec3 lightDir = normalize(nearestPointInside - V);
        attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * dist + light.quadraticAttenuation * dist * dist);

        float NdotL = max( dot( pnormal, -lightDir ), 0.0 );
        float NdotL2 = max( dot( N, lightDir ), 0.0 );
        if ( NdotL * NdotL2 > 0.0 ) {
            float diffuseFactor = sqrt( NdotL * NdotL2 );
            vec3 R = reflect( normalize( -V ), N );
            vec3 E = linePlaneIntersect( V, R, light.position.xyz, pnormal );
            float specAngle = dot( R, pnormal );
            if (specAngle > 0.0){
                vec3 dirSpec = E - light.position.xyz;
                vec2 dirSpec2D = vec2(dot(dirSpec,right),dot(dirSpec,up));
                vec2 nearestSpec2D = vec2(clamp( dirSpec2D.x,-width,width  ),clamp(  dirSpec2D.y,-height,height));
                float specFactor = 1.0-clamp(length(nearestSpec2D-dirSpec2D) * 0.05 * mat_shininess,0.0,1.0);
                specular += light.specular.rgb * specFactor * specAngle * diffuseFactor * attenuation * shadow;
            }
            diffuse  += light.diffuse.rgb * shadow * diffuseFactor * attenuation;
        }
        ambient += light.ambient.rgb * attenuation;
    }

        void areaLight(in lightData light, in vec3 N, in vec3 V, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular) {
                areaLight(light, N, V, 1.0, ambient, diffuse, specular);
        }

#ifdef HAS_SHADOWS
float SpotShadow(in lightData light, in vec3 ecPosition3, in shadowData aShadowData, in vec3 aWorldFragPos, in vec3 aWorldNormal) {

        vec3 VP = light.position.xyz - ecPosition3;
        float spotEffect = dot(light.spotDirection, -normalize(VP));

        if (spotEffect < light.spotCosCutoff) {
                return 0.0;
        }

        float shadowStrength = 1.0;
        if( light.spotExponent > 0.0 ) {
                float d = length(VP);
                spotEffect = pow(spotEffect, light.spotExponent);
                float attenuation = spotEffect / (light.constantAttenuation + light.linearAttenuation * d + light.quadraticAttenuation * d * d);
                shadowStrength = attenuation;
        }

        return shadowStrength * SpotShadow( aShadowData, aWorldFragPos, aWorldNormal);
}
#endif


    vec4 postFragment(vec4 localColor){ return localColor; }

    //////////////////////////////////////////////////////
    // here's the main method
    //////////////////////////////////////////////////////


    void main (void){

        vec3 ambient = global_ambient.rgb;
        vec3 diffuse = vec3(0.0,0.0,0.0);
        vec3 specular = vec3(0.0,0.0,0.0);

        vec3 tNormal = v_transformedNormal;
        #ifdef HAS_TEX_NORMAL
                tNormal = PerturbNormal(TEXTURE(tex_normal, v_texcoord).rgb, N, V, v_texcoord);
        #endif

        vec3 transformedNormal = normalize(tNormal);

                #if defined(HAS_SHADOWS)
                vec3 worldNormalN = normalize(v_worldNormal);
                #endif

                #if defined(MAX_LIGHTS) && MAX_LIGHTS
        for( int i = 0; i < MAX_LIGHTS; i++ ){
            if(lights[i].enabled<0.5) continue;
                        float shadow = 0.0;
            if(lights[i].type<0.5){
                                #ifdef HAS_SHADOWS
                                if( shadows[i].enabled > 0.5 ) {
                                        shadow = PointLightShadow(shadows[i], v_worldPosition, worldNormalN);
                                        shadow *= shadows[i].strength;
                                }
                                #endif
                pointLight(lights[i], transformedNormal, v_eyePosition, 1.0-shadow, ambient, diffuse, specular);
            }else if(lights[i].type<1.5){
                                #ifdef HAS_SHADOWS
                                if( shadows[i].enabled > 0.5 ) {
                                        shadow = DirectionalShadow(shadows[i], v_worldPosition, worldNormalN);
                                        shadow *= shadows[i].strength;
                                }
                                #endif
                directionalLight(lights[i], transformedNormal, 1.0-shadow, ambient, diffuse, specular);
            }else if(lights[i].type<2.5){
                                #ifdef HAS_SHADOWS
                                if( shadows[i].enabled > 0.5 ) {
                                        shadow = SpotShadow(lights[i], v_eyePosition, shadows[i], v_worldPosition, worldNormalN);
                                        shadow *= shadows[i].strength;
                                }
                                #endif
                spotLight(lights[i], transformedNormal, v_eyePosition, 1.0-shadow, ambient, diffuse, specular);
            }else{
                                #ifdef HAS_SHADOWS
                                if( shadows[i].enabled > 0.5 ) {
                                        shadow = AreaShadow( shadows[i], v_worldPosition, worldNormalN, vec2(lights[i].width, lights[i].height) );
                                        shadow *= shadows[i].strength;
                                }
                                #endif
                areaLight(lights[i], transformedNormal, v_eyePosition, 1.0-shadow, ambient, diffuse, specular);
            }
        }
                #else
                // there are no lights, so just set to fully lit, otherwise would show as black
                diffuse = vec3(1.0);
                #endif

        // apply emmisive texture
        vec4 mat_emissive_color = mat_emissive;
        #ifdef HAS_TEX_EMISSIVE
                mat_emissive_color *= TEXTURE(tex_emissive, v_texcoord);
        #endif

        // apply specular texture // these are mostly black and white
        #ifdef HAS_TEX_SPECULAR
                vec4 spec_value = TEXTURE(tex_specular, v_texcoord);
                specular *= spec_value.rgb; //apply the color
                specular *= spec_value.a; //also apply alpha which is sometimes used as a mask
        #endif

        // apply ambient texture // these are mostly black and white
        #ifdef HAS_TEX_AMBIENT
                vec4 ambient_value = TEXTURE(tex_ambient, v_texcoord);
                ambient *= ambient_value.rgb; //apply the color
        #endif

        ////////////////////////////////////////////////////////////
        // now add the material info
        #if HAS_TEXTURE && !HAS_COLOR
            vec4 tex = TEXTURE(tex0, v_texcoord);
                vec4 localColor = vec4(ambient,1.0) * tex + vec4(diffuse,1.0) * tex + vec4(specular,1.0) + mat_emissive_color;
                        localColor.a = tex.a; // allow for alpha in the texture
        #elif HAS_TEXTURE && HAS_COLOR
            vec4 tex = TEXTURE(tex0, v_texcoord);
            vec4 localColor = vec4(ambient,1.0) * tex * v_color + vec4(diffuse,1.0) * tex * v_color + vec4(specular,1.0) * mat_specular + mat_emissive_color;
                        localColor.a = tex.a*v_color.a; // allow for alpha in the texture or vertex
        #elif HAS_COLOR
            vec4 localColor = vec4(ambient,1.0) * v_color + vec4(diffuse,1.0) * v_color + vec4(specular,1.0) * mat_specular + mat_emissive_color;
        #else
                        vec4 localColor = vec4(ambient,1.0) * mat_ambient + vec4(diffuse,1.0) * mat_diffuse + vec4(specular,1.0) * mat_specular + mat_emissive_color;
        #endif





        #ifdef HAS_TEX_OCCLUSION
            float occlusioon = TEXTURE(tex_occlusion, v_texcoord).r;
            localColor.rgb *= occlusioon;
        #endif

        FRAG_COLOR = clamp( postFragment(localColor), 0.0, 1.0 );
    }
