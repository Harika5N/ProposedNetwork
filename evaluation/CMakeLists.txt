cmake_minimum_required(VERSION 2.8.3)

#set(TARGET_BINARY_NAME ${CURRENT_TARGET})

project(pspnet_test)
#message(STATUS ${CUDA_NVCC_FLAGS})
#nvcc flags
#set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-gencode arch=compute_61,code=sm_61)


#file(GLOB SOURCES "sources/*.cpp")

find_package(Caffe REQUIRED)
find_package(CUDA REQUIRED)
#find_package(PCL REQUIRED)

include_directories(
  ${Caffe_INCLUDE_DIRS}
  ${CUDA_INCLUDE_DIRS}
  )
#message("hi")
#message("hi")
#message("hi")
#message("hi")
#message(STATUS ${CUDA_LIBRARIES})
#message(STATUS ${CUDA_INCLUDE_DIRS})
#message(STATUS ${CUDA_NVCC_FLAGS})
#message("hi")
#message("hi")
#message("hi")



find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Boost REQUIRED filesystem system)
include_directories(${Boost_INCLUDE_DIRS} )

add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
add_definitions(${Caffe_DEFINITIONS})
add_definitions(${Caffe_DEFINITIONS})
add_definitions(${CUDA_DEFINITIONS})

#add_executable(test sources/test.cpp)
add_executable(test2 sources/test2.cpp)
#target_link_libraries(test ${Caffe_LIBRARIES} ${OpenCV_LIBRARIES})
target_link_libraries(test2 ${Caffe_LIBRARIES} ${OpenCV_LIBRARIES})
