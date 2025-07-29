// External Includes
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

// Local Includes
#include "TeensyOpt/TeensyMat/matrix_core.hpp"

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
  SECTION("Swap Columns") {
    auto test_matrix = teensymat::Matrix<int>{3, 2, {1, 2, 3, 4, 5, 6}};
    // Swap the first and second row
    test_matrix.swap_col(0, 1);
    // Check that the columns have been swapped
    REQUIRE(*test_matrix(0, 0) == 2);
    REQUIRE(*test_matrix(0, 1) == 1);
    REQUIRE(*test_matrix(1, 0) == 4);
    REQUIRE(*test_matrix(1, 1) == 3);
    REQUIRE(*test_matrix(2, 0) == 6);
    REQUIRE(*test_matrix(2, 1) == 5);
  }
  SECTION("Multiply row by scalar") {
    auto test_matrix = teensymat::Matrix<int>{2, 3, {1, 2, 3, 4, 5, 6}};
    test_matrix.mult_row_scalar(0, 2);
    std::vector<int> expected_row{2, 4, 6};
    for (int i = 0; i < 3; i++) {
      REQUIRE(*test_matrix(0, i) == expected_row.at(i));
    }
  }
  SECTION("Divide row by scalar") {
    auto test_matrix = teensymat::Matrix<float>{2, 3, {1, 2, 3, 4, 5, 6}};
    test_matrix.div_row_scalar(0, 2);
    std::vector<float> expected_row{0.5, 1.0, 1.5};
    for (int i = 0; i < 3; i++) {
      REQUIRE_THAT(*test_matrix(0, i),
                   Catch::Matchers::WithinAbs(expected_row.at(i), 1e-12));
    }
  }
  SECTION("Add scalar to row") {
    auto test_matrix = teensymat::Matrix<int>{2, 3, {1, 2, 3, 4, 5, 6}};
    test_matrix.add_row_scalar(0, 2);
    std::vector<int> expected_row{3, 4, 5};
    for (int i = 0; i < 3; i++) {
      REQUIRE(*test_matrix(0, i) == expected_row.at(i));
    }
  }
  SECTION("Subtract scalar from row") {
    auto test_matrix = teensymat::Matrix<int>{2, 3, {1, 2, 3, 4, 5, 6}};
    test_matrix.sub_row_scalar(0, 1);
    std::vector<int> expected_row{0, 1, 2};
    for (int i = 0; i < 3; i++) {
      REQUIRE(*test_matrix(0, i) == expected_row.at(i));
    }
  }
  SECTION("Multiply column by scalar") {
    auto test_matrix = teensymat::Matrix<int>{3, 2, {1, 4, 2, 5, 3, 6}};
    test_matrix.mult_col_scalar(0, 2);
    std::vector<int> expected_col{2, 4, 6};
    for (int i = 0; i < 3; i++) {
      REQUIRE(*test_matrix(i, 0) == expected_col.at(i));
    }
  }
  SECTION("Divide column by scalar") {
    auto test_matrix = teensymat::Matrix<float>{3, 2, {1, 4, 2, 5, 3, 6}};
    test_matrix.div_col_scalar(0, 2);
    std::vector<float> expected_col{0.5, 1.0, 1.5};
    for (int i = 0; i < 3; i++) {
      REQUIRE_THAT(*test_matrix(i, 0),
                   Catch::Matchers::WithinAbs(expected_col.at(i), 1e-12));
    }
  }
  SECTION("Add scalar to column") {
    auto test_matrix = teensymat::Matrix<int>{3, 2, {1, 4, 2, 5, 3, 6}};
    test_matrix.add_col_scalar(0, 2);
    std::vector<int> expected_col{3, 4, 5};
    for (int i = 0; i < 3; i++) {
      REQUIRE(*test_matrix(i, 0) == expected_col.at(i));
    }
  }
  SECTION("Subtract scalar from column") {
    auto test_matrix = teensymat::Matrix<int>{3, 2, {1, 4, 2, 5, 3, 6}};
    test_matrix.sub_col_scalar(0, 1);
    std::vector<int> expected_col{0, 1, 2};
    for (int i = 0; i < 3; i++) {
      REQUIRE(*test_matrix(i, 0) == expected_col.at(i));
    }
  }
  // SECTION("FAIL TEST") { REQUIRE(false); }
}
