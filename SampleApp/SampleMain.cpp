#include <windows.h>

#include "engine-renderer/RenderableObject.h"
#include "engine-renderer/RenderingEngineInstance.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	RenderableWorld *world = new RenderableWorld();
	ObjectCtorTable *ctors = new ObjectCtorTable(10);
	RenderingEngineInstance *engineInstance = new RenderingEngineInstance(world, ctors, hInstance);

	char* whiteBloodFbxFilePath = "../SampleApp/whitey.fbx";
	char* ecoliFbxFilePath = "../SampleApp/ecoli6_nomedia.fbx";
	char* ecoliObjFilePath = "../SampleApp/Ecoli6_obj.obj";
	char* boxFbxFilePath = "../SampleApp/cube.fbx";

	char* textureLocation = "../SampleApp/ecoli6_TXTR.dds";
	char* textureLocationW = "../SampleApp/Wood.dds";
	char* whiteTexture = "../SampleApp/whitebloodcell_3_TXTR.dds";
	char* cubeTexture = "../SampleApp/cube_uvmap2.dds";

	int ecoliIndex = engineInstance->loadModel(ecoliFbxFilePath);
	int textureIndex = engineInstance->loadTexture(textureLocationW);

	RenderableObject *object = new RenderableObject(
		0,
		engineInstance->createModelFromIndex(ecoliIndex, textureIndex));

	world->allocateHandle(object, HandleType::LOCAL);
	world->insert(object);

	engineInstance->run();

	delete engineInstance;
	delete ctors;
	delete world;
}

