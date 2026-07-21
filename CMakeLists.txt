# CMakeList.txt : CMake project for TireCentral
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("TireCentral")

# Define ThirdParty Directories
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui")
set(IMPLOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/implot")
set(GLFW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/glfw")

set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build GLFW library examples")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build GLFW library tests")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build GLFW library documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate GLFW install targets")

# Include GLFW CMake project
add_subdirectory(${GLFW_DIR})

# Source files definitions
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
)

set(IMPLOT_SOURCES
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_demo.cpp
    ${IMPLOT_DIR}/implot_items.cpp
)

set(BACKEND_SOURCES
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Add source to this project's executable.
add_executable (${PROJECT_NAME} 
    "TireCentral.cpp" 
    "TireCentral.h" 
    "tinyfiledialogs.c"
    ${IMGUI_SOURCES} 
    ${IMPLOT_SOURCES} 
    ${BACKEND_SOURCES}
)

# Include Directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${IMPLOT_DIR}
    ${GLFW_DIR}/include
)

# Link Libraries
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()
