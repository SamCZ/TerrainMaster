#pragma once

#include "SharedPtr.h"
#include <GL/glew.h>

namespace TM {
	class NativeObject {
	public:
		bool IsNeedsUpdate() const;
		void MarkClean();
		void MarkUpdate();

		GLuint GetId();
		void SetId(GLuint id);
	protected:
		bool _needsUpdate = true;
		GLuint _id = -1;
	};
}