include_directories (${DarwinPP_SOURCE_DIR}/include)

file(GLOB Darwin_SRC
    "*.h"
    "*.cpp"
)

add_library (Darwin ${Darwin_SRC})
#add_executable (demo main.cpp)
#target_link_libraries (demo Sqr)
