#include "Camera.h"

#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/quaternion.hpp>

namespace TM {

	Camera::Camera(int width, int height) : Spatial("Camera"), _projectionMatrix(), _viewMatrix(), _viewProjectionMatrix(), _looAtMatrix() {
		_width = width;
		_height = height;
		this->SetFromFrustum(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
		_planeState = 0;
	}

	glm::mat4& Camera::GetProjectionMatrix() {
		return _projectionMatrix;
	}

	glm::mat4& Camera::GetViewMatrix() {
		return _viewMatrix;
	}

	glm::mat4& Camera::GetProjectionViewMatrix() {
		return _viewProjectionMatrix;
	}

	void Camera::SetFromFrustum(float left, float right, float bottom, float top, float zNear, float zFar) {
		_projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
		_mode = ORTHOGRAPHIC_MODE;
		if (left == 0.0f && top == 0.0f && zNear == -1.0f && zFar == 1.0f) {
			_is2DOrtho = true;
		}
		else {
			_is2DOrtho = false;
		}
	}

	float Camera::GetFrustumLeft() {
		return frustumLeft;
	}

	float Camera::GetFrustumRight() {
		return frustumRight;
	}

	float Camera::GetFrustumBottom() {
		return frustumBottom;
	}

	float Camera::GetFrustumTop() {
		return frustumTop;
	}

	float Camera::GetFrustumNear() {
		return frustumNear;
	}

	float Camera::GetFrustumFar() {
		return frustumFar;
	}

	void Camera::SetFrustumPerspective(float fovY, float aspect, float zNear, float zFar) {
		_fovY = fovY;
		_zNear = zNear;
		_zFar = zFar;
		//this->_projectionMatrix = glm::perspective(glm::radians(fovY), aspect, zNear, zFar);
		_mode = PERSPECTIVE_MODE;

		float h = glm::tan(glm::radians(fovY) * .5f) * zNear;
		float w = h * aspect;
		frustumLeft = -w;
		frustumRight = w;
		frustumBottom = -h;
		frustumTop = h;
		frustumNear = zNear;
		frustumFar = zFar;

		this->_projectionMatrix = glm::frustum(frustumLeft, frustumRight, frustumBottom, frustumTop, frustumNear, frustumFar);

		{
			float nearSquared = frustumNear * frustumNear;
			float leftSquared = frustumLeft * frustumLeft;
			float rightSquared = frustumRight * frustumRight;
			float bottomSquared = frustumBottom * frustumBottom;
			float topSquared = frustumTop * frustumTop;
		}
	}

	glm::vec3 getRotationColumn(glm::mat4 mat, int i) {
		glm::quat q = glm::quat(mat);

		float norm = q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
		if (norm != 1.0f) {
			norm = 1.0f / glm::sqrt(norm);
		}

		float xx = q.x * q.x * norm;
		float xy = q.x * q.y * norm;
		float xz = q.x * q.z * norm;
		float xw = q.x * q.w * norm;
		float yy = q.y * q.y * norm;
		float yz = q.y * q.z * norm;
		float yw = q.y * q.w * norm;
		float zz = q.z * q.z * norm;
		float zw = q.z * q.w * norm;

		glm::vec3 store;

		switch (i) {
		case 0:
			store.x = 1 - 2 * (yy + zz);
			store.y = 2 * (xy + zw);
			store.z = 2 * (xz - yw);
			break;
		case 1:
			store.x = 2 * (xy - zw);
			store.y = 1 - 2 * (xx + zz);
			store.z = 2 * (yz + xw);
			break;
		case 2:
			store.x = 2 * (xz + yw);
			store.y = 2 * (yz - xw);
			store.z = 1 - 2 * (xx + yy);
			break;
		}
		return store;
	}

	glm::vec3 Camera::GetLeft() {
		return getRotationColumn(GetViewMatrix(), 0);
	}

	glm::vec3 Camera::GetDirection() {
		return getRotationColumn(GetViewMatrix(), 2);
	}

	glm::vec3 Camera::GetUp() {
		return getRotationColumn(GetViewMatrix(), 1);
	}

	Ray Camera::GetRay(int x, int y) {
		glm::vec3 click3d = GetWorldPosition(x, y, 0);
		glm::vec3 dir = glm::normalize(GetWorldPosition(x, y, 1) - click3d);
		return Ray(click3d, dir);
	}

	void Camera::Resize(int width, int height) {
		_width = width;
		_height = height;
		if (_mode == PERSPECTIVE_MODE) {
			//setFrustumPerspective(_fovY, (float)width / (float)height, _zNear, _zFar);
		}
		else if (_mode == ORTHOGRAPHIC_MODE && _is2DOrtho) {
			this->SetFromFrustum(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
		}
	}

	void Camera::LookAt(glm::vec3 toPos, glm::vec3 eye) {
		_looAtMatrix = glm::lookAt(eye, toPos, glm::vec3(0, 1, 0));
	}

	void Camera::LookAtDir(glm::vec3 dir) {
		_looAtMatrix = glm::lookAt(glm::vec3(), dir, glm::vec3(0, 1, 0));
	}

	glm::vec3 Camera::GetWorldPosition(float x, float y, float projectionZPos) const {
		glm::mat4 inverseMat = glm::inverse(_viewProjectionMatrix);

		glm::vec3 store = glm::vec3((2.0f * x) / _width - 1.0f, (2.0f * y) / _height - 1.0f, projectionZPos * 2 - 1);
		glm::vec4 proStore = inverseMat * glm::vec4(store, 1.0f);
		store.x = proStore.x;
		store.y = proStore.y;
		store.z = proStore.z;
		store *= 1.0f / proStore.w;
		return store;
	}

	glm::vec3 Camera::GetScreenCoordinates(glm::vec3 pos) {
		glm::vec4 proj = (GetProjectionViewMatrix() * glm::vec4(pos, 1.0f));

		glm::vec3 store = glm::vec3(proj.x, proj.y, proj.z);
		store = store / proj.w;

		int viewPortRight = 1;
		int viewPortLeft = 0;
		int viewPortTop = 0;
		int viewPortBottom = 1;

		store.x = ((store.x + 1.0f) * (viewPortRight - viewPortLeft) / 2.0f + viewPortLeft) * GetWidth();
		store.y = ((store.y + 1.0f) * (viewPortTop - viewPortBottom) / 2.0f + viewPortBottom) * GetHeight();
		store.z = (store.z + 1.0f) / 2.0f;

		return store;
	}

	void Camera::Update() {
		this->_viewMatrix = glm::mat4();
		// define your up vector
		glm::vec3 upVector = glm::vec3(0, 1, 0);
		// rotate around to a given bearing: yaw
		glm::mat4 camera = glm::rotate(glm::mat4(), glm::radians(_rotation.y), upVector);
		// Define the 'look up' axis, should be orthogonal to the up axis
		glm::vec3 pitchVector = glm::vec3(1, 0, 0);
		// rotate around to the required head tilt: pitch
		camera = glm::rotate(camera, glm::radians(_rotation.x), pitchVector);

		glm::vec3 rollVector = glm::vec3(0, 0, 1);
		camera = glm::rotate(camera, glm::radians(_rotation.z), rollVector);

		// now get the view matrix by taking the camera inverse
		this->_viewMatrix = _looAtMatrix * glm::inverse(camera) * glm::translate(this->_viewMatrix, glm::vec3(-_location.x, -_location.y, -_location.z));

		_viewProjectionMatrix = _projectionMatrix * _viewMatrix;
	}

	int Camera::GetWidth() const {
		return _width;
	}

	int Camera::GetHeight() const {
		return _height;
	}

	void Camera::SetProjectionMatrix(glm::mat4 m) {
		_projectionMatrix = m;
	}

	void Camera::SetViewMatrix(glm::mat4 m) {
		_viewMatrix = m;
	}
}