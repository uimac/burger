/**
 * @file UMMain.cpp
 * entry point
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMTga.h"

#include "UMScene.h"
#include "UMRenderParameter.h"
#include "UMRayTracer.h"

#include <iostream>

using namespace burger;

int main()
{
	const int width = 640;
	const int height = 640;
	const int wh = width * height;

	UMScene scene(width, height);
	scene.create_sample_scene_1();

	UMRendererPtr renderer(std::make_shared<UMRayTracer>());
	UMRenderParameter parameter;
	renderer->init();
	renderer->set_width(width);
	renderer->set_height(height);
	renderer->render(scene, parameter);

	if (!parameter.output_image().is_valid()) {
		return -1;
	}

	UMTga tga;
	tga.save("hoge.tga", parameter.output_image());

	return 0;
}