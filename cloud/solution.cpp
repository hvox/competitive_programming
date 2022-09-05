// Score: 164_878
// TLE: 2, 4

#include <iostream>
#include <math.h>
#include <set>
#include <vector>

#define DEBUG false

struct Server {
  int16_t total_cpu;
  int16_t total_ram;
  std::set<int> vms;
  int cpu_usage;
  int free_cpu;
  int free_ram;
  int penalties = 0;
};

struct VirtualServer {
  int16_t cpu;
  int16_t ram;
  int16_t cpu_usage;
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
  int steps[100] = {};
  std::vector<std::pair<int, int>> reallocations;
  for (int i = 0; i < NUMBER_OF_SERVERS; i++) {
    auto &u = SERVERS[i];
    if (u.cpu_usage * 10 <= u.total_cpu * 3 || steps[i] == 2)
      continue;
    for (int j = 0; j < NUMBER_OF_SERVERS; j++) {
      auto &v = SERVERS[j];
      if (i == j || steps[j] == 2 || v.cpu_usage * 10 > v.total_cpu * 3)
        continue;
      int vm_index = -1;
      for (int i : u.vms) {
        auto &vm = VMS[i];
        if ((v.cpu_usage + vm.cpu_usage) * 10 <= v.total_cpu * 3 &&
            v.free_cpu >= vm.cpu && v.free_ram >= vm.ram) {
          vm_index = i;
          break;
        }
      }
      if (vm_index == -1)
        continue;
      auto &vm = VMS[vm_index];
      steps[i]++;
      steps[j]++;
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
    cpu_usage = (cpu_usage * VMS[i].cpu + 999999) / 1000000;
    VMS[i].cpu_usage = cpu_usage;
  }
  for (int i = 0; i < NUMBER_OF_SERVERS; i++) {
    auto &srv = SERVERS[i];
    int cpu_usage = 0;
    for (auto &vm : srv.vms)
      cpu_usage += VMS[vm].cpu_usage;
    srv.cpu_usage = cpu_usage;
    if (DEBUG)
      std::cout << "Server #" << i << " cpu_usage: " << cpu_usage << " / "
                << srv.total_cpu << std::endl;
    if (srv.cpu_usage * 10 > srv.total_cpu * 3)
      ++srv.penalties;
  }
}

void update_total_penalty() {
  for (int i = 0; i < NUMBER_OF_SERVERS; i++) {
    auto &srv = SERVERS[i];
    if (srv.cpu_usage * 10 > srv.total_cpu * 3)
      TOTAL_PENALTY += pow(2.0, srv.penalties - 1) * srv.vms.size();
  }
  if (DEBUG)
    std::cout << "Total penalty: " << TOTAL_PENALTY << std::endl;
}

int main() {
  std::cin >> NUMBER_OF_SERVERS >> NUMBER_OF_VMS >> NUMBER_OF_TIME_POINTS;
  for (int i = 0; i < NUMBER_OF_SERVERS; i++) {
    std::cin >> SERVERS[i].total_cpu >> SERVERS[i].total_ram;
    SERVERS[i].free_cpu = SERVERS[i].total_cpu;
    SERVERS[i].free_ram = SERVERS[i].total_ram;
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
  update_total_penalty();
  for (int time = 1; time < NUMBER_OF_TIME_POINTS; time++) {
    reallocate_vms(time);
    update_statistics();
    update_total_penalty();
  }
}
