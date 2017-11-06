// ==========================================================================
// An Object-Oriented Boilerplate Code for GLFW
//
// Author:  Kamyar Allahverdi, University of Calgary. Minor tweaks by Haysn Hornbeck.
// Date:    January 2017
// ==========================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shared.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <unistd.h>

#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

//#include <GL/gl.h> 
//#include <GL/glu.h> 

#include "camera.h"



using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;


int width, height;




// check if the last command resulted in an error
bool errorCheck( string t ) {
	
		// retrieve the error code
		GLenum errorCode	= glGetError();
	
		// exit early, if nothing's wrong
		if ( errorCode == GL_NO_ERROR )
			return false;
	
		// while we have errors
		while( errorCode != GL_NO_ERROR ) {
	
			string errorMsg;
			switch( errorCode ) {
	
				case GL_INVALID_ENUM:
	
					errorMsg = "Invalid GLenum";
					break;
	
				case GL_INVALID_VALUE:
	
					errorMsg = "Illegal value";
					break;
	
				case GL_INVALID_OPERATION:
	
					errorMsg = "Invalid operation";
					break;
	
				case GL_STACK_OVERFLOW:
	
					errorMsg = "Stack overflow";
					break;
	
				case GL_STACK_UNDERFLOW:
	
					errorMsg = "Removal from empty stack";
					break;
	
				case GL_OUT_OF_MEMORY:	// this one is special
	
					cerr << "ERROR: Out of memory";
					if ( !t.empty() )
						cerr << " at " << t;
					cerr << endl;
	
					// die here? OOM is usually fatal
	
					errorCode = glGetError();
					continue;
	
				case GL_INVALID_FRAMEBUFFER_OPERATION:
	
					errorMsg = "Invalid framebuffer operation";
					break;
	
				case GL_CONTEXT_LOST:
	
					errorMsg = "Graphics context lost";
					break;
	
				default:
	
					errorMsg = "Unknown error";
					break;
	
				}
	
			// actually print out the error message
			cerr << "WARNING: " << errorMsg;
			if ( !t.empty() )
				cerr << " at " << t;
			cerr << endl;
	
			// keep processing, there may be more
			errorCode = glGetError();
			}
	
		// we can only reach this line if an error was detected
		return true;
	
	}
	

static float zoomFactor = 10; /* Global, if you want. Modified by user input. Initially 1.0 */ 

/* A routine for setting the projection matrix. May be called from a resize event handler in a typical application. 
Takes integer width and height dimensions of the drawing area. 
Creates a projection matrix with correct aspect ratio and zoom factor. */
// void setProjectionMatrix (int width, int height) 
// { 
// 	glMatrixMode(GL_PROJECTION); 
// 	/* ...Where 'zNear' and 'zFar' are up to you to fill in. */ 
// 	glLoadIdentity(); 
// 	gluPerspective (50.0*zoomFactor, (float)width/(float)height, 1.0, 20.0); 
// 	glMatrixMode (GL_MODELVIEW);
// }



vector<float> BaseVertex = { 
  -1, -1,
  -1, 1, 
   1, -1,
  -1, 1,
   1, 1,  
   1, -1
};


float g_color_buffer_data[50] = {
    1.0, 1.0, 1.0, 1.0
};

vector<float> scale = {
	1, 1,
	1, 1, 
	1, 1,
	1, 1,
	1, 1,  
	1, 1
};

vector<float> grayscale = {
	0, 0,
	0, 0, 
	0, 0,
	0, 0,
	0, 0,  
	0, 0
};

vector<float> xSpeed = {
	0, 0,
	0, 0, 
	0, 0,
	0, 0,
	0, 0,  
	0, 0
};
vector<float> ySpeed = {
	0, 0,
	0, 0, 
	0, 0,
	0, 0,
	0, 0,  
	0, 0
};

vector<float> xSpeedInit = {
	0, 0,
	0, 0, 
	0, 0,
	0, 0,
	0, 0,  
	0, 0
};
vector<float> ySpeedInit = {
	0, 0,
	0, 0, 
	0, 0,
	0, 0,
	0, 0,  
	0, 0
};

vector<float> frag2Bit = {
	0, 0,
	0, 0, 
	0, 0,
	0, 0,
	0, 0,  
	0, 0
};



// float xpos, ypos;

double down_xpos, down_ypos;

double init_xpos, init_ypos;



bool isMouseDown = false;

//float up_xpos, up_ypos;


// Get Cursor position
static void cursorPositionCallback (GLFWwindow *window, double xPos, double yPos)
{
	// float wPartition = width/3;
	// float hPartition = height/3;
	
	if (isMouseDown)
	{
		for (unsigned int i = 0; i < xSpeed.size(); i++)
		{
			
			xSpeed[i] = (xSpeedInit[i] + ((xPos - down_xpos)/(0.5*width)));				

			cout << xSpeed[i] << endl;		
		}

		for (unsigned int i = 0; i < ySpeed.size(); i++)
		{

			ySpeed[i] = ySpeedInit[i] - ((yPos - down_ypos)/(0.5*height));				

			cout << ySpeed[i] << endl;		
		}
	}

}



// Use this to draw point later probably
static void mouseClickCallback (GLFWwindow *window, int button, int action, int mods)
{
	double xpos, ypos;
	
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && action != GLFW_RELEASE)
	{	
		
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout << "xPos at click: " << xpos  << endl;
		std::cout << "yPos at click: " << ypos  << endl;	
		down_xpos = xpos;
		down_ypos = ypos;	
		
		xSpeedInit = xSpeed;
		ySpeedInit = ySpeed;
		
		isMouseDown = true;
	}

		
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{	
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout << "xPos at click release: " << xpos  << endl;
		std::cout << "yPos at click release: " << ypos  << endl;	
		isMouseDown = false;
	}


	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		std::cout << "Left Click" << endl;
	}
}


class Program {

public:
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint id;
	Program() {
		vertex_shader = 1;
		fragment_shader = 2;
		id = 0;
	}
	Program(string vertex_path, string fragment_path) {
		init(vertex_path, fragment_path);
	}
	void init(string vertex_path, string fragment_path) {
		id = glCreateProgram();
		vertex_shader = addShader(vertex_path, GL_VERTEX_SHADER);
		fragment_shader = addShader(fragment_path, GL_FRAGMENT_SHADER);
		if (vertex_shader)
			glAttachShader(id, vertex_shader);
		if (fragment_shader)
			glAttachShader(id, fragment_shader);
			
		glLinkProgram(id);
		
	}
	GLuint addShader(string path, GLuint type) {
		std::ifstream in(path);
		string buffer = [&in] {
			std::ostringstream ss {};
			ss << in.rdbuf();
			return ss.str();
		}();
		const char *buffer_array[] = { buffer.c_str() };

		GLuint shader = glCreateShader(type);

		glShaderSource(shader, 1, buffer_array, 0);
		glCompileShader(shader);

		// Compile results
		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE) {
			GLint length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			string info(length, ' ');
			glGetShaderInfoLog(shader, info.length(), &length, &info[0]);
			cerr << "ERROR compiling shader:" << endl << endl;
			cerr << info << endl;
		}
		return shader;
	}
	~Program() {
		glUseProgram(0);
		glDeleteProgram(id);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}
};

class VertexArray {
	std::map<string, GLuint> buffers;
	std::map<string, int> indices;
public:
	GLuint id;
	unsigned int count;
	VertexArray(int c) {
		glGenVertexArrays(1, &id);
		count = c;
	}

	VertexArray(const VertexArray &v) {
		glGenVertexArrays(1, &id);

		// Copy data from the old object
		this->indices = std::map<string, int>(v.indices);
		count = v.count;

		vector<GLuint> temp_buffers(v.buffers.size());

		// Allocate some temporary buffer object handles
		glGenBuffers(v.buffers.size(), &temp_buffers[0]);

		// Copy each old VBO into a new VBO
		int i = 0;
		for (auto &ent : v.buffers) {
			int size = 0;
			glBindBuffer(GL_ARRAY_BUFFER, ent.second);
			glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

			glBindBuffer(GL_COPY_READ_BUFFER, temp_buffers[i]);
			glBufferData(GL_COPY_READ_BUFFER, size, NULL, GL_STATIC_COPY);

			glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_READ_BUFFER, 0, 0,
					size);
			i++;
		}

		// Copy those temporary buffer objects into our VBOs

		i = 0;
		for (auto &ent : v.buffers) {
			GLuint buffer_id;
			int size = 0;
			int index = indices[ent.first];

			glGenBuffers(1, &buffer_id);

			glBindVertexArray(this->id);
			glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
			glBindBuffer(GL_COPY_READ_BUFFER, temp_buffers[i]);
			glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);

			// Allocate VBO memory and copy
			glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0,
					size);
			string indexs = ent.first;

			buffers[ent.first] = buffer_id;
			indices[ent.first] = index;

			// Setup the attributes
			size = size / (sizeof(float) * this->count);
			glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(index);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			i++;
		}

		// Delete temporary buffers
		glDeleteBuffers(v.buffers.size(), &temp_buffers[0]);
	}

	void addBuffer(string name, int index, vector<float> buffer) {
		GLuint buffer_id;
		glBindVertexArray(id);

		glGenBuffers(1, &buffer_id);
		glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float),
				buffer.data(), GL_STATIC_DRAW);
		buffers[name] = buffer_id;
		indices[name] = index;

		int components = buffer.size() / count;
		glVertexAttribPointer(index, components, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(index);

		// unset states
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void updateBuffer(string name, vector<float> buffer) {
		glBindBuffer(GL_ARRAY_BUFFER, buffers[name]);
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float),
				buffer.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	~VertexArray() {
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		for (auto &ent : buffers)
			glDeleteBuffers(1, &ent.second);
	}
};

void renderPoints(Program &program, VertexArray &va) {
	//clear screen to a dark grey colour
	// glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	// glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program.id);
	glBindVertexArray(va.id);
	glDrawArrays(GL_POINT, 0, va.count);

	glBindVertexArray(0);
	glUseProgram(0);

}



void render(Program &program, VertexArray &va) {
	//clear screen to a dark grey colour
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program.id);
	glBindVertexArray(va.id);
	glDrawArrays(GL_TRIANGLES, 0, va.count);

	glBindVertexArray(0);
	glUseProgram(0);

}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Zoom in a level
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{	
		for (unsigned int i = 0; i < scale.size(); i++)
		{
			scale[i] = scale[i] * (1.2);
			
		}
	} 

	// Zoom out a level
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		for (unsigned int i = 0; i < scale.size(); i++)
		{
			scale[i] = scale[i] * (0.8);
			
		}
	} 


	// turn screen gray
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		for (unsigned int i = 0; i < grayscale.size(); i++)
		{
			if (grayscale[i] == 0)
			{	
				grayscale[i] = 1;	
			}
			
			else
			{
				grayscale[i] = 0;				
			}
		}
	} 

	// turn screen 2Bit
	if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		for (unsigned int i = 0; i < frag2Bit.size(); i++)
		{
			if (frag2Bit[i] == 0)
			{	
				frag2Bit[i] = 1;	
			}
			
			else
			{
				frag2Bit[i] = 0;				
			}
		}
	} 





	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{		
		for (unsigned int i = 0; i < xSpeed.size(); i++)
		{

			xSpeed[i] += 0.1;				

			cout << xSpeed[i] << endl;		
		}
		
	}



	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{		
		for (unsigned int i = 0; i < xSpeed.size(); i++)
		{

			xSpeed[i] -= 0.1;				

			cout << xSpeed[i] << endl;		
		}
		
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{		
		for (unsigned int i = 0; i < ySpeed.size(); i++)
		{

			ySpeed[i] -= 0.1;				

			cout << ySpeed[i] << endl;		
		}
		
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{		
		for (unsigned int i = 0; i < ySpeed.size(); i++)
		{

			ySpeed[i] += 0.1;				

			cout << ySpeed[i] << endl;		
		}
		
	}

}
	
int main(const int argc, const char *argv[]) {

	// initialize the GLFW windowing system
	if (!glfwInit()) {
		cout << "ERROR: GLFW failed to initialize, TERMINATING" << endl;
		return -1;
	}
	glfwSetErrorCallback([](int error, const char* description) {
		cout << "GLFW ERROR " << error << ":" << endl;
		cout << description << endl;
	});

	int channels;
	stbi_set_flip_vertically_on_load(true);
	//float* data = stbi_loadf("./sintel.png", &width, &height, &channels, 0);
	unsigned char* data = stbi_load("./sintel.png", &width, &height, &channels, 0);
	
	// glMatrixMode(GL_PROJECTION);
	// glLoadIdentity();
	// glOrtho(-width/2, width/2, height/2,-height/2, -1, 1);
	// glMatrixMode(GL_MODELVIEW);

	// glScalef( 2.0, 2.0, 1.0 );


	// Use OpenGL 4.1
	GLFWwindow *window = 0;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(width, height, "CPSC 453 OpenGL Boilerplate", 0, 0);
	if (!window) {
		cout << "Program failed to create GLFW window, TERMINATING" << endl;
		glfwTerminate();
		return -1;
	}


	glfwMakeContextCurrent(window);
	
	glfwSetCursorPosCallback(window, cursorPositionCallback);

	glfwSetMouseButtonCallback(window, mouseClickCallback);
	

	glfwSetKeyCallback(window, key_callback);
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

	// Create Camera 
	// Position
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);  

	// Camera direction
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	// Points in the oppposite direction that you might think
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	// Get X axis
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	// Get y axis
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	// Use these values to create a "Look at" matrix

	glm::mat4 view;
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), 
				 glm::vec3(0.0f, 0.0f, 0.0f), 
				 glm::vec3(0.0f, 1.0f, 0.0f));
	errorCheck("");
				 
//////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////



	//GLuint image = loadBMP_custom("./my_texture.bmp");
	// Load image
	//int x,y,n;
	Program p("data/vertex.glsl", "data/fragment.glsl");
	
	// Program for drawing spline 
	Program p2("data/vertex.glsl", "data/fragment.glsl");
	
	//bool RGB = true;

	GLuint textureID;
	

    if (data == NULL) {
        // error
        cout << "Error, data was null";
    } else {
		// process
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);

		glGenTextures(1, &textureID);

		
		glBindTexture(GL_TEXTURE_2D, textureID);

		// width = width/10;
		// height = height/10;
		// Send texture to GPU
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);


		cout << p.id << endl;
		
		//glUniform1i(glGetUniformLocation(p.id, "texture") , 0);
		

        cout << width << height << endl << endl;
	}
	
	stbi_image_free(data);

    //GLint texAttrib = glGetAttribLocation(p.id, "texcoord");


	// Apply camera stuff

	Camera camera (glm::vec3(0,0,-3), 70.0f, (float)width/(float)height, 0.01f, 1000.0f);


	//va.addBuffer("v", 0, BaseVertex);

	//VertexArray baseCase(6);



	// run an event-triggered main loop
	while (!glfwWindowShouldClose(window)) {

		
		float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));  
		errorCheck("");
		
		// multiply I guess?


		VertexArray va(6);

		VertexArray points(6);

		points.addBuffer("r", 0, BaseVertex);
		
		
		va.addBuffer("v", 0, BaseVertex);

		va.addBuffer("s", 1, scale);

		va.addBuffer("g", 2, grayscale);
		
		va.addBuffer("x", 3, xSpeed);

		va.addBuffer("y", 4, ySpeed);

		va.addBuffer("f", 5, frag2Bit);
		
		
		render(p, va);
		
		//renderPoints(p, va);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	cout << "The End" << endl;
	return 0;
}
