cmake_minimum_required(VERSION 3.10.2)

project(TestSocket)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS SerialPort REQUIRED)


find_package( OpenCV REQUIRED )
find_package(jetson-utils)
find_package(jetson-inference)
find_package(CUDA)
#find_library(Gpio /usr/local/lib/libJetsonGPIO.a)
link_directories(/usr/lib/aarch64-linux-gnu/tegra)
include_directories( ${OpenCV_INCLUDE_DIRS} )

cuda_add_executable(${PROJECT_NAME} "main.cpp" "sockettest.cpp" "sockettest.h")

target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::SerialPort Qt${QT_VERSION_MAJOR}::Core ${OpenCV_LIBS})
