find_package (Boost)
find_package( Eigen3 REQUIRED )
include_directories (${DarwinPP_SOURCE_DIR}/include
${DarwinPP_SOURCE_DIR}/src
${EIGEN_INCLUDE_DIR}
${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE Darwin_INC
    "${DarwinPP_SOURCE_DIR}/include/*.h"
)

file(GLOB_RECURSE Example_Evolutionary_Permutation_SRC
    "*.h"
    "*.cpp"
)

add_executable (example_Evolutionary_permutation ${Darwin_INC} ${Example_Evolutionary_Permutation_SRC})

target_compile_definitions(example_Evolutionary_permutation PRIVATE DARWIN_INLINE=1)

target_link_libraries(example_Evolutionary_permutation 
                       Darwin)
