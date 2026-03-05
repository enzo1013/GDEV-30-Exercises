/******************************************************************************
 * This is a really simple vertex shader that simply sets the output vertex's
 * position to be the same as the input.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec2 vertexTexCoord;
layout (location = 3) in float vertexType;

uniform float time;

out vec3 shaderColor;
out vec2 shaderTexCoord;

void main()
{
    gl_Position = vec4(vertexPosition, 1.0f);

    float grad = (vertexPosition.x + 1.0) / 2.0;
    float phase = grad + time;

    if (int(vertexType) == 1) {
        shaderColor = vec3(
            sin(phase),
            cos(phase),
            cos(phase)
        );
    } else if (int(vertexType) == 2) {

        shaderColor = vec3(
            sin(phase) *4,
            cos(phase),
            cos(phase)
        );
    } else if (int(vertexType) == 0) {

        shaderColor = vec3(
            sin(phase),
            cos(phase)*4,
            cos(phase)
        );
    } else if (int(vertexType) == 4) {

        shaderColor = vec3(
            sin(phase)*-4,
            cos(phase),
            cos(phase)*4
        );
    } else {
        shaderColor = vertexColor;
    }

    shaderTexCoord = vertexTexCoord;
}
