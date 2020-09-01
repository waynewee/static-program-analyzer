file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers} src/STMT_TABLE.h src/VAR_TABLE.h src/VAR_TABLE.cpp src/VAR_NAME.h src/STMT_IDX.h src/STMT_TABLE.cpp src/LIST_OF_STMT_IDX.h src/LIST_OF_TNODE_PTR.h src/LIST_OF_VAR_NAME.h)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


