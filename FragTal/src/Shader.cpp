#include "Shader.h"

// Shader from: https://www.shadertoy.com/view/lcsXRM

const float FOV = 90.f;

glm::vec3 palette( float t) {
    glm::vec3 a = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 b = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 c = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 d = glm::vec3(0.263f, 0.416f, 0.557f);

    return a + b*glm::cos( 6.28318f*(c*t*d) );
}


float apollonianSDF( glm::vec3 p ) {
    float width = 1.5f;  // 1.5f
    float s = 3.0f, e;
    for ( int i = 0; i++ < 15; ) {
        p = glm::mod(p - 1.0f, 2.0f ) - 1.0f;
        s *= e = width / glm::dot( p, p );
        p *= e;
    }
    
    glm::vec2 p2 = glm::vec2(p.y, p.z);
    
    return glm::length( p2 ) / s;
}


// Calculate the Signed Distance Field for the scene

glm::vec2 map(glm::vec3 point) {
    return glm::vec2(apollonianSDF(point), 1.);
}


glm::vec4 rayMarch(glm::vec3 point, glm::vec3 direction) {
    glm::vec2 signedDistance;
    glm::vec4 result;
    
    const int maxMarchingSteps = 2555;  // Maximum number of times ray is marched until loop breaks
    const float maxDistance = 1000.0f;  // Maximum distance to object for a miss
    const float minDistance = 0.00005f;  // Minimum distance to object for a hit
    
    glm::vec3 rayPosition = point;
    
    for (int i = 0; i < maxMarchingSteps; i++)  // March the ray forwards  
    {        
        // Distance to nearest object
        glm::vec2 signedDistance = map(rayPosition);

        rayPosition += direction * signedDistance.x;  // Move the ray
        
        result = glm::vec4(rayPosition, 0.0f);
        if (signedDistance.x > maxDistance) break;  // If the ray misses
        if (signedDistance.x < minDistance) {
            result = glm::vec4(rayPosition, signedDistance.y);
            break;  // If the ray hits something
            }
    }
    
    return result;
}


glm::vec3 calculateNormal(glm::vec3 position) {
    const float EPSILON = 0.001f;
    
    glm::vec3 v1 = glm::vec3(
        map(position + glm::vec3(EPSILON, 0.0f, 0.0f)).x,
        map(position + glm::vec3(0.0f, EPSILON, 0.0f)).x,
        map(position + glm::vec3(0.0f, 0.0f, EPSILON)).x);
    glm::vec3 v2 = glm::vec3(
        map(position - glm::vec3(EPSILON, 0.0f, 0.0f)).x,
        map(position - glm::vec3(0.0f, EPSILON, 0.0f)).x,
        map(position - glm::vec3(0.0f, 0.0f, EPSILON)).x);
    
    return normalize(v1 - v2);
        
}

// Default colour for a miss
glm::vec3 col = glm::vec3(0.f, 0.f, 0.f);
GLM::vec4 mainImage(glm::vec2 fragCoord, glm::vec2 iResolution, float iTime) {


    // Normalized pixel coordinates (from 0 to 1)
    // Shifted so (0, 0) is the centre rather than bottom-left
    glm::vec2 shiftedCoord = fragCoord - (iResolution / 2.0f);
    glm::vec2 uv = shiftedCoord / iResolution.y;   
    
    glm::vec3 rayPosition = glm::vec3(uv.x, uv.y, 0.0);
    glm::vec3 rayDirection = glm::vec3(0.0, 0.0, 1.0);
    glm::vec2 rayDir = uv * glm::atan(glm::radians(FOV));
    rayDirection.x = rayDir.x;
    rayDirection.y = rayDir.y;
    rayDirection = normalize(rayDirection);
    
    rayPosition += glm::vec3(1, 1, iTime); // 3.789f
    
    glm::vec4 hitPosID = rayMarch(rayPosition, rayDirection);
    
    glm::vec3 hitPosID3 = glm::vec3(hitPosID.x, hitPosID.y, hitPosID.z);
    
    glm::vec3 hitPosition = hitPosID3;
    float objectID = hitPosID.w;
    
    // If the ray hit, calculate lighting
    if (objectID != 0.0) {
        col = palette(glm::length(hitPosition));
    }
    
    // Output to screen
    glm::vec4 fragColor = glm::vec4(col * 255.0f, 1.0f * 255.0f);
    
    return fragColor;
}






