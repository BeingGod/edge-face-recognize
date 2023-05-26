cmake_minimum_required(VERSION 3.20)

project(edge_face_recognize VERSION 0.0.0 LANGUAGES CXX)

cmake_policy(SET CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# 依赖库路径 
set(QT_PREFIX_PATH "${CMAKE_SOURCE_DIR}/third_party/qtbase-release")
set(OpenCV_PREFIX_PATH "${CMAKE_SOURCE_DIR}/third_party/opencv-release")

set(CMAKE_PREFIX_PATH
    ${CMAKE_PREFIX_PATH}
    ${QT_PREFIX_PATH}
    ${OpenCV_PREFIX_PATH})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THIRD_PARTY_DEFINE "")
set(THIRD_PARTY_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/third_party/include
    )

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core)
find_package(OpenCV REQUIRED)

find_package(OpenMP)

if(OPENMP_FOUND)
    message("openmp found!")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
else()
    message(WARNING "openmp not found!")
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/source
    ${THIRD_PARTY_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${QT_INCLUDE_DIRS}
    ${OpenMP_INCLUDE_DIRS}
)

add_definitions(${THIRD_PARTY_DEFINE})

add_subdirectory(third_party)
add_subdirectory(source)

install(TARGETS edge_face_recognize DESTINATION bin COMPONENT binaries)