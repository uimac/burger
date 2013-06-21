/**
 * @file UMMain.cpp
 * entry point
 */
#include "UMTga.h"

#include "UMScene.h"
#include "UMRenderParameter.h"
#include "UMRaytracer.h"

#include <iostream>

using namespace burger;

int main()
{
	const int width = 640;
	const int height = 640;
	const int wh = width * height;

	UMScene scene;
	scene.create_sample_scene_1();

	UMRendererPtr renderer(std::make_shared<UMRaytracer>());
	UMRenderParameter parameter;
	renderer->init();
	renderer->set_width(width);
	renderer->set_height(height);
	renderer->render(scene, parameter);

	if (!parameter.output_image().is_validate()) {
		return -1;
	}

	UMTga tga;
	tga.save("hoge.tga", parameter.output_image());

	return 0;
}