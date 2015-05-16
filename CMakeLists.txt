cmake_minimum_required(VERSION 2.8)

project(DarwinPP CXX)

#target_compile_features(DarwinPP PRIVATE cxx_generic_lambdas cxx_constexpr)

add_subdirectory (src) 
add_subdirectory(tests)
enable_testing ()
add_test (NAME MyTest COMMAND Test)

