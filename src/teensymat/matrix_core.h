#ifndef TEENSYOPT_TEENSYMAT_MATRIX_CORE
#define TEENSYOPT_TEENSYMAT_MATRIX_CORE
// std includes
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
  /*! Construct a matrix will all entried set to elem.
   *
   * @param nrows Number of rows the new Matrix will have
   * @param ncols Number of columns the new Matrix will have
   * @param elem The initial value each entry of the matrix will have
   * */
  Matrix(size_t nrows, size_t ncols, Scalar elem)
      : nrows(nrows), ncols(ncols), matrix_size(nrows * ncols),
        row_stride(ncols), col_stride(1), data(nrows * ncols, elem) {}
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
}; // namespace template<typenameScalar>class TeensyMatrix
} // namespace teensymat
#endif // TEENSYOPT_TEENSYMAT_MATRIX_CORE
