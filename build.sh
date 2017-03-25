#!/bin/bash
compiler=clang++

if [ ! -d build ]; then
	mkdir build
fi

pushd build

cmake -DCMAKE_CXX_COMPILER=${compiler} ..
make -j ${MAKE_JOB_NUMBER}
build_result=$?

popd

if [ ${build_result} -eq 0 ]; then
  if [ -d bin ]; then
    rm -rf bin
  fi

  mkdir bin
  cp ./build/ren_viewer bin
fi


