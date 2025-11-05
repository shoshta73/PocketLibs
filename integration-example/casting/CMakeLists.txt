cmake_minimum_required(VERSION 3.25)
project(ExprEval VERSION 1.0.0 LANGUAGES CXX)

set(CASTING_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/casting.hxx")
if(NOT EXISTS "${CASTING_HEADER}")
        message(STATUS "Downloading casting.hxx...")
        file(
                DOWNLOAD
                "https://raw.githubusercontent.com/shoshta73/PocketLibs/main/casting.hxx"
                "${CASTING_HEADER}"
                SHOW_PROGRESS
        )
endif()

add_executable(expr_eval src/main.cxx)

target_compile_features(
        expr_eval
        PRIVATE
        cxx_std_23
)

target_include_directories(expr_eval PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(expr_eval PRIVATE CASTING_NAMESPACE=ExprEval)

target_compile_options(expr_eval PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-fno-rtti>
        $<$<CXX_COMPILER_ID:MSVC>:/GR->
)

if(MSVC)
        target_compile_options(expr_eval PRIVATE /W4 /Zc:__cplusplus)
else()
        target_compile_options(expr_eval PRIVATE -Wall -Wextra -Wpedantic)
endif()
