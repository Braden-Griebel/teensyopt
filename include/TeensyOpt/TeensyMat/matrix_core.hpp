#pragma once
// std includes
#include <functional>
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
  // SECTION: Constructors
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
  /*! Construct a Matrix with values from a vector.
   *
   * @param nrows Number of rows the new Matrix will have
   * @param ncols Number of columns the new Matrix will have
   * @param elements Values of the elements of the new Matrix, will be set in
   * row major order. Note, the vector will NOT be copied, so
   * changing its values will alter elements of the Matrix
   * */
  Matrix(size_t nrows, size_t ncols, std::vector<Scalar> elements)
      : data(elements), row_stride(ncols), col_stride(1), nrows(nrows),
        ncols(ncols), matrix_size(nrows * ncols) {}
  /*! Construct a Matrix with values from a vector.
   *
   * @param nrows Number of rows the new Matrix will have
   * @param ncols Number of columns the new Matrix will have
   * @param row_stride The stride for rows in the Matrix (length of step in the
   * underlying data vector which will increase the row index by 1)
   * @param col_stride THe stride for columns in the Matrix (length of step
   * in the underlying data vector which will increase the column index by 1)
   * @param elements Values of the elements of the new Matrix, will be set in
   * row major order. Note, the vector will NOT be copied, so
   * changing its values will alter elements of the Matrix
   * */
  Matrix(size_t nrows, size_t ncols, size_t row_stride, size_t col_stride,
         std::vector<Scalar> elements)
      : data(elements), row_stride(row_stride), col_stride(col_stride),
        nrows(nrows), ncols(ncols), matrix_size(nrows * ncols) {}

  // SECTION: Getters
  /*! Get the number of rows in the Matrix.*/
  size_t get_nrows() { return this->nrows; }
  /*! Get the number of columns in the Matrix.*/
  size_t get_ncols() { return this->ncols; }
  /*! Get the stride for the rows of the matrix.*/
  size_t get_row_stride() { return this->row_stride; }
  /*! Get the stride for the columns of the matrix. */
  size_t get_col_stride() { return this->col_stride; }
  /*! Get the underlying data vector*/
  std::vector<Scalar> *get_data() { return &(this->data); }
  /*! Get the size of the matrix (the total number of elements).*/
  size_t get_size() { return this->matrix_size; }
  /*! Get the shape of the matrix (nrows, ncols). */
  std::pair<size_t, size_t> get_shape() {
    return std::pair<size_t, size_t>{this->nrows, this->ncols};
  }
  // SECTION: Elementary operations
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
  void mult_row_scalar(size_t row, Scalar by) {
    for (size_t col = 0; col < this->ncols; col++) {
      *(*this)(row, col) *= by;
    }
  }
  /*! Multiply a column by a given value.
   *
   * @param column Which col to multiply
   * @param by What to multiple the column by
   * */
  void mult_col_scalar(size_t column, Scalar by) {
    for (size_t row = 0; row < this->nrows; row++) {
      *(*this)(row, column) *= by;
    }
  }
  /*! Divide a row by a given value.
   *
   * @param row Which row to divide
   * @param by What to divide the row by
   * */
  void div_row_scalar(size_t row, Scalar by) {
    this->mult_row_scalar(row, ((Scalar)1) / by);
  }
  /*! Divide a column by a given value.
   *
   * @param column Which column to divide
   * @param by What to divide the column by
   * */
  void div_col_scalar(size_t column, Scalar by) {
    this->mult_col_scalar(column, ((Scalar)1) / by);
  }
  /*! Add a value to each element in a row.
   * @param row Which row to add the value to
   * @param what What to add to the elements of the row
   * */
  void add_row_scalar(size_t row, Scalar what) {
    for (size_t col = 0; col < this->ncols; col++) {
      *(*this)(row, col) += what;
    }
  }
  /*! Add a value to each element in a column.
   * @param column Which column to add the value to
   * @param what What to add to the elements of the column
   * */
  void add_col_scalar(size_t column, Scalar what) {
    for (size_t row = 0; row < this->nrows; row++) {
      *(*this)(row, column) += what;
    }
  }
  /*! Subtract a value from each element in a row.
   *
   * @param row Which row to subtract the value from
   * @param what What to subtract from the elements of the row
   * */
  void sub_row_scalar(size_t row, Scalar what) {
    this->add_row_scalar(row, -what);
  }
  /*! Subtract a value from each element in a column.
   *
   * @param column Which column to subtract the value from
   * @param what What to subtract from the elements of the column
   * */
  void sub_col_scalar(size_t column, Scalar what) {
    this->add_col_scalar(column, -what);
  }
  /*! Add row2 to row1, storing the result in row1.
   *
   * @param row1 First row to add, also row into which the result is stored
   * @param row2 Second row to add*/
  void add_row_elementwise(size_t row1, size_t row2) {
    for (size_t col = 0; col < this->ncols; col++) {
      *(*this)(row1, col) += *(*this)(row2, col);
    }
  }
  /*! Add col2 to col1, storing the result in col1.
   *
   * @param col1 First column to add, also column into which the result is
   * stored
   * @param col2 Second column to add*/
  void add_col_elementwise(size_t col1, size_t col2) {
    for (size_t row = 0; row < this->nrows; row++) {
      *(*this)(row, col1) += *(*this)(row, col2);
    }
  }
  /*! Subtract row2 from row1, storing the result in row1.
   *
   * @param row1 Left hand side of subtraction, also row into which the result
   * is stored
   * @param row2 Right hand side of subtraction*/
  void sub_row_elementwise(size_t row1, size_t row2) {
    for (size_t col = 0; col < this->ncols; col++) {
      *(*this)(row1, col) -= *(*this)(row2, col);
    }
  }
  /*! Subtract col2 from col1, storing the result in col1.
   *
   * @param col1 Left hand side of subtraction, also column into which the
   * result is stored
   * @param col2 Right hand side of subtraction
   * */
  void sub_col_elementwise(size_t col1, size_t col2) {
    for (size_t row = 0; row < this->nrows; row++) {
      *(*this)(row, col1) -= *(*this)(row, col2);
    }
  }
  /*! Return the transpose of the matrix*/
  Matrix<Scalar> transpose() {
    std::vector<Scalar> transpose_data = this->data;
    return Matrix<Scalar>{this->ncols, this->nrows, this->col_stride,
                          this->row_stride, transpose_data};
  }
  // SECTION: Operator overloads
  /*! Apply a binary operator elementwise between this Matrix and Scalar other
   *
   * @param other The right hand size Scalar of the function being applied
   * @param to_apply Binary function to apply to each element of this, and
   * the Scalar other. Must accept two Scalars and return a single element of
   * specified Result type.
   *
   * @return New Matrix whose elements are the result of the function
   * */
  template <typename Result>
  Matrix<Result>
  scalar_binary_apply(Scalar other,
                      std::function<Result(Scalar, Scalar)> to_apply) {
    Matrix<Result> result_matrix{this->nrows, this->ncols};
    for (size_t row = 0; row < this->nrows; row++) {
      for (size_t col = 0; col < this->ncols; col++) {
        *result_matrix(row, col) = to_apply(*(*this)(row, col), other);
      }
    }
    return result_matrix;
  }
  /*! Apply a binary function in place elementwise between this Matrix and
   * Scalar other
   *
   * @param other The right hand size Scalar of the function being applied
   * @param to_apply Binary function to apply to each element of this, and
   * the Scalar other
   *
   * @return New Matrix whose elements are the result of the function
   * */
  void
  scalar_binary_apply_inplace(Scalar other,
                              std::function<Scalar(Scalar, Scalar)> to_apply) {
    for (size_t row = 0; row < this->nrows; row++) {
      for (size_t col = 0; col < this->ncols; col++) {
        *(*this)(row, col) = to_apply(*(*this)(row, col), other);
      }
    }
  }
  /*! Apply a binary function elementwise to two Matrices.
   *
   * @param other The right hand side Matrix of the function being applied
   * @param to_apply Binary function to apply elementwise to this and other,
   * which accepts two Scalar values and returns a Scalar value
   *
   * @return New Matrix whose elements are the result of the function
   * */
  template <typename Result>
  Matrix<Result>
  elementwise_binary_apply(Matrix<Scalar> const &other,
                           std::function<Result(Scalar, Scalar)> to_apply) {
    if ((this->nrows != other.nrows) || (this->ncols != other.ncols)) {
      throw std::runtime_error("Tried to add Matrices of different shapes");
    }
    Matrix<Result> result_matrix{this->nrows, this->ncols};
    for (size_t row = 0; row < this->nrows; row++) {
      for (size_t col = 0; col < this->ncols; col++) {
        *result_matrix(row, col) =
            to_apply(*(*this)(row, col), *(*this)(row, col));
      }
    }
    return result_matrix;
  }
  /*! Apply a binary function in place elementwise to two Matrices.
   *
   * @param other The right hand side Matrix of the function being applied
   * @param to_apply Binary function to apply elementwise to this and other,
   * which accepts two Scalar values and returns a Scalar value
   *
   * @return New Matrix whose elements are the result of the function
   * */
  void elementwise_binary_apply_inplace(
      Matrix<Scalar> const &other,
      std::function<Scalar(Scalar, Scalar)> to_apply) {
    if ((this->nrows != other.nrows) || (this->ncols != other.ncols)) {
      throw std::runtime_error("Tried to add Matrices of different shapes");
    }
    for (size_t row = 0; row < this->nrows; row++) {
      for (size_t col = 0; col < this->ncols; col++) {
        *(*this)(row, col) = to_apply(*(*this)(row, col), *(*this)(row, col));
      }
    }
  }
  /*! Elementwise addition of two matrices*/
  Matrix<Scalar> operator+(Matrix<Scalar> const &other) {
    return this->elementwise_binary_apply<Scalar>(
        other, [](Scalar rhs, Scalar lhs) { return rhs + lhs; });
  }
  /*! Elementwise addition of a Matrix and a Scalar element*/
  Matrix<Scalar> operator+(Scalar other) {
    return this->scalar_binary_apply<Scalar>(
        other, [](Scalar rhs, Scalar lhs) { return rhs + lhs; });
  }
  /*! Elementwise inplace addition of two matrices*/
  friend Matrix<Scalar> &operator+=(Matrix<Scalar> &lhs,
                                    Matrix<Scalar> const &rhs) {
    lhs.elementwise_binary_apply_inplace(
        rhs, [](Scalar l, Scalar r) { return l + r; });
    return lhs;
  }
  /*! Elementwise inplace addition of a Matrix and a Scalar */
  friend Matrix<Scalar> &operator+=(Matrix<Scalar> &lhs, Scalar rhs) {
    lhs.scalar_binary_apply_inplace(rhs,
                                    [](Scalar l, Scalar r) { return l + r; });
    return lhs;
  }
  /*! Elementwise subtraction of two matrices*/
  Matrix<Scalar> operator-(Matrix<Scalar> const &other) {
    return this->elementwise_binary_apply<Scalar>(
        other, [](Scalar rhs, Scalar lhs) { return rhs - lhs; });
  }
  /*! Elementwise subtraction of a Matrix and a Scalar element*/
  Matrix<Scalar> operator-(Scalar other) {
    return this->scalar_binary_apply<Scalar>(
        other, [](Scalar rhs, Scalar lhs) { return rhs - lhs; });
  }
  /*! Elementwise inplace subtraction of two matrices*/
  friend Matrix<Scalar> &operator-=(Matrix<Scalar> &lhs,
                                    Matrix<Scalar> const &rhs) {
    lhs.elementwise_binary_apply_inplace(
        rhs, [](Scalar l, Scalar r) { return l - r; });
    return lhs;
  }
  /*! Elementwise inplace subtraction of a Matrix and a Scalar */
  friend Matrix<Scalar> &operator-=(Matrix<Scalar> &lhs, Scalar rhs) {
    lhs.scalar_binary_apply_inplace(rhs,
                                    [](Scalar l, Scalar r) { return l - r; });
    return lhs;
  }
  /*! Elementwise multiplication of two matrices*/
  Matrix<Scalar> operator*(Matrix<Scalar> const &other) {
    return this->elementwise_binary_apply<Scalar>(
        other, [](Scalar rhs, Scalar lhs) { return rhs * lhs; });
  }
  /*! Elementwise multiplication of a Matrix and a Scalar element*/
  Matrix<Scalar> operator*(Scalar other) {
    return this->scalar_binary_apply<Scalar>(
        other, [](Scalar rhs, Scalar lhs) { return rhs * lhs; });
  }
  /*! Elementwise inplace multiplication of two matrices*/
  friend Matrix<Scalar> &operator*=(Matrix<Scalar> &lhs,
                                    Matrix<Scalar> const &rhs) {
    lhs.elementwise_binary_apply_inplace(
        rhs, [](Scalar l, Scalar r) { return l * r; });
    return lhs;
  }
  /*! Elementwise inplace multiplication of a Matrix and a Scalar */
  friend Matrix<Scalar> &operator*=(Matrix<Scalar> &lhs, Scalar rhs) {
    lhs.scalar_binary_apply_inplace(rhs,
                                    [](Scalar l, Scalar r) { return l * r; });
    return lhs;
  }
  /*! Elementwise division of two matrices*/
  Matrix<Scalar> operator/(Matrix<Scalar> const &other) {
    return this->elementwise_binary_apply<Scalar>(
        other, [](Scalar rhs, Scalar lhs) { return rhs / lhs; });
  }
  /*! Elementwise division of a Matrix and a Scalar element*/
  Matrix<Scalar> operator/(Scalar other) {
    return this->scalar_binary_apply<Scalar>(
        other, [](Scalar rhs, Scalar lhs) { return rhs / lhs; });
  }
  /*! Elementwise inplace division of two matrices*/
  friend Matrix<Scalar> &operator/=(Matrix<Scalar> &lhs,
                                    Matrix<Scalar> const &rhs) {
    lhs.elementwise_binary_apply_inplace(
        rhs, [](Scalar l, Scalar r) { return l / r; });
    return lhs;
  }
  /*! Elementwise inplace division of a Matrix and a Scalar */
  friend Matrix<Scalar> &operator/=(Matrix<Scalar> &lhs, Scalar rhs) {
    lhs.scalar_binary_apply_inplace(rhs,
                                    [](Scalar l, Scalar r) { return l / r; });
    return lhs;
  }
  // SECTION: Boolean comparison overloads
  /*! Elementwise equality comparison between this Matrix and other*/
  Matrix<bool> operator==(Matrix<Scalar> const &other) {
    return this->elementwise_binary_apply<bool>(
        other, [](Scalar lhs, Scalar rhs) { return lhs == rhs; });
  }
  /*! Elementwise equality comparison between this Matrix and a Scalar other*/
  Matrix<bool> operator==(Scalar other) {
    return this->elementwise_binary_apply<bool>(
        other, [](Scalar lhs, Scalar rhs) { return lhs == rhs; });
  }
  /*! Elementwise less than comparison between this Matrix and other*/
  Matrix<bool> operator<(Matrix<Scalar> const &other) {
    return this->elementwise_binary_apply<bool>(
        other, [](Scalar lhs, Scalar rhs) { return lhs < rhs; });
  }
  /*! Elementwise less than comparison between this Matrix and a Scalar other*/
  Matrix<bool> operator<(Scalar other) {
    return this->elementwise_binary_apply<bool>(
        other, [](Scalar lhs, Scalar rhs) { return lhs < rhs; });
  }
  /*! Elementwise less equal comparison between this Matrix and other*/
  Matrix<bool> operator<=(Matrix<Scalar> const &other) {
    return this->elementwise_binary_apply<bool>(
        other, [](Scalar lhs, Scalar rhs) { return lhs <= rhs; });
  }
  /*! Elementwise less equal comparison between this Matrix and a Scalar other*/
  Matrix<bool> operator<=(Scalar other) {
    return this->elementwise_binary_apply<bool>(
        other, [](Scalar lhs, Scalar rhs) { return lhs <= rhs; });
  }
  /*! Elementwise greater than comparison between this Matrix and other*/
  Matrix<bool> operator>(Matrix<Scalar> const &other) {
    return this->elementwise_binary_apply<bool>(
        other, [](Scalar lhs, Scalar rhs) { return lhs > rhs; });
  }
  /*! Elementwise greater than comparison between this Matrix and a Scalar
   * other*/
  Matrix<bool> operator>(Scalar other) {
    return this->elementwise_binary_apply<bool>(
        other, [](Scalar lhs, Scalar rhs) { return lhs > rhs; });
  }
  /*! Elementwise greater equal comparison between this Matrix and other*/
  Matrix<bool> operator>=(Matrix<Scalar> const &other) {
    return this->elementwise_binary_apply<bool>(
        other, [](Scalar lhs, Scalar rhs) { return lhs >= rhs; });
  }
  /*! Elementwise greater equal comparison between this Matrix and a Scalar
   * other*/
  Matrix<bool> operator>=(Scalar other) {
    return this->elementwise_binary_apply<bool>(
        other, [](Scalar lhs, Scalar rhs) { return lhs >= rhs; });
  }
  // SECTION: Boolean helper functions
  /*! Returns True if any of the elements of this Matrix are truthy*/
  bool any() {
    for (size_t row = 0; row < this->nrows; row++) {

      for (size_t col = 0; col < this->ncols; col++) {
        if (*(*this)(row, col)) {
          return true;
        }
      }
    }
    return false;
  }
  /*! Returns True if all of the elements of this Matrix are truthy*/
  bool all() {
    bool all_truthy = true;
    for (size_t row = 0; row < this->nrows; row++) {
      for (size_t col = 0; col < this->ncols; col++) {
        if (!(*(*this)(row, col))) {
          all_truthy = false;
        }
      }
    }
    return all_truthy;
  }
}; // namespace template<typenameScalar>class TeensyMatrix
} // namespace teensymat
