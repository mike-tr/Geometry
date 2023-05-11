cmake_minimum_required(VERSION 3.21.1)
project(openGLLearning VERSION 1.0)

# Generate a config.h file with some settings like DEBUG etc...
configure_file(config.h.in config.h)
add_executable(${PROJECT_NAME} src/main.cpp src/propertyTest.cpp)
# add_library(${PROJECT_NAME} src/functions.cpp)

# file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/assets/shaders)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC src/)
target_include_directories(${PROJECT_NAME} PUBLIC src/headers)
target_include_directories(${PROJECT_NAME} PUBLIC external/headers)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Download all git submodules
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# update submodules
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT
        )
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT} please checkout submodules")
        endif()
    endif()
endif()

# add the glfw and glm to the project
add_subdirectory(external/glfw)
add_subdirectory(external/glew)
add_subdirectory(external/glm)

# find_package(Vulkan REQUIRED)

target_include_directories(${PROJECT_NAME}
    # PRIVATE ${Vulkan_INCLUDE_DIRS}
    PRIVATE ${PROJECT_SOURCE_DIR}/external/glew/include
    PRIVATE ${PROJECT_SOURCE_DIR}/external/glfw/include
    PRIVATE ${PROJECT_SOURCE_DIR}/external/glm
)

add_subdirectory(internal/Engine)
add_subdirectory(internal/Essentials)
# add_subdirectory(internal/vkEngine)
target_include_directories(${PROJECT_NAME} PUBLIC ${shadersIncludeDirs})
target_include_directories(${PROJECT_NAME} PUBLIC ${engineIncludeDirs})
# target_include_directories(${PROJECT_NAME} PUBLIC ${vkEngineIncludeDirs})

target_link_libraries(${PROJECT_NAME} glfw libglew_static engine components nodes essentials)

message(STATUS "test")

# uncomment to have install
# install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
# install(FILES "${PROJECT_BINARY_DIR}/Progconfig.hpp" DESTINATION include)