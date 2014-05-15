#pragma once

#include "renderer/VertexBuffer.h"
#include "renderer/IndexBuffer.h"

struct ModelData {
	Transmission::VertexBuffer *vertexBuffer;
	Transmission::IndexBuffer *indexBuffer;
};