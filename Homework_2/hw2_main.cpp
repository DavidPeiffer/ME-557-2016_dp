//
//  main.cpp
//  OpenGL4Test
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//

// DAVID CODE
#include <algorithm>
// END DAVID CODE


// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"

// this line tells the compiler to use the namespace std.
// Each object, command without a namespace is assumed to be part of std. 
using namespace std;




static const string vs_string =
"#version 410 core                                                 \n"
"                                                                   \n"
"uniform mat4 projectionMatrix;                                    \n"
"uniform mat4 viewMatrix;                                           \n"
"uniform mat4 modelMatrix;                                          \n"
"in vec3 in_Position;                                               \n"
"                                                                   \n"
"in vec3 in_Color;                                                  \n"
"out vec3 pass_Color;                                               \n"
"                                                                  \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
"    pass_Color = in_Color;                                         \n"
"}                                                                 \n";

// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
static const string fs_string  =
"#version 410 core                                                 \n"
"                                                                  \n"
"in vec3 pass_Color;                                                 \n"
"out vec4 color;                                                    \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    color = vec4(pass_Color, 1.0);                               \n"
"}                                                                 \n";




/// Camera control matrices
glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix; // Store the view matrix
glm::mat4 modelMatrix; // Store the model matrix




// The handle to the window object
GLFWwindow*         window;


// Define some of the global variables we're using for this sample
GLuint program;







///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Fill this functions with your model code.

// USE THESE vertex array objects to define your objects
unsigned int vaoID[2];

unsigned int vboID[4];

/*!
 ADD YOUR CODE TO CREATE THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 */
unsigned int createTriangleStripModel(void)
{
    // use the vertex array object vaoID[0] for this model representation
    
	// DAVID CODE
	

	int numVertices;
	numVertices = 107;

	float* vertices = new float[numVertices * 3];  // Vertices for our square
	float *colors = new float[numVertices * 3]; // Colors for our vertices

												// Vertices
	vertices[0] = 0;  vertices[1] = 2;  vertices[2] = 0;   // (B) Front right corner [TriangleStrip1]
	vertices[3] = -1;  vertices[4] = 2;  vertices[5] = 0;   // (O) Back Right Bottom Corner [TriangleStrip1]
	vertices[6] = 0;  vertices[7] = 2;  vertices[8] = 1;   // (C) Front right corner up 1 [TriangleStrip1]
	vertices[9] = -1;  vertices[10] = 2;  vertices[11] = 1;   // (P) Back Right Middle Corner [TriangleStrip1]
	vertices[12] = 0;  vertices[13] = 0;  vertices[14] = 2;   // (D) Origin up 2 [TriangleStrip1]
	vertices[15] = -1;  vertices[16] = 0;  vertices[17] = 2;   // (N) Back Right Corner on Top Square Face [TriangleStrip1]
	vertices[18] = 0;  vertices[19] = -1;  vertices[20] = 2;   // (E) Odd shape top left corner [TriangleStrip1]
	vertices[21] = -1;  vertices[22] = -1;  vertices[23] = 2;   // (M) Back Left Top Corner [TriangleStrip1]
	vertices[24] = 0;  vertices[25] = -1;  vertices[26] = 1;   // (F) Odd shape back left corner [TriangleStrip1]
	vertices[27] = -1;  vertices[28] = -1;  vertices[29] = 0;   // (L) Back Left Bottom Corner [TriangleStrip1]
	vertices[30] = 2;  vertices[31] = -1;  vertices[32] = 1;   // (K) Front square top left [TriangleStrip1]
	vertices[33] = 2;  vertices[34] = -1;  vertices[35] = 0;   // (J) Front square bottom left [TriangleStrip1]
	vertices[36] = 2;  vertices[37] = 0;  vertices[38] = 1;   // (I) Front square top right [TriangleStrip1]
	vertices[39] = 2;  vertices[40] = 0;  vertices[41] = 0;   // (H) Front square bottom right [TriangleStrip1]
	vertices[42] = 0;  vertices[43] = 0;  vertices[44] = 1;   // (G) Origin up 1 [TriangleStrip1]
	vertices[45] = 0;  vertices[46] = 0;  vertices[47] = 0;   // (A) Origin [TriangleStrip1]
	vertices[48] = 0;  vertices[49] = 2;  vertices[50] = 1;   // (C) Front right corner up 1 [TriangleStrip1]
	vertices[51] = 0;  vertices[52] = 2;  vertices[53] = 0;   // (B) Front right corner [TriangleStrip1]
	vertices[54] = 2;  vertices[55] = -1;  vertices[56] = 1;   // (K) Front square top left [TriangleStrip2]
	vertices[57] = 2;  vertices[58] = 0;  vertices[59] = 1;   // (I) Front square top right [TriangleStrip2]
	vertices[60] = 0;  vertices[61] = -1;  vertices[62] = 1;   // (F) Odd shape back left corner [TriangleStrip2]
	vertices[63] = 0;  vertices[64] = 0;  vertices[65] = 1;   // (G) Origin up 1 [TriangleStrip2]
	vertices[66] = 0;  vertices[67] = -1;  vertices[68] = 2;   // (E) Odd shape top left corner [TriangleStrip2]
	vertices[69] = 0;  vertices[70] = 0;  vertices[71] = 2;   // (D) Origin up 2 [TriangleStrip2]
	vertices[72] = -1;  vertices[73] = -1;  vertices[74] = 2;   // (M) Back Left Top Corner [TriangleStrip3]
	vertices[75] = -1;  vertices[76] = 0;  vertices[77] = 2;   // (N) Back Right Corner on Top Square Face [TriangleStrip3]
	vertices[78] = -1;  vertices[79] = -1;  vertices[80] = 0;   // (L) Back Left Bottom Corner [TriangleStrip3]
	vertices[81] = -1;  vertices[82] = 2;  vertices[83] = 1;   // (P) Back Right Middle Corner [TriangleStrip3]
	vertices[84] = -1;  vertices[85] = 2;  vertices[86] = 0;   // (O) Back Right Bottom Corner [TriangleStrip3]
	vertices[87] = 0;  vertices[88] = 2;  vertices[89] = 0;   // (B) Front right corner [TriangleStrip4]
	vertices[90] = -1;  vertices[91] = 2;  vertices[92] = 0;   // (O) Back Right Bottom Corner [TriangleStrip4]
	vertices[93] = 0;  vertices[94] = 0;  vertices[95] = 0;   // (A) Origin [TriangleStrip4]
	vertices[96] = -1;  vertices[97] = -1;  vertices[98] = 0;   // (L) Back Left Bottom Corner [TriangleStrip4]
	vertices[99] = 2;  vertices[100] = 0;  vertices[101] = 0;   // (H) Front square bottom right [TriangleStrip4]
	vertices[102] = 2;  vertices[103] = -1;  vertices[104] = 0;   // (J) Front square bottom left [TriangleStrip4]
	vertices[105] = 0;  vertices[106] = 0;  vertices[107] = 2;   // (D) Origin up 2 [TriangleStrip5]
	vertices[108] = 0;  vertices[109] = 0;  vertices[110] = 1;   // (G) Origin up 1 [TriangleStrip5]
	vertices[111] = 0;  vertices[112] = 2;  vertices[113] = 1;   // (C) Front right corner up 1 [TriangleStrip5]

	colors[0] = 0;  colors[1] = 0;  colors[2] = 1;  //B [TriangleStrip1]
	colors[3] = 0;  colors[4] = 0;  colors[5] = 1;  //O [TriangleStrip1]
	colors[6] = 0;  colors[7] = 0;  colors[8] = 1;  //C [TriangleStrip1]
	colors[9] = 0;  colors[10] = 0;  colors[11] = 1;  //P [TriangleStrip1]
	colors[12] = 0;  colors[13] = 0;  colors[14] = 1;  //D [TriangleStrip1]
	colors[15] = 0;  colors[16] = 0;  colors[17] = 1;  //N [TriangleStrip1]
	colors[18] = 0;  colors[19] = 0;  colors[20] = 1;  //E [TriangleStrip1]
	colors[21] = 0;  colors[22] = 0;  colors[23] = 1;  //M [TriangleStrip1]
	colors[24] = 0;  colors[25] = 0;  colors[26] = 1;  //F [TriangleStrip1]
	colors[27] = 0;  colors[28] = 0;  colors[29] = 1;  //L [TriangleStrip1]
	colors[30] = 0;  colors[31] = 0;  colors[32] = 1;  //K [TriangleStrip1]
	colors[33] = 0;  colors[34] = 0;  colors[35] = 1;  //J [TriangleStrip1]
	colors[36] = 0;  colors[37] = 0;  colors[38] = 1;  //I [TriangleStrip1]
	colors[39] = 0;  colors[40] = 0;  colors[41] = 1;  //H [TriangleStrip1]
	colors[42] = 0;  colors[43] = 0;  colors[44] = 1;  //G [TriangleStrip1]
	colors[45] = 0;  colors[46] = 0;  colors[47] = 1;  //A [TriangleStrip1]
	colors[48] = 0;  colors[49] = 0;  colors[50] = 1;  //C [TriangleStrip1]
	colors[51] = 0;  colors[52] = 0;  colors[53] = 1;  //B [TriangleStrip1]
	colors[54] = 0;  colors[55] = 0;  colors[56] = 1;  //K [TriangleStrip2]
	colors[57] = 0;  colors[58] = 0;  colors[59] = 1;  //I [TriangleStrip2]
	colors[60] = 0;  colors[61] = 0;  colors[62] = 1;  //F [TriangleStrip2]
	colors[63] = 0;  colors[64] = 0;  colors[65] = 1;  //G [TriangleStrip2]
	colors[66] = 0;  colors[67] = 0;  colors[68] = 1;  //E [TriangleStrip2]
	colors[69] = 0;  colors[70] = 0;  colors[71] = 1;  //D [TriangleStrip2]
	colors[72] = 0;  colors[73] = 0;  colors[74] = 1;  //M [TriangleStrip3]
	colors[75] = 0;  colors[76] = 0;  colors[77] = 1;  //N [TriangleStrip3]
	colors[78] = 0;  colors[79] = 0;  colors[80] = 1;  //L [TriangleStrip3]
	colors[81] = 0;  colors[82] = 0;  colors[83] = 1;  //P [TriangleStrip3]
	colors[84] = 0;  colors[85] = 0;  colors[86] = 1;  //O [TriangleStrip3]
	colors[87] = 0;  colors[88] = 0;  colors[89] = 1;  //B [TriangleStrip4]
	colors[90] = 0;  colors[91] = 0;  colors[92] = 1;  //O [TriangleStrip4]
	colors[93] = 0;  colors[94] = 0;  colors[95] = 1;  //A [TriangleStrip4]
	colors[96] = 0;  colors[97] = 0;  colors[98] = 1;  //L [TriangleStrip4]
	colors[99] = 0;  colors[100] = 0;  colors[101] = 1;  //H [TriangleStrip4]
	colors[102] = 0;  colors[103] = 0;  colors[104] = 1;  //J [TriangleStrip4]
	colors[105] = 0;  colors[106] = 0;  colors[107] = 1;  //D [TriangleStrip5]
	colors[108] = 0;  colors[109] = 0;  colors[110] = 1;  //G [TriangleStrip5]
	colors[111] = 0;  colors[112] = 0;  colors[113] = 1;  //C [TriangleStrip5]







	// General setup for loading to graphics card

	glGenVertexArrays(2, vaoID); // Create our Vertex Array Object
	glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object so we can use it

	glGenBuffers(2, vboID);
	
	// Vertex buffer

	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, 3 * numVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	
	// Enables vertex array
	glEnableVertexAttribArray(0);

	// Colors

	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our Color Buffer Object
	glBufferData(GL_ARRAY_BUFFER, 3 * numVertices * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer

	// Enables color array
	glEnableVertexAttribArray(1);
	


	// END DAVID CODE



    //TODO:
    // vaoID[0];
    
    return 1;
}

/*!
 ADD YOUR CODE TO CREATE A MODEL USING PRIMITIVES OF YOUR CHOISE TO THIS FUNCTION
 */
unsigned int createPolygonModel(void)
{
    // use the vertex array object vaoID[1] for this model representation
  
	// DAVID CODE

	int numVertices;
	numVertices = 200;

	float* vertices = new float[numVertices * 3];  // Vertices for our square
	float *colors = new float[numVertices * 3]; // Colors for our vertices

	// Vertices
	vertices[0] = 0;  vertices[1] = 0;  vertices[2] = 0;   // (a) Origin [1]
	vertices[3] = 0;  vertices[4] = 2;  vertices[5] = 0;   // (b) Front right corner [1]
	vertices[6] = 0;  vertices[7] = 2;  vertices[8] = 1;   // (c) Front right corner up 1 [1]
	vertices[9] = 0;  vertices[10] = 2;  vertices[11] = 1;   // (c) Front right corner up 1 [2]
	vertices[12] = 0;  vertices[13] = 0;  vertices[14] = 0;   // (a) Origin [2]
	vertices[15] = 0;  vertices[16] = 0;  vertices[17] = 2;   // (d) Origin up 2 [2]
	vertices[18] = 0;  vertices[19] = 0;  vertices[20] = 2;   // (d) Origin up 2 [3]
	vertices[21] = 0;  vertices[22] = 0;  vertices[23] = 1;   // (g) Origin up 1 [3]
	vertices[24] = 0;  vertices[25] = -1;  vertices[26] = 1;   // (f) Odd shape back left corner [3]
	vertices[27] = 0;  vertices[28] = -1;  vertices[29] = 1;   // (f) Odd shape back left corner [4]
	vertices[30] = 0;  vertices[31] = 0;  vertices[32] = 2;   // (d) Origin up 2 [4]
	vertices[33] = 0;  vertices[34] = -1;  vertices[35] = 2;   // (e) Odd shape top left corner [4]
	vertices[36] = -1;  vertices[37] = 0;  vertices[38] = 2;   // (n) Back Right Corner on Top Square Face [5]
	vertices[39] = 0;  vertices[40] = 0;  vertices[41] = 2;   // (d) Origin up 2 [5]
	vertices[42] = 0;  vertices[43] = 2;  vertices[44] = 1;   // (c) Front right corner up 1 [5]
	vertices[45] = -1;  vertices[46] = 0;  vertices[47] = 2;   // (n) Back Right Corner on Top Square Face [6]
	vertices[48] = -1;  vertices[49] = 2;  vertices[50] = 1;   // (p) Back Right Middle Corner [6]
	vertices[51] = 0;  vertices[52] = 2;  vertices[53] = 1;   // (c) Front right corner up 1 [6]
	vertices[54] = -1;  vertices[55] = 2;  vertices[56] = 1;   // (p) Back Right Middle Corner [7]
	vertices[57] = -1;  vertices[58] = 2;  vertices[59] = 0;   // (o) Back Right Bottom Corner [7]
	vertices[60] = -1;  vertices[61] = -1;  vertices[62] = 0;   // (l) Back Left Bottom Corner [7]
	vertices[63] = -1;  vertices[64] = -1;  vertices[65] = 0;   // (l) Back Left Bottom Corner [8]
	vertices[66] = -1;  vertices[67] = 2;  vertices[68] = 1;   // (p) Back Right Middle Corner [8]
	vertices[69] = -1;  vertices[70] = 0;  vertices[71] = 2;   // (n) Back Right Corner on Top Square Face [8]
	vertices[72] = -1;  vertices[73] = 0;  vertices[74] = 2;   // (n) Back Right Corner on Top Square Face [9]
	vertices[75] = -1;  vertices[76] = -1;  vertices[77] = 0;   // (l) Back Left Bottom Corner [9]
	vertices[78] = -1;  vertices[79] = -1;  vertices[80] = 2;   // (m) Back Left Top Corner [9]
	vertices[81] = 0;  vertices[82] = -1;  vertices[83] = 2;   // (e) Odd shape top left corner [10]
	vertices[84] = 0;  vertices[85] = 0;  vertices[86] = 2;   // (d) Origin up 2 [10]
	vertices[87] = -1;  vertices[88] = -1;  vertices[89] = 2;   // (m) Back Left Top Corner [10]
	vertices[90] = 0;  vertices[91] = 0;  vertices[92] = 2;   // (d) Origin up 2 [11]
	vertices[93] = -1;  vertices[94] = -1;  vertices[95] = 2;   // (m) Back Left Top Corner [11]
	vertices[96] = -1;  vertices[97] = 0;  vertices[98] = 2;   // (n) Back Right Corner on Top Square Face [11]
	vertices[99] = -1;  vertices[100] = -1;  vertices[101] = 2;   // (m) Back Left Top Corner [10]
	vertices[102] = 0;  vertices[103] = -1;  vertices[104] = 2;   // (e) Odd shape top left corner [10]
	vertices[105] = -1;  vertices[106] = -1;  vertices[107] = 0;   // (l) Back Left Bottom Corner [10]
	vertices[108] = -1;  vertices[109] = -1;  vertices[110] = 0;   // (l) Back Left Bottom Corner [10]
	vertices[111] = 0;  vertices[112] = -1;  vertices[113] = 2;   // (e) Odd shape top left corner [10]
	vertices[114] = 0;  vertices[115] = -1;  vertices[116] = 1;   // (f) Odd shape back left corner [10]
	vertices[117] = 0;  vertices[118] = -1;  vertices[119] = 1;   // (f) Odd shape back left corner [10]
	vertices[120] = -1;  vertices[121] = -1;  vertices[122] = 0;   // (l) Back Left Bottom Corner [10]
	vertices[123] = 2;  vertices[124] = -1;  vertices[125] = 1;   // (k) Front square top left [10]
	vertices[126] = 2;  vertices[127] = -1;  vertices[128] = 1;   // (k) Front square top left [10]
	vertices[129] = -1;  vertices[130] = -1;  vertices[131] = 0;   // (l) Back Left Bottom Corner [10]
	vertices[132] = 2;  vertices[133] = -1;  vertices[134] = 0;   // (j) Front square bottom left [10]
	vertices[135] = 0;  vertices[136] = -1;  vertices[137] = 1;   // (f) Odd shape back left corner [8]
	vertices[138] = 2;  vertices[139] = -1;  vertices[140] = 1;   // (k) Front square top left [8]
	vertices[141] = 2;  vertices[142] = 0;  vertices[143] = 1;   // (i) Front square top right [8]
	vertices[144] = 2;  vertices[145] = 0;  vertices[146] = 1;   // (i) Front square top right [8]
	vertices[147] = 0;  vertices[148] = -1;  vertices[149] = 1;   // (f) Odd shape back left corner [8]
	vertices[150] = 0;  vertices[151] = 0;  vertices[152] = 1;   // (g) Origin up 1 [8]
	vertices[153] = 2;  vertices[154] = 0;  vertices[155] = 1;   // (i) Front square top right [7]
	vertices[156] = 0;  vertices[157] = 0;  vertices[158] = 1;   // (g) Origin up 1 [7]
	vertices[159] = 2;  vertices[160] = 0;  vertices[161] = 0;   // (h) Front square bottom right [7]
	vertices[162] = 2;  vertices[163] = 0;  vertices[164] = 0;   // (h) Front square bottom right [7]
	vertices[165] = 0;  vertices[166] = 0;  vertices[167] = 1;   // (g) Origin up 1 [7]
	vertices[168] = 0;  vertices[169] = 0;  vertices[170] = 0;   // (a) Origin [7]
	vertices[171] = 2;  vertices[172] = 0;  vertices[173] = 0;   // (h) Front square bottom right [9]
	vertices[174] = 2;  vertices[175] = -1;  vertices[176] = 0;   // (j) Front square bottom left [9]
	vertices[177] = 2;  vertices[178] = 0;  vertices[179] = 1;   // (i) Front square top right [9]
	vertices[180] = 2;  vertices[181] = -1;  vertices[182] = 0;   // (j) Front square bottom left [9]
	vertices[183] = 2;  vertices[184] = 0;  vertices[185] = 1;   // (i) Front square top right [9]
	vertices[186] = 2;  vertices[187] = -1;  vertices[188] = 1;   // (k) Front square top left [9]
	vertices[189] = 0;  vertices[190] = 2;  vertices[191] = 1;   // (c) Front right corner up 1 [1]
	vertices[192] = 0;  vertices[193] = 2;  vertices[194] = 0;   // (b) Front right corner [1]
	vertices[195] = -1;  vertices[196] = 2;  vertices[197] = 0;   // (o) Back Right Bottom Corner [1]
	vertices[198] = -1;  vertices[199] = 2;  vertices[200] = 0;   // (o) Back Right Bottom Corner [1]
	vertices[201] = -1;  vertices[202] = 2;  vertices[203] = 1;   // (p) Back Right Middle Corner [1]
	vertices[204] = 0;  vertices[205] = 2;  vertices[206] = 1;   // (c) Front right corner up 1 [1]
	vertices[207] = 0;  vertices[208] = 2;  vertices[209] = 0;   // (b) Front right corner [12]
	vertices[210] = -1;  vertices[211] = 2;  vertices[212] = 0;   // (o) Back Right Bottom Corner [12]
	vertices[213] = -1;  vertices[214] = -1;  vertices[215] = 0;   // (l) Back Left Bottom Corner [12]
	vertices[216] = -1;  vertices[217] = -1;  vertices[218] = 0;   // (l) Back Left Bottom Corner [12]
	vertices[219] = 0;  vertices[220] = 0;  vertices[221] = 0;   // (a) Origin [12]
	vertices[222] = 0;  vertices[223] = 2;  vertices[224] = 0;   // (b) Front right corner [12]
	vertices[225] = -1;  vertices[226] = -1;  vertices[227] = 0;   // (l) Back Left Bottom Corner [12]
	vertices[228] = 0;  vertices[229] = 0;  vertices[230] = 0;   // (a) Origin [12]
	vertices[231] = 2;  vertices[232] = 0;  vertices[233] = 0;   // (h) Front square bottom right [12]
	vertices[234] = 2;  vertices[235] = 0;  vertices[236] = 0;   // (h) Front square bottom right [12]
	vertices[237] = -1;  vertices[238] = -1;  vertices[239] = 0;   // (l) Back Left Bottom Corner [12]
	vertices[240] = 2;  vertices[241] = -1;  vertices[242] = 0;   // (j) Front square bottom left [12]


																  // Colors
	colors[0] = 0;  colors[1] = 0;  colors[2] = 1;  //Origin [1]
	colors[3] = 0;  colors[4] = 0;  colors[5] = 1;  //Front right corner [1]
	colors[6] = 0;  colors[7] = 0;  colors[8] = 1;  //Front right corner up 1 [1]
	colors[9] = 0;  colors[10] = 0;  colors[11] = 1;  //Front right corner up 1 [2]
	colors[12] = 0;  colors[13] = 0;  colors[14] = 1;  //Origin [2]
	colors[15] = 0;  colors[16] = 0;  colors[17] = 1;  //Origin up 2 [2]
	colors[18] = 0;  colors[19] = 0;  colors[20] = 1;  //Origin up 2 [3]
	colors[21] = 0;  colors[22] = 0;  colors[23] = 1;  //Origin up 1 [3]
	colors[24] = 0;  colors[25] = 0;  colors[26] = 1;  //Odd shape back left corner [3]
	colors[27] = 0;  colors[28] = 0;  colors[29] = 1;  //Odd shape back left corner [4]
	colors[30] = 0;  colors[31] = 0;  colors[32] = 1;  //Origin up 2 [4]
	colors[33] = 0;  colors[34] = 0;  colors[35] = 1;  //Odd shape top left corner [4]
	colors[36] = 0;  colors[37] = 0;  colors[38] = 1;  //Back Right Corner on Top Square Face [5]
	colors[39] = 0;  colors[40] = 0;  colors[41] = 1;  //Origin up 2 [5]
	colors[42] = 0;  colors[43] = 0;  colors[44] = 1;  //Front right corner up 1 [5]
	colors[45] = 0;  colors[46] = 0;  colors[47] = 1;  //Back Right Corner on Top Square Face [6]
	colors[48] = 0;  colors[49] = 0;  colors[50] = 1;  //Back Right Middle Corner [6]
	colors[51] = 0;  colors[52] = 0;  colors[53] = 1;  //Front right corner up 1 [6]
	colors[54] = 0;  colors[55] = 0;  colors[56] = 1;  //Back Right Middle Corner [7]
	colors[57] = 0;  colors[58] = 0;  colors[59] = 1;  //Back Right Bottom Corner [7]
	colors[60] = 0;  colors[61] = 0;  colors[62] = 1;  //Back Left Bottom Corner [7]
	colors[63] = 0;  colors[64] = 0;  colors[65] = 1;  //Back Left Bottom Corner [8]
	colors[66] = 0;  colors[67] = 0;  colors[68] = 1;  //Back Right Middle Corner [8]
	colors[69] = 0;  colors[70] = 0;  colors[71] = 1;  //Back Right Corner on Top Square Face [8]
	colors[72] = 0;  colors[73] = 0;  colors[74] = 1;  //Back Right Corner on Top Square Face [9]
	colors[75] = 0;  colors[76] = 0;  colors[77] = 1;  //Back Left Bottom Corner [9]
	colors[78] = 0;  colors[79] = 0;  colors[80] = 1;  //Back Left Top Corner [9]
	colors[81] = 0;  colors[82] = 0;  colors[83] = 1;  //Odd shape top left corner [10]
	colors[84] = 0;  colors[85] = 0;  colors[86] = 1;  //Origin up 2 [10]
	colors[87] = 0;  colors[88] = 0;  colors[89] = 1;  //Back Left Top Corner [10]
	colors[90] = 0;  colors[91] = 0;  colors[92] = 1;  //Origin up 2 [11]
	colors[93] = 0;  colors[94] = 0;  colors[95] = 1;  //Back Left Top Corner [11]
	colors[96] = 0;  colors[97] = 0;  colors[98] = 1;  //Back Right Corner on Top Square Face [11]
	colors[99] = 0;  colors[100] = 0;  colors[101] = 1;  //Back Left Top Corner [10]
	colors[102] = 0;  colors[103] = 0;  colors[104] = 1;  //Odd shape top left corner [10]
	colors[105] = 0;  colors[106] = 0;  colors[107] = 1;  //Back Left Bottom Corner [10]
	colors[108] = 0;  colors[109] = 0;  colors[110] = 1;  //Back Left Bottom Corner [10]
	colors[111] = 0;  colors[112] = 0;  colors[113] = 1;  //Odd shape top left corner [10]
	colors[114] = 0;  colors[115] = 0;  colors[116] = 1;  //Odd shape back left corner [10]
	colors[117] = 0;  colors[118] = 0;  colors[119] = 1;  //Odd shape back left corner [10]
	colors[120] = 0;  colors[121] = 0;  colors[122] = 1;  //Back Left Bottom Corner [10]
	colors[123] = 0;  colors[124] = 0;  colors[125] = 1;  //Front square top left [10]
	colors[126] = 0;  colors[127] = 0;  colors[128] = 1;  //Front square top left [10]
	colors[129] = 0;  colors[130] = 0;  colors[131] = 1;  //Back Left Bottom Corner [10]
	colors[132] = 0;  colors[133] = 0;  colors[134] = 1;  //Front square bottom left [10]
	colors[135] = 0;  colors[136] = 0;  colors[137] = 1;  //Odd shape back left corner [8]
	colors[138] = 0;  colors[139] = 0;  colors[140] = 1;  //Front square top left [8]
	colors[141] = 0;  colors[142] = 0;  colors[143] = 1;  //Front square top right [8]
	colors[144] = 0;  colors[145] = 0;  colors[146] = 1;  //Front square top right [8]
	colors[147] = 0;  colors[148] = 0;  colors[149] = 1;  //Odd shape back left corner [8]
	colors[150] = 0;  colors[151] = 0;  colors[152] = 1;  //Origin up 1 [8]
	colors[153] = 0;  colors[154] = 0;  colors[155] = 1;  //Front square top right [7]
	colors[156] = 0;  colors[157] = 0;  colors[158] = 1;  //Origin up 1 [7]
	colors[159] = 0;  colors[160] = 0;  colors[161] = 1;  //Front square bottom right [7]
	colors[162] = 0;  colors[163] = 0;  colors[164] = 1;  //Front square bottom right [7]
	colors[165] = 0;  colors[166] = 0;  colors[167] = 1;  //Origin up 1 [7]
	colors[168] = 0;  colors[169] = 0;  colors[170] = 1;  //Origin [7]
	colors[171] = 0;  colors[172] = 0;  colors[173] = 1;  //Front square bottom right [9]
	colors[174] = 0;  colors[175] = 0;  colors[176] = 1;  //Front square bottom left [9]
	colors[177] = 0;  colors[178] = 0;  colors[179] = 1;  //Front square top right [9]
	colors[180] = 0;  colors[181] = 0;  colors[182] = 1;  //Front square bottom left [9]
	colors[183] = 0;  colors[184] = 0;  colors[185] = 1;  //Front square top right [9]
	colors[186] = 0;  colors[187] = 0;  colors[188] = 1;  //Front square top left [9]
	colors[189] = 0;  colors[190] = 0;  colors[191] = 1;  //Front right corner up 1 [1]
	colors[192] = 0;  colors[193] = 0;  colors[194] = 1;  //Front right corner [1]
	colors[195] = 0;  colors[196] = 0;  colors[197] = 1;  //Back Right Bottom Corner [1]
	colors[198] = 0;  colors[199] = 0;  colors[200] = 1;  //Back Right Bottom Corner [1]
	colors[201] = 0;  colors[202] = 0;  colors[203] = 1;  //Back Right Middle Corner [1]
	colors[204] = 0;  colors[205] = 0;  colors[206] = 1;  //Front right corner up 1 [1]
	colors[207] = 0;  colors[208] = 0;  colors[209] = 1;  //Front right corner [12]
	colors[210] = 0;  colors[211] = 0;  colors[212] = 1;  //Back Right Bottom Corner [12]
	colors[213] = 0;  colors[214] = 0;  colors[215] = 1;  //Back Left Bottom Corner [12]
	colors[216] = 0;  colors[217] = 0;  colors[218] = 1;  //Back Left Bottom Corner [12]
	colors[219] = 0;  colors[220] = 0;  colors[221] = 1;  //Origin [12]
	colors[222] = 0;  colors[223] = 0;  colors[224] = 1;  //Front right corner [12]
	colors[225] = 0;  colors[226] = 0;  colors[227] = 1;  //Back Left Bottom Corner [12]
	colors[228] = 0;  colors[229] = 0;  colors[230] = 1;  //Origin [12]
	colors[231] = 0;  colors[232] = 0;  colors[233] = 1;  //Front square bottom right [12]
	colors[234] = 0;  colors[235] = 0;  colors[236] = 1;  //Front square bottom right [12]
	colors[237] = 0;  colors[238] = 0;  colors[239] = 1;  //Back Left Bottom Corner [12]
	colors[240] = 0;  colors[241] = 0;  colors[242] = 1;  //Front square bottom left [12]


















	// General setup for loading to graphics card

	glGenVertexArrays(1, &vaoID[1]); // Create our Vertex Array Object
	glBindVertexArray(vaoID[1]); // Bind our Vertex Array Object so we can use it

	glGenBuffers(2, &vboID[2]);

	// Vertex buffer

	glBindBuffer(GL_ARRAY_BUFFER, vboID[2]); // Bind our Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, 3 * numVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer

																   // Enables vertex array
	glEnableVertexAttribArray(0);

	// Colors

	glBindBuffer(GL_ARRAY_BUFFER, vboID[3]); // Bind our Color Buffer Object
	glBufferData(GL_ARRAY_BUFFER, 3 * numVertices * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer

																   // Enables color array
	glEnableVertexAttribArray(1);



	// END DAVID CODE


    //TODO:
    // vaoID[1];
    
    return 1;
}



/*!
 ADD YOUR CODE TO RENDER THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 */
void renderTriangleStripModel(void)
{
    // Bind the buffer and switch it to an active buffer
    glBindVertexArray(vaoID[0]);
        
	// HERE: THIS CAUSES AN ERROR BECAUSE I DO NOT KNOW HOW MANY TRIANGLES / VERTICES YOU HAVE.
	// COMPLETE THE LINE
    // Draw the triangles
    
	glDrawArrays(GL_TRIANGLE_STRIP, 0 , 18); // Faces 1, 2, 3, 10, 9, 12, 5
	glDrawArrays(GL_TRIANGLE_STRIP, 18, 6); // Faces 4 and 8
	glDrawArrays(GL_TRIANGLE_STRIP, 24, 5); // Face 11
	glDrawArrays(GL_TRIANGLE_STRIP, 29, 6); // Face 12
	glDrawArrays(GL_TRIANGLE_STRIP, 35, 3); // Face 6


    // Unbind our Vertex Array Object
    glBindVertexArray(0);
}



/*!
 ADD YOUR CODE TO RENDER THE POLYGON MODEL TO THIS FUNCTION
 */
void renderPolygonModel(void)
{

    // Bind the buffer and switch it to an active buffer
    glBindVertexArray(vaoID[1]);
        

	// HERE: THIS CAUSES AN ERROR BECAUSE I DO NOT KNOW HOW MANY POLYGONS YOU HAVE.
	// COMPLETE THE LINE
    // Draw the triangles

	// Couldn't get GL_POLYGON running on my machine.
	glDrawArrays(GL_TRIANGLES, 0, 243);
	

    // Unbind our Vertex Array Object
    glBindVertexArray(0);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*!
 This function creates the two models
 */
void setupScene(void) {
    
    createTriangleStripModel();
	renderTriangleStripModel();


	// DAVID CODE
	createPolygonModel();
	renderPolygonModel();
	// END DAVID CODE
}




int main(int argc, const char * argv[])
{
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    window = initWindow();
    
    
    // Init the glew api
    initGlew();
    
	// Prepares some defaults
	CoordSystemRenderer* coordinate_system_renderer = new CoordSystemRenderer(10.0);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// The Shader Program starts here
    
    // Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
    // Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
    static const string vertex_code = vs_string;
    static const char * vs_source = vertex_code.c_str();
    
    // Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
    static const string fragment_code = fs_string;
    static const char * fs_source = fragment_code.c_str();
    
    // This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
    program = glCreateProgram();
    
    // We create a shader with our fragment shader source code and compile it.
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    
    // We create a shader with our vertex shader source code and compile it.
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    
    // We'll attach our two compiled shaders to the OpenGL program.
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    glLinkProgram(program);
    
    // We'll specify that we want to use this program that we've attached the shaders to.
    glUseProgram(program);
    
    //// The Shader Program ends here
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    /// IGNORE THE NEXT PART OF THIS CODE
    /// IGNORE THE NEXT PART OF THIS CODE
    /// IGNORE THE NEXT PART OF THIS CODE
    // It controls the virtual camera
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    
    projectionMatrix = glm::perspective(1.1f, (float)800 / (float)600, 0.1f, 100.f);
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
    viewMatrix = glm::lookAt(glm::vec3(1.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    int projectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
    int viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
    int modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader
    
    
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
    
    
	 //// The Shader Program ends here
    //// START TO READ AGAIN
    //// START TO READ AGAIN
    //// START TO READ AGAIN
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    


    glBindAttribLocation(program, 0, "in_Position");
    glBindAttribLocation(program, 1, "in_Color");
    

    
    // this creates the scene
    setupScene();
    
    int i=0;

    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
        // this draws the coordinate system
		coordinate_system_renderer->draw();
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This generate the object
        // Enable the shader program
        glUseProgram(program);
        
        // this changes the camera location
        glm::mat4 rotated_view = viewMatrix * GetRotationMatrix();
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
        

        // This moves the model to the right
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
        

		// This line renders your triangle strip model
        renderTriangleStripModel();
        

        // This moves the model to the left
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -0.0f, 0.0f));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
        
		// This line renders your Ppolygon model
        renderPolygonModel();
        
        
		// disable the shader program
        glUseProgram(0);


        //// This generate the object
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
	// delete the coordinate system object
	delete coordinate_system_renderer;

    // Program clean up when the window gets closed.
    glDeleteVertexArrays(2, vaoID);
    glDeleteProgram(program);
}

