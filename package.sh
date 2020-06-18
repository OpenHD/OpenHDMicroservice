#!/bin/bash

PLATFORM=$1
DISTRO=$2


if [[ "${PLATFORM}" == "pi" ]]; then
    OS="raspbian"
    ARCH="arm"
    PACKAGE_ARCH="armhf"
fi


PACKAGE_NAME=openhd-microservice

TMPDIR=/tmp/${PACKAGE_NAME}-installdir

rm -rf ${TMPDIR}/*

mkdir -p ${TMPDIR} || exit 1

make clean || exit 1

make install DESTDIR=${TMPDIR} || exit 1

VERSION=$(git describe)

rm ${PACKAGE_NAME}_${VERSION//v}_${PACKAGE_ARCH}.deb > /dev/null 2>&1

fpm -a ${PACKAGE_ARCH} -s dir -t deb -n ${PACKAGE_NAME} -v ${VERSION//v} -C ${TMPDIR} \
  -p ${PACKAGE_NAME}_VERSION_ARCH.deb \
  -d "libasio-dev >= 1.10" \
  -d "libboost-system-dev >= 1.62.0" \
  -d "libboost-program-options-dev >= 1.62.0" \
  -d "openhd-router >= 0.1.3" || exit 1

#
# Only push to cloudsmith for tags. If you don't want something to be pushed to the repo, 
# don't create a tag. You can build packages and test them locally without tagging.
#
git describe --exact-match HEAD > /dev/null 2>&1
if [[ $? -eq 0 ]]; then
    echo "Pushing package to OpenHD repository"
    cloudsmith push deb openhd/openhd/${OS}/${DISTRO} ${PACKAGE_NAME}_${VERSION//v}_${PACKAGE_ARCH}.deb
else
    echo "Not a tagged release, skipping push to OpenHD repository"
fi
