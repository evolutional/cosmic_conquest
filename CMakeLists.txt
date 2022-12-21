cmake_minimum_required(VERSION 3.0.0)
project(cosmic_conquest VERSION 0.1.0)

# include(CTest)
# enable_testing()
include_directories("external/dos-like/source")
include_directories("external/ZX Eurostile")
add_executable(cosmic_conquest
    source/platform_doslike.c
    source/main.c
    source/alien.c
    source/berserker.c
    source/conquest.c
    source/human.c
    source/klingon.c
    source/romulan.c
)

set_property(TARGET cosmic_conquest PROPERTY COMPILE_WARNING_AS_ERROR ON)

if(MSVC)
# target_compile_options(cosmic_conquest PRIVATE /W3 /WX)

# target_compile_definitions(cosmic_conquest PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    # target_compile_options(cosmic_conquest PRIVATE -Wall -Wextra -Wpedantic -Werror)
    find_package(SDL2 REQUIRED)
    find_package(OpenGL REQUIRED)
    target_include_directories(cosmic_conquest PUBLIC ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
    target_link_libraries(cosmic_conquest ${SDL2_LIBRARY} SDL2 GLEW GL m pthread)
endif()

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
