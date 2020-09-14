file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers}  
"src/frontend/CodeExtractor.cpp" "src/frontend/CodeExtractor.h"
"src/CustomTypes.h"
"src/DesignExtractor.cpp"
"src/frontend/ExprEvaluator.cpp" "src/frontend/ExprEvaluator.h"
"src/ListOfTNodePointers.h"
"src/frontend/Parser.cpp" "src/frontend/Parser.h"
"src/PKB.cpp" "src/PKB.h"
"src/frontend/Statement.cpp" "src/frontend/Statement.h"
"src/StmtTable.cpp" "src/StmtTable.h"
"src/pkb/TNode.cpp" "src/pkb/TNode.h"
"src/frontend/Token.cpp" "src/frontend/Token.h"
"src/frontend/Tokenizer.h" "src/frontend/Tokenizer.cpp" 
"src/frontend/TokenType.cpp" "src/frontend/TokenType.h"
"src/VarTable.cpp" "src/Vartable.h"
)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)