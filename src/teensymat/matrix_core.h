#ifndef TEENSYOPT_TEENSYMAT_MATRIX_CORE
#define TEENSYOPT_TEENSYMAT_MATRIX_CORE
// std includes
#include <vector>

namespace teensymat {
/*! A class representing a two dimensional array*/
template <typename Scalar> class Matrix {
private:
  std::vector<Scalar> data;
  size_t row_stride;
  size_t col_stride;
  size_t nrows;
  size_t ncols;
  size_t matrix_size;

public:
  Matrix() : row_stride(0), col_stride(0), nrows(0), ncols(0), matrix_size(0) {
    this->data = std::vector<Scalar>{};
  };
  Matrix(size_t nrows, size_t ncols) : Matrix(nrows, ncols, 0) {};
  Matrix(size_t nrows, size_t ncols, Scalar elem) : nrows(nrows), ncols(ncols) {
    this->matrix_size = nrows * ncols;
    this->row_stride = ncols;
    this->row_stride = 1;
    this->data = std::vector<Scalar>{this->matrix_size, elem};
  }
}; // namespace template<typenameScalar>class TeensyMatrix
} // namespace teensymat
#endif // TEENSYOPT_TEENSYMAT_MATRIX_CORE
