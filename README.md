# Crawler: a c++ approach

### Devlopment platform: Ubuntu 18.04

### API installation

***X DevAPI***
 * install mysql version 8: follow the instruction in [this link](https://www.sqlshack.com/how-to-install-mysql-on-ubuntu-18-04/): look at the part that teach installing mysql8)
     * remember to set username: root
     * password: cht123456

***C++ connector***
 1.  go to [c++ connector page](https://dev.mysql.com/downloads/connector/cpp/) install c++ connector
 2.  `sudo cp libmysqlcppconn8.so.2 /usr/lib`

***libcurl***
```
sudo apt-get install -y libcurl-dev
```

***tidy***
choos e proper binary installer from [here](http://binaries.html-tidy.org/)

***c++ boost***
```
sudo apt-get install libboost-all-dev
```

or you can follow the simplified version

***simplified version***
1. bash ./install.sh
2. install C++ connector manually
     1.  go to [c++ connector page](https://dev.mysql.com/downloads/connector/cpp/) install c++ connector
     2. sudo cp libmysqlcppconn8.so.2 /usr/lib


#### Build
under the project ***CrawlerNew*** directory
```
cd cpp
make
cd ../
./crawler
```

### things to note
* Before you run the crawler, you have to place ***url of the website*** that you want to crawler under **seed.log**
