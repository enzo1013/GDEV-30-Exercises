/******************************************************************************
 * This demo draws a triangle by defining its vertices in 3 dimensions
 * (the 3rd dimension is currently ignored and is just set to 0).
 *
 * The drawing is accomplished by:
 * - Uploading the vertices to the GPU using a Vertex Buffer Object (VBO).
 * - Specifying the vertices' format using a Vertex Array Object (VAO).
 * - Using a GLSL shader program (consisting of a simple vertex shader and a
 *   simple fragment shader) to actually draw the vertices as a triangle.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <gdev.h>

// change this to your desired window attributes
#define WINDOW_WIDTH  748
#define WINDOW_HEIGHT 748
#define WINDOW_TITLE  "Exercise 2"
GLFWwindow *pWindow;

// define a vertex array to hold our vertices
float vertices[] =
{
    // x, y, z, r, g, b, s, t

    // lower part

    -0.800f, -0.800f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.0000f,
    0.500f, -0.800f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.0000f,
    -0.800f, -0.400f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.2500f,

    0.000f, -0.660f, 0.00f, 1.00f, 1.00f, 1.00f, 0.5000f, 0.0875f,
    0.500f, -0.800f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.0000f,
    0.180f, -0.520f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6125f, 0.1750f,

    0.180f, -0.520f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6125f, 0.1750f,
    0.500f, -0.800f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.0000f,
    0.175f, -0.470f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6094f, 0.2063f,

    0.175f, -0.470f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6094f, 0.2063f,
    0.500f, -0.800f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.0000f,
    0.270f, -0.475f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6687f, 0.2031f,

    0.270f, -0.475f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6687f, 0.2031f,
    0.500f, -0.800f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.0000f,
    0.600f, -0.610f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8750f, 0.1188f,

    0.600f, -0.610f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8750f, 0.1188f,
    0.500f, -0.800f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.0000f,
    0.750f, -0.800f, 0.00f, 1.00f, 1.00f, 1.00f, 0.9688f, 0.0000f,

    // head

    0.000f, -0.660f, 0.00f, 1.00f, 1.00f, 1.00f, 0.5000f, 0.0875f,
    -0.800f, 0.605f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.8781f,
    -0.800f, -0.400f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.2500f,

    0.180f, -0.520f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6125f, 0.1750f,
    -0.800f, 0.605f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.8781f,
    0.000f, -0.660f, 0.00f, 1.00f, 1.00f, 1.00f, 0.5000f, 0.0875f,

    -0.800f, 0.605f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.8781f,
    0.080f, 0.800f, 0.00f, 1.00f, 1.00f, 1.00f, 0.5500f, 1.0000f,
    -0.400f, 0.800f, 0.00f, 1.00f, 1.00f, 1.00f, 0.2500f, 1.0000f,

    -0.800f, 0.605f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.8781f,
    0.000f, -0.660f, 0.00f, 1.00f, 1.00f, 1.00f, 0.5000f, 0.0875f,
    0.180f, -0.520f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6125f, 0.1750f,

    -0.800f, 0.605f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.8781f,
    0.180f, -0.520f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6125f, 0.1750f,
    0.175f, -0.470f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6094f, 0.2063f,

    -0.800f, 0.605f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.8781f,
    0.175f, -0.470f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6094f, 0.2063f,
    0.170f, -0.330f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6063f, 0.2938f,

    0.170f, -0.330f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6063f, 0.2938f,
    0.185f, -0.245f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6156f, 0.3469f,
    0.030f, -0.215f, 0.00f, 1.00f, 1.00f, 1.00f, 0.5188f, 0.3656f,

    0.030f, -0.215f, 0.00f, 1.00f, 1.00f, 1.00f, 0.5188f, 0.3656f,
    -0.800f, 0.605f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.8781f,
    0.030f, -0.193f, 0.00f, 1.00f, 1.00f, 1.00f, 0.5188f, 0.3797f,

    0.030f, -0.193f, 0.00f, 1.00f, 1.00f, 1.00f, 0.5188f, 0.3797f,
    0.185f, -0.200f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6156f, 0.3750f,
    0.205f, -0.110f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6281f, 0.4313f,

    -0.800f, 0.605f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.8781f,
    0.030f, -0.193f, 0.00f, 1.00f, 1.00f, 1.00f, 0.5188f, 0.3797f,
    0.205f, -0.110f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6281f, 0.4313f,

    0.205f, -0.110f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6281f, 0.4313f,
    0.275f, 0.075f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6719f, 0.5469f,
    -0.800f, 0.605f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.8781f,

    0.275f, 0.075f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6719f, 0.5469f,
    0.265f, 0.130f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6656f, 0.5813f,
    -0.800f, 0.605f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.8781f,

    0.280f, 0.360f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6750f, 0.7250f,
    -0.800f, 0.605f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.8781f,
    0.265f, 0.130f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6656f, 0.5813f,

    0.260f, 0.500f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6625f, 0.8125f,
    -0.800f, 0.605f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.8781f,
    0.280f, 0.360f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6750f, 0.7250f,

    0.250f, 0.512f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6562f, 0.8203f,
    -0.800f, 0.605f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.8781f,
    0.260f, 0.500f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6625f, 0.8125f,

    0.172f, 0.600f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6078f, 0.8750f,
    -0.800f, 0.605f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.8781f,
    0.250f, 0.512f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6562f, 0.8203f,

    0.163f, 0.640f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6016f, 0.9000f,
    -0.800f, 0.605f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.8781f,
    0.172f, 0.600f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6078f, 0.8750f,

    0.165f, 0.725f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6031f, 0.9531f,
    -0.800f, 0.605f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.8781f,
    0.163f, 0.640f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6016f, 0.9000f,

    0.080f, 0.800f, 0.00f, 1.00f, 1.00f, 1.00f, 0.5500f, 1.0000f,
    -0.800f, 0.605f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.8781f,
    0.165f, 0.725f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6031f, 0.9531f,

    // large portion of hat

    -0.800f, 0.605f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 0.8781f,
    -0.400f, 0.800f, 0.00f, 1.00f, 1.00f, 1.00f, 0.2500f, 1.0000f,
    -0.800f, 0.800f, 0.00f, 1.00f, 1.00f, 1.00f, 0.0000f, 1.0000f,

    // ear

    0.265f, 0.130f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6656f, 0.5813f,
    0.360f, 0.184f, 0.00f, 1.00f, 1.00f, 1.00f, 0.7250f, 0.6150f,
    0.280f, 0.360f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6750f, 0.7250f,

    0.360f, 0.184f, 0.00f, 1.00f, 1.00f, 1.00f, 0.7250f, 0.6150f,
    0.400f, 0.395f, 0.00f, 1.00f, 1.00f, 1.00f, 0.7500f, 0.7469f,
    0.280f, 0.360f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6750f, 0.7250f,

    0.360f, 0.184f, 0.00f, 1.00f, 1.00f, 1.00f, 0.7250f, 0.6150f,
    0.431f, 0.247f, 0.00f, 1.00f, 1.00f, 1.00f, 0.7694f, 0.6544f,
    0.400f, 0.395f, 0.00f, 1.00f, 1.00f, 1.00f, 0.7500f, 0.7469f,

    0.431f, 0.247f, 0.00f, 1.00f, 1.00f, 1.00f, 0.7694f, 0.6544f,
    0.500f, 0.417f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.7609f,
    0.400f, 0.395f, 0.00f, 1.00f, 1.00f, 1.00f, 0.7500f, 0.7469f,

    0.431f, 0.247f, 0.00f, 1.00f, 1.00f, 1.00f, 0.7694f, 0.6544f,
    0.500f, 0.320f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.7000f,
    0.500f, 0.417f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.7609f,

    0.400f, 0.395f, 0.00f, 1.00f, 1.00f, 1.00f, 0.7500f, 0.7469f,
    0.500f, 0.320f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.7000f,
    0.500f, 0.417f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.7609f,

    0.500f, 0.320f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.7000f,
    0.500f, 0.417f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.7609f,
    0.560f, 0.420f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8500f, 0.7625f,

    // small portion of hat

    0.080f, 0.800f, 0.00f, 1.00f, 1.00f, 1.00f, 0.5500f, 1.0000f,
    0.165f, 0.725f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6031f, 0.9531f,
    0.160f, 0.800f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6000f, 1.0000f,

    0.160f, 0.800f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6000f, 1.0000f,
    0.165f, 0.725f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6031f, 0.9531f,
    0.202f, 0.768f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6262f, 0.9800f,

    // cigarette

    0.030f, -0.193f, 0.00f, 1.00f, 1.00f, 1.00f, 0.5188f, 0.3797f,
    0.030f, -0.215f, 0.00f, 1.00f, 1.00f, 1.00f, 0.5188f, 0.3656f,
    0.185f, -0.200f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6156f, 0.3750f,

    0.030f, -0.193f, 0.00f, 1.00f, 1.00f, 1.00f, 0.5188f, 0.3797f,
    0.030f, -0.215f, 0.00f, 1.00f, 1.00f, 1.00f, 0.5188f, 0.3656f,
    0.185f, -0.245f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6156f, 0.3469f,

    0.030f, -0.193f, 0.00f, 1.00f, 1.00f, 1.00f, 0.5188f, 0.3797f,
    0.185f, -0.245f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6156f, 0.3469f,
    0.185f, -0.200f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6156f, 0.3750f,

    0.185f, -0.245f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6156f, 0.3469f,
    0.290f, -0.262f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6813f, 0.3362f,
    0.185f, -0.200f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6156f, 0.3750f,

    0.185f, -0.200f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6156f, 0.3750f,
    0.300f, -0.210f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6875f, 0.3688f,
    0.290f, -0.262f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6813f, 0.3362f,

    0.030f, -0.210f, 0.00f, 1.00f, 1.00f, 1.00f, 0.5188f, 0.3688f,
    0.385f, -0.280f, 0.00f, 1.00f, 1.00f, 1.00f, 0.7406f, 0.3250f,
    0.500f, -0.255f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.3409f,

    0.185f, -0.245f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6156f, 0.3469f,
    0.290f, -0.262f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6813f, 0.3362f,
    0.300f, -0.210f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6875f, 0.3688f,

    0.300f, -0.210f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6875f, 0.3688f,
    0.290f, -0.262f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6813f, 0.3362f,
    0.500f, -0.255f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.3409f,

    0.290f, -0.262f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6813f, 0.3362f,
    0.385f, -0.280f, 0.00f, 1.00f, 1.00f, 1.00f, 0.7406f, 0.3250f,
    0.300f, -0.210f, 0.00f, 1.00f, 1.00f, 1.00f, 0.6875f, 0.3688f,

    0.385f, -0.280f, 0.00f, 1.00f, 1.00f, 1.00f, 0.7406f, 0.3250f,
    0.500f, -0.310f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.3063f,
    0.500f, -0.255f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.3409f,

    0.500f, -0.310f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.3063f,
    0.555f, -0.274f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8469f, 0.3287f,
    0.500f, -0.255f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.3409f,

    0.500f, -0.310f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8125f, 0.3063f,
    0.565f, -0.330f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8531f, 0.2938f,
    0.555f, -0.274f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8469f, 0.3287f,

    0.555f, -0.274f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8469f, 0.3287f,
    0.565f, -0.330f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8531f, 0.2938f,
    0.590f, -0.292f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8688f, 0.3175f,

    0.590f, -0.292f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8688f, 0.3175f,
    0.565f, -0.330f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8531f, 0.2938f,
    0.580f, -0.330f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8625f, 0.2938f,

    0.590f, -0.292f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8688f, 0.3175f,
    0.580f, -0.330f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8625f, 0.2938f,
    0.596f, -0.298f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8725f, 0.3137f,

    0.580f, -0.330f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8625f, 0.2938f,
    0.598f, -0.314f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8738f, 0.3038f,
    0.596f, -0.298f, 0.00f, 1.00f, 1.00f, 1.00f, 0.8725f, 0.3137f,

};

// define OpenGL object IDs to represent the vertex array and the shader program in the GPU
GLuint vao;         // vertex array object (stores the render state for our vertex array)
GLuint vbo;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint shader;      // combined vertex and fragment shader
GLuint texture;

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
    // - layout location 0...
    // - ... shall consist of 3 GL_FLOATs (corresponding to x, y, and z coordinates)
    // - ... its values will NOT be normalized (GL_FALSE)
    // - ... the stride length is the number of bytes of all 3 floats of each vertex (hence, 3 * sizeof(float))
    // - ... and we start at the beginning of the array (hence, (void*) 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));

    // enable the newly-created layout location 0;
    // this shall be used by our vertex shader to read the vertex's x, y, and z
    glEnableVertexAttribArray(0);

    // enable the newly-created layout location 1;
    // this shall be used by our vertex shader to read the vertex's r, g, and b
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // important: if you have more vertex arrays to draw, make sure you separately define them
    // with unique VAO and VBO IDs, and follow the same process above to upload them to the GPU

    texture = gdevLoadTexture("main.jpg", GL_REPEAT, true, true);
    if (! texture)
        return false;

    // load our shader program
    shader = gdevLoadShader("demo1.vs", "demo1.fs");
    if (! shader)
        return false;

    return true;
}

// called by the main function to do rendering per frame
void render()
{
    // clear the whole frame
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // using our shader program...
    glUseProgram(shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // ... draw our triangles
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (8 * sizeof(float)));
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
