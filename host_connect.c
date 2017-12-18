/*compile w/ gcc -g -Wall host_connection.c -o ex -lvirt */

#include <stdio.h>
#include <stdlib.h>
#include <libvirt/libvirt.h>

int main(int argc, char *argv[]) {

    //host connection pointer
    virConnectPtr host_connection;

    host_connection = virConnectOpen("qemu:///system");
    if (host_connection == NULL) {
        fprintf(stderr, "Host Connection KVM/Qemu failed\n");
        return 1;
        }
    else {
        printf("Host connection [KVM/QEMU] established\n");
    }

    //close connection
    virConnectClose(host_connection);
    host_connection = NULL;
    return 0;
}


