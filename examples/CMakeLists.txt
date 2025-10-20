add_executable(
        casting_example
)

target_sources(
        casting_example
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/casting_example.cxx
)

target_include_directories(
        casting_example
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_compile_features(casting_example PRIVATE cxx_std_23)
