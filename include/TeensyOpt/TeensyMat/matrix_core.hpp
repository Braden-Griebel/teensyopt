#pragma once
// std includes
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>

namespace teensymat {
/*! A class representing a two dimensional array*/
template <typename Scalar> class Matrix {
private:
  /*! The data associated with the Matrix (linear array) */
  std::vector<Scalar> data;
  /*! The increase in the data index that will increment by one row */
  size_t row_stride;
  /*! The increase in the data index that will increment by one column*/
  size_t col_stride;
  /*! The number of rows of the Matrix*/
  size_t nrows;
  /*! The number of columns of the Matrix*/
  size_t ncols;
  /*! The size of the matrix (the length of the associated linear memory)*/
  size_t matrix_size;

public:
  /*! Construct an empty Matrix with no rows or columns
   * */
  Matrix()
      : row_stride(0), col_stride(0), nrows(0), ncols(0), matrix_size(0),
        data(0) {};
  /*! Construct a matrix will all 0 entries with nrows rows and ncols columns
   *
   * @param nrows Number of rows the new Matrix will have
   * @param ncols Number of columns the new Matrix will have
   * */
  Matrix(size_t nrows, size_t ncols) : Matrix(nrows, ncols, (Scalar)0) {};
  /*! Construct a matrix will all entried set to element.
   *
   * @param nrows Number of rows the new Matrix will have
   * @param ncols Number of columns the new Matrix will have
   * @param element The initial value each entry of the matrix will have
   * */
  Matrix(size_t nrows, size_t ncols, Scalar element)
      : nrows(nrows), ncols(ncols), matrix_size(nrows * ncols),
        row_stride(ncols), col_stride(1), data(nrows * ncols, element) {}
  /*! Construct a Matrix with values from an initializer list
   *
   * @param nrows Number of rows the new Matrix will have
   * @param ncols Number of columns the new Matrix will have
   * @param elements Values of the elements of the new Matrix, will be set in
   * row major order
   * */
  Matrix(size_t nrows, size_t ncols, std::initializer_list<Scalar> elements)
      : nrows(nrows), ncols(ncols), matrix_size(nrows * ncols),
        row_stride(ncols), col_stride(1), data(nrows * ncols, (Scalar)0) {
    size_t data_index = 0;
    for (Scalar elem : elements) {
      if (data_index >= this->data.size()) {
        throw std::range_error(
            "Received too many elements to initialize Matrix");
      }
      this->data[data_index] = elem;
      data_index++;
    }
    if (data_index != this->data.size()) {
      throw std::range_error("Received too few elements to initialize Matrix");
    }
  }
  /*! Access an element of the matrix by position.
   *
   * @param row Row of the element to be accessed
   * @param col Column of the element to be accessed
   * @return Pointer to element of the matrix at position (row,col)
   * */
  Scalar *operator()(size_t row, size_t col) {
    if (row >= this->nrows || col >= this->ncols) {
      throw std::range_error("Invalid index");
    }
    size_t data_position = row * this->row_stride + col * this->col_stride;
    if (data_position >= this->matrix_size ||
        data_position >= this->data.size()) {
      throw std::range_error("Tried accessing element beyond Matrix data");
    }
    return &(this->data[data_position]);
  }
  /*! Swap the values held in two rows of the Matrix.
   *
   * @param row1 First row to swap
   * @param row2 Second row to swap
   * */
  void swap_row(size_t row1, size_t row2) {
    for (size_t col = 0; col < this->ncols; col++) {
      std::swap(*(*this)(row1, col), *(*this)(row2, col));
    }
  }
  /*! Swap the values help in two columns of the Matrix.
   *
   * @param col1 First column to swap
   * @param col2 Second column to swap
   * */
  void swap_col(size_t col1, size_t col2) {
    for (size_t row = 0; row < this->nrows; row++) {
      std::swap(*(*this)(row, col1), *(*this)(row, col2));
    }
  }
  /*! Multiply a row by a given value.
   *
   * @param row Which row to multiply
   * @param by What to multiple the row by
   * */
  void mult_row(size_t row, Scalar by) {
    for (size_t col = 0; col < this->ncols; col++) {
      *(*this)(row, col) *= by;
    }
  }
  /*! Multiply a column by a given value.
   *
   * @param column Which col to multiply
   * @param by What to multiple the column by
   * */
  void mult_col(size_t column, Scalar by) {
    for (size_t row = 0; row < this->nrows; row++) {
      *(*this)(row, column) *= by;
    }
  }
  /*! Divide a row by a given value.
   *
   * @param row Which row to divide
   * @param by What to divide the row by
   * */
  void div_row(size_t row, Scalar by) { this->mult_row(row, ((Scalar)1) / by); }
  /*! Divide a column by a given value.
   *
   * @param column Which column to divide
   * @param by What to divide the column by
   * */
  void div_col(size_t column, Scalar by) {
    this->mult_row(column, ((Scalar)1) / by);
  }
  /*! Add a value to each element in a row.
   * @param row Which row to add the value to
   * @param what What to add to the elements of the row
   * */
  void add_row(size_t row, Scalar what) {
    for (size_t col = 0; col < this->ncols; col++) {
      *(*this)(row, col) += what;
    }
  }
  /*! Add a value to each element in a column.
   * @param column Which column to add the value to
   * @param what What to add to the elements of the column
   * */
  void add_col(size_t column, Scalar what) {
    for (size_t row = 0; row < this->nrows; row++) {
      *(*this)(row, column) += what;
    }
  }
  /*! Subtract a value from each element in a row.
   *
   * @param row Which row to subtract the value from
   * @param what What to subtract from the elements of the row
   * */
  void sub_row(size_t row, Scalar what) { this->sub_row(row, -what); }
  /*! Subtract a value from each element in a column.
   *
   * @param column Which column to subtract the value from
   * @param what What to subtract from the elements of the column
   * */
  void sub_col(size_t column, Scalar what) { this->sub_row(column, -what); }
}; // namespace template<typenameScalar>class TeensyMatrix
} // namespace teensymat
