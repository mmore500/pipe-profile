#!/bin/sh

cd metis
make config shared=1 cc=gcc
make install

