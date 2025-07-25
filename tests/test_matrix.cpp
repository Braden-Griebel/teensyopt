// External Includes
#include "catch2/catch_test_macros.hpp"

// Local Includes
#include "matrix_core.h"

TEST_CASE("Matrix Construction", "[matrix_core]") {
  SECTION("Creating an empty matrix") {
    auto test_matrix = teensymat::Matrix<double>{};
  }
  SECTION("Creating a matrix of zeros") {
    // Create a zero matrix with 2 rows and 3 columns
    auto test_matrix = teensymat::Matrix<double>{2, 3};
    // Every element of the matrix should be 0
    for (size_t row = 0; row < 2; row++) {
      for (size_t col = 0; col < 3; col++) {
        REQUIRE(*test_matrix(row, col) == 0.0);
      }
    }
  }
}
