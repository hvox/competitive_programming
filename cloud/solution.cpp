// Score: 928_787
// TLE: 2, 4

#include <algorithm>
#include <cmath>
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
  int cpu_usage = 0;
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
    if (steps[u.home] == 1 || srv.cpu_usage <= srv.total_cpu * CPU_LIMIT)
      continue;
    if (DEBUG)
      std::cout << "Try to move VM#" << i << " from server#" << (int)u.home
                << std::endl;
    int best_candidate = -1;
    int best_score = 1e9;
    for (int j = 0; j < NUMBER_OF_SERVERS; j++) {
      auto &v = SERVERS[j];
      if (u.home == j || steps[j] == 1 || v.free_cpu < u.cpu ||
          v.free_ram < u.ram)
        continue;
      int score = v.cpu_usage + u.cpu * 1e6 <= v.total_cpu * CPU_LIMIT
                      ? 0
                      : (1 << v.penalties) * (v.total_vms + 1);
      if (score < best_score) {
        best_score = score;
        best_candidate = j;
      }
    }
    if (best_candidate == -1 ||
        best_score + u.ram >= (1 << srv.penalties) * srv.total_vms)
      continue;
    if (DEBUG)
      std::cout << "\tmoved to server#" << best_candidate << std::endl;
    steps[u.home]++;
    steps[best_candidate]++;
    reallocations.push_back(move_vm(i, best_candidate));
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
  std::sort(VMS, VMS + NUMBER_OF_VMS, cmp);
  if (DEBUG)
    std::cout << "Total penalty: " << TOTAL_PENALTY << std::endl;
}

int main() {
  std::cin >> NUMBER_OF_SERVERS >> NUMBER_OF_VMS >> NUMBER_OF_TIME_POINTS;
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
  for (int time = 1; time < NUMBER_OF_TIME_POINTS; time++) {
    reallocate_vms(time);
    update_statistics();
  }
}
