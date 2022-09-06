// Score: 88_182
// TLE: 2, 4

#include <iostream>
#include <math.h>
#include <set>
#include <vector>

#define DEBUG false
const int CPU_LIMIT = 300000;

struct Server {
  int16_t total_cpu;
  int16_t free_cpu;
  int16_t free_ram;
  std::set<int> vms;
  int cpu_usage;
  int16_t penalties = 0;
};

struct VirtualServer {
  int16_t cpu;
  int16_t ram;
  int cpu_usage;
};

int NUMBER_OF_SERVERS, NUMBER_OF_VMS, NUMBER_OF_TIME_POINTS;
long double TOTAL_PENALTY = 0;
Server SERVERS[100];
VirtualServer VMS[10000];

inline std::pair<int, int> move_vm(int vm, int source, int destination) {
  SERVERS[source].free_cpu += VMS[vm].cpu;
  SERVERS[source].free_ram += VMS[vm].ram;
  SERVERS[source].cpu_usage -= VMS[vm].cpu_usage;
  SERVERS[source].vms.erase(vm);
  TOTAL_PENALTY += VMS[vm].ram;
  SERVERS[destination].free_cpu -= VMS[vm].cpu;
  SERVERS[destination].free_ram -= VMS[vm].ram;
  SERVERS[destination].cpu_usage += VMS[vm].cpu_usage;
  SERVERS[destination].vms.insert(vm);
  return {vm + 1, destination + 1};
}

void reallocate_vms(int next_time_point) {
  if (TOTAL_PENALTY > 1e19) {
    std::cout << 0 << std::endl;
    return;
  }
  int steps[100] = {};
  bool moved[10000] = {};
  std::vector<std::pair<int, int>> reallocations;
  for (int i = 0; i < NUMBER_OF_SERVERS; i++) {
    auto &u = SERVERS[i];
    if (u.cpu_usage <= u.total_cpu * CPU_LIMIT || steps[i] == 2)
      continue;
    if (DEBUG)
      std::cout << "Let's move vms from server#" << i << std::endl;
    for (int j = 0; j < NUMBER_OF_SERVERS; j++) {
      auto &v = SERVERS[j];
      if (i == j || steps[j] == 2)
        continue;
      if (DEBUG)
        std::cout << "\tinto server#" << j << std::endl;
      if (v.cpu_usage > v.total_cpu * CPU_LIMIT &&
          (1 << u.penalties) * (u.vms.size()) <
              (1 << v.penalties) * (v.vms.size() + 1) + 512)
        continue;
      int vm_index = -1;
      for (int i : u.vms) {
        if (moved[i])
          continue;
        auto &vm = VMS[i];
        if (((v.cpu_usage + vm.cpu_usage) <= v.total_cpu * CPU_LIMIT ||
             (1 << u.penalties) * (u.vms.size()) >
                 vm.ram + (1 << v.penalties) * (v.vms.size() + 1)) &&
            v.free_cpu >= vm.cpu && v.free_ram >= vm.ram) {
          vm_index = i;
          break;
        }
      }
      if (vm_index == -1)
        continue;
      steps[i]++;
      steps[j]++;
      moved[vm_index] = true;
      reallocations.push_back(move_vm(vm_index, i, j));
      break;
    }
  }
  std::cout << reallocations.size();
  for (auto &[u, v] : reallocations)
    std::cout << " " << u << " " << v;
  std::cout << std::endl;
}

void update_statistics() {
  for (int i = 0; i < NUMBER_OF_VMS; i++) {
    int cpu_usage;
    std::cin >> cpu_usage;
    VMS[i].cpu_usage = cpu_usage * VMS[i].cpu;
  }
  for (int i = 0; i < NUMBER_OF_SERVERS; i++) {
    auto &srv = SERVERS[i];
    int cpu_usage = 0;
    for (auto &vm : srv.vms)
      cpu_usage += VMS[vm].cpu_usage;
    srv.cpu_usage = cpu_usage;
    if (srv.cpu_usage > srv.total_cpu * CPU_LIMIT) {
      TOTAL_PENALTY += pow(2.0, srv.penalties++) * srv.vms.size();
    }
    if (DEBUG)
      std::cout << "Server #" << i << " penalties: " << srv.penalties
                << " cpu_usage: " << cpu_usage / 1000000 << " / "
                << srv.total_cpu << std::endl;
  }
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
    SERVERS[parent - 1].vms.insert(i);
    SERVERS[parent - 1].free_cpu -= VMS[i].cpu;
    SERVERS[parent - 1].free_ram -= VMS[i].ram;
  }
  update_statistics();
  for (int time = 1; time < NUMBER_OF_TIME_POINTS; time++) {
    reallocate_vms(time);
    update_statistics();
  }
}
