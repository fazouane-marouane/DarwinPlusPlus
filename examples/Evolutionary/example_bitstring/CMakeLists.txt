include_directories (${DarwinPP_SOURCE_DIR}/include)

file(GLOB_RECURSE Darwin_INC
    "${DarwinPP_SOURCE_DIR}/include/*.h"
)

file(GLOB_RECURSE Example_Evolutionary_Bitstring_SRC
    "*.h"
    "*.cpp"
)

add_executable (example_Evolutionary_bitstring ${Darwin_INC} ${Example_Evolutionary_Bitstring_SRC})

target_link_libraries(example_Evolutionary_bitstring
                       Darwin)
