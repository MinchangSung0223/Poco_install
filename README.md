# Poco_install
```bash
sudo apt-get install openssl libssl-dev
sudo apt-get install libiodbc2 libiodbc2-dev
wget http://pocoproject.org/releases/poco-1.7.4/poco-1.7.4.tar.gz
./configure --omit=Data/ODBC,Data/MySQL
make -j16
make install
```
