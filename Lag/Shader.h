#pragma once
/*
	Created by VoidR on 2022/5/13
*/


#ifndef SHADER_H
#define SHADER_H


#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<vector>
#include<string>


class Shader
{
public:
	static unsigned int createShader(GLenum tpye, const char *Shadersource);
	static unsigned int createProgram(const std::vector<unsigned int> &shaderList);

};

#endif // !SHADER_H
