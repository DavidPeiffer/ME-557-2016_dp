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



GLObjectObj* loadedModel0 = NULL; // this is a teapot
GLObjectObj* loadedModel1 = NULL; // this is a box
GLObjectObj* loadedModel2 = NULL; // this is a new object
GLObjectObj* loadedModel3 = NULL; // this is a new object
GLObjectObj* loadedModel4 = NULL; // this is a new object
GLObjectObj* loadedModel5 = NULL; // this is a new object
GLObjectObj* loadedModel6 = NULL; // this is a new object
// GLObjectObj* loadedModel7 = NULL; // this is a new object





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
		case 10:
			setSelectColor(loadedModel4->getProgram(), false);
			g_selected_object_id = 0;
			break;
		case 12:
			setSelectColor(loadedModel5->getProgram(), false);
			g_selected_object_id = 0;
			break;
		case 14:
			setSelectColor(loadedModel6->getProgram(), false);
			g_selected_object_id = 0;
			break;
        }

    }
    
	cout << "object " << to_string(selected_object_id) << " selected\n";
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
	case 10:
		setSelectColor(loadedModel4->getProgram(), true);
		g_selected_object_id = selected_object_id;
		break;
	case 12:
		setSelectColor(loadedModel5->getProgram(), true);
		g_selected_object_id = selected_object_id;
		break;
	case 14:
		setSelectColor(loadedModel6->getProgram(), true);
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
    
#pragma region Materials
	// Create a material object
	GLMaterial material_0;
	material_0._diffuse_material = glm::vec3(0.5, 0.5, 0.5);
	material_0._ambient_material = glm::vec3(0.5, 0.5, 0.5);
	material_0._specular_material = glm::vec3(0.5, 0.5, 0.5);
	material_0._shininess = 12.0;
	material_0._transparency = 1.0;

	// Create a material object
	GLMaterial material_light_blue;
	material_light_blue._diffuse_material = glm::vec3(0.0, 0.5, 0.5);
	material_light_blue._ambient_material = glm::vec3(0.0, 0.5, 0.5);
	material_light_blue._specular_material = glm::vec3(0.1, 0.1, 0.1);
	material_light_blue._shininess = 12.0;
	material_light_blue._transparency = 1.0;
#pragma endregion

#pragma region Lights
	GLDirectLightSource  light_source;
	light_source._lightPos = glm::vec4(20.0, 20.0, 20.0, 0.0);
	light_source._ambient_intensity = 0.2;
	light_source._specular_intensity = 1.5;
	light_source._diffuse_intensity = 2.0;
	light_source._attenuation_coeff = 0.0;
	

	GLDirectLightSource origin_light;
	origin_light._lightPos = glm::vec4(-20.0, -20.0, -20.0, 0.0);
	origin_light._ambient_intensity = 0.2;
	origin_light._specular_intensity = 1.5;
	origin_light._diffuse_intensity = 2.0;
	origin_light._attenuation_coeff = 0.0;
	

	GLSpotLightSource spotlight;
	spotlight._lightPos = glm::vec4(-25.0, 25.0, 25.0, 1.0);
	spotlight._ambient_intensity = 0.0;
	spotlight._specular_intensity = 3;
	spotlight._diffuse_intensity = 10.0;
	spotlight._attenuation_coeff = 0.02;
	spotlight._cone_angle = 10.0; // in degree
	spotlight._cone_direction = glm::vec3(1.0, -1.0, -1.0);;

	

#pragma endregion
    
#pragma region textures
	//************************************************************************************************
	// Add a texture
	GLTexture* texture_0 = new GLTexture();
	int texid = texture_0->loadAndCreateTexture("../models_for_project/simple_texture.bmp");
	

	// GLTexture* texture_0 = new GLTexture("../models_for_project/simple_texture.bmp")

#pragma endregion

#pragma region Setting Appearances
    // Create appearances 
    GLAppearance* apperance_0 = new GLAppearance("../class_project_shaders/class_project_shader.vs", "../class_project_shaders/class_project_shader.fs");
    // apperance_0->addLightSource(light_source);
	// apperance_0->addLightSource(light_source2);
	// apperance_0->addLightSource(origin_light);
    apperance_0->setMaterial(material_light_blue);
	apperance_0->setTexture(texture_0); 
	apperance_0->finalize();
    
	// create an additional apperance object.
    GLAppearance* apperance_1 = new GLAppearance("../class_project_shaders/class_project_shader.vs", "../class_project_shaders/class_project_shader.fs");
    apperance_1->addLightSource(light_source);
	apperance_1->addLightSource(origin_light);
	apperance_1->setMaterial(material_light_blue);
    apperance_1->finalize();

	// DAVID CODE
	// Generate an appearance for each object in the model
	GLAppearance* apperance_2 = new GLAppearance("../class_project_shaders/class_project_shader.vs", "../class_project_shaders/class_project_shader.fs");
	apperance_2->addLightSource(light_source);
	// apperance_2->addLightSource(origin_light);
	apperance_2->setMaterial(material_light_blue);
	apperance_2->finalize();

	GLAppearance* apperance_3 = new GLAppearance("../class_project_shaders/class_project_shader.vs", "../class_project_shaders/class_project_shader.fs");
	apperance_3->addLightSource(light_source);
	apperance_3->addLightSource(origin_light);
	apperance_3->setMaterial(material_light_blue);
	apperance_3->finalize();

	GLAppearance* apperance_4 = new GLAppearance("../class_project_shaders/class_project_shader.vs", "../class_project_shaders/class_project_shader.fs");
	apperance_4->addLightSource(light_source);
	apperance_4->addLightSource(origin_light);
	apperance_4->setMaterial(material_light_blue);
	apperance_4->finalize();

	GLAppearance* apperance_5 = new GLAppearance("../class_project_shaders/class_project_shader.vs", "../class_project_shaders/class_project_shader.fs");
	apperance_5->addLightSource(light_source);
	apperance_5->addLightSource(origin_light);
	apperance_5->setMaterial(material_light_blue);
	apperance_5->finalize();

	GLAppearance* apperance_6 = new GLAppearance("../class_project_shaders/class_project_shader.vs", "../class_project_shaders/class_project_shader.fs");
	apperance_6->addLightSource(light_source);
	apperance_6->addLightSource(origin_light);
	apperance_6->setMaterial(material_light_blue);
	apperance_6->finalize();

	GLAppearance* apperance_7 = new GLAppearance("../class_project_shaders/class_project_shader.vs", "../class_project_shaders/class_project_shader.fs");
	apperance_7->addLightSource(light_source);
	apperance_7->addLightSource(origin_light);
	apperance_7->setMaterial(material_light_blue);
	apperance_7->finalize();




	// END DAVID CODE
#pragma endregion

#pragma region Attaching OBJ files to loadedModel objects
	// Model 0 (Binary 2)
	glm::mat4 tranform_0 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
    
    loadedModel0 = new GLObjectObj("../models_for_project/0.obj");
	loadedModel0->setApperance(*apperance_0);
	loadedModel0->init();
	loadedModel0->setMatrix(tranform_0);
        
    loadedModel1 = new GLObjectObj("../models_for_project/1.obj");
	glm::mat4 tranform_1 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));

    loadedModel1->setApperance(*apperance_1);
    loadedModel1->init();
	loadedModel1->setMatrix(tranform_1);

	glm::mat4 tranform_2 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
	loadedModel2 = new GLObjectObj("../models_for_project/2.obj");
	loadedModel2->setApperance(*apperance_2);
	loadedModel2->init();
	loadedModel2->setMatrix(tranform_2);

	glm::mat4 tranform_3 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
	loadedModel3 = new GLObjectObj("../models_for_project/3.obj");
	loadedModel3->setApperance(*apperance_3);
	loadedModel3->init();
	loadedModel3->setMatrix(tranform_3);
    
	glm::mat4 tranform_4 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
	loadedModel4 = new GLObjectObj("../models_for_project/4.obj");
	loadedModel4->setApperance(*apperance_4);
	loadedModel4->init();
	loadedModel4->setMatrix(tranform_4);

	glm::mat4 tranform_5 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
	loadedModel5 = new GLObjectObj("../models_for_project/5.obj");
	loadedModel5->setApperance(*apperance_5);
	loadedModel5->init();
	loadedModel5->setMatrix(tranform_5);

	glm::mat4 tranform_6 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
	loadedModel6 = new GLObjectObj("../models_for_project/6.obj");
	loadedModel6->setApperance(*apperance_6);
	loadedModel6->init();
	loadedModel6->setMatrix(tranform_6);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Prepare the shader for the scissor test
    //// 1. Activate the shader programs
    //// 2. Set a select color and remember the position of the select-switch.
    //// 3. Set the values.
    
	// Binary 2
	// Object 0
    glUseProgram(apperance_0->getProgram());
    int l0 = glGetUniformLocation(apperance_0->getProgram(), "select_mode");
    int sel0 = glGetUniformLocation(apperance_0->getProgram(), "is_selected");
    glUniform1i(l0, false);
    glUniform1i(sel0, false);
    glUniform4f( glGetUniformLocation(apperance_0->getProgram(), "select_color_id"), 0.0, 0.0, 1.0, 1.0 );

    
	// Binary 4
	// Object 1
    glUseProgram(apperance_1->getProgram());
    int l1 = glGetUniformLocation(apperance_1->getProgram(), "select_mode");
    int sel1 = glGetUniformLocation(apperance_1->getProgram(), "is_selected");
    glUniform1i(l1, false);
    glUniform1i(sel1, false);
    glUniform4f( glGetUniformLocation(apperance_1->getProgram(), "select_color_id"), 0.0 ,1.0, 0.0, 1.0 );

	// Binary 6
	// Object 2
	glUseProgram(apperance_2->getProgram());
	int l2 = glGetUniformLocation(apperance_2->getProgram(), "select_mode");
	int sel2 = glGetUniformLocation(apperance_2->getProgram(), "is_selected");
	glUniform1i(l2, false);
	glUniform1i(sel2, false);
	glUniform4f(glGetUniformLocation(apperance_2->getProgram(), "select_color_id"), 0.0, 1.0, 1.0, 1.0);


	// Binary 8
	// Object 3
	glUseProgram(apperance_3->getProgram());
	int l3 = glGetUniformLocation(apperance_3->getProgram(), "select_mode");
	int sel3 = glGetUniformLocation(apperance_3->getProgram(), "is_selected");
	glUniform1i(l3, false);
	glUniform1i(sel3, false);
	glUniform4f(glGetUniformLocation(apperance_3->getProgram(), "select_color_id"), 1.0, 0.0, 0.0, 1.0);


	// Binary 10
	// Object 4
	glUseProgram(apperance_4->getProgram());
	int l4 = glGetUniformLocation(apperance_4->getProgram(), "select_mode");
	int sel4 = glGetUniformLocation(apperance_4->getProgram(), "is_selected");
	glUniform1i(l4, false);
	glUniform1i(sel4, false);
	glUniform4f(glGetUniformLocation(apperance_4->getProgram(), "select_color_id"), 1.0, 0.0, 1.0, 1.0);


	// Binary 12
	// Object 5
	glUseProgram(apperance_5->getProgram());
	int l5 = glGetUniformLocation(apperance_5->getProgram(), "select_mode");
	int sel5 = glGetUniformLocation(apperance_5->getProgram(), "is_selected");
	glUniform1i(l5, false);
	glUniform1i(sel5, false);
	glUniform4f(glGetUniformLocation(apperance_5->getProgram(), "select_color_id"), 1.0, 1.0, 0.0, 1.0);


	// Binary 14
	// Object 6
	glUseProgram(apperance_6->getProgram());
	int l6 = glGetUniformLocation(apperance_6->getProgram(), "select_mode");
	int sel6 = glGetUniformLocation(apperance_6->getProgram(), "is_selected");
	glUniform1i(l6, false);
	glUniform1i(sel6, false);
	glUniform4f(glGetUniformLocation(apperance_6->getProgram(), "select_color_id"), 1.0, 1.0, 1.0, 1.0);

    glUseProgram(0);
    
    

    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    
    // Set up our green background color
    static const GLfloat clear_color_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    static const GLfloat clear_color_black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    // This sets the camera to a new location
    // the first parameter is the eye position, the second the center location, and the third the up vector. 
    SetViewAsLookAt(glm::vec3(0.0f, 0.0f, 65.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    
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

		#pragma region Shape 0
		glUniform1i(l0, false);
		glUseProgram(apperance_0->getProgram());
        glUniform1i(l0, true);
        
        // render
        loadedModel0->draw();
        glUniform1i(l0, false); // and switch to regular mode.
		

#pragma endregion

		#pragma region Shape 1
        // 4. Render the second object
        // Switch to seletion mode and render the first object
        glUseProgram(apperance_1->getProgram());
        glUniform1i(l1, true);
        
        // render
        loadedModel1->draw();
		#pragma endregion

		#pragma region Shape 2
		glUniform1i(l2, false); // and switch to regular mode.
		glUseProgram(apperance_2->getProgram());
		glUniform1i(l2, true);

		// render
		loadedModel2->draw();
		#pragma endregion

		#pragma region Shape 3
		glUniform1i(l3, false); // and switch to regular mode.
		glUseProgram(apperance_3->getProgram());
		glUniform1i(l3, true);

		// render
		loadedModel3->draw();
		#pragma endregion

		
		#pragma region Shape 4
		glUniform1i(l4, false); // and switch to regular mode.
		glUseProgram(apperance_4->getProgram());
		glUniform1i(l4, true);

		// render
		loadedModel4->draw();
		#pragma endregion


		#pragma region Shape 5
		glUniform1i(l5, false); // and switch to regular mode.
		glUseProgram(apperance_5->getProgram());
		glUniform1i(l5, true);

		// render
		loadedModel5->draw();
		#pragma endregion


#pragma region Shape 6
		glUniform1i(l6, false); // and switch to regular mode.
		glUseProgram(apperance_6->getProgram());
		glUniform1i(l6, true);

		// render
		loadedModel6->draw();
#pragma endregion

		


		// loadedModel2->draw();
		
        
        // switch back.
        // glUniform1i(l1, false);

		// glUseProgram(apperance_0->getProgram());
		// glUniform1i(l0, true);
        
        glUseProgram(0);
        
        // AT THIS LOCATION, WE HAVE TO RENDER ANY ADDITIONAL OBJECT IN "SELECT"-MODE
        // NOTE, GLSL, OPENGL 4.5 DOES NOT PROVIDE A SELECT MODE ANYMORE, WE SIMULATE THIS MODE.
        
        // 5. Disable the scissor test.
        glDisable(GL_SCISSOR_TEST);
        
        
        /////////////////////////////////////////////////////////
        // Read the color information at that pixel.
        float col[4];
        glReadPixels(GetMouseX(), 600-GetMouseY(), 1, 1, GL_RGB,GL_FLOAT,&col);
        cout << "COLOR:\t" << col[0] << "\t" << col[1] << "\t" << col[2]  << "\t" << col[3] << endl;
        
        int object_id = colorToInteger(col[0], col[1], col[2], col[3]);
        // cout << "Found object with id: " << object_id << endl;
        
        
        
        /////////////////////////////////////////////////////////
        // Process the color information
        // Change the color of the selected object
        handle_pick(object_id);
        
        
        /////////////////////////////////////////////////////////
        // Render the visible scene
       
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color_white);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
       
        loadedModel0->draw();
        loadedModel1->draw();
		loadedModel2->draw();
		loadedModel3->draw();
		loadedModel4->draw();
		loadedModel5->draw();
		loadedModel6->draw();
		
		
       
       
        //// This renders the objects
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    delete cs;


}

