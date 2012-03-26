#include "openGLrender.h"


openGLrender::openGLrender()
{
	showNodes = true;
	showEdges = false;
	showMap = false;
	mapCount = 16;
}

openGLrender::~openGLrender()
{
}

void openGLrender::setNodeArray(openGL_Node_3d *in_Nodes)
{
	nodeArray = new openGL_Node_3d[nodeCount];
	nodeArray = in_Nodes;
}

void openGLrender::setEdgeArray(openGL_Node_3d *in_Edges)
{
	edgeArray = new openGL_Node_3d[edgeCount];
	edgeArray = in_Edges;
}

void openGLrender::setNodeCount(int count)
{
	this->nodeCount = count;
}

void openGLrender::setEdgeCount(int count)
{
	this->edgeCount = count;
}

void openGLrender::setCamera(float x,float y,float z)
{
	cameraPos = glm::vec3(x,y,z);
}

char* openGLrender::file_read(const char* filename)
{
  FILE* in = fopen(filename, "rb");
  if (in == NULL) return NULL;

  int res_size = BUFSIZ;
  char* res = (char*)malloc(res_size);
  int nb_read_total = 0;

  while (!feof(in) && !ferror(in)) {
    if (nb_read_total + BUFSIZ > res_size) {
      if (res_size > 10*1024*1024) break;
      res_size = res_size * 2;
      res = (char*)realloc(res, res_size);
    }
    char* p_res = res + nb_read_total;
    nb_read_total += fread(p_res, 1, BUFSIZ, in);
  }
  
  fclose(in);
  res = (char*)realloc(res, nb_read_total + 1);
  res[nb_read_total] = '\0';
  return res;
}

GLint openGLrender::loadShader(const char* filename, GLenum type)
{
	const GLchar* shaderSource = file_read(filename);
	if (shaderSource == NULL)
	{
		fprintf(stderr, "Couldn't read shader sourcefile \" %s \" \n",filename);
		return false;
	}

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderSource, NULL);
	free((void*)shaderSource);

	glCompileShader(shader);
	GLint compile_ok = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
	if(compile_ok == GL_FALSE)
	{
		fprintf(stderr, "Couldn't compile shader %s \n",filename);
		glDeleteShader(shader);
		return false;
	}

	return shader;
}

bool openGLrender::initShaderProgram()
{
	//nodes & edges shader program
	GLuint vShader;
	GLuint fShader;
	if ((vShader = loadShader("pferdVertex.glsl", GL_VERTEX_SHADER)) == 0)
	{ return false; };
	if ((fShader = loadShader("pferdFragment.glsl", GL_FRAGMENT_SHADER)) == 0)
	{ return false; };

	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glBindAttribLocation(program, 0, "in_position");
	glBindAttribLocation(program, 1, "in_color");

	glLinkProgram(program);
	glUseProgram(program);

	//map shader program
	GLuint map_vShader;
	GLuint map_fShader;
	if ((map_vShader = loadShader("pferdMapVertex.glsl", GL_VERTEX_SHADER)) == 0)
	{ return false; };
	if ((map_fShader = loadShader("pferdMapFragment.glsl", GL_FRAGMENT_SHADER)) == 0)
	{ return false; };

	program2 = glCreateProgram();
	glAttachShader(program2, map_vShader);
	glAttachShader(program2, map_fShader);
	glBindAttribLocation(program2, 0, "in_position");
	glBindAttribLocation(program2, 1, "in_color");
	glBindAttribLocation(program2, 2, "in_uv_coord");

	glLinkProgram(program2);
	glUseProgram(program2);

	return true;
}

bool openGLrender::initNodes()
{
	glGenVertexArrays(1, &vbo_nodes);
	glBindVertexArray(vbo_nodes);
	glGenBuffers(1, &vbo_nodes);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_nodes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(openGL_Node_3d)*nodeCount, nodeArray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(openGL_Node_3d),NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(openGL_Node_3d),(GLvoid*) (sizeof(float)+sizeof(float)));

	glBindVertexArray(0);

	return true;
}

bool openGLrender::initEdges()
{
	glGenVertexArrays(1, &vbo_edges);
	glBindVertexArray(vbo_edges);
	glGenBuffers(1, &vbo_edges);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_edges);
	glBufferData(GL_ARRAY_BUFFER, sizeof(openGL_Node_3d)*edgeCount, edgeArray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(openGL_Node_3d), NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(openGL_Node_3d),(GLvoid*) (sizeof(float) + sizeof(float)));

	glBindVertexArray(0);

	return true;
}

bool openGLrender::initMap()
{
	openGL_quad *map = new openGL_quad[mapCount];

	map[0].vert1 = openGL_Node_3d_uv(8.25,48.0,1.0,0.0,0.0);
	map[0].vert2 = openGL_Node_3d_uv(8.75,48.0,1.0,1.0,0.0);
	map[0].vert3 = openGL_Node_3d_uv(8.25,48.33357,1.0,0.0,1.0);
	map[0].vert4 = openGL_Node_3d_uv(8.75,48.33357,1.0,1.0,1.0);

	map[1].vert1 = openGL_Node_3d_uv(8.75,48.0,1.0,0.0,0.0);
	map[1].vert2 = openGL_Node_3d_uv(9.25,48.0,1.0,1.0,0.0);
	map[1].vert3 = openGL_Node_3d_uv(8.75,48.33357,1.0,0.0,1.0);
	map[1].vert4 = openGL_Node_3d_uv(9.25,48.33357,1.0,1.0,1.0);

	map[2].vert1 = openGL_Node_3d_uv(9.25,48.0,1.0,0.0,0.0);
	map[2].vert2 = openGL_Node_3d_uv(9.75,48.0,1.0,1.0,0.0);
	map[2].vert3 = openGL_Node_3d_uv(9.25,48.33357,1.0,0.0,1.0);
	map[2].vert4 = openGL_Node_3d_uv(9.75,48.33357,1.0,1.0,1.0);

	map[3].vert1 = openGL_Node_3d_uv(9.75,48.0,1.0,0.0,0.0);
	map[3].vert2 = openGL_Node_3d_uv(10.25,48.0,1.0,1.0,0.0);
	map[3].vert3 = openGL_Node_3d_uv(9.75,48.33357,1.0,0.0,1.0);
	map[3].vert4 = openGL_Node_3d_uv(10.25,48.33357,1.0,1.0,1.0);

	map[4].vert1 = openGL_Node_3d_uv(8.25,48.33357,1.0,0.0,0.0);
	map[4].vert2 = openGL_Node_3d_uv(8.75,48.33357,1.0,1.0,0.0);
	map[4].vert3 = openGL_Node_3d_uv(8.25,48.66495,1.0,0.0,1.0);
	map[4].vert4 = openGL_Node_3d_uv(8.75,48.66495,1.0,1.0,1.0);

	map[5].vert1 = openGL_Node_3d_uv(8.75,48.33357,1.0,0.0,0.0);
	map[5].vert2 = openGL_Node_3d_uv(9.25,48.33357,1.0,1.0,0.0);
	map[5].vert3 = openGL_Node_3d_uv(8.75,48.66495,1.0,0.0,1.0);
	map[5].vert4 = openGL_Node_3d_uv(9.25,48.66495,1.0,1.0,1.0);

	map[6].vert1 = openGL_Node_3d_uv(9.25,48.33357,1.0,0.0,0.0);
	map[6].vert2 = openGL_Node_3d_uv(9.75,48.33357,1.0,1.0,0.0);
	map[6].vert3 = openGL_Node_3d_uv(9.25,48.66495,1.0,0.0,1.0);
	map[6].vert4 = openGL_Node_3d_uv(9.75,48.66495,1.0,1.0,1.0);

	map[7].vert1 = openGL_Node_3d_uv(9.75,48.33357,1.0,0.0,0.0);
	map[7].vert2 = openGL_Node_3d_uv(10.25,48.33357,1.0,1.0,0.0);
	map[7].vert3 = openGL_Node_3d_uv(9.75,48.66495,1.0,0.0,1.0);
	map[7].vert4 = openGL_Node_3d_uv(10.25,48.66495,1.0,1.0,1.0);

	map[8].vert1 = openGL_Node_3d_uv(8.25,48.66495,1.0,0.0,0.0);
	map[8].vert2 = openGL_Node_3d_uv(8.75,48.66495,1.0,1.0,0.0);
	map[8].vert3 = openGL_Node_3d_uv(8.25,48.9942,1.0,0.0,1.0);
	map[8].vert4 = openGL_Node_3d_uv(8.75,48.9942,1.0,1.0,1.0);

	map[9].vert1 = openGL_Node_3d_uv(8.75,48.66495,1.0,0.0,0.0);
	map[9].vert2 = openGL_Node_3d_uv(9.25,48.66495,1.0,1.0,0.0);
	map[9].vert3 = openGL_Node_3d_uv(8.75,48.9942,1.0,0.0,1.0);
	map[9].vert4 = openGL_Node_3d_uv(9.25,48.9942,1.0,1.0,1.0);

	map[10].vert1 = openGL_Node_3d_uv(9.25,48.66495,1.0,0.0,0.0);
	map[10].vert2 = openGL_Node_3d_uv(9.75,48.66495,1.0,1.0,0.0);
	map[10].vert3 = openGL_Node_3d_uv(9.25,48.9942,1.0,0.0,1.0);
	map[10].vert4 = openGL_Node_3d_uv(9.75,48.9942,1.0,1.0,1.0);

	map[11].vert1 = openGL_Node_3d_uv(9.75,48.66495,1.0,0.0,0.0);
	map[11].vert2 = openGL_Node_3d_uv(10.25,48.66495,1.0,1.0,0.0);
	map[11].vert3 = openGL_Node_3d_uv(9.75,48.9942,1.0,0.0,1.0);
	map[11].vert4 = openGL_Node_3d_uv(10.25,48.9942,1.0,1.0,1.0);

	map[12].vert1 = openGL_Node_3d_uv(8.25,48.9942,1.0,0.0,0.0);
	map[12].vert2 = openGL_Node_3d_uv(8.75,48.9942,1.0,1.0,0.0);
	map[12].vert3 = openGL_Node_3d_uv(8.25,49.32125,1.0,0.0,1.0);
	map[12].vert4 = openGL_Node_3d_uv(8.75,49.32125,1.0,1.0,1.0);

	map[13].vert1 = openGL_Node_3d_uv(8.75,48.9942,1.0,0.0,0.0);
	map[13].vert2 = openGL_Node_3d_uv(9.25,48.9942,1.0,1.0,0.0);
	map[13].vert3 = openGL_Node_3d_uv(8.75,49.32125,1.0,0.0,1.0);
	map[13].vert4 = openGL_Node_3d_uv(9.25,49.32125,1.0,1.0,1.0);

	map[14].vert1 = openGL_Node_3d_uv(9.25,48.9942,1.0,0.0,0.0);
	map[14].vert2 = openGL_Node_3d_uv(9.75,48.9942,1.0,1.0,0.0);
	map[14].vert3 = openGL_Node_3d_uv(9.25,49.32125,1.0,0.0,1.0);
	map[14].vert4 = openGL_Node_3d_uv(9.75,49.32125,1.0,1.0,1.0);

	map[15].vert1 = openGL_Node_3d_uv(9.75,48.9942,1.0,0.0,0.0);
	map[15].vert2 = openGL_Node_3d_uv(10.25,48.9942,1.0,1.0,0.0);
	map[15].vert3 = openGL_Node_3d_uv(9.75,49.32125,1.0,0.0,1.0);
	map[15].vert4 = openGL_Node_3d_uv(10.25,49.32125,1.0,1.0,1.0);


	glGenVertexArrays(1, &vbo_map);
	glBindVertexArray(vbo_map);
	glGenBuffers(1, &vbo_map);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_map);
	glBufferData(GL_ARRAY_BUFFER, sizeof(openGL_quad)*mapCount, map, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(openGL_Node_3d_uv), NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(openGL_Node_3d_uv), (GLvoid*) (sizeof(float) + sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(openGL_Node_3d_uv), (GLvoid*) (sizeof(float) + sizeof(float) + sizeof(float)));

	glBindVertexArray(0);
	
	return true;
}

bool openGLrender::initTextures()
{
	map_textures = new GLuint[mapCount];
	
	const char *texLoc[16];
	texLoc[0] = "48_4833357_825_875.png";
	texLoc[1] = "48_4833357_875_925.png";
	texLoc[2] = "48_4833357_925_975.png";
	texLoc[3] = "48_4833357_975_1025.png";
	texLoc[4] = "4833357_4866495_825_875.png";
	texLoc[5] = "4833357_4866495_875_925.png";
	texLoc[6] = "4833357_4866495_925_975.png";
	texLoc[7] = "4833357_4866495_975_1025.png";
	texLoc[8] = "4866495_489942_825_875.png";
	texLoc[9] = "4866495_489942_875_925.png";
	texLoc[10] = "4866495_489942_925_975.png";
	texLoc[11] = "4866495_489942_975_1025.png";
	texLoc[12] = "489942_4932125_825_875.png";
	texLoc[13] = "489942_4932125_875_925.png";
	texLoc[14] = "489942_4932125_925_975.png";
	texLoc[15] = "489942_4932125_975_1025.png";

	for(int i = 0; i < mapCount; i++)
	{
		map_textures[i] = SOIL_load_OGL_texture(
			texLoc[i],
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
			);

		if( 0 == map_textures[i] )
		{
			printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
			return false;
		}
	}

	return true;
}

void openGLrender::uninit()
{
	//delete nodes
	glDeleteBuffers(1, &vbo_nodes);
	glDeleteVertexArrays(1, &vbo_nodes);

	//delete edges
	glDeleteBuffers(1, &vbo_edges);
	glDeleteVertexArrays(1, &vbo_edges);

	//delete map
	glDeleteBuffers(1, &vbo_map);
	glDeleteVertexArrays(1, &vbo_map);
	glDeleteTextures(1, map_textures);
}

void openGLrender::keyboard (unsigned char key, int x, int y)
{
	switch( key ) 
	{
        case 'q': case 'Q':
            exit( EXIT_SUCCESS );
            break;
		case '+':
			cameraPos = cameraPos - glm::vec3(0.0f,0.0f,cameraPos.z/40.0f);
			glutPostRedisplay();
			break;
		case '-':
			cameraPos = cameraPos + glm::vec3(0.0f,0.0f,cameraPos.z/40.0f);
			glutPostRedisplay();
			break;
		case 'n':
			showNodes = !showNodes;
			glutPostRedisplay();
			break;
		case 'e':
			showEdges = !showEdges;
			glutPostRedisplay();
			break;
		case 'm':
			showMap = !showMap;
			glutPostRedisplay();
			break;
	}
}

void openGLrender::keyboardArrows(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_LEFT:
		cameraPos = cameraPos - glm::vec3(cameraPos.z/100.0f,0.0f,0.0f);
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		cameraPos = cameraPos + glm::vec3(cameraPos.z/100.0f,0.0f,0.0f);
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		cameraPos = cameraPos + glm::vec3(0.0f,cameraPos.z/100.0f,0.0f);
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		cameraPos = cameraPos - glm::vec3(0.0f,cameraPos.z/100.0f,0.0f);
		glutPostRedisplay();
		break;
	}
}

void openGLrender::keyboardCallback(unsigned char key, int x, int y)
{
	currentInstance -> keyboard(key,x,y);
}

void openGLrender::keyboardArrowsCallback(int key, int x, int y)
{
	currentInstance -> keyboardArrows(key, x, y);
}

void openGLrender::display()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	//set projection matrix
	float fnear = 0.001f;
    float fov = 45;
    float aspect = float(wWidth) / float(wHeight);
    projMX = glm::perspective(fov, aspect, fnear, 10000.0f);
	//set view matrix
	viewMX = glm::lookAt(cameraPos,cameraPos - glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//set model matrix
	modelMX = glm::mat4(1.0f);
	//set model_view_projection matrix
	//glm::mat4 mvpMX = projMX * viewMX * modelMX;

	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, glm::value_ptr(viewMX));
	glUniformMatrix4fv(glGetUniformLocation(program, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projMX));
	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, glm::value_ptr(modelMX));
	//glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, glm::value_ptr(mvpMX));
	
	if(showNodes)
	{
		glBindVertexArray(vbo_nodes);
		glDrawArrays(GL_POINTS, 0, nodeCount);
	}
	if(showEdges)
	{
		glBindVertexArray(vbo_edges);
		glDrawArrays(GL_LINES, 0, edgeCount);
	}

	glUseProgram(program2);
	glUniformMatrix4fv(glGetUniformLocation(program2, "view_matrix"), 1, GL_FALSE, glm::value_ptr(viewMX));
	glUniformMatrix4fv(glGetUniformLocation(program2, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projMX));
	glUniformMatrix4fv(glGetUniformLocation(program2, "model_matrix"), 1, GL_FALSE, glm::value_ptr(modelMX));
	if(showMap)
	{
		glEnable( GL_TEXTURE_2D );
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glBindVertexArray(vbo_map);

		//draw the available map tiles
		for(int i=0; i<mapCount; i++)
		{
			glBindTexture(GL_TEXTURE_2D, map_textures[i]);
			GLint locTex = glGetUniformLocation(program2, "mytexture");
			glUniform1i(locTex, 0);
			glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
		}
	}
	else
	{
		//disabling depth test when not needed speeds up performance
		glDisable(GL_DEPTH_TEST);
	}

	glutSwapBuffers();
}

void openGLrender::displayCallback()
	{
		currentInstance -> display();
	}

void openGLrender::idle()
{
	glutPostRedisplay();
}

void openGLrender::idleCallback()
{
	currentInstance -> idle();
}

void openGLrender::resize(int w, int h)
{
	glViewport( 0, 0, (GLsizei)w, (GLsizei)h );
    wWidth = w;
    wHeight = h;
}

void openGLrender::resizeCallback(int w, int h)
{
	currentInstance->resize(w,h);
}

openGLrender* openGLrender::currentInstance = 0;

void openGLrender::setInstance(openGLrender* instance)
{
	currentInstance = instance;
}

bool openGLrender::start(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Pferd");

	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK)
	{
		return false;
	}

	if (!initShaderProgram())
	{
		fprintf(stderr, "Couldn't initialize shaders\n");
		return false;
	}
	else
	{
		fprintf(stderr, "Shaders initialized...\n");
	}

	if (!initNodes())
	{
		fprintf(stderr, "Couldn't initialize node geometry");
		return false;
	}
	else
	{
		fprintf(stderr, "Node geometry initialized...\n");
	}

	if (!initEdges())
	{
		fprintf(stderr, "Couldn't initialize edge geometry");
		return false;
	}
	else
	{
		fprintf(stderr, "Edge geometry initialized...\n");
	}

	if (!initMap())
	{
		fprintf(stderr, "Couldn't initialize map geometry");
		return false;
	}
	else
	{
		fprintf(stderr, "Map geometry initialized...\n");
	}

	if (!initTextures())
	{
		fprintf(stderr, "Couldn't initialize textures");
		return false;
	}
	else
	{
		fprintf(stderr, "Map textures initialized...\n");
	}


	setInstance(this);
	glutDisplayFunc(displayCallback);
	glutIdleFunc(idleCallback);
	glutReshapeFunc(resizeCallback);
	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(keyboardArrowsCallback);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutMainLoop();

	uninit();
	return true;
}