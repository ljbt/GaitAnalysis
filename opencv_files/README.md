This project is our Master 1 technical project.
Its purpose is to analyse gait of patients. For this there is a set of videos of persons wearing a special suit, that allows articulation detection.
Thanks to image processing, computer vision and machine learning, the program is able to extract biometric data and expose its (simple) pathological gait diagnosis.

The program requires images from videos, so you have to execute the script folder_and_images.sh:
  $ cd ./learning_images/
  $ chmod +x folder_and_images.sh
  $ ./folder_and_images.sh


Once each video folder contains its images, you can launch the main program that detect articulations, computes some biometric data, and deduce if there is a pathology or not.

For that, the program has to detect the color stickers, thanks to HSV masks.
Then, it has to know if the red sticker is the knee or the foot, for example. So, it usess neural networks that learned to classify each sticker position.
Indeed, another program, saveCenters.cpp, asks the user to tell him wich articulation is each red and blue stickers, and save these in text files.
These text files have to be used by another program from NeuralNet directory, that creates, do the learning, and save the neural network parameters in paramsNNRed.txt and paramsNNBlue.txt.
