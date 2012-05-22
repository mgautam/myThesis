CC = g++
CFLAGS = -w -I../ -c

all: Rotation clean

Rotation: AffineTransformer.o bmpAccess.o DSP.o CriticalPoints.o Matrix.o ModelAffine.o MotionExtract.o Features.o API.o main.o
	$(CC) *.o -o ./Rotation

AffineTransformer.o: 
	$(CC) $(CFLAGS) ../AffineTransformer/*.cpp

bmpAccess.o: 
	$(CC) $(CFLAGS) ../bmpAccess/*.cpp

DSP.o: 
	$(CC) $(CFLAGS) ../DSP/*.cpp

CriticalPoints.o: 
	$(CC) $(CFLAGS) ../CriticalPoints/*.cpp

Matrix.o: 
	$(CC) $(CFLAGS) ../Matrix/*.cpp

ModelAffine.o: 
	$(CC) $(CFLAGS) ../ModelAffine/*.cpp

MotionExtract.o: 
	$(CC) $(CFLAGS) ../MotionExtract/*.cpp

Features.o: 
	$(CC) $(CFLAGS) ../Features/*.cpp

API.o:
	$(CC) $(CFLAGS) ../API/*.cpp

main.o:
	$(CC) $(CFLAGS) ../main.cpp

clean:
	rm -rf *.o
