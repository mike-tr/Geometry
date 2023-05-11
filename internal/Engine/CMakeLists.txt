set(proj engine)
set(includeDir ${proj}IncludeDirs)

add_library(${proj} src/engine.cpp src/simpleCamera.cpp src/inputHandler.cpp)
target_compile_features(${proj} PRIVATE cxx_std_20)

target_include_directories(${proj} PUBLIC Components/src/headers)
target_include_directories(${proj} PUBLIC src/headers)
#target_include_directories(${proj} PUBLIC ../Engine/src/headers)

# add include directories
target_include_directories(${proj}
    PRIVATE ${PROJECT_SOURCE_DIR}/external/glew/include
    PRIVATE ${PROJECT_SOURCE_DIR}/external/glfw/include
    PRIVATE ${PROJECT_SOURCE_DIR}/external/glm
    PUBLIC ${PROJECT_SOURCE_DIR}/external/headers

    # Add subdirectories
    PUBLIC ${PROJECT_SOURCE_DIR}/internal/Essentials/src/headers
)

# add_subdirectory(${PROJECT_SOURCE_DIR}/external/glfw)
# add_subdirectory(${PROJECT_SOURCE_DIR}/external/glew)
# add_subdirectory(${PROJECT_SOURCE_DIR}/external/glm)

add_subdirectory(${PROJECT_SOURCE_DIR}/internal/Engine/Nodes)
add_subdirectory(${PROJECT_SOURCE_DIR}/internal/Engine/Components)

# add dependencies to this subproject
target_link_libraries(${proj} glfw libglew_static components nodes essentials)

# create variable that holds reference to location of all source files ( includes )
set (${includeDir} ${CMAKE_CURRENT_SOURCE_DIR}/src/ PARENT_SCOPE) 