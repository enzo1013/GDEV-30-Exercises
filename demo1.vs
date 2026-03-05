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
uniform vec2 pivotPoints;

out vec3 shaderColor;
out vec2 shaderTexCoord;
out float shaderType;

void main()
{
    vec2 positions = vertexPosition.xy;

    if (vertexType > -1 && vertexType < 4) {
        positions -= pivotPoints;

        float cosAng = cos(time) * sin(time);
        float sinAng = sin(time);
        mat2 transformMatrix = mat2(cosAng, -sinAng, -sinAng, cosAng);

        positions = positions * transformMatrix;

        positions += pivotPoints;
    } else if (vertexType > 3) {
        positions -= pivotPoints;

        float cosAng = cos(time);
        float sinAng = sin(time) * cos(time);
        mat2 transformMatrix = mat2(cosAng, sinAng, -sinAng, -cosAng);

        positions = positions * transformMatrix;

        positions += pivotPoints;
    }

    gl_Position = vec4(positions, vertexPosition.z, 1.0f);

        shaderColor = vertexColor;
        shaderTexCoord = vertexTexCoord;
        shaderType = vertexType;

}
