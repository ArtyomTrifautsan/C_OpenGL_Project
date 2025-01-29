#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define _USE_MATH_DEFINES
#include <math.h>

// #include "rendering/drawable_object.h"
#include "rendering/camera.h"
#include "rendering/shader_program.h"

#include "resource_managing/load_model.h"
#include "rendering/Model3D.h"

#include "data_structures/matrix/matrix.h"

#include "event_handling/event_handler.h"
#include "event_handling/input_keys.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);






//====================
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "data_structures/dynamic_array/dynamic_array_float.h"
#include "data_structures/dynamic_array/dynamic_array_int.h"

typedef struct Triangle {
    float x, y, z;
    unsigned int VBO, VAO, EBO;
    DynamicArrayFloat vertices;
    DynamicArrayInt indices;
} Triangle;

void create_triangle(Triangle* triangle);
void render_triangle(Triangle* triangle, ShaderProgram shader_program);

Triangle test_triangle;
//====================



// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// EVENT HANDLER
EventHandler event_handler;

// Shader Program
ShaderProgram default_shader_program;

// Model
Model3D tree_model;


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

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);


    event_handler = create_event_handler();

    // RENDER OBJECTS
    default_shader_program = load_shaders(
        "Resources/Shaders/default_vertex_shader.txt",
        "Resources/Shaders/default_fragment_shader.txt"
    );
    // default_shader_program = load_shaders("1.txt", "2.txt");

    Camera camera;
    camera = create_camera();
    set_camera_coords(&camera, 0.0f, 0.0f, -1.0f);


    tree_model = load_OBJ("1.obj", "Resources/Models/");
    set_model_scale(&tree_model, 0.2, 0.2, 0.2);

    // Meshes
    // for (int i = 0; i < tree_model.meshes.size; i++)
    // for (int i = 0; i < 1; i++)
    // {
    //     print_mesh(&tree_model.meshes.array[i]);
    // }


    // create_triangle(&test_triangle);


    // render loop
    // -----------
    int flag = 1;
    float step = 0.05f;
    float angle_step = 1.0f;
    float camera_z_pos = 1.0f;
    float camera_y_rotate = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);


        if (is_keyboard_key_pressed(&event_handler, KEY_A) || is_keyboard_key_repeated(&event_handler, KEY_A))
        {
            // camera.x -= step;
            // set_camera_coord_x(&camera, camera.x + step);
            set_model_coord_x(&tree_model, tree_model.x - step);
            
            // printf("GLFW PRESS: a\n");
        }
        if (is_keyboard_key_pressed(&event_handler, KEY_D) || is_keyboard_key_repeated(&event_handler, KEY_D))
        {
            // camera.x += step;
            // set_camera_coord_x(&camera, camera.x - step);
            set_model_coord_x(&tree_model, tree_model.x + step);
            // printf("GLFW PRESS: d\n");
        }

        if (is_keyboard_key_pressed(&event_handler, KEY_Q) || is_keyboard_key_repeated(&event_handler, KEY_Q))
        {
            // camera.y -= step;
            // set_camera_coord_y(&camera, camera.y - step);
            set_model_coord_y(&tree_model, tree_model.y + step);
            // printf("GLFW PRESS: q\n");
        }
        if (is_keyboard_key_pressed(&event_handler, KEY_E) || is_keyboard_key_repeated(&event_handler, KEY_E))
        {
            // camera.y += step;
            // set_camera_coord_y(&camera, camera.y + step);
            set_model_coord_y(&tree_model, tree_model.y - step);
            // printf("GLFW PRESS: e\n");
        }

        if (is_keyboard_key_pressed(&event_handler, KEY_S) || is_keyboard_key_repeated(&event_handler, KEY_S))
        {
            // camera.z += step;
            // set_camera_coord_z(&camera, camera.z - step);
            set_model_coord_z(&tree_model, tree_model.z + step);
            // printf("GLFW PRESS: s\n");
        }
        if (is_keyboard_key_pressed(&event_handler, KEY_W) || is_keyboard_key_repeated(&event_handler, KEY_W))
        {
            // camera.z -= step;
            // set_camera_coord_z(&camera, camera.z + step);
            set_model_coord_z(&tree_model, tree_model.z - step);
            // printf("GLFW PRESS: w\n");
        }

        if (is_keyboard_key_pressed(&event_handler, KEY_UP) || is_keyboard_key_repeated(&event_handler, KEY_UP))
        {
            // camera.z -= step;
            // set_camera_rotate_x(&camera, camera.rotate_x + angle_step);
            set_model_rotate_x(&tree_model, tree_model.rotate_x - angle_step);
            // printf("GLFW PRESS: UP\n");
        }
        if (is_keyboard_key_pressed(&event_handler, KEY_DOWN) || is_keyboard_key_repeated(&event_handler, KEY_DOWN))
        {
            // camera.z -= step;
            // set_camera_rotate_x(&camera, camera.rotate_x - angle_step);
            set_model_rotate_x(&tree_model, tree_model.rotate_x + angle_step);
            // printf("GLFW PRESS: DOWN\n");
        }

        if (is_keyboard_key_pressed(&event_handler, KEY_LEFT) || is_keyboard_key_repeated(&event_handler, KEY_LEFT))
        {
            // camera.z -= step;
            // set_camera_rotate_y(&camera, camera.rotate_y + angle_step);
            set_model_rotate_y(&tree_model, tree_model.rotate_y - angle_step);
            // printf("GLFW PRESS: LEFT\n");
        }
        if (is_keyboard_key_pressed(&event_handler, KEY_RIGHT) || is_keyboard_key_repeated(&event_handler, KEY_RIGHT))
        {
            // camera.z -= step;
            // set_camera_rotate_y(&camera, camera.rotate_y - angle_step);
            set_model_rotate_y(&tree_model, tree_model.rotate_y + angle_step);
            // printf("GLFW PRESS: RIGHT\n");
        }

        clear_event_storage(&event_handler);

        // print_matrix(&camera.view_matrix);

        // if (flag > 600) flag = -610;
        // flag += 1;
        // camera_z_pos -= step * 10;
        // camera_y_rotate += step * 100;
        
        // set_camera_coords(&camera, 0.0f, 0.0f, camera_z_pos);
        // set_camera_rotate_y(&camera, camera_y_rotate);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        send_view_projection_matrix_to_shaders(&camera, default_shader_program);
        send_scene_parameters_to_shaders(&camera, default_shader_program);

        // render_triangle(&test_triangle, default_shader_program);
    
        render_model3D(&tree_model, default_shader_program);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------


    // delete_drawable_object(&square);

    glDeleteProgram(default_shader_program.shader_program);

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
        // printf("GLFW PRESS: %c\n", (char)key);
        push_to_keyboard_pressed(&event_handler, key);
        break;

    case GLFW_RELEASE:
        // printf("GLFW RELEASE: %c\n", (char)key);
        push_to_keyboard_released(&event_handler, key);
        break;

    case GLFW_REPEAT:
        // printf("GLFW REPEAT: %c\n", (char)key);
        push_to_keyboard_repeated(&event_handler, key);
        break;
    
    default:
        break;
    }
}


Model3D create_cube()
{
    DynamicArrayMesh meshes;
    meshes = create_darray_mesh(1);

    
}







void _create_vertices_indices(Triangle* triangle);

void create_triangle(Triangle* triangle)
{
    _create_vertices_indices(triangle);

    glGenVertexArrays(1, &triangle->VAO);
    glGenBuffers(1, &triangle->VBO);
    glGenBuffers(1, &triangle->EBO);

    glBindVertexArray(triangle->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, triangle->VBO); 
    glBufferData(GL_ARRAY_BUFFER, triangle->vertices.size * sizeof(float), triangle->vertices.array, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangle->indices.size * sizeof(unsigned int), triangle->indices.array, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void _create_vertices_indices(Triangle* triangle)
{
    triangle->vertices = create_darray_float(2);
    triangle->indices = create_darray_int(2);

    float triangle_pos[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    int triangle_indices[] = {1, 2, 3};

    for (int i = 0; i < 9; i++)
        add_element_darray_float(&triangle->vertices, triangle_pos[i]);

    for (int i = 0; i < 3; i++)
        add_element_darray_int(&triangle->indices, triangle_indices[i]);
}

void render_triangle(Triangle* triangle, ShaderProgram shader_program)
{
    glUseProgram(shader_program.shader_program);
    glBindVertexArray(triangle->VAO); 
    glDrawElements(GL_TRIANGLES, triangle->indices.size, GL_UNSIGNED_INT, NULL);
}