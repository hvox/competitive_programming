// Score: 9_835_480
// TLE: 2, 4

#include <algorithm>
#include <assert.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <set>
#include <vector>

#define DEBUG false
const int CPU_LIMIT = 300000;

struct Server {
  int16_t total_cpu;
  int16_t free_cpu;
  int16_t free_ram;
  int16_t total_vms;
  int8_t penalties = 0;
  int64_t cpu_usage = 0;
};

struct VirtualServer {
  int cpu_usage = 0;
  int max_cpu_usage = 0;
  int16_t cpu;
  int16_t ram;
  int8_t home;
  int16_t id;
};

int NUMBER_OF_SERVERS, NUMBER_OF_VMS, NUMBER_OF_TIME_POINTS;
long double TOTAL_PENALTY = 0;
Server SERVERS[100];
VirtualServer VMS[10000];

uint64_t fast_randint(void) {
  static uint64_t x = 123456789, y = 362436069, z = 521288629;
  uint64_t t;
  x ^= x << 16;
  x ^= x >> 5;
  x ^= x << 1;
  t = x, x = y, y = z;
  return z = t ^ x ^ y;
}

inline std::pair<int, int> move_vm(int vm, int destination) {
  int source = VMS[vm].home;
  SERVERS[source].free_cpu += VMS[vm].cpu;
  SERVERS[source].free_ram += VMS[vm].ram;
  SERVERS[source].cpu_usage -= VMS[vm].cpu_usage;
  SERVERS[source].total_vms--;
  VMS[vm].home = destination;
  SERVERS[destination].free_cpu -= VMS[vm].cpu;
  SERVERS[destination].free_ram -= VMS[vm].ram;
  SERVERS[destination].cpu_usage += VMS[vm].cpu_usage;
  SERVERS[destination].total_vms++;
  TOTAL_PENALTY += VMS[vm].ram;
  return {VMS[vm].id + 1, destination + 1};
}

void reallocate_vms(int next_time_point) {
  if (TOTAL_PENALTY > 1e19) {
    std::cout << 0 << std::endl;
    return;
  }
  int steps[100] = {};
  std::vector<std::pair<int, int>> reallocations;
  for (int i = 0; i < NUMBER_OF_VMS; i++) {
    auto &u = VMS[i];
    auto &srv = SERVERS[u.home];
    if (steps[u.home] == 2 || srv.cpu_usage <= srv.total_cpu * CPU_LIMIT)
      continue;
    if (DEBUG)
      std::cout << "Try to move VM#" << i << " from server#" << (int)u.home
                << std::endl;
    for (int t = 0; t < 1e4 / NUMBER_OF_VMS; t++) {
      int j = fast_randint() % NUMBER_OF_SERVERS;
      auto &v = SERVERS[j];
      if (u.home == j || steps[j] == 2 || v.free_cpu < u.cpu ||
          v.free_ram < u.ram)
        continue;
      int score =
          (v.total_cpu - v.free_cpu) * 10 + u.cpu * 10 <= v.total_cpu * 3
              ? 0
              : (1 << v.penalties) * (v.total_vms + 1);
      if (score + u.ram >= (1 << srv.penalties) * srv.total_vms)
        continue;
      if (DEBUG)
        std::cout << "\tmoved to server#" << j << std::endl;
      steps[u.home]++;
      steps[j]++;
      reallocations.push_back(move_vm(i, j));
      break;
    }
  }
  std::cout << reallocations.size();
  for (auto &[u, v] : reallocations)
    std::cout << " " << u << " " << v;
  std::cout << std::endl;
}

bool cmp(VirtualServer &a, VirtualServer &b) { return a.cpu >= b.cpu; }
void update_statistics() {
  int cpu_usages[10000];
  for (int i = 0; i < NUMBER_OF_VMS; i++)
    std::cin >> cpu_usages[i];
  for (int i = 0; i < NUMBER_OF_VMS; i++) {
    int cpu_usage = cpu_usages[VMS[i].id] * VMS[i].cpu;
    VMS[i].max_cpu_usage = std::max(VMS[i].max_cpu_usage, cpu_usage);
    int delta = cpu_usage - VMS[i].cpu_usage;
    VMS[i].cpu_usage += delta;
    SERVERS[VMS[i].home].cpu_usage += delta;
  }
  for (int i = 0; i < NUMBER_OF_SERVERS; i++) {
    auto &srv = SERVERS[i];
    if (srv.cpu_usage > srv.total_cpu * CPU_LIMIT) {
      TOTAL_PENALTY += std::pow(2.0, srv.penalties++) * srv.total_vms;
    }
    if (DEBUG)
      std::cout << "Server #" << i << " penalties: " << (int)srv.penalties
                << "  cpu_usage: " << srv.cpu_usage / 1000000 << " / "
                << srv.total_cpu << std::endl;
  }
  if (DEBUG)
    std::cout << "Total penalty: " << TOTAL_PENALTY << std::endl;
}

int main() {
  std::cin >> NUMBER_OF_SERVERS >> NUMBER_OF_VMS >> NUMBER_OF_TIME_POINTS;
  assert(NUMBER_OF_SERVERS <= 100);
  assert(NUMBER_OF_VMS <= 10000);
  assert(NUMBER_OF_TIME_POINTS == 432 or NUMBER_OF_TIME_POINTS == 5);
  for (int i = 0; i < NUMBER_OF_SERVERS; i++) {
    std::cin >> SERVERS[i].free_cpu >> SERVERS[i].free_ram;
    SERVERS[i].total_cpu = SERVERS[i].free_cpu;
  }
  for (int i = 0; i < NUMBER_OF_VMS; i++)
    std::cin >> VMS[i].cpu >> VMS[i].ram;
  for (int i = 0; i < NUMBER_OF_VMS; i++) {
    int parent;
    std::cin >> parent;
    VMS[i].id = i;
    VMS[i].home = --parent;
    SERVERS[parent].free_cpu -= VMS[i].cpu;
    SERVERS[parent].free_ram -= VMS[i].ram;
    SERVERS[parent].total_vms++;
  }
  update_statistics();
  std::sort(VMS, VMS + NUMBER_OF_VMS, cmp);
  for (int time = 1; time < NUMBER_OF_TIME_POINTS; time++) {
    reallocate_vms(time);
    update_statistics();
  }
}
