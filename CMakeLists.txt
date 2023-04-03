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


add_executable(image_server Common.h SyncTcpServer.h TcpImageServer.cpp)
target_link_libraries(
        image_server
        ${OpenCV_LIBS}
        ${Boost_LIBRARIES}
)


add_executable(image_client Common.h SyncTcpClient.h TcpImageClient.cpp)
target_link_libraries(
        image_client
        ${OpenCV_LIBS}
        ${Boost_LIBRARIES}
)

