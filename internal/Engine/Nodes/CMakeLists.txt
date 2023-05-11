set(proj nodes)
set(includeDir ${proj}IncludeDirs)

add_library(${proj} src/camera.cpp src/transform.cpp src/node.cpp src/scene.cpp)
target_compile_features(${proj} PRIVATE cxx_std_20)

target_include_directories(${proj} PUBLIC src/headers)
#target_include_directories(${proj} PUBLIC ../Engine/src/headers)

# add include directories
target_include_directories(${proj}
    PRIVATE ${PROJECT_SOURCE_DIR}/external/glew/include
    PRIVATE ${PROJECT_SOURCE_DIR}/external/glfw/include
    PRIVATE ${PROJECT_SOURCE_DIR}/external/glm
    PUBLIC ${PROJECT_SOURCE_DIR}/external/headers
    

    # Add subdirectories
    PUBLIC ${PROJECT_SOURCE_DIR}/internal/Engine/Components/src/headers
    PUBLIC ${PROJECT_SOURCE_DIR}/internal/Engine/src/headers
    PUBLIC ${PROJECT_SOURCE_DIR}/internal/Essentials/src/headers
)

# add dependencies to this subproject
target_link_libraries(${proj} glfw libglew_static)

# create variable that holds reference to location of all source files ( includes )
set (${includeDir} ${CMAKE_CURRENT_SOURCE_DIR}/src/ PARENT_SCOPE) 