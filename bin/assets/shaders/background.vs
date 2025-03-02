#version 330 core


uniform mat4 uModelMatrix;
uniform mat4 uProjectionMatrix;

attribute vec4 aPosition;
attribute vec2 aTexCoord;
varying vec2 vTexCoord;
varying vec3 vModelPos;

void main()
{
   vec4 pos = vec4(aPosition.xyz, 1);
   vModelPos = vec3(pos * uModelMatrix);
   vTexCoord = aTexCoord;
   gl_Position = pos * uModelMatrix * uProjectionMatrix;
}
