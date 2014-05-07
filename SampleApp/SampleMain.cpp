#include <windows.h>

#include "engine-renderer/RenderableObject.h"
#include "engine-renderer/RenderingEngine.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	MessageBoxA(0, "You compiled fine :D", "", MB_OK);
	return 0;

	RenderableWorld *world = new RenderableWorld();
	ConstructorTable<IHasHandle> *ctors = new ConstructorTable<IHasHandle>(10);
	RenderingEngine *engineInstance = new RenderingEngine(world, ctors, hInstance);

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
		engineInstance->createModelFromIndex(ecoliIndex, textureIndex)
	);

	engineInstance->run();

	delete engineInstance;
	delete ctors;
	delete world;
}

