#pragma once

#include <string>
#include <vector>
#include <map>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "TerrainMaster/Utils/SharedPtr.h"
#include "TerrainMaster/Render/Mesh/Mesh.h"
#include "TerrainMaster/Utils/File.h"

namespace TM {
	enum class ProgramType {
		Vertex,
		Geometry,
		Fragment,
		TessControl,
		TessEvaluation,
		Compute
	};

	typedef struct SHADER_SOURCE {
		ProgramType type;
		std::string program;
		File* fileSource;
	} ShaderSource;

	struct Attribute {
		GLuint id;
		std::string name;
	};

	struct SUniform {
		std::string name;
		GLenum type;
		bool isArray;
		int arrSize;
		float floatVal;
		int intVal;
		glm::vec2 vec2Val;
		glm::vec3 vec3Val;
		glm::vec4 vec4Val;
	};

	class Shader {
	public:
		Shader();
		~Shader();

		void DeleteShader();

		bool Compile();
		void AddShader(ProgramType type, std::string program, File fileSource);

		void SetUniform1i(std::string name, int v);
		void SetUniform1f(std::string name, float v);
		void SetUniform2f(std::string name, glm::vec2 vec);
		void SetUniform3f(std::string name, glm::vec3 vec);
		void SetUniform4f(std::string name, glm::vec4 vec);
		void SetUniformMatrix4f(std::string name, glm::mat4 mat);

		void SetUniform1iv(std::string name, const GLint* value);
		void SetUniform1fv(std::string name, const GLfloat* value);
		void SetUniform2fv(std::string name, const GLfloat* value);
		void SetUniform3fv(std::string name, const GLfloat* value);
		void SetUniform4fv(std::string name, const GLfloat* value);
		void SetUniformMatrix3fv(std::string name, const GLfloat* value);
		void SetUniformMatrix4fv(std::string name, const GLfloat* value);

		int GetAttributeLocation(std::string alias);
		void BindAttributeLocation(GLuint location, std::string alias);

		void BindFragOutput(int attachement, std::string name);

		Attribute* GetAttribute(MeshBuffer::Type type);

		SUniform* GetSUniform(std::string name);
		std::vector<SUniform>& GetSUniforms();

		GLuint GetId() const;

		static std::string GetShaderTypeExtensionName(ProgramType type);

		int GetProgramCount();

		bool IsCompiled() const;

		static std::vector<ProgramType> GetAllProgramTypes();
	private:
		std::map<ProgramType, ShaderSource> _programs;
		std::map<std::string, GLint> _uniformLocationCahce;
		std::vector<GLuint> _programIds;
		std::map<MeshBuffer::Type, Attribute*> _attributes;
		std::vector<SUniform> _uniforms;
		GLuint _shaderProgram;
		bool _isCompiled;

		bool CompileProgram(ProgramType type, std::string source);

		GLenum GetShaderEnumType(ProgramType type);
		std::string GetShaderName(ProgramType type);
		GLint GetUniformLocation(std::string name);

		bool CheckError(GLuint shader, GLuint flag, bool isProgram, ProgramType* type);

		bool GetUniformArrayIndex(std::string name, std::string* nname, int* index);
	};

	typedef SharedPtr<Shader> ShaderPtr;
}