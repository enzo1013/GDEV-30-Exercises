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
    // x, y, z, r, g, b, s, t, ID tag

    // super uber simplified, now made even easier to manage

    // the pieces here are shifted. read original_vertices.txt for
    // the pieced-up picture.

    // lower part, part 1 (the large triangle nearest to bottom left of picture) (change to grape purple?)

    -0.900f, -0.500f, 0.00f, 0.58f, 0.29f, 0.70f, 0.0000f, 0.2500f, 7.0f,
    -0.900f, -0.900f, 0.00f, 0.58f, 0.29f, 0.70f, 0.0000f, 0.0000f, 7.0f,
    0.400f, -0.900f, 0.00f, 0.58f, 0.29f, 0.70f, 0.8125f, 0.0000f, 7.0f,

    // lower part, part 2 (the small triangle nearest to sky and head, part 4) (change to dark green)

    0.270f, -0.470f, 0.00f, 0.00f, 0.31f, 0.22f, 0.6063f, 0.2687f, 8.0f,
    0.120f, -0.760f, 0.00f, 0.00f, 0.31f, 0.22f, 0.5125f, 0.0875f, 8.0f,
    0.600f, -0.900f, 0.00f, 0.00f, 0.31f, 0.22f, 0.8125f, 0.0000f, 8.0f,

    0.270f, -0.470f, 0.00f, 0.00f, 0.31f, 0.22f, 0.6063f, 0.2687f, 8.0f,
    0.600f, -0.900f, 0.00f, 0.00f, 0.31f, 0.22f, 0.8125f, 0.0000f, 8.0f,
    0.900f, -0.900f, 0.00f, 0.00f, 0.31f, 0.22f, 1.0000f, 0.0000f, 8.0f,

    // sky (change color to indigo)

    0.370f, -0.170f, 0.00f, 0.29f, 0.00f, 0.51f, 0.6063f, 0.2687f, 0.0f,
    1.000f, -0.600f, 0.00f, 0.29f, 0.00f, 0.51f, 1.0000f, 0.0000f, 0.0f,
    0.820f, -0.120f, 0.00f, 0.29f, 0.00f, 0.51f, 0.8875f, 0.3000f, 0.0f,

    0.820f, -0.120f, 0.00f, 0.29f, 0.00f, 0.51f, 0.8875f, 0.3000f, 0.0f,
    0.470f, 0.290f, 0.00f, 0.29f, 0.00f, 0.51f, 0.6687f, 0.5563f, 0.0f,
    0.405f, 0.090f, 0.00f, 0.29f, 0.00f, 0.51f, 0.6281f, 0.4313f, 0.0f,

    0.820f, -0.120f, 0.00f, 0.29f, 0.00f, 0.51f, 0.8875f, 0.3000f, 0.0f,
    0.760f, 0.620f, 0.00f, 0.29f, 0.00f, 0.51f, 0.8500f, 0.7625f, 0.0f,
    0.470f, 0.290f, 0.00f, 0.29f, 0.00f, 0.51f, 0.6687f, 0.5563f, 0.0f,

    0.760f, 0.620f, 0.00f, 0.29f, 0.00f, 0.51f, 0.8500f, 0.7625f, 0.0f,
    1.000f, 1.000f, 0.00f, 0.29f, 0.00f, 0.51f, 1.0000f, 1.0000f, 0.0f,
    0.475f, 0.610f, 0.00f, 0.29f, 0.00f, 0.51f, 0.6719f, 0.7562f, 0.0f,

    1.000f, 1.000f, 0.00f, 0.29f, 0.00f, 0.51f, 1.0000f, 1.0000f, 0.0f,
    0.360f, 0.910f, 0.00f, 0.29f, 0.00f, 0.51f, 0.6000f, 0.9437f, 0.0f,
    0.475f, 0.610f, 0.00f, 0.29f, 0.00f, 0.51f, 0.6719f, 0.7562f, 0.0f,

    1.000f, 1.000f, 0.00f, 0.29f, 0.00f, 0.51f, 1.0000f, 1.0000f, 0.0f,
    0.405f, 0.965f, 0.00f, 0.29f, 0.00f, 0.51f, 0.6281f, 0.9781f, 0.0f,
    0.360f, 0.910f, 0.00f, 0.29f, 0.00f, 0.51f, 0.6000f, 0.9437f, 0.0f,

    1.000f, 1.000f, 0.00f, 0.29f, 0.00f, 0.51f, 1.0000f, 1.0000f, 0.0f,
    0.360f, 1.000f, 0.00f, 0.29f, 0.00f, 0.51f, 0.6000f, 1.0000f, 0.0f,
    0.405f, 0.965f, 0.00f, 0.29f, 0.00f, 0.51f, 0.6281f, 0.9781f, 0.0f,

    1.000f, 1.000f, 0.00f, 0.29f, 0.00f, 0.51f, 1.0000f, 1.0000f, 0.0f,
    0.760f, 0.620f, 0.00f, 0.29f, 0.00f, 0.51f, 0.8500f, 0.7625f, 0.0f,
    0.820f, -0.120f, 0.00f, 0.29f, 0.00f, 0.51f, 0.8875f, 0.3000f, 0.0f,

    1.000f, 1.000f, 0.00f, 0.29f, 0.00f, 0.51f, 1.0000f, 1.0000f, 0.0f,
    0.820f, -0.120f, 0.00f, 0.29f, 0.00f, 0.51f, 0.8875f, 0.3000f, 0.0f,
    1.000f, -0.600f, 0.00f, 0.29f, 0.00f, 0.51f, 1.0000f, 0.0000f, 0.0f,

    // head, part 1 (the part of the head nearest to the left edge) (change to red)

    -0.500f, 0.800f, 0.00f, 1.00f, 0.45f, 0.46f, 0.2500f, 1.0000f, 1.0f,
    -0.900f, 0.600f, 0.00f, 1.00f, 0.45f, 0.46f, 0.0000f, 0.8750f, 1.0f,
    -0.900f, -0.400f, 0.00f, 1.00f, 0.45f, 0.46f, 0.0000f, 0.2500f, 1.0f,

    // head, part 2 (the left center part of the head) (change to orange)

    -0.400f, 0.800f, 0.00f, 1.00f, 0.65f, 0.00f, 0.2500f, 1.0000f, 2.0f,
    -0.800f, -0.400f, 0.00f, 1.00f, 0.65f, 0.00f, 0.0000f, 0.2500f, 2.0f,
    0.020f, -0.660f, 0.00f, 1.00f, 0.65f, 0.00f, 0.5125f, 0.0875f, 2.0f,

    // head, part 3 (the right center of the head) (change to yellow)

    -0.400f, 0.900f, 0.00f, 1.00f, 1.00f, 0.00f, 0.2500f, 1.0000f, 3.0f,
    0.020f, -0.560f, 0.00f, 1.00f, 1.00f, 0.00f, 0.5125f, 0.0875f, 3.0f,
    0.080f, 0.900f, 0.00f, 1.00f, 1.00f, 0.00f, 0.5500f, 1.0000f, 3.0f,

    // head, part 4 (the right side of the head nearest to sky, also has cigarette) (change to yellow-green)

    0.180f, 1.000f, 0.00f, 0.65f, 0.80f, 0.20f, 0.5500f, 1.0000f, 4.0f,
    0.375f, 0.610f, 0.00f, 0.65f, 0.80f, 0.20f, 0.6719f, 0.7562f, 4.0f,
    0.260f, 0.910f, 0.00f, 0.65f, 0.80f, 0.20f, 0.6000f, 0.9437f, 4.0f,

    0.375f, 0.610f, 0.00f, 0.65f, 0.80f, 0.20f, 0.6719f, 0.7562f, 4.0f,
    0.370f, 0.290f, 0.00f, 0.65f, 0.80f, 0.20f, 0.6687f, 0.5563f, 4.0f,
    0.660f, 0.620f, 0.00f, 0.65f, 0.80f, 0.20f, 0.8500f, 0.7625f, 4.0f,

    0.180f, 1.000f, 0.00f, 0.65f, 0.80f, 0.20f, 0.5500f, 1.0000f, 4.0f,
    0.370f, 0.290f, 0.00f, 0.65f, 0.80f, 0.20f, 0.6687f, 0.5563f, 4.0f,
    0.260f, 0.910f, 0.00f, 0.65f, 0.80f, 0.20f, 0.6000f, 0.9437f, 4.0f,

    0.260f, 0.910f, 0.00f, 0.65f, 0.80f, 0.20f, 0.6000f, 0.9437f, 4.0f,
    0.370f, 0.290f, 0.00f, 0.65f, 0.80f, 0.20f, 0.6687f, 0.5563f, 4.0f,
    0.375f, 0.610f, 0.00f, 0.65f, 0.80f, 0.20f, 0.6719f, 0.7562f, 4.0f,

    0.180f, 1.000f, 0.00f, 0.65f, 0.80f, 0.20f, 0.5500f, 1.0000f, 4.0f,
    0.120f, -0.460f, 0.00f, 0.65f, 0.80f, 0.20f, 0.5125f, 0.0875f, 4.0f,
    0.370f, 0.290f, 0.00f, 0.65f, 0.80f, 0.20f, 0.6687f, 0.5563f, 4.0f,

    0.370f, 0.290f, 0.00f, 0.65f, 0.80f, 0.20f, 0.6687f, 0.5563f, 4.0f,
    0.120f, -0.460f, 0.00f, 0.65f, 0.80f, 0.20f, 0.5125f, 0.0875f, 4.0f,
    0.270f, -0.170f, 0.00f, 0.65f, 0.80f, 0.20f, 0.6063f, 0.2687f, 4.0f,

    0.305f, 0.090f, 0.00f, 0.65f, 0.80f, 0.20f, 0.6281f, 0.4313f, 4.0f,
    0.120f, -0.460f, 0.00f, 0.65f, 0.80f, 0.20f, 0.5125f, 0.0875f, 4.0f,
    0.270f, -0.170f, 0.00f, 0.65f, 0.80f, 0.20f, 0.6063f, 0.2687f, 4.0f,

    0.305f, 0.090f, 0.00f, 0.65f, 0.80f, 0.20f, 0.6281f, 0.4313f, 4.0f,
    0.270f, -0.170f, 0.00f, 0.65f, 0.80f, 0.20f, 0.6063f, 0.2687f, 4.0f,
    0.720f, -0.120f, 0.00f, 0.65f, 0.80f, 0.20f, 0.8875f, 0.3000f, 4.0f,

    // large hat segment (the one nearest to upper left of image) (change to magenta)

    -0.950f, 0.950f, 0.00f, 1.00f, 0.00f, 1.00f, 0.0000f, 1.0000f, 5.0f,
    -0.950f, 0.750f, 0.00f, 1.00f, 0.00f, 1.00f, 0.0000f, 0.8750f, 5.0f,
    -0.550f, 0.950f, 0.00f, 1.00f, 0.00f, 1.00f, 0.2500f, 1.0000f, 5.0f,

    // small hat segment (the small one nearest to head, part 4 and sky) (change to cyan)

    0.230f, 1.000f, 0.00f, 0.00f, 1.00f, 1.00f, 0.5500f, 1.0000f, 6.0f,
    0.310f, 0.910f, 0.00f, 0.00f, 1.00f, 1.00f, 0.6000f, 0.9437f, 6.0f,
    0.310f, 1.000f, 0.00f, 0.00f, 1.00f, 1.00f, 0.6000f, 1.0000f, 6.0f,

    0.310f, 1.000f, 0.00f, 0.00f, 1.00f, 1.00f, 0.6000f, 1.0000f, 6.0f,
    0.310f, 0.910f, 0.00f, 0.00f, 1.00f, 1.00f, 0.6000f, 0.9437f, 6.0f,
    0.355f, 0.965f, 0.00f, 0.00f, 1.00f, 1.00f, 0.6281f, 0.9781f, 6.0f,
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (6 * sizeof(float)));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (8 * sizeof(float)));

    // enable the newly-created layout location 0;
    // this shall be used by our vertex shader to read the vertex's x, y, and z
    glEnableVertexAttribArray(0);

    // enable the newly-created layout location 1;
    // this shall be used by our vertex shader to read the vertex's r, g, and b
    glEnableVertexAttribArray(1);

    // enable the newly-created layout location 2;
    // this shall be used by our vertex shader to read the vertex's s and t (texture coordinates)
    glEnableVertexAttribArray(2);

    // enable the newly-created layout location 3;
    // this shsall be used by our vertex shader to read the vertex's ID tag
    glEnableVertexAttribArray(3);

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

    float time = (float)(glfwGetTime());

    // using our shader program...
    glUseProgram(shader);

    // ... set the uniform variables of the shader...
    glUniform1f(glGetUniformLocation(shader, "time"), time);

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
