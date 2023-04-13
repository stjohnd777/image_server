cmake_minimum_required(VERSION 3.22)
project(image_server)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)
if(Boost_FOUND)
    message(STATUS "Found Boost ${Boost_LIBRARIES}")
endif()

find_package(OpenCV  REQUIRED)
if(OpenCV_FOUND)
    message(STATUS "Found OpenCV ${OpenCV_LIBS}")
endif()


add_executable(imgclient ImgClient.cpp)
target_link_libraries(
        imgclient
        ${OpenCV_LIBS}
        ${Boost_LIBRARIES}
)

add_executable(imgserver ImgServer.cpp)
target_link_libraries(
        imgserver
        ${OpenCV_LIBS}
        ${Boost_LIBRARIES}
)

