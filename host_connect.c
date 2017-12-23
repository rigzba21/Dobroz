/*compile w/ gcc -g -Wall host_connection.c -o ex -lvirt */

#include <stdio.h>
#include <stdlib.h>
#include <libvirt/libvirt.h>

//Text Color Definitions (ANSI escape)
#define CYAN "\x1B[36m"
#define TXTRST "\x1B[0m"


//struct w/HostConnector
struct hst_cn {
    virConnectPtr host_connection;
    char *host_sys;
    int v_cpus;
    unsigned long long free_mem;
    //nodeinfo struct contains:
    /*
     char model[]  CPU Model
     unsigned long memory
     unsigned int cpus
     unsigned int mhz
     unsigned int nodes
     unsigned int sockets
     unsigned int cores
     unsigned int threads
     */
    virNodeInfo host_node_info;
} hst_cn;


int main(int argc, char *argv[]) {

    //connect to host
    hst_cn.host_connection = virConnectOpen("qemu:///system");
    if (hst_cn.host_connection == NULL) {
        fprintf(stderr, "Host connection KVM/Qemu failed!!!\n");
        return 0;
    }
    else {
        printf(CYAN "Host Connection: [KVM/Qemu] established\n" TXTRST);
        printf(CYAN "========================================\n" TXTRST);

    }
    //query hostname
    hst_cn.host_sys = virConnectGetHostname(hst_cn.host_connection);
    fprintf(stdout, CYAN "[Host]: %s\n" TXTRST, hst_cn.host_sys);
    printf(CYAN "=========================================\n" TXTRST);
    free (hst_cn.host_sys);

    //query available VirtualCPUs
    hst_cn.v_cpus = virConnectGetMaxVcpus(hst_cn.host_connection, NULL);
    fprintf(stdout, CYAN "[VCPUS]: %d\n" TXTRST, hst_cn.v_cpus);
    printf(CYAN "=========================================\n" TXTRST);

    //query available memory
    hst_cn.free_mem = virNodeGetFreeMemory(hst_cn.host_connection);
    fprintf(stdout, CYAN "[MEMORY]: %llu\n" TXTRST, hst_cn.free_mem);
    printf(CYAN "=========================================\n" TXTRST);
    printf(CYAN "=========================================\n" TXTRST);

    //query detailed host info
    printf(CYAN "[DETAILED HOST STATS:]\n");
    virNodeGetInfo(hst_cn.host_connection, &hst_cn.host_node_info);
    fprintf(stdout, "[CPU Model:] %s\n", hst_cn.host_node_info.model);
    fprintf(stdout, "[MEMORY SIZE:] %lukb\n", hst_cn.host_node_info.memory);
    fprintf(stdout, "[Active CPUs:] %u\n", hst_cn.host_node_info.cpus);
    fprintf(stdout, "[CPU Frequency:] %uMhz\n", hst_cn.host_node_info.mhz);
    fprintf(stdout, "[CPU Sockets per Node:] %u\n", hst_cn.host_node_info.sockets);
    fprintf(stdout, "[NUMA Nodes ([*1] if Uniform):] %u\n", hst_cn.host_node_info.nodes);
    fprintf(stdout, "[CPU Cores/Socket:] %u\n", hst_cn.host_node_info.cores);
    fprintf(stdout, "[CPU Threads/Core:] %u\n" TXTRST, hst_cn.host_node_info.threads);
    printf(CYAN "=========================================\n" TXTRST);

    //query vm stats here
    //
    //
    //
    printf(CYAN "[Closing Host KVM/Qemu connection]\n" TXTRST);
    virConnectClose(hst_cn.host_connection);
    return 1;
}





