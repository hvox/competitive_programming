// Score: 15

#include <iostream>
#include <math.h>
#include <set>

#define DEBUG false

struct Server {
  int16_t total_cpu;
  int16_t total_ram;
  std::set<int> vms;
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

void reallocate_vms(int next_time_point) {
  // I don't have solution yet
  std::cout << 0 << std::endl;
}
void update_statistics() {
  for (int i = 0; i < NUMBER_OF_SERVERS; i++) {
    auto &srv = SERVERS[i];
    int cpu_usage = 0;
    for (auto &vm : srv.vms)
      cpu_usage += VMS[vm].cpu_usage;
    if (cpu_usage * 10 > srv.total_cpu * 3)
      TOTAL_PENALTY += pow(2.0, srv.penalties++) * srv.vms.size();
  }
}

int main() {
  std::cin >> NUMBER_OF_SERVERS >> NUMBER_OF_VMS >> NUMBER_OF_TIME_POINTS;
  for (int i = 0; i < NUMBER_OF_SERVERS; i++)
    std::cin >> SERVERS[i].total_cpu >> SERVERS[i].total_ram;
  for (int i = 0; i < NUMBER_OF_VMS; i++)
    std::cin >> VMS[i].cpu >> VMS[i].ram;
  for (int i = 0; i < NUMBER_OF_VMS; i++) {
    int parent;
    std::cin >> parent;
    SERVERS[parent - 1].vms.insert(i);
  }
  for (int i = 0; i < NUMBER_OF_VMS; i++)
    std::cin >> VMS[i].cpu_usage;
  for (int time = 1; time < NUMBER_OF_TIME_POINTS; time++) {
    reallocate_vms(time);
    for (int i = 0; i < NUMBER_OF_VMS; i++)
      std::cin >> VMS[i].cpu_usage;
    update_statistics();
  }
  if (DEBUG)
    std::cout << "Total penalty: " << TOTAL_PENALTY << std::endl;
}
