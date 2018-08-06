#include "Shader.h"
#include <iostream>
#include "TerrainMaster/Utils/Files.h"

namespace TM {
	Shader::Shader() {
		_isCompiled = false;
		_shaderProgram = 0;
	}

	Shader::~Shader() {
		DeleteShader();
	}

	void Shader::DeleteShader() {
		if (_shaderProgram == 0) {
			return;
		}
		for (GLuint programId : _programIds) {
			glDetachShader(_shaderProgram, programId);
			glDeleteShader(programId);
		}
		glDeleteProgram(_shaderProgram);
	}

	bool Shader::Compile() {
		DeleteShader();
		_programIds.clear();
		_shaderProgram = glCreateProgram();
		_isCompiled = true;
		for (std::map<ProgramType, ShaderSource>::iterator i = _programs.begin(); i != _programs.end(); i++) {
			std::string source = i->second.program;
			if (source.length() == 0) {
				source = Files::ReadFile(*i->second.fileSource, "\r\n");
			}
			if (!CompileProgram(i->first, source)) {
				return false;
			}
		}
		glLinkProgram(_shaderProgram);
		if (CheckError(_shaderProgram, GL_LINK_STATUS, true, nullptr)) {
			return false;
		}
		glValidateProgram(_shaderProgram);
		if (CheckError(_shaderProgram, GL_VALIDATE_STATUS, true, nullptr)) {
			return false;
		}

		int uniformCount;
		glGetProgramiv(_shaderProgram, GL_ACTIVE_UNIFORMS, &uniformCount);

		_uniforms.clear();

		GLint size;
		GLenum type;
		const GLsizei bufSize = 64;
		GLchar name[bufSize];
		GLsizei length;
		for (int i = 0; i < uniformCount; i++) {
			glGetActiveUniform(_shaderProgram, (GLuint)i, bufSize, &length, &size, &type, name);
			std::string uname = std::string(name, length);

			SUniform u;
			u.name = name;
			u.type = type;
			if (type == GL_SAMPLER_2D) {

			}
			else if (type == GL_FLOAT) {
				GLfloat f;
				glGetUniformfv(_shaderProgram, i, &f);
				u.floatVal = (float)f;
			}
			else if (type == GL_INT) {
				GLint ii;
				glGetUniformiv(_shaderProgram, i, &ii);
				u.intVal = (int)ii;
			}
			else if (type == GL_FLOAT_VEC2) {
				GLfloat vec[2];
				glGetUniformfv(_shaderProgram, i, vec);
				u.vec2Val = glm::vec2(vec[0], vec[1]);
				//std::cout << u.vec2Val.x << ":" << u.vec2Val.y << std::endl;
			}
			else if (type == GL_FLOAT_VEC3) {
				GLfloat vec[3];
				glGetUniformfv(_shaderProgram, i, vec);
				u.vec3Val = glm::vec3(vec[0], vec[1], vec[2]);
				//std::cout << u.vec3Val.x << ":" << u.vec3Val.y << ":" << u.vec3Val.z << std::endl;
			}
			else if (type == GL_FLOAT_VEC4) {
				GLfloat vec[4];
				glGetUniformfv(_shaderProgram, i, vec);
				u.vec4Val = glm::vec4(vec[0], vec[1], vec[2], vec[3]);
				//std::cout << u.vec4Val.x << ":" << u.vec4Val.y << ":" << u.vec4Val.z << ":" << u.vec4Val.w << std::endl;
			}
			else if (type == GL_BOOL) {

			}
			else {
				continue;
			}

			int arId;
			std::string nname;
			if (GetUniformArrayIndex(uname, &nname, &arId)) {
				SUniform* su = GetSUniform(uname);
				if (su == nullptr) {
					u.name = nname;
					u.isArray = true;
					u.arrSize = arId;
					_uniforms.push_back(u);
				}
				else {
					if (su->arrSize < arId) {
						su->arrSize = arId;
					}
				}
			}
			else {
				_uniforms.push_back(u);
			}
		}

		return true;
	}

	int Shader::GetProgramCount() {
		return _programs.size();
	}

	SUniform* Shader::GetSUniform(std::string name) {
		int arId;
		std::string nname;
		if (GetUniformArrayIndex(name, &nname, &arId)) {
			name = nname;
		}

		for (SUniform& uniform : _uniforms) {
			if (uniform.name == name) {
				return &uniform;
			}
		}

		return nullptr;
	}

	std::vector<SUniform>& Shader::GetSUniforms() {
		return _uniforms;
	}

	bool Shader::GetUniformArrayIndex(std::string name, std::string* nname, int* index) {
		int fi = name.find_last_of('[');
		if (fi > 0) {
			std::string numb = name.substr(fi + 1);
			int fl = numb.find_last_of(']');
			if (fl > 0) {
				numb = numb.substr(0, fl);
				try {
					*index = std::stoi(numb);
					*nname = name.substr(0, fi);
					return true;
				}
				catch (std::exception&e) {}
			}
		}
		return false;
	}

	void Shader::AddShader(ProgramType type, std::string program, File fileSource) {
		ShaderSource source;
		source.type = type;
		source.program = program;
		source.fileSource = new File(fileSource);
		_programs[type] = source;
	}

	bool Shader::CompileProgram(ProgramType type, std::string source) {
		GLuint id = glCreateShader(GetShaderEnumType(type));
		const GLchar* p[1];
		p[0] = source.c_str();
		GLint lengths[1];
		lengths[0] = source.length();
		glShaderSource(id, 1, p, lengths);
		glCompileShader(id);
		if (CheckError(id, GL_COMPILE_STATUS, false, &type)) {
			return false;
		}
		glAttachShader(_shaderProgram, id);
		return true;
	}

	GLenum Shader::GetShaderEnumType(ProgramType type) {
		switch (type) {
		case ProgramType::Vertex:
			return GL_VERTEX_SHADER;
		case ProgramType::Geometry:
			return GL_GEOMETRY_SHADER;
		case ProgramType::Fragment:
			return GL_FRAGMENT_SHADER;
		case ProgramType::TessControl:
			return GL_TESS_CONTROL_SHADER;
		case ProgramType::TessEvaluation:
			return GL_TESS_EVALUATION_SHADER;
		case ProgramType::Compute:
			return GL_COMPUTE_SHADER;
		default:
			return GL_FALSE;
		}
	}

	std::string Shader::GetShaderName(ProgramType type) {
		switch (type) {
		case ProgramType::Vertex:
			return "Vertex";
		case ProgramType::Geometry:
			return "Geometry";
		case ProgramType::Fragment:
			return "Fragment";
		case ProgramType::TessControl:
			return "TessControl";
		case ProgramType::TessEvaluation:
			return "TessEvaluation";
		case ProgramType::Compute:
			return "Compute";
		default:
			return GL_FALSE;
		}
	}

	std::string Shader::GetShaderTypeExtensionName(ProgramType type) {
		switch (type) {
		case ProgramType::Vertex:
			return "vert";
		case ProgramType::Geometry:
			return "geom";
		case ProgramType::Fragment:
			return "frag";
		case ProgramType::TessControl:
			return "tesscont";
		case ProgramType::TessEvaluation:
			return "tesseval";
		case ProgramType::Compute:
			return "comp";
		default:
			return "";
		}
	}

	std::vector<ProgramType> Shader::GetAllProgramTypes() {
		return {
			ProgramType::Vertex,
			ProgramType::Geometry,
			ProgramType::TessControl,
			ProgramType::TessEvaluation,
			ProgramType::Compute,
			ProgramType::Fragment,
		};
	}

	Attribute* Shader::GetAttribute(MeshBuffer::Type type) {
		if (_attributes.find(type) != _attributes.end()) {
			return _attributes[type];
		}
		else {
			Attribute* attr = new Attribute();
			attr->id = -1;
			switch (type) {
			case MeshBuffer::Type::BiTangent:
				attr->name = "a_BiTangent";
				break;
			case MeshBuffer::Type::Color:
				attr->name = "a_Color";
				break;
			case MeshBuffer::Type::Normal:
				attr->name = "a_Normal";
				break;
			case MeshBuffer::Type::Position:
				attr->name = "a_Position";
				break;
			case MeshBuffer::Type::Tangent:
				attr->name = "a_Tangent";
				break;
			case MeshBuffer::Type::TexCoord:
				attr->name = "a_TexCoord";
				break;
			case MeshBuffer::Type::InstanceData:
				attr->name = "a_ModelMatrix";
				break;
			}
			_attributes[type] = attr;
			return attr;
		}
	}

	bool Shader::IsCompiled() const {
		return _isCompiled;
	}

	void Shader::BindAttributeLocation(GLuint location, std::string alias) {
		glBindAttribLocation(_shaderProgram, location, alias.c_str());
	}

	GLuint Shader::GetId() const {
		return _shaderProgram;
	}


	int Shader::GetAttributeLocation(std::string alias) {
		return glGetAttribLocation(_shaderProgram, alias.c_str());
	}

	GLint Shader::GetUniformLocation(std::string name) {
		if (_uniformLocationCahce.find(name) != _uniformLocationCahce.end()) {
			return _uniformLocationCahce[name];
		}
		GLuint loc = glGetUniformLocation(_shaderProgram, name.c_str());
		_uniformLocationCahce[name] = loc;
		return loc;
	}

	void Shader::BindFragOutput(int attachement, std::string name) {
		glBindFragDataLocation(_shaderProgram, attachement, name.c_str());
	}

	bool Shader::CheckError(GLuint shader, GLuint flag, bool isProgram, ProgramType* type) {
		GLint success = 0;
		GLchar error[1024] = { 0 };

		if (isProgram)
			glGetProgramiv(shader, flag, &success);
		else
			glGetShaderiv(shader, flag, &success);

		if (success == GL_FALSE) {
			if (isProgram)
				glGetProgramInfoLog(shader, sizeof(error), NULL, error);
			else
				glGetShaderInfoLog(shader, sizeof(error), NULL, error);
			if (type) {
				std::cerr << "Cannot compile " << GetShaderName(*type) << " program !" << std::endl << error << std::endl;
			}
			else {
				std::cerr << "Cannot compile " << " program !" << std::endl << error << std::endl;
			}
			return true;
		}
		return false;
	}

	void Shader::SetUniformMatrix4f(std::string name, glm::mat4 mat) {
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
	}

	void Shader::SetUniform1f(std::string name, float val) {
		glUniform1f(GetUniformLocation(name), val);
	}

	void Shader::SetUniform1i(std::string name, int val) {
		glUniform1i(GetUniformLocation(name), val);
	}

	void Shader::SetUniform2f(std::string name, glm::vec2 vec) {
		glUniform2f(GetUniformLocation(name), vec.x, vec.y);
	}

	void Shader::SetUniform3f(std::string name, glm::vec3 vec) {
		glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z);
	}

	void Shader::SetUniform4f(std::string name, glm::vec4 vec) {
		glUniform4f(GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
	}

	void Shader::SetUniform1iv(std::string name, const GLint* value) {
		glUniform1iv(GetUniformLocation(name), 1, value);
	}

	void Shader::SetUniform1fv(std::string name, const GLfloat* value) {
		glUniform1fv(GetUniformLocation(name), 1, value);
	}

	void Shader::SetUniform2fv(std::string name, const GLfloat* value) {
		glUniform2fv(GetUniformLocation(name), 1, value);
	}

	void Shader::SetUniform3fv(std::string name, const GLfloat* value) {
		glUniform3fv(GetUniformLocation(name), 1, value);
	}

	void Shader::SetUniform4fv(std::string name, const GLfloat* value) {
		glUniform4fv(GetUniformLocation(name), 1, value);
	}

	void Shader::SetUniformMatrix3fv(std::string name, const GLfloat * value) {
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, value);
	}

	void Shader::SetUniformMatrix4fv(std::string name, const GLfloat * value) {
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value);
	}
}