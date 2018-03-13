DOBROZ
=========
A command-line utility for managing local virtual machines, viewing statistics, and resource handling.

Local Environment Installation
------------------------------
Run the *install.sh* script to locally install **libvirt**, **uvtool**, **qemu-kvm**, **libvirt-dev**, **virt-manager** as well as the **Ubuntu 16.04LTS cloud image**.
1. ```git clone https://github.com/rigzba21/Dobroz.git```
2. ```cd Dobroz```
3. ```./install.sh```
4. ```uvt-kvm create <vm_name>```
5. ```make```
6. ```make clean```
7. ```./host_connect```

TO-DO:
--------
- [ ] add XML domain configuration
- [ ] ncurses menu options
- [ ] ncurses border/window options
- [ ] refresh display stats
- [x] added Cargo.toml for virt crate (Rust bindings for libvirt)
