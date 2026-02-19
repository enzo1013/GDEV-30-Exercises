/******************************************************************************
 * This demo draws a somewhat familiar geometric object composed of three
 * triangles.
 *
 * The main differences between this demo and the previous demo are:
 * - Each vertex now defines two subcomponents: position and color.
 * - A uniform variable is introduced to the shader program, which is used to
 *   accomplish a glowing effect.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <gdev.h>

// change this to your desired window attributes
#define WINDOW_WIDTH  768
#define WINDOW_HEIGHT 768
#define WINDOW_TITLE  "GDEV 30 Exercise 1"
GLFWwindow *pWindow;

// define a vertex array to hold our vertices
float vertices[] =
{
    // position (x, y, z)    color (r, g, b)

    // main head
    -0.9816f, 0.7452f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.9223f, 0.8103f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.8505f, 0.6681f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.8327f, 0.8155f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.9223f, 0.8103f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.8505f, 0.6681f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.8505f, 0.6681f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.8327f, 0.8155f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7314f, 0.6680f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.7314f, 0.6680f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.8327f, 0.8155f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7082f, 0.8481f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.7082f, 0.8481f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6039f, 0.8049f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7314f, 0.6680f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.8505f, 0.6681f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7314f, 0.6680f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7640f, 0.5357f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.8505f, 0.6681f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.8618f, 0.5194f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7640f, 0.5357f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.8505f, 0.6681f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.8618f, 0.5194f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.9381f, 0.6157f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.9816f, 0.7452f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.8505f, 0.6681f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.9381f, 0.6157f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.9816f, 0.7452f, 0.0f, 1.0f, 1.0f, 1.0f,
    -1.0000f, 0.6140f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.9381f, 0.6157f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.9490f, 0.5096f, 0.0f, 1.0f, 1.0f, 1.0f,
    -1.0000f, 0.6140f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.9381f, 0.6157f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.9490f, 0.5096f, 0.0f, 1.0f, 1.0f, 1.0f,
    -1.0000f, 0.6140f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.9636f, 0.4899f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.7314f, 0.6680f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7640f, 0.5357f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6395f, 0.5618f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.7314f, 0.6680f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5208f, 0.7383f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6395f, 0.5618f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.7314f, 0.6680f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5208f, 0.7383f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6044f, 0.8043f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.5607f, 0.5705f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5208f, 0.7383f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6395f, 0.5618f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.9490f, 0.5096f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.8864f, 0.5437f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.9381f, 0.6157f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.9490f, 0.5096f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.8864f, 0.5437f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.8664f, 0.4248f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.9490f, 0.5096f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.9203f, 0.3762f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.8664f, 0.4248f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.8509f, 0.4744f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.8864f, 0.5437f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.8664f, 0.4248f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.8509f, 0.4744f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.8618f, 0.5194f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7640f, 0.5357f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.8509f, 0.4744f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6061f, 0.4571f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7640f, 0.5357f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.6395f, 0.5618f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6061f, 0.4571f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7640f, 0.5357f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.8509f, 0.4744f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6503f, 0.2574f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6061f, 0.4571f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.6395f, 0.5618f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5607f, 0.5705f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6061f, 0.4571f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.5607f, 0.5705f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6061f, 0.4571f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5007f, 0.4676f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.6503f, 0.2574f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6061f, 0.4571f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5007f, 0.4676f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.5607f, 0.5705f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5208f, 0.7383f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.4542f, 0.7250f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.5607f, 0.5705f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5007f, 0.4676f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.4542f, 0.7250f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.8509f, 0.4744f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7970f, 0.3218f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.8664f, 0.4248f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.8509f, 0.4744f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6503f, 0.2574f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7970f, 0.3218f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.8694f, 0.3432f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7970f, 0.3218f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.8664f, 0.4248f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.8694f, 0.3432f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7970f, 0.3218f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.8501f, 0.2615f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.7396f, 0.2227f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6503f, 0.2574f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7970f, 0.3218f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.7396f, 0.2227f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.8021f, 0.1833f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7970f, 0.3218f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.7035f, 0.2057f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6503f, 0.2574f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7396f, 0.2227f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.7035f, 0.2057f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6762f, 0.0134f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6503f, 0.2574f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.6665f, -0.1256f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6762f, 0.0134f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6503f, 0.2574f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.6665f, -0.1256f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6503f, 0.2574f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5607f, -0.2822f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.6665f, -0.1256f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6762f, 0.0134f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7687f, -0.0351f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.7282f, 0.0080f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6762f, 0.0134f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7687f, -0.0351f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.7282f, 0.0080f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7687f, -0.0351f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.8157f, 0.0572f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.6665f, -0.1256f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6512f, -0.2453f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5607f, -0.2822f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.6665f, -0.1256f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6512f, -0.2453f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7070f, -0.1220f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.7559f, -0.1550f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6512f, -0.2453f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7070f, -0.1220f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.7559f, -0.1550f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7923f, -0.0624f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.7070f, -0.1220f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.5639f, -0.3095f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6512f, -0.2453f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5607f, -0.2822f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.5639f, -0.3095f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6512f, -0.2453f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6584f, -0.2921f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.7494f, -0.2101f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6512f, -0.2453f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6584f, -0.2921f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.7494f, -0.2101f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6584f, -0.2921f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6441f, -0.3703f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.5583f, -0.3880f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6584f, -0.2921f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6441f, -0.3703f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.5583f, -0.3880f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6584f, -0.2921f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5639f, -0.3095f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.5639f, -0.3095f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.4377f, -0.3956f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5607f, -0.2822f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.5583f, -0.3880f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.4377f, -0.3956f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5607f, -0.2822f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.5583f, -0.3880f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.4377f, -0.3956f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.4053f, -0.5852f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.5583f, -0.3880f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6441f, -0.3703f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.4053f, -0.5852f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.4438f, -0.2991f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.4377f, -0.3956f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5607f, -0.2822f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.4438f, -0.2991f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.4377f, -0.3956f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3145f, -0.3576f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2111f, -0.4300f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.4377f, -0.3956f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3145f, -0.3576f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2111f, -0.4300f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.4377f, -0.3956f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.4026f, -0.5827f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.4438f, -0.2991f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5607f, -0.2822f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3789f, -0.2005f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.6503f, 0.2574f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5607f, -0.2822f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3789f, -0.2005f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.4438f, -0.2991f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3789f, -0.2005f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3145f, -0.3576f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2532f, -0.2202f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3789f, -0.2005f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3145f, -0.3576f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2532f, -0.2202f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2111f, -0.4300f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3145f, -0.3576f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2532f, -0.2202f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3789f, -0.2005f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3409f, -0.1447f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.3754f, -0.0610f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3789f, -0.2005f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3409f, -0.1447f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.3754f, -0.0610f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3789f, -0.2005f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6503f, 0.2574f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.3754f, -0.0610f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3556f, 0.0503f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6503f, 0.2574f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.3591f, 0.2279f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3556f, 0.0503f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6503f, 0.2574f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.3591f, 0.2279f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3587f, 0.3291f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6503f, 0.2574f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.4552f, 0.3494f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3587f, 0.3291f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6503f, 0.2574f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.4552f, 0.3494f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5007f, 0.4676f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.6503f, 0.2574f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.4552f, 0.3494f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5007f, 0.4676f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3812f, 0.4366f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.4552f, 0.3494f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3587f, 0.3291f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3812f, 0.4366f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.4542f, 0.7250f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5007f, 0.4676f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3812f, 0.4366f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.4542f, 0.7250f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3692f, 0.6708f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3812f, 0.4366f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2032f, 0.5906f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3692f, 0.6708f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3812f, 0.4366f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2032f, 0.5906f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3587f, 0.3291f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3812f, 0.4366f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2032f, 0.5906f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3587f, 0.3291f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2490f, 0.3307f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.3579f, 0.2315f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3587f, 0.3291f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2490f, 0.3307f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.3579f, 0.2315f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3556f, 0.0503f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2490f, 0.3307f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2753f, 0.0419f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3556f, 0.0503f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2490f, 0.3307f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2753f, 0.0419f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3556f, 0.0503f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2311f, -0.0899f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.3754f, -0.0610f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3556f, 0.0503f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2311f, -0.0899f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.3754f, -0.0610f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2311f, -0.0899f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3409f, -0.1447f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2518f, -0.2167f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2311f, -0.0899f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.3409f, -0.1447f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2532f, -0.2202f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2111f, -0.4300f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.0135f, -0.0439f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2532f, -0.2202f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2311f, -0.0899f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.0135f, -0.0439f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2753f, 0.0419f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2311f, -0.0899f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.0135f, -0.0439f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2753f, 0.0419f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.0135f, -0.0439f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2490f, 0.3307f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.0275f, 0.5747f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.0135f, -0.0439f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2490f, 0.3307f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.0275f, 0.5747f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2490f, 0.3307f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1188f, 0.5751f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2032f, 0.5906f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2490f, 0.3307f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1188f, 0.5751f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2032f, 0.5906f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1281f, 0.6333f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1188f, 0.5751f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.0275f, 0.5747f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1281f, 0.6333f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1188f, 0.5751f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2032f, 0.5906f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1281f, 0.6333f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2788f, 0.6258f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.3692f, 0.6708f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1281f, 0.6333f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2788f, 0.6258f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.3692f, 0.6708f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1281f, 0.6333f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1152f, 0.6777f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.3692f, 0.6708f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2869f, 0.7948f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1152f, 0.6777f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2020f, 0.8459f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2869f, 0.7948f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1152f, 0.6777f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.2020f, 0.8459f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1655f, 0.8656f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1701f, 0.7884f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.0973f, 0.8771f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1655f, 0.8656f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1701f, 0.7884f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.0275f, 0.5747f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1281f, 0.6333f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1118f, 0.6767f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.0275f, 0.5747f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0185f, 0.6470f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1118f, 0.6767f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.0275f, 0.5747f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0185f, 0.6470f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0929f, 0.6439f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.0275f, 0.5747f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.2802f, 0.6005f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0929f, 0.6439f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.2297f, 0.7260f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.2802f, 0.6005f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0929f, 0.6439f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.2297f, 0.7260f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.2802f, 0.6005f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.3961f, 0.7411f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.2297f, 0.7260f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.3379f, 1.0000f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.3961f, 0.7411f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.0275f, 0.5747f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.2802f, 0.6005f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.4985f, 0.3463f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.5392f, 0.5933f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.2802f, 0.6005f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.4985f, 0.3463f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.5392f, 0.5933f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5834f, 0.3773f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.4985f, 0.3463f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.5392f, 0.5933f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5834f, 0.3773f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.6439f, 0.5330f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.7312f, 0.3965f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5834f, 0.3773f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.6439f, 0.5330f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.7312f, 0.3965f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.9279f, 0.6078f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.6439f, 0.5330f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.7312f, 0.3965f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.9279f, 0.6078f, 0.0f, 1.0f, 1.0f, 1.0f,
    1.0000f, 0.5612f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.9922f, 0.6977f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.9279f, 0.6078f, 0.0f, 1.0f, 1.0f, 1.0f,
    1.0000f, 0.5612f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.0275f, 0.5747f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.4985f, 0.3463f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.0135f, -0.0439f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.6009f, 0.2191f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.4985f, 0.3463f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.0135f, -0.0439f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.6009f, 0.2191f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.4985f, 0.3463f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.7068f, 0.2718f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.6009f, 0.2191f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.8336f, 0.1504f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.7068f, 0.2718f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.6009f, 0.2191f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.6835f, -0.2057f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.0135f, -0.0439f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.6009f, 0.2191f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.7103f, 0.1354f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.6230f, 0.0289f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.8214f, 0.0388f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.7103f, 0.1354f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.6230f, 0.0289f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.8214f, 0.0388f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.9162f, -0.1473f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.6230f, 0.0289f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.6835f, -0.2057f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.6230f, 0.0289f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.8290f, -0.1716f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.6835f, -0.2057f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.9593f, -0.3886f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.8290f, -0.1716f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.6835f, -0.2057f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2111f, -0.4300f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.0135f, -0.0439f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.6835f, -0.2057f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.2111f, -0.4300f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1101f, -0.5530f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.6835f, -0.2057f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0109f, -0.6858f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1101f, -0.5530f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.1403f, -0.7204f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0109f, -0.6858f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.1101f, -0.5530f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.6835f, -0.2057f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0109f, -0.6858f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.2681f, -0.7917f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.0668f, -0.8165f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0109f, -0.6858f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.2681f, -0.7917f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.0668f, -0.8165f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.1797f, -0.8496f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.2681f, -0.7917f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.0668f, -0.8165f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.1797f, -0.8496f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0342f, -0.8961f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.6835f, -0.2057f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5986f, -0.8031f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.2681f, -0.7917f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.3740f, -0.8961f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5986f, -0.8031f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.2681f, -0.7917f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.3740f, -0.8961f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5986f, -0.8031f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5229f, -0.9075f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.3740f, -0.8961f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.4182f, -1.0000f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5229f, -0.9075f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.6835f, -0.2057f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5986f, -0.8031f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.8394f, -0.4537f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.8429f, -0.7607f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5986f, -0.8031f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.8394f, -0.4537f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.8429f, -0.7607f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5986f, -0.8031f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.7021f, -0.8527f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.7952f, -0.8358f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.8429f, -0.7607f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.7021f, -0.8527f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.7010f, -0.9008f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.7952f, -0.8358f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.7021f, -0.8527f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.8429f, -0.7607f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.9104f, -0.6491f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.8394f, -0.4537f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.8429f, -0.7607f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.9104f, -0.6491f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.8860f, -0.8176f, 0.0f, 1.0f, 1.0f, 1.0f,

    // eyes



    // mouth
    
    

};

// define OpenGL object IDs to represent the vertex array and the shader program in the GPU
GLuint vao;         // vertex array object (stores the render state for our vertex array)
GLuint vbo;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint shader;      // combined vertex and fragment shader

// called by the main function to do initial setup, such as uploading vertex
// arrays, shader programs, etc.; returns true if successful, false otherwise
bool setup()
{
    // generate the VAO and VBO objects and store their IDs in vao and vbo, respectively
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // bind the newly-created VAO to make it the current one that OpenGL will apply state changes to
    glBindVertexArray(vao);

    // upload our vertex array data to the newly-created VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // on the VAO, register the current VBO with the following vertex attribute layout:
    // - the stride length of the vertex array is 6 floats (6 * sizeof(float))
    // - layout location 0 (position) is 3 floats and starts at the first float of the vertex array (offset 0)
    // - layout location 1 (color) is also 3 floats but starts at the fourth float (offset 3 * sizeof(float))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));

    // enable the layout locations so they can be used by the vertex shader
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // important: if you have more vertex arrays to draw, make sure you separately define them
    // with unique VAO and VBO IDs, and follow the same process above to upload them to the GPU

    // load our shader program
    shader = gdevLoadShader("exercise1.vs", "exercise1.fs");
    if (! shader)
        return false;

    return true;
}

// called by the main function to do rendering per frame
void render()
{
    // clear the whole frame
    glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // compute a value for the glow amount for this frame
    float glow = fabs(sin(glfwGetTime() / 3.0f)) / 2.0f + 0.5f;

    // using our shader program...
    glUseProgram(shader);

    // ... set the uniform variables of the shader...
    // (in this case, simply set the value of the glow)
    glUniform1f(glGetUniformLocation(shader, "glow"), glow);

    // ... then draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (6 * sizeof(float)));
}

/*****************************************************************************/

// handler called by GLFW when there is a keyboard event
void handleKeys(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    // pressing Esc closes the window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
}

// handler called by GLFW when the window is resized
void handleResize(GLFWwindow* pWindow, int width, int height)
{
    // tell OpenGL to do its drawing within the entire "client area" (area within the borders) of the window
    glViewport(0, 0, width, height);
}

// main function
int main(int argc, char** argv)
{
    // initialize GLFW and ask for OpenGL 3.3 core
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create a GLFW window with the specified width, height, and title
    pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (! pWindow)
    {
        // gracefully terminate if we cannot create the window
        std::cout << "Cannot create the GLFW window.\n";
        glfwTerminate();
        return -1;
    }

    // make the window the current context of subsequent OpenGL commands,
    // and enable vertical sync and aspect-ratio correction on the GLFW window
    glfwMakeContextCurrent(pWindow);
    glfwSwapInterval(1);
    glfwSetWindowAspectRatio(pWindow, WINDOW_WIDTH, WINDOW_HEIGHT);

    // set up callback functions to handle window system events
    glfwSetKeyCallback(pWindow, handleKeys);
    glfwSetFramebufferSizeCallback(pWindow, handleResize);

    // don't miss any momentary keypresses
    glfwSetInputMode(pWindow, GLFW_STICKY_KEYS, GLFW_TRUE);

    // initialize GLAD, which acts as a library loader for the current OS's native OpenGL library
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // if our initial setup is successful...
    if (setup())
    {
        // do rendering in a loop until the user closes the window
        while (! glfwWindowShouldClose(pWindow))
        {
            // render our next frame
            // (by default, GLFW uses double-buffering with a front and back buffer;
            // all drawing goes to the back buffer, so the frame does not get shown yet)
            render();

            // swap the GLFW front and back buffers to show the next frame
            glfwSwapBuffers(pWindow);

            // process any window events (such as moving, resizing, keyboard presses, etc.)
            glfwPollEvents();
        }
    }

    // gracefully terminate the program
    glfwTerminate();
    return 0;
}
