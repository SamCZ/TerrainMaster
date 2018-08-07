#pragma once

#include "Spatial.h"
#include "TerrainMaster/Collisions/Ray.h"

namespace TM {
	class Camera : public Spatial {
	private:
		static const int ORTHOGRAPHIC_MODE = 0;
		static const int PERSPECTIVE_MODE = 1;

		int _planeState;

		float frustumLeft;
		float frustumRight;
		float frustumBottom;
		float frustumTop;
		float frustumNear;
		float frustumFar;

		int _width;
		int _height;
		glm::mat4 _projectionMatrix;
		glm::mat4 _viewMatrix;
		glm::mat4 _viewProjectionMatrix;
		glm::mat4 _looAtMatrix;
		int _mode;
		float _fovY;
		float _zNear;
		float _zFar;
		bool _is2DOrtho;
	public:
		Camera(int width, int height);

		void SetProjectionMatrix(glm::mat4 m);
		void SetViewMatrix(glm::mat4 m);

		glm::mat4& GetProjectionMatrix();
		glm::mat4& GetViewMatrix();
		glm::mat4& GetProjectionViewMatrix();

		void SetFromFrustum(float left, float right, float bottom, float top, float zNear, float zFar);
		void SetFrustumPerspective(float fovY, float aspect, float zNear, float zFar);
		void Resize(int width, int height);

		void LookAt(glm::vec3 toPos, glm::vec3 eye);
		void LookAtDir(glm::vec3 dir);

		glm::vec3 GetWorldPosition(float x, float y, float projectionZPos) const;
		glm::vec3 GetScreenCoordinates(glm::vec3 pos);

		virtual void Update();

		int GetWidth() const;
		int GetHeight() const;

		float GetFrustumLeft();
		float GetFrustumRight();
		float GetFrustumBottom();
		float GetFrustumTop();
		float GetFrustumNear();
		float GetFrustumFar();

		glm::vec3 GetLeft();
		glm::vec3 GetDirection();
		glm::vec3 GetUp();

		Ray GetRay(int x, int y);
	};
}