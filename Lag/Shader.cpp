#include "Shader.h"
#include<iostream>

unsigned int Shader::createShader(GLenum shadertype, const char *Shadersource) {

	unsigned int shader = glCreateShader(shadertype);
	glShaderSource(shader, 1, &Shadersource, NULL); //��shader�ַ���
	glCompileShader(shader);
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}

unsigned int Shader::createProgram(const std::vector<unsigned int> &shaderList)
{
	unsigned int programid = glCreateProgram();
	for (std::vector<unsigned int>::size_type iLoop = 0; iLoop < shaderList.size(); iLoop++) {
		glAttachShader(programid, shaderList[iLoop]);
	}

	glLinkProgram(programid);
	return programid;
}