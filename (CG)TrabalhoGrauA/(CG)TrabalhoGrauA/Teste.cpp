/*
	Incluindo as Bibliotecas
*/
#include <iostream>
#include <cmath>
// GLAD
#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>
// Incluindo o arquivo de cabeçalho


#define PI 3.14

void renderCircle()
{
	// Inicializa o GLFW
	if (!glfwInit())
	{
		std::cerr << "Falha ao iniciar o GLFW" << std::endl;
		return;
	}
	// Cria uma Janela (800x600)
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Test", nullptr, nullptr);
	// Verifica se a janela foi criada com sucesso
	if (!window)
	{
		// Se falhar, imprime uma mensagem de erro
		std::cerr << "Falha ao criar uma janela GLFW" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	// Inicializa o GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// Se falhar, imprime uma mensagem de erro
		std::cerr << "Falha ao inicializar o GLAD" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return;
	}
	// Configura o Viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	// Define a cor de fundo da janela
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Cria o Círculo
	// Gera os vértices do círculo
	const int numVertices = 100; // Número de vértices do círculo
	const float radius = 0.5f; // Raio do círculo
	GLfloat vertices[numVertices * 3]; // Array para armazenar os vértices
	// Adiciona o centro do círculo
	vertices[0] = 0.0f; // x
	vertices[1] = 0.0f; // y
	vertices[2] = 0.0f; // z

	// Adiciona os vértices do perímetro do círculo
	for (int i = 0; i < numVertices; ++i)
	{
		float angle = 2.0f * PI * float(i) / float(numVertices); // Calcula o ângulo
		vertices[i * 3] = cos(angle); // Coordenada X
		vertices[i * 3 + 1] = sin(angle); // Coordenada Y
		vertices[i * 3 + 2] = 0.0f; // Coordenada Z
	}

	// Cria um Vertex Array Object (VAO)
	unsigned int VAO;
	glGenVertexArrays(1, &VAO); //Gera um VAO

	// Cria um buffer de vértices (VBO)
	unsigned int VBO;
	glGenBuffers(1, &VBO); //Gera um VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Adiciona os dados do buffer

	// Configura os atributos do vértice
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Define o formato dos dados
	glEnableVertexAttribArray(0); // Ativa o atributo do vértice

	/*

		Shaders

	*/

	// Cria o shader de vértice
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	// Verifica se o shader foi compilado com sucesso
	if (!vertexShader)
	{
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			// Se falhar, imprime uma mensagem de erro
			glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
			std::cerr << "Erro ao compilar o shader de vértice: " << infoLog << std::endl;
		}
	}

	// Cria o shader de fragmento
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
		"}\0";
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	// Verifica se o shader foi compilado com sucesso
	if (!fragmentShader)
	{
		int success;
		char infoLog[512];
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			// Se falhar, imprime uma mensagem de erro
			glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
			std::cerr << "Erro ao compilar o shader de fragmento: " << infoLog << std::endl;
		}
	}

	// Cria um programa de shader
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Verifica se o programa de shader foi vinculado com sucesso
	if (!shaderProgram)
	{
		int success;
		char infoLog[512];
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			// Se falhar, imprime uma mensagem de erro
			glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
			std::cerr << "Erro ao vincular o programa de shader: " << infoLog << std::endl;
		}
	}
	// Deleta os shaders, pois não são mais necessários
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	std::cout << "Shaders deletados com sucesso" << std::endl;

	// Loop principal
	while (!glfwWindowShouldClose(window))
	{
		// Limpa a tela
		glClear(GL_COLOR_BUFFER_BIT);
		// Usa o programa de shader
		glUseProgram(shaderProgram);
		// Define o VAO como o atual
		glBindVertexArray(VAO); // Define o VAO como o atual
		// Verifica se o Buffer foi gerado com sucesso
		if (VAO == 0)
		{
			// Se falhar, imprime uma mensagem de erro
			std::cerr << "Falha ao gerar o OpenGL buffer (VAO)" << std::endl;
			glfwDestroyWindow(window);
			glfwTerminate();
			return;
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBO); // Define o buffer como um buffer de vértices
		// Verifica se o buffer foi gerado com sucesso
		if (VBO == 0)
		{
			// Se falhar, imprime uma mensagem de erro
			std::cerr << "Falha ao gerar o OpenGL buffer (VBO)" << std::endl;
			glfwDestroyWindow(window);
			glfwTerminate();
			return;
		}
		// Desenha o Retangulo
		glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices); // Desenha o círculo
		// Troca os buffers e verifica eventos
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Destroi os objetos
	glDeleteVertexArrays(1, &VAO); // Deleta o VAO
	glDeleteBuffers(1, &VBO); // Deleta o VBO
	glfwDestroyWindow(window); // Destroi a janela
	glfwTerminate(); // Termina o GLFW



}