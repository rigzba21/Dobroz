/*compile w/ gcc -g -Wall host_connection.c -o ex -lvirt */

#include <stdio.h>
#include <stdlib.h>
#include <libvirt/libvirt.h>

struct hst_cn {
    virConnectPtr host_connection;
} hst_cn;


int main(int argc, char *argv[]) {

    //connect to host
    hst_cn.host_connection = virConnectOpen("qemu:///system");
    if (hst_cn.host_connection == NULL) {
        fprintf(stderr, "Host connection KVM/Qemu failed!!!\n");
        return 0;
    }
    else {
        printf("Host Connection: [KVM/Qemu] established\n");

    }
    //query vm stats here
    //
    //
    //
    printf("Closing Host KVM/Qemu connection\n");
    virConnectClose(hst_cn.host_connection);
    return 1;
}





