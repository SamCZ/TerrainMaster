#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace TM {
	class Spatial {
	public:
		Spatial();
		Spatial(std::string name);
		~Spatial();

		void SetLocation(const glm::vec3& location);
		void SetRotation(const glm::vec3& rotation);
		void SetScale(const glm::vec3& scale);

		void SetLocation(float x, float y, float z);
		virtual void SetRotation(float x, float y, float z);
		void SetScale(float x, float y, float z);

		glm::vec3& GetLocation();
		glm::vec3& GetRotation();
		glm::vec3& GetScale();

		virtual void Update();

		void SetName(std::string name);
		std::string GetName() const;
	protected:
		std::string _name;
		glm::vec3 _location;
		glm::vec3 _rotation;
		glm::vec3 _scale;
	};
}