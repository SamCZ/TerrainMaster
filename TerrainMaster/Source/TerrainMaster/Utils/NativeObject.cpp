#include "NativeObject.h"

namespace TM {
	bool NativeObject::IsNeedsUpdate() const {
		return _needsUpdate;
	}
	void NativeObject::MarkClean() {
		_needsUpdate = false;
	}
	void NativeObject::MarkUpdate() {
		_needsUpdate = true;
	}

	GLuint NativeObject::GetId() {
		return _id;
	}

	void NativeObject::SetId(GLuint id) {
		_id = id;
	}
}