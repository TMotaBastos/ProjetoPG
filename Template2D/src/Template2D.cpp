/*
-----------------------------------------------------------------------------
OpenGL Tutorial
VOXAR Labs
Computer Science Center - CIn
Federal University of Pernambuco - UFPE
http://www.cin.ufpe.br/~voxarlabs

@author Tullio Lucena - tjsl

Este programa foi feito para mostrar conceitos basicos de OpenGl e glut para
a turma de Processamento Grafico do Centro de Informatica da UFPE.

Funcionalidades:
1- Criar um quadrado com tamanho e cor aleatoria ao clicar com o botao
direito do mouse. A posicao e definida pela posicao atual do mouse.
2- Movimentar o quadrado segurando o botao esquerdo do mouse e arrastando.
3- Limpar a tela apertando a tecla F5.
4- Sair do programa apertando a tecla ESC.

Referencias:
Funcoes de C/C++:
http://www.cplusplus.com/
Copia online do Red Book (OpenGL Programming Guide):
http://fly.cc.fer.hr/~unreal/theredbook/
Referencia online para os comandos OpenGL (Man pages):
http://www.opengl.org/sdk/docs/man/
-----------------------------------------------------------------------------
*/

#include "Template2D.h"

//int qtdQuadrados;
int estado;
GLfloat mouse_x, mouse_y;
//Quadrado quad[1000];
Quadrado quad;
//std::vector<Ponto> ponto;
Ponto ponto[2];
int qtdPontos;
GLfloat window_width = /*350.0*/600.0;
GLfloat window_height = /*350.0*/600.0;

GLuint loadTexture(const char* imagepath) {
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;
	unsigned char* data;

	FILE* file = fopen(imagepath, "rb");
	if (!file) {
		printf("Nao existe essa imagem!\n");
		return 0;
	}
	if (fread(header, 1, 54, file) != 54) {
		printf("Nao eh uma imagem valida!\n");
		return 0;
	}
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Nao eh uma imagem valida!\n");
		return 0;
	}
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if (imageSize == 0) imageSize = width * height * 3;
	if (dataPos == 0) dataPos = 54;

	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);

	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return textureID;
}

GLuint loadTexture2(const char * filename)
{

	GLuint texture;

	int width, height;

	unsigned char * data;

	FILE * file;

	file = fopen(filename, "rb");

	if (file == NULL) return 0;
	width = 1024;
	height = 512;
	data = (unsigned char *)malloc(width * height * 3);
	//int size = fseek(file,);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	for (int i = 0; i < width * height; ++i)
	{
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];

		data[index] = R;
		data[index + 2] = B;

	}


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);


	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	return texture;
}

void myinit() {
	srand(time(NULL));
	//qtdQuadrados = 0;	
	qtdPontos = 0;
	quad = Quadrado(/*(((GLfloat)(rand()%50))/100.0)+0.1*/(GLfloat)0.5, /*0*/-0.25, /*0*/-0.25,
		((GLfloat)(rand() % 256)) / 255.0, ((GLfloat)(rand() % 256)) / 255.0, ((GLfloat)(rand() % 256)) / 255.0);
	//ponto.clear();	
	estado = MODIFIED;
	loop(0);
}

void myreshape(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	window_width = (GLfloat)w;
	window_height = (GLfloat)h;
	glOrtho(0, window_width, 0, window_height, -1.0, -1.0);
}

void mydisplay() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//for(int i = 0; i < qtdQuadrados; i++){	
	// glColor3f(quad[i].r, quad[i].g, quad[i].b);	
	// glBegin(GL_QUADS);	
	//	 glVertex2f(quad[i].x, quad[i].y);	
	//	 glVertex2f(quad[i].x+/*(GLfloat)1/175*/quad[i].lado, quad[i].y);	
	// 	 glVertex2f(quad[i].x +/*(GLfloat)1/175*/quad[i].lado, quad[i].y -/*(GLfloat)1/175)*/quad[i].lado);	
	// 	 glVertex2f(quad[i].x, quad[i].y-/*(GLfloat)1/175)*/quad[i].lado);	
	// glEnd();	
	//}	
	//colocando na tela o quadrado	
	//glColor3f(quad.r, quad.g, quad.b);
	glColor3f(1.0f, 1.0f, 1.0f);
	//colocando a textura
	GLuint texture = loadTexture("C:/Users/TMB/Documents/ProjetoPG/ProjetoPG/Template2D/images/phmb.bmp");
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	/*glVertex2f(quad.x, quad.y);
	glVertex2f(quad.x + quad.lado, quad.y);
	glVertex2f(quad.x + quad.lado, quad.y - quad.lado);
	glVertex2f(quad.x, quad.y - quad.lado);*/
	/*
	//bottom left
	glTexCoord2f(0, 1);
	glVertex2f(quad.x, quad.y);
	//bottom right
	glTexCoord2f(1, 1);
	glVertex2f(quad.x + quad.lado, quad.y);
	//top right
	glTexCoord2f(1, 0);
	glVertex2f(quad.x + quad.lado, quad.y + quad.lado);
	//top left
	glTexCoord2f(0, 0);
	glVertex2f(quad.x, quad.y + quad.lado);
	*/
	//top left
	glTexCoord2f(0, 1);
	glVertex2f(quad.x, quad.y + quad.lado);
	//bottom left
	glTexCoord2f(0, 0);
	glVertex2f(quad.x, quad.y);
	//bottom right
	glTexCoord2f(1, 0);
	glVertex2f(quad.x + quad.lado, quad.y);
	//top right
	glTexCoord2f(1, 1);
	glVertex2f(quad.x + quad.lado, quad.y + quad.lado);
	glEnd();
	//outro teste	
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0f, 0.0f, 0.0f);
	//printf("qtdPontos = %d\n", qtdPontos);	
	if (qtdPontos > 1) {
		for (auto p : ponto) {
			glVertex2f(p.x, p.y);
		}
	}
	else {
		//auto p = ponto.front();	
		glVertex2f(ponto[0].x, ponto[0].y);
	}
	glEnd();
	//colocando na tela os pontos	
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 0.0f, 0.0f);
	/*for (auto p : ponto)
	glVertex2f(p.x, p.y);*/
	for (int i = 0; i < qtdPontos; i++) {
		glVertex2f(ponto[i].x, ponto[i].y);
	}
	glEnd();
	glFlush();
}

void handleMotion(int x, int y) {
	if ((estado != MODIFIED) && (estado != IDLE)) {
		quad/*[estado]*/.x = (((((GLfloat)x) / window_width)*2.0) - 1.0) + mouse_x;
		quad/*[estado]*/.y = (-(((((GLfloat)y) / window_height)*2.0) - 1.0)) + mouse_y;
	}
}

//x e y sao as coordenas onde cliquei na tela
//x2 e y2 eh onde cliquei so que ele converte pra as coordenadas de tela do OpenGL
void handleMouse(int btn, int state, int x, int y) {
	if (estado == IDLE && btn == GLUT_LEFT_BUTTON) {
		mouse_x = ((((GLfloat)x) / window_width)*2.0) - 1.0;
		mouse_y = -(((((GLfloat)y) / window_height)*2.0) - 1.0);
		if (state == GLUT_DOWN) {
			/*for(int i = qtdQuadrados-1; i >= 0; i--){
			if((mouse_x >= quad[i].x) && (mouse_x <= (quad[i].x+quad[i].lado)) && (mouse_y <= quad[i].y) && (mouse_y >= (quad[i].y-quad[i].lado))){
			mouse_x = quad[i].x - mouse_x;
			mouse_y = quad[i].y - mouse_y;
			estado = i;
			break;
			}
			}*/
			quad.x = mouse_x;
			quad.y = mouse_y;
			estado = MODIFIED;
		}
	}
	else if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		estado = MODIFIED;
	}
	else if (estado == IDLE && btn == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN && qtdPontos < 2) {
			GLfloat x2 = ((((GLfloat)x) / window_width)*2.0) - 1.0;
			GLfloat y2 = -(((((GLfloat)y) / window_height)*2.0) - 1.0);
			//ponto.push_back(Ponto(x2, y2));	
			ponto[qtdPontos++] = Ponto(x2, y2);
			estado = MODIFIED;
		}
	} //else if(estado == IDLE && btn == GLUT_RIGHT_BUTTON){
	  // if (state == GLUT_DOWN){	
	  //	 GLfloat x2 = ((((GLfloat)x)/window_width)*2.0)-1.0;	
	  //	 GLfloat y2 = -(((((GLfloat)y)/window_height)*2.0)-1.0);	
	  // 	
	  //	 quad[qtdQuadrados++] = Quadrado(/*(((GLfloat)(rand()%50))/100.0)+0.1*/(GLfloat)0.5, x2, y2,
	  //	 ((GLfloat)(rand()%256))/255.0, ((GLfloat)(rand()%256))/255.0, ((GLfloat)(rand()%256))/255.0);	
	  //	 estado = MODIFIED;	
	  // }	
	  //}
}

void hadleKeyboard(unsigned char key, int x, int y) {
	if (key == ESC) {
		exit(0);
	}
}

void hadleSpecialKeyboard(int key, int x, int y) {
	if (key == GLUT_KEY_F5) {
		myinit();
	}
}

void loop(int id) {
	if (estado == MODIFIED) {
		mydisplay();
		estado = IDLE;
	}
	else if (estado != IDLE) {
		mydisplay();
	}
	glutTimerFunc(1000 / FPS, loop, id);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Projeto1");
	glutDisplayFunc(mydisplay);
	glutReshapeFunc(myreshape);
	glutMouseFunc(handleMouse);
	glutMotionFunc(handleMotion);
	glutKeyboardUpFunc(hadleKeyboard);
	glutSpecialUpFunc(hadleSpecialKeyboard);
	myinit();
	glutMainLoop();
	return 0;
}