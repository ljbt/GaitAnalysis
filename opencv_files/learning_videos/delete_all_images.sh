#!/bin/bash

# script that deletes all .bmp images of every folder

for name in *;
do
	if [ -d ${name} ];
	then
		cd $name
		rm *.bmp
		cd ..
	fi
done
