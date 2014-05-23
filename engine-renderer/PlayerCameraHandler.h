#pragma once

#include "engine-renderer.h"
#include "engine-core/IHasHandle.h" 
#include "common/Vector4.h"

class RENDERDLL PlayerCameraHandler
{
public:
	Common::Vector4 position;
	Common::Vector4 lookAt;

	PlayerCameraHandler();
	~PlayerCameraHandler();

	virtual void updateFor(IHasHandle *playerObject) = 0;
};

