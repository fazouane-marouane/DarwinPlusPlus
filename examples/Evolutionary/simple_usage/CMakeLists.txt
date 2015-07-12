find_package( Eigen3 REQUIRED )
include_directories (${DarwinPP_SOURCE_DIR}/include ${EIGEN_INCLUDE_DIR} )

file(GLOB_RECURSE Darwin_INC
    "${DarwinPP_SOURCE_DIR}/include/*.h"
)

file(GLOB_RECURSE Example_Evolutionary_Simple_Usage_SRC
    "*.h"
    "*.cpp"
)

add_executable (example_Evolutionary_simple_usage ${Darwin_INC} ${Example_Evolutionary_Simple_Usage_SRC})

target_link_libraries(example_Evolutionary_simple_usage
                       Darwin)
