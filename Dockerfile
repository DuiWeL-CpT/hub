FROM ubuntu:latest

RUN apt-get -y update
RUN apt-get -y install git cmake g++ libconfig++-dev mc libgtest-dev python valgrind vim expect telnet python-pip screen inetutils-ping
RUN apt-get -y install sqlite3 libsqlite3-dev gcovr

RUN pip install --upgrade pip
RUN pip install junit_xml

RUN git config --global user.name "Seba"
RUN git config --global user.email "sebszczec@wp.pl"

WORKDIR /usr/src/gtest
RUN cmake . && \
    make && \
    cp libgtest.a /usr/lib/libgtest.a && \
    cp libgtest_main.a /usr/lib/libgtest_main.a

WORKDIR /root
RUN git clone https://github.com/sebszczec/libsocket.git

WORKDIR /root/libsocket
RUN cmake . && \
    make && \
    make install

WORKDIR /root
