#!/bin/sh

# install c++ boost
sudo apt-get install libboost-all-dev
# install libcurl
sudo apt-get install  libcurl4-openssl-dev
# install mysql8
wget -c https://dev.mysql.com/get/mysql-apt-config_0.8.15-1_all.deb
sudo dpkg -i mysql-apt-config_0.8.15-1_all.deb
sudo apt update
sudo apt install mysql-server
# install ltidy
wget -c https://github.com/htacg/tidy-html5/releases/download/5.4.0/tidy-5.4.0-64bit.deb
sudo dpkg -i tidy-5.4.0-64bit.deb
