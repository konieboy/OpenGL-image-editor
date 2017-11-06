#version 410

layout(location = 0) in vec2 position;
layout(location = 1) in float scale;
layout(location = 2) in vec2 grayscale;
layout(location = 3) in vec2 xShift;
layout(location = 4) in vec2 yShift;
layout(location = 5) in vec2 frag2BitV;

out vec2 fragPosition;
out vec2 fragGrayScale;
out vec2 frag2Bit;

void main() {

  //gl_Position = vec4(position[0] + 0.5, position[1] + 0.5, 0, 2.0);

  gl_Position = vec4(position[0]+ xShift[0], position[1] + yShift[0], 0, scale);
  fragPosition = ((position + vec2(1,1)) * vec2(.5,.5));
  frag2Bit = frag2BitV;
  fragGrayScale = grayscale;
}
