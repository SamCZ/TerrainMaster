#include <iostream>
#include <memory>
#include <Windows.h>


#include "TerrainMaster/Render/Mesh/Mesh.h"
#include "TerrainMaster/Utils/Buffers/Buffers.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TerrainMaster/Window/Display.h"
#include "TerrainMaster/Render/Renderer.h"

#include "TerrainMaster/Utils/Timing.h"

#include "TerrainMaster/Scene/Camera.h"
#include "TerrainMaster/Control/FPSControl.h"

TM::MeshPtr MakeTerrainMesh(int lod, int leftLod, int rightLod, int topLod, int bottomLod) {
	std::vector<glm::vec3> verts;
	std::vector<int> inds;

	int vertexCount = 0;
	if (lod == 0) {
		vertexCount = 8;
	}
	else if(lod == 1) {
		vertexCount = 4;
	}
	else {
		vertexCount = 2;
	}

	vertexCount = 64 * 2;

	float size = 256.0f * 4;
	float segmentSize = size / vertexCount;
	vertexCount++;

	int offset = lod + 1;

	for (int j = 0; j < vertexCount; j++) {
		for (int i = 0; i < vertexCount; i++) {
			float x = i * segmentSize;
			float z = j * segmentSize;

			verts.push_back(glm::vec3(x, 0, z));

			int ii = i * offset;
			int jj = j * offset;

			if (ii < vertexCount-1 && jj < vertexCount-1) {
				inds.push_back(ii + jj * vertexCount);
				inds.push_back(ii + (jj + offset) * vertexCount);
				inds.push_back((ii + offset) + (jj + offset) * vertexCount);

				inds.push_back(ii + jj * vertexCount);
				inds.push_back((ii + offset) + (jj + offset) * vertexCount);
				inds.push_back((ii + offset) + jj * vertexCount);
			}
		}
	}

	

	TM::MeshPtr mesh = MakeShared<TM::Mesh>();
	mesh->SetData(TM::MeshBuffer::Type::Position, 3, TM::CreateFloatBuffer(verts));
	mesh->SetData(TM::MeshBuffer::Type::Index, 3, TM::CreateIntBuffer(inds));
	mesh->SetMode(TM::Mode::Triangles);
	

	return mesh;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);

	TM::Display display;
	display.Create();

	TM::Camera cam(display.GetWidth(), display.GetHeight());
	cam.SetFrustumPerspective(60.0f, (float)display.GetWidth() / (float)display.GetHeight(), 0.1f, 10000.0f);

	TM::Renderer r;

	/*TM::MeshPtr mesh = MakeShared<TM::Mesh>(); {

		TM::FloatBufferPtr fb = TM::CreateFloatBuffer(new float[3*4]{ 
			0, 0, 0,
			0, 1, 0,
			1, 1, 0,
			1, 0, 0
		}, 3*4);
		mesh->SetData(TM::MeshBuffer::Type::Position, 3, fb);

		TM::IntBufferPtr ib = TM::CreateIntBuffer(new int[6]{
			0, 1, 2,
			0, 2, 3
		}, 6);

		mesh->SetData(TM::MeshBuffer::Type::Index, 3, ib);
	}*/

	TM::MeshPtr leftMesh = MakeTerrainMesh(0, 0, 0, 0, 0);
	TM::MeshPtr rightMesh = MakeTerrainMesh(1, 0, 0, 0, 0);
	TM::MeshPtr rightRightMesh = MakeTerrainMesh(3, 1, 0, 0, 0);

	//glm::mat4 projection = glm::ortho(0.0f, (float)display.GetWidth(), (float)display.GetHeight(), 0.0f, -1.0f, 1.0f);
	//glm::mat4 view = glm::mat4();

	TM::ShaderPtr shader = MakeShared<TM::Shader>();
	shader->AddShader(TM::ProgramType::Vertex, "", "Assets/Shaders/shader.vert");
	shader->AddShader(TM::ProgramType::Geometry, "", "Assets/Shaders/shader.geom");
	shader->AddShader(TM::ProgramType::Fragment, "", "Assets/Shaders/shader.frag");
	shader->Compile();

	uint32 fps = 0;
	double lastTime = TM::Time::getTime();
	double fpsTimeCounter = 0.0;
	double updateTimer = 1.0;
	float frameTime = 1.0 / 60.0;

	float f = 0;

	while (display.IsRunning()) {
		double currentTime = TM::Time::getTime();
		double passedTime = currentTime - lastTime;
		lastTime = currentTime;

		fpsTimeCounter += passedTime;
		updateTimer += passedTime;

		if (fpsTimeCounter >= 1.0) {
			double msPerFrame = 1000.0 / (double)fps;
			DEBUG_LOG("FPS", "NONE", "%f ms (%d fps)", msPerFrame, fps);
			fpsTimeCounter = 0;
			fps = 0;
		}

		display.UpdateEvents();

		bool shouldRender = false;
		while (updateTimer >= frameTime) {
			//Update game

			cam.SetLocation(0, 100, 500);
			cam.SetRotation(-20, -45, 0);
			cam.Update();

			updateTimer -= frameTime;
			shouldRender = true;
		}

		TM::RenderState rs;
		rs.Wireframe = false;
		rs.LineWidth = 1.5f;
		rs.FaceCull = TM::FaceCull::Off;


		if (shouldRender) {
			//Render game

			r.ClearColor(0.0, 0.0, 0.0, 1.0);
			r.ClearScreen(true, true, false);

			r.ApplyRenderState(rs);
			glPointSize(1.5f);

			r.SetShader(shader);
			shader->SetUniform1f("_Time", f += 0.05f);

			shader->SetUniformMatrix4fv("ProjectionMatrix", glm::value_ptr(cam.GetProjectionMatrix()));
			shader->SetUniformMatrix4fv("ViewMatrix", glm::value_ptr(cam.GetViewMatrix()));

			glm::mat4 model = glm::translate(glm::vec3(0, 0, 0));
			shader->SetUniformMatrix4fv("ModelMatrix", glm::value_ptr(model));
			r.RenderMesh(leftMesh, 0);

			/*model = glm::translate(glm::vec3(100 + 256, 100, 0));
			shader->SetUniformMatrix4fv("ModelMatrix", glm::value_ptr(model));
			r.RenderMesh(rightMesh, 0);

			model = glm::translate(glm::vec3(100 + 256 * 2, 100, 0));
			shader->SetUniformMatrix4fv("ModelMatrix", glm::value_ptr(model));
			r.RenderMesh(rightRightMesh, 0);*/

			display.Update();

			fps++;
		} else {
			TM::Time::sleep(1);
		}

		
	}

	return 0;
}