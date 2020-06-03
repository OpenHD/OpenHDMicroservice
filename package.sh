#!/bin/bash

PACKAGE_NAME=openhd-microservice

TMPDIR=/tmp/${PACKAGE_NAME}-installdir

rm -rf ${TMPDIR}/*

mkdir -p ${TMPDIR} || exit 1

make clean || exit 1

make install DESTDIR=${TMPDIR} || exit 1

VERSION=$(git describe)

fpm -s dir -t deb -n ${PACKAGE_NAME} -v ${VERSION//v} -C ${TMPDIR} \
  -p ${PACKAGE_NAME}_VERSION_ARCH.deb \
  -d "libasio-dev >= 1.10" \
  -d "libboost-system-dev >= 1.62.0" \
  -d "libboost-program-options-dev >= 1.62.0" \
  -d "openhd-router >= 0.1.3" || exit 1
