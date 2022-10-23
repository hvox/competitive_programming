#include <set>
#include <tuple>
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
Matrix ORIGINAL_PRODUCT, ONES_MATRIX;

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

vector<Matrix> get_prefixes(vector<Matrix> const &matrices) {
  vector<Matrix> prefixes;
  prefixes.push_back(matrices[0]);
  for (int i = 1; i < matrices.size(); i++)
    prefixes.push_back(matmul(prefixes[i - 1], matrices[i]));
  return prefixes;
}

vector<Matrix> get_suffixes(vector<Matrix> const &matrices) {
  vector<Matrix> suffixes;
  suffixes.push_back(*matrices.rbegin());
  for (int i = matrices.size() - 2; i >= 0; i--)
    suffixes.push_back(matmul(matrices[i], *suffixes.rbegin()));
  return suffixes;
}

double sum_div(Matrix const &A, Matrix const &B) {
  double acc = 0;
  for (int i = 0; i < N; i++) for (int j = 0; j < N; j++)
    acc += ((double)A[i][j]) / B[i][j];
  return acc / (N * N);
}

int get_score(vector<Matrix> const &matrices) {
  auto [matrix, products] = matmulcnt(matrices);
  double acc = sum_div(matrix, ORIGINAL_PRODUCT);
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
  // cout << "ORIGINAL_PRODUCT:"; for (int i = 0; i < N; i++) {cout << " [" << product[i][0]; for(int j = 1; j < N; j++) cout << " " << product[i][j]; cout << "]";} cout << endl;
  ORIGINAL_MATRICES = matrices;
  ORIGINAL_NUMBER_OF_MULTIPLICATIONS = operations;
  for (int i = 0; i < N; i++)
    ONES_MATRIX[i][i] = 1;

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
    double acc = sum_div(matmul(matrices), ORIGINAL_PRODUCT);
    cout << "score = " << score << "\t " << "acc = " << acc << endl;
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
  int lines = N * 4 / 10;
  for (int i = 0; i < lines; i++)
    for (int j = 0; j < N; j++)
      matrices[0][i][j] = 0;
  return matrices;
}

vector<Matrix> first_elements_to_zeros(vector<Matrix> matrices) {
  const int zeros = N * N * 4 / 10;
  for (int i = 0; i < zeros; i++)
    matrices[0][i / N][i % N] = 0;
  return matrices;
}

vector<Matrix> first_lines_and_ones_to_zeros(vector<Matrix> matrices) {
  int lines = N * 4 / 10;
  for (int i = 0; i < lines; i++)
    for (int j = 0; j < N; j++)
      matrices[0][i][j] = 0;
  const int zeros = N * N * 4 / 10;
  for (int j = 0; j < zeros - lines * N; j++)
    if (matrices[0][lines][j] == 1)
      matrices[0][lines][j] = 0;
  return matrices;
}

vector<Matrix> greedy_with_approximation(vector<Matrix> matrices) {
  const int matrices_size = matrices.size();
  auto prefixes = get_prefixes(matrices), suffixes = get_suffixes(matrices);
  // cout << "-- prefixes --" << endl; for (int i = 0; i < prefixes.size(); i++) {for (int j = 0; j < N; j++) { for (int k = 0; k < N; k++) cout << prefixes[i][j][k] << " "; cout << endl;} cout << endl;}
  // cout << "-- suffixes --" << endl; for (int i = 0; i < suffixes.size(); i++) {for (int j = 0; j < N; j++) { for (int k = 0; k < N; k++) cout << suffixes[i][j][k] << " "; cout << endl;} cout << endl;}
  vector<tuple<int, double, int>> candidates;
  for (int i = 0; i < matrices_size; i++) {
    auto &prefix = i > 0 ? prefixes[i - 1] : ONES_MATRIX;
    auto &suffix = i < matrices_size-1 ? suffixes[matrices_size-i-2] : ONES_MATRIX;
    // cout << "\ti = " << i << endl;
    Matrix &matrix = matrices[i];
    // cout << "prefix:"; for (int i = 0; i < N; i++) {cout << " [" << prefix[i][0]; for(int j = 1; j < N; j++) cout << " " << prefix[i][j]; cout << "]";} cout << endl;
    // cout << "matrix:"; for (int i = 0; i < N; i++) {cout << " [" << matrix[i][0]; for(int j = 1; j < N; j++) cout << " " << matrix[i][j]; cout << "]";} cout << endl;
    // cout << "suffix:"; for (int i = 0; i < N; i++) {cout << " [" << suffix[i][0]; for(int j = 1; j < N; j++) cout << " " << suffix[i][j]; cout << "]";} cout << endl;
    for (int x0 = 0; x0 < N; x0++) for (int y0 = 0; y0 < N; y0++) {
      double value = matrices[i][x0][y0];
      // if (value == 0) continue;
      double delta_acc = 0;
      int delta_multiplications = 0;
      for (int x = 0; x < N; x++) {
        if (prefix[x][x0] == 0) continue;
        delta_multiplications += 1; // TODO: have better counting of producs
        for (int y = 0; y < N; y++) {
          delta_acc += value * prefix[x][x0] * suffix[y0][y] / ORIGINAL_PRODUCT[x][y];
        }
      }
      candidates.push_back({i*N*N+x0*N+y0, delta_acc / (N*N), delta_multiplications});
    }
  }
  sort(candidates.begin(), candidates.end(), [&](auto const &u, auto const &v) {
    return get<2>(u) / get<1>(u) > get<2>(v) / get<1>(v);
  });
  // cout << "candidates:" << endl; for (auto [i, acc, mults] : candidates) cout << "\ti=" << i << " acc=" << acc << " mults=" << mults << " -> " << mults / acc << endl;
  double acc = 1;
  for (int i = 0; i < matrices_size * N * N; i++) {
    auto [j, delta_acc, delta_mults] = candidates[i];
    // cout << "iteration: " << i << " -> " << j << " " << delta_acc << " " << delta_mults << endl;
    if (acc - delta_acc < 0.6 || delta_mults * 2 < delta_acc * ORIGINAL_NUMBER_OF_MULTIPLICATIONS)
      break;
    acc -= delta_acc;
    matrices[j / N / N][(j / N) % N][j % N] = 0;
  }
  return matrices;
}

vector<Matrix> greedy2(vector<Matrix> matrices) {
  if (matrices.size() != 2) return matrices;
  Matrix &A = matrices[0], &B = matrices[1];
  auto cmp = [](tuple<int, double, int> const &u, tuple<int, double, int> const &v) {
    if (N > 5)
      return get<1>(u) < get<1>(v);
    return get<2>(u) * 2 - get<1>(u) * ORIGINAL_NUMBER_OF_MULTIPLICATIONS > get<2>(v) * 2 - get<1>(v) * ORIGINAL_NUMBER_OF_MULTIPLICATIONS;
  };
  set<tuple<int, double, int>, decltype(cmp)> candidates;
  vector<tuple<double, int>> candidates_flat(N*N*2);
  for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) {
    double a_acc = 0; for (int k = 0; k < N; k++) a_acc += ((double) B[j][k]) / ORIGINAL_PRODUCT[i][k];
    double b_acc = 0; for (int k = 0; k < N; k++) b_acc += ((double) A[k][i]) / ORIGINAL_PRODUCT[k][j];
    candidates.insert({i * N + j, a_acc * A[i][j] / N / N, N});
    candidates_flat[i*N+j] = {a_acc * A[i][j] / N / N, N};
    candidates.insert({N*N + i * N + j, b_acc * B[i][j] / N / N, N});
    candidates_flat[N*N+i*N+j] = {b_acc * B[i][j] / N / N, N};
  }
  double acc = 1.0;
  while (not candidates.empty()) {
    // cout << "cands:\n"; for (auto [i, acc, zeros] : candidates) cout << "AB"[i / N / N] << " " << i / N % N << " " << i % N << " -> " << acc << " " << zeros << "\n";
    auto candidate_iterator = candidates.begin();
    auto [index, delta_acc, delta_ops] = *candidate_iterator;
    candidates.erase(candidate_iterator);
    if (delta_ops * 2 < delta_acc * ORIGINAL_NUMBER_OF_MULTIPLICATIONS) continue;
    if (acc - delta_acc < 0.6) continue;
    int d = index / N / N, i = index / N % N, j = index % N;
    // cout << "delete " << d << " " << i << " " << j << endl;
    int value = matrices[d][i][j];
    matrices[d][i][j] = 0;
    acc -= delta_acc;
    if (d == 0) {
      for (int k = 0; k < N; k++) { // B[j][k]
        int b_index = N*N + j*N + k;
        auto[b_acc, b_ops] = candidates_flat[b_index];
        if (candidates.erase({b_index, b_acc, b_ops}) == 0) continue;
        b_acc -= (double) value * B[j][k] / ORIGINAL_PRODUCT[i][k] / N / N; b_ops--;
        candidates_flat[b_index] = {b_acc, b_ops};
        candidates.insert({b_index, b_acc, b_ops});
      }
    } else {
      for (int k = 0; k < N; k++) { // A[k][i]
        int a_index = k*N + i;
        auto[a_acc, a_ops] = candidates_flat[a_index];
        if (candidates.erase({a_index, a_acc, a_ops}) == 0) continue;
        a_acc -= (double) value * A[k][i] / ORIGINAL_PRODUCT[k][j] / N / N; a_ops--;
        candidates_flat[a_index] = {a_acc, a_ops};
        candidates.insert({a_index, a_acc, a_ops});
      }
    }
  }
  return matrices;
}

vector<Matrix> static_greedy_with_approximation(vector<Matrix> matrices) {
  const int matrices_size = matrices.size();
  auto prefixes = get_prefixes(matrices), suffixes = get_suffixes(matrices);
  vector<tuple<int, double>> candidates;
  for (int i = 0; i < matrices_size; i++) {
    auto &prefix = i > 0 ? prefixes[i - 1] : ONES_MATRIX;
    auto &suffix = i < matrices_size-1 ? suffixes[matrices_size-i-2] : ONES_MATRIX;
    Matrix &matrix = matrices[i];
    for (int x0 = 0; x0 < N; x0++) for (int y0 = 0; y0 < N; y0++) {
      double value = matrices[i][x0][y0];
      double delta_acc = 0;
      int delta_multiplications = 0;
      for (int x = 0; x < N; x++) {
        if (prefix[x][x0] == 0) continue;
        delta_multiplications += 1; // TODO: have better counting of producs
        for (int y = 0; y < N; y++) {
          delta_acc += value * prefix[x][x0] * suffix[y0][y] / ORIGINAL_PRODUCT[x][y];
        }
      }
      candidates.push_back({i*N*N+x0*N+y0, delta_acc / (N*N)});
    }
  }
  sort(candidates.begin(), candidates.end(), [&](auto const &u, auto const &v) { return get<1>(u) < get<1>(v); });
  double acc = 1;
  for (int i = 0; i < matrices_size * N * N; i++) {
    auto [j, delta_acc] = candidates[i];
    if (acc - delta_acc < 0.6 || N * 2 < delta_acc * ORIGINAL_NUMBER_OF_MULTIPLICATIONS)
      break; // continue?
    acc -= delta_acc;
    matrices[j / N / N][(j / N) % N][j % N] = 0;
  }
  return matrices;
}

int main() {
  read_input();
  auto result = static_greedy_with_approximation(ORIGINAL_MATRICES);
  print_output(result);
  return 0;
}
