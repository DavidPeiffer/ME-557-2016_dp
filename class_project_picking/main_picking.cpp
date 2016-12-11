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
#include "Texture.h"
#include "GLObjectObj.h"

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



GLObjectObj* loadedModel1 = NULL; // this is a teapot
GLObjectObj* loadedModel2 = NULL; // this is a box
GLObjectObj* loadedModel3 = NULL; // this is a box
GLObjectObj* loadedModel4 = NULL; // this is a box
GLObjectObj* loadedModel5 = NULL; // this is a box
GLObjectObj* loadedModel6 = NULL; // this is a box
GLObjectObj* loadedModel7 = NULL; // this is a box
GLObjectObj* loadedModel8 = NULL; // this is a box





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
		case 1:
			// cout << "object 2 selected" << "Object " << to_string(selected_object_id) << " selected";
			setSelectColor(loadedModel1->getProgram(), false);
			g_selected_object_id = 0;
			break;
		case 2:
			// cout << "object 2 selected" << "Object " << to_string(selected_object_id) << " selected";
			setSelectColor(loadedModel2->getProgram(), false);
			g_selected_object_id = 0;
			break;
		case 3:
			// cout << "object 2 selected" << "Object " << to_string(selected_object_id) << " selected";
			setSelectColor(loadedModel3->getProgram(), false);
			g_selected_object_id = 0;
			break;
        case 4:
			// cout << "object 2 selected" << "Object " << to_string(selected_object_id) << " selected"; 
			setSelectColor(loadedModel4->getProgram(), false);
            g_selected_object_id = 0;
            break;
		case 5:
			// cout << "object 2 selected" << "Object " << to_string(selected_object_id) << " selected";
			setSelectColor(loadedModel5->getProgram(), false);
			g_selected_object_id = 0;
			break;
		case 6:
			// cout << "object 2 selected" << "Object " << to_string(selected_object_id) << " selected";
			setSelectColor(loadedModel6->getProgram(), false);
			g_selected_object_id = 0;
			break;
		case 7:
			// cout << "object 2 selected" << "Object " << to_string(selected_object_id) << " selected";
			setSelectColor(loadedModel7->getProgram(), false);
			g_selected_object_id = 0;
			break;
        case 8:
			setSelectColor(loadedModel8->getProgram(), false);
            g_selected_object_id = 0;
            break;
        }
		cout << "Object " << to_string(selected_object_id) << " selected\n";

    }
    
    
    // Now we change the color of the selected object
    switch(selected_object_id)
    {
	case 1:
		setSelectColor(loadedModel1->getProgram(), true);
		g_selected_object_id = selected_object_id;
		break;
	case 2:
		setSelectColor(loadedModel2->getProgram(), true);
		g_selected_object_id = selected_object_id;
		break;
	case 3:
		setSelectColor(loadedModel3->getProgram(), true);
		g_selected_object_id = selected_object_id;
		break;
    case 4:
        setSelectColor(loadedModel4->getProgram(), true);
        g_selected_object_id = selected_object_id;
        break;
	case 5:
		setSelectColor(loadedModel5->getProgram(), true);
		g_selected_object_id = selected_object_id;
		break;
	case 6:
		setSelectColor(loadedModel6->getProgram(), true);
		g_selected_object_id = selected_object_id;
		break;
	case 7:
		setSelectColor(loadedModel7->getProgram(), true);
		g_selected_object_id = selected_object_id;
		break;
	case 8:
		setSelectColor(loadedModel8->getProgram(), true);
		g_selected_object_id = selected_object_id;
		break;
    }
}



int main(int argc, const char * argv[])
{


    // int we = colorToInteger(0, 1, 1,0);
    
    
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
    // GLAppearance* appearance_neutral = new GLAppearance("../data/shaders/multi_vertex_lights_ext.vs", "../data/shaders/multi_vertex_lights.fs");
	GLAppearance* appearance_neutral = new GLAppearance("../class_project_shaders/multi_vertex_lights_ext.vs", "../class_project_shaders/multi_vertex_lights.fs");
    
    GLDirectLightSource  light_source;
    light_source._lightPos = glm::vec4(20.0,20.0,0.0, 0.0);
    light_source._ambient_intensity = 0.2;
    light_source._specular_intensity = 1.5;
    light_source._diffuse_intensity = 2.0;
    light_source._attenuation_coeff = 0.0;

	GLDirectLightSource  light_source2;
	light_source._lightPos = glm::vec4(-20.0, -20.0, 0.0, 0.0);
	light_source._ambient_intensity = 0.2;
	light_source._specular_intensity = 1.5;
	light_source._diffuse_intensity = 2.0;
	light_source._attenuation_coeff = 0.0;
    
    // add the light to this apperance object
    appearance_neutral->addLightSource(light_source);
	appearance_neutral->addLightSource(light_source2);
    
    // Create a material object
    GLMaterial material_0;
    material_0._diffuse_material = glm::vec3(0.0, 0.2, 0.8);
    material_0._ambient_material = glm::vec3(0.0, 0.2, 0.8);
    material_0._specular_material = glm::vec3(0.1, 0.1, 0.1);
    material_0._shininess = 12.0;
    material_0._transparency = 1.0;
    
    // Add the material to the apperance object
    appearance_neutral->setMaterial(material_0);


	//************************************************************************************************
	// Add a texture
	// GLMultiTexture* texture = new GLMultiTexture();

	GLTexture* texture_0 = new GLTexture();

	// int texid = texture->loadAndCreateTextures("../data/textures/texture_brick.bmp", "../data/textures/light_512_512a.bmp");
	int texid = texture_0->loadAndCreateTexture("../models_for_project/simple_texture.bmp");

	appearance_neutral->setTexture(texture_0);



    appearance_neutral->finalize();


	// If you want to change appearance parameters after you init the object, call the update function
    appearance_neutral->updateLightSources();

    
	// Create a material object
	GLMaterial material_yellow;
	material_yellow._diffuse_material = glm::vec3(0.8, 0.9, 0.0);
	material_yellow._ambient_material = glm::vec3(0.8, 0.9, 0.0);
	material_yellow._specular_material = glm::vec3(0.1, 0.1, 0.1);
	material_yellow._shininess = 12.0;
	material_yellow._transparency = 1.0;


     // create an additional apperance object.
	GLAppearance* apperance_selected = new GLAppearance("../class_project_shaders/multi_vertex_lights_ext.vs", "../class_project_shaders/multi_vertex_lights.fs");
   
	apperance_selected->addLightSource(light_source);

	// Add the material to the apperance object
	apperance_selected->setMaterial(material_yellow);
	apperance_selected->finalize();

	GLAppearance* apperance_2 = new GLAppearance("../class_project_shaders/multi_vertex_lights_ext.vs", "../class_project_shaders/multi_vertex_lights.fs");

	apperance_2->addLightSource(light_source);

	// Create a material object
	GLMaterial material_orange;
	material_orange._diffuse_material = glm::vec3(1, 0.6, 0.0);
	material_orange._ambient_material = glm::vec3(1, 0.6, 0.0);
	material_orange._specular_material = glm::vec3(1, 0.6, 0.0);
	material_orange._shininess = 12.0;
	material_orange._transparency = 1.0;

	// Add the material to the apperance object
	apperance_2->setMaterial(material_orange);
	apperance_2->finalize();
    

	// DAVID CODE
	// create an additional apperance objects (3-8).
	GLAppearance* apperance_3 = new GLAppearance("../class_project_shaders/multi_vertex_lights_ext.vs", "../class_project_shaders/multi_vertex_lights.fs");
	apperance_3->addLightSource(light_source);
	apperance_3->setMaterial(material_yellow);
	apperance_3->finalize();

	GLAppearance* apperance_4 = new GLAppearance("../class_project_shaders/multi_vertex_lights_ext.vs", "../class_project_shaders/multi_vertex_lights.fs");
	apperance_4->addLightSource(light_source);
	apperance_4->setMaterial(material_yellow);
	apperance_4->finalize();

	GLAppearance* apperance_5 = new GLAppearance("../class_project_shaders/multi_vertex_lights_ext.vs", "../class_project_shaders/multi_vertex_lights.fs");
	apperance_5->addLightSource(light_source);
	apperance_5->setMaterial(material_yellow);
	apperance_5->finalize();

	GLAppearance* apperance_6 = new GLAppearance("../class_project_shaders/multi_vertex_lights_ext.vs", "../class_project_shaders/multi_vertex_lights.fs");
	apperance_6->addLightSource(light_source);
	apperance_6->setMaterial(material_yellow);
	apperance_6->finalize();

	GLAppearance* apperance_7 = new GLAppearance("../class_project_shaders/multi_vertex_lights_ext.vs", "../class_project_shaders/multi_vertex_lights.fs");
	apperance_7->addLightSource(light_source);
	apperance_7->setMaterial(material_yellow);
	apperance_7->finalize();

	GLAppearance* apperance_8 = new GLAppearance("../class_project_shaders/multi_vertex_lights_ext.vs", "../class_project_shaders/multi_vertex_lights.fs");
	apperance_8->addLightSource(light_source);
	apperance_8->setMaterial(material_yellow);
	apperance_8->finalize();



	// END DAVID CODE





    
    // loadedModel1 = new GLObjectObj("../data/teapot_t.obj");
	loadedModel1 = new GLObjectObj("../models_for_project/1.obj");
    loadedModel1->setApperance(*appearance_neutral);
    loadedModel1->init();
    

    glm::mat4 tranform_1 =   glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f)) ;
    loadedModel1->setMatrix(tranform_1);
    
    
    // loadedModel2 = new GLObjectObj("../data/box_t.obj");
	loadedModel2 = new GLObjectObj("../models_for_project/2.obj");
    loadedModel2->setApperance(*apperance_selected);
    loadedModel2->init();
    
	glm::mat4 tranform_2 =   glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f)) ;
    loadedModel2->setMatrix(tranform_2);
    


	loadedModel3 = new GLObjectObj("../models_for_project/3.obj");
	loadedModel3->setApperance(*apperance_3);
	loadedModel3->init();

	// glm::mat4 tranform_2 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
	loadedModel3->setMatrix(tranform_2);



	loadedModel4 = new GLObjectObj("../models_for_project/4.obj");
	loadedModel4->setApperance(*apperance_4);
	loadedModel4->init();

	// glm::mat4 tranform_2 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
	loadedModel4->setMatrix(tranform_2);



	loadedModel5 = new GLObjectObj("../models_for_project/5.obj");
	loadedModel5->setApperance(*apperance_5);
	loadedModel5->init();

	// glm::mat4 tranform_2 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
	loadedModel5->setMatrix(tranform_2);



	loadedModel6 = new GLObjectObj("../models_for_project/6.obj");
	loadedModel6->setApperance(*apperance_6);
	loadedModel6->init();

	// glm::mat4 tranform_2 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
	loadedModel6->setMatrix(tranform_2);



	loadedModel7 = new GLObjectObj("../models_for_project/7.obj");
	loadedModel7->setApperance(*apperance_7);
	loadedModel7->init();

	// glm::mat4 tranform_2 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
	loadedModel7->setMatrix(tranform_2);



	loadedModel8 = new GLObjectObj("../models_for_project/8.obj");
	loadedModel8->setApperance(*apperance_8);
	loadedModel8->init();

	// glm::mat4 tranform_2 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
	loadedModel8->setMatrix(tranform_2);



	// loadedModel3 = new GLObjectObj("../models_for_project/3.obj");
	// loadedModel3->setApperance(*apperance_selected);
	// loadedModel3->init();

	// glm::mat4 tranform_2 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
	// loadedModel3->setMatrix(tranform_2);



    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Prepare the shader for the scissor test
    //// 1. Activate the shader programs
    //// 2. Set a select color and remember the position of the select-switch.
    //// 3. Set the values.
    

	// 0
	glUseProgram(appearance_neutral->getProgram());
	int l0 = glGetUniformLocation(appearance_neutral->getProgram(), "select_mode");
	int sel0 = glGetUniformLocation(appearance_neutral->getProgram(), "is_selected");
	glUniform1i(l0, false);
	glUniform1i(sel0, false);
	glUniform4f(glGetUniformLocation(appearance_neutral->getProgram(), "select_color_id"), 0.0, 0.0, 0.0, 1.0);
	
	// 1
	glUseProgram(apperance_2->getProgram());
	int l1 = glGetUniformLocation(apperance_2->getProgram(), "select_mode");
	int sel1 = glGetUniformLocation(apperance_2->getProgram(), "is_selected");
	glUniform1i(l1, false);
	glUniform1i(sel1, false);
	glUniform4f(glGetUniformLocation(apperance_2->getProgram(), "select_color_id"), 0.0, 0.0, 1.0, 1.0);

	// 2
	glUseProgram(apperance_2->getProgram());
	int l2 = glGetUniformLocation(apperance_2->getProgram(), "select_mode");
	int sel2 = glGetUniformLocation(apperance_2->getProgram(), "is_selected");
	glUniform1i(l2, false);
	glUniform1i(sel2, false);
	glUniform4f(glGetUniformLocation(apperance_2->getProgram(), "select_color_id"), 0.0, 1.0, 0.0, 1.0);

	// 3
    glUseProgram(apperance_selected->getProgram());
    int l3 = glGetUniformLocation(apperance_selected->getProgram(), "select_mode");
    int sel3 = glGetUniformLocation(apperance_selected->getProgram(), "is_selected");
    glUniform1i(l3, false);
    glUniform1i(sel3, false);
    glUniform4f( glGetUniformLocation(apperance_selected->getProgram(), "select_color_id"), 0.0,1.0,1.0,1.0 );

	// 4
	glUseProgram(apperance_selected->getProgram());
	int l4 = glGetUniformLocation(apperance_selected->getProgram(), "select_mode");
	int sel4 = glGetUniformLocation(apperance_selected->getProgram(), "is_selected");
	glUniform1i(l4, false);
	glUniform1i(sel4, false);
	glUniform4f(glGetUniformLocation(apperance_selected->getProgram(), "select_color_id"), 1.0, 0.0, 0.0, 1.0);

	// 5
	glUseProgram(appearance_neutral->getProgram());
	int l5 = glGetUniformLocation(appearance_neutral->getProgram(), "select_mode");
	int sel5 = glGetUniformLocation(appearance_neutral->getProgram(), "is_selected");
	glUniform1i(l5, false);
	glUniform1i(sel5, false);
	glUniform4f(glGetUniformLocation(appearance_neutral->getProgram(), "select_color_id"), 1.0, 0.0, 1.0, 1.0);

	// 6
	glUseProgram(appearance_neutral->getProgram());
	int l6 = glGetUniformLocation(appearance_neutral->getProgram(), "select_mode");
	int sel6 = glGetUniformLocation(appearance_neutral->getProgram(), "is_selected");
	glUniform1i(l6, false);
	glUniform1i(sel6, false);
	glUniform4f(glGetUniformLocation(appearance_neutral->getProgram(), "select_color_id"), 1.0, 1.0, 0.0, 1.0);

	// 7
	glUseProgram(appearance_neutral->getProgram());
	int l7 = glGetUniformLocation(appearance_neutral->getProgram(), "select_mode");
	int sel7 = glGetUniformLocation(appearance_neutral->getProgram(), "is_selected");
	glUniform1i(l7, false);
	glUniform1i(sel7, false);
	glUniform4f(glGetUniformLocation(appearance_neutral->getProgram(), "select_color_id"), 1.0, 1.0, 1.0, 1.0);


	
	



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
        glUseProgram(appearance_neutral->getProgram());
        glUniform1i(l0, true);
        
        // render
        loadedModel1->draw();
        glUniform1i(l0, false); // and switch to regular mode.
        
        // 4. Render the second object
        // Switch to seletion mode and render the first object
        glUseProgram(apperance_selected->getProgram());
        glUniform1i(l1, true);
        
        // render
        loadedModel2->draw();


		// DAVID CODE
		// Draw all objects for selection mode
		loadedModel3->draw();
		loadedModel4->draw();
		loadedModel5->draw();
		loadedModel6->draw();
		loadedModel7->draw();
		loadedModel8->draw();
        // END DAVID CODE


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
