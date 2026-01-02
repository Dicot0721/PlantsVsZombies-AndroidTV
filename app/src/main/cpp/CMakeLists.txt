cmake_minimum_required(VERSION 3.20) # 3.20 is for C++23
project(PvZTV)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

string(APPEND CMAKE_CXX_FLAGS
        " -Wall" # 开启绝大部分警告
        " -Werror=return-type"
        " -Werror=unused-result"
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_VISIBILITY_PRESET hidden)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden) # 隐藏非 inline 符号
    set(CMAKE_VISIBILITY_INLINES_HIDDEN ON) # 隐藏 inline 符号
endif ()

add_subdirectory(ThirdParty)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
        "Homura/src/*.cpp"
        "PvZ/src/*.cpp"
)
add_library(Homura SHARED Main.cpp ${SRC_FILES})

target_include_directories(Homura PRIVATE Homura/include PvZ/include)

target_compile_definitions(Homura PRIVATE PVZ_VERSION=${PVZ_VERSION})
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Homura PRIVATE PVZ_DEBUG=1)
endif ()

find_library(android-lib android)
find_library(EGL-lib EGL)
find_library(GLESv2-lib GLESv2)
find_library(log-lib log)
find_library(OpenSLES-lib OpenSLES)
target_link_libraries(Homura PRIVATE
        Substrate
        ${android-lib}
        ${EGL-lib}
        ${GLESv2-lib}
        ${log-lib}
        ${OpenSLES-lib}
)
