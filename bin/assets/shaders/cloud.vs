#version 330 core

uniform mat4 uModelMatrix;
uniform mat4 uProjectionMatrix;
uniform vec4 uColor;
attribute vec4 aColor;
attribute vec4 aPosition;
varying vec3 vModelPos;
varying vec4 vColor;

void main()
{
   vec4 pos = vec4(aPosition.xyz, 1);
   vModelPos = vec3(pos * uModelMatrix);
   gl_Position = pos * uModelMatrix * uProjectionMatrix;

   vColor = aColor;

   //float z = vModelPos.z;
   //if(-7.0 < z)
   //   vColor = vec4(0,1,0, 1);
   //else if(-14.0 < z)
   //   vColor = vec4(1,0,0, 1);
   //else
   //   vColor = vec4(1,1,1, 1);
   //vColor = vec4(aColor.xyz * uColor.xyz, max(0, min(uColor.w / ((-z/7)*(-z/7)*(-z/7)), 1)));
}
