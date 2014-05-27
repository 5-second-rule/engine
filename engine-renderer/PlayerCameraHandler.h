#pragma once

#include "engine-renderer.h"
#include "engine-core/IHasHandle.h" 
#include "common/Vector4.h"

class RENDERDLL PlayerCameraHandler
{
public:
	Common::Vector4 position;
	Common::Vector4 lookAt;
	Common::Vector4 up;

	PlayerCameraHandler();
	~PlayerCameraHandler();

	virtual void updateFor(IHasHandle *playerObject) = 0;
};

