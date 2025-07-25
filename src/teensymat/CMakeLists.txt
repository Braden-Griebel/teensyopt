add_library(teensymat
  matrix_core.h
)

set_target_properties(teensymat PROPERTIES LINKER_LANGUAGE CXX)


target_include_directories(teensymat PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
