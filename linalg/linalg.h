// Zoltán Gilián <zoltan.gilian@gmail.com>
// 2014

#include <type_traits>

namespace linalg {

template <typename T, int M, int N>
struct Matrix
{
	typedef T elem_type;
	static constexpr int rows = M;
	static constexpr int cols = N;
	Matrix() {}
	Matrix(T t) { fill(t); }
	T operator()(int i, int j) const { return a[i][j]; }
	T & operator()(int i, int j) { return a[i][j]; }
	Matrix<T, N, M> transposed();
	void fill(T);
private:
	T a[M][N];
};
template <typename T, int M, int N>
void Matrix<T, M, N>::fill(T t)
{
	for (int i = 0; i < M; ++i)
		for (int j = 0; j < N; ++j)
			a[i][j] = t;
}

template <typename T, int M, int N>
Matrix<T, N, M> Matrix<T, M, N>::transposed()
{
	Matrix <T, N, M> m;
	for (int i = 0; i < a.rows; ++i)
		for (int j = 0; j < a.cols; ++j)
			m(j, i) = operator()(i, j);
	return m;
}

template <typename T, int M>
struct Vector: Matrix<T, M, 1>
{
	enum { length = M };
	Vector(T t = T()): Matrix<T, M, 1>(t) {}
	T operator()(int i) const { return operator()(i, 0); }
	T & operator()(int i) { return operator()(i, 0); }
};

template <typename Mat>
Mat operator-(Mat a)
{
	for (int i = 0; i < a.rows; ++i)
		for (int j = 0; j < a.cols; ++j)
			a(i, j) = -a(i, j);
	return a;
}

template <typename MatA, typename MatB,
	typename OutElemType = typename std::common_type<typename MatA::elem_type, typename MatB::elem_type>::type,
	typename std::enable_if<(MatA::rows == MatB::rows && MatA::cols == MatB::cols), int>::type = 0>
Matrix<OutElemType, MatA::rows, MatA::cols>
operator+(MatA const & a, MatB const & b)
{
	Matrix<OutElemType, MatA::rows, MatA::cols> c;
	for (int i = 0; i < a.rows; ++i)
		for (int j = 0; j < a.cols; ++j)
			c(i, j) = a(i, j) + b(i, j);
	return c;
}

template <typename MatA, typename MatB>
decltype(operator+(std::declval<MatA>(), std::declval<MatB>()))
operator-(MatA const & a, MatB const & b)
{
	return a + (-b);
}

template <typename MatA, typename MatB,
	typename OutElemType = typename std::common_type<typename MatA::elem_type, typename MatB::elem_type>::type,
	typename std::enable_if<(MatA::cols == MatB::rows), int>::type = 0>
Matrix<OutElemType, MatA::rows, MatB::cols>
operator*(MatA const & a, MatB const & b)
{
	Matrix<OutElemType, MatA::rows, MatB::cols> c;
	for (int i = 0; i < a.rows; ++i) {
		for (int j = 0; j < b.cols; ++j) {
			c(i, j) = OutElemType(0);
			for (int k = 0; k < a.cols; ++k) {
				c(i, j) += a(i, k) * b(k, j);
			}
		}
	}
	return c;
}

template <typename MatA, typename MatB,
	typename OutElemType = typename std::common_type<typename MatA::elem_type, typename MatB::elem_type>::type,
	typename std::enable_if<(MatA::rows == MatB::rows && MatA::cols == MatB::cols), int>::type = 0>
bool operator==(MatA const & a, MatB const & b)
{
	for (int i = 0; i < a.rows; ++i)
		for (int j = 0; j < b.cols; ++j)
		if (a(i, j) != b(i, j))
			return false;
	return true;
}

template <typename T, int M, int N>
Matrix<T, M, N> zeros()
{
	return Matrix<T, M, N>(0);
}

template <typename T, int M>
Matrix<T, M, M> identity()
{
	auto a = zeros<T, M, M>();
	for (int i = 0; i < M; ++i)
		a(i, i) = T(1);
	return a;
}

} // namespace linalg
