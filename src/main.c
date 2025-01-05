#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "rendering/drawable_object.h"
#include "rendering/camera.h"

#include "data_structures/matrix/matrix.h"

#include "input_keys.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    unsigned int shaderProgram = load_shaders();

    Camera camera;
    camera = create_camera();
    set_camera_coords(&camera, 0.0f, 0.0f, -1.0f);

    DrawableObject square;
    create_drawable_object(&square);


    // render loop
    // -----------
    int flag = 1;
    float step = 0.0005;
    float camera_z_pos = 0.0f;
    float camera_y_rotate = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        

        if (flag > 600) flag = -610;
        flag += 1;
        camera_z_pos -= step * 10;
        camera_y_rotate += step * 100;
        
        set_camera_coords(&camera, 0.0f, 0.0f, camera_z_pos);
        // set_camera_rotate_y(&camera, camera_y_rotate);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        send_view_projection_matrix_to_shaders(&camera, shaderProgram);
        render_drawable_object(&square, shaderProgram);

 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------


    delete_drawable_object(&square);

    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (action)
    {
    case GLFW_PRESS:
        printf("GLFW PRESS: %c\n", (char)key);
        break;

    case GLFW_RELEASE:
        printf("GLFW RELEASE: %c\n", (char)key);
        break;

    case GLFW_REPEAT:
        printf("GLFW REPEAT: %c\n", (char)key);
        break;
    
    default:
        break;
    }
}