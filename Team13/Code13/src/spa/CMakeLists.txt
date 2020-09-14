file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.hpp")
add_library(spa ${srcs} ${headers} src/pkb/RelationManager.cpp src/pkb/RelationManager.h src/pkb/DataManager.cpp src/pkb/DataManager.h src/pkb/DataTables.cpp src/pkb/DataTables.h src/pkb/BTNode.tpp src/pkb/BTNode.h)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/pkb")
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb
        ${CMAKE_CURRENT_SOURCE_DIR}/src)

