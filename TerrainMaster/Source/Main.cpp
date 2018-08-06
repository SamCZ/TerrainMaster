#include <iostream>
#include <memory>

#include "TerrainMaster/Render/Mesh/Mesh.h"
#include "TerrainMaster/Utils/Buffers/Buffers.h"

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	TM::MeshPtr mesh = MakeShared<TM::Mesh>();
	TM::FloatBufferPtr fb = TM::CreateFloatBuffer(new float[2] { 1, 2 }, 2);
	mesh->SetData(TM::MeshBuffer::Type::Position, 2, fb);

	return 0;
}