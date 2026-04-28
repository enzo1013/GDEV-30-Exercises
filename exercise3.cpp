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
 * Q rotates the camera left.
 * E rotates the camera right.
 * 1 or 2 rotates the camera down.
 * 3 or 4 rotates the camera up.
 *****************************************************************************/

/*****************************************************************************
 * Keyboard controls for lights:
 * K moves the light to the right. Shift + K, to the left.
 * I moves the light down. Shift + I, up.
 * J moves the light forward. Shift + J, back.
 *****************************************************************************/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <gdev.h>

// change this to your desired window attributes
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE  "Greek Facade"
GLFWwindow *pWindow;

// an attempt of rotating the camera
float cameraYaw = -110.0f;
float cameraPitch = -54.0f;
float cameraRotate = 2.0f;

float lightX = 2.0f;
float lightY = 2.0f;
float lightZ = 0.0f;

// values for the lookAt matrix
glm::vec3 cameraEye = glm::vec3(2.0f, 5.0f, -2.0f);  // eye
glm::vec3 cameraCenter = glm::normalize(glm::vec3(
    cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch)),
    sin(glm::radians(cameraPitch)),
    sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch))
    ));   // center
glm::vec3 cameraGlobUp = glm::vec3(0.0f, 1.0f, 0.0f);     // up

float cameraSpeed = 0.5f;

// define a vertex array to hold our vertices
float vertices[] =
{
    // position (x, y, z), colours (r, g, b), normals (x, y, z), orientation

    // normal calculation = norm((B - A) cross (C - A))

    // pillar 1

    // segment 1, rear
    -0.50f, -0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // A
    -0.40f, 0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // B
    -0.40f, -0.2f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // C

    -0.50f, -0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    -0.50f, 0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    -0.40f, 0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,

    // segment 2, rear left
    -0.50f, -0.20f, -1.00f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,
    -0.55f, -0.20f, -0.90f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,
    -0.50f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,

    -0.55f, -0.20f, -0.90f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,
    -0.55f, 0.20f, -0.90f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,
    -0.50f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,

    // segment 3, front left
    -0.55f, -0.20f, -0.90f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,
    -0.50f, -0.20f, -0.80f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,
    -0.55f, 0.20f, -0.90f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,

    -0.55f, 0.20f, -0.90f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,
    -0.50f, -0.20f, -0.80f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,
    -0.50f, 0.20f, -0.80f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,

    // segment 4, front
    -0.50f, -0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    -0.40f, -0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    -0.40f, 0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,

    -0.50f, -0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    -0.40f, 0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    -0.50f, 0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,

    // segment 5, front right
    -0.40f, -0.20f, -0.80f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,
    -0.35f, -0.20f, -0.90f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,
    -0.40f, 0.20f, -0.80f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,

    -0.35f, -0.20f, -0.90f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,
    -0.35f, 0.20f, -0.90f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,
    -0.40f, 0.20f, -0.80f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,

    // segment 6, rear right
    -0.40f, -0.20f, -1.00f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,
    -0.40f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,
    -0.35f, -0.20f, -0.90f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,

    -0.35f, -0.20f, -0.90f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,
    -0.40f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,
    -0.35f, 0.20f, -0.90f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,

    // pillar 2

    // segment 1
    0.40f, -0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    0.50f, 0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    0.50f, -0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,

    0.40f, -0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    0.40f, 0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    0.50f, 0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,

    // segment 2
    0.40f, -0.20f, -1.00f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,
    0.35f, -0.20f, -0.90f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,
    0.40f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,

    0.35f, -0.20f, -0.90f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,
    0.35f, 0.20f, -0.90f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,
    0.40f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,

    // segment 3
    0.35f, -0.20f, -0.90f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,
    0.40f, -0.20f, -0.80f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,
    0.35f, 0.20f, -0.90f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,

    0.35f, 0.20f, -0.90f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,
    0.40f, -0.20f, -0.80f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,
    0.40f, 0.20f, -0.80f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,

    // segment 4
    0.40f, -0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    0.50f, -0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    0.50f, 0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,

    0.40f, -0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    0.50f, 0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    0.40f, 0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,

    // segment 5
    0.50f, -0.20f, -0.80f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,
    0.55f, -0.20f, -0.90f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,
    0.50f, 0.20f, -0.80f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,

    0.55f, -0.20f, -0.90f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,
    0.55f, 0.20f, -0.90f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,
    0.50f, 0.20f, -0.80f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,

    // segment 6
    0.50f, -0.20f, -1.00f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,
    0.50f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,
    0.55f, -0.20f, -0.90f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,

    0.55f, -0.20f, -0.90f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,
    0.50f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,
    0.55f, 0.20f, -0.90f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,

    // pillar 3

    // segment 1
    -0.20f, -0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    -0.10f, 0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    -0.10f, -0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,

    -0.20f, -0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    -0.20f, 0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    -0.10f, 0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,

    // segment 2
    -0.20f, -0.20f, -1.00f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,
    -0.25f, -0.20f, -0.90f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,
    -0.20f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,

    -0.25f, -0.20f, -0.90f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,
    -0.25f, 0.20f, -0.90f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,
    -0.20f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,

    // segment 3
    -0.25f, -0.20f, -0.90f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,
    -0.20f, -0.20f, -0.80f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,
    -0.25f, 0.20f, -0.90f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,

    -0.25f, 0.20f, -0.90f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,
    -0.20f, -0.20f, -0.80f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,
    -0.20f, 0.20f, -0.80f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,

    // segment 4
    -0.20f, -0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    -0.10f, -0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    -0.10f, 0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,

    -0.20f, -0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    -0.10f, 0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    -0.20f, 0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,

    // segment 5
    -0.10f, -0.20f, -0.80f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,
    -0.05f, -0.20f, -0.90f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,
    -0.10f, 0.20f, -0.80f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,

    -0.05f, -0.20f, -0.90f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,
    -0.05f, 0.20f, -0.90f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,
    -0.10f, 0.20f, -0.80f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,

    // segment 6
    -0.10f, -0.20f, -1.00f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,
    -0.10f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,
    -0.05f, -0.20f, -0.90f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,

    -0.05f, -0.20f, -0.90f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,
    -0.10f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,
    -0.05f, 0.20f, -0.90f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,

    // pillar 4

    // segment 1
    0.10f, -0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    0.20f, 0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    0.20f, -0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,

    0.10f, -0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    0.10f, 0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    0.20f, 0.20f, -1.00f, 1.00f, 1.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,

    // segment 2
    0.05f, -0.20f, -0.90f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,
    0.10f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,
    0.10f, -0.20f, -1.00f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,

    0.05f, -0.20f, -0.90f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,
    0.05f, 0.20f, -0.90f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,
    0.10f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, -0.89f, 0.00f, -0.45f, 0.00f,

    // segment 3
    0.05f, -0.20f, -0.90f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,
    0.10f, -0.20f, -0.80f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,
    0.05f, 0.20f, -0.90f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,

    0.05f, 0.20f, -0.90f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,
    0.10f, -0.20f, -0.80f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,
    0.10f, 0.20f, -0.80f, 1.00f, 0.00f, 1.00f, -0.89f, 0.00f, 0.45f, 0.00f,

    // segment 4
    0.10f, -0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    0.20f, -0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    0.20f, 0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,

    0.10f, -0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    0.20f, 0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    0.10f, 0.20f, -0.80f, 1.00f, 1.00f, 0.00f, 0.00f, 0.00f, 1.00f, 0.00f,

    // segment 5
    0.20f, -0.20f, -0.80f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,
    0.25f, -0.20f, -0.90f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,
    0.20f, 0.20f, -0.80f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,

    0.25f, -0.20f, -0.90f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,
    0.25f, 0.20f, -0.90f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,
    0.20f, 0.20f, -0.80f, 1.00f, 0.00f, 0.00f, 0.89f, -0.00f, 0.45f, 0.00f,

    // segment 6
    0.20f, -0.20f, -1.00f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,
    0.20f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,
    0.25f, -0.20f, -0.90f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,

    0.25f, -0.20f, -0.90f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,
    0.20f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,
    0.25f, 0.20f, -0.90f, 0.00f, 1.00f, 1.00f, 0.89f, 0.00f, -0.45f, 0.00f,

    // front triangle

    -0.65f, 0.20f, -0.80f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    0.65f, 0.20f, -0.80f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    0.00f, 0.40f, -0.80f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, 1.00f, 0.00f,

    // back triangle

    -0.65f, 0.20f, -1.00f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    0.00f, 0.40f, -1.00f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    0.65f, 0.20f, -1.00f, 0.00f, 0.00f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,

    // triangle edges

    -0.65f, 0.20f, -0.80f, 0.00f, 1.00f, 1.00f, -0.29f, 0.96f, 0.00f, 1.00f,
    0.00f, 0.40f, -0.80f, 0.00f, 1.00f, 1.00f, -0.29f, 0.96f, 0.00f, 1.00f,
    -0.65f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, -0.29f, 0.96f, 0.00f, 1.00f,

    0.00f, 0.40f, -0.80f, 0.00f, 1.00f, 1.00f, -0.29f, 0.96f, 0.00f, 1.00f,
    0.00f, 0.40f, -1.00f, 0.00f, 1.00f, 1.00f, -0.29f, 0.96f, 0.00f, 1.00f,
    -0.65f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, -0.29f, 0.96f, 0.00f, 1.00f,

    0.00f, 0.40f, -0.80f, 0.00f, 1.00f, 1.00f, 0.29f, 0.96f, 0.00f, 1.00f,
    0.65f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, 0.29f, 0.96f, 0.00f, 1.00f,
    0.00f, 0.40f, -1.00f, 0.00f, 1.00f, 1.00f, 0.29f, 0.96f, 0.00f, 1.00f,

    0.65f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, 0.29f, 0.96f, -0.00f, 1.00f,
    0.00f, 0.40f, -0.80f, 0.00f, 1.00f, 1.00f, 0.29f, 0.96f, -0.00f, 1.00f,
    0.65f, 0.20f, -0.80f, 0.00f, 1.00f, 1.00f, 0.29f, 0.96f, -0.00f, 1.00f,

    -0.65f, 0.20f, -0.80f, 0.00f, 1.00f, 1.00f, 0.00f, -1.00f, 0.00f, 1.00f,
    -0.65f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, 0.00f, -1.00f, 0.00f, 1.00f,
    0.65f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, 0.00f, -1.00f, 0.00f, 1.00f,

    0.65f, 0.20f, -1.00f, 0.00f, 1.00f, 1.00f, 0.00f, -1.00f, 0.00f, 1.00f,
    0.65f, 0.20f, -0.80f, 0.00f, 1.00f, 1.00f, 0.00f, -1.00f, 0.00f, 1.00f,
    -0.65f, 0.20f, -0.80f, 0.00f, 1.00f, 1.00f, 0.00f, -1.00f, 0.00f, 1.00f,

    // front base top

    -0.65f, -0.20f, -0.80f, 1.00f, 0.00f, 1.00f, -0.00f, 0.00f, 1.00f, 1.00f,
    0.65f, -0.25f, -0.80f, 1.00f, 0.00f, 1.00f, -0.00f, 0.00f, 1.00f, 1.00f,
    0.65f, -0.20f, -0.80f, 1.00f, 0.00f, 1.00f, -0.00f, 0.00f, 1.00f, 1.00f,

    0.65f, -0.25f, -0.80f, 1.00f, 0.00f, 1.00f, 0.00f, 0.00f, 1.00f, 1.00f,
    -0.65f, -0.20f, -0.80f, 1.00f, 0.00f, 1.00f, 0.00f, 0.00f, 1.00f, 1.00f,
    -0.65f, -0.25f, -0.80f, 1.00f, 0.00f, 1.00f, 0.00f, 0.00f, 1.00f, 1.00f,

    // back base top

    -0.65f, -0.20f, -1.00f, 1.00f, 0.00f, 1.00f, 0.00f, 0.00f, -1.00f, 1.00f,
    0.65f, -0.20f, -1.00f, 1.00f, 0.00f, 1.00f, 0.00f, 0.00f, -1.00f, 1.00f,
    0.65f, -0.25f, -1.00f, 1.00f, 0.00f, 1.00f, 0.00f, 0.00f, -1.00f, 1.00f,

    0.65f, -0.25f, -1.00f, 1.00f, 0.00f, 1.00f, 0.00f, 0.00f, -1.00f, 1.00f,
    -0.65f, -0.25f, -1.00f, 1.00f, 0.00f, 1.00f, 0.00f, 0.00f, -1.00f, 1.00f,
    -0.65f, -0.20f, -1.00f, 1.00f, 0.00f, 1.00f, 0.00f, 0.00f, -1.00f, 1.00f,

    // edges of base top

    -0.65f, -0.20f, -0.80f, 0.50f, 0.50f, 1.00f, 0.00f, 1.00f, 0.00f, 0.00f,
    0.65f, -0.20f, -1.00f, 0.50f, 0.50f, 1.00f, 0.00f, 1.00f, 0.00f, 0.00f,
    -0.65f, -0.20f, -1.00f, 0.50f, 0.50f, 1.00f, 0.00f, 1.00f, 0.00f, 0.00f,

    0.65f, -0.20f, -1.00f, 0.50f, 0.50f, 1.00f, 0.00f, 1.00f, -0.00f, 0.00f,
    -0.65f, -0.20f, -0.80f, 0.50f, 0.50f, 1.00f, 0.00f, 1.00f, -0.00f, 0.00f,
    0.65f, -0.20f, -0.80f, 0.50f, 0.50f, 1.00f, 0.00f, 1.00f, -0.00f, 0.00f,

    -0.65f, -0.25f, -0.80f, 0.50f, 0.00f, 0.75f, -1.00f, 0.00f, 0.00f, 0.00f,
    -0.65f, -0.20f, -0.80f, 0.50f, 0.00f, 0.75f, -1.00f, 0.00f, 0.00f, 0.00f,
    -0.65f, -0.20f, -1.00f, 0.50f, 0.00f, 0.75f, -1.00f, 0.00f, 0.00f, 0.00f,

    -0.65f, -0.20f, -1.00f, 0.50f, 0.00f, 0.75f, -1.00f, 0.00f, 0.00f, 0.00f,
    -0.65f, -0.25f, -1.00f, 0.50f, 0.00f, 0.75f, -1.00f, 0.00f, 0.00f, 0.00f,
    -0.65f, -0.25f, -0.80f, 0.50f, 0.00f, 0.75f, -1.00f, 0.00f, 0.00f, 0.00f,

    0.65f, -0.20f, -1.00f, 0.50f, 0.00f, 0.75f, 1.00f, 0.00f, -0.00f, 0.00f,
    0.65f, -0.20f, -0.80f, 0.50f, 0.00f, 0.75f, 1.00f, 0.00f, -0.00f, 0.00f,
    0.65f, -0.25f, -0.80f, 0.50f, 0.00f, 0.75f, 1.00f, 0.00f, -0.00f, 0.00f,

    0.65f, -0.25f, -0.80f, 0.50f, 0.00f, 0.75f, 1.00f, 0.00f, 0.00f, 0.00f,
    0.65f, -0.25f, -1.00f, 0.50f, 0.00f, 0.75f, 1.00f, 0.00f, 0.00f, 0.00f,
    0.65f, -0.20f, -1.00f, 0.50f, 0.00f, 0.75f, 1.00f, 0.00f, 0.00f, 0.00f,

    // front base bottom

    -0.75f, -0.25f, -0.75f, 0.50f, 0.50f, 1.00f, -0.00f, 0.00f, 1.00f, 0.00f,
    0.75f, -0.30f, -0.75f, 0.50f, 0.50f, 1.00f, -0.00f, 0.00f, 1.00f, 0.00f,
    0.75f, -0.25f, -0.75f, 0.50f, 0.50f, 1.00f, -0.00f, 0.00f, 1.00f, 0.00f,

    0.75f, -0.30f, -0.75f, 0.50f, 0.50f, 1.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    -0.75f, -0.25f, -0.75f, 0.50f, 0.50f, 1.00f, 0.00f, 0.00f, 1.00f, 0.00f,
    -0.75f, -0.30f, -0.75f, 0.50f, 0.50f, 1.00f, 0.00f, 0.00f, 1.00f, 0.00f,

    // back base bottom

    -0.75f, -0.25f, -1.00f, 0.50f, 0.50f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    0.75f, -0.25f, -1.00f, 0.50f, 0.50f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    0.75f, -0.30f, -1.00f, 0.50f, 0.50f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,

    0.75f, -0.30f, -1.00f, 0.50f, 0.50f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    -0.75f, -0.30f, -1.00f, 0.50f, 0.50f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,
    -0.75f, -0.25f, -1.00f, 0.50f, 0.50f, 1.00f, 0.00f, 0.00f, -1.00f, 0.00f,

    // edges of base bottom

    -0.75f, -0.25f, -0.75f, 0.50f, 0.00f, 1.00f, 0.00f, 1.00f, 0.00f, 1.00f,
    0.75f, -0.25f, -1.00f, 0.50f, 0.00f, 1.00f, 0.00f, 1.00f, 0.00f, 1.00f,
    -0.75f, -0.25f, -1.00f, 0.50f, 0.00f, 1.00f, 0.00f, 1.00f, 0.00f, 1.00f,

    0.75f, -0.25f, -1.00f, 0.50f, 0.00f, 1.00f, 0.00f, 1.00f, -0.00f, 1.00f,
    -0.75f, -0.25f, -0.75f, 0.50f, 0.00f, 1.00f, 0.00f, 1.00f, -0.00f, 1.00f,
    0.75f, -0.25f, -0.75f, 0.50f, 0.00f, 1.00f, 0.00f, 1.00f, -0.00f, 1.00f,

    -0.75f, -0.30f, -0.75f, 0.50f, 0.00f, 1.00f, 0.00f, -1.00f, 0.00f, 1.00f,
    -0.75f, -0.30f, -1.00f, 0.50f, 0.00f, 1.00f, 0.00f, -1.00f, 0.00f, 1.00f,
    0.75f, -0.30f, -1.00f, 0.50f, 0.00f, 1.00f, 0.00f, -1.00f, 0.00f, 1.00f,

    0.75f, -0.30f, -1.00f, 0.50f, 0.00f, 1.00f, 0.00f, -1.00f, 0.00f, 1.00f,
    0.75f, -0.30f, -0.75f, 0.50f, 0.00f, 1.00f, 0.00f, -1.00f, 0.00f, 1.00f,
    -0.75f, -0.30f, -0.75f, 0.50f, 0.00f, 1.00f, 0.00f, -1.00f, 0.00f, 1.00f,

    -0.75f, -0.30f, -0.75f, 0.50f, 0.00f, 1.00f, -1.00f, 0.00f, 0.00f, 1.00f,
    -0.75f, -0.25f, -0.75f, 0.50f, 0.00f, 1.00f, -1.00f, 0.00f, 0.00f, 1.00f,
    -0.75f, -0.25f, -1.00f, 0.50f, 0.00f, 1.00f, -1.00f, 0.00f, 0.00f, 1.00f,

    -0.75f, -0.25f, -1.00f, 0.50f, 0.00f, 1.00f, -1.00f, 0.00f, 0.00f, 1.00f,
    -0.75f, -0.30f, -1.00f, 0.50f, 0.00f, 1.00f, -1.00f, 0.00f, 0.00f, 1.00f,
    -0.75f, -0.30f, -0.75f, 0.50f, 0.00f, 1.00f, -1.00f, 0.00f, 0.00f, 1.00f,

    0.75f, -0.25f, -1.00f, 0.50f, 0.00f, 1.00f, 1.00f, 0.00f, -0.00f, 1.00f,
    0.75f, -0.25f, -0.75f, 0.50f, 0.00f, 1.00f, 1.00f, 0.00f, -0.00f, 1.00f,
    0.75f, -0.30f, -0.75f, 0.50f, 0.00f, 1.00f, 1.00f, 0.00f, -0.00f, 1.00f,

    0.75f, -0.30f, -0.75f, 0.50f, 0.00f, 1.00f, 1.00f, 0.00f, 0.00f, 1.00f,
    0.75f, -0.30f, -1.00f, 0.50f, 0.00f, 1.00f, 1.00f, 0.00f, 0.00f, 1.00f,
    0.75f, -0.25f, -1.00f, 0.50f, 0.00f, 1.00f, 1.00f, 0.00f, 0.00f, 1.00f,

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*) 0);                      // vertices
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*) (3 * sizeof(float)));    // rgb
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*) (6 * sizeof(float)));    // normals
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*) (9 * sizeof(float)));    // orientation

    // enable the newly-created layout location 0;
    // this shall be used by our vertex shader to read the vertex's x, y, and z
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    // important: if you have more vertex arrays to draw, make sure you separately define them
    // with unique VAO and VBO IDs, and follow the same process above to upload them to the GPU

    // load our shader program
    shader = gdevLoadShader("exercise3.vs", "exercise3.fs");
    if (! shader)
        return false;
    texture = gdevLoadTexture("e3tex1.png", GL_REPEAT, true, true);
    if (! texture)
        return false;
    return true;
}

// called by the main function to do rendering per frame
void render()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Calculate projection-view matrix
    // (combine glm::perspective transform with glm::lookAt transform)
    glm::mat4 projectionViewMatrix;
    projectionViewMatrix = glm::perspective(glm::radians(60.0f), (float) WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
    projectionViewMatrix *= glm::lookAt(
        cameraEye,
        cameraEye + cameraCenter,
        cameraGlobUp
    );

    // clear the whole frame
    glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // using our shader program...
    glUseProgram(shader);

    // Bind VAO
    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Draw three copies...
    for(int i = 0; i < 3; i++){
        // Calculate model matrix
        glm::mat4 modelMatrix = glm::mat4(1.0f); // set to identity first!

        // then translate...
        float tx[3] = {-3.0f, 0.0f, 3.0f};
        modelMatrix = glm::translate(modelMatrix, glm::vec3(tx[i], 0.0f, -5.0f));

        // then rotate...
        float time = (float)glfwGetTime();
        glm::vec3 axis;
        if(i == 0) axis = glm::vec3(1.0f, 0.0f, 0.0f); // X-axis
        else if(i == 1) axis = glm::vec3(0.0f, 1.0f, 0.0f); // Y-axis
        else axis = glm::vec3(0.0f, 0.0f, 1.0f); // Z-axis
        modelMatrix = glm::rotate(modelMatrix, time * 1.0f, axis);

        // and scale
        float scales[3] = {1.0f, 0.7f, 1.3f};
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scales[i], scales[i], scales[i]));

        // Calculate normal matrix (transpose of inverse of model matrix)
        glm::mat4 normalMatrix;
        normalMatrix = glm::transpose(glm::inverse(modelMatrix));

        glm::vec3 lightPosition = glm::vec3(lightX, lightY, lightZ);

        // Pass the three matrices to the vertex shader
        glUniformMatrix4fv(glGetUniformLocation(shader, "projectionViewMatrix"), 1, GL_FALSE, glm::value_ptr(projectionViewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shader, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shader, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
        glUniform3fv(glGetUniformLocation(shader, "lightPosition"), 1, glm::value_ptr(lightPosition));

        // Draw the model
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (10 * sizeof(float)));
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

    // pressing K will move the light to the right. Add shift, and it moves to the left.
    if (key == GLFW_KEY_K && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        if (mode & GLFW_MOD_SHIFT) {
            lightX -= 10.0f;
        } else {
            lightX += 10.0f;
        }
    }

    // pressing I will move the light down. Add shift, and it moves up.
    if (key == GLFW_KEY_I && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        if (mode & GLFW_MOD_SHIFT) {
            lightY -= 10.0f;
        } else {
            lightY += 10.0f;
        }
    }

    // pressing J will move the light forward. Add shift, and it moves back.
    if (key == GLFW_KEY_J && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        if (mode & GLFW_MOD_SHIFT) {
            lightZ -= 10.0f;
        } else {
            lightZ += 10.0f;
        }
    }
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
