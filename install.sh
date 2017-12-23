#!/bin/sh
LB=`tput setaf 4`
BG=`tput setab 7`
rst=`tput sgr0`
echo "${LB}${BG}[Easy-Install Libvirt/virt-manager]${rst}"
echo "===================================="
sleep 3
echo

echo "${LB}${BG}[Updating System]${rst}"
echo "==============="
sleep 3
sudo apt-get update && sudo apt-get upgrade && sudo apt-get dist-upgrade
echo

echo "${LB}${BG}[Installing Virt-Manager]${rst}"
echo "======================="
sleep 3
sudo apt-get install virt-manager
echo

echo "${LB}${BG}[Installing qemu-kvm and libvirt]${rst}"
echo "================================="
sleep 3
sudo apt install qemu-kvm libvirt-bin
echo

echo "${LB}${BG}[Installing libvirt-dev]${rst}"
sleep 3
sudo apt-get install libvirt-dev
echo

echo "${LB}${BG}[Checking KVM Support]${rst}"
echo "===================="
kvm-ok
sleep 3
echo

echo "${LB}${BG}[Installing uvtool]${rst}"
echo "==================="
sudo apt -y install uvtool
echo
sleep 3

echo "${LB}${BG}[Downloading cloud image]${rst}"
echo "========================"
echo "${LB}{RELEASE=XENIAL (UBUNTU 16.04LTS)}${rst}"
sleep 2
sudo uvt-simplestreams-libvirt sync release=xenial arch=amd64
echo
echo "${LB}${BG}[[Cloud Image Installed]]${rst}"
echo "=========================="
echo "Run: 'uvt-kvm create <vm name> ' to create/start .qcow"
echo
