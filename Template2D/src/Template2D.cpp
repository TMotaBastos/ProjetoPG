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
#include <utility>
#define ESPACO 10

//int qtdQuadrados;
int estado;
GLfloat mouse_x, mouse_y;
//Quadrado quad[1000];
Quadrado quad;
QuadradoAvancado quad2;
//std::vector<Ponto> ponto;
Ponto ponto[2];
int qtdPontos;
int cont;
GLfloat movimentoX;
GLfloat movimentoY;
bool emMovimento;
int qtdPassos;
GLfloat window_width = /*350.0*/600.0;
GLfloat window_height = /*350.0*/600.0;
int t_matrix[3][3];
int matrix[3][1]; //só para não ficar dando erro, tirar depois
int matrix_c[3][1];
char comando;
//pair<GLfloat, GLfloat> ponto_t;


//lembrar de colocar a matrix[][] (matriz do ponto) como paramêtro na função
//trocar o retorno também, os corretos estão comentados, assim como os próprios retornos
void clear() {
	t_matrix[0][0] = 0;//a
	t_matrix[0][1] = 0;//b
	t_matrix[0][2] = 0;//tx
	t_matrix[1][0] = 0;//c
	t_matrix[1][1] = 0;//d
	t_matrix[1][2] = 0;//ty
	t_matrix[2][0] = 0;
	t_matrix[2][1] = 0;
	t_matrix[2][2] = 1;
}

pair<GLfloat, GLfloat>/*void*//*int***/ scale(int sx, int sy/*, int matrix[3][1]*//*, pair<GLfloat, GLfloat> ponto_t*/, GLfloat xrec, GLfloat yrec) {
	t_matrix[0][0] = sx;
	t_matrix[1][1] = sy;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			printf("%d ", t_matrix[i][j]);
		}
		printf("\n");
	}

	printf("x = %lf\n", xrec);
	printf("y = %lf\n", yrec);
	printf("(%lf,%lf)\n", xrec, yrec);

	//int matrix_c[3][1];
	GLfloat x, y;
	for (int i = 0; i < 3; i++) {
		GLfloat sum = 0;
		for (int j = 0; j < 3; j++) {
			if (j == 0) {
				sum += (t_matrix[i][j] * /*matrix[j][0]*/xrec);
			}else if(j == 1){
				sum += (t_matrix[i][j] * /*matrix[j][0]*/yrec);
			}
			if (j == 0) {
				printf("i(%d) , j(%d)  %lf\n", i, j, xrec);
			}else if(j == 1){
				printf("i(%d) , j(%d)  %lf\n", i, j, yrec);
			}
			//matrix_c[i][0] = sum;
		}
		if (i == 0) x = sum;
		else if (i == 1) y = sum;
	}
	//for (int i = 0; i < 3; i++) matrix[i][0] = matrix_c[i][0];
	//return matrix_c;
	return pair<GLfloat, GLfloat>(x, y);
}

void/*int***/ refletion() {


	int matrix_c[3][3];
	for (int i = 0; i < 3; i++) {
		int sum = 0;
		for (int k = 0; k < 3; k++) {
			sum += (matrix[i][k] * t_matrix[k][0]);
			matrix_c[i][0] = sum;
		}
	}
	clear();
	//return matrix_c;
}

void/*int***/ shear(int kx, int ky) {
	t_matrix[0][0] = 1;
	t_matrix[0][1] = kx;
	t_matrix[1][0] = ky;
	t_matrix[1][1] = 1;

	int matrix_c[3][3];
	for (int i = 0; i < 3; i++) {
		int sum = 0;
		for (int k = 0; k < 3; k++) {
			sum += (matrix[i][k] * t_matrix[k][0]);
			matrix_c[i][1] = sum;
		}
	}
	clear();
	//return matrix_c;
}

void/*int***/ rotation() {

	// rotação antihorária
	//(cos -sen 0)
	//(sen  cos 0)
	//( 0    0  1)

	// rotação horária
	//( cos sen 0)
	//(-sen cos 0)
	//(  0   0  1)
	int matrix_c[3][3];
	for (int i = 0; i < 3; i++) {
		int sum = 0;
		for (int k = 0; k < 3; k++) {
			sum += (matrix[i][k] * t_matrix[k][0]);
			matrix_c[i][0] = sum;
		}
	}
	clear();
	//return matrix_c;
}

void/*int***/ translation(int x, int y) {
	t_matrix[0][0] = 1;
	t_matrix[1][1] = 1;
	t_matrix[0][2] -= x;
	t_matrix[1][2] -= y;

	int matrix_c[3][3];
	for (int i = 0; i < 3; i++) {
		int sum = 0;
		for (int k = 0; k < 3; k++) {
			sum += (matrix[i][k] * t_matrix[k][0]);
			matrix_c[i][0] = sum;
		}
	}
	clear();
	//return matrix_c;
}

GLuint loadTexture(const char* imagepath) {
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;
	unsigned char* data;

	FILE *file = fopen(imagepath, "rb");
	/*FILE *file;
	errno_t err;
	if ((err = fopen_s(&file, imagepath, "rb")) != 0) {
		printf("Nao pode abrir o arquivo");
	}*/

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
	cont = 0;
	movimentoX = 0.0;
	movimentoY = 0.0;
	emMovimento = false;
	comando = 'n';
	qtdPassos = 0;
	qtdPontos = 0;
	quad2 = QuadradoAvancado((GLfloat)-0.25, (GLfloat)-0.25, (GLfloat)0.25, (GLfloat)-0.25,
		(GLfloat)0.25, (GLfloat)0.25, (GLfloat)-0.25, (GLfloat)0.25);
	//quad2 = QuadradoAvancado((GLfloat)-0.25, (GLfloat)-0.25, (GLfloat)0.25, (GLfloat)-0.25,
	//	(GLfloat)0.50, (GLfloat)0.25, (GLfloat)0.00, (GLfloat)0.25);
	//quad = Quadrado(/*(((GLfloat)(rand()%50))/100.0)+0.1*/(GLfloat)0.5, /*0*/-0.25, /*0*/-0.25,
	//	((GLfloat)(rand() % 256)) / 255.0, ((GLfloat)(rand() % 256)) / 255.0, ((GLfloat)(rand() % 256)) / 255.0);
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
	GLuint texture1 = loadTexture("C:/Users/TMB/Documents/ProjetoPG/ProjetoPG/Template2D/images/dandy.bmp");
	//GLuint texture2 = loadTexture("C:/Users/TMB/Documents/ProjetoPG/ProjetoPG/Template2D/images/phmb2.bmp");
	//GLuint texture3 = loadTexture("C:/Users/TMB/Documents/ProjetoPG/ProjetoPG/Template2D/images/phmb3.bmp");
	
	glBindTexture(GL_TEXTURE_2D, texture1);
	//imagem animada
	//if(cont < 10) glBindTexture(GL_TEXTURE_2D, texture1);
	//else if(cont >= 10 && cont < 20) glBindTexture(GL_TEXTURE_2D, texture2);
	//else if(cont >= 20 && cont < 30) glBindTexture(GL_TEXTURE_2D, texture3);
	
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
	glTexCoord2f(0, 1);  //so trocar o ultimo valor de todos pra ficar em pé/de cabeça pra baixo
	glVertex2f(quad2.x4, quad2.y4);
	//glVertex2f(quad.x, quad.y + quad.lado);
	//bottom left
	glTexCoord2f(0, 0);
	glVertex2f(quad2.x1, quad2.y1);
	//glVertex2f(quad.x, quad.y);
	//bottom right
	glTexCoord2f(1, 0);
	glVertex2f(quad2.x2, quad2.y2);
	//glVertex2f(quad.x + quad.lado, quad.y);
	//top right
	glTexCoord2f(1, 1);
	glVertex2f(quad2.x3, quad2.y3);
	//glVertex2f(quad.x + quad.lado, quad.y + quad.lado);

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
			int cont1 = 0, cont2 = 0, cont3 = 0, cont4 = 0;
			if (mouse_x > quad2.x1) cont1++;
			if (mouse_x > quad2.x2) cont1++;
			if (mouse_x > quad2.x3) cont1++;
			if (mouse_x > quad2.x4) cont1++;
			
			if (mouse_x < quad2.x1) cont2++;
			if (mouse_x < quad2.x2) cont2++;
			if (mouse_x < quad2.x3) cont2++;
			if (mouse_x < quad2.x4) cont2++;

			if (mouse_y > quad2.y1) cont3++;
			if (mouse_y > quad2.y2) cont3++;
			if (mouse_y > quad2.y3) cont3++;
			if (mouse_y > quad2.y4) cont3++;

			if (mouse_y < quad2.y1) cont4++;
			if (mouse_y < quad2.y2) cont4++;
			if (mouse_y < quad2.y3) cont4++;
			if (mouse_y < quad2.y4) cont4++;

			if (cont1 == 4 || cont2 == 4 || cont3 == 4 || cont4 == 4) {
				emMovimento = true;
				movimentoX = ((mouse_x - quad2.x1) + 0.0) / ESPACO;
				movimentoY = ((mouse_y - quad2.y1) + 0.0) / ESPACO;
				//quad.x = mouse_x;
				//quad.y = mouse_y;
				estado = MODIFIED;
			}else{
				//nanogui::Widget *panel = new nanogui::Widget(window);
				//nanogui::init();

				//talvez codigo aqui

				//nanogui::FormHelper *gui = new nanogui::FormHelper(screen);
				//ref<nanogui::Window> teste = gui->addWindow(vector<int>(10, 10), "AE");
				//ref<nanogui::Window> window = gui->addWindow();
				//vector<int> bla;
				//bla.push_back(10);
				//bla.push_back(10);
			}
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
	else if (key == 'r') { //rotacao
		comando = 'r';
	}
	else if (key == 'e') { //mudanca de escala
		comando = 'e';
	}
	else if (key == 'c') { //cisalhamento
		comando = 'c';
	}
	else if (key == 'o') { //pode realizar o comando
		//doMagic(); //funcao que vai realizar o comando
	}
	else if (key == 't') {
		//ponto_t.first = quad2.x1;
		//ponto_t.second = quad2.y1;
		printf("loucura (%lf,%lf)\n", quad2.x1, quad2.y1);
		//printf("loucura2 (%lf,%lf)\n", ponto_t.first, ponto_t.second);
		pair<GLfloat, GLfloat> ret = scale(2, 2/*, pair<GLfloat, GLfloat>(quad2.x1, quad2.y1)*/, quad2.x1, quad2.y1);
		quad2.x1 = ret.first;
		quad2.y1 = ret.second;
		printf("Ponto 1 - (%lf,%lf)\n", quad2.x1, quad2.y1);
		ret = scale(2, 2/*, pair<GLfloat, GLfloat>(quad2.x2, quad2.y2)*/, quad2.x2, quad2.y2);
		quad2.x2 = ret.first;
		quad2.y2 = ret.second;
		printf("Ponto 2 - (%lf,%lf)\n", quad2.x2, quad2.y2);
		ret = scale(2, 2/*, pair<GLfloat, GLfloat>(quad2.x3, quad2.y3)*/, quad2.x3, quad2.y3);
		quad2.x3 = ret.first;
		quad2.y3 = ret.second;
		printf("Ponto 3 - (%lf,%lf)\n", quad2.x3, quad2.y3);
		ret = scale(2, 2/*, pair<GLfloat, GLfloat>(quad2.x4, quad2.y4)*/, quad2.x4, quad2.y4);
		quad2.x4 = ret.first;
		quad2.y4 = ret.second;
		printf("Ponto 4 - (%lf,%lf)\n", quad2.x4, quad2.y4);
		estado = MODIFIED;
	}
}

void hadleSpecialKeyboard(int key, int x, int y) {
	if (key == GLUT_KEY_F5) {
		myinit();
	}
}

void loop(int id) {
	if (emMovimento) {
		qtdPassos++;
		quad2.x1 += movimentoX;
		quad2.y1 += movimentoY;
		quad2.x2 += movimentoX;
		quad2.y2 += movimentoY;
		quad2.x3 += movimentoX;
		quad2.y3 += movimentoY;
		quad2.x4 += movimentoX;
		quad2.y4 += movimentoY;
		estado = MODIFIED;
		if (qtdPassos == ESPACO) {
			qtdPassos = 0;
			movimentoX = 0.0;
			movimentoY = 0.0;
			emMovimento = false;
		}
	}
	if (estado == MODIFIED) {
		mydisplay();
		estado = IDLE;
	}
	else if (estado != IDLE) {
		mydisplay();
	}
	//cont = (cont + 1) % 30;
	if (cont == 10 || cont == 20 || cont == 0) mydisplay();//estado = MODIFIED;
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