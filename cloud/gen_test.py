import random

number_of_servers = 100
number_of_vms = 10000
number_of_time_points = 432
servers = [
    (random.randint(100, 1000), random.randint(150, 4000))
    for _ in range(number_of_servers)
]
free_resources = list(map(list, servers))
vms = []
for _ in range(number_of_vms):
    i = random.randint(0, len(servers) - 1)
    if min(free_resources[i]) == 0:
        free_resources[i] = list(x + 1 for x in free_resources[i])
        servers[i] = tuple(x + 1 for x in servers[i])
    cpu = random.randint(1, (free_resources[i][0] + 9)//10)
    ram = random.randint(1, (free_resources[i][1]+9)//10)
    free_resources[i][0] -= cpu
    free_resources[i][1] -= ram
    vms.append((cpu, ram, i))

print(number_of_servers, number_of_vms, number_of_time_points)
for server in servers:
    print(" ".join(map(str, server)))
for vm in vms:
    print(" ".join(map(str, vm[:2])))
for vm in vms:
    print(vm[2] + 1)
for _ in range(number_of_time_points):
    cpu_usage = [random.randint(10**3, 10**6) for _ in range(number_of_vms)]
    print(" ".join(map(str, cpu_usage)))
