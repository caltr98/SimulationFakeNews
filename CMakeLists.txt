cmake_minimum_required(VERSION 3.28)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

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
        main.cpp
        DatasetModelFiles/datasetstats.h)

# Link against the Ws2_32 library
target_link_libraries(untitled PRIVATE Ws2_32)
