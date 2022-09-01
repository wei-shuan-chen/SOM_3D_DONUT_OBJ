add_library(glad STATIC)

target_sources(glad PRIVATE "ext/glad/src/glad.c")
target_include_directories(glad PUBLIC "ext/glad/include")