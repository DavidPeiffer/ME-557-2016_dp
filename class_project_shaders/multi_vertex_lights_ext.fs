#version 410 core                                                 

// DAVID CODE
uniform sampler2D texture_background; //this is the texture
uniform sampler2D texture_foreground; //this is the texture

in vec2 pass_TexCoord; //this is the texture coord
// END DAVID CODE

in vec4 pass_Color;
out vec4 color;   

void main(void)                                                   
{                                                                 
	// DAVID CODE
	// This function finds the color component for each texture coordinate. 
    vec4 tex_color =  texture(texture_background, pass_TexCoord);
    
    vec4 tex_color_light =  texture(texture_foreground, pass_TexCoord);


    // Set the color to a combination of texture color and pass_color
    color = pass_Color + tex_color + tex_color_light ;


	// END DAVID CODE





    // color = vec4(pass_Color);
}                           