close all
clear all
clc

actualRotation = [0:99]*2*pi/100;
index = 1;
for threshold = 50:10:940
    fileName = sprintf ('..\\..\\TestRepo\\02.Test\\Rotation\\Threshold(%d).bin',threshold);
    rotationFile = fopen (fileName,'r');
    observedRotation(index,:) = fread (rotationFile, 100, 'double');
    fclose (rotationFile);
    index = index + 1;
end;

for i=1:index-1
     value (i) = [ (sum((observedRotation(i,:) - actualRotation).^2))];
end;
i = 1:index-1;
plot (51+10*(i-1), value(i));