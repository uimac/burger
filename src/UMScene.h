/// UMScene.h ///
#pragma once

#include <memory>
#include <vector>
#include "UMVector.h"
#include "UMPrimitive.h"

namespace burger
{

class UMScene;
typedef std::shared_ptr<UMScene> UMScenePtr;

/**
 * 3d scene including many objects, lights, cameras, ...
 */
class UMScene
{
public:
	UMScene() {}
	~UMScene() {}
	
	typedef std::vector<UMPrimitivePtr> Primitives;

	const Primitives& primitives() const { return primitives_; }
	Primitives& mutable_primitives() { return primitives_; }

	UMVec3d background_color() const { return UMVec3d(0.1, 0.1, 0.1); }

private:
	Primitives primitives_;
};

} // burger
