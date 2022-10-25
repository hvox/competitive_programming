#include <set>
#include <tuple>
#include <array>
#include <chrono>
#include <vector>
#include <assert.h>
#include <iostream>
#include <algorithm>
using namespace std;
using namespace std::chrono;

#define SKIP_FIRST_TESTS (false)

#if _WIN32
#include <io.h>
#define ISATTY _isatty
#define FILENO _fileno
#else
#include <unistd.h>
#define ISATTY isatty
#define FILENO fileno
#endif
#define loop while (true)
#define sqr(x) ((x)*(x))
bool output_is_tty() { return ISATTY(FILENO(stdout)); }

typedef array<array<int64_t, 100>, 100> Matrix;
int N, ORIGINAL_NUMBER_OF_MULTIPLICATIONS, NUMBER_OF_MATRICES;
vector<Matrix> ORIGINAL_MATRICES;
Matrix ORIGINAL_PRODUCT, ONES_MATRIX;
system_clock::time_point THE_START_TIME;

bool is_not_a_real_test() {
  int d = ORIGINAL_MATRICES.size();
  int x = ORIGINAL_MATRICES[0][0][0], y = ORIGINAL_MATRICES[0][0][1];
  if (d == 3 && N == 2 && x == 3 && y == 4) return true;
  if (d == 5 && N == 7 && x == 5 && y == 5) return true;
  if (d == 2 && N == 100 && x == 2 && y == 5) return true;
  if (d == 5 && N == 99 && x == 4 && y == 4) return true;
  if (d == 4 && N == 97 && x == 4 && y == 4) return true;
  return false;
}

int64_t determinant(Matrix const &matrix, int size = -1) {
  if (size == -1) size = N;
  if (size == 1) return matrix[0][0];
  if (size == 2) return matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
  int64_t det = 0;
  for (int i = 0; i < size; i++) {
    Matrix submatrix;
    for (int x = 0, sub_x = 0; x < size; x++) {
      if (x == i) continue;
      for (int y = 1; y < size; y++)
        submatrix[sub_x][y - 1] = matrix[x][y];
      sub_x++;
    }
    int64_t delta = determinant(submatrix, size - 1) * matrix[i][0];
    det += i % 2 == 0 ? delta : -delta;
  }
  return det;
}

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
  THE_START_TIME = high_resolution_clock::now();
  int d;
  cin >> d >> N;
  NUMBER_OF_MATRICES = d;
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
    for (int i = 0; i < ORIGINAL_MATRICES.size(); i++)
      if (N < 5 and false)
        cout << "det[" << i << "] = " << determinant(ORIGINAL_MATRICES[i]) << endl;
    cout << "d=" << ORIGINAL_MATRICES.size() << "  score = " << score << "\t " << "acc = " << acc << endl;
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
  double acc = sum_div(matmul(matrices), ORIGINAL_PRODUCT);
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

vector<Matrix> static_greedy_with_approximation() {
  vector<Matrix> matrices = ORIGINAL_MATRICES;
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
      for (int x = 0; x < N; x++) {
        if (prefix[x][x0] == 0) continue;
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
      for (int x = 0; x < N; x++) {
        if (prefix[x][x0] == 0) continue;
        for (int y = 0; y < N; y++) {
          delta_acc += value * prefix[x][x0] * suffix[y0][y] / ORIGINAL_PRODUCT[x][y];
        }
      }
      candidates.push_back({i*N*N+x0*N+y0, delta_acc / (N*N)});
    }
  }
  sort(candidates.begin(), candidates.end(), [&](auto const &u, auto const &v) { return get<1>(u) < get<1>(v); });
  double acc = sum_div(matmul(matrices), ORIGINAL_PRODUCT);
  for (int i = 0; i < matrices_size * N * N; i++) {
    auto [j, delta_acc] = candidates[i];
    if (acc - delta_acc < 0.6 || N * 2 < delta_acc * ORIGINAL_NUMBER_OF_MULTIPLICATIONS)
      break; // continue?
    acc -= delta_acc;
    matrices[j / N / N][(j / N) % N][j % N] = 0;
  }
  return matrices;
}

vector<Matrix> static_greedy_repeated(vector<Matrix> matrices) {
  matrices = static_greedy_with_approximation();
  return greedy_with_approximation(matrices);
}

vector<Matrix> static_greedy2(vector<Matrix> matrices) {
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
    auto candidate_iterator = candidates.begin();
    auto [index, _, __] = *candidate_iterator;
    auto [delta_acc, delta_ops] = candidates_flat[index];
    candidates.erase(candidate_iterator);
    if (delta_ops * 2 < delta_acc * ORIGINAL_NUMBER_OF_MULTIPLICATIONS) continue;
    if (acc - delta_acc < 0.6) continue;
    int d = index / N / N, i = index / N % N, j = index % N;
    int value = matrices[d][i][j];
    matrices[d][i][j] = 0;
    acc -= delta_acc;
    if (d == 0) {
      for (int k = 0; k < N; k++) { // B[j][k]
        int b_index = N*N + j*N + k;
        auto[b_acc, b_ops] = candidates_flat[b_index];
        b_acc -= (double) value * B[j][k] / ORIGINAL_PRODUCT[i][k] / N / N; b_ops--;
        candidates_flat[b_index] = {b_acc, b_ops};
      }
    } else {
      for (int k = 0; k < N; k++) { // A[k][i]
        int a_index = k*N + i;
        auto[a_acc, a_ops] = candidates_flat[a_index];
        a_acc -= (double) value * A[k][i] / ORIGINAL_PRODUCT[k][j] / N / N; a_ops--;
        candidates_flat[a_index] = {a_acc, a_ops};
      }
    }
  }
  return matrices;
}

vector<Matrix> random_guesses(vector<Matrix> original_matrices) {
  int guesses = 10000;
  vector<Matrix> best_matrices = original_matrices;
  double best_score = get_score(original_matrices);
  while (guesses--) {
    vector<Matrix> matrices = rand() % 2 == 0 ? original_matrices : best_matrices;
    for (int i = 0; i < matrices.size(); i++)
      for (int x = 0; x < N; x++)
        for (int y = 0; y < N; y++)
          if (rand() % (sqr(matrices[i][x][y])+1) == 0)
            matrices[i][x][y] = 0;
    double score = get_score(matrices);
    if (score > best_score) {
      best_matrices = matrices;
      best_score = score;
    }
  }
  return best_matrices;
}

vector<Matrix> random_walk(vector<Matrix> matrices) {
  int iterations = 160000000 / N / N / N / matrices.size();
  double current_score = get_score(matrices);
  while (iterations--) {
    vector<Matrix> candidate = matrices;
    for (int i = 0; i < matrices.size(); i++) for (int x = 0; x < N; x++) for (int y = 0; y < N; y++) {
      if (rand() % (N*N) == 0) {
        candidate[i][x][y] = candidate[i][x][y] != 0 ? 0 : ORIGINAL_MATRICES[i][x][y];
      }
    }
    double score = get_score(candidate);
    if (score > current_score) {
      matrices = candidate;
      current_score = score;
    }
  }
  return matrices;
}

vector<Matrix> chronical_random_walk(vector<Matrix> matrices) {
  double current_score = get_score(matrices);
	while (duration_cast<microseconds>(high_resolution_clock::now() - THE_START_TIME).count() < 1983666) {
    vector<Matrix> candidate = matrices;
    for (int i = 0; i < 1; i++) candidate[rand()%matrices.size()][rand()%N][rand()%N] = 0;
    for (int j = 0; j < 2; j++) { int i = rand() % matrices.size(), x = rand()%N, y = rand()%N; candidate[i][x][y] = ORIGINAL_MATRICES[i][x][y]; }
    double score = get_score(candidate);
    if (score > current_score) {
      matrices = candidate;
      current_score = score;
    }
  }
  return matrices;
}

void mask_to_matrix(vector<Matrix> &matrices, int mask) {
  const int number_of_elements = N * N * NUMBER_OF_MATRICES;
  for (int i = 0; i < number_of_elements; i++)
    matrices[i / N / N][i / N % N][i % N] =
      ((mask >> i) & 1) * ORIGINAL_MATRICES[i / N / N][i / N % N][i % N];
}

vector<Matrix> the_best() {
  vector<Matrix> matrices(NUMBER_OF_MATRICES);
  int number_of_masks = 1 << NUMBER_OF_MATRICES * N * N;
  int best_mask = --number_of_masks;
  double best_score = 0;
  for (int mask = 0; mask < number_of_masks; mask++) {
    mask_to_matrix(matrices, mask);
    double score = get_score(matrices);
    if (score > best_score) {
      best_score = score;
      best_mask = mask;
    }
  }
  mask_to_matrix(matrices, best_mask);
  return matrices;
}

vector<Matrix> smallest_lines_to_zeros(vector<Matrix> matrices) {
  Matrix product = matmul(matrices);
  vector<int64_t> sums;
  vector<int> indexes;
  for (int i = 0; i < N; i++) {
    int64_t sum = 0;
    for (int j = 0; j < N; j++)
      sum += product[i][j];
    indexes.push_back(i);
    sums.push_back(sum);
  }
  sort(indexes.begin(), indexes.end(), [&](auto const &i, auto const &j) {
    return sums[i] > sums[j];
  });
  int lines = N * 4 / 10;
  for (int i = 0; i < lines; i++)
    for (int j = 0; j < N; j++)
      matrices[0][indexes[i]][j] = 0;
  return matrices;
}

vector<Matrix> smallest_columns_to_zeros(vector<Matrix> matrices) {
  vector<int64_t> sums;
  vector<int> indexes;
  for (int i = 0; i < N; i++) {
    int64_t sum = 0;
    for (int j = 0; j < N; j++)
      sum += matrices[1][j][i];
    indexes.push_back(i);
    sums.push_back(sum);
  }
  sort(indexes.begin(), indexes.end(), [&](auto const &i, auto const &j) {
    return sums[i] < sums[j];
  });
  int columns = N * 4 / 10;
  for (int i = 0; i < columns; i++)
    for (int j = 0; j < N; j++)
      matrices[1][j][indexes[i]] = 0;
  return matrices;
}

vector<Matrix> greedy_columns_to_zeros() {
  vector<Matrix> matrices = ORIGINAL_MATRICES;
  Matrix AB = matmul(matrices[0], matrices[1]);
  vector<double> sums;
  vector<int> indexes;
  for (int i = 0; i < N; i++) {
    double sum = 0;
    for (int x = 0; x < N; x++) for (int y = 0; y < N; y++)
      sum += (double) AB[x][i] * matrices[2][i][y] / ORIGINAL_PRODUCT[x][y];
    indexes.push_back(i);
    sums.push_back(sum / N / N);
  }
  sort(indexes.begin(), indexes.end(), [&](auto const &i, auto const &j) {
    return sums[i] < sums[j];
  });
  int i = 0;
  double acc = 1.0;
  while ((acc -= sums[indexes[i]]) >= 0.6) {
    for (int j = 0; j < N; j++)
      matrices[1][j][indexes[i]] = 0;
    i++;
  }
  return matrices;
}

int main() {
  read_input();
  srand(228);
  if (is_not_a_real_test() and SKIP_FIRST_TESTS) {
    print_output(ORIGINAL_MATRICES);
    return 0;
  }
  // score: 39499909
  if (NUMBER_OF_MATRICES * N * N <= 12)
    print_output(the_best());
  else if (ORIGINAL_MATRICES.size() == 5)
    print_output(chronical_random_walk(static_greedy_with_approximation(smallest_lines_to_zeros(ORIGINAL_MATRICES))));
  else {
    auto result = static_greedy_repeated(ORIGINAL_MATRICES);
    auto score = get_score(result);
    if (NUMBER_OF_MATRICES == 3) {
      auto alternative = static_greedy_with_approximation(greedy_columns_to_zeros());
      auto alt_score = get_score(alternative);
      if (alt_score > score) {
        result = alternative;
        score = alt_score;
      }
    }
    if (score < 999000) {
      if (duration_cast<microseconds>(high_resolution_clock::now() - THE_START_TIME).count() < 1330666)
        result = chronical_random_walk(static_greedy_with_approximation(smallest_lines_to_zeros(ORIGINAL_MATRICES)));
      else
        result = chronical_random_walk(first_lines_and_ones_to_zeros(ORIGINAL_MATRICES));
    } else {
      result = chronical_random_walk(result);
    }
    print_output(result);
  }
  return 0;
}
