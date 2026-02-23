#!/bin/sh

cmake ex1 -B build/ex1 && make -C build/ex1 || exit 1

 build/ex1/to_json 
 build/ex1/from_json 
 build/ex1/from_to_json 

