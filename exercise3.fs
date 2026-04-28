/******************************************************************************
 * This fragment shader applies basic diffuse lighting to the model.
 * It receives world-space position, world-space normal, and object color from
 * the vertex shader.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

in vec3 worldSpacePosition;
in vec3 worldSpaceNormal;
in vec3 objectColor;
in vec2 texCoord;

uniform sampler2D texture1;
out vec4 fragmentColor;

void main()
{
    // For now, just output the texture color blended with object color
    // Lighting calculations will be added in later parts of the exercise
    vec4 texColor = texture(texture1, texCoord);
    fragmentColor = texColor * vec4(objectColor, 1.0f);
}

