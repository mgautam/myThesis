#include <stdlib.h>

#include <ModelAffine/testAffineFit.h>
#include <ModelAffine/fitAffineMatrix.h>

#define NUMBER_OF_PIXELS 100

void testAffineModel (void) {
	COORDS initial,final;

	initial.Number_of_Coordinates = NUMBER_OF_PIXELS;
	initial.x = new double[initial.Number_of_Coordinates];
	initial.y = new double[initial.Number_of_Coordinates];	

	final.Number_of_Coordinates = NUMBER_OF_PIXELS;
	final.x = new double[final.Number_of_Coordinates];
	final.y = new double[final.Number_of_Coordinates];
	
	cout << "Test Vectors:" << endl;
	for (int index = 0; index < NUMBER_OF_PIXELS; index++) {
		initial.x[index] = index;// + 256;
		initial.y[index] = rand();// + 256;
	
		cout << "(" << initial.x[index] << "," << initial.y[index] << ")" ;

		final.x[index] = 1*initial.x[index] + 0*initial.y[index] + 2;// + 256;//6,7
		final.y[index] = 0*initial.x[index] + 1*initial.y[index] + 3;// + 256;//1,13

		cout << " => (" << final.x[index] << "," << final.y[index] << ")" << endl;

	}

	cout << endl << "Estimated AffineMatrix:" << endl;
	cout << fitAffineMatrix ( initial, final, 0, 0 ) << endl;
}
