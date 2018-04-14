//see makefile for compilation flags
//
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <libvirt/libvirt.h>
#include <omp.h> //need to establish critical sections for threads
#include <menu.h>
#include <unistd.h>

#define SCREEN_DELAY 50000

WINDOW *new_window(int h, int w, int win_y, int win_x) {
	WINDOW *this_window;
	this_window = newwin(h, w, win_y, win_x);
	box(this_window, 0, 0);
	wrefresh(this_window);
	return this_window;
}

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

//Domain Stats
struct dom_stats {
    virTypedParameterPtr params;
    virDomainInfo dom_info;
    virDomainPtr dom;
    unsigned int stats;
    virDomainStatsRecordPtr ** retStats;
    int dom_id;
    unsigned int nparams;
    int start_cpu;
    unsigned int ncpus;
    unsigned int flags;
} dom_stats;

//ncurses helper functions
void ncurses_init_func() {
    initscr();
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    scrollok(stdscr, TRUE);
    keypad(stdscr, TRUE);
    cbreak();//added for key input support
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

void window_destroy(WINDOW *this_window) {
	wrefresh(this_window);
	delwin(this_window);
}

void print_top_box() {
    printw("*----------------------------------------*\n");
}

void get_vm_stats() {
 for (domains.i = 0; domains.i < domains.num_active; domains.i++) {
         domains.all_domains[domains.total_domains] = virDomainLookupByID(hst_cn.host_connection,
                domains.active_domains[domains.i]);
                virDomainGetInfo(domains.all_domains[domains.total_domains], &dom_stats.dom_info);//& pointer required
                bold_on();
                printw("[VM ID:] %d\n", domains.active_domains[domains.i]);
                bold_off();
                print_top_box();
                printw("||->[vCPUs:] %d                          ||\n", dom_stats.dom_info.nrVirtCpu);
                printw("||->[CPU Time:] %llu nanoseconds ||\n", dom_stats.dom_info.cpuTime);
                printw("||->[Memory Used:] %llukb             ||\n", dom_stats.dom_info.memory);
                printw("||->[Max Mem Allowed:] %llukb         ||\n", dom_stats.dom_info.maxMem);
                printw("||->[VM State:] %u                       ||\n", dom_stats.dom_info.state);
                print_top_box();
                printw("THREAD: %d\n", omp_get_thread_num());
                domains.total_domains++;
    }
}

int stats_refresh_opt(int key_press) {
    printw("Press <ENTER> to Refresh VM Stats, otherwise press <ESC>");
    key_press = getch();
    if (key_press != KEY_ENTER) {
    return key_press;
    }
    else {
        ncurses_continue();
        return key_press;
    }
}


void print_bars() {
 printw("========================================\n");
}

int main(int argc, char *argv[]) {

    omp_set_num_threads(10);
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

    if (virConnectGetHostname(hst_cn.host_connection) != NULL) {
            hst_cn.host_sys = virConnectGetHostname(hst_cn.host_connection);
            ncurses_color_on();
            printw("[HOST]: %s\n", hst_cn.host_sys);
            print_bars();
            free (hst_cn.host_sys);
            ncurses_color_off();
            ncurses_continue();
        }
    else  {
            printw("Error Resolving HostName");
            ncurses_continue();
            return 0;
            }


    //query available VirtualCPUs
    if (virConnectGetMaxVcpus(hst_cn.host_connection, NULL) != -1) {
            hst_cn.v_cpus = virConnectGetMaxVcpus(hst_cn.host_connection, NULL);
            ncurses_color_on();
            printw("[VCPUS]: %d\n", hst_cn.v_cpus);
            print_bars();
    }
    else {
        printw("Error resolving MaxVcpus");
        ncurses_continue();
        return 0;
    }

    //query available memory
    if (virNodeGetFreeMemory(hst_cn.host_connection) != 0) {
            hst_cn.free_mem = virNodeGetFreeMemory(hst_cn.host_connection);
            printw("[MEMORY]: %lluMB\n", (hst_cn.free_mem/1024));
            print_bars();
            print_bars();
            ncurses_color_off();
            ncurses_continue();
    }
    else {
        printw("Error resolving FreeMemory");
        ncurses_continue();
        return 0;
    }

    //query detailed host info
    ncurses_color_on();
    print_bars();
    bold_on();
    printw("[DETAILED HOST STATS:]\n");
    bold_off();
    print_bars();

    if (virNodeGetInfo(hst_cn.host_connection, &hst_cn.host_node_info) != -1) {
            virNodeGetInfo(hst_cn.host_connection, &hst_cn.host_node_info); //&pointer required
            printw("[CPU Model:] %s\n", hst_cn.host_node_info.model);
            printw("[MEMORY SIZE:] %lukb\n", hst_cn.host_node_info.memory);
            printw("[Active CPUs:] %u\n", hst_cn.host_node_info.cpus);
            printw("[CPU Frequency:] %uMhz\n", hst_cn.host_node_info.mhz);
            printw("[CPU Sockets per Node:] %u\n", hst_cn.host_node_info.sockets);
            printw("[NUMA Nodes ([*1] if Uniform):] %u\n", hst_cn.host_node_info.nodes);
            printw("[CPU Cores/Socket:] %u\n", hst_cn.host_node_info.cores);
            printw("[CPU Threads/Core:] %u\n", hst_cn.host_node_info.threads);
            print_bars();
    }
    else {
        printw("Error populating virNodeInfo struct");
        ncurses_continue();
        return 0;
    }

    bold_on();
    printw("[SECURITY DETAILS:]\n");
    bold_off();
    print_bars();
    ncurses_color_off();
    ncurses_continue();

    //get security details
    if (virConnectIsEncrypted(hst_cn.host_connection) != -1) {
            hst_cn.encryp_status = virConnectIsEncrypted(hst_cn.host_connection);
            virNodeGetSecurityModel(hst_cn.host_connection, &hst_cn.sec_model);
            ncurses_color_on();
            printw("[Secure Connection Status ([*1] Encrypted; [*0] Plaintext):] %u\n", hst_cn.encryp_status);
            printw("[Security Model:] %s\n", hst_cn.sec_model.model);
            printw("[Security DOI:] %s\n", hst_cn.sec_model.doi);
            print_bars();
            ncurses_color_off();
            ncurses_continue();
    }
    else {
        printw("Error resolving Security Status");
        ncurses_continue();
        return 0;
    }


    //-------------VM (DOMAIN) STATS-----------------------------------------------------------------------------------//
    //-----------------------------------------------------------------------------------------------------------------//
    ncurses_color_on();
    print_bars();
    bold_on();
    printw("[VIRTUAL MACHINE STATS:]\n");
    bold_off();
    print_bars();
    //------------Initialize Domain Objects---------------------------------------------------------------------------//
    domains.total_domains = 0;
    if (virConnectNumOfDomains(hst_cn.host_connection) != -1 && virConnectNumOfDomains(hst_cn.host_connection) != 0) {
            domains.num_active = virConnectNumOfDomains(hst_cn.host_connection);
    }
    else {
        printw("Error retrieving domains OR No Active Domains\n");
        ncurses_continue();
        endwin();
        return 0;
    }

    if (virConnectNumOfDefinedDomains(hst_cn.host_connection) != -1) {
            domains.num_non_active = virConnectNumOfDefinedDomains(hst_cn.host_connection);
    }
    else {
        printw("Error retrieving defined domains");
        ncurses_continue();
        return 0;
    }
    //allocate dynamic storage for domain listings
    domains.all_domains = malloc(sizeof(virDomainPtr) * (domains.num_active + domains.num_non_active));
    domains.non_active_domains = malloc(sizeof(char *) * domains.num_non_active);
    domains.active_domains = malloc(sizeof(int) * domains.num_active);

    if (virConnectListDomains(hst_cn.host_connection, domains.active_domains, domains.num_active) != -1) {
            domains.num_active = virConnectListDomains(hst_cn.host_connection,
                    domains.active_domains, domains.num_active);
            domains.num_non_active = virConnectListDefinedDomains(hst_cn.host_connection,
                    domains.non_active_domains, domains.num_non_active);
    }
    else {
        printw("error listing domains");
        ncurses_continue();
        return 0;
    }
    //----------------print active domains------------------------------------------------------------------//
    bold_on();
    printw("[Active VMs:]\n");
    bold_off();
    //iterate through active domains

    //TODO create a loop to refresh vm stats func call
    for (int i = 0; i < 5; i++) {
        clear();
        get_vm_stats();
        refresh();
        usleep(SCREEN_DELAY);
        ncurses_continue();
    }
    //free malloc resources
    free(domains.active_domains);
    free(domains.non_active_domains);
    ncurses_color_off();
    ncurses_continue();


    ncurses_color_on();
    bold_on();
    print_bars();
    printw("===[Closing Host KVM/Qemu connection]===\n");

    if (virConnectClose(hst_cn.host_connection) != -1) {
            virConnectClose(hst_cn.host_connection);
            print_bars();
            bold_off();
            ncurses_color_off();
            ncurses_continue();
            endwin();
            return 1;
    }
    else {
        printw("error closing host conn");
        ncurses_continue();
        return 0;
    }
}





