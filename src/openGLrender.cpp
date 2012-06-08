#include "openGLrender.h"


openGLrender::openGLrender()
{
	showNodes = true;
	showEdges = false;
	showMap = false;
	showShortcuts = false;
	mapCount = 16;
}

openGLrender::~openGLrender()
{
}

void openGLrender::setNodeArray(openGL_Node_3d *in_Nodes)
{
	//nodeArray = new openGL_Node_3d[nodeCount];
	nodeArray = in_Nodes;
}

void openGLrender::setEdgeArray(openGL_Edge_Node *in_Edges)
{
	//edgeArray = new openGL_Node_3d[edgeCount];
	edgeArray = in_Edges;
}

void openGLrender::setShortcutEdgeArray(openGL_Node_3d *in_Edges)
{
	//shortcutArray = new openGL_Node_3d[edgeCount];
	shortcutArray = in_Edges;
}

void openGLrender::setNodeCount(int count)
{
	this->nodeCount = count;
}

void openGLrender::setEdgeCount(int count)
{
	this->edgeCount = count;
}

void openGLrender::setShortcutEdgeCount(int count)
{
	this->shortcutCount = count;
}

void openGLrender::setCamera(float x,float y,float z)
{
   float pi = 3.1415926535821480865144288;
   float pos_radian = (pi/180.0)*y;
	float y_mercator = log((1.0 + sin(pos_radian))/cos(pos_radian));
	float x_mercator = (pi/180.0)*x;
	cameraPos = glm::vec3(x_mercator,y_mercator,z);
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
	//nodes shader program
	GLuint vShader;
	GLuint fShader;
	if ((fShader = loadShader("pferdFragment.glsl", GL_FRAGMENT_SHADER)) == 0)
	{ return false; };
	if ((vShader = loadShader("pferdVertex.glsl", GL_VERTEX_SHADER)) == 0)
	{ return false; };

	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glBindAttribLocation(program, 0, "in_position");
	glBindAttribLocation(program, 1, "in_color");

	glLinkProgram(program);
	glUseProgram(program);

	//edges shader program
	GLuint edge_vShader;
	GLuint edge_fShader;
	if ((edge_fShader = loadShader("pferdEdgeFragment.glsl", GL_FRAGMENT_SHADER)) == 0)
	{ return false; };
	if ((edge_vShader = loadShader("pferdEdgeVertex.glsl", GL_VERTEX_SHADER)) == 0)
	{ return false; };

	program1 = glCreateProgram();
	glAttachShader(program1, edge_vShader);
	glAttachShader(program1, edge_fShader);
	glBindAttribLocation(program1, 0, "in_position");
	glBindAttribLocation(program1, 1, "in_color");
	glBindAttribLocation(program1, 2, "in_normal");

	glLinkProgram(program1);
	glUseProgram(program1);

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

	//shortcut shader program
	GLuint sc_vShader;
	GLuint sc_fShader;
	if ((sc_fShader = loadShader("pferdEdgeFragment.glsl", GL_FRAGMENT_SHADER)) == 0)
	{ return false; };
	if ((sc_vShader = loadShader("pferdVertex.glsl", GL_VERTEX_SHADER)) == 0)
	{ return false; };

	program3 = glCreateProgram();
	glAttachShader(program3, sc_vShader);
	glAttachShader(program3, sc_fShader);
	glBindAttribLocation(program3, 0, "in_position");
	glBindAttribLocation(program3, 1, "in_color");

	glLinkProgram(program3);
	glUseProgram(program3);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(openGL_Edge_Node)*edgeCount, edgeArray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(openGL_Edge_Node), NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(openGL_Edge_Node),(GLvoid*) (sizeof(float) + sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(openGL_Edge_Node),(GLvoid*) (sizeof(float)*3));

	glBindVertexArray(0);

	return true;
}

bool openGLrender::initShortcuts()
{
	glGenVertexArrays(1, &vbo_shortcuts);
	glBindVertexArray(vbo_shortcuts);
	glGenBuffers(1, &vbo_shortcuts);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_shortcuts);
	glBufferData(GL_ARRAY_BUFFER, sizeof(openGL_Node_3d)*shortcutCount, shortcutArray, GL_STATIC_DRAW);
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
	texLoc[0] = "../data/48_4833357_825_875.png";
	texLoc[1] = "../data/48_4833357_875_925.png";
	texLoc[2] = "../data/48_4833357_925_975.png";
	texLoc[3] = "../data/48_4833357_975_1025.png";
	texLoc[4] = "../data/4833357_4866495_825_875.png";
	texLoc[5] = "../data/4833357_4866495_875_925.png";
	texLoc[6] = "../data/4833357_4866495_925_975.png";
	texLoc[7] = "../data/4833357_4866495_975_1025.png";
	texLoc[8] = "../data/4866495_489942_825_875.png";
	texLoc[9] = "../data/4866495_489942_875_925.png";
	texLoc[10] = "../data/4866495_489942_925_975.png";
	texLoc[11] = "../data/4866495_489942_975_1025.png";
	texLoc[12] = "../data/489942_4932125_825_875.png";
	texLoc[13] = "../data/489942_4932125_875_925.png";
	texLoc[14] = "../data/489942_4932125_925_975.png";
	texLoc[15] = "../data/489942_4932125_975_1025.png";

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

bool openGLrender::drawText(glm::mat4 mvp)
{
	/*glm::vec4 textPos;
	glColor3f(0.5f, 0.5f, 0.5f);
	float pi = 3.141592;
	float pos_radian;
	float y_mercator;
	float x_mercator;
	char* text;

	for(int i = 0; i < 10; i++)
	{
		textPos = glm::vec4(nodeArray[i].lon,nodeArray[i].lat,0.0,1.0);
		itoa(i,text,10);
		
		pos_radian = (pi/180.0)*textPos.y;
		y_mercator = log((1.0 + sin(pos_radian))/cos(pos_radian));
		x_mercator = (pi/180.0)*textPos.x;
		textPos = glm::vec4(x_mercator,y_mercator,0.0,1.0);

		textPos = mvp * textPos;

		glRasterPos2f((textPos.x/textPos.w),(textPos.y/textPos.w));
		glutBitmapString(GLUT_BITMAP_HELVETICA_12, (unsigned char*)text);
	}*/

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

void openGLrender::mouse(int x, int y)
{
	if(mouse_mode)
	{
		if(swap)
		{
			mouse_delta_x1 = x;
			mouse_delta_y1 = y;
			
			cameraPos = glm::vec3(cameraPos.x + ((float)(mouse_delta_x0-x)/900.0)*cameraPos.z, cameraPos.y + ((float)(y-mouse_delta_y0)/900.0)*cameraPos.z, cameraPos.z);

			swap = false;
			glutPostRedisplay();
		}
		else
		{
			mouse_delta_x0 = x;
			mouse_delta_y0 = y;
			
			cameraPos = glm::vec3(cameraPos.x + ((float)(mouse_delta_x1-x)/900.0)*cameraPos.z, cameraPos.y + ((float)(y-mouse_delta_y1)/900.0)*cameraPos.z, cameraPos.z);

			swap = true;
			glutPostRedisplay();
		}
	}
	else
	{
		if(swap)
		{
			mouse_delta_y1 = y;
			
			cameraPos = glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z + ((float)(y-mouse_delta_y0)/150.0)*cameraPos.z);

			swap = false;
			glutPostRedisplay();
		}
		else
		{
			mouse_delta_y0 = y;
			
			cameraPos = glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z + ((float)(y-mouse_delta_y1)/150.0)*cameraPos.z);

			swap = true;
			glutPostRedisplay();
		}
	}

}

void openGLrender::mouseCallback(int x, int y)
{
	currentInstance->mouse(x, y);
}

void openGLrender::mouseClick(int button, int state, int x, int y)
{
	if(state== GLUT_DOWN)
	{
		if(button == GLUT_LEFT_BUTTON)
		{
			if(swap)
			{
				mouse_delta_x0 = x;
				mouse_delta_y0 = y;
			}
			else
			{
				mouse_delta_x1 = x;
				mouse_delta_y1 = y;
			}
			mouse_mode = true;
		}
		if(button == GLUT_RIGHT_BUTTON)
		{
			if(swap)
			{
				mouse_delta_y0 = y;
			}
			else
			{
				mouse_delta_y1 = y;
			}
			mouse_mode = false;
		}
	}
}

void openGLrender::mouseClickCallback(int button, int state, int x, int y)
{
	currentInstance->mouseClick(button, state, x, y);
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
		case 's':
			showShortcuts = !showShortcuts;
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
	float fnear = 0.00001f;
    float fov = 45;
    float aspect = float(wWidth) / float(wHeight);
    projMX = glm::perspective(fov, aspect, fnear, 10000.0f);
	//set view matrix
	viewMX = glm::lookAt(cameraPos,cameraPos - glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//set model matrix
	modelMX = glm::mat4(1.0f);
	//set model_view_projection matrix
	
	/* TODO wird nicht verwendet */ 
	//glm::mat4 mvpMX = projMX * viewMX * modelMX;

	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, glm::value_ptr(viewMX));
	glUniformMatrix4fv(glGetUniformLocation(program, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projMX));
	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, glm::value_ptr(modelMX));
	//glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, glm::value_ptr(mvpMX));
   
   glPointSize(4.0);
   glLineWidth(2.5);
	
	if(showNodes)
	{
		glBindVertexArray(vbo_nodes);
		glDrawArrays(GL_POINTS, 0, nodeCount);
	}

	glUseProgram(program3);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, glm::value_ptr(viewMX));
	glUniformMatrix4fv(glGetUniformLocation(program, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projMX));
	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, glm::value_ptr(modelMX));
	if(showShortcuts)
	{
		glBindVertexArray(vbo_shortcuts);
		glDrawArrays(GL_LINES, 0, shortcutCount);
	}

	glUseProgram(program1);
	glUniformMatrix4fv(glGetUniformLocation(program1, "view_matrix"), 1, GL_FALSE, glm::value_ptr(viewMX));
	glUniformMatrix4fv(glGetUniformLocation(program1, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projMX));
	glUniformMatrix4fv(glGetUniformLocation(program1, "model_matrix"), 1, GL_FALSE, glm::value_ptr(modelMX));
	if(showEdges)
	{
		glBindVertexArray(vbo_edges);
		glDrawArrays(GL_LINES, 0, edgeCount);
	}

	//We don't want to draw labels with Glut atm
	//drawText(mvpMX);

	glUseProgram(program2);
	glUniformMatrix4fv(glGetUniformLocation(program2, "view_matrix"), 1, GL_FALSE, glm::value_ptr(viewMX));
	glUniformMatrix4fv(glGetUniformLocation(program2, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projMX));
	glUniformMatrix4fv(glGetUniformLocation(program2, "model_matrix"), 1, GL_FALSE, glm::value_ptr(modelMX));
/*	if(showMap)
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
*/
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

	if (!initShortcuts())
	{
		fprintf(stderr, "Couldn't initialize shortcuts geometry");
		return false;
	}
	else
	{
		fprintf(stderr, "Shortcuts geometry initialized...\n");
	}
	/*
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
	}*/


	setInstance(this);
	glutDisplayFunc(displayCallback);
	glutIdleFunc(idleCallback);
	glutReshapeFunc(resizeCallback);
	glutMotionFunc(mouseCallback);
	glutMouseFunc(mouseClickCallback);
	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(keyboardArrowsCallback);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutMainLoop();

	uninit();
	return true;
}
