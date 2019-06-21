
#include <iostream>
#include <glad\\glad.h>
#include <GLFW\glfw3.h>
#include <DreamMath.h>

void WindowResizeCallBack(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Bitch Niggas", NULL, NULL);

	if (window == NULL)
	{
		printf("Failed to create a window");
		glfwTerminate();
		return - 1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to Init GLAD");
		return -1;
	 }


	glViewport(0,0,800,600);
	glfwSetWindowSizeCallback(window, WindowResizeCallBack);

	glClearColor(0.2f,0.3f,0.3f,1.0f);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
