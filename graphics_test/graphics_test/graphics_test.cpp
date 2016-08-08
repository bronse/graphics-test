// graphics_test.cpp : Defines the entry point for the application.
//

// 3rd party headers
#include "stdafx.h"
#include "glew.h"
#include <iostream>

// my headers
#include "create_window.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // // Place code here.

	initializeWindow();   /// create the window with our function
	
	SDL_GLContext mainContext = SDL_GL_CreateContext(window);

	// gl stuff
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_CreateContext(window); // changed this from the original

	// sync
	SDL_GL_SetSwapInterval(1);

	// glew stuff
	glewExperimental = GL_TRUE;
	glewInit();


	//--- set up shaders
	GLuint vertexShader, fragmentShader;
	//---
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//---
	const char * vertexShaderSource = "\nattribute vec2 position;\n\nvoid main(){\ngl_Position = vec4(position, 0.0, 1.0);\n}\n\0";
	//const char * fragmentShaderSource = "\nprecision highp float;\n\nvoid main(){\ngl_FragColor = vec4(100/gl_FragCoord.x, 100/gl_FragCoord.y, 0.0, 1.0);\n}\n\0";
	const char * fragmentShaderSource = "\nprecision highp float;\n\nvoid main(){\ngl_FragColor = vec4(gl_FragCoord.x*0.001, gl_FragCoord.y*0.001, 0.0, 1.0);\n}\n\0";
	//---
	glShaderSource(vertexShader, 1, &vertexShaderSource , NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//---
	int status;
	glCompileShader(vertexShader);                             /// compile the vertex shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);   /// check to see if the vertex shader compiled correctly
	if (!status) {
		return -1;
	}
	glCompileShader(fragmentShader);                           /// compile the fragment shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status); /// check to see if the fragment shader compiled correctly
	if (!status) {
		return -2;
	}
	//---
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		return -3;
	}
	glUseProgram(program);

	//--- set up vertex data
	float vertexBuffer[] = { -0.5f,  0.5f,
							 -0.5f, -0.5f,
						      0.5f,  0.5f,
						      0.5f, -0.5f };
	//---
	GLuint vbo, vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vertexBuffer, GL_STATIC_DRAW);

	//--- set up vertex attributes
	GLuint vertexHandle = glGetAttribLocation(program, "position");
	if (vertexHandle == -1) {
		return -4;
	}
	glEnableVertexAttribArray(vertexHandle);
	glVertexAttribPointer(vertexHandle, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	//--- do initial clear
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(window);

	glBindVertexArray(vao);

	SDL_Event event;      /// create an event that is inexplicably linked to the window we have created
	float count [3] = { 0.01, 0.03, 0.09 };
	float countMod [3] = { 0.001, 0.005, 0.007 };

	while (true) {
		SDL_PollEvent(&event);   /// poll events for the window
		for (int i = 0; i < 3; i++) {
			if (count[i] <= 0 || count[i] >= 1)
				countMod[i] *= -1;
			count[i] += countMod[i];
		}
		
		glClearColor(count[0], count[1], count[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		SDL_GL_SwapWindow(window);
	}

    return 0;
}
