cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME Tetris3D)

project(${PROJECT_NAME})

set(PROJECT_INCLUDES
    src/rendering/drawable_object.h
    src/rendering/camera.h

    src/data_structures/matrix/matrix.h
    src/data_structures/array/array.h
    src/data_structures/dynamic_array/dynamic_array.h
    src/data_structures/queue/queue.h

    src/event_handling/input_keys.h
    src/event_handling/event_handler.h
)

set(PROJECT_SOURCES
    src/rendering/drawable_object.c
    src/rendering/camera.c

    src/data_structures/matrix/matrix.c
    src/data_structures/array/array.c
    src/data_structures/dynamic_array/dynamic_array.c
    src/data_structures/queue/queue.c

    src/event_handling/event_handler.c
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
