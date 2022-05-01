#include <stdio.h>
#include <iostream>
#include <cmath>

#define GLFW_DLL

// Includes for ImGui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void create_buffers(unsigned int *VAO, unsigned int *VBO, float vertices[], unsigned int size);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
// void processInput(GLFWwindow *window);
void processInput(GLFWwindow *window, float deltaTime, glm::vec3 *cameraPos, glm::vec3 *cameraFront, glm::vec3 *cameraUp);
void scrollScale(GLFWwindow *window, double xoffset, double yoffset);
void glfw_error_callback(int error, const char *description);
void setWindowIcon(GLFWwindow *window, const char *path);
GLFWwindow* initialiseOpenGLWindow(const char *programTitle, int windowHeight, int windowWidth,	const char *logoPath);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);

float rotation = 0;
float scroll = 0;

// Setting up to capture mouse information
float lastX = 800/2, lastY= 600/2;
float yaw = -90.0f;
float pitch = 0.0f;
float roll = 0.0f;
bool firstMouse = true;
bool godmode = true;

int main(int argc, char* argv[]){
	printf("Hello world\n");
	const char* programTitle = "Model Renderer";
	int windowHeight = 600;
	int windowWidth = 800;
	
	const char* glsl_version = "#version 130";
	
	GLFWwindow *window = initialiseOpenGLWindow(programTitle, windowHeight, windowWidth, "resources/roboticon.png");

	// Set up ImGui
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	// Setup Dear ImGui style
    ImGui::StyleColorsDark();

	 // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

	// Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

	Shader shader1("Shaders/vertexShader.vert", "Shaders/fragmentShader.frag");
	Shader shader2("Shaders/vertexTexShader.vert", "Shaders/fragmentTexShader.frag");
	Shader shader3("Shaders/coord.vert", "Shaders/coord.frag");

	float triangle1[] = {
		// positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	float triangle2[] = {
		// Positions			// Colours
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 		0.0f, 0.0f, 1.0f
	};
	// Vertices for drawing a rectangle with an EBO
	// float vertices[] = {
	// 	0.5f,  0.5f, 0.0f,  // top right
	// 	0.5f, -0.5f, 0.0f,  // bottom right
	// 	-0.5f, -0.5f, 0.0f,  // bottom left
	// 	-0.5f,  0.5f, 0.0f   // top left 
	// };
	unsigned int indices[] = {
		0, 1, 3,	// First triangle
		1, 2, 3		// Second triangle
	};

	float cube[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// unsigned int VBO, VAO, EBO;
	// // Generate the buffers
	// glGenVertexArrays(1, &VAO);
	// glGenBuffers(1, &VBO);
	// glGenBuffers(1, &EBO);
	// // Bind the vertex array object
	// glBindVertexArray(VAO);
	// // Copy the vertices into the vertex buffer
	// glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// // Copy the index array into an element buffer
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// // Sert the vertex attribute pointers
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	// glEnableVertexAttribArray(0);
	unsigned int VBOs[3], VAOs[3], EBO;
	glGenVertexArrays(3, VAOs);
	glGenBuffers(3, VBOs);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAOs[0]);
    
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    // Position Attribute
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);
	// Colour Attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture Attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);
	// Colour Attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

	// create_buffers(&VAO1, &VBO1, triangle1, sizeof(triangle1));
	// create_buffers(&VAO2, &VBO2, triangle2, sizeof(triangle2));

	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int i_width, i_height, nrChannels;
	unsigned char *data = stbi_load("resources/Textures/container.jpg", &i_width, &i_height, &nrChannels, 0);
	if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, i_width, i_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
	stbi_image_free(data);
	
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("resources/Textures/awesomeface.png", &i_width, &i_height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, i_width, i_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	shader3.use();
	shader3.setInt("texture1", 0);
	shader3.setInt("texture2", 1);


	// double xpos, ypos; // Mouse positions
	glfwSetScrollCallback(window, scrollScale);
	
	// Projection stuff
	glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
	// glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)windowWidth/(float)windowHeight, 0.1f, 100.0f);

	// 3D rendering!
	glm::mat4 model1 = glm::mat4(1.0f); // Create and identity matrix
	model1 = glm::rotate(model1, glm::radians(-55.0f), glm::vec3(1.0f, 0.0, 0.0));

	glm::vec3 cubePositions[] = {
		glm::vec3( 0.0f,  0.0f,  0.0f), 
		glm::vec3( 2.0f,  5.0f, -15.0f), 
		glm::vec3(-1.5f, -2.2f, -2.5f),  
		glm::vec3(-3.8f, -2.0f, -12.3f),  
		glm::vec3( 2.4f, -0.4f, -3.5f),  
		glm::vec3(-1.7f,  3.0f, -7.5f),  
		glm::vec3( 1.3f, -2.0f, -2.5f),  
		glm::vec3( 1.5f,  2.0f, -2.5f), 
		glm::vec3( 1.5f,  0.2f, -1.5f), 
		glm::vec3(-1.3f,  1.0f, -1.5f)  
	};

	// Makes sure draw calls are overlayed correctly with depth priority
	glEnable(GL_DEPTH_TEST);

	bool show_demo_window = false;
	static float im_x_rotation = 0.0f;
	static float im_y_rotation = 0.0f;
	static float im_z_rotation = 0.0f;
	static float im_xpos = 0.0f;
	static float im_ypos = 0.0f;
	static float im_perspective = 45.0f;
	glm::vec4 im_background_colour = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
	
	// Camera Variables
	bool cameraRotate = false;
	static float im_camera_rotation = 0.0f;
	static float cameraPos_Z = 3.0f;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, cameraPos_Z);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// Frame Timings
	float currentFrame = 0.0f;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// Shader projection view
	glm::mat4 projection    = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(im_perspective), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
	shader3.setMat4("projection", projection);

	// Capture the mouse like an fps style shooter
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	// The render loop
	while(!glfwWindowShouldClose(window)){
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window, deltaTime, &cameraPos, &cameraFront, &cameraUp);

		// Do ImGUi Things
		// Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

		// Show a simple window
		{
			ImGui::Begin("Control Panel");
			ImGui::Checkbox("Demo Window", &show_demo_window);
			ImGui::Checkbox("God Mode", &godmode);
			ImGui::SliderFloat("X Rotation (°)", &im_x_rotation, -180.0f, 180.0f);
			ImGui::SliderFloat("Y Rotation (°)", &im_y_rotation, -180.0f, 180.0f);
			ImGui::SliderFloat("Z Rotation (°)", &im_z_rotation, -180.0f, 180.0f);
			ImGui::SliderFloat("X Pos", &im_xpos, -10.0f, 10.0f);
			ImGui::SliderFloat("Y Pos", &im_ypos, -10.0f, 10.0f);
			ImGui::SliderFloat("Perspective Angle", &im_perspective, 30.0f, 90.0f);
			ImGui::SliderFloat("Camera Pos", &cameraPos_Z, 3.0f, 15.0f);
			ImGui::ColorEdit3("Background Colour", (float*)&im_background_colour);
			ImGui::Checkbox("Camera Rotate", &cameraRotate);
			if (cameraRotate){
				ImGui::SliderFloat("Camera Rotation (°)", &im_camera_rotation, -180.0f, 180.0f);
			}
			ImGui::Text("Camera Position:\nX: %0.2f\tY: %0.2f\tZ: %0.2f", (float)cameraPos.x, (float)cameraPos.y, (float)cameraPos.z);
			// ImGui::SameLine();
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// Rendering things
		glClearColor(im_background_colour.x, im_background_colour.y, im_background_colour.z, im_background_colour.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		 // ImGui Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

		// glfwGetCursorPos(window, &xpos, &ypos);
		// glm::vec2 mouse_pos = glm::vec2(xpos, ypos)/glm::vec2(windowWidth, windowHeight);
		// mouse_pos -=0.5f;
		// mouse_pos.y = -mouse_pos.y;
		// // localise bottom left of triangle to mouse pointer
		// mouse_pos.x -= triangle2[6];
		// mouse_pos.y += 0.5;
		// shader1.setVec2("mouse_pos", mouse_pos);
		// apply a rotation based on the mousewheel
		// glm::mat4 trans = glm::mat4(1.0f); // create identity matrix
		// trans = glm::rotate(trans, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));
		// trans = glm::rotate(trans, glm::radians((float)xpos), glm::vec3(0.0, 1.0, 0.0));
		// trans = glm::rotate(trans, glm::radians((float)ypos), glm::vec3(1.0, 0.0, 0.0));
		
		// Set the model view
		glm::mat4 model1 = glm::mat4(1.0f); // Create and identity matrix
		model1 = glm::rotate(model1, glm::radians(-55.0f), glm::vec3(1.0f, 0.0, 0.0));
		// glm::vec3 lame = glm::vec3(scroll, 0, 0);
		glm::vec3 lame = glm::vec3(im_xpos, im_ypos, 0);
		model1 = glm::translate(model1, lame);
		// model1 = glm::rotate(model1, (float)glfwGetTime()*glm::radians(50.0f)/1000.0f, glm::vec3(0.5f, 1.0f, 0.0f));
		model1 = glm::rotate(model1, glm::radians(im_x_rotation), glm::vec3(1.0f, 0.0f, 0.0f));
		model1 = glm::rotate(model1, glm::radians(im_y_rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		model1 = glm::rotate(model1, glm::radians(im_z_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		// shader1.setMat4("rotation", trans);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		shader3.use();
		
		// Setting up a camera system
		glm::mat4 view          = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		// view = glm::translate(view, cameraPos);
		if (cameraRotate){
			cameraPos.x = static_cast<float>(sin(glm::radians(im_camera_rotation))*cameraPos_Z);
			cameraPos.z = static_cast<float>(cos(glm::radians(im_camera_rotation))*cameraPos_Z);
			glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
			cameraUp = glm::cross(cameraDirection, cameraRight);
			view = glm::lookAt(cameraPos, cameraTarget, cameraUp); // Camera tracks towards the cameraTarget position
		}else{
			// cameraPos = glm::vec3(0.0f, 0.0f, cameraPos_Z);
			glm::vec3 direction;
			direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			direction.y = sin(glm::radians(pitch));
			direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			cameraFront = glm::normalize(direction);
			if (!godmode)
				cameraPos.y = 0.0f;
			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); // Camera looks at the space directly in front of it
		}

		// pass transformation matrices to the shader
		shader3.setMat4("view", view);
		// shader3.setMat4("model", model1);

		glBindVertexArray(VAOs[2]);
		// glDrawArrays(GL_TRIANGLES, 0, 36);
		for(unsigned int i=0; i < 10;i++){
			glm::mat4 model = glm::mat4(1.0f);
			if (i == 0){
				shader3.setMat4("model", model1);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				continue;
			}
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle)+(float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
            shader3.setMat4("model", model);
			
			glBindVertexArray(VAOs[2]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// De-allocate all resources once during shutdown
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	// glDeleteBuffers(1, &EBO);
	// glDeleteProgram(shaderProgram);

	// Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void create_buffers(unsigned int *VAO, unsigned int *VBO, float vertices[], unsigned int size){
	// This doesn't work - my guess is that it's because they're declared on the stack, but then idk how to fix
	glGenBuffers(1, VBO);
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, size, &vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, float deltaTime, glm::vec3 *cameraPos, glm::vec3 *cameraFront, glm::vec3 *cameraUp){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
	if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
		int polygonMode;
		glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
		if(polygonMode == GL_LINE){
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}if(polygonMode == GL_FILL){
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}

	const float cameraSpeed = 3.5f*deltaTime;
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		*cameraPos += cameraSpeed * *cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        *cameraPos -= cameraSpeed * *cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        *cameraPos -= glm::normalize(glm::cross(*cameraFront, *cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        *cameraPos += glm::normalize(glm::cross(*cameraFront, *cameraUp)) * cameraSpeed;		
}

void scrollScale(GLFWwindow *window, double xoffset, double yoffset){
	// rotation += yoffset;
	scroll += yoffset/100.0f;
}

void glfw_error_callback(int error, const char *description){
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void setWindowIcon(GLFWwindow *window, const char *path){
	GLFWimage images[1]; 
	images[0].pixels = stbi_load(path, &images[0].width, &images[0].height, 0, 4); //rgba channels 
	glfwSetWindowIcon(window, 1, images); 
	stbi_image_free(images[0].pixels);
}

GLFWwindow* initialiseOpenGLWindow(const char *programTitle, int windowHeight, int windowWidth,	const char *logoPath){
	
	glfwSetErrorCallback(glfw_error_callback);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window with GLFW
	GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, programTitle, NULL, NULL);
	if (window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Initialise Glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    	std::cout << "Failed to initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Set the size of the view port to be the same as the window
	// This sets the origin in the lower left corner, and 
	glViewport(0, 0, windowWidth, windowHeight);
	// Set the render frame to scale with the window as it gets resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	setWindowIcon(window, logoPath);

	return window;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos){
	if (firstMouse){
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = -ypos + lastY;
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if(pitch > 89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;

	yaw = glm::mod(yaw, 360.0f); // keep it constrained so our numbers don't get stupidly big
}