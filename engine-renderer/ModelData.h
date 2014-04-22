#pragma once

#include "renderer/VertexBuffer.h"
#include "renderer/IndexBuffer.h"

struct ModelData {
	VertexBuffer *vertexBuffer;
	IndexBuffer *indexBuffer;
};