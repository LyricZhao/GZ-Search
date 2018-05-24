#!/usr/bin/env bash
g++ check.cpp -o check -O2 -std=c++11 -I /usr/local/Cellar/berkeley-db/6.2.32/include -L/usr/local/Cellar/berkeley-db/6.2.32/lib -ldb_cxx