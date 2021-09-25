add_llvm_library(LLVMProspector
    FunctionAnalyzer.cpp
    LoopAnalyzer.cpp
    statistics.cpp
    utility.cpp)

target_link_libraries (LLVMProspector)

