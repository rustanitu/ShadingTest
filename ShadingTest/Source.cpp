#include <stdio.h>
#include <GL\glew.h>
#include <GL\freeglut.h>

static int p_id = -1;

static const char*vsource =
"in vec4 myVertex;"
"in vec4 color;"

"out vec4 colorV;"
"void main (void)"
"{ "
" colorV = color;"
" gl_Position = myVertex; "
"}";

static const char*fsource =
"in vec4 color;"
"void main (void) "
"{"
" gl_FragColor = color;"
"}";

void Error(const char* err)
{
	if (err)
		printf("%s\n", err);
}

static void CompileShader(GLuint id)
{
	GLint status;
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	if (!status) {
		GLint len;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
		char* message = (char*) malloc(len*sizeof(char));
		glGetShaderInfoLog(id, len, 0, message);
		Error(message);
		free(message);
	}
}

static void LinkProgram(GLuint id)
{
	GLint status;
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &status);
	if (!status) {
		GLint len;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
		char* message = (char*)malloc(len*sizeof(char));
		glGetProgramInfoLog(id, len, 0, message);
		Error(message);
		free(message);
	}
}

// Cria programa de shader //
static void CreateShaderProgram()
{
	// vertex shader //
	GLuint v_id = glCreateShader(GL_VERTEX_SHADER);

	if (v_id == 0)
		Error("Could not create vertex shader object");

	glShaderSource(v_id, 1, &vsource, 0);
	CompileShader(v_id);

	// fragment shader */
	GLuint f_id = glCreateShader(GL_FRAGMENT_SHADER);
	if (f_id == 0)
		Error("Could not create fragment shader object");
	
	glShaderSource(f_id, 1, &fsource, 0);
	CompileShader(f_id);
	
	// program */
	p_id = glCreateProgram();
	if (p_id == 0)
		Error("Could not create program object");
	
	glBindAttribLocation(p_id, 0, "myVertex");
	glBindAttribLocation(p_id, 1, "color");
	glAttachShader(p_id, v_id);
	glAttachShader(p_id, f_id);
	LinkProgram(p_id);
}

// Initialization function
static void Init()
{
	// open GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		exit(!1);
	}
	// init OpenGL state
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	CreateShaderProgram();
}

// Reshape callback
static void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

// Draw scene //
static void DrawScene()
{
	static float coord[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	static float color[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	glUseProgram(p_id);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, coord);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, color);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

// Display callback
static void Display(void)
{
	int vp[4]; glGetIntegerv(GL_VIEWPORT, vp);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw scene
	DrawScene();
	glutSwapBuffers();
}

// Main function
int main(int argc, char* argv[])
{
	// open GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize(800, 600);
	
	// create window
	glutCreateWindow("Triangle");
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	
	// initiate OpenGL context
	Init();
	
	// interact...
	glutMainLoop();
	return 0;
}