close all
clear all
clc

v = [1,0,0];
x = [v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v];
plot(x);
n = -10:10;
a = 3;
s = sinc(n/a)/a;
y = filter(s,1,x);
figure;plot(y);
y1 = zeros(1,72);
for n = 1:72
    for m = 1:72
        y1(m) = y1(m)+x(n)*sinc(m/a-n);
    end;
end;
figure;plot(y1);