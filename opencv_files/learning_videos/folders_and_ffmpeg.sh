#!/bin/bash

# script that makes folders from video names,
# put the video in its folder,
# and make images from video

for name in *;
do
	if [[ $name == *.mp4 ]];
	then
		length=${#name} 
		length_wo_extension=$(($length-4))
		foldername=${name:0:$((length_wo_extension))}

		if [ -d $foldername ];
		then
			echo "folder $foldername and video exist so moove video into folder"
		      	 mv $name $foldername
		else
			echo "folder $foldername doesnt exist, so creation and moove video into folder"
			mkdir $foldername
			mv $name $foldername
		fi
	
	elif [ -d ${name} ];
		then
			echo "$name is a folder and contain its video"
			cd $name
			shopt -s nullglob
			numfiles=(*)
			numfiles=${#numfiles[@]} # count number of files in folder
			echo "$name contains $numfiles files"
			if [[ $num -gt 1 ]]; # if greater than 1
			then
				echo "more than 1 file in folder, so video converted to images is likely"
			elif [ $numfiles -eq 1 ]
			then
				filename=(*)
				if [[ $filename == *.mp4 ]];
				then
					echo "video $filename not converted"
					#extension="${filename##*.}"	
					filename_wo_extension="${filename%.*}"
					#echo "video $filename_wo_extension wo extension $extension"
					ffmpeg -i $filename -vf scale=500:-1 -r 8 %02d.bmp

				fi
			fi
			cd ..
	fi
done
