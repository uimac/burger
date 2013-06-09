/// UMMain.cpp ///

///#include "bmpexporter.h"

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
	UMSpherePtr sphere1(new UMSphere(-20.0, 250.0));
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

	if (static_cast<int>(parameter.output_image().size()) != wh) {
		return -1;
	}

	// floating image to 8bit rgb
	const double inv_gamma = 1.0 / 22.0;
	std::vector<unsigned char> img;
	img.resize(width * height * 3);
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int pos = width * y + x;
			UMVec3d col = parameter.output_image().at(pos);
			img[pos * 3 + 0] = static_cast<int>(pow(col.z, inv_gamma) * 0xFF);
			img[pos * 3 + 1] = static_cast<int>(pow(col.y, inv_gamma) * 0xFF);
			img[pos * 3 + 2] = static_cast<int>(pow(col.x, inv_gamma) * 0xFF);
		}
	}
	
	// TODO
	//exportToBmp("hoge.bmp", reinterpret_cast<unsigned char*>(&(*img.begin())), width, height);

	return 0;
}