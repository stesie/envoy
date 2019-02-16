#!/bin/bash

set -e

# Setup basic requirements and install them.
apt-get update
export DEBIAN_FRONTEND=noninteractive
apt-get install -y --no-install-recommends wget software-properties-common make cmake git python python-pip python3 python3-pip \
  unzip bc libtool ninja-build automake zip time golang gdb strace wireshark tshark tcpdump

ln -sf /usr/bin/python3.5 /usr/bin/python3

# gcc-7
apt-add-repository -y "deb http://ppa.launchpad.net/ubuntu-toolchain-r/test/ubuntu xenial main"
apt-key adv --keyserver keyserver.ubuntu.com --recv-keys BA9EF27F
apt-get update -y
apt-get install -y g++-7
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 1000
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 1000
update-alternatives --config gcc
update-alternatives --config g++
rm -rf /var/lib/apt/lists/*

# Setup tcpdump for non-root.
groupadd pcap
chgrp pcap /usr/sbin/tcpdump
chmod 750 /usr/sbin/tcpdump
setcap cap_net_raw,cap_net_admin=eip /usr/sbin/tcpdump

# virtualenv
pip3 install virtualenv
