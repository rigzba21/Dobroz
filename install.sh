#!/bin/sh

echo "[Updating System]"
echo "==============="
sleep 1
sudo apt-get update && sudo apt-get upgrade && sudo apt-get dist-upgrade
echo

echo "[Installing Virt-Manager]"
echo "======================="
sleep 1
sudo apt-get install virt-manager
echo

echo "[Installing qemu-kvm and libvirt]"
echo "================================="
sleep 2
sudo apt install qemu-kvm libvirt-bin
echo

echo "[Checking KVM Support]"
echo "===================="
kvm-ok
sleep 3
echo

echo "[Installing uvtool]"
echo "==================="
sudo apt -y install uvtool
echo
sleep 2

echo "[Downloading cloud image]"
echo "========================"
echo "{RELEASE=XENIAL (UBUNTU 16.04LTS)}"
sleep 2
sudo uvt-simplestreams-libvirt sync release=xenial arch=amd64
echo
