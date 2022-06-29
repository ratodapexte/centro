#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define MAX_VERTICES 60000
// gcc camrea.c -o camrea.exe -Ifreeglut/include -Lfreeglut/lib -lopengl32 -lfreeglut -lglu32
// Colors

#define BLACK 0.0f, 0.0f, 0.0f
#define RED 1.0f, 0.0f, 0.0f
#define GREEN 0.0f, 1.0f, 0.0f
#define BLUE 0.0f, 0.0f, 1.0f
#define PASTELBLUE 0.1882f, 0.3961f, 0.6745f
#define WHITE 1.0f, 1.0f, 1.0f
#define GREY 0.752941f, 0.752941f, 0.752941f


// Math

#define ORIGIN 0.0f, 0.0f, 0.0f
#define RIGHT 1.0f, 0.0f, 0.0f
#define UP 0.0f, 1.0f, 0.0f
#define FORWARD 0.0f, 0.0f, -1.0f
#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286


// Data types

struct Vec2 {
	float x, y;
};

typedef struct Vec2 Vec2;

struct Vec3 {
	float x, y, z;
};

typedef struct Vec3 Vec3;

struct Transform {
	Vec3 position;
	Vec3 rotation;
	Vec3 scale;
};

typedef struct Transform Transform;

// Constants

const float DEG2RAD = M_PI / 180.0f;
const float RAD2DEG = 180.0f / M_PI;


// Global variables

Vec2 WINDOW_SIZE = {1920, 1080};
Vec2 WINDOW_CENTER = {960, 540};
float FOVY = 75.0f;
float ZNEAR = 10e-3;
float ZFAR = 10e3;
Transform CAM;
int KEYBOARD[128] = {0};
Vec2 MOTION;

// Object 1
int BUILDING_VERTEX_COUNT;
Vec3 BUILDING_VERTICES[MAX_VERTICES];
Vec3 BUILDING_NORMALS[MAX_VERTICES];
Vec2 BUILDING_TEX_COORDS[MAX_VERTICES];
unsigned int building_tex_id;

// Object 2
int DOOR_VERTEX_COUNT;
Vec3 DOOR_VERTICES[MAX_VERTICES];
Vec3 DOOR_NORMALS[MAX_VERTICES];
Vec2 DOOR_TEX_COORDS[MAX_VERTICES];
float door_angle_increment = 0.05f, door_angle = 0, door_x_transl = 0, door_z_transl = 0;
int  enable_open_door = 0, enable_close_door = 0;


// Object 3
int WINDOW_VERTEX_COUNT;
Vec3 WINDOW_VERTICES[MAX_VERTICES];
Vec3 WINDOW_NORMALS[MAX_VERTICES];
Vec2 WINDOW_TEX_COORDS[MAX_VERTICES];
float window_angle_increment = 0.05f, window_angle = 0, window_x_transl = 0, window_z_transl = 0;
int  enable_open_window = 0, enable_close_window = 0;

// Object 4
int CHAIR_VERTEX_COUNT;
Vec3 CHAIR_VERTICES[MAX_VERTICES];
Vec3 CHAIR_NORMALS[MAX_VERTICES];
Vec2 CHAIR_TEX_COORDS[MAX_VERTICES];
unsigned int chair_tex_id;
float transl_chair[16][3] = {
    {-5.75f, 0.0f, -2.5f},
    {-5.75f, 0.0f, -6.0f},
    {-5.75f, 0.0f, -10.0f},
    {-5.75f, 0.0f, -14.0f},
    {-2.75f, 0.0f, -2.5f},
    {-2.75f, 0.0f, -6.0f},
    {-2.75f, 0.0f, -10.0f},
    {-2.75f, 0.0f, -14.0f},
    {2.25f, 0.0f, -2.5f},
    {2.25f, 0.0f, -6.0f},
    {2.25f, 0.0f, -10.0f},
    {2.25f, 0.0f, -14.0f},
    {5.25f, 0.0f, -2.5f},
    {5.25f, 0.0f, -6.0f},
    {5.25f, 0.0f, -10.0f},
    {5.25f, 0.0f, -14.0f}
};

// Object 5
int TABLE_VERTEX_COUNT;
Vec3 TABLE_VERTICES[MAX_VERTICES];
Vec3 TABLE_NORMALS[MAX_VERTICES];
Vec2 TABLE_TEX_COORDS[MAX_VERTICES];
unsigned int table_tex_id, table_teacher_tex_id;
float transl_table[16][3] = {
    {-5.25f, 0.0f, -1.0f},
    {-5.25f, 0.0f, -4.5f},
    {-5.25f, 0.0f, -8.5f},
    {-5.25f, 0.0f, -12.5f},
    {-2.25f, 0.0f, -1.0f},
    {-2.25f, 0.0f, -4.5f},
    {-2.25f, 0.0f, -8.5f},
    {-2.25f, 0.0f, -12.5f},
    {2.75f, 0.0f, -1.0f},
    {2.75f, 0.0f, -4.5f},
    {2.75f, 0.0f, -8.5f},
    {2.75f, 0.0f, -12.5f},
    {5.75f, 0.0f, -1.0f},
    {5.75f, 0.0f, -4.5f},
    {5.75f, 0.0f, -8.5f},
    {5.75f, 0.0f, -12.5f},
};

// Object 6
int MACHINE_VERTEX_COUNT;
Vec3 MACHINE_VERTICES[MAX_VERTICES];
Vec3 MACHINE_NORMALS[MAX_VERTICES];
Vec2 MACHINE_TEX_COORDS[MAX_VERTICES];
unsigned int machine_tex_id;
float transl_machine[16][3] = {
    {-5.0f, 1.5f, -1.0f},
    {-5.0f, 1.5f, -4.5f},
    {-5.0f, 1.5f, -8.5f},
    {-5.0f, 1.5f, -12.5f},
    {-2.0f, 1.5f, -1.0f},
    {-2.0f, 1.5f, -4.5f},
    {-2.0f, 1.5f, -8.5f},
    {-2.0f, 1.5f, -12.5f},
    {3.0f, 1.5f, -1.0f},
    {3.0f, 1.5f, -4.5f},
    {3.0f, 1.5f, -8.5f},
    {3.0f, 1.5f, -12.5f},
    {6.0f, 1.5f, -1.0f},
    {6.0f, 1.5f, -4.5f},
    {6.0f, 1.5f, -8.5f},
    {6.0f, 1.5f, -12.5f},
};

// Object 7
int CHAIR_TEACHER_VERTEX_COUNT;
Vec3 CHAIR_TEACHER_VERTICES[MAX_VERTICES];
Vec3 CHAIR_TEACHER_NORMALS[MAX_VERTICES];
Vec2 CHAIR_TEACHER_TEX_COORDS[MAX_VERTICES];
unsigned int chair_teacher_tex_id;

// Object 8
int BOARD_VERTEX_COUNT;
Vec3 BOARD_VERTICES[MAX_VERTICES];
Vec3 BOARD_NORMALS[MAX_VERTICES];
Vec2 BOARD_TEX_COORDS[MAX_VERTICES];
unsigned int board_tex_id;

// Object 10
int ROOF_FAN_VERTEX_COUNT;
Vec3 ROOF_FAN_VERTICES[MAX_VERTICES];
Vec3 ROOF_FAN_NORMALS[MAX_VERTICES];
Vec2 ROOF_FAN_TEX_COORDS[MAX_VERTICES];
unsigned int roof_fan_tex_id;
float roof_fan_angle_increment = 0.05f, roof_fan_angle = 0;

// Object 11
int FLOOR_VERTEX_COUNT;
Vec3 FLOOR_VERTICES[MAX_VERTICES];
Vec3 FLOOR_NORMALS[MAX_VERTICES];
Vec2 FLOOR_TEX_COORDS[MAX_VERTICES];
unsigned int floor_tex_id;

void setup_lighting();

void init_gl();

// Callbacks

void display();

void idle();

void motion(int x, int y);

void keyboard(unsigned char key, int x, int y);

void keyboard_up(unsigned char key, int x, int y);

void reshape(int width, int height);

int load_all_objects();

unsigned int load_texture(const char* path);

void load_all_textures();

void windowRotation();

void doorRotation();

void roofFanRotation(int rotation_control);

// Drawing utils

void draw_axis(int x, int y, int z);

void draw_grid(int n);

void draw_chair_rows(float transl[16][3]);
void draw_table_rows(float transl[16][3]);
void draw_machine_rows(float transl[16][3]);

// Math utils

Vec3 forward(Transform* t);

Vec3 right(Transform* t);

Vec3 up(Transform* t);

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(WINDOW_SIZE.x, WINDOW_SIZE.y);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Centro");
	glutWarpPointer(WINDOW_CENTER.x, WINDOW_CENTER.y);	

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutPassiveMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_up);

	init_gl();

	CAM.position = (Vec3) {0.0f, 2.0f, 14.0f};
	CAM.rotation = (Vec3) {0.0f, 0.0f, 0.0f};

	load_all_objects();
    load_all_textures();

	glutMainLoop();

	return 0;
}

void init_gl() {
	glEnable(GL_DEPTH_TEST);
    glutSetCursor(GLUT_CURSOR_NONE);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    setup_lighting();
}

void setup_lighting()
{


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

// Callbacks
// Função para o posicionamento e desenho dos objetos na tela
void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// View matrix
	Vec3 fwd = forward(&CAM);
	Vec3 u = up(&CAM);
	Vec3 eye = CAM.position;
	Vec3 center = {eye.x + fwd.x, eye.y + fwd.y, eye.z + fwd.z};

    setup_lighting();

	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, u.x, u.y, u.z); 

	int i;
    // Centro
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, building_tex_id);
    
    glBegin(GL_TRIANGLES);
    for(i = 0; i < BUILDING_VERTEX_COUNT; i++)
    {
        glNormal3f(BUILDING_NORMALS[i].x, BUILDING_NORMALS[i].y, BUILDING_NORMALS[i].z);
        glTexCoord2f(BUILDING_TEX_COORDS[i].x, BUILDING_TEX_COORDS[i].y);
        glVertex3f(BUILDING_VERTICES[i].x, BUILDING_VERTICES[i].y, BUILDING_VERTICES[i].z);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

	// Porta
	glPushMatrix();
	// glColor3f(GREY);
	glScalef(1.0f, 1.55f, 1.0f);
	glTranslatef(-7.0f, 0.0f, 1.875f);
	glTranslatef(door_x_transl, 0.0f, door_z_transl);
	glRotatef(door_angle,0,1,0);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < DOOR_VERTEX_COUNT; i++)
    {
        glNormal3f(DOOR_NORMALS[i].x, DOOR_NORMALS[i].y, DOOR_NORMALS[i].z);
        glTexCoord2f(DOOR_TEX_COORDS[i].x, DOOR_TEX_COORDS[i].y);
        glVertex3f(DOOR_VERTICES[i].x, DOOR_VERTICES[i].y, DOOR_VERTICES[i].z);
    }
    glEnd();
    glPopMatrix();

	//Janela
	glPushMatrix();
	// glColor3f(GREY);
	// glScalef(1.0f, 1.55f, 1.0f); 6,125
	glTranslatef(-7.0f, 3.5f, -1.8f);
	glTranslatef(window_x_transl, 0.0f, window_z_transl);
	glRotatef(window_angle,0,1,0);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < WINDOW_VERTEX_COUNT; i++)
    {
        glNormal3f(WINDOW_NORMALS[i].x, WINDOW_NORMALS[i].y, WINDOW_NORMALS[i].z);
        glTexCoord2f(WINDOW_TEX_COORDS[i].x, WINDOW_TEX_COORDS[i].y);
        glVertex3f(WINDOW_VERTICES[i].x, WINDOW_VERTICES[i].y, WINDOW_VERTICES[i].z);
    }
    glEnd();
    glPopMatrix();

	//Cadeira recepção
	glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, chair_teacher_tex_id);
	glTranslatef(-2.8f, 0.0f, 10.0f);
	glRotatef(180,0,1,0);
	glScalef(2.0f, 2.0f, 2.0f);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < CHAIR_TEACHER_VERTEX_COUNT; i++)
    {
        glNormal3f(CHAIR_TEACHER_NORMALS[i].x, CHAIR_TEACHER_NORMALS[i].y, CHAIR_TEACHER_NORMALS[i].z);
        glTexCoord2f(CHAIR_TEACHER_TEX_COORDS[i].x, CHAIR_TEACHER_TEX_COORDS[i].y);
        glVertex3f(CHAIR_TEACHER_VERTICES[i].x, CHAIR_TEACHER_VERTICES[i].y, CHAIR_TEACHER_VERTICES[i].z);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

	//Mesa recepção
	glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, table_teacher_tex_id);
	glTranslatef(-3.3f, 0.0f, 9.0f);
	glScalef(-3.5f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < TABLE_VERTEX_COUNT; i++)
    {
        glNormal3f(TABLE_NORMALS[i].x, TABLE_NORMALS[i].y, TABLE_NORMALS[i].z);
        glTexCoord2f(TABLE_TEX_COORDS[i].x, TABLE_TEX_COORDS[i].y);
        glVertex3f(TABLE_VERTICES[i].x, TABLE_VERTICES[i].y, TABLE_VERTICES[i].z);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

    draw_chair_rows(transl_chair);
    draw_table_rows(transl_table);
    draw_machine_rows(transl_machine);

	//Board
	glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, board_tex_id);
	glTranslatef(0.0f, 3.0f, 14.9f);
	glRotatef(180,0,1,0);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < BOARD_VERTEX_COUNT; i++)
    {
        glNormal3f(BOARD_NORMALS[i].x, BOARD_NORMALS[i].y, BOARD_NORMALS[i].z);
        glTexCoord2f(BOARD_TEX_COORDS[i].x, BOARD_TEX_COORDS[i].y);
        glVertex3f(BOARD_VERTICES[i].x, BOARD_VERTICES[i].y, BOARD_VERTICES[i].z);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

    //Board
	glPushMatrix();
	glTranslatef(0.0f, 12.0f, 0.0f);
	// glRotatef(180,0,1,0);
	glRotatef(roof_fan_angle,0,1,0);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < ROOF_FAN_VERTEX_COUNT; i++)
    {
        glNormal3f(ROOF_FAN_NORMALS[i].x, ROOF_FAN_NORMALS[i].y, ROOF_FAN_NORMALS[i].z);
        glTexCoord2f(ROOF_FAN_TEX_COORDS[i].x, ROOF_FAN_TEX_COORDS[i].y);
        glVertex3f(ROOF_FAN_VERTICES[i].x, ROOF_FAN_VERTICES[i].y, ROOF_FAN_VERTICES[i].z);
    }
    glEnd();
    glPopMatrix();

    // Floor
	glPushMatrix();
	glTranslatef(0.0f, -0.2f, 0.0f);
    glBindTexture(GL_TEXTURE_2D, floor_tex_id);
    glBegin(GL_TRIANGLES);
    for(i = 0; i < FLOOR_VERTEX_COUNT; i++)
    {
        glNormal3f(FLOOR_NORMALS[i].x, FLOOR_NORMALS[i].y, FLOOR_NORMALS[i].z);
        glTexCoord2f(FLOOR_TEX_COORDS[i].x, FLOOR_TEX_COORDS[i].y);
        glVertex3f(FLOOR_VERTICES[i].x, FLOOR_VERTICES[i].y, FLOOR_VERTICES[i].z);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

	glutSwapBuffers();
}

void idle() {
	// Forward movement
	int move_forward = KEYBOARD['w'] - KEYBOARD['s'];
	Vec3 fwd = forward(&CAM);
	
	fwd.x *= move_forward;
	fwd.y = 0.0f; // Projects fwd in the xz plane
	fwd.z *= move_forward;

	// Lateral movement
	int move_right = KEYBOARD['d'] - KEYBOARD['a'];
	Vec3 rgt = right(&CAM);

	rgt.x *= move_right;
	rgt.z *= move_right;
	
	CAM.position.x += 0.1f * (fwd.x + rgt.x);
	CAM.position.z += 0.1f * (fwd.z + rgt.z);

	if(enable_open_window && window_angle <= 90.0f)
		windowRotation(1);
	if(enable_close_window && window_angle >= 0.0f)
		windowRotation(-1);

	if(enable_open_door && door_angle <= 90.0f)
		doorRotation(1);
	if(enable_close_door && door_angle >= 0.0f)
		doorRotation(-1);

    roofFanRotation(-1);

	glutPostRedisplay();
}

void motion(int x, int y) { 
	static int wrap = 0;
	Vec2 delta;
	
    if (!wrap) {
        delta.x = x - WINDOW_CENTER.x;
        delta.y = y - WINDOW_CENTER.y;

		CAM.rotation.x += delta.y > 0 ? 0.7f : (delta.y < 0 ? -0.7f : 0.0f);
		CAM.rotation.y -= delta.x > 0 ? 0.7f : (delta.x < 0 ? -0.7f : 0.0f);

        wrap = 1;
        glutWarpPointer(WINDOW_CENTER.x, WINDOW_CENTER.y);
    } else {
        wrap = 0;
    }
}

// Função para reconhecer os eventos de teclado
void keyboard(unsigned char key, int x, int y){
	if(key == 27)
		glutLeaveMainLoop();

	if(key == 113)
	{
		enable_open_window = !enable_open_window;
		enable_close_window = 0;
	}

	if(key == 101){
		enable_close_window = !enable_close_window;
		enable_open_window = 0;
	}

	if(key == 114)
	{
		enable_open_door = !enable_open_door;
		enable_close_door = 0;
	}

	if(key == 102){
		enable_close_door = !enable_close_door;
		enable_open_door = 0;
	}

	KEYBOARD[tolower(key)] = 1;
}

void keyboard_up(unsigned char key, int x, int y){
	KEYBOARD[tolower(key)] = 0;
}

void reshape(int width, int height) {
	float aspect = (float) width / (float) height;
	WINDOW_SIZE.x = width;
	WINDOW_SIZE.y = height;
	WINDOW_CENTER.x = width / 2;
	WINDOW_CENTER.y = height / 2;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOVY, aspect, ZNEAR, ZFAR);
}


// Drawing utils
void draw_axis(int x, int y, int z) {
	glLineWidth(3.0f);
	glBegin(GL_LINES);
	if(x) {
		glColor3f(RED);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(ZFAR, 0.0f, 0.0f);
	}
	if(y) {
		glColor3f(GREEN);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, ZFAR, 0.0f);
	}
	if(z) {
		glColor3f(BLUE);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, ZFAR);
	}
	glEnd();
	glLineWidth(1.0f);
}

void draw_grid(int n) {
	int i;

	glBegin(GL_LINES);
	glColor3f(WHITE);
	for(i = -n;i < n;i++){
		float d = (float) i;
		// Parallel to x-axis 
		glVertex3f(-n, 0.0f, d);
		glVertex3f(n, 0.0f, d);
		// Parallel to z-axis
		glVertex3f(d, 0.0f, -n);
		glVertex3f(d, 0.0f, n);
	}
	glEnd();
}


// Math utils

Vec3 forward(Transform* t) {
	Vec3 v;
	float a = t->rotation.x * DEG2RAD;
	float b = t->rotation.y * DEG2RAD;
	float c = t->rotation.z * DEG2RAD;

	v.x = -(sin(c) * sin(a) + cos(c) * sin(b) * cos(a));
	v.y = -(-cos(c) * sin(a) + sin(c) * sin(b) * cos(a));
	v.z = -(cos(b) * cos(a));

	return v;
}

Vec3 up(Transform* t) {
	Vec3 v;
	float a = t->rotation.x * DEG2RAD;
	float b = t->rotation.y * DEG2RAD;
	float c = t->rotation.z * DEG2RAD;

	v.x = -sin(c) * cos(a) + cos(c) * sin(b) * sin(a);
	v.y = cos(c) * cos(a) + sin(c) * sin(b) * sin(a);
	v.z = cos(b) * sin(a);
	
	return v;
}

Vec3 right(Transform* t) {
	Vec3 v;
	float a = t->rotation.x * DEG2RAD;
	float b = t->rotation.y * DEG2RAD;
	float c = t->rotation.z * DEG2RAD;

	v.x = cos(c) * cos(b);
	v.y = sin(c) * cos(b);
	v.z = -sin(b);

	return v;
}

//Função para a rotação da janela
void windowRotation(int rotation_control){
	window_angle += window_angle_increment * rotation_control;
	window_x_transl = sin(window_angle*PI/180) * 1.4f;
	window_z_transl = cos(window_angle*PI/180) * 1.4f - 1.4f; 
}
//Função para a rotação da porta
void doorRotation(int rotation_control){
	door_angle += door_angle_increment * rotation_control;
	door_x_transl = sin(door_angle*PI/180) * 1.875f;
	door_z_transl = cos(door_angle*PI/180) * 1.875f - 1.875f;
}
//Função para a rotação do ventilador de teto
void roofFanRotation(int rotation_control){
	roof_fan_angle += roof_fan_angle_increment * rotation_control;
}

//Carrega as texturas
unsigned int load_texture(const char* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if(data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        perror("Texture failed to load");
        stbi_image_free(data);
    }
    return textureID;
}

void load_all_textures()
{
    table_tex_id = load_texture("textures/table.jpg");
    table_teacher_tex_id = load_texture("textures/table_teacher.jpg");
    chair_tex_id = load_texture("textures/chair.jpg");
    chair_teacher_tex_id = load_texture("textures/chair_teacher.png");
    machine_tex_id = load_texture("textures/machine.png");
    board_tex_id = load_texture("textures/white_board.jpg");
    floor_tex_id = load_texture("textures/grey-concrete-texture.jpg");
    building_tex_id = load_texture("textures/building.png");
}

// Função para o carregamento dos objetos
int load_obj(const char* path, int object)
{
    FILE *fp = fopen(path, "r");

    if(!fp) return 0;

    char buffer[512] = "";
    int vertex_count = 0;
    int normal_count = 0;
    int tex_coord_count = 0;

    Vec3 vertices[MAX_VERTICES];
    Vec3 normals[MAX_VERTICES];
    Vec2 tex_coords[MAX_VERTICES];

    while(fgets(buffer, 512, fp))
    {
        // Comment
        if(buffer[0] == '#') continue;

        char* token = strtok(buffer, " ");

        if(strcmp(token, "v") == 0)
        {
            // Parse vertex
            vertices[vertex_count].x = atof(strtok(NULL, " "));
            vertices[vertex_count].y = atof(strtok(NULL, " "));
            vertices[vertex_count].z = atof(strtok(NULL, " "));
            vertex_count++;
        }
        else if(strcmp(token, "vn") == 0)
        {
            // Parse normal vector
            normals[normal_count].x = atof(strtok(NULL, " "));
            normals[normal_count].y = atof(strtok(NULL, " "));
            normals[normal_count].z = atof(strtok(NULL, " "));
            normal_count++;
        }
        else if(strcmp(token, "vt") == 0)
        {
            // Parse texture coordinate
            tex_coords[tex_coord_count].x = atof(strtok(NULL, " "));
            tex_coords[tex_coord_count].y = -atof(strtok(NULL, " "));
            tex_coord_count++;
        }
        else if(strcmp(token, "f") == 0)
        {
            // Parse face
            int i;
            for(i = 0; i < 3; i++)
            {
                if(object == 1) // Main door
                {
                    BUILDING_VERTICES[BUILDING_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    BUILDING_TEX_COORDS[BUILDING_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    BUILDING_NORMALS[BUILDING_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    BUILDING_VERTEX_COUNT++;
                }
				if(object == 2) // Main door
                {
                    DOOR_VERTICES[DOOR_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    DOOR_TEX_COORDS[DOOR_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    DOOR_NORMALS[DOOR_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    DOOR_VERTEX_COUNT++;
                }
				if(object == 3) // Main door
                {
                    WINDOW_VERTICES[WINDOW_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    WINDOW_TEX_COORDS[WINDOW_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    WINDOW_NORMALS[WINDOW_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    WINDOW_VERTEX_COUNT++;
                }
				if(object == 4) // Chair
                {
                    CHAIR_VERTICES[CHAIR_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    CHAIR_TEX_COORDS[CHAIR_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    CHAIR_NORMALS[CHAIR_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    CHAIR_VERTEX_COUNT++;
                }
				if(object == 5) // Table
                {
                    TABLE_VERTICES[TABLE_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    TABLE_TEX_COORDS[TABLE_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    TABLE_NORMALS[TABLE_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    TABLE_VERTEX_COUNT++;
                }
				if(object == 6) // Sewing machine
                {
                    MACHINE_VERTICES[MACHINE_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    MACHINE_TEX_COORDS[MACHINE_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    MACHINE_NORMALS[MACHINE_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    MACHINE_VERTEX_COUNT++;
                }
                if(object == 7) // Chair teacher
                {
                    CHAIR_TEACHER_VERTICES[CHAIR_TEACHER_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    CHAIR_TEACHER_TEX_COORDS[CHAIR_TEACHER_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    CHAIR_TEACHER_NORMALS[CHAIR_TEACHER_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    CHAIR_TEACHER_VERTEX_COUNT++;
                }
                if(object == 8) // Board
                {
                    BOARD_VERTICES[BOARD_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    BOARD_TEX_COORDS[BOARD_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    BOARD_NORMALS[BOARD_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    BOARD_VERTEX_COUNT++;
                }
                if(object == 10) // Roof fan
                {
                    ROOF_FAN_VERTICES[ROOF_FAN_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    ROOF_FAN_TEX_COORDS[ROOF_FAN_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    ROOF_FAN_NORMALS[ROOF_FAN_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    ROOF_FAN_VERTEX_COUNT++;
                }
                if(object == 11) // Floor
                {
                    FLOOR_VERTICES[FLOOR_VERTEX_COUNT] = vertices[atoi(strtok(NULL, "/")) - 1];
                    FLOOR_TEX_COORDS[FLOOR_VERTEX_COUNT] = tex_coords[atoi(strtok(NULL, "/")) - 1];
                    FLOOR_NORMALS[FLOOR_VERTEX_COUNT] = normals[atoi(strtok(NULL, " ")) - 1];
                    FLOOR_VERTEX_COUNT++;
                }
            }
        }
    }

    fclose(fp);

    return 1;
}

// Função para o controle do carregamento dos objetos
int load_all_objects()
{
    if(!load_obj("obj/centro.obj", 1))
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }
	if(!load_obj("obj/porta.obj", 2))
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }
	if(!load_obj("obj/janela.obj", 3))
    {
        perror("Erro ao abrir o arquivo");
        return -1;
    }
	if(!load_obj("obj/cadeira.obj", 4))
    {
        perror("Erro ao abrir o arquivo da cadeira");
        return -1;
    }
	if(!load_obj("obj/mesa.obj", 5))
    {
        perror("Erro ao abrir o arquivo da mesa");
        return -1;
    }
	if(!load_obj("obj/maquina.obj", 6))
    {
        perror("Erro ao abrir o arquivo da mesa");
        return -1;
    }
    if(!load_obj("obj/chair_teacher.obj", 7))
    {
        perror("Erro ao abrir o arquivo da mesa");
        return -1;
    }
    if(!load_obj("obj/board.obj", 8))
    {
        perror("Erro ao abrir o arquivo da mesa");
        return -1;
    }
    if(!load_obj("obj/roof_fan.obj", 10))
    {
        perror("Erro ao abrir o arquivo da mesa");
        return -1;
    }
    if(!load_obj("obj/floor.obj", 11))
    {
        perror("Erro ao abrir o arquivo da mesa");
        return -1;
    }
}

void draw_chair_rows(float transl[16][3]){
	for(int j = 0; j < 16; j++){
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, chair_tex_id);
        glTranslatef(transl[j][0], transl[j][1], transl[j][2]);
        glRotatef(90,0,1,0);
        glBegin(GL_TRIANGLES);
        for(int i = 0; i < CHAIR_VERTEX_COUNT; i++)
        {
            glNormal3f(CHAIR_NORMALS[i].x, CHAIR_NORMALS[i].y, CHAIR_NORMALS[i].z);
            glTexCoord2f(CHAIR_TEX_COORDS[i].x, CHAIR_TEX_COORDS[i].y);
            glVertex3f(CHAIR_VERTICES[i].x, CHAIR_VERTICES[i].y, CHAIR_VERTICES[i].z);
        }
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
void draw_table_rows(float transl[16][3]){
	for(int j = 0; j < 16; j++){
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, table_tex_id);
        glTranslatef(transl[j][0], transl[j][1], transl[j][2]);
        glBegin(GL_TRIANGLES);
        for(int i = 0; i < TABLE_VERTEX_COUNT; i++)
        {
            glNormal3f(TABLE_NORMALS[i].x, TABLE_NORMALS[i].y, TABLE_NORMALS[i].z);
            glTexCoord2f(TABLE_TEX_COORDS[i].x, TABLE_TEX_COORDS[i].y);
            glVertex3f(TABLE_VERTICES[i].x, TABLE_VERTICES[i].y, TABLE_VERTICES[i].z);
        }
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
void draw_machine_rows(float transl[16][3]){
	for(int j = 0; j < 16; j++){
        glPushMatrix();
        // glColor3f(GREY);
        glBindTexture(GL_TEXTURE_2D, machine_tex_id);
        glTranslatef(transl[j][0], transl[j][1], transl[j][2]);
        glRotatef(-90,0,1,0);
        glBegin(GL_TRIANGLES);
        for(int i = 0; i < MACHINE_VERTEX_COUNT; i++)
        {
            glNormal3f(MACHINE_NORMALS[i].x, MACHINE_NORMALS[i].y, MACHINE_NORMALS[i].z);
            glTexCoord2f(MACHINE_TEX_COORDS[i].x, MACHINE_TEX_COORDS[i].y);
            glVertex3f(MACHINE_VERTICES[i].x, MACHINE_VERTICES[i].y, MACHINE_VERTICES[i].z);
        }
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}