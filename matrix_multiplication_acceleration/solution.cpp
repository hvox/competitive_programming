#include <array>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

#if _WIN32
#include <io.h>
#define ISATTY _isatty
#define FILENO _fileno
#else
#include <unistd.h>
#define ISATTY isatty
#define FILENO fileno
#endif
bool output_is_tty() { return ISATTY(FILENO(stdout)); }

typedef array<array<int64_t, 100>, 100> Matrix;
int N, ORIGINAL_NUMBER_OF_MULTIPLICATIONS;
vector<Matrix> ORIGINAL_MATRICES;
Matrix ORIGINAL_PRODUCT;

Matrix matmul(Matrix const &A, Matrix const &B) {
  Matrix matrix;
  for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) {
    int64_t sum = 0;
    for (int k = 0; k < N; k++)
      sum += A[i][k] * B[k][j];
    matrix[i][j] = sum;
  }
  return matrix;
}

Matrix matmul(vector<Matrix> const &matrices) {
  Matrix matrix = matrices[0];
  for (int i = 1; i < matrices.size(); i++)
    matrix = matmul(matrix, matrices[i]);
  return matrix;
}

pair<Matrix, int> matmulcnt(Matrix const &A, Matrix const &B) {
  Matrix matrix;
  int products = 0;
  for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) {
    int64_t sum = 0;
    for (int k = 0; k < N; k++) if (A[i][k] > 0 && B[k][j] > 0) {
      sum += A[i][k] * B[k][j];
      products++;
    }
    matrix[i][j] = sum;
  }
  return {matrix, products};
}

pair<Matrix, int> matmulcnt(vector<Matrix> const &matrices) {
  Matrix matrix = matrices[0];
  int total_number_of_operations = 0;
  for (int i = 1; i < matrices.size(); i++) {
    auto [new_matrix, operations] = matmulcnt(matrix, matrices[i]);
    total_number_of_operations += operations;
    matrix = new_matrix;
  }
  return {matrix, total_number_of_operations};
}

int get_score(vector<Matrix> const &matrices) {
  auto [matrix, products] = matmulcnt(matrices);
  double acc = 0;
  for (int i = 0; i < N; i++) for (int j = 0; j < N; j++)
    acc += ((double)matrix[i][j]) / ORIGINAL_PRODUCT[i][j];
  acc /= N * N;
  // cout << "acc = " << acc << endl;
  if (acc < 0.6) return 0;
  double param = ((double)products) / ORIGINAL_NUMBER_OF_MULTIPLICATIONS;
  return 5e6 * max(0.0, (acc - 1) / 2 + 1 - param);
}

void read_input() {
  int d;
  cin >> d >> N;
  vector<Matrix> matrices(d);
  matrices.resize(d);
  for (int i = 0; i < d; i++)
    for (int j = 0; j < N; j++)
      for (int k = 0; k < N; k++)
        cin >> matrices[i][j][k];
  auto [product, operations] = matmulcnt(matrices);
  ORIGINAL_PRODUCT = product;
  ORIGINAL_MATRICES = matrices;
  ORIGINAL_NUMBER_OF_MULTIPLICATIONS = operations;
}

void print_output(vector<Matrix> const &matrices) {
  if (!output_is_tty()) {
    for (auto matrix: matrices)
      for (int i = 0; i < N; i++) {
        cout << matrix[i][0];
        for (int j = 1; j < N; j++)
          cout << " " << matrix[i][j];
        cout << endl;
      }
  } else {
    int score = get_score(matrices);
    cout << "score = " << score << endl;
  }
}

vector<Matrix> ones_to_zeros(vector<Matrix> matrices) {
  int max_x = N < 5 ? matrices.size() - 2 : matrices.size() - 1;
  for (int x = 0; x <= max_x; x++)
    for (int y = 0; y < N; y++)
      for (int z = 0; z < N; z++)
        if (matrices[x][y][z] == 1)
          matrices[x][y][z] = 0;
  return matrices;
}

vector<Matrix> first_lines_to_zeros(vector<Matrix> matrices) {
  int lines = 0.4 * N;
  for (int i = 0; i < lines; i++)
    for (int j = 0; j < N; j++)
      matrices[0][i][j] = 0;
  return matrices;
}

int main() {
  read_input();
  auto result = first_lines_to_zeros(ORIGINAL_MATRICES);
  print_output(result);
  return 0;
}
