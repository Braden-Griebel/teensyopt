// External Includes
#include "catch2/catch_test_macros.hpp"

// Local Includes
#include "matrix_core.hpp"

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
  SECTION("Creating a matrix with initializer list") {
    // Create a 2x3 matrix with elements 1 to 6
    auto test_matrix = teensymat::Matrix<int>{2, 3, {1, 2, 3, 4, 5, 6}};
    // Check that the elements were set correctly
    int row;
    int col;
    for (int i = 0; i < 6; i++) {
      row = i / 3;
      col = i % 3;
      REQUIRE(*test_matrix(row, col) == i + 1);
    }
  }
}

TEST_CASE("Basic Matrix Operations", "[operations]") {
  SECTION("Swap Rows") {
    auto test_matrix = teensymat::Matrix<int>{3, 2, {1, 2, 3, 4, 5, 6}};
    // Swap the first and second row
    test_matrix.swap_row(0, 1);
    // Check that the rows are indeed swapped
    REQUIRE(*test_matrix(0, 0) == 3);
    REQUIRE(*test_matrix(0, 1) == 4);
    REQUIRE(*test_matrix(1, 0) == 1);
    REQUIRE(*test_matrix(1, 1) == 2);
  }
}
