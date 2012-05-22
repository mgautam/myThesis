clear all
close all
clc

run 'C:\Users\Gautam\Desktop\vlfeat\toolbox\vl_setup.m';

%SIFT descriptors are often used find similar regions in two images.
% vl_ubcmatch implements a basic matching algorithm.
% Let Ia and Ib be images of the same object or scene. We extract and match the descriptors by:
Ia = single(rgb2gray(imread('C:\Users\Gautam\Desktop\Project\00.Test_Images\lena.bmp')));
[fa, da] = vl_sift (Ia) ;
siz=size(da);

featureFile = fopen ('C:\Users\Gautam\Desktop\Project\01.Training\07.SIFT_Keys\siftfeature.bin','w');
fwrite (featureFile,siz,'uint32');

for i = 1:siz(2)
    fwrite (featureFile,fa(1:2,i)','double');
    fwrite (featureFile,da(:,i)','uint8');   
    
end;
fclose (featureFile);
 

for i = 0:99
    
    filename = sprintf('C:\\Users\\Gautam\\Desktop\\Project\\00.Test_Images\\lena.bmp(%d).bmp',i);
    Ib = single(imread(filename));
    [fb, db] = vl_sift (Ib) ;
    siz=size(db);
    
    filename = sprintf('C:\\Users\\Gautam\\Desktop\\Project\\02.Test\\07.SIFT_Keys\\siftfeature(%d).bin',i);
    featureFile = fopen (filename,'w');
    fwrite (featureFile,siz,'uint32');

    
    for j = 1:siz(2)
        fwrite (featureFile,fb(1:2,j)','double');
        fwrite (featureFile,db(:,j)','uint8');   

    end;
    
    sprintf( '%d : %d,%d',i , fb(1,i+1), fb(2,i+1))
    fclose (featureFile);
end;


featureFile = fopen ('C:\Users\Gautam\Desktop\Project\01.Training\07.SIFT_Keys\siftfeature.bin','r');
fread (featureFile,2,'uint32')
fread (featureFile,2,'double')
fread (featureFile,128,'uint8')


fread (featureFile,2,'double')
fread (featureFile,128,'uint8')
fclose (featureFile);