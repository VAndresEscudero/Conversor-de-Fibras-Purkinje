cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)

PROJECT(main)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
#SET ( CMAKE_CXX_FLAGS "-D_GLIBCXX_USE_CXX11_ABI=0" )

#add_executable(main MACOSX_BUNDLE main.cpp Datasets/Dataset.cpp Datasets/DatasetDouble.cpp VtkParser.cpp)
add_executable(HeartConverter MACOSX_BUNDLE main.cpp Datasets/DatasetAbstract.cpp Datasets/Dataset.h VtkParser.cpp Outputs/AbstractFile.h Outputs/CarpPoints.cpp Outputs/CarpPurkinje.cpp Outputs/CarpElements.cpp)

if(VTK_LIBRARIES)
    target_link_libraries(HeartConverter ${VTK_LIBRARIES})
else()
    target_link_libraries(HeartConverter vtkHybrid vtkWidgets)
endif()

#target_link_libraries(main ${VTK_LIBRARIES})