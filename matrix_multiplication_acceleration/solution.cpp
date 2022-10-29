// Score: 39_567_143
// Final score: 63_022_700

/*

My solution is a combination of a few simple solutions that are good for different cases.

SOLUTION #1. Target case: D * N * N is very small.
There are only 2^(D*N*N) ways to put zeros in these matrices.
So we can check them all and find the optimal one.

SOLUTION #2. Target case: when nothing else works.
1. Randomly change a few elements into zeros or from zeros into original values.
   If the changed matrices give better score, keep them as new answer,
   otherwise discard changes.
2. Repeat #1 for two seconds

SOLUTION #3. Target case: number of matrices is too big for other solutions
Fill with zeros 40% of rows of the first matrix.
This gives us param=0.4 and acc=0.6, which is pretty good result.
However 0.4 * N is not always an integer number, so we have some extra acc to spend.
Because of that we can make a few more zeros. So we just make a few ones into zeros.

SOLUTION #4. Target case: number of matrices equals 3.
I fill x*n columns by zeros in the second matrix. With this approach we have param=1-x,
but acc depends on the deleted columns.
So I sort columns by their accuracies and fill first columns with zeros until acc=0.6

SOLUTION #5. Target case: number of matrices equals 4.
Do the same thing as in solution #4, but not only for the second matrix:
I also delete columns from third matrix.

SOLUTION #6. Target case: when D*N^4 is not very big
1. For each matrix element I calculate which will be acc if I make that element zero.
2. Sort elements by acc and fill first elements with zeros untill acc=0.6
*/

// include everything, that I will be using
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

#if _WIN32
#include <io.h>
#define ISATTY _isatty
#define FILENO _fileno
#else
#include <unistd.h>
#define ISATTY isatty
#define FILENO fileno
#endif
#define sqr(x) ((x)*(x))
bool output_is_tty() { return ISATTY(FILENO(stdout)); }

// Matrices are represented by 2-d arrays of integers
typedef array<array<int64_t, 100>, 100> Matrix;

// A few global variables, that are used everywhere
int N; // size of matrix
int ORIGINAL_NUMBER_OF_MULTIPLICATIONS;
int NUMBER_OF_MATRICES;
vector<Matrix> ORIGINAL_MATRICES;
Matrix ORIGINAL_PRODUCT;
Matrix ONES_MATRIX; // Matrix with ones on the main diagonal
system_clock::time_point THE_START_TIME; // Time of start of the program

// Matrix multiplication of two matrices
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

// Matrix multiplication of a few matrices
Matrix matmul(vector<Matrix> const &matrices) {
  Matrix matrix = matrices[0];
  for (int i = 1; i < matrices.size(); i++)
    matrix = matmul(matrix, matrices[i]);
  return matrix;
}

// Matrix multiplication of two matrices with counting of element multiplications
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

// Matrix multiplication with counting of element multiplications
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

// Prefix products of matrices
vector<Matrix> get_prefixes(vector<Matrix> const &matrices) {
  vector<Matrix> prefixes;
  prefixes.push_back(matrices[0]);
  for (int i = 1; i < matrices.size(); i++)
    prefixes.push_back(matmul(prefixes[i - 1], matrices[i]));
  return prefixes;
}

// Prefix products of matrices
vector<Matrix> get_suffixes(vector<Matrix> const &matrices) {
  vector<Matrix> suffixes;
  suffixes.push_back(*matrices.rbegin());
  for (int i = matrices.size() - 2; i >= 0; i--)
    suffixes.push_back(matmul(matrices[i], *suffixes.rbegin()));
  return suffixes;
}

// This function calculates the acc value for defined matrices:
// A - new matrix, score for which is currently being calculated
// B - the original product of matrices
double sum_div(Matrix const &A, Matrix const &B) {
  double acc = 0;
  for (int i = 0; i < N; i++) for (int j = 0; j < N; j++)
    acc += ((double)A[i][j]) / B[i][j];
  return acc / (N * N);
}

// Score calculation for defined matrices according to formula from task statement
int get_score(vector<Matrix> const &matrices) {
  auto [matrix, products] = matmulcnt(matrices);
  double acc = sum_div(matrix, ORIGINAL_PRODUCT);
  if (acc < 0.6) return 0;
  double param = ((double)products) / ORIGINAL_NUMBER_OF_MULTIPLICATIONS;
  return 5e6 * max(0.0, (acc - 1) / 2 + 1 - param);
}

// Read input and initialize a few global variables
void read_input() {
  // Save current time
  THE_START_TIME = high_resolution_clock::now();
  // Read input
  int d;
  cin >> d >> N;
  NUMBER_OF_MATRICES = d;
  vector<Matrix> matrices(d);
  matrices.resize(d);
  for (int i = 0; i < d; i++)
    for (int j = 0; j < N; j++)
      for (int k = 0; k < N; k++)
        cin >> matrices[i][j][k];
  // Initialize a few global variables
  auto [product, operations] = matmulcnt(matrices);
  ORIGINAL_PRODUCT = product;
  ORIGINAL_MATRICES = matrices;
  ORIGINAL_NUMBER_OF_MULTIPLICATIONS = operations;
  for (int i = 0; i < N; i++)
    ONES_MATRIX[i][i] = 1;
}

// Output the output
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
    cout << "d=" << ORIGINAL_MATRICES.size() << "  score = " << score << "\t " << "acc = " << acc << endl;
  }
}

/*
SOLUTION #1. Target case: D * N * N is very small.
There are only 2^(D*N*N) ways to put zeros in these matrices.
So we can check them all and find the optimal one.
*/
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

/*
SOLUTION #2. Target case: when nothing else works.
1. Randomly change a few elements into zeros or from zeros into original values.
   If the changed matrices give better score, keep them as new answer,
   otherwise discard changes.
2. Repeat #1 for two seconds
*/
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

/*
SOLUTION #3. Target case: number of matrices is too big for other solutions
Fill with zeros 40% of rows of the first matrix.
This gives us param=0.4 and acc=0.6, which is pretty good result.
However 0.4 * N is not always an integer number, so we have some extra acc to spend.
Because of that we can make a few more zeros. So we just make a few ones into zeros.
*/
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

/*
ALTERNATIVE SOLUTION #3
This solution is the same as previous one, but rows with biggest sums in
the matrix product are deleted first. And ones are not replaced by zeros.
This modifications *slightly* improve result of postprocessing...
*/
vector<Matrix> biggest_lines_to_zeros(vector<Matrix> matrices) {
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

/*
SOLUTION #4. Target case: number of matrices equals 3.
I fill x*n columns by zeros in the second matrix. With this approach we have param=1-x,
but acc depends on the deleted columns.
So I sort columns by their accuracies and fill first columns with zeros until acc=0.6
*/
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

/*
SOLUTION #5. Target case: number of matrices equals 4.
Do the same thing as in solution #4, but not only for the second matrix:
I also delete columns from third matrix.
*/
vector<Matrix> greedy_columns_to_zeros_multi() {
  vector<Matrix> matrices = ORIGINAL_MATRICES;
  auto prefixes = get_prefixes(matrices), suffixes = get_suffixes(matrices);
  vector<double> sums;
  vector<int> indexes;
  for (int m = 1; m < NUMBER_OF_MATRICES - 1; m++) {
    auto &prefix = prefixes[m], &suffix = suffixes[NUMBER_OF_MATRICES-m-2];
    for (int i = 0; i < N; i++) {
      double sum = 0;
      for (int x = 0; x < N; x++) for (int y = 0; y < N; y++)
        sum += (double) prefix[x][i] * suffix[i][y] / ORIGINAL_PRODUCT[x][y];
      indexes.push_back((m - 1) * N + i);
      sums.push_back(sum / N / N);
    }
  }
  sort(indexes.begin(), indexes.end(), [&](auto const &i, auto const &j) {
    return sums[i] < sums[j];
  });
  int i = 0;
  double acc = 1.0;
  while ((acc -= sums[indexes[i]]) >= 0.6) {
    for (int j = 0; j < N; j++)
      matrices[1 + indexes[i] / N][j][indexes[i] % N] = 0;
    i++;
  }
  return matrices;
}

/*
SOLUTION #6. Target case: when D*N^4 is not too big
1. For each matrix element I calculate which will be acc if I make that element zero.
2. Sort elements by acc and fill first elements with zeros untill acc=0.6
*/
vector<Matrix> static_greedy_with_approximation(vector<Matrix> matrices=ORIGINAL_MATRICES) {
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
      break;
    acc -= delta_acc;
    matrices[j / N / N][(j / N) % N][j % N] = 0;
  }
  return matrices;
}

// This is the same function, the only differense is sorting.
// In the previous function sorting was done by acc, but here it is done by 
// approximate number of saved multiplication divided by acc.
vector<Matrix> greedy_with_approximation(vector<Matrix> matrices) {
  const int matrices_size = matrices.size();
  auto prefixes = get_prefixes(matrices), suffixes = get_suffixes(matrices);
  vector<tuple<int, double, int>> candidates;
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
        delta_multiplications += 1; // TODO: improve counting of multiplications
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
  double acc = sum_div(matmul(matrices), ORIGINAL_PRODUCT);
  for (int i = 0; i < matrices_size * N * N; i++) {
    auto [j, delta_acc, delta_mults] = candidates[i];
    if (acc - delta_acc < 0.6 || delta_mults * 2 < delta_acc * ORIGINAL_NUMBER_OF_MULTIPLICATIONS)
      break;
    acc -= delta_acc;
    matrices[j / N / N][(j / N) % N][j % N] = 0;
  }
  return matrices;
}

// In solution #6 I don't update acc values of matrix elements,
// therefore the value of the acc is overrated.
// In order to fight this problem I apply the algorithm twise.
vector<Matrix> static_greedy_repeated(vector<Matrix> matrices) {
  matrices = static_greedy_with_approximation();
  return greedy_with_approximation(matrices);
}

int main() {
  read_input();
  srand(228); // This is absolutly random number
  if (NUMBER_OF_MATRICES * N * N <= 12)
    print_output(the_best()); // SOLUTION #1
  else if (ORIGINAL_MATRICES.size() == 5)
    // This is composition of sequention application of following solutions:
    // SOLUTION #3
    // SOLUTION #6
    // SOLUTION #2
    print_output(chronical_random_walk(static_greedy_with_approximation(biggest_lines_to_zeros(ORIGINAL_MATRICES))));
  else {
    // SOLUTION #6 repeated
    auto result = static_greedy_repeated(ORIGINAL_MATRICES);
    auto score = get_score(result);
    // try alternative solutions
    if (NUMBER_OF_MATRICES == 3) {
      // composition of SOLUTION #4 and SOLUTION #6
      auto alternative = static_greedy_with_approximation(greedy_columns_to_zeros());
      auto alt_score = get_score(alternative);
      if (alt_score > score) {
        result = alternative;
        score = alt_score;
      }
    }
    if (NUMBER_OF_MATRICES == 4) {
      // SOLUTION #5
      auto alternative = (greedy_columns_to_zeros_multi());
      auto alt_score = get_score(alternative);
      if (alt_score > score) {
        result = alternative;
        score = alt_score;
      }
    }
    // If current answer gives too small score, try another solutions
    if (score < 999000) {
      if (duration_cast<microseconds>(high_resolution_clock::now() - THE_START_TIME).count() < 1330666)
        // If there is not much time left, use composition of the following solutions:
        // SOLUTION #3, SOLUTION #6, SOLUTION #2
        result = chronical_random_walk(static_greedy_with_approximation(biggest_lines_to_zeros(ORIGINAL_MATRICES)));
      else
        // Otherwise use composition of the following solutions:
        // SOLUTION #3, SOLUTION #2
        result = chronical_random_walk(first_lines_and_ones_to_zeros(ORIGINAL_MATRICES));
    } else {
      // If current score is good enough, improve it by SOLUTION #2 until time limit is almost exceded
      result = chronical_random_walk(result);
    }
    // Output
    print_output(result);
  }
  return 0;
}
