cmake_minimum_required(VERSION 3.11.3)

# set(CMAKE_CXX_STANDARD 17)
project(SAM_1.3_Actuarial_Valuation_Engine)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

find_package(Boost 1.7 COMPONENTS date_time REQUIRED)


include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
add_definitions(${Boost_DEFINITIONS})

# Find all executables
include_directories(include)
file(GLOB project_SRCS src/*.cpp)

# Add project executable
add_executable(goSAM ${project_SRCS})
#target_link_libraries(goSAM ${Boost_LIBRARIES})
target_link_libraries(goSAM Boost::date_time)
