/******************************************************************************
 * This is a vertex shader that assumes that each vertex has a position and
 * color. The color of the vertex is further manipulated via a uniform, and
 * this color is passed to the fragment shader as an output.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in float vertexType;
out vec3 shaderColor;
uniform float glow;
uniform float time;
const float DOUBLEPI = 6.2832f;

void main()
{
    gl_Position = vec4((vertexPosition.x), (vertexPosition.y), (vertexPosition.z), 1.0f);

    if (vertexType > 0.5 && vertexType < 1.5) {

        float grad = (vertexPosition.x + 1.0) / 2.0;
        float phase = grad + (time * 5);

        shaderColor = vec3(
            sin(phase),
            cos(phase),
            cos(phase)
        );

    } else if (vertexType > 1.5) {
        
        float grad = (vertexPosition.x + 1.0) * 0.5;
        float rotateSpeed = 0.5;
        float rotateAngle = time * rotateSpeed * DOUBLEPI + grad * DOUBLEPI;

        shaderColor = vec3(
            cos(rotateAngle + 9.111),
            sin(rotateAngle + 6.222),
            sin(rotateAngle + 1.222)
        );
    
    } else {
        shaderColor = vertexColor;
    }

}
