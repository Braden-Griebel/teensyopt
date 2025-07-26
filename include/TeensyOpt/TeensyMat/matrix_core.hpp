#pragma once
// std includes
#include <initializer_list>
#include <stdexcept>
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
  /*! Swap the values held in two rows of the Matrix
   *
   * @param row1 First row to swap
   * @param row2 Second row to swap
   * */
  void swap_row(size_t row1, size_t row2) {
    Scalar swap_tmp;
    for (int col = 0; col < this->ncols; col++) {
      swap_tmp = *(*this)(row1, col);
      *(*this)(row1, col) = *(*this)(row2, col);
      *(*this)(row2, col) = swap_tmp;
    }
  }
}; // namespace template<typenameScalar>class TeensyMatrix
} // namespace teensymat
