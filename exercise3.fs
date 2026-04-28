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

uniform vec3 lightPosition;
uniform sampler2D texture1;
out vec4 fragmentColor;

void main()
{
    vec3 l = normalize(lightPosition - worldSpacePosition);
    vec3 n = normalize(worldSpaceNormal);
    float Cd = max((dot(n, l)), 0.0f);
    float Ca = 0.3f;
    vec4 texColor = texture(texture1, texCoord);
    vec3 finalColor = (Cd + Ca) * objectColor * texColor.rgb;
    fragmentColor = vec4(finalColor, 1.0f);
}
