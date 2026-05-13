/******************************************************************************
 * This is a really simple vertex shader that simply sets the output vertex's
 * position to be the same as the input.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in float face;

// Three matrices for lighting calculations
uniform mat4 projectionViewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

out vec3 worldSpacePosition;
out vec3 worldSpaceNormal;
// out vec3 objectColor;
out vec2 texCoord;

void main()
{
    // Calculate world-space position using model matrix
    worldSpacePosition = (modelMatrix * vec4(vertexPosition, 1.0f)).xyz;

    // Calculate world-space normal using normal matrix
    worldSpaceNormal = (normalMatrix * vec4(vertexNormal, 1.0f)).xyz;

    // objectColor = vertexColor;
    if (face == 0.0)
    {
        texCoord = vec2(vertexPosition.x, vertexPosition.y);
    }
    else if (face == 1.0)
    {
        texCoord = vec2(vertexPosition.x, vertexPosition.z);
    }
    gl_Position = projectionViewMatrix * vec4(worldSpacePosition, 1.0f);
}
