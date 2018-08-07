#include "Spatial.h"

namespace TM {
	Spatial::Spatial() : Spatial("Unknown") {

	}

	Spatial::Spatial(std::string name) : _location(0, 0, 0), _rotation(0, 0, 0), _scale(1, 1, 1) {
		_name = name;
	}

	Spatial::~Spatial() {
		
	}

	void Spatial::SetLocation(const glm::vec3 & location) {
		_location = location;
	}

	void Spatial::SetRotation(const glm::vec3 & rotation) {
		_rotation = rotation;
	}

	void Spatial::SetScale(const glm::vec3 & scale) {
		_scale = scale;
	}

	void Spatial::SetLocation(float x, float y, float z) {
		_location.x = x;
		_location.y = y;
		_location.z = z;
	}

	void Spatial::SetRotation(float x, float y, float z) {
		_rotation.x = x;
		_rotation.y = y;
		_rotation.z = z;
	}

	void Spatial::SetScale(float x, float y, float z) {
		_scale.x = x;
		_scale.y = y;
		_scale.z = z;
	}

	glm::vec3 & Spatial::GetLocation() {
		return _location;
	}

	glm::vec3 & Spatial::GetRotation() {
		return _rotation;
	}

	glm::vec3 & Spatial::GetScale() {
		return _scale;
	}

	void Spatial::Update() {

	}

	void Spatial::SetName(std::string name) {
		_name = name;
	}

	std::string Spatial::GetName() const {
		return _name;
	}
}