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
#include <map>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>


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
using namespace glm;


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
GLPlane3D* plane4 = NULL; // this is a new object
GLPlane3D* plane5 = NULL; // this is a new object
GLPlane3D* plane6 = NULL; // this is a new object
// GLObjectObj* loadedModel7 = NULL; // this is a new object

// Variables to determine whether the planes are shown
bool ShowPlane4 = true;
bool ShowPlane5 = true;
bool ShowPlane6 = true;


double keyframefraction = 0;


#pragma region Keyframe Stuff
typedef struct _keyframe
{
	float               _t; // the time fraction
	glm::vec3           _p; // the position
	glm::quat           _q; // the orientation

							/*
							Constructor
							*/
	_keyframe(float t, glm::vec3 p, glm::quat q)
	{
		_t = t;
		_p = p;
		_q = q;
	}

	/*
	Default constructor
	*/
	_keyframe()
	{
		_t = -1.0;
		_p = glm::vec3(0.0, 0.0, 0.0);
		_q = glm::quat(0.0, 0.0, 0.0, 0.0);
	}

	// prints the data into a terminal
	void print(void)
	{
		cout << "t: " << _t << "\tp: " << _p.x << ", " << _p.y << ", " << _p.z << "\tq: " << _q.x << ", " << _q.y << ", " << _q.z << ", " << _q.w << endl;
	}

}Keyframe;

/*
Type for the keyframe animation
*/
typedef map<double, Keyframe> KeyframeAnimation;

// Variable to store the keyframes
KeyframeAnimation myKeyframes;

/*!
@brief returns the two keyframes for a given time.
@param keyframes - a map with all keyframes of type KeyframeAnimation
@param time - the time fraction between 0 and 1.
@param k0, reference to the first keyframe
@param k2, reference to the second keyframe
@return the number of keyframes. 1 if the time is equal to a keyframe, otherwise 2.
*/
int getKeyframes(KeyframeAnimation& keyframes, const double time, Keyframe& k0, Keyframe& k1)
{
	int num_keyframes = 0;

	// get a keyframe iterator
	KeyframeAnimation::iterator k_itr = keyframes.lower_bound(time);

	Keyframe k0_temp, k1_temp;

	// Obtain the first keyframe
	k1 = (*k_itr).second; num_keyframes++;


	// Check whether we are not at the beginning of this map
	if (k_itr != keyframes.begin())
	{
		k_itr--;  // decrement
		k0 = (*k_itr).second; // obtain the second keyframe
		num_keyframes++;
	}

	// write the first keyframe into k0 if we only have one
	if (num_keyframes == 1)
	{
		k0 = k1;
	}

	return num_keyframes;

}

/*!
@brief Interpolate between two keyframes
@param fraction - the time fraction for the interpolation / the location between two keyframes.
The value must be between 0 and 1.
@param k0, the start keyframe
@param k1, the end keyframe,
@param res, reference to a variable for the result.
*/
bool interpolateKeyframe(const float fraction, const Keyframe& k0, const Keyframe& k1, Keyframe& res)
{
	/////////////////////////////////////////////////////////////////////////
	// 1. Check the time delta

	// delta time
	float delta_t = k1._t - k0._t;

	// Check whether we have a delta time. Otherwise, we are at the location of exactly one keyframe
	if (delta_t == 0.0f) {
		res = k0;
		return true;
	}

	/////////////////////////////////////////////////////////////////////////
	// 2. Interpolat the position

	// get the delta
	glm::vec3 delta_p = k1._p - k0._p;

	// position interpolation
	glm::vec3 p_int = k0._p + delta_p * (fraction - k0._t) / (delta_t);


	/////////////////////////////////////////////////////////////////////////
	// 3. Rotation interpolation

	// Calculate the distance between the target angle and the current angle.
	float delta_angle = sqrt((k1._q.x - k0._q.x)*(k1._q.x - k0._q.x) +
		(k1._q.y - k0._q.y)*(k1._q.y - k0._q.y) +
		(k1._q.z - k0._q.z)*(k1._q.z - k0._q.z) +
		(k1._q.w - k0._q.w)*(k1._q.w - k0._q.w));


	// Linear interpolation of the rotation using slerp
	glm::quat r_int = glm::slerp(k0._q, k1._q, (fraction - k0._t) / (delta_t));


	/////////////////////////////////////////////////////////////////////////
	// 4. Write the result
	res = Keyframe(fraction, p_int, r_int);

	return true;
}

/*!
This initializes the keyframes.
*/
void initKeyframeAnimation(void)
{
	myKeyframes[0.0] = Keyframe(0.0, glm::vec3(0.0, 0.0, 0.0), angleAxis(0.0f, glm::vec3(0.0, 0.0, 1.0)));
	myKeyframes[0.5] = Keyframe(0.5, glm::vec3(0.5, 0.0, 0.0), angleAxis(0.57f, glm::vec3(0.0, 0.0, 1.0)));
	myKeyframes[0.7] = Keyframe(0.7, glm::vec3(0.7, 0.5, 0.0), angleAxis(1.28f, glm::vec3(0.0, 0.0, 1.0)));
	myKeyframes[0.8] = Keyframe(0.8, glm::vec3(0.35, 0.7, 0.0), angleAxis(1.53f, glm::vec3(0.0, 0.0, 1.0)));
	myKeyframes[1.0] = Keyframe(1.0, glm::vec3(0.2, 0.9, 0.0), angleAxis(1.98f, glm::vec3(0.0, 0.1, 1.0)));
}

#pragma endregion


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

		// SetViewAsLookAt(glm::vec3(0.0f, 0.0f, 65.5f), glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		
	}
	else if (key == 50 && action == GLFW_PRESS) // 2
	{
		cout << "key 2 pressed" << endl;
		if (ShowPlane5)
		{
			ShowPlane5 = false;
		}
		else {
			ShowPlane5 = true;
		}
	}
	else if (key == 51 && action == GLFW_PRESS) // 3
	{
		cout << "key 3 pressed" << endl;
		if (ShowPlane6)
		{
			ShowPlane6 = false;
		}
		else {
			ShowPlane6 = true;
		}
	}
	else if (key == 52 && action == GLFW_PRESS) // 4
	{
		cout << "key 4 pressed" << endl;
		if (ShowPlane5)
		{
			ShowPlane5 = false;
		}
		else {
			ShowPlane5 = true;
		}
	}

	else if (key == 321 && action == GLFW_PRESS) // Numpad 1
	{
		cout << "key Numpad 1 (" << key << ") pressed" << endl;
		if (ShowPlane4)
		{
			ShowPlane4 = false;
		}
		else {
			ShowPlane4 = true;
		}
	}

	else if (key == 322 && action == GLFW_PRESS) // Numpad 2
	{
		cout << "key Numpad 2 (" << key << ") pressed" << endl;
		if (ShowPlane5)
		{
			ShowPlane5 = false;
		}
		else {
			ShowPlane5 = true;
		}
	}

	else if (key == 323 && action == GLFW_PRESS) // Numpad 3
	{
		cout << "key Numpad 3 (" << key << ") pressed" << endl;
		if (ShowPlane6)
		{
			ShowPlane6 = false;
		}
		else {
			ShowPlane6 = true;
		}
	}
	else {
		cout << key << endl;
	}

	// g_change_texture_blend++;
	// g_change_texture_blend = g_change_texture_blend % 3;


	////////////////////////////////////////////////////////////////////////////////////////////
	// Translation
	if ((key == 87 && action == GLFW_REPEAT) || (key == 87 && action == GLFW_PRESS)) // key w
	{
		

		keyframefraction += 0.01;

		cout << "key w pressed.  keyframefraction = " << keyframefraction << endl;

		glm::mat4 w_tranform = glm::translate(glm::vec3(0.1, 0.1f, 0.1f)) * glm::scale(glm::vec3(5.0 + 5 * keyframefraction, 5.0 + 5 * keyframefraction, 5.0 + 5 * keyframefraction));
		
		

		// loadedModel0 = new GLObjectObj("../class_project_models/0.obj");
		// loadedModel0->setApperance(*apperance_0);
		// loadedModel0->init();
		loadedModel0->setMatrix(w_tranform);

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
		case 10:
			setSelectColor(plane4->getProgram(), false);
			g_selected_object_id = 0;
			break;
		case 12:
			setSelectColor(plane5->getProgram(), false);
			g_selected_object_id = 0;
			break;
		case 14:
			setSelectColor(plane6->getProgram(), false);
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
		setSelectColor(plane4->getProgram(), true);
		g_selected_object_id = selected_object_id;
		break;
	case 12:
		setSelectColor(plane5->getProgram(), true);
		g_selected_object_id = selected_object_id;
		break;
	case 14:
		setSelectColor(plane6->getProgram(), true);
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
    apperance_0->addLightSource(light_source);
	// apperance_0->addLightSource(light_source2);
	apperance_0->addLightSource(origin_light);
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
    
    loadedModel0 = new GLObjectObj("../class_project_models/0.obj");
	loadedModel0->setApperance(*apperance_0);
	loadedModel0->init();
	loadedModel0->setMatrix(tranform_0);
        
    loadedModel1 = new GLObjectObj("../class_project_models/1.obj");
	glm::mat4 tranform_1 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));

    loadedModel1->setApperance(*apperance_1);
    loadedModel1->init();
	loadedModel1->setMatrix(tranform_1);

	glm::mat4 tranform_2 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
	loadedModel2 = new GLObjectObj("../class_project_models/2.obj");
	loadedModel2->setApperance(*apperance_2);
	loadedModel2->init();
	loadedModel2->setMatrix(tranform_2);

	glm::mat4 tranform_3 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
	loadedModel3 = new GLObjectObj("../class_project_models/3.obj");
	loadedModel3->setApperance(*apperance_3);
	loadedModel3->init();
	loadedModel3->setMatrix(tranform_3);
    
	glm::mat4 tranform_4 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
	plane4= new GLPlane3D(0.0, 0.0, -2.0, 5.0, 5.0);
	plane4->setApperance(*apperance_4);
	plane4->init();
	plane4->setMatrix(tranform_4);

	glm::mat4 tranform_5 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
	plane5 = new GLPlane3D(0.0, 0.0, 0.0, 5.0, 5.0);
	plane5->setApperance(*apperance_5);
	plane5->init();
	plane5->setMatrix(tranform_5);

	glm::mat4 tranform_6 = glm::translate(glm::vec3(0.0, 0.0f, 0.0f)) * glm::scale(glm::vec3(5.0, 5.0f, 5.0f));
	plane6 = new GLPlane3D(0.0, 0.0, 2.0, 5.0, 5.0);
	plane6->setApperance(*apperance_6);
	plane6->init();
	plane6->setMatrix(tranform_6);

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
	//// Here we set a new keyboard callback

	// Set the keyboard callback so that when we press ESC, it knows what to do.
	glfwSetKeyCallback(window, keyboard_callback);

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
		plane4->draw();
		#pragma endregion


		#pragma region Shape 5
		glUniform1i(l5, false); // and switch to regular mode.
		glUseProgram(apperance_5->getProgram());
		glUniform1i(l5, true);

		// render
		plane5->draw();
		#pragma endregion


#pragma region Shape 6
		glUniform1i(l6, false); // and switch to regular mode.
		glUseProgram(apperance_6->getProgram());
		glUniform1i(l6, true);

		// render
		plane6->draw();
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
        // cout << "COLOR:\t" << col[0] << "\t" << col[1] << "\t" << col[2]  << "\t" << col[3] << endl;
        
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
		
		// Render planes
		if (ShowPlane4)
		{
			plane4->draw();
		}

		if (ShowPlane5)
		{
			plane5->draw();
		}
		if (ShowPlane6)
		{
			plane6->draw();
		}

		// plane4->draw();
		// plane5->draw();
		// plane6->draw();
		
		
       
       
        //// This renders the objects
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    delete cs;


}

