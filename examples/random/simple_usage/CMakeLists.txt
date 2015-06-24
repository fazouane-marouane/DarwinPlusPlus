include_directories (${DarwinPP_SOURCE_DIR}/include)

file(GLOB_RECURSE Darwin_INC
    "${DarwinPP_SOURCE_DIR}/include/*.h"
)

file(GLOB_RECURSE Example_Random_Simple_Usage_SRC
    "*.h"
    "*.cpp"
)

add_executable (example_Random_simple_usage ${Darwin_INC} ${Example_Random_Simple_Usage_SRC})

target_link_libraries(example_Random_simple_usage
                       Darwin)
