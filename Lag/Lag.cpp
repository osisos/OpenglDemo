#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include"Shader.h"
#include<vector>
#include"board.h"
#include"pointnode.h"

unsigned int VBO, shaderProgram, VAO, EBO;
Object *oj;
int n;
float xt;

int idx = 0;

//顶点着色器
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// 片段着色器代码
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";


std::vector<pointnode> vertices;

/*窗口大小调整回调函数*/
void framebuffer_size_callback(GLFWwindow* windows, int width, int height) {
	glViewport(0, 0, width, height);
}

/*处理输入函数*/
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}


void init_()
{
	int sizeofvbo = n * n * 3 * sizeof(float);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //glfwWindowsHit(argv1,argv2):第一个参数为选项的名称，第二个参数为这个选项的值
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//顶点着色器绑定
	unsigned int vertexshader = Shader::createShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentshader = Shader::createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	std::vector<unsigned int> shaderlist;
	shaderlist.push_back(vertexshader);
	shaderlist.push_back(fragmentshader);

	shaderProgram = Shader::createProgram(shaderlist);
	glUseProgram(shaderProgram);
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeofvbo, NULL, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, 0);

	glEnableVertexAttribArray(0);
}

void draw()
{
	idx++;
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	
	int sums = vertices.size();
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices);
	if (idx % 60 == 0) {
		std::cout << idx / 60 << std::endl;
		oj->fecth();
		int **table = oj->show();

		vertices.clear();

		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				if (table[i][j] == 1) {
					float left_down_x = ((float)(j - 1)*xt - 1.0);
					float left_down_y = ((float)(i - 1)*xt - 1.0);

					pointnode temp_p_nd(left_down_x, left_down_y, 0.0f);
					vertices.push_back(temp_p_nd);

					temp_p_nd.a += xt;
					vertices.push_back(temp_p_nd);

					temp_p_nd.b += xt;
					vertices.push_back(temp_p_nd);

					temp_p_nd.a -= xt;
					vertices.push_back(temp_p_nd);
				}
			}
		}

		if (vertices.size() == 0)
			vertices.resize(10);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sums * 3 * sizeof(float), &vertices[0]);
		sums = int(vertices.size());
	}
	
	for (int i = 0; i < sums / 4; i++) {
		glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
	}

}

int main()
{
	n = 250;
	int wth = n * 4;
	xt = 2.0 / n;
	oj = new Object(n,n);
	oj->reshape();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //glfwWindowsHit(argv1,argv2):第一个参数为选项的名称，第二个参数为这个选项的值
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建一个GLFW窗口对象，这个对象存放了所有和窗口数据相关的数据
	GLFWwindow *window = glfwCreateWindow(wth, wth, "LearnOpengl", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "创建GLFW窗口失败" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);//创建窗口后，通知GLFW将窗口的上下文设置为当前线程的上下文
	// GLAD是用来管理opengl函数指针的，调用任何opengl函数之前。需要初始化GLAD

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "初始化GLAD失败" << std::endl;
		return -1;
	}
	//glfwGetProcAddress，转化为GLADloadproc


	glViewport(0, 0, wth, wth); // 前两个参数控制窗口左下角的位置
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//将函数注册到很多其他的回调函数中，渲染循环初始化之前注册回调函数
	init_();

		//循环绘制
	while (!glfwWindowShouldClose(window)) {
		// 输入指令处理
		processInput(window);
		// 渲染指令
		draw();

		//检查并且调用时间，交换缓冲
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;

}