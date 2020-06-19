#!/bin/sh

# install c++ boost
sudo apt-get install libboost-all-dev
# install libcurl
sudo apt-get install -y libcurl-dev
# install mysql8
wget -c https://dev.mysql.com/get/mysql-apt-config_0.8.15-1_all.deb
sudo dpkg -i mysql-apt-config_0.8.15-1_all.deb
sudo apt update
sudo apt install MySQL-server
