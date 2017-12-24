
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <libvirt/libvirt.h>


//HostConnector
struct hst_cn {
    virConnectPtr host_connection;
    char *host_sys;
    int v_cpus;
    unsigned long long free_mem;
    virNodeInfo host_node_info;
    //nodeinfo struct contains:
    /*
        -> char model[]  CPU Model
        -> unsigned long memory
        -> unsigned int cpus
        -> unsigned int mhz
        -> unsigned int nodes
        ->unsigned int sockets
        ->unsigned int cores
        ->unsigned int threads
                */
    unsigned int encryp_status;
    virSecurityModel sec_model;
    // security model struct contains:
    // ->model
    // ->DOI
} hst_cn;

//Domains
struct domains {
    int i;
    int total_domains;
    int num_active, num_non_active;
    int *active_domains;
    char **non_active_domains;
    virDomainPtr *all_domains;
} domains;

//ncurses helper functions
void ncurses_init_func() {
    initscr();
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    scrollok(stdscr, TRUE);
}

void ncurses_color_on() {
    attron(COLOR_PAIR(1));
}

void ncurses_color_off() {
    attroff(COLOR_PAIR(1));
}

void bold_on() {
    attron(A_BOLD);
}

void bold_off() {
    attroff(A_BOLD);
}

void ncurses_continue() {
    printw("Press <ENTER> To Continue");
    refresh();
    getch();
}

void print_bars() {
 printw("========================================\n");
}

int main(int argc, char *argv[]) {

    //connect to host
    hst_cn.host_connection = virConnectOpen("qemu:///system");
    if (hst_cn.host_connection == NULL) {
        fprintf(stderr, "Host connection KVM/Qemu failed!!!\n");
        return 0;
    }
    else {
        ncurses_init_func();
        ncurses_color_on();
        print_bars();
        bold_on();
        printw("HOST CONNECTION: [KVM/Qemu] ESTABLISHED\n");
        bold_off();
        print_bars();
        ncurses_color_off();
        ncurses_continue();
    }
    //query hostname
    hst_cn.host_sys = virConnectGetHostname(hst_cn.host_connection);
    ncurses_color_on();
    printw("[HOST]: %s\n", hst_cn.host_sys);
    print_bars();
    free (hst_cn.host_sys);
    ncurses_color_off();
    ncurses_continue();


    //query available VirtualCPUs
    hst_cn.v_cpus = virConnectGetMaxVcpus(hst_cn.host_connection, NULL);
    ncurses_color_on();
    printw("[VCPUS]: %d\n", hst_cn.v_cpus);
    print_bars();

    //query available memory
    hst_cn.free_mem = virNodeGetFreeMemory(hst_cn.host_connection);
    printw("[MEMORY]: %llu\n", hst_cn.free_mem);
    print_bars();
    print_bars();
    ncurses_color_off();
    ncurses_continue();


    //query detailed host info
    ncurses_color_on();
    print_bars();
    bold_on();
    printw("[DETAILED HOST STATS:]\n");
    bold_off();
    print_bars();
    virNodeGetInfo(hst_cn.host_connection, &hst_cn.host_node_info);
    printw("[CPU Model:] %s\n", hst_cn.host_node_info.model);
    printw("[MEMORY SIZE:] %lukb\n", hst_cn.host_node_info.memory);
    printw("[Active CPUs:] %u\n", hst_cn.host_node_info.cpus);
    printw("[CPU Frequency:] %uMhz\n", hst_cn.host_node_info.mhz);
    printw("[CPU Sockets per Node:] %u\n", hst_cn.host_node_info.sockets);
    printw("[NUMA Nodes ([*1] if Uniform):] %u\n", hst_cn.host_node_info.nodes);
    printw("[CPU Cores/Socket:] %u\n", hst_cn.host_node_info.cores);
    printw("[CPU Threads/Core:] %u\n", hst_cn.host_node_info.threads);
    print_bars();
    bold_on();
    printw("[SECURITY DETAILS:]\n");
    bold_off();
    print_bars();
    ncurses_color_off();
    ncurses_continue();

    //get security details
    hst_cn.encryp_status = virConnectIsEncrypted(hst_cn.host_connection);
    virNodeGetSecurityModel(hst_cn.host_connection, &hst_cn.sec_model);
    ncurses_color_on();
    printw("[Secure Connection Status ([*1] Encrypted; [*0] Plaintext):] %u\n", hst_cn.encryp_status);
    printw("[Security Model:] %s\n", hst_cn.sec_model.model);
    printw("[Security DOI:] %s\n", hst_cn.sec_model.doi);
    print_bars();
    ncurses_color_off();
    ncurses_continue();

    //query vm stats here
    //query # of vm(domains)

    ncurses_color_on();
    print_bars();
    bold_on();
    printw("[VIRTUAL MACHINE STATS:]\n");
    bold_off();
    print_bars();

    domains.total_domains = 0;
    domains.num_active = virConnectNumOfDomains(hst_cn.host_connection);
    domains.num_non_active = virConnectNumOfDefinedDomains(hst_cn.host_connection);
    domains.all_domains = malloc(sizeof(virDomainPtr) * (domains.num_active + domains.num_non_active));
    domains.non_active_domains = malloc(sizeof(char *) * domains.num_non_active);
    domains.active_domains = malloc(sizeof(int) * domains.num_active);

    domains.num_active = virConnectListDomains(hst_cn.host_connection, domains.active_domains, domains.num_active);
    domains.num_non_active = virConnectListDefinedDomains(hst_cn.host_connection, domains.non_active_domains, domains.num_non_active);
    bold_on();
    printw("[Active VM Ids:]\n");
    bold_off();

    for (domains.i = 0; domains.i < domains.num_active; domains.i++) {
        domains.all_domains[domains.total_domains] = virDomainLookupByID(hst_cn.host_connection, domains.active_domains[domains.i]);
        printw("|*|[VM ID:] %d|*|\n", domains.active_domains[domains.i]);
        domains.total_domains++;
    }
    free(domains.active_domains);
    free(domains.non_active_domains);
    ncurses_color_off();
    ncurses_continue();
    //
    //
    ncurses_color_on();
    print_bars();
    bold_on();
    printw("[Closing Host KVM/Qemu connection]\n");
    virConnectClose(hst_cn.host_connection);
    print_bars();
    bold_off();
    ncurses_color_off();
    ncurses_continue();
    endwin();

    return 1;
}





