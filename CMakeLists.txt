cmake_minimum_required(VERSION 3.28)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# Add your source files
add_executable(untitled main.cpp
        Post.cpp
        User.cpp
        Checker.cpp
        Checker.h
        Model.cpp
        Model.h
        DatasetModelFiles/DatasetPost.cpp
        DatasetModelFiles/DatasetUser.cpp
        DatasetModelFiles/HTTPRequest.hpp
        DatasetModelFiles/ModelDataset.cpp
        DatasetModelFiles/ModelDataset.h
        DatasetModelFiles/json.hpp
        DatasetModelFiles/DataSetChecker.h
        DatasetModelFiles/DataSetChecker.cpp
        DatasetModelFiles/HTTPRequester.cpp
        DatasetModelFiles/datasetstats.h)

# Link against the Ws2_32 library
target_link_libraries(untitled PRIVATE Ws2_32)
