/******************************************************************************
 * This is a really simple fragment shader that simply sets the output fragment
 * color to yellow.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

in vec3 shaderColor;
in vec2 texCoord;
uniform sampler2D texture1;
out vec4 fragmentColor;

void main()
{
    fragmentColor = texture(texture1, texCoord);
}
