cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME Tetris3D)

project(${PROJECT_NAME})

set(PROJECT_INCLUDES
    src/rendering/camera.h
    src/rendering/material.h
    src/rendering/mesh.h
    src/rendering/Model3D.h
    src/rendering/shader_program.h

    src/data_structures/matrix/matrix.h
    src/data_structures/array/array.h
    src/data_structures/queue/queue.h
    src/data_structures/dynamic_array/dynamic_array_int.h
    src/data_structures/dynamic_array/dynamic_array_float.h
    src/data_structures/dynamic_array/dynamic_array_material.h
    src/data_structures/dynamic_array/dynamic_array_mesh.h
    src/data_structures/dynamic_array/dynamic_array_string.h

    src/event_handling/input_keys.h
    src/event_handling/event_handler.h

    src/resource_managing/load_model.h
)

set(PROJECT_SOURCES
    src/rendering/camera.c
    src/rendering/Model3D.c
    src/rendering/material.c
    src/rendering/mesh.c
    src/rendering/shader_program.c

    src/data_structures/matrix/matrix.c
    src/data_structures/array/array.c
    src/data_structures/queue/queue.c
    src/data_structures/dynamic_array/dynamic_array_int.c
    src/data_structures/dynamic_array/dynamic_array_float.c
    src/data_structures/dynamic_array/dynamic_array_material.c
    src/data_structures/dynamic_array/dynamic_array_mesh.c
    src/data_structures/dynamic_array/dynamic_array_string.c

    src/event_handling/event_handler.c

    src/resource_managing/load_model.c
)

add_executable(${PROJECT_NAME}
    src/main.c
    ${PROJECT_INCLUDES}
    ${PROJECT_SOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_subdirectory(external/glfw ${CMAKE_CURRENT_BINARY_DIR/glfw})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

add_subdirectory(external/glad ${CMAKE_CURRENT_BINARY_DIR/glad})
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

add_subdirectory(external/glm ${CMAKE_CURRENT_BINARY_DIR/glm})
target_link_libraries(${PROJECT_NAME} PRIVATE glm)

add_subdirectory(external/spdlog ${CMAKE_CURRENT_BINARY_DIR/spdlog})
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/Resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources)