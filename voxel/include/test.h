#ifndef TESTS_H_
#define TESTS_H_

#include <iostream>
#include "image.h"
#include "file.h"
#include "wavefront.h"
#include "util.h"
#include "algorithm2d.h"
#include "algorithm3d.h"
#include "voxel.h"


void testParameterParser1(int argc, char** argv);
void testTargaImageWriter1();
void testWavefrontOBJReader1(std::string objFilePath);
void testAlgorithmClassesOperators1();
void testMathematicianRayTriangleIntersection();
void testMathematicianTriangleBoxOverlap();
void testAlg2DQuad();
void testAlg2DQuad2();
void testFaceGetShapeMethods(std::string objFilePath);
void testQuadTree(std::string objFilePath);
void testGridVoxelization(std::string objFilePath);

#endif
