#include "Shader.h"

glm::vec3 FragTal(glm::vec2 fragCoord, glm::vec2 iResolution) {
    glm::vec2 uv = 2.5f * (fragCoord / iResolution - 0.5f);
    uv.x *= iResolution.x / iResolution.y;

    glm::vec2 c = uv;
    glm::vec2 z(0.0f, 0.0f);
    int maxIterations = 100;
    int i;

    for (i = 0; i < maxIterations && glm::length(z) <= 2.0f; ++i) {
        float x = z.x * z.x - z.y * z.y + c.x;
        float y = 2.0f * z.x * z.y + c.y;
        z = glm::vec2(x, y);
    }

    float color = static_cast<float>(i) / maxIterations;
    return glm::vec3(color);
}

GLM::vec4 mainImage(glm::vec2 fragCoord, glm::vec2 iResolution, float iTime) {

    glm::vec2 uv = fragCoord / iResolution;
    uv.x *= iResolution.x / iResolution.y;

    glm::vec3 uv3(uv.x, uv.y, uv.x);
    glm::vec3 col = 0.5f + 0.5f * glm::cos(iTime + uv3) * glm::cos(iTime / glm::vec3(1.2f, 1.5f, 2.0f));

    glm::vec4 fragColor = glm::vec4((col + FragTal(fragCoord, iResolution) * iTime) * 255.0f, 1.0f * 255.0f);
    //glm::vec4 fragColor = glm::vec4((FragTal(fragCoord, iResolution) * iTime) * 255.0f, 1.0f * 255.0f);
    //glm::vec4 fragColor = glm::vec4(FragTal(fragCoord, iResolution) * 255.0f, 1.0f * 255.0f);
    
    //glm::vec4 fragColor = glm::vec4(1.0f*255.0f);
    return fragColor;
}

