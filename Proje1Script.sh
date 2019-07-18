#!/usr/bin/octave -qf

argumentList=argv();
inputFileName=argumentList{1};

inputFileNameLength=length(inputFileName);
inputFileNameWithoutExtension=substr(inputFileName,1,inputFileNameLength-4);
outputFileName=strcat(inputFileNameWithoutExtension,'.jpg');

filteredGrayImage=uint8(importdata(inputFileName));
imwrite(filteredGrayImage,outputFileName);
