# Crawler: a c++ approach

### I. Devlopment platform: Ubuntu 18.04

### II. API installation

***X DevAPI***
* install mysql version 8: follow the instruction in [this link](https://www.sqlshack.com/how-to-install-mysql-on-ubuntu-18-04/): look at the part that teach installing mysql8)

***libcurl***
```
sudo apt-get install -y libcurl-dev
```

***tidy***
* choos e proper binary installer from [here](http://binaries.html-tidy.org/)

***c++ boost***
```
sudo apt-get install libboost-all-dev
```

### III. Build
under the project ***CrawlerNew*** directory
```
cd cpp
make
cd ../
./crawler
```

### IV. things to note
* Before you run the crawler, you have to place ***url of the website*** that you want to crawler under **seed.log**
