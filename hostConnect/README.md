DOBROZ
=========
~~A command-line utility for managing local virtual machines, viewing statistics, and resource handling.~~

A Go/Buffalo application for monitoring and managing virtual CPU/Memory performance. 

Local Environment Installation (old version) 
------------------------------
Run the *install.sh* script to locally install **libvirt**, **uvtool**, **qemu-kvm**, **libvirt-dev**, **virt-manager** as well as the **Ubuntu 16.04LTS cloud image**.
1. ```git clone https://github.com/rigzba21/Dobroz.git```
2. ```cd Dobroz```
3. ```./install.sh```
4. ```uvt-kvm create <vm_name>```
5. ```make```
6. ```make clean```
7. ```./host_connect```

**May require a restart**

TO-DO:
--------
- [ ] Change language/framework to Go/Buffalo 
- ~~[ ] Remove ncurses, and switch to a GUI or update to ncurses 6~~
- [ ] add XML domain configuration
- ~~[ ] gui (QT or JavaFX;ncurses keybindings not consistent)~~
- [x] refresh display stats
- [x] added Cargo.toml for virt crate (Rust bindings for libvirt)
- ~~[ ] fix makefile to include cargo changes?~~
