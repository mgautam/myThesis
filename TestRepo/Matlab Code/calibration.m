clear all
close all
clc
%angles = fopen ('../02.Test/estimated_rotation.bin','rb');
angles = fopen ('../all.bin','rb');
X = fread (angles, inf, 'float64');
fclose (angles);

Y = 2*pi*[0:length(X)-1]'/100;

e = Y - X;

MSE = sqrt (mean (e.*e));

A = [abs(e).*X abs(e)];

T = inv(A'*A)*A'*(abs(e).*Y);

e_new = Y-A*T;

MSE_new = sqrt (mean (e_new.*e_new));