close all
clear all
clc

lena = single(rgb2gray(imread('C:\Users\Gautam\Desktop\Project\00.Test_Images\lena.bmp')));
figure (1);
imshow(uint8(lena));

%We compute the SIFT frames (keypoints) and descriptors by
[F,D] = vl_sift(lena) ;
%[f,d] = vl_sift(lena,'frames',F(:,1));
% The matrix f has a column for each frame. 
% A frame is a disk of center f(1:2), scale f(3) and orientation f(4) . 
% We visualize a random selection of 50 features by:

perm = randperm(size(F,2)) ; 
sel = perm(1:50) ;

h1 = vl_plotframe(F(:,sel)) ; 
h2 = vl_plotframe(F(:,sel)) ; 
set(h1,'color','k','linewidth',3) ;
set(h2,'color','y','linewidth',2) ;

%We can also overlay the descriptors by
h3 = vl_plotsiftdescriptor(D(:,sel),F(:,sel)) ;  
set(h3,'color','g') ;

