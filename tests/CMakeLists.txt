set(Boost_USE_STATIC_LIBS        ON) # only find static libs
#   set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package (Boost COMPONENTS unit_test_framework REQUIRED)

include_directories (${DarwinPP_SOURCE_DIR}/include
                     ${DarwinPP_SOURCE_DIR}/src
                     ${Boost_INCLUDE_DIRS}
                     )
#add_definitions (-DBOOST_TEST_DYN_LINK)


add_executable (Test test.cpp)
target_link_libraries (Test
                       Darwin
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       )
