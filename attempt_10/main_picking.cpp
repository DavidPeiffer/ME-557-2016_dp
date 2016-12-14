//
//  main_picking.cpp
//  HCI 557 Picking example
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
#include "Plane3D.h"
#include "GLObjectObj.h"
#include "Texture.h"

using namespace std;


// The handle to the window object
GLFWwindow*         window;

// Define some of the global variables we're using for this sample
GLuint program;

/* A trackball to move and rotate the camera view */
extern Trackball trackball;


//////////////////////////////////////////////////////////////////////
// Picking using the scissor test
//
// Picking via scissor test requires to render an object twice. First, with the visual appearance (=render mode),
// Second, using a code as identifier, compressed as color value (=Selection mode).
//
// In selection mode, we grab the color at the mouse tip and use this color to identify an object.
// In render mode, we render as usual.
//
// This variable allows us to keep track of the selected object.
// Start with your idle state.
int g_selected_object_id = 0;



GLObjectObj* loadedModel0 = NULL; // this is a teapot
GLObjectObj* loadedModel1 = NULL; // this is a box
GLObjectObj* loadedModel2 = NULL; // this is a box
GLObjectObj* loadedModel3 = NULL; // this is a box
GLPlane3D* plane_1 = NULL; // Cutting Plane
GLPlane3D* plane_2 = NULL; // Cutting Plane
GLPlane3D* plane_3 = NULL; // Cutting Plane


						   // These variables will be used to display or hide the respective planes
bool ShowPlane1 = true;
bool ShowPlane2 = true;
bool ShowPlane3 = true;

// This is the callback we'll be registering with GLFW for keyboard handling.
// The only thing we're doing here is setting up the window to close when we press ESC
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	bool move = false;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	// Cutting Plane Visibility


	else if (key == 49 && action == GLFW_PRESS) // 1
	{
		// Toggle plane 1 visibility
		cout << "key 1 pressed" << endl;
		if (ShowPlane1)
		{
			ShowPlane1 = false;
		}
		else {
			ShowPlane1 = true;
		}
	}
	else if (key == 50 && action == GLFW_PRESS) // 2
	{
		cout << "key 2 pressed" << endl;
		if (ShowPlane2)
		{
			ShowPlane2 = false;
		}
		else {
			ShowPlane2 = true;
		}
	}
	else if (key == 51 && action == GLFW_PRESS) // 3
	{
		cout << "key 3 pressed" << endl;
		if (ShowPlane3)
		{
			ShowPlane3 = false;
		}
		else {
			ShowPlane3 = true;
		}
	}

	// g_change_texture_blend++;
	// g_change_texture_blend = g_change_texture_blend % 3;


	////////////////////////////////////////////////////////////////////////////////////////////
	// Translation
	if ((key == 87 && action == GLFW_REPEAT) || (key == 87 && action == GLFW_PRESS)) // key w
	{
		cout << "key w pressed" << endl;
	}
	else if ((key == 83 && action == GLFW_REPEAT) || (key == 83 && action == GLFW_PRESS)) // key s
	{
		cout << "key s pressed" << endl;
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	// Rotation
	if ((key == 65 && action == GLFW_REPEAT) || (key == 65 && action == GLFW_PRESS)) // key a
	{
		cout << "key a pressed" << endl;
	}

	else if ((key == 68 && action == GLFW_REPEAT) || (key == 68 && action == GLFW_REPEAT)) // key d
	{
		cout << "key d pressed" << endl;
	}
	// cout << key << endl;
}



/**
This functions converts four integer digits, to an integer value.
The digits should be either 0 or 1.
*/
int colorToInteger(int r, int g, int b, int a)
{
    int selected_object_id = 0;
    
    selected_object_id = selected_object_id << 1;
    selected_object_id |= r;
    selected_object_id = selected_object_id << 1;
    selected_object_id |= g;
    selected_object_id = selected_object_id << 1;
    selected_object_id |= b;
    selected_object_id = selected_object_id << 1;
    selected_object_id |= a;

    return selected_object_id;
}


/*!
@brief - indicate that this object has been selected. 
@param shader_program - the number of the glsl shader program
@param value - set is_selected true or false
*/
void setSelectColor(int shader_program,  bool value)
{
    glUseProgram(shader_program);
    int uniform_id = glGetUniformLocation(shader_program, "is_selected");
    
    glUniform1i(uniform_id, value);
    
    glUseProgram(0);
}



void handle_pick(int selected_object_id)
{

    // first, detect whether a pick event occured.
    // If the same object is picked of the clear color shows up, we return. No state change.
    if(selected_object_id == g_selected_object_id)
    {
        return;
    }
    
    // consider, an object is already picked, disable the pick color.
    // g_selected_object_id == 0, means, no object selected
    if(g_selected_object_id > 0)
    {
        // Note, this can be optimized/
        // We need a list that keeps an association between model and its id.
        // this can be an additional list or part of the object itself.
        // In this case, and for training reasonse (not hiding the code, this is a switch-case control flow.
        switch(g_selected_object_id)
        {
		case 2:
			setSelectColor(loadedModel0->getProgram(), false);
			g_selected_object_id = 0;
			break;
        case 4:
            setSelectColor(loadedModel1->getProgram(), false);
            g_selected_object_id = 0;
            break;
		case 6:
			setSelectColor(loadedModel2->getProgram(), false);
			g_selected_object_id = 0;
			break;
		case 8:
			setSelectColor(loadedModel3->getProgram(), false);
			g_selected_object_id = 0;
			break;
        
        }

    }
    
    
    // Now we change the color of the selected object
    switch(selected_object_id)
    {
    
    case 2:
        setSelectColor(loadedModel0->getProgram(), true);
        g_selected_object_id = selected_object_id;
        break;
	case 4:
		setSelectColor(loadedModel1->getProgram(), true);
		g_selected_object_id = selected_object_id;
		break;
	case 6:
		setSelectColor(loadedModel2->getProgram(), true);
		g_selected_object_id = selected_object_id;
		break;
	case 8:
		setSelectColor(loadedModel3->getProgram(), true);
		g_selected_object_id = selected_object_id;
		break;
    }


}



int main(int argc, const char * argv[])
{


    int we = colorToInteger(0, 1, 1,0);
    
    
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
    GLAppearance* apperance_0 = new GLAppearance("../data/shaders/multi_vertex_lights_ext.vs", "../data/shaders/multi_vertex_lights.fs");
    
    GLDirectLightSource  light_source;
    light_source._lightPos = glm::vec4(20.0,20.0,0.0, 0.0);
    light_source._ambient_intensity = 0.2;
    light_source._specular_intensity = 1.5;
    light_source._diffuse_intensity = 2.0;
    light_source._attenuation_coeff = 0.0;
    
    // add the light to this apperance object
    apperance_0->addLightSource(light_source);
    
    // Create a material object
    GLMaterial material_0;
    material_0._diffuse_material = glm::vec3(0.0, 0.2, 0.8);
    material_0._ambient_material = glm::vec3(0.0, 0.2, 0.8);
    material_0._specular_material = glm::vec3(0.1, 0.1, 0.1);
    material_0._shininess = 12.0;
    material_0._transparency = 1.0;
    
    // Add the material to the apperance object
    apperance_0->setMaterial(material_0);
    apperance_0->finalize();
    

    // If you want to change appearance parameters after you init the object, call the update function
    apperance_0->updateLightSources();
    
    
     // create an additional apperance object.
    GLAppearance* apperance_1 = new GLAppearance("../data/shaders/multi_vertex_lights_ext.vs", "../data/shaders/multi_vertex_lights.fs");
   
    apperance_1->addLightSource(light_source);

   
     // Create a material object
    GLMaterial material_yellow;
    material_yellow._diffuse_material = glm::vec3(0.8, 0.9, 0.0);
    material_yellow._ambient_material = glm::vec3(0.8, 0.9, 0.0);
    material_yellow._specular_material = glm::vec3(0.1, 0.1, 0.1);
    material_yellow._shininess = 12.0;
    material_yellow._transparency = 1.0;
    
    // Add the material to the apperance object
    apperance_1->setMaterial(material_yellow);
    apperance_1->finalize();


    
    loadedModel1 = new GLObjectObj("../data/teapot_t.obj");
    loadedModel1->setApperance(*apperance_0);
    loadedModel1->init();
    

	glm::mat4 tranform_1 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
	loadedModel1->setMatrix(tranform_1);


	loadedModel2 = new GLObjectObj("../data/box_t.obj");
	loadedModel2->setApperance(*apperance_1);
	loadedModel2->init();


	glm::mat4 tranform_2 = glm::translate(glm::vec3(0.0, 0.0f, 10.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
	loadedModel2->setMatrix(tranform_2);
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Prepare the shader for the scissor test
    //// 1. Activate the shader programs
    //// 2. Set a select color and remember the position of the select-switch.
    //// 3. Set the values.
    
	// Binary value 2
    glUseProgram(apperance_0->getProgram());
    int l0 = glGetUniformLocation(apperance_0->getProgram(), "select_mode");
    int sel0 = glGetUniformLocation(apperance_0->getProgram(), "is_selected");
    glUniform1i(l0, false);
    glUniform1i(sel0, false);
    glUniform4f( glGetUniformLocation(apperance_0->getProgram(), "select_color_id"), 0.0, 0.0, 1.0, 1.0 );
    

	// Binary value 4
    glUseProgram(apperance_1->getProgram());
    int l1 = glGetUniformLocation(apperance_1->getProgram(), "select_mode");
    int sel1 = glGetUniformLocation(apperance_1->getProgram(), "is_selected");
    glUniform1i(l1, false);
    glUniform1i(sel1, false);
    glUniform4f( glGetUniformLocation(apperance_1->getProgram(), "select_color_id"), 0.0, 1.0, 0.0, 1.0 );
    glUseProgram(0);
    
    

    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    
    // Set up our green background color
    static const GLfloat clear_color_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    static const GLfloat clear_color_black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
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
        glClearBufferfv(GL_COLOR , 0, clear_color_black);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
 
    
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This renders the objects
        
        // Set the trackball locatiom
        SetTrackballLocation(GetCurrentCameraMatrix(), GetCurrentCameraTranslation());
        
        // draw the objects
        cs->draw();
        
        /////////////////////////////////////////////////////
        // For selection.
        // FIRST, RENDER IN SELECT MODE
        
        // 1. start the scissor test
        glEnable(GL_SCISSOR_TEST);
        
        // 2. Set the window with window size 1x1
        // 600 is the size of the frame, make sure you know it. 
        glScissor(GetMouseX(), 600-GetMouseY(), 1, 1);
        
        // 3. Render the first object
        // Switch to seletion mode and render the first object
        glUseProgram(apperance_0->getProgram());
        glUniform1i(l0, true);
        
        // render
        loadedModel1->draw();
        glUniform1i(l0, false); // and switch to regular mode.
        
        // 4. Render the second object
        // Switch to seletion mode and render the first object
        glUseProgram(apperance_1->getProgram());
        glUniform1i(l1, true);

        // render
        loadedModel2->draw();
        
        // switch back.
        glUniform1i(l1, false);
        
        glUseProgram(0);
        
        // AT THIS LOCATION, WE HAVE TO RENDER ANY ADDITIONAL OBJECT IN "SELECT"-MODE
        // NOTE, GLSL, OPENGL 4.5 DOES NOT PROVIDE A SELECT MODE ANYMORE, WE SIMULATE THIS MODE.
        
        // 5. Disable the scissor test.
        glDisable(GL_SCISSOR_TEST);
        
        
        /////////////////////////////////////////////////////////
        // Read the color information at that pixel.
        float col[4];
        glReadPixels(GetMouseX(), 600-GetMouseY(), 1, 1, GL_RGB,GL_FLOAT,&col);
        //cout << "COLOR:\t" << col[0] << "\t" << col[1] << "\t" << col[2]  << "\t" << col[3] << endl;
        
        int object_id = colorToInteger(col[0], col[1], col[2], col[3]);
        //cout << "Found object with id: " << object_id << endl;
        
        
        
        /////////////////////////////////////////////////////////
        // Process the color information
        // Change the color of the selected object
        handle_pick(object_id);
        
        
        /////////////////////////////////////////////////////////
        // Render the visible scene
       
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color_white);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
       
        loadedModel1->draw();
        loadedModel2->draw();
       
       
        //// This renders the objects
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    delete cs;


}

