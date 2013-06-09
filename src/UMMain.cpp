/// UMMain.cpp ///

#include "UMTga.h"

#include "UMVector.h"
#include "UMPrimitive.h"
#include "UMScene.h"
#include "UMSphere.h"
#include "UMPlane.h"
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
	UMRenderParameter parameter;
	UMSpherePtr sphere1(new UMSphere(UMVec3d(0, -100, 0), 250.0));
	UMSpherePtr sphere2(new UMSphere(UMVec3d(200, 0, 0), 200.0));
	sphere1->set_color(UMVec3d(1.0, 0.0, 0.0));
	sphere2->set_color(UMVec3d(0.0, 0.0, 1.0));
	scene.mutable_primitives().push_back(UMPrimitivePtr(sphere1));
	scene.mutable_primitives().push_back(UMPrimitivePtr(sphere2));

	UMRendererPtr renderer(new UMRaytracer);
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