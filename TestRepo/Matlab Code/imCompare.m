x = (imread('C:\Users\Gautam\Desktop\Project\01.Training\02.Laplacian_Pyramid\Image_Lapla(10).bmp'));
y = imread('C:\Users\Gautam\Desktop\Project\01.Training\02.Laplacian_Pyramid\Image_Lapla(11).bmp');
z = x-y;
sqrt(sum(sum(sum(z.^2))))