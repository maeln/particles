#include "shaderdb.hh"
#include "src/tools.hh"

#include <iostream>
#include <sstream>
#include <utility>

Shader ShaderDB::get_shader(GLuint handler) { return m_shaders.at(handler); }

Program ShaderDB::get_program(GLuint handler) { return m_programs.at(handler); }

GLenum ShaderDB::shader_type(std::string path) {
    std::string ext = path.substr(path.rfind('.'), std::string::npos);
    if (ext == ".vs") {
	return GL_VERTEX_SHADER;
    } else if (ext == ".fs") {
	return GL_FRAGMENT_SHADER;
    } else if (ext == ".gs") {
	return GL_GEOMETRY_SHADER;
    } else if (ext == ".cs") {
	return GL_COMPUTE_SHADER;
    } else {
	std::cerr << "[ERR] Unkown shader extension '" << ext << "' for file: " << path << std::endl;
	return 0;
    }
}

Shader ShaderDB::load_shader(std::string path) {
    GLuint addr = glCreateShader(shader_type(path));
    std::string source = read_file(path).c_str();
    const char *source_ptr = source.c_str();
    time_t mt = get_last_modified_time(path);
    glShaderSource(addr, 1, &source_ptr, NULL);
    glCompileShader(addr);

    // Check if the compilation went well
    GLint status;
    glGetShaderiv(addr, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
	GLint log_len;
	glGetShaderiv(addr, GL_INFO_LOG_LENGTH, &log_len);
	GLchar *log = new GLchar[log_len + 1];
	glGetShaderInfoLog(addr, log_len, NULL, log);

	std::cerr << "[ERR] Shader: '" << path << "' compilation error: " << log << std::endl;
	delete[] log;
	throw ShaderLoadingException();
    }

    // Check if the shader object already exist:
    bool reload = false;
    GLuint id = m_shader_counter;
    Shader shader(id, addr, path, mt);

    auto it = m_shaders_path.find(path);
    if (it != m_shaders_path.end()) {
	id = it->second;
	glDeleteShader(m_shaders[id].addr);
	reload = true;
	shader.handler = id;
    }

    if (!reload) {
	m_shader_counter += 1;
    }

    // Load the uniforms from the shader
    std::istringstream src(source);
    std::string line;
    while (getline(src, line)) {
	if (line.substr(0, 8) == "uniform ") {
	    std::istringstream str(line.substr(7));
	    std::string type;
	    std::string name;
	    str >> type;
	    str >> name;
	    name.erase(name.find(";"), 1);
	    shader.uniforms.push_back(name);
	}
    }

    m_shaders[shader.handler] = shader;
    m_shaders_path[path] = shader.handler;
    return shader;
}

GLuint ShaderDB::load_program(const std::initializer_list<std::string> shaders_path) {
    GLuint addr = glCreateProgram();

    std::vector<Shader> shaders;
    for (std::string path : shaders_path) {
	// NOTE: Check if shader already loaded. Add an option to force reload ?
	shaders.push_back(load_shader(path));
    }

    for (Shader shader : shaders) {
	glAttachShader(addr, shader.addr);
    }
    glLinkProgram(addr);

    GLint status;
    glGetProgramiv(addr, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
	GLint log_length;
	glGetProgramiv(addr, GL_INFO_LOG_LENGTH, &log_length);
	GLchar *log = new GLchar[log_length + 1];
	glGetProgramInfoLog(addr, log_length, NULL, log);
	std::cerr << "Program link failure: " << log << std::endl;
	delete[] log;
	throw ProgramLoadingException();
    }

    for (Shader shader : shaders) {
	glDetachShader(addr, shader.addr);
    }

    // NOTE: Check if program already exist ?
    Program program(m_program_counter, addr);
    m_program_counter += 1;

    // Manage uniforms
    for (Shader shader : shaders) {
	program.shaders.push_back(shader.handler);
	for (std::string uniform : shader.uniforms) {
	    if (program.uniforms_location.find(uniform) == program.uniforms_location.end()) {
		program.uniforms_location[uniform] = glGetUniformLocation(program.addr, uniform.c_str());
	    }
	}
    }

    m_programs[program.handler] = program;
    return program.handler;
}

void ShaderDB::reload_program(GLuint handler) {
    Program program = m_programs[handler];

    // TODO: Some duplicate code, should be refactored.
    std::vector<Shader> shaders;
    for (GLuint sh : program.shaders) {
	shaders.push_back(m_shaders[sh]);
    }

    GLuint new_addr = glCreateProgram();
    for (Shader shader : shaders) {
	glAttachShader(new_addr, shader.addr);
    }
    glLinkProgram(new_addr);

    GLint status;
    glGetProgramiv(new_addr, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
	GLint log_length;
	glGetProgramiv(new_addr, GL_INFO_LOG_LENGTH, &log_length);
	GLchar *log = new GLchar[log_length + 1];
	glGetProgramInfoLog(new_addr, log_length, NULL, log);
	std::cerr << "Program link failure: " << log << std::endl;
	delete[] log;
	throw ProgramLoadingException();
    }

    for (Shader shader : shaders) {
	glDetachShader(new_addr, shader.addr);
    }

    glDeleteProgram(program.addr);
    program.addr = new_addr;

    // Manage uniforms
    for (Shader shader : shaders) {
	for (std::string uniform : shader.uniforms) {
	    if (program.uniforms_location.find(uniform) != program.uniforms_location.end()) {
		program.uniforms_location[uniform] = glGetUniformLocation(program.addr, uniform.c_str());
	    }
	}
    }

    m_programs[program.handler] = program;
}

void ShaderDB::check_reload() {
    for (std::pair<std::string, GLuint> shader_path : m_shaders_path) {
	Shader shader = m_shaders[shader_path.second];
	time_t mt = get_last_modified_time(shader_path.first);
	if (mt > shader.last_modified) {
	    try {
		std::cerr << "[NFO] Reloading shader '" << shader.path << "' ..." << std::endl;
		Shader reloaded = load_shader(shader_path.first);
		for (std::pair<GLuint, Program> program : m_programs) {
		    bool should_reload = false;
		    for (GLuint sh : program.second.shaders) {
			if (sh == reloaded.handler) {
			    should_reload = true;
			    break;
			}
		    }

		    if (should_reload) {
			reload_program(program.first);
		    }
		}
	    } catch (ShaderLoadingException &) {
		std::cerr << "[ERR] Failed to reload '" << shader.path << "', failling back to previous version." << std::endl;
	    } catch (ProgramLoadingException &) {
		std::cerr << "[ERR] Failed to reload a program, failling back to previous version." << std::endl;
	    }
	    m_shaders[shader_path.second].last_modified = mt;
	}
    }
}
