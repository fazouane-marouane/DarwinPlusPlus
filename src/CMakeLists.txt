find_package (Boost)
include_directories (${DarwinPP_SOURCE_DIR}/include
${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE Darwin_INC
    "${DarwinPP_SOURCE_DIR}/include/*.h"
    "${DarwinPP_SOURCE_DIR}/include/*.cpp"
)

file(GLOB_RECURSE Darwin_SRC
    "*.h"
    "*.cpp"
)

add_library (Darwin ${Darwin_INC} ${Darwin_SRC})
