//
//  main_spotlight.cpp
//  HCI 557 Spotlight example
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"
#include "Sphere3D.h"


#include "GLObjectObj.h"

using namespace std;


// The handle to the window object
GLFWwindow*         window;

// Define some of the global variables we're using for this sample
GLuint program;

/* A trackball to move and rotate the camera view */
extern Trackball trackball;




// The sphere object
GLSphere3D* sphere;
GLSphere3D* sphere_result;

// The loaded object
GLObjectObj* cutting_plane;

// to transform the sphere
glm::mat4 g_transform_cutting_plane;
glm::mat4 g_transform_cutting_plane_result;

float g_delta = 1;






// This is the callback we'll be registering with GLFW for keyboard handling.
// The only thing we're doing here is setting up the window to close when we press ESC
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	bool move = false;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (key == 49 && action == GLFW_PRESS) // 1
	{
		
	}
	else if (key == 50 && action == GLFW_PRESS) // 2
	{
		
	}


	////////////////////////////////////////////////////////////////////////////////////////////
	// Translation
	if ((key == 87 && action == GLFW_REPEAT) || (key == 87 && action == GLFW_PRESS)) // key w
	{
		//cout <<  "key w pressed" << endl;
		g_transform_cutting_plane = g_transform_cutting_plane * glm::translate(glm::vec3(-g_delta, 0.0, 0.0f));
		cutting_plane->setMatrix(g_transform_cutting_plane);
		move = true;
	}
	else if ((key == 83 && action == GLFW_REPEAT) || (key == 83 && action == GLFW_PRESS)) // key s
	{
		//cout <<  "key s pressed" << endl;
		g_transform_cutting_plane = g_transform_cutting_plane * glm::translate(glm::vec3(g_delta, 0.0, 0.0f));
		cutting_plane->setMatrix(g_transform_cutting_plane);
		move = true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	// Rotation
	if ((key == 65 && action == GLFW_REPEAT) || (key == 65 && action == GLFW_PRESS)) // key a
	{
		//cout <<  "key a pressed" << endl;
		g_transform_cutting_plane = g_transform_cutting_plane * glm::rotate(0.04f, glm::vec3(0.0f, 0.0f, 1.0f));
		cutting_plane->setMatrix(g_transform_cutting_plane);
		move = true;
	}

	else if ((key == 68 && action == GLFW_REPEAT) || (key == 68 && action == GLFW_REPEAT)) // key d
	{
		//cout <<  "key d pressed" << endl;
		g_transform_cutting_plane = g_transform_cutting_plane * glm::rotate(-0.04f, glm::vec3(0.0f, 0.0f, 1.0f));
		cutting_plane->setMatrix(g_transform_cutting_plane);
		move = true;
	}


	if (move)
	{
		glm::vec3 s(g_transform_cutting_plane[3][0], g_transform_cutting_plane[3][1], g_transform_cutting_plane[3][2]);
		glm::vec3 e(g_transform_cutting_plane[3][0], g_transform_cutting_plane[3][1], g_transform_cutting_plane[3][2] - 20);
		vector<glm::vec3> res;

		// perform the ray intersectiont test.
		/*RayIntersectionTest::intersect(s, e, *loadedModel1, res);

		// pick the first result if one is available
		if (res.size() > 0) {
			glm::vec3 position = res[0];

			g_transform_cutting_plane_result = g_transform_cutting_plane;
			g_transform_cutting_plane_result[3][0] = position[0];
			g_transform_cutting_plane_result[3][1] = position[1];
			g_transform_cutting_plane_result[3][2] = position[2];
			sphere_result->setMatrix(g_transform_cutting_plane_result);
		}*/


	}

	//cout << key << endl;
}




int main(int argc, const char * argv[])
{
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    window = initWindow();
    
    
    // Init the glew api
    initGlew();
    SetCameraManipulator(CameraTypes::CAMERA_MANIPULATOR);

    

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Create some models
    
    // coordinate system
    CoordSystem* cs = new CoordSystem(40.0);
    
    
    // create an apperance object.
    GLAppearance* apperance_0 = new GLAppearance("../data/shaders/multi_vertex_lights.vs", "../data/shaders/multi_vertex_lights.fs");
    

	// FIRST LIGHT

    GLDirectLightSource  light_source1;
	light_source1._lightPos = glm::vec4(20.0, 20.0, 20.0, 0.0);
	light_source1._ambient_intensity = 0.2;
	light_source1._specular_intensity = 5.5;
	light_source1._diffuse_intensity = 1.0;
	light_source1._attenuation_coeff = 0.0;
    
    // add the light to this apperance object
    apperance_0->addLightSource(light_source1);


	// SECOND LIGHT
	GLDirectLightSource  light_source2;
	light_source2._lightPos = glm::vec4(-20.0, -20.0, -20.0, 0.0);
	light_source2._ambient_intensity = 0.2;
	light_source2._specular_intensity = 5.5;
	light_source2._diffuse_intensity = 1.0;
	light_source2._attenuation_coeff = 0.0;

	// add the light to this apperance object
	apperance_0->addLightSource(light_source2);


    
    // Create a material object
    GLMaterial material_0;
    material_0._diffuse_material = glm::vec3(0.8, 0.8, 0.8);
    material_0._ambient_material = glm::vec3(0.8, 0.8, 0.8);
    material_0._specular_material = glm::vec3(0.8, 0.8, 0.8);
    material_0._shininess = 12.0;
    material_0._transparency = 1.0;
    
    // Add the material to the apperance object
    apperance_0->setMaterial(material_0);
    apperance_0->finalize();
    

	

    // If you want to change appearance parameters after you init the object, call the update function
    apperance_0->updateLightSources();
	
	
	glm::mat4 transform_scale = glm::scale(glm::vec3(7.0, 7.0f, 7.0f));
	glm::mat4 transform_translate_1 = glm::translate(glm::vec3(3.0f, 3.0f, 3.0f));


	glm::mat4 plane_transform = glm::translate(glm::vec3(0.0, 0.0f, 10.0f)) * glm::scale(glm::vec3(10.0, 10.0f, 10.0f));
	
	// glm::mat4 transform_translate_2 = glm::translate(glm::vec3(3.0f, -3.0f, -3.0f));

	// Cutting Plane
	GLObjectObj* cutting_plane = new GLObjectObj("Object_Models/simple_models/cutting_plane.obj");
	cutting_plane->setApperance(*apperance_0);
	cutting_plane->init();
	cutting_plane->setMatrix(plane_transform);

    // MODEL 1
	GLObjectObj* loadedModel1 = new GLObjectObj("Object_Models/simple_models/1.obj");
	loadedModel1->setApperance(*apperance_0);
	loadedModel1->init();
	loadedModel1->setMatrix(transform_scale);

	// MODEL 2
	GLObjectObj* loadedModel2 = new GLObjectObj("Object_Models/simple_models/2.obj");
	// GLObjectObj* loadedModel2 = new GLObjectObj("Object_Models/Skull_with_UV_Mapping_6839.obj");
	loadedModel2->setApperance(*apperance_0);
	loadedModel2->init();
	loadedModel2->setMatrix(transform_scale);
	
	// MODEL 3
	GLObjectObj* loadedModel3 = new GLObjectObj("Object_Models/simple_models/3.obj");
	loadedModel3->setApperance(*apperance_0);
	loadedModel3->init();
	loadedModel3->setMatrix(transform_scale);

	// MODEL 4
	GLObjectObj* loadedModel4 = new GLObjectObj("Object_Models/simple_models/4.obj");
	loadedModel4->setApperance(*apperance_0);
	loadedModel4->init();
	loadedModel4->setMatrix(transform_scale);

	// MODEL 5
	GLObjectObj* loadedModel5 = new GLObjectObj("Object_Models/simple_models/5.obj");
	loadedModel5->setApperance(*apperance_0);
	loadedModel5->init();
	loadedModel5->setMatrix(transform_scale);

	// MODEL 6
	GLObjectObj* loadedModel6 = new GLObjectObj("Object_Models/simple_models/6.obj");
	loadedModel6->setApperance(*apperance_0);
	loadedModel6->init();
	loadedModel6->setMatrix(transform_scale);

	// MODEL 7
	GLObjectObj* loadedModel7 = new GLObjectObj("Object_Models/simple_models/7.obj");
	loadedModel7->setApperance(*apperance_0);
	loadedModel7->init();
	loadedModel7->setMatrix(transform_scale);

	// MODEL 8
	GLObjectObj* loadedModel8 = new GLObjectObj("Object_Models/simple_models/8.obj");
	loadedModel8->setApperance(*apperance_0);
	loadedModel8->init();
	loadedModel8->setMatrix(transform_scale);

	
    







	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Here we set a new keyboard callback

	// Set the keyboard callback so that when we press ESC, it knows what to do.
	glfwSetKeyCallback(window, keyboard_callback);

    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    // This sets the camera to a new location
    // the first parameter is the eye position, the second the center location, and the third the up vector. 
    SetViewAsLookAt(glm::vec3(12.0f, 12.0f, 65.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    
    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Blending
    
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
    
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This renders the objects
        
        // Set the trackball locatiom
        SetTrackballLocation(GetCurrentCameraMatrix(), GetCurrentCameraTranslation());
        
        // draw the objects
        cs->draw();
        
		cutting_plane->draw();

        loadedModel1->draw();
		loadedModel2->draw();
		loadedModel3->draw();
		loadedModel4->draw();
		loadedModel5->draw();
		loadedModel6->draw();
		loadedModel7->draw();
		loadedModel8->draw();
        
       
        //// This renders the objects
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    delete cs;


}

