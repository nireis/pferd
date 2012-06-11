#include "openGLrender.h"


openGLrender::openGLrender()
{
	mapCount = 16;
	camZoom = 1.0;
}

openGLrender::~openGLrender()
{
}

/*
*	Graph rendering methods
*/
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

void openGLrender::setClusterArray(openGL_Cluster *in_Clusters)
{
	clusterArray = in_Clusters;
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

void openGLrender::setClusterCount(int count)
{
	this->clusterCount = count;
}

void openGLrender::setCamera(float x,float y,float z)
{
   float pi = 3.1415926535821480865144288;
   float pos_radian = (pi/180.0)*y;
	float y_mercator = log((1.0 + sin(pos_radian))/cos(pos_radian));
	float x_mercator = (pi/180.0)*x;
	cameraPos = glm::vec3(x_mercator,y_mercator,z);
}

bool openGLrender::initOpenGL_Node_3d(GLuint* vbo, openGL_Node_3d* nodeData, int size)
{
	glGenVertexArrays(1, vbo);
	glBindVertexArray(*vbo);
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(openGL_Node_3d)*size, nodeData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(openGL_Node_3d),NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(openGL_Node_3d),(GLvoid*) (sizeof(float)+sizeof(float)));

	glBindVertexArray(0);

	return true;
}

bool openGLrender::initOpenGL_Edge_Node(GLuint *vbo, openGL_Edge_Node* nodeData, int size)
{
	glGenVertexArrays(1, vbo);
	glBindVertexArray(*vbo);
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(openGL_Edge_Node)*size, nodeData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(openGL_Edge_Node), NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(openGL_Edge_Node),(GLvoid*) (sizeof(float) + sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(openGL_Edge_Node),(GLvoid*) (sizeof(float)*3));

	glBindVertexArray(0);

	return true;
}

bool openGLrender::initOpenGL_Cluster(GLuint *vbo, openGL_Cluster clusterData)
{

	openGL_Node_3d tempVertData[10];
	tempVertData[0] = openGL_Node_3d(0.0,0.0,clusterData.color);

	for(int i=0; i<9; i++)
	{	
		tempVertData[i+1] =
			openGL_Node_3d( clusterData.radius*glm::cos(((float)i/4.0)*3.141592),
							clusterData.radius*glm::sin(((float)i/4.0)*3.141592),
							clusterData.color);
	}

	glGenVertexArrays(1, vbo);
	glBindVertexArray(*vbo);
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(openGL_Node_3d)*10, tempVertData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(openGL_Node_3d),NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(openGL_Node_3d),(GLvoid*) (sizeof(float)+sizeof(float)));

	glBindVertexArray(0);

	return true;
}

/*
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
	glm::vec4 textPos;
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
	}

	return true;
}*/

bool openGLrender::initGraphVis()
{
	entityCount = 3 + clusterCount;
	sceneEntities = new openGL_Entity[entityCount];

	//init edges
	sceneEntities[0] = openGL_Entity();
	initOpenGL_Edge_Node(&(sceneEntities[0].vbo_handler),edgeArray,edgeCount);
	sceneEntities[0].geometryType = GL_LINES;
	sceneEntities[0].numElements = edgeCount;
	const char* atrb0[] = { "in_position", "in_color", "in_normal" };
	// [0]: Fragment, [1] Vertex
	const char* shader0[] = 
		{ "pferdEdgeFragment.glsl", "pferdEdgeVertex.glsl" };
	sceneEntities[0].shader_program = initShaderProgram(shader0[0] , shader0[1], atrb0, 3);
	glLinkProgram(sceneEntities[0].shader_program);
	sceneEntities[0].texture = 0;
	sceneEntities[0].visabilty = true;
	sceneEntities[0].world_position = glm::vec3(0.0);

	//init nodes
	sceneEntities[1] = openGL_Entity();
	initOpenGL_Node_3d(&(sceneEntities[1].vbo_handler),nodeArray,nodeCount);
	sceneEntities[1].geometryType = GL_POINTS;
	sceneEntities[1].numElements = nodeCount;
	const char* atrb1[] = { "in_position", "in_color" };
	// [0]: Fragment, [1] Vertex
	const char* shader1[] = 
		{ "pferdFragment.glsl", "pferdVertex.glsl" };
	sceneEntities[1].shader_program = initShaderProgram(shader1[0] , shader1[1], atrb1, 2);
	glLinkProgram(sceneEntities[1].shader_program);
	sceneEntities[1].texture = 0;
	sceneEntities[1].visabilty = true;
	sceneEntities[1].world_position = glm::vec3(0.0);

	//init shortcuts
	sceneEntities[2] = openGL_Entity();
	initOpenGL_Node_3d(&(sceneEntities[2].vbo_handler),shortcutArray,shortcutCount);
	sceneEntities[2].geometryType = GL_LINES;
	sceneEntities[2].numElements = shortcutCount;
	const char* atrb2[] = { "in_position", "in_color" };
	// [0]: Fragment, [1] Vertex
	const char* shader2[] = 
		{ "pferdShortcutFragment.glsl", "pferdShortcutVertex.glsl" };
	sceneEntities[2].shader_program = initShaderProgram(shader2[0] , shader2[1], atrb2, 2);
	glLinkProgram(sceneEntities[2].shader_program);
	sceneEntities[2].texture = 0;
	sceneEntities[2].visabilty = true;
	sceneEntities[2].world_position = glm::vec3(0.0);

	for(int i=0; i<clusterCount; i++)
	{
		//init cluster-highlight
		sceneEntities[i+3] = openGL_Entity();
		initOpenGL_Cluster(&(sceneEntities[i+3].vbo_handler),clusterArray[i]);
		sceneEntities[i+3].geometryType = GL_TRIANGLE_FAN;
		sceneEntities[i+3].numElements = 10;
		const char* atrb3[] = { "in_position", "in_color" };
		// [0]: Fragment, [1] Vertex
		const char* shader3[] = 
			{ "pferdClusterFragment.glsl", "pferdClusterVertex.glsl" };
		sceneEntities[i+3].shader_program = initShaderProgram(shader3[0] , shader3[1], atrb3, 2);
		glLinkProgram(sceneEntities[i+3].shader_program);
		sceneEntities[i+3].texture = 0;
		sceneEntities[i+3].visabilty = true;
		sceneEntities[i+3].world_position = glm::vec3(clusterArray[i].xCenter,clusterArray[i].yCenter,0.0);
	}

	return true;
}

void openGLrender::displayGraph()
{
	//set projection matrix
	float fnear = 0.00001f;
	float ffar = 10000.0f;
	//projMX = glm::perspective(fov, aspect, fnear, 10000.0f);
	projMX = glm::ortho(-(float)wWidth/(8000.0f*camZoom),(float)wWidth/(8000.0f*camZoom),-(float)wHeight/(8000.0f*camZoom),(float)wHeight/(8000.0f*camZoom),fnear,ffar);
	//set view matrix
	viewMX = glm::lookAt(cameraPos,cameraPos - glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//set model matrix
	modelMX = glm::mat4(1.0f);
	//set model_view_projection matrix
	glm::mat4 mvpMX = projMX * viewMX * modelMX;

	glEnable(GL_DEPTH_TEST);
	glLineWidth(2.0);
	glPointSize(4.0);

	for(int i=0; i<entityCount; i++)
	{
		if(sceneEntities[i].visabilty)
		{
			//set model matrix
			modelMX = glm::translate(glm::mat4(1.0f), sceneEntities[i].world_position);
			//set model_view_projection matrix
			glm::mat4 mvpMX = projMX * viewMX * modelMX;

			glUseProgram(sceneEntities[i].shader_program);
			glUniformMatrix4fv(glGetUniformLocation(sceneEntities[i].shader_program, "mvp"), 1, GL_FALSE, glm::value_ptr(mvpMX));
			glBindVertexArray(sceneEntities[i].vbo_handler);
			glDrawArrays(sceneEntities[i].geometryType, 0, sceneEntities[i].numElements);
		}
	}
}

void openGLrender::uninitGraph()
{
	for(int i=0; i<entityCount; i++)
	{
		glDeleteBuffers(1, &(sceneEntities[i].vbo_handler));
		glDeleteVertexArrays(1, &(sceneEntities[i].vbo_handler));
	}
}

/*
*	Volume rendering methods
*/
bool openGLrender::initBoundingBox(glm::vec3 dim)
{
	vertex *vArray = new vertex[14];

	vArray[0] = vertex(1.0, dim.y/dim.x, dim.z/dim.x,1.0,1.0,1.0);
	vArray[1] = vertex(0.0, dim.y/dim.x, dim.z/dim.x,0.0,1.0,1.0);
	vArray[2] = vertex(1.0, 0.0, dim.z/dim.x,1.0,0.0,1.0);
	vArray[3] = vertex(0.0, 0.0, dim.z/dim.x,0.0,0.0,1.0);
	vArray[4] = vertex(0.0, 0.0, 0.0,0.0,0.0,0.0);
	vArray[5] = vertex(0.0, dim.y/dim.x, dim.z/dim.x,0.0,1.0,1.0);
	vArray[6] = vertex(0.0, dim.y/dim.x, 0.0,0.0,1.0,0.0);
	vArray[7] = vertex(1.0, dim.y/dim.x, dim.z/dim.x,1.0,1.0,1.0);
	vArray[8] = vertex(1.0, dim.y/dim.x, 0.0,1.0,1.0,0.0);
	vArray[9] = vertex(1.0, 0.0, dim.z/dim.x,1.0,0.0,1.0);
	vArray[10] = vertex(1.0, 0.0, 0.0,1.0,0.0,0.0);
	vArray[11] = vertex(0.0, 0.0, 0.0,0.0,0.0,0.0);
	vArray[12] = vertex(1.0, dim.y/dim.x, 0.0,1.0,1.0,0.0);
	vArray[13] = vertex(0.0, dim.y/dim.x, 0.0,0.0,1.0,0.0);

	glGenVertexArrays(1, &vbo_boundingBox);
	glBindVertexArray(vbo_boundingBox);
	glGenBuffers(1, &vbo_boundingBox);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_boundingBox);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*14, vArray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),(GLvoid*) (sizeof(float)+sizeof(float)+sizeof(float)));

	glBindVertexArray(0);

	return true;
}

bool openGLrender::init3DTex(glm::vec3 dim, char* filename)
{
	FILE *pFile;

	int size = 3* dim.x *dim.y *dim.z;

	pFile = fopen ("pferd.raw", "rb");
	if (pFile==NULL) {return false;}

	
	GLubyte *vol = NULL;
	vol = new GLubyte[size];
	if (vol == NULL) {return false;}

	GLubyte *buffer = NULL;
	int buffersize = (3 * dim.x *dim.y);
	buffer = new GLubyte[buffersize];
	fread(buffer,sizeof(GLubyte),buffersize,pFile);
	
	for(int i=0; i<size; i++)
	{
		vol[(size-1) - i] = buffer[i%buffersize];
	}
	
	glGenTextures(1, &tex_3D);
	glBindTexture(GL_TEXTURE_3D, tex_3D);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage3D(GL_TEXTURE_3D,0,GL_RGB,dim.x,dim.y,dim.z,0,GL_RGB,GL_UNSIGNED_BYTE,vol);
	delete [] vol;

	return true;
}

bool openGLrender::init3Dto2DTex(glm::vec3 dim)
{
	FILE *pFile;

	int size = 3* dim.x *dim.y *dim.z;

	pFile = fopen ("pferd.raw", "rb");
	if (pFile==NULL) {return false;}

	
	GLubyte *vol = NULL;
	vol = new GLubyte[size];
	if (vol == NULL) {return false;}

	GLubyte *buffer = NULL;
	int buffersize = (3 * dim.x *dim.y);
	buffer = new GLubyte[buffersize];
	fread(buffer,sizeof(GLubyte),buffersize,pFile);
	
	for(int i=0; i<size; i++)
	{
		vol[(size-1) -i] = buffer[i%buffersize];
	}
	
	glGenTextures(1, &tex_3D);
	glBindTexture(GL_TEXTURE_2D, tex_3D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,dim.x,dim.y*dim.z,0,GL_RGB,GL_UNSIGNED_BYTE,vol);
	delete [] vol;

	return true;
}

bool openGLrender::initVolumeVis()
{
	cam_alpha = 0.0;
	cam_beta = 0.0;
	cam_dist = 2.0;
	dimension = glm::vec3(256.0,256.0,32.0);
	initBoundingBox(dimension);
	init3Dto2DTex(dimension);
	const char* atrb0[] = { "in_position", "in_color"};
	const char* shader0[] = { "raycastFragment.glsl", "raycastVertex.glsl" };
	frontface_prgm = initShaderProgram(shader0[0],shader0[1],atrb0,2);
	glLinkProgram(frontface_prgm);
	const char* atrb1[] = { "in_position", "in_color"};
	const char* shader1[] = { "raycastFragment2.glsl", "raycastVertex2.glsl" };
	backface_prgm = initShaderProgram(shader1[0],shader1[1],atrb1,2);
	glLinkProgram(backface_prgm);

	return true;
}

void openGLrender::displayVolume()
{
	//cam_alpha = cam_alpha + 0.1;
	//calculate camera position
	//note that glm functions use radian values
	float cam_alpha_r = cam_alpha * (2.0*3.14/360.0);
	float cam_beta_r = cam_beta * (2.0*3.14/360.0);
	cameraPos.y = (glm::sin(cam_beta_r) * cam_dist) + (dimension.y/dimension.x)/2.0;
	float temp = (glm::cos(cam_beta_r) * cam_dist);
	cameraPos.z = glm::cos(cam_alpha_r) * temp + (dimension.z/dimension.x)/2.0;
	cameraPos.x = glm::sin(cam_alpha_r) * temp + 0.5;

	//set projection matrix
	float fnear = 0.001f;
	float fov = 60;
	float aspect = float(wWidth) / float(wHeight);
	projMX = glm::perspective(fov, aspect, fnear, 10000.0f);
	//set view matrix
	viewMX = glm::lookAt(cameraPos,glm::vec3(0.5,(dimension.y/dimension.x)/2.0,(dimension.z/dimension.x)/2.0), glm::vec3(0.0f, 1.0f, 0.0f));
	//set model matrix
	modelMX = glm::mat4(1.0f);
	//set model-view-projection matrix
	glm::mat4 mvpMX = projMX * viewMX * modelMX;
	//set transformation to texture coordinates of the bounding box
	texMX = glm::scale(glm::mat4(1.0), glm::vec3(1.0,1.0/(dimension.y/dimension.x),1.0/(dimension.z/dimension.x)));

	// check whether the camera is inside the bounding box
	glm::vec4 camPosTexCoord = texMX * glm::vec4(cameraPos,1.0);
	if(camPosTexCoord.x <= 1.0
		&& camPosTexCoord.y <= 1.0
		&& camPosTexCoord.z <= 1.0
		&& camPosTexCoord.x >= 0.0
		&& camPosTexCoord.y >= 0.0
		&& camPosTexCoord.z >= 0.0)
	{
		cam_BBtest = false;
	}
	else
	{
		cam_BBtest = true;
	}

	if(cam_BBtest)
	{
		// Render frontfaces of the Bounding Box 
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		glUseProgram(frontface_prgm);
		glUniformMatrix4fv(glGetUniformLocation(frontface_prgm, "mvp_matrix"), 1, GL_FALSE, glm::value_ptr(mvpMX));
		glUniformMatrix4fv(glGetUniformLocation(frontface_prgm, "tex_matrix"), 1, GL_FALSE, glm::value_ptr(texMX));
		glUniform3fv(glGetUniformLocation(frontface_prgm, "camera_pos"), 1, glm::value_ptr(cameraPos));
		glUniform3fv(glGetUniformLocation(frontface_prgm, "tex3D_dim"), 1, glm::value_ptr(dimension));
		
		glEnable(GL_TEXTURE_2D);
		glUniform1i(glGetUniformLocation(frontface_prgm, "Tex3D"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_3D);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glBindVertexArray(vbo_boundingBox);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
	}
	else
	{
		// Render backfaces of the Bounding Box
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		glUseProgram(backface_prgm);
		glUniformMatrix4fv(glGetUniformLocation(backface_prgm, "mvp_matrix"), 1, GL_FALSE, glm::value_ptr(mvpMX));
		glUniformMatrix4fv(glGetUniformLocation(backface_prgm, "tex_matrix"), 1, GL_FALSE, glm::value_ptr(texMX));
		glUniform3fv(glGetUniformLocation(backface_prgm, "camera_pos"), 1, glm::value_ptr(cameraPos));
		glUniform3fv(glGetUniformLocation(backface_prgm, "tex3D_dim"), 1, glm::value_ptr(dimension));
			
		glEnable(GL_TEXTURE_2D);
		glUniform1i(glGetUniformLocation(backface_prgm, "Tex3D"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_3D);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glBindVertexArray(vbo_boundingBox);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
	}
}

void openGLrender::uninitVolume()
{
	glDeleteBuffers(1, &vbo_boundingBox);
	glDeleteVertexArrays(1, &vbo_boundingBox);
	glDeleteTextures(1, &tex_3D);
}

/*
*	General purpose methods
*/
void openGLrender::setActivePointer(bool *active)
{
	this->run = active;
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

GLuint openGLrender::initShaderProgram(const char* fragment, const char* vertex, const char** attributes, int attributeCount)
{
	//nodes shader program
	GLuint vShader;
	GLuint fShader;
	GLuint program;

	if ((fShader = loadShader(fragment, GL_FRAGMENT_SHADER)) == 0)
	{ return false; };
	if ((vShader = loadShader(vertex, GL_VERTEX_SHADER)) == 0)
	{ return false; };

	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);

	for(int i=0; i< attributeCount; i++)
	{
		glBindAttribLocation(program, i, attributes[i]);
	}

	return program;
}

void openGLrender::mouse(int x, int y)
{
	if(render_mode == 0)
	{
		if(mouse_mode)
		{
			if(swap)
			{
				mouse_delta_x1 = x;
				mouse_delta_y1 = y;
			
				cameraPos = glm::vec3(cameraPos.x + ((float)(mouse_delta_x0-x)/(4000.0*camZoom)), cameraPos.y + ((float)(y-mouse_delta_y0)/(4000.0*camZoom)), cameraPos.z);

				swap = false;
				glutPostRedisplay();
			}
			else
			{
				mouse_delta_x0 = x;
				mouse_delta_y0 = y;
			
				cameraPos = glm::vec3(cameraPos.x + ((float)(mouse_delta_x1-x)/(4000.0*camZoom)), cameraPos.y + ((float)(y-mouse_delta_y1)/(4000.0*camZoom)), cameraPos.z);

				swap = true;
				glutPostRedisplay();
			}
		}
		else
		{
			if(swap)
			{
				mouse_delta_y1 = y;
			
				camZoom = camZoom - ((float)(y-mouse_delta_y0)/150.0)*camZoom;

				swap = false;
				glutPostRedisplay();
			}
			else
			{
				mouse_delta_y0 = y;
			
				camZoom = camZoom - ((float)(y-mouse_delta_y1)/150.0)*camZoom;

				swap = true;
				glutPostRedisplay();
			}
		}
	}

}

void openGLrender::mouseCallback(int x, int y)
{
	currentInstance->mouse(x, y);
}

void openGLrender::mouseClick(int button, int state, int x, int y)
{
	if(render_mode == 0)
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
}

void openGLrender::mouseClickCallback(int button, int state, int x, int y)
{
	currentInstance->mouseClick(button, state, x, y);
}

void openGLrender::keyboard (unsigned char key, int x, int y)
{
	if(render_mode == 0)
	{
		switch( key ) 
		{
			case 'q': case 'Q':
				glutLeaveMainLoop();
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
				sceneEntities[1].visabilty = ! sceneEntities[1].visabilty;
				glutPostRedisplay();
				break;
			case 'e':
				sceneEntities[0].visabilty = ! sceneEntities[0].visabilty;
				glutPostRedisplay();
				break;/*
			case 'm':
				showMap = !showMap;
				glutPostRedisplay();
				break;*/
			case 's':
				sceneEntities[2].visabilty = ! sceneEntities[2].visabilty;
				glutPostRedisplay();
				break;/*
			case 'c':
				showCluster = !showCluster;
				glutPostRedisplay();
				break;*/
		}
	}
	else
	{
		switch( key ) 
		{
			case 'q': case 'Q':
				glutLeaveMainLoop();
				break;
		}
	}
}

void openGLrender::keyboardArrows(int key, int x, int y)
{
	if(render_mode == 0)
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
	
	//render_mode == 0 for graph rendering
	if(render_mode == 0)
	{
		displayGraph();
	}
	//render_mode == 1 for volume rendering
	else
	{
		displayVolume();
	}

	glutSwapBuffers();
}

void openGLrender::displayCallback()
	{
		currentInstance -> display();
	}

void openGLrender::idle()
{
	if(*run)
	{
		glutPostRedisplay();
	}
	else
	{
		glutLeaveMainLoop();
	}
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

void openGLrender::timer()
{
	cam_alpha = cam_alpha + 0.5;
	glutPostRedisplay();
}

void openGLrender::timerCallback(int value)
{
	currentInstance->timer();

	glutTimerFunc(16,timerCallback,1);
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
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_CONTINUE_EXECUTION);

	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK)
	{
		return false;
	}

	if(argv[0] == "graph")
	{
		render_mode = 0;
	}
	else
	{
		render_mode = 1;
	}
	

	if(render_mode == 0)
	{
		initGraphVis();
	}
	else
	{
		initVolumeVis();
	}

	setInstance(this);
	glutDisplayFunc(displayCallback);
	glutIdleFunc(idleCallback);
	glutReshapeFunc(resizeCallback);
	glutMotionFunc(mouseCallback);
	glutMouseFunc(mouseClickCallback);
	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(keyboardArrowsCallback);
	glutTimerFunc(16,timerCallback,1);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutMainLoop();

	if(render_mode == 0)
	{
		uninitGraph();
	}
	else
	{
		uninitVolume();
	}
	
	return true;
}
