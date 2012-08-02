function H = gauss2d (sigma,center)
n = [-3:3];
h = exp (-(n-center).^2 / sigma^2)/(sigma*sqrt(2*pi));
H = n'*n;