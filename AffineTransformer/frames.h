#ifndef FRAMES_H
#define FRAMES_H

#include <stdio.h>

void createFrames (char *fileName, char *FramesFolder, int numFrames, FILE *logFile = stdout, bool color_frames = true, char *backgroundFile = 0);

#endif