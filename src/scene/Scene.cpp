#include "ShaderManager.hpp"
#include "DefaultShader.hpp"
#include "Scene.hpp"


static DrawingProgramRegistrator<DefaultShader> shaderReg("background");


Scene::Scene(const VisualSettings& visSettings)
	: skyBox("{assets}/skybox.obj")
{
	lightCmd.shaderName = "LightHandler";
	lightCmd.light.material = *visSettings.getMaterial("Light0");
	lightCmd.light.pos4 = {1, 1, 1, 0};

	skyBox.setShader("background");
}


void Scene::draw(DrawingQueue& drawing)
{
	drawing.add(&lightCmd);
	skyBox.draw(drawing);
}
