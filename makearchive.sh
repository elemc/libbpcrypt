#!/bin/sh

PROJECT_NAME=libbpcrypt
VERSION=0.1.1
PKGNAME=${PROJECT_NAME}-${VERSION}

pushd ..
cp -R ${PROJECT_NAME} ${PKGNAME}
rm -rf ${PKGNAME}/.git
tar cfJv ${PKGNAME}.tar.xz ${PKGNAME}
rm -rf ${PKGNAME}
popd
