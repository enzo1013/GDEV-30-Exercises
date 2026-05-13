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

/*****************************************************************************
 * Keyboard controls for camera:
 * W moves the camera forward.
 * S moves the camera back.
 * A moves the camera left.
 * D moves the camera right.
 *****************************************************************************/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <gdev.h>
#include <vector>
#include <cstddef>
#include <cfloat>

// change this to your desired window attributes
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE  "Greek Facade"
GLFWwindow *pWindow;

// an attempt of rotating the camera
float cameraYaw = -110.0f;
float cameraPitch = -54.0f;
float cameraRotate = 2.0f;

// values for the lookAt matrix
glm::vec3 cameraEye = glm::vec3(2.0f, 5.0f, -2.0f);  // eye
glm::vec3 cameraCenter = glm::normalize(glm::vec3(
    cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch)),
    sin(glm::radians(cameraPitch)),
    sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch))
    ));   // center
glm::vec3 cameraGlobUp = glm::vec3(0.0f, 1.0f, 0.0f);     // up

float cameraSpeed = 0.5f;
// mouse / look controls
float mouseSensitivity = 0.12f;
bool firstMouse = true;
double lastX = WINDOW_WIDTH / 2.0;
double lastY = WINDOW_HEIGHT / 2.0;
// computed Y offset to place scaled facade on the floor
float facadeYOffset = 0.0f;

// define a vertex array to hold our vertices
float vertices[] =
{
    // position (x, y, z), colours (r, g, b), orientation

    // pillar 1

    // segment 1, rear
    -0.50f, -0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.40f, 0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.40f, -0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    -0.50f, -0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.50f, 0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.40f, 0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    // segment 2, rear left
    -0.50f, -0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.55f, -0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.50f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    -0.55f, -0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.55f, 0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.50f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    // segment 3, front left
    -0.55f, -0.2f, -0.9f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.50f, -0.2f, -0.8f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.55f, 0.2f, -0.9f, 1.0f, 0.0f, 1.0f, 0.0f,

    -0.55f, 0.2f, -0.9f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.50f, -0.2f, -0.8f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.50f, 0.2f, -0.8f, 1.0f, 0.0f, 1.0f, 0.0f,

    // segment 4, front
    -0.50f, -0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.40f, -0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.40f, 0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,

    -0.50f, -0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.40f, 0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.50f, 0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,

    // segment 5, front right
    -0.40f, -0.2f, -0.8f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.35f, -0.2f, -0.9f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.40f, 0.2f, -0.8f, 1.0f, 0.0f, 0.0f, 0.0f,

    -0.35f, -0.2f, -0.9f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.35f, 0.2f, -0.9f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.40f, 0.2f, -0.8f, 1.0f, 0.0f, 0.0f, 0.0f,

    // segment 6, rear right
    -0.40f, -0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.40f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.35f, -0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,

    -0.35f, -0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.40f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.35f, 0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,

    // pillar 2

    // segment 1
    0.40f, -0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.50f, 0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.50f, -0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    0.40f, -0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.40f, 0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.50f, 0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    // segment 2
    0.40f, -0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.35f, -0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.40f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    0.35f, -0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.35f, 0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.40f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    // segment 3
    0.35f, -0.2f, -0.9f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.40f, -0.2f, -0.8f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.35f, 0.2f, -0.9f, 1.0f, 0.0f, 1.0f, 0.0f,

    0.35f, 0.2f, -0.9f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.40f, -0.2f, -0.8f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.40f, 0.2f, -0.8f, 1.0f, 0.0f, 1.0f, 0.0f,

    // segment 4
    0.40f, -0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.50f, -0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.50f, 0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,

    0.40f, -0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.50f, 0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.40f, 0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,

    // segment 5
    0.50f, -0.2f, -0.8f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.55f, -0.2f, -0.9f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.50f, 0.2f, -0.8f, 1.0f, 0.0f, 0.0f, 0.0f,

    0.55f, -0.2f, -0.9f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.55f, 0.2f, -0.9f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.50f, 0.2f, -0.8f, 1.0f, 0.0f, 0.0f, 0.0f,

    // segment 6
    0.50f, -0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.50f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.55f, -0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,

    0.55f, -0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.50f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.55f, 0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,

    // pillar 3

    // segment 1
    -0.20f, -0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.10f, 0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.10f, -0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    -0.20f, -0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.20f, 0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.10f, 0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    // segment 2
    -0.20f, -0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.25f, -0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.20f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    -0.25f, -0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.25f, 0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.20f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    // segment 3
    -0.25f, -0.2f, -0.9f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.20f, -0.2f, -0.8f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.25f, 0.2f, -0.9f, 1.0f, 0.0f, 1.0f, 0.0f,

    -0.25f, 0.2f, -0.9f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.20f, -0.2f, -0.8f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.20f, 0.2f, -0.8f, 1.0f, 0.0f, 1.0f, 0.0f,

    // segment 4
    -0.20f, -0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.10f, -0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.10f, 0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,

    -0.20f, -0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.10f, 0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.20f, 0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,

    // segment 5
    -0.10f, -0.2f, -0.8f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.05f, -0.2f, -0.9f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.10f, 0.2f, -0.8f, 1.0f, 0.0f, 0.0f, 0.0f,

    -0.05f, -0.2f, -0.9f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.05f, 0.2f, -0.9f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.10f, 0.2f, -0.8f, 1.0f, 0.0f, 0.0f, 0.0f,

    // segment 6
    -0.10f, -0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.10f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.05f, -0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,

    -0.05f, -0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.10f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -0.05f, 0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,

    // pillar 4

    // segment 1
    0.10f, -0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.20f, 0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.20f, -0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    0.10f, -0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.10f, 0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.20f, 0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    // segment 2
    0.05f, -0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.10f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.10f, -0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    0.05f, -0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.05f, 0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.10f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    // segment 3
    0.05f, -0.2f, -0.9f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.10f, -0.2f, -0.8f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.05f, 0.2f, -0.9f, 1.0f, 0.0f, 1.0f, 0.0f,

    0.05f, 0.2f, -0.9f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.10f, -0.2f, -0.8f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.10f, 0.2f, -0.8f, 1.0f, 0.0f, 1.0f, 0.0f,

    // segment 4
    0.10f, -0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.20f, -0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.20f, 0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,

    0.10f, -0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.20f, 0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.10f, 0.2f, -0.8f, 1.0f, 1.0f, 0.0f, 0.0f,

    // segment 5
    0.20f, -0.2f, -0.8f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.25f, -0.2f, -0.9f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.20f, 0.2f, -0.8f, 1.0f, 0.0f, 0.0f, 0.0f,

    0.25f, -0.2f, -0.9f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.25f, 0.2f, -0.9f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.20f, 0.2f, -0.8f, 1.0f, 0.0f, 0.0f, 0.0f,

    // segment 6
    0.20f, -0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.20f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.25f, -0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,

    0.25f, -0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.20f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.25f, 0.2f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f,

    // front triangle

    -0.65f, 0.2f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.65f, 0.2f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.4f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f,

    // back triangle

    -0.65f, 0.2f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.4f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.65f, 0.2f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    // triangle edges

    -0.65f, 0.2f, -0.8f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.4f, -0.8f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.65f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.0f, 0.4f, -0.8f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.4f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.65f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.0f, 0.4f, -0.8f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.65f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.4f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.65f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.4f, -0.8f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.65f, 0.2f, -0.8f, 0.0f, 1.0f, 1.0f, 1.0f,

    -0.65f, 0.2f, -0.8f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.65f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.65f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.65f, 0.2f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.65f, 0.2f, -0.8f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.65f, 0.2f, -0.8f, 0.0f, 1.0f, 1.0f, 1.0f,

    // front base top

    -0.65f, -0.2f, -0.8f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.65f, -0.25f, -0.8f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.65f, -0.2f, -0.8f, 1.0f, 0.0f, 1.0f, 1.0f,

    0.65f, -0.25f, -0.8f, 1.0f, 0.0f, 1.0f, 1.0f,
    -0.65f, -0.2f, -0.8f, 1.0f, 0.0f, 1.0f, 1.0f,
    -0.65f, -0.25f, -0.8f, 1.0f, 0.0f, 1.0f, 1.0f,

    // back base top

    -0.65f, -0.2f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.65f, -0.2f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.65f, -0.25f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

    0.65f, -0.25f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    -0.65f, -0.25f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    -0.65f, -0.2f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

    // edges of base top

    -0.65f, -0.2f, -0.8f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.65f, -0.2f, -1.0f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.65f, -0.2f, -1.0f, 0.5f, 0.5f, 1.0f, 0.0f,

    0.65f, -0.2f, -1.0f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.65f, -0.2f, -0.8f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.65f, -0.2f, -0.8f, 0.5f, 0.5f, 1.0f, 0.0f,

    -0.65f, -0.25f, -0.8f, 0.5f, 0.0f, 0.75f, 0.0f,
    -0.65f, -0.2f, -0.8f, 0.5f, 0.0f, 0.75f, 0.0f,
    -0.65f, -0.2f, -1.0f, 0.5f, 0.0f, 0.75f, 0.0f,

    -0.65f, -0.2f, -1.0f, 0.5f, 0.0f, 0.75f, 0.0f,
    -0.65f, -0.25f, -1.0f, 0.5f, 0.0f, 0.75f, 0.0f,
    -0.65f, -0.25f, -0.8f, 0.5f, 0.0f, 0.75f, 0.0f,

    0.65f, -0.2f, -1.0f, 0.5f, 0.0f, 0.75f, 0.0f,
    0.65f, -0.2f, -0.8f, 0.5f, 0.0f, 0.75f, 0.0f,
    0.65f, -0.25f, -0.8f, 0.5f, 0.0f, 0.75f, 0.0f,

    0.65f, -0.25f, -0.8f, 0.5f, 0.0f, 0.75f, 0.0f,
    0.65f, -0.25f, -1.0f, 0.5f, 0.0f, 0.75f, 0.0f,
    0.65f, -0.2f, -1.0f, 0.5f, 0.0f, 0.75f, 0.0f,

    // front base bottom

    -0.75f, -0.25f, -0.75f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.75f, -0.3f, -0.75f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.75f, -0.25f, -0.75f, 0.5f, 0.5f, 1.0f, 0.0f,

    0.75f, -0.3f, -0.75f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.75f, -0.25f, -0.75f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.75f, -0.3f, -0.75f, 0.5f, 0.5f, 1.0f, 0.0f,

    // back base bottom

    -0.75f, -0.25f, -1.0f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.75f, -0.25f, -1.0f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.75f, -0.3f, -1.0f, 0.5f, 0.5f, 1.0f, 0.0f,

    0.75f, -0.3f, -1.0f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.75f, -0.3f, -1.0f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.75f, -0.25f, -1.0f, 0.5f, 0.5f, 1.0f, 0.0f,

    // edges of base bottom

    -0.75f, -0.25f, -0.75f, 0.5f, 0.0f, 1.0f, 1.0f,
    0.75f, -0.25f, -1.0f, 0.5f, 0.0f, 1.0f, 1.0f,
    -0.75f, -0.25f, -1.0f, 0.5f, 0.0f, 1.0f, 1.0f,

    0.75f, -0.25f, -1.0f, 0.5f, 0.0f, 1.0f, 1.0f,
    -0.75f, -0.25f, -0.75f, 0.5f, 0.0f, 1.0f, 1.0f,
    0.75f, -0.25f, -0.75f, 0.5f, 0.0f, 1.0f, 1.0f,

    -0.75f, -0.3f, -0.75f, 0.5f, 0.0f, 1.0f, 1.0f,
    -0.75f, -0.3f, -1.0f, 0.5f, 0.0f, 1.0f, 1.0f,
    0.75f, -0.3f, -1.0f, 0.5f, 0.0f, 1.0f, 1.0f,

    0.75f, -0.3f, -1.0f, 0.5f, 0.0f, 1.0f, 1.0f,
    0.75f, -0.3f, -0.75f, 0.5f, 0.0f, 1.0f, 1.0f,
    -0.75f, -0.3f, -0.75f, 0.5f, 0.0f, 1.0f, 1.0f,

    -0.75f, -0.3f, -0.75f, 0.5f, 0.0f, 1.0f, 1.0f,
    -0.75f, -0.25f, -0.75f, 0.5f, 0.0f, 1.0f, 1.0f,
    -0.75f, -0.25f, -1.0f, 0.5f, 0.0f, 1.0f, 1.0f,

    -0.75f, -0.25f, -1.0f, 0.5f, 0.0f, 1.0f, 1.0f,
    -0.75f, -0.3f, -1.0f, 0.5f, 0.0f, 1.0f, 1.0f,
    -0.75f, -0.3f, -0.75f, 0.5f, 0.0f, 1.0f, 1.0f,

    0.75f, -0.25f, -1.0f, 0.5f, 0.0f, 1.0f, 1.0f,
    0.75f, -0.25f, -0.75f, 0.5f, 0.0f, 1.0f, 1.0f,
    0.75f, -0.3f, -0.75f, 0.5f, 0.0f, 1.0f, 1.0f,

    0.75f, -0.3f, -0.75f, 0.5f, 0.0f, 1.0f, 1.0f,
    0.75f, -0.3f, -1.0f, 0.5f, 0.0f, 1.0f, 1.0f,
    0.75f, -0.25f, -1.0f, 0.5f, 0.0f, 1.0f, 1.0f,

};

/* -------------------------------------------------------------------------
 * Model code and helpers (floor + horse).
 * Provides `Model`, vertex layout, helpers, and model builders.
 * ------------------------------------------------------------------------- */

struct Model {
    GLuint  VAO;         // vertex array object
    GLuint  VBO;         // vertex buffer (positions + normals + UVs)
    GLuint  EBO;         // element (index) buffer
    GLsizei indexCount;  // number of indices to pass to glDrawElements
    GLenum  drawMode;    // GL_TRIANGLES
};

struct Vertex {
    float pos[3];   // XYZ position
    float nor[3];   // XYZ normal (unit vector)
    float uv[2];    // UV texture coordinate
};

static void addBox(std::vector<Vertex>&        verts,
                   std::vector<unsigned int>&  idxs,
                   float cx, float cy, float cz,
                   float hx, float hy, float hz)
{
    static const float UV[4][2] = { {0,1},{1,1},{1,0},{0,0} };
    struct Face { float nx, ny, nz; float c[4][3]; };
    const Face faces[6] = {
        { 1, 0, 0, {{ hx, hy, hz}, { hx,-hy, hz}, { hx,-hy,-hz}, { hx, hy,-hz}} },
        {-1, 0, 0, {{-hx, hy,-hz}, {-hx,-hy,-hz}, {-hx,-hy, hz}, {-hx, hy, hz}} },
        { 0, 1, 0, {{-hx, hy,-hz}, { hx, hy,-hz}, { hx, hy, hz}, {-hx, hy, hz}} },
        { 0,-1, 0, {{-hx,-hy, hz}, { hx,-hy, hz}, { hx,-hy,-hz}, {-hx,-hy,-hz}} },
        { 0, 0, 1, {{-hx, hy, hz}, { hx, hy, hz}, { hx,-hy, hz}, {-hx,-hy, hz}} },
        { 0, 0,-1, {{ hx, hy,-hz}, {-hx, hy,-hz}, {-hx,-hy,-hz}, { hx,-hy,-hz}} },
    };

    for (const Face& f : faces) {
        unsigned int base = static_cast<unsigned int>(verts.size());
        for (int v = 0; v < 4; v++) {
            Vertex vtx;
            vtx.pos[0] = cx + f.c[v][0];
            vtx.pos[1] = cy + f.c[v][1];
            vtx.pos[2] = cz + f.c[v][2];
            vtx.nor[0] = f.nx; vtx.nor[1] = f.ny; vtx.nor[2] = f.nz;
            vtx.uv[0]  = UV[v][0]; vtx.uv[1]  = UV[v][1];
            verts.push_back(vtx);
        }
        idxs.push_back(base + 0); idxs.push_back(base + 1); idxs.push_back(base + 2);
        idxs.push_back(base + 0); idxs.push_back(base + 2); idxs.push_back(base + 3);
    }
}

static Model uploadToGPU(const std::vector<Vertex>&       verts,
                         const std::vector<unsigned int>& idxs)
{
    Model m;
    m.indexCount = static_cast<GLsizei>(idxs.size());
    m.drawMode   = GL_TRIANGLES;

    glGenVertexArrays(1, &m.VAO);
    glGenBuffers(1, &m.VBO);
    glGenBuffers(1, &m.EBO);

    glBindVertexArray(m.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m.VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(verts.size() * sizeof(Vertex)),
                 verts.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(idxs.size() * sizeof(unsigned int)),
                 idxs.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, pos)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, nor)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, uv)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    return m;
}

Model createFloorModel()
{
    std::vector<Vertex> verts;
    std::vector<unsigned int> idxs;
    const int GRID = 4; const float HALF = 10.0f;
    const float STEP = (2.0f * HALF) / GRID; const float UV_SCALE = 2.5f;
    for (int row = 0; row <= GRID; row++) {
        for (int col = 0; col <= GRID; col++) {
            Vertex v;
            v.pos[0] = -HALF + col * STEP; v.pos[1] = 0.0f; v.pos[2] = -HALF + row * STEP;
            v.nor[0] = 0.0f; v.nor[1] = 1.0f; v.nor[2] = 0.0f;
            v.uv[0] = col * UV_SCALE; v.uv[1] = row * UV_SCALE;
            verts.push_back(v);
        }
    }
    const int W = GRID + 1;
    for (int row = 0; row < GRID; row++) {
        for (int col = 0; col < GRID; col++) {
            unsigned int tl = row * W + col;
            unsigned int tr = tl + 1;
            unsigned int bl = tl + W;
            unsigned int br = bl + 1;
            idxs.push_back(tl); idxs.push_back(bl); idxs.push_back(tr);
            idxs.push_back(tr); idxs.push_back(bl); idxs.push_back(br);
        }
    }
    return uploadToGPU(verts, idxs);
}

Model createHorseModel()
{
    std::vector<Vertex> verts;
    std::vector<unsigned int> idxs;
    addBox(verts, idxs,   0.00f, 0.58f,  0.00f,  0.26f, 0.20f, 0.60f);
    addBox(verts, idxs,   0.00f, 0.52f,  0.53f,  0.22f, 0.16f, 0.12f);
    addBox(verts, idxs,   0.00f, 0.60f, -0.56f,  0.23f, 0.17f, 0.10f);
    addBox(verts, idxs,  -0.17f, 0.20f,  0.45f,  0.07f, 0.20f, 0.07f);
    addBox(verts, idxs,   0.17f, 0.20f,  0.45f,  0.07f, 0.20f, 0.07f);
    addBox(verts, idxs,  -0.17f, 0.20f, -0.44f,  0.07f, 0.20f, 0.07f);
    addBox(verts, idxs,   0.17f, 0.20f, -0.44f,  0.07f, 0.20f, 0.07f);
    addBox(verts, idxs,  -0.17f, 0.04f,  0.45f,  0.08f, 0.04f, 0.08f);
    addBox(verts, idxs,   0.17f, 0.04f,  0.45f,  0.08f, 0.04f, 0.08f);
    addBox(verts, idxs,  -0.17f, 0.04f, -0.44f,  0.08f, 0.04f, 0.08f);
    addBox(verts, idxs,   0.17f, 0.04f, -0.44f,  0.08f, 0.04f, 0.08f);
    addBox(verts, idxs,   0.00f, 0.81f,  0.59f,  0.11f, 0.17f, 0.11f);
    addBox(verts, idxs,   0.00f, 1.03f,  0.67f,  0.10f, 0.14f, 0.10f);
    addBox(verts, idxs,   0.00f, 1.21f,  0.75f,  0.10f, 0.12f, 0.20f);
    addBox(verts, idxs,   0.00f, 1.13f,  0.92f,  0.08f, 0.09f, 0.10f);
    addBox(verts, idxs,  -0.07f, 1.38f,  0.70f,  0.04f, 0.09f, 0.04f);
    addBox(verts, idxs,   0.07f, 1.38f,  0.70f,  0.04f, 0.09f, 0.04f);
    addBox(verts, idxs,   0.00f, 1.02f,  0.64f,  0.04f, 0.20f, 0.16f);
    addBox(verts, idxs,   0.00f, 0.73f, -0.71f,  0.05f, 0.16f, 0.05f);
    addBox(verts, idxs,   0.00f, 0.55f, -0.77f,  0.04f, 0.12f, 0.04f);
    return uploadToGPU(verts, idxs);
}

void drawModel(const Model& model)
{
    glBindVertexArray(model.VAO);
    glDrawElements(model.drawMode, model.indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void destroyModel(Model& model)
{
    glDeleteBuffers(1, &model.EBO);
    glDeleteBuffers(1, &model.VBO);
    glDeleteVertexArrays(1, &model.VAO);
    model.VAO = model.VBO = model.EBO = 0;
    model.indexCount = 0;
}

// define OpenGL object IDs to represent the vertex array and the shader program in the GPU
GLuint vao;         // vertex array object (stores the render state for our vertex array)
GLuint vbo;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint shader;      // combined vertex and fragment shader
GLuint texture;
// model instances
Model floorModel;
Model horseModel;

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) (6 * sizeof(float)));

    // enable the newly-created layout location 0;
    // this shall be used by our vertex shader to read the vertex's x, y, and z
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // important: if you have more vertex arrays to draw, make sure you separately define them
    // with unique VAO and VBO IDs, and follow the same process above to upload them to the GPU

    // load our shader program
    shader = gdevLoadShader("exercise3.vs", "exercise3.fs");
    if (! shader)
        return false;
    texture = gdevLoadTexture("e3tex1.png", GL_REPEAT, true, true);
    if (! texture)
        return false;
    // bind texture unit 0 and tell shader to use it
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUseProgram(shader);
    GLint loc = glGetUniformLocation(shader, "texture1");
    if (loc >= 0) glUniform1i(loc, 0);
    // compute lowest Y of the facade vertices so we can place its base on the floor
    {
        size_t floatsPerVertex = 7;
        size_t totalFloats = sizeof(vertices) / sizeof(vertices[0]);
        size_t vertexCount = totalFloats / floatsPerVertex;
        float minY = FLT_MAX;
        for (size_t vi = 0; vi < vertexCount; ++vi) {
            float y = vertices[vi * floatsPerVertex + 1];
            if (y < minY) minY = y;
        }
        // first model is scaled by 2.0f in render; compute offset so minY*scale + offset == 0
        float scale0 = 2.0f;
        facadeYOffset = -minY * scale0;
    }
    // create models
    floorModel = createFloorModel();
    horseModel = createHorseModel();
    if (floorModel.VAO == 0 || horseModel.VAO == 0)
        return false;
    return true;
}

// called by the main function to do rendering per frame
void render()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float) WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

    // viewed at using this matrix
    glm::mat4 view = glm::lookAt(
        cameraEye,
        cameraEye + cameraCenter,
        cameraGlobUp
    );

    // clear the whole frame
    glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // using our shader program...
    glUseProgram(shader);

    // Bind VAO for the original facade
    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Draw three objects: original facade (i==0), floor (i==1), horse (i==2)
    for(int i = 0; i < 3; i++){
        glm::mat4 model = glm::mat4(1.0f);

        // horizontal placement: left, center, right
        float tx[3] = {-3.0f, 0.0f, 3.0f};
        // vertical placement: compute using facadeYOffset so lowest point rests on floor
        float ty[3] = {facadeYOffset, 0.0f, 0.0f};
        model = glm::translate(model, glm::vec3(tx[i], ty[i], -5.0f));

        // keep models static (no rotation)

        // scale (first model is 2× larger)
        float scales[3] = {2.0f, 0.7f, 1.3f};
        model = glm::scale(model, glm::vec3(scales[i], scales[i], scales[i]));

        // MVP matrix
        glm::mat4 mvp = projection * view * model;
        glUniformMatrix4fv(glGetUniformLocation(shader, "matrix"), 1, GL_FALSE, glm::value_ptr(mvp));

        if (i == 0) {
            // draw original facade using the VAO
            GLsizei count = static_cast<GLsizei>(sizeof(vertices) / (7 * sizeof(float)));
            glDrawArrays(GL_TRIANGLES, 0, count);
        } else if (i == 1) {
            // draw floor model
            glBindVertexArray(0);
            drawModel(floorModel);
            glBindVertexArray(vao);
        } else {
            // draw horse model — disable face culling so both sides render
            glBindVertexArray(0);
            GLboolean wasCull = glIsEnabled(GL_CULL_FACE);
            if (wasCull) glDisable(GL_CULL_FACE);
            drawModel(horseModel);
            if (wasCull) glEnable(GL_CULL_FACE);
            glBindVertexArray(vao);
        }
    }
}

// handler called by GLFW when there is a keyboard event
void handleKeys(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    // pressing Esc closes the window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    
    // https://www.glfw.org/docs/latest/group__keys.html

    // pressing W will move the camera up
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraEye += cameraSpeed * cameraCenter;
    // pressing S will move the camera down
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraEye -= cameraSpeed * cameraCenter;
    // pressing A will move the camera left
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraEye -= glm::normalize(glm::cross(cameraCenter, cameraGlobUp)) * cameraSpeed;
    // pressing D will move the camera right
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraEye += glm::normalize(glm::cross(cameraCenter, cameraGlobUp)) * cameraSpeed;

    // pressing Q will rotate the camera to the left
    if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cameraYaw -= cameraRotate;
        cameraCenter = glm::normalize(glm::vec3(
        cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch)),
        sin(glm::radians(cameraPitch)),
        sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch))
        ));
    }

    // pressing E will rotate the camera to the right
    if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cameraYaw += cameraRotate;
        cameraCenter = glm::normalize(glm::vec3(
        cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch)),
        sin(glm::radians(cameraPitch)),
        sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch))
        ));
    }

    // pressing 1 or 2 will rotate the camera down
    if ((key == GLFW_KEY_1 || key == GLFW_KEY_2) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cameraPitch -= cameraRotate;
        cameraCenter = glm::normalize(glm::vec3(
        cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch)),
        sin(glm::radians(cameraPitch)),
        sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch))
        ));
    }

    // pressing 3 or 4 will rotate the camera up
    if ((key == GLFW_KEY_3 || key == GLFW_KEY_4) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        cameraPitch += cameraRotate;
        cameraCenter = glm::normalize(glm::vec3(
        cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch)),
        sin(glm::radians(cameraPitch)),
        sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch))
        ));
    }
}

// mouse look callback
void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; // reversed: y ranges bottom->top
    lastX = xpos;
    lastY = ypos;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    cameraYaw += (float)xoffset;
    cameraPitch += (float)yoffset;
    if (cameraPitch > 89.0f) cameraPitch = 89.0f;
    if (cameraPitch < -89.0f) cameraPitch = -89.0f;

    cameraCenter = glm::normalize(glm::vec3(
        cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch)),
        sin(glm::radians(cameraPitch)),
        sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch))
    ));
}

// scroll callback to adjust camera speed (zoom-like)
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    cameraSpeed += (float)yoffset * 0.1f;
    if (cameraSpeed < 0.05f) cameraSpeed = 0.05f;
    if (cameraSpeed > 10.0f) cameraSpeed = 10.0f;
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

    // mouse and scroll callbacks for free-look and speed control
    glfwSetCursorPosCallback(pWindow, mouseCallback);
    glfwSetScrollCallback(pWindow, scrollCallback);

    // capture and hide the cursor for FPS-style look
    glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
