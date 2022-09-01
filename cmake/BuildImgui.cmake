find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_library(imgui STATIC)

target_sources(imgui PRIVATE
    "ext/imgui/imgui_demo.cpp"    
    "ext/imgui/imgui_draw.cpp"    
    "ext/imgui/imgui_tables.cpp"    
    "ext/imgui/imgui_widgets.cpp"    
    "ext/imgui/imgui.cpp"    
    # backends
    "ext/imgui/backends/imgui_impl_glfw.cpp"
    "ext/imgui/backends/imgui_impl_opengl3.cpp"
)

target_include_directories(imgui PUBLIC 
    "ext/imgui/backends"
    "ext/imgui"
)

target_link_libraries(imgui PRIVATE
    glfw
    OpenGL::GL
)