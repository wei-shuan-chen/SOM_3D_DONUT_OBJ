cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project("SOM")
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
add_subdirectory(ext/glm)

add_executable(SOM)

include("cmake/BuildGlad.cmake")
include("cmake/BuildImgui.cmake")

set_target_properties(SOM
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

target_sources(SOM PRIVATE
    "src/main.cpp"
    "src/world.cpp"
    "src/matrixStack.cpp"
    "src/OBJmodel.cpp"
    "src/SOM.cpp"
)
target_sources(glad PRIVATE
    "ext/glad/src/glad.c"
)

target_include_directories(SOM PRIVATE
    "include/"
)
target_include_directories(glad PUBLIC
    "ext/glad/include"
)

target_link_libraries(SOM PRIVATE 
    glad # include(BuildGlad.cmake)
    imgui # include(BuildImgui.cmake)
    glm::glm # add_subdirectory(ext/glm)
    glfw  # find_package(glfw3 REQUIRED)
    OpenGL::GL # find_package(OpenGL REQUIRED)
)

add_custom_command(TARGET SOM POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_CURRENT_SOURCE_DIR}/shader"
        "$<TARGET_FILE_DIR:SOM>/shader"
    VERBATIM
)


add_custom_command(TARGET SOM POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_CURRENT_SOURCE_DIR}/obj"
        "$<TARGET_FILE_DIR:SOM>/obj"
    VERBATIM
)