/******************************************************************************
 * This is a really simple fragment shader that simply sets the output fragment
 * color to yellow.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

in vec3 shaderColor;
in vec2 shaderTexCoord;
in float shaderType;
uniform sampler2D shaderTexture;
uniform int offset; // integer offset to shift starting palette index
uniform float time; // time in seconds
uniform float speed; // speed multiplier for transitions
out vec4 fragmentColor;

const int PALETTE_SIZE = 9;
const vec3 palette[PALETTE_SIZE] = vec3[](
    vec3(1.0, 0.0, 0.0), // red
    vec3(1.0, 0.5, 0.0), // orange
    vec3(1.0, 1.0, 0.0), // yellow
    vec3(0.0, 0.5, 0.0), // dark green
    vec3(0.65, 0.80, 0.20), // yellow-green
    vec3(1.0, 0.0, 1.0), // magenta
    vec3(0.0, 1.0, 1.0), // cyan
    vec3(0.29, 0.00, 0.51), // indigo/purple
    vec3(0.58, 0.29, 0.70)  // grape/purple
);

void main() {
    // per-group base index (shifted by integer offset)
    int id = int(round(shaderType));
    float base = float(id + offset);

    // continuous rotating position along the palette
    float pos = base + time * speed;
    // wrap within palette size
    float wrapped = mod(pos, float(PALETTE_SIZE));

    int a = int(floor(wrapped));
    int b = (a + 1) % PALETTE_SIZE;
    float f = fract(wrapped);

    vec3 ca = palette[a];
    vec3 cb = palette[b];
    vec3 color = mix(ca, cb, f);

    fragmentColor = vec4(color, 1.0) * texture(shaderTexture, shaderTexCoord);
}
