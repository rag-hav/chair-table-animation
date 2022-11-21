cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF) # use c++14 not gnu c++14
set(BUILD_SHARED_LIBS OFF)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

project(ChairTableAnimation) 

set(OpenGL_GL_PREFERENCE GLVND) # specify which opengl library we prefer
find_package(OpenGL REQUIRED)

add_subdirectory(lib/glsdk)

set(RC_DEPENDS "")

function(add_resources input)
    string(MAKE_C_IDENTIFIER ${input} input_identifier)
    message("\ninput: ${input}")
    message("input_identifier: ${input_identifier}")
    set(output "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${input_identifier}.o")
    message("output: ${output}")
    add_custom_command(
        OUTPUT ${output}
        COMMAND ${CMAKE_LINKER} --relocatable --format binary --output ${output} ${input}
        DEPENDS ${input}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    target_link_libraries(${PROJECT_NAME} ${output})

    set(RC_DEPENDS "${RC_DEPENDS}" "${output}" PARENT_SCOPE)
endfunction()
 

add_executable(${PROJECT_NAME} src/main.cpp src/model.cpp src/utils.cpp src/camera.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} glload glimg glutil freeglut OpenGL::GL X11)

# Resource file list
add_resources(data/chair.obj)
add_resources(data/table.obj)
add_resources(shader/vertex.vert)
add_resources(shader/fragment.frag)

message(${RC_DEPENDS})
add_custom_target(resources ALL DEPENDS ${RC_DEPENDS})
add_dependencies(${PROJECT_NAME} resources)



set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)
