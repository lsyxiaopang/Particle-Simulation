

cmake_minimum_required (VERSION 3.8)

project ("ParticleSimulation")

find_package(HDF5 REQUIRED COMPONENTS C CXX)

set(CMAKE_BUILD_TYPE "Release")

include_directories(${HDF5_INCLUDE_DIR})

add_executable( ParticleSimulation "ParticleSimulation.cpp" "IOHelper.cpp" "particle.cpp" "ParticleSystem.cpp" "vec.cpp" "wall.cpp")

target_link_libraries( ParticleSimulation ${HDF5_CXX_LIBRARIES} ${HDF5_LIBRARIES})

install(TARGETS ParticleSimulation DESTINATION bin) 
# 将源代码添加到此项目的可执行文件。

# TODO: 如有需要，请添加测试并安装目标。
