// Score: 11_056_076

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

std::vector<int> get_relocation_candidate() {
  int steps[100] = {};
  std::vector<int> reallocations(NUMBER_OF_VMS);
  std::vector<Server> servers(NUMBER_OF_SERVERS);
  for (int i = 0; i < NUMBER_OF_SERVERS; i++)
    servers[i] = SERVERS[i];
  for (int i = 0; i < NUMBER_OF_VMS; i++) {
    reallocations[i] = VMS[i].home;
    auto &u = VMS[i];
    auto &srv = servers[u.home];
    if (steps[u.home] == 2 || srv.cpu_usage <= srv.total_cpu * CPU_LIMIT)
      continue;
    if (DEBUG)
      std::cout << "Try to move VM#" << i << " from server#" << (int)u.home
                << std::endl;
    int limit = NUMBER_OF_TIME_POINTS == 432 ? (int)1e7 / NUMBER_OF_VMS : 4;
    for (int t = 0; t < limit; t++) {
      int j = fast_randint() % NUMBER_OF_SERVERS;
      auto &v = servers[j];
      if (u.home == j || steps[j] == 2 || v.free_cpu < u.cpu ||
          v.free_ram < u.ram)
        continue;
      int score = (v.total_cpu - v.free_cpu) * 10 + u.max_cpu_usage / 100000 <=
                          v.total_cpu * 3
                      ? 0
                      : (1 << v.penalties) * (v.total_vms + 1);
      if (score + u.ram >= (1 << srv.penalties) * srv.total_vms)
        continue;
      if (DEBUG)
        std::cout << "\tmoved to server#" << j << std::endl;
      steps[u.home]++;
      steps[j]++;
      int source = VMS[i].home;
      servers[source].free_cpu += VMS[i].cpu;
      servers[source].free_ram += VMS[i].ram;
      servers[source].cpu_usage -= VMS[i].cpu_usage;
      servers[source].total_vms--;
      reallocations[i] = j;
      servers[j].free_cpu -= VMS[i].cpu;
      servers[j].free_ram -= VMS[i].ram;
      servers[j].cpu_usage += VMS[i].cpu_usage;
      servers[j].total_vms++;
      break;
    }
  }
  return reallocations;
}

void reallocate_vms(int next_time_point) {
  auto next_homes = get_relocation_candidate();
  std::vector<std::pair<int, int>> reallocation_list;
  for (int i = 0; i < NUMBER_OF_VMS; i++)
    if (next_homes[i] != VMS[i].home)
      reallocation_list.push_back(move_vm(i, next_homes[i]));
  std::cout << reallocation_list.size();
  for (auto reallocation : reallocation_list)
    std::cout << " " << reallocation.first << " " << reallocation.second;
  std::cout << std::endl;
}

bool cmp(VirtualServer &a, VirtualServer &b) {
  // Comparator should implement strict inequality, but my doesn't.
  // In other words this is undefined behavior, but I won't fix it,
  // since in this case the behavior is "get more points"
  return a.cpu >= b.cpu;
}
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
  std::ios::sync_with_stdio(false);
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
  std::sort(VMS, VMS + NUMBER_OF_VMS - 1, cmp);
  for (int time = 1; time < NUMBER_OF_TIME_POINTS; time++) {
    reallocate_vms(time);
    update_statistics();
  }
}
