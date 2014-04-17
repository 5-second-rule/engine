#pragma once

#include "engine-renderer.h"

class RENDERDLL IRenderable {
public:
	virtual void render() = 0;
};