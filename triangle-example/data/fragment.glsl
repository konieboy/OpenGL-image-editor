#version 410

uniform sampler2D texture1;
in vec2 fragPosition;
in vec2 fragGrayScale;
in vec2 frag2Bit;

out vec3 FragmentColour;

void main() {


  // Check if gray scale mode is on
  
  if (fragGrayScale[0] != 0)
  {
    float intensity = dot (FragmentColour = texture(texture1, fragPosition).rgb, vec3(0.33333,0.33333,0.33333));
    
    FragmentColour = vec3 (intensity,intensity,intensity);
  }
  else
  {
    FragmentColour = texture(texture1, fragPosition).rgb;
  }

  if (frag2Bit[0] != 0)
  {
    // round to nearest 1/4
    for (int i = 0 ; i < 3; i++)
    {
      FragmentColour[i] = round(FragmentColour[i]*4)/4;
    }
  }

  //FragmentColour = vec4(1,0,0,1);
}
