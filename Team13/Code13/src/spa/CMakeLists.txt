file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers}  
"src/CodeExtractor.cpp" "src/CodeExtractor.h"
"src/CustomTypes.h"
"src/DesignExtractor.cpp"
"src/ExprEvaluator.cpp" "src/ExprEvaluator.h"
"src/ListOfTNodePointers.h"
"src/Parser.cpp" "src/Parser.h"
"src/PKB.cpp" "src/PKB.h"
"src/Statement.cpp" "src/Statement.h"
"src/StmtTable.cpp" "src/StmtTable.h"
"src/pkb/TNode.cpp" "src/pkb/TNode.h"
"src/Token.cpp" "src/Token.h"
"src/Tokenizer.h" "src/Tokenizer.cpp" 
"src/TokenType.cpp" "src/TokenType.h"
"src/VarTable.cpp" "src/Vartable.h"
)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)