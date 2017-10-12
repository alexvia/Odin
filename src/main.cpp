#include <Odin/ColladaLoader.h>
#include <Odin/OBJLoader.h>
#include <Odin/Shader.h>
#include <Odin/Window.h>
#include <Util/XmlParser.h>

#include <iostream>
#include <chrono>

int main()
{
	XmlParser parser;
	auto start = std::chrono::high_resolution_clock::now();
	//XmlNode *root = parser.Load("../data/models/model.dae");
	tinyxml2::XMLDocument doc;
	doc.LoadFile("../data/models/model.dae");
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	//delete root;

	TCHAR pwd[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pwd);
	std::cout << pwd << std::endl;

	// Open Window
	Window window;
	if (!window.Create()) 
		return -1;

	//----------------------------
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)window.Width() / (float)window.Height(), 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(
		glm::vec3(0, 13, 15), // Camera is at (x, y, z), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
	glm::mat4 mvp = proj * view * model;
	//----------------------------

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	// Load Shaders
	Shader shader;
	if(!shader.Load("simple")) return -1;
	shader.Use();
	shader.SetUniform("mvp", mvp);

	// Load Models
	Mesh quad;
	Mesh suzanne;
	OBJLoader loader;
	//loader.Load("../data/models/cube.obj", quad);
	loader.Load("../data/models/suzanne.obj", suzanne);

	ColladaLoader c;
	c.Load("../data/models/animated.dae", quad);
	
	// Game Loop
	while (!window.ShouldClose())
	{
		model = glm::rotate(model, glm::radians(3.0f), glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));
		mvp = proj * view * model;
		shader.SetUniform("mvp", mvp);

		window.PollEvents();

		// render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		quad.Draw();

		window.SwapBuffers();
	}
}
