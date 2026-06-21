cmake_minimum_required(VERSION 3.30)
set(VCPKG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/glt/vcpkg")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON)
project(tf2scanner)

file(GLOB_RECURSE all_files CONFIGURE_DEPENDS "src/*.cpp" "src/*.cc" "src/*.h")

add_executable(tf2scanner main.cpp ${all_files})
target_compile_features(tf2scanner PUBLIC cxx_std_23)
set_target_properties(tf2scanner PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(tf2scanner PUBLIC src)

if (MSVC)
    # target_compile_options(tf2scanner PUBLIC /fsanitize=address)
    target_compile_options(tf2scanner PUBLIC /std:c++latest /bigobj)
    target_compile_definitions(tf2scanner PUBLIC _DISABLE_STRING_ANNOTATION _DISABLE_VECTOR_ANNOTATION)
else ()
    target_compile_options(tf2scanner PUBLIC -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Wno-c++11-narrowing -Wno-narrowing -Werror)
    # target_link_options(tf2scanner PUBLIC -fsanitize=address,undefined)
    # target_link_options(tf2scanner PUBLIC -fsanitize=undefined,thread)
endif ()

add_subdirectory(glt)
include_directories(glt/include)
target_link_libraries(tf2scanner PUBLIC glt)

include(FetchContent)

FetchContent_Declare(
        glaze
        GIT_REPOSITORY https://github.com/stephenberry/glaze.git
        GIT_TAG main
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glaze)
target_link_libraries(tf2scanner PUBLIC glaze::glaze)

