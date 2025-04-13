#version 330 core


uniform mat4 uModelMatrix;
uniform mat4 uProjectionMatrix;

attribute vec4 aPosition;
attribute vec2 aTexCoord;
varying vec2 vTexCoord;

void main()
{
   vec4 pos = vec4(aPosition.xyz, 1);
   vTexCoord = aTexCoord;
   gl_Position = pos * uModelMatrix * uProjectionMatrix;
}
