/******************************************************************************
 * This is a really simple vertex shader that simply sets the output vertex's
 * position to be the same as the input.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in float face;

uniform mat4 matrix;
out vec3 shaderColor;
out vec2 texCoord;

void main()
{
    gl_Position = matrix * vec4(vertexPosition, 1.0f);
    shaderColor = vertexColor;
    if (face == 0.0)
    {
        texCoord = vec2(vertexPosition.x, vertexPosition.y);
    }
    else if (face == 1.0)
    {
        texCoord = vec2(vertexPosition.x, vertexPosition.z);
    }
}
