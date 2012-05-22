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
		initial.x[index] = index;
		initial.y[index] = rand();
	
		cout << "(" << initial.x[index] << "," << initial.y[index] << ")" ;

		final.x[index] = 6*initial.x[index] + 7*initial.y[index] + 2;
		final.y[index] = 1*initial.x[index] + 13*initial.y[index] + 3;

		cout << " => (" << final.x[index] << "," << final.y[index] << ")" << endl;

	}

	cout << endl << "Estimated AffineMatrix:" << endl;
	cout << fitAffineMatrix ( initial, final ) << endl;
}
