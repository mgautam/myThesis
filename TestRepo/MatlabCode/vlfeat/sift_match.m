clear all
close all
clc

%SIFT descriptors are often used find similar regions in two images.
% vl_ubcmatch implements a basic matching algorithm.
% Let Ia and Ib be images of the same object or scene. We extract and match the descriptors by:
Ia = single(rgb2gray(imread('C:\Users\Gautam\Desktop\Project\00.Test_Images\lena.bmp')));
Ib = single((imread('C:\Users\Gautam\Desktop\Project\00.Test_Images\lena.bmp(25).bmp')));
%single(imfilter(single((imread('C:\Users\Gautam\Desktop\Project\00.Test_Images\Solidball.bmp(50).bmp'))),gauss2d(1,0),'conv'));

[fa, da] = vl_sift (Ib) ;
[fb, db] = vl_sift (Ia) ;

%For each descriptor in da, vl_ubcmatch finds the closest descriptor in db (as measured by the L2 norm of the difference between them).
% The index of the original match and the closest descriptor is stored in each column of matches and the distance between the pair is stored in scores.
[matches, scores] = vl_ubcmatch(da, db) ; 
%Matches also can be filtered for uniqueness by passing a third parameter to vl_ubcmatch which specifies a threshold.
% Here, the uniqueness of a pair is measured as the ratio of the distance between the best matching keypoint and the distance to the second best one (see vl_ubcmatch for further details).


figure (1)
imshow (uint8 (Ia));
% We visualize a random selection of 50 or less features by:
perm = randperm(size(fa,2)) ; 
sel = perm(1:min(50,length(perm))) ;

h1 = vl_plotframe(fa(:,sel)) ; 
h2 = vl_plotframe(fa(:,sel)) ; 
set(h1,'color','k','linewidth',3) ;
set(h2,'color','y','linewidth',2) ;


figure (2);
imshow (uint8 (Ib));
% We visualize a random selection of 50 or less features by:
perm = randperm(size(fb,2)) ; 
sel = perm(1:min(50,length(perm))) ;

h1 = vl_plotframe(fb(:,sel)) ; 
h2 = vl_plotframe(fb(:,sel)) ; 
set(h1,'color','k','linewidth',3) ;
set(h2,'color','y','linewidth',2) ;

% Writing x,y positions to binary file
initCoords = fa(1:2,matches(1,:));
finalCoords = fb(1:2,matches(2,:));

matchFile = fopen ('C:\Users\Gautam\Desktop\Project\Matlab Code\siftmatches.bin','w');
fwrite (matchFile,length(scores),'uint32');
fwrite (matchFile,initCoords(1,:),'double');
fwrite (matchFile,initCoords(2,:),'double');
fwrite (matchFile,finalCoords(1,:),'double');
fwrite (matchFile,finalCoords(2,:),'double');
fclose (matchFile);

matchFile = fopen ('C:\Users\Gautam\Desktop\Project\Matlab Code\siftmatches.bin','r');
x = fread (matchFile,1,'uint32')
fclose (matchFile);
