#version 410 core                                                 

uniform sampler2D texture_sunset; //this is the texture
// uniform sampler2D texture_face; //this is the texture
// uniform sampler2D texture_colorwheel; //this is the NEW texture DAVID CODE

in vec2 pass_TexCoord; //this is the texture coord
in vec4 pass_Color;

out vec4 color;

uniform int texture_blend;

void main(void)                                                   
{
    // This function finds the color component for each texture coordinate. 

	vec4 tex_sunset =  texture(texture_sunset, pass_TexCoord);
    
    // vec4 tex_colorwheel =  texture(texture_colorwheel, pass_TexCoord);

    // vec4 tex_face =  texture(texture_face, pass_TexCoord);
    
    // This mixes the background color with the texture color.
    // The GLSL shader code replaces the former envrionment. It is now up to us
    // to figure out how we like to blend a texture with the background color.
    if(texture_blend == 0)
    {
        // color = 0.0 * pass_Color + tex_sunset * 0 + tex_face + tex_colorwheel ;

        // color = (tex_face * tex_face + tex_colorwheel * tex_colorwheel + tex_sunset * tex_sunset) / (tex_face + tex_colorwheel + tex_sunset);
        color = tex_sunset;
    }
    else if(texture_blend == 1)
    {
        // color = 0.0 * pass_Color + tex_sunset * 0 + tex_face + tex_colorwheel ;

        // color = (tex_face + tex_colorwheel + 2 * tex_sunset) / 4; 
        color = tex_sunset;
    }
    else if(texture_blend == 2)
    {
        // color = 0.0 * pass_Color + tex_sunset * 0 + tex_face + tex_colorwheel ;
        // color = (tex_face + tex_colorwheel + tex_sunset) / 3;
        color = tex_sunset;
    }
    /*else if(texture_blend == 3)
    {
        // color = 0.0 * pass_Color + tex_sunset * 0 + tex_face + tex_colorwheel ;
        color = (tex_sunset + tex_colorwheel) / 2;
    }
    else if(texture_blend == 4)
    {
        // color = 0.0 * pass_Color + tex_sunset * 0 + tex_face + tex_colorwheel ;
        color = (tex_colorwheel + tex_face) / 2;
    }
    else if(texture_blend == 5)
    {
        // color = 0.0 * pass_Color + tex_sunset * 0 + tex_face + tex_colorwheel ;
        color = (tex_face + tex_sunset) / 2;
    }
    else if(texture_blend == 6)
    {
        // color = 0.0 * pass_Color + tex_sunset * 0 + tex_face + tex_colorwheel ;
        color = (tex_sunset + tex_colorwheel + tex_face) / 3;
    }

    */
    else
    {
        // color = 0.0 * pass_Color + tex_sunset * 0 + tex_face + tex_colorwheel ;
        color = (tex_sunset + tex_face) / 2;
    }
    
}