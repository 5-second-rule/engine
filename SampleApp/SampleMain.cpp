#include <windows.h>

#include "engine-renderer/IRenderable.h"
#include "engine-core/IHasHandle.h"
#include "engine-renderer/RenderingEngineInstance.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//static int reduceSpam = 1;

//void moveBlob(Window* w, Model* m) {
//	float moveAmt = 1 / 1000.0;
//	Input* inp = (Input*) w->getInput();
//
//	if (inp->keys[Input::KEY_W] == Input::STATE_DOWN) {
//		m->move(Vector4(0, moveAmt, 0));
//	}
//	if (inp->keys[Input::KEY_A] == Input::STATE_DOWN) {
//		m->move(Vector4(-moveAmt, 0, 0));
//	}
//	if (inp->keys[Input::KEY_S] == Input::STATE_DOWN) {
//		m->move(Vector4(0, -moveAmt, 0));
//	}
//	if (inp->keys[Input::KEY_D] == Input::STATE_DOWN) {
//		m->move(Vector4(moveAmt, 0, 0));
//	}	
//	if (inp->keys[Input::KEY_SP] == Input::STATE_DOWN) {
//		m->move(Vector4(0, 0, moveAmt));
//	}
//}

class TestObject : public IRenderable, public IHasHandle
{
private:
	VertexBuffer* vbuf;
	IndexBuffer* ibuf;
	Model *model;
	Handle handle;

public:
	TestObject(RenderingEngineInstance *engine) {
		char* whiteBloodFbxFilePath = "../SampleApp/whitey.fbx";
		char* ecoliFbxFilePath = "../SampleApp/ecoli4_animated_binary.fbx";
		char* ecoliObjFilePath = "../SampleApp/Ecoli4_Object.obj";

		Model* model = engine->createModelFromFile(ecoliFbxFilePath, &vbuf, &ibuf);
	};

	~TestObject() {
		delete model;
	};

	virtual Handle getHandle(){
		return this->handle;
	};

	virtual void setHandle(Handle handle){
		this->handle = handle;
	};

	virtual void render() {
		this->model->draw();
	};
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	RenderableWorld *world = new RenderableWorld();
	ObjectCtorTable *ctors = new ObjectCtorTable(10);
	RenderingEngineInstance *engineInstance = new RenderingEngineInstance(world, ctors, hInstance);

	TestObject *object = new TestObject(engineInstance);
	world->allocateHandle(object, HandleType::LOCAL);
	world->insert(object);

	engineInstance->run();

	delete engineInstance;
	delete ctors;
	delete world;

	/*
	// FBX Load
	FBXLoader fileLoaded;

	VertexBuffer* vbuf;
	IndexBuffer* ibuf;
	char* filePath = "../SampleApp/whitey.fbx";

	fileLoaded.loadFBXFile(filePath, &vbuf, &ibuf, renderer);
	*/

	/*VertexBuffer* vbuf;// = renderer->createVertexBuffer(cube, 8);
	IndexBuffer* ibuf;// = renderer->createIndexBuffer(index, 36);
	//Model* model = renderer->createModel(vbuf, ibuf);

	//model->move(Vector4(0, 0, 10));

	char* whiteBloodFbxFilePath = "../SampleApp/whitey.fbx";
	char* ecoliFbxFilePath = "../SampleApp/ecoli4_animated_binary.fbx";
	char* ecoliObjFilePath = "../SampleApp/Ecoli4_Object.obj";

	Model* model = renderer->createModelFromFile(ecoliFbxFilePath, &vbuf, &ibuf);

	while (messagePump(window)) {
		renderer->clearFrame();

		model->draw();

		renderer->drawFrame();

		moveBlob(window, model); // temp input handler
	}

	delete model;
	delete vbuf;
	delete ibuf;*/

	//delete renderer;
	//delete window;
}

