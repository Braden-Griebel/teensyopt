add_library(teensy_matrix
  matrix_core.h 
)

target_include_directories(matrix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
