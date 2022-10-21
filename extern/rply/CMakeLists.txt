project(rply)

add_library(rply src/rply.c)
target_include_directories(rply PUBLIC include)