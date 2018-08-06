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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	TM::Display display;
	display.Create();

	TM::Renderer r;

	TM::MeshPtr mesh = MakeShared<TM::Mesh>(); {

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
	}

	glm::mat4 projection = glm::ortho(0.0f, (float)display.GetWidth(), (float)display.GetHeight(), 0.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::mat4();
	glm::mat4 model = glm::scale(glm::vec3(100, 100, 100));

	TM::ShaderPtr shader = MakeShared<TM::Shader>();
	shader->AddShader(TM::ProgramType::Vertex, "", "Assets/Shaders/shader.vert");
	shader->AddShader(TM::ProgramType::Fragment, "", "Assets/Shaders/shader.frag");
	shader->Compile();

	while (display.IsRunning()) {
		display.UpdateEvents();

		r.ClearColor(1.0, 0.5, 1.0, 1.0);
		r.ClearScreen(true, true, false);

		r.SetShader(shader);
		shader->SetUniformMatrix4fv("ProjectionMatrix", glm::value_ptr(projection));
		shader->SetUniformMatrix4fv("ViewMatrix", glm::value_ptr(view));
		shader->SetUniformMatrix4fv("ModelMatrix", glm::value_ptr(model));

		r.RenderMesh(mesh, 0);

		display.Update();
	}

	return 0;
}