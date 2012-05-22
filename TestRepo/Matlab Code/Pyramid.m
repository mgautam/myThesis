close all
clear all
clc

in_Image = imread('C:\Users\Gautam\Desktop\Project\00.Test_Images\lena.bmp');
input = double(rgb2gray(in_Image));
insize = size(input);

start = -3;
k = start:-start;
std = 1.4142;
h = exp(-(k.^2)/(2*std^2))/(std*sqrt(2*pi));
h = h/sum(h);
%plot(h);

interim = zeros(insize);
for n1=1:insize(1)
    for n2=1:insize(2)
        for k = -3:3
            if((n2-k)>0 && (n2-k)<=insize(2))
                interim((n1-1)*insize(2)+n2) = interim((n1-1)*insize(2)+n2) + h(k-start+1)*input((n1-1)*insize(2)+(n2-k));
            end;
        end
    end
end

final = zeros(insize);
for n1=1:insize(1)
    for n2=1:insize(2)
        for k = -3:3
            if((n1-k)>0 && (n1-k)<=insize(1))
                final((n1-1)*insize(2)+n2) = final((n1-1)*insize(2)+n2) + h(k-start+1)*interim(((n1-1)-k)*insize(2)+n2);
            end;
        end
    end
end

inputFFT = fft2(input);
hFFT = fft2(h'*h,512,512);
finalFFT = inputFFT .* hFFT;
finale = ifft2(finalFFT);

imshow(uint8(final));