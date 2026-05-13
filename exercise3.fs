/******************************************************************************
 * This is a really simple fragment shader that simply sets the output fragment
 * color to yellow.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

in vec3 worldSpacePosition;
in vec3 worldSpaceNormal;
in vec3 objectColor;
in vec2 texCoord;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 cameraPosition;
uniform float specularity;
uniform sampler2D texture1;
out vec4 fragmentColor;

void main()
{
    vec3 lightvec = normalize(lightPosition - worldSpacePosition);
    vec3 normlightvec = normalize(worldSpaceNormal);

    float diffuser = max((dot(normlightvec, lightvec)), 0.0f);
    float ambience = 0.3f;

    vec3 sureye = normalize(cameraPosition - worldSpacePosition);
    vec3 reflect = reflect(-lightvec, normlightvec);

    vec3 specLight = pow(max(dot(reflect, sureye), 0.0f), specularity) * lightColor;

    vec4 texColor = texture(texture1, texCoord);
    vec3 finalColor = (diffuser + ambience) * texColor.rgb * lightColor + specLight;
    fragmentColor = vec4(finalColor, 1.0f);
}
