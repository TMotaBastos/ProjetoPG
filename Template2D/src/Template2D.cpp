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
#include <list>
#define ESPACO 10

//como funciona:
//1 Caso - Usar rotacao('r'), aumento de escala('e'), cisalhamento('c')
//ou alterar a quantidade de passos (quao rapido a transformacao ira ocorrer)
//
//	1- selecione qual transformacao quer fazer ('r', 'e', 'c')
//	2- coloque os parametros de entrada (1 parametro para 'r', 2 parametros para 'e' e 'c')
//		caso haja mais de um parametro, separar com uma '/'
//		em caso de numero decimal, usar '.', ex.: em vez de "3,14", colocar "3.14"
//	3- insira a letra 'o' para aplicar a transformacao
//
//2 Caso - Fazer uma translacao
//	1- clique com o botao esquerdo do mouse para onde voce quer que o quadrado va
//
//3 Caso - Fazer uma reflexao
//	1- clique com o botao direito do mouse onde um dos pontos do segmento de reta deve estar
//	2- clique com o botao direito do mouse onde o outro ponto do segmento de reta deve estar
//	3- apos ter colocado todos os pontos, o programa ira fazer a reflexao automaticamente
//

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
GLfloat movimentoX1;
GLfloat movimentoY1;
GLfloat movimentoX2;
GLfloat movimentoY2;
GLfloat movimentoX3;
GLfloat movimentoY3;
GLfloat movimentoX4;
GLfloat movimentoY4;
bool emMovimento, emRotacao, emEscala, emCisalhamento;
int qtdPassos;
GLfloat window_width = /*350.0*/600.0;
GLfloat window_height = /*350.0*/600.0;
double t_matrix[3][3];
int matrix[3][1]; //só para não ficar dando erro, tirar depois
int matrix_c[3][1];
char comando;
//pair<GLfloat, GLfloat> ponto_t;
list<char> entradaNumero;
double parametro1, parametro2;
int padraoPassos;


//lembrar de colocar a matrix[][] (matriz do ponto) como paramêtro na função
//trocar o retorno também, os corretos estão comentados, assim como os próprios retornos
void clear() {
	t_matrix[0][0] = 0.0;//a
	t_matrix[0][1] = 0.0;//b
	t_matrix[0][2] = 0.0;//tx
	t_matrix[1][0] = 0.0;//c
	t_matrix[1][1] = 0.0;//d
	t_matrix[1][2] = 0.0;//ty
	t_matrix[2][0] = 0.0;
	t_matrix[2][1] = 0.0;
	t_matrix[2][2] = 1.0;
}

pair<GLfloat, GLfloat> scale(double sx, double sy, GLfloat xrec, GLfloat yrec) {
	
	t_matrix[0][0] = sx;
	t_matrix[1][1] = sy;

	GLfloat x, y;
	for (int i = 0; i < 3; i++) {
		GLfloat sum = 0;
		for (int j = 0; j < 3; j++) {
			if (j == 0) {
				sum += (t_matrix[i][j] * xrec);
			}else if(j == 1){
				sum += (t_matrix[i][j] * yrec);
			}
		}
		if (i == 0) x = sum;
		else if (i == 1) y = sum;
	}
	clear();
	return pair<GLfloat, GLfloat>(x, y);
}

pair<GLfloat, GLfloat> shear(double kx, double ky, GLfloat xrec, GLfloat yrec) {
	
	t_matrix[0][0] = 1;
	t_matrix[0][1] = kx;
	t_matrix[1][0] = ky;
	t_matrix[1][1] = 1;

	GLfloat x, y;
	for (int i = 0; i < 3; i++) {
		GLfloat sum = 0;
		for (int k = 0; k < 3; k++) {
			if (k == 0) {
				sum += (t_matrix[i][k] * xrec);
			}else if (k == 1) {
				sum += (t_matrix[i][k] * yrec);
			}
		}
		if (i == 0) x = sum;
		else if (i == 1) y = sum;
	}

	clear();
	return pair<GLfloat, GLfloat>(x, y);
}

pair<GLfloat, GLfloat> rotation(double angle, GLfloat xrec, GLfloat yrec) {
	// rotação antihorária
	//(cos -sen 0)
	//(sen  cos 0)
	//( 0    0  1)
	// rotação horária
	//( cos sen 0)
	//(-sen cos 0)
	//(  0   0  1)

	//0.017
	if (angle < 0) {
		t_matrix[0][0] = cos(angle*0.0174533);
		t_matrix[0][1] = -(sin(angle*0.0174533));
		t_matrix[1][0] = sin(angle*0.0174533);
		t_matrix[1][1] = cos(angle*0.0174533);
	}
	else if (angle > 0) {
		t_matrix[0][0] = cos(angle*0.0174533);
		t_matrix[0][1] = sin(angle*0.0174533);
		t_matrix[1][0] = -(sin(angle*0.0174533));
		t_matrix[1][1] = cos(angle*0.0174533);
	}
	else {
		t_matrix[0][0] = 1;
		t_matrix[0][1] = 0;
		t_matrix[1][0] = 0;
		t_matrix[1][1] = 1;
	}
	GLfloat x, y;
	for (int i = 0; i < 3; i++) {
		GLfloat sum = 0;
		for (int k = 0; k < 3; k++) {
			if (k == 0) {
				if (qtdPontos == 1) {
					sum += (t_matrix[i][k] * (xrec - ponto[0].x));
				}
				else if(qtdPontos == 0) {
					sum += (t_matrix[i][k] * xrec);
				}
			}else if (k == 1) {
				if (qtdPontos == 1) {
					sum += (t_matrix[i][k] * (yrec - ponto[0].y));
				}
				else if (qtdPontos == 0) {
					sum += (t_matrix[i][k] * yrec);
				}
			}
		}
		if (qtdPontos == 1) {
			if (i == 0) x = sum + ponto[0].x;
			else if (i == 1) y = sum + ponto[0].y;
		}
		else if(qtdPontos == 0){
			if (i == 0) x = sum;
			else if (i == 1) y = sum;
		}
	}
	clear();
	return pair<GLfloat, GLfloat>(x, y);
}

// para ser resolvido
pair<GLfloat, GLfloat>/*void*//*int***/ translation(double xrec, double yrec) {
	t_matrix[0][0] = 1;
	t_matrix[1][1] = 1;
	t_matrix[0][2] -= xrec;
	t_matrix[1][2] -= yrec;

	//int matrix_c[3][3];
	GLfloat x, y;
	for (int i = 0; i < 3; i++) {
		GLfloat sum = 0;
		for (int k = 0; k < 3; k++) {
			//sum += (matrix[i][k] * t_matrix[k][0]);
			//matrix_c[i][0] = sum;
			if (k == 0) {
				sum += (t_matrix[i][k] * xrec);
			}
			else if (k == 1) {
				sum += (t_matrix[i][k] * yrec);
			}
		}
		if (i == 0) x = sum;
		else if (i == 1) y = sum;
	}
	clear();
	return pair<GLfloat, GLfloat>(x, y);
	//return matrix_c;
}

//codigo antigo
//pair<GLfloat, GLfloat>/*int***/ reflection(Ponto a, Ponto b, double xrec, double yrec) {
/*Ponto vetor;
GLfloat xsai, ysai;
int dx = 0, dy = 0;
double matrixa[3][3];
double matrixb[3][3];
double matrixc[3][3];
double matrixd[3][3];
double matrixe[3][3];
double matrixaux[3][3];
double matrix_c[3][1];
double m = (b.y - a.y) / (b.x - a.x);
double y = (m*a.x) + a.y;
//if (/*a.x <= b.x &&*/ //a.y <= b.y) {
/*	vetor.x = b.x - a.x;
vetor.y = b.y - a.y;
}
else {
vetor.x = a.x - b.x;
vetor.y = a.y - b.y;
}*/
/*if (a.x <= b.x) {
matrixa[0][0] = 1;
matrixa[0][1] = 0;
matrixa[0][2] -= a.x;
matrixa[1][0] = 0;
matrixa[1][1] = 1;
matrixa[1][2] -= a.y;
matrixa[2][0] = 0;
matrixa[2][1] = 0;
matrixa[2][2] = 1;

matrixe[0][0] = 1;
matrixe[0][1] = 0;
matrixe[0][2] = a.x;
matrixe[1][0] = 0;
matrixe[1][1] = 1;
matrixe[1][2] = a.y;
matrixe[2][0] = 0;
matrixe[2][1] = 0;
matrixe[2][2] = 1;
}
else {
matrixa[0][0] = 1;
matrixa[0][1] = 0;
matrixa[0][2] -= b.x;
matrixa[1][0] = 0;
matrixa[1][1] = 1;
matrixa[1][2] -= b.y;
matrixa[2][0] = 0;
matrixa[2][1] = 0;
matrixa[2][2] = 1;

matrixe[0][0] = 1;
matrixe[0][1] = 0;
matrixe[0][2] = b.x;
matrixe[1][0] = 0;
matrixe[1][1] = 1;
matrixe[1][2] = b.y;
matrixe[2][0] = 0;
matrixe[2][1] = 0;
matrixe[2][2] = 1;
}*/
/*matrixa[0][0] = 1;
matrixa[0][1] = 0;
matrixa[0][2] -= 0;
matrixa[1][0] = 0;
matrixa[1][1] = 1;
matrixa[1][2] -= y;
matrixa[2][0] = 0;
matrixa[2][1] = 0;
matrixa[2][2] = 1;

matrixe[0][0] = 1;
matrixe[0][1] = 0;
matrixe[0][2] = 0;
matrixe[1][0] = 0;
matrixe[1][1] = 1;
matrixe[1][2] = y;
matrixe[2][0] = 0;
matrixe[2][1] = 0;
matrixe[2][2] = 1;

matrixb[0][0] = ((vetor.x * 0 + vetor.y * 1 + 1) / (sqrt(pow(vetor.x, 2) + pow(vetor.y, 2) + 1)*(sqrt(2))));
matrixb[0][1] = -(sqrt(1 - (pow(((vetor.x * 0 + vetor.y * 1 + 1) / (sqrt(pow(vetor.x, 2) + pow(vetor.y, 2) + 1)*(sqrt(2)))), 2))));
matrixb[0][2] = 0;
matrixb[1][0] = sqrt(1 - (pow(((vetor.x * 0 + vetor.y * 1 + 1) / (sqrt(pow(vetor.x, 2) + pow(vetor.y, 2) + 1)*(sqrt(2)))), 2)));
matrixb[1][1] = ((vetor.x * 0 + vetor.y * 1 + 1) / (sqrt(pow(vetor.x, 2) + pow(vetor.y, 2) + 1)*(sqrt(2))));
matrixb[1][2] = 0;
matrixb[2][0] = 0;
matrixb[2][1] = 0;
matrixb[2][2] = 1;

matrixd[0][0] = ((vetor.x * 0 + vetor.y * 1 + 1) / (sqrt(pow(vetor.x, 2) + pow(vetor.y, 2) + 1)*(sqrt(2))));
matrixd[0][1] = sqrt(1 - (pow(((vetor.x * 0 + vetor.y * 1 + 1) / (sqrt(pow(vetor.x, 2) + pow(vetor.y, 2) + 1)*(sqrt(2)))), 2)));
matrixd[0][2] = 0;
matrixd[1][0] = -(sqrt(1 - (pow(((vetor.x * 0 + vetor.y * 1 + 1) / (sqrt(pow(vetor.x, 2) + pow(vetor.y, 2) + 1)*(sqrt(2)))), 2))));
matrixd[1][1] = ((vetor.x * 0 + vetor.y * 1 + 1) / (sqrt(pow(vetor.x, 2) + pow(vetor.y, 2) + 1)*(sqrt(2))));
matrixd[1][2] = 0;
matrixd[2][0] = 0;
matrixd[2][1] = 0;
matrixd[2][2] = 1;

matrixc[0][0] = -1;
matrixc[0][1] = 0;
matrixc[0][2] = 0;
matrixc[1][0] = 0;
matrixc[1][1] = 1;
matrixc[1][2] = 0;
matrixc[2][0] = 0;
matrixc[2][1] = 0;
matrixc[2][2] = 1;
for (int i = 0; i < 3; i++) {
for (int j = 0; j < 3; j++) {
double sum = 0;
for (int k = 0; k < 3; k++) {
sum += (matrixa[i][j] * matrixb[j][k]);
matrixaux[i][j] = sum;
}
}
}
for (int i = 0; i < 3; i++) {
for (int j = 0; j < 3; j++) {
double sum = 0;
for (int k = 0; k < 3; k++) {
sum += (matrixaux[i][j] * matrixc[j][k]);
matrixa[i][j] = sum;
}
}
}
for (int i = 0; i < 3; i++) {
for (int j = 0; j < 3; j++) {
double sum = 0;
for (int k = 0; k < 3; k++) {
sum += (matrixa[i][j] * matrixd[j][k]);
matrixaux[i][j] = sum;
}
}
}
for (int i = 0; i < 3; i++) {
for (int j = 0; j < 3; j++) {
double sum = 0;
for (int k = 0; k < 3; k++) {
sum += (matrixaux[i][j] * matrixe[j][k]);
t_matrix[i][j] = sum;
}
}
}

for (int i = 0; i < 3; i++) {
double sum = 0;
for (int k = 0; k < 3; k++) {
//sum += (matrix[i][k] * t_matrix[k][0]);
//matrix_c[i][0] = sum;
if (k == 0) {
sum += (t_matrix[i][k] * xrec);
}
else if (k == 1) {
sum += (t_matrix[i][k] * yrec);
}
}
if (i == 0) xsai = sum;
else if (i == 1) ysai = sum;
}
clear();
return pair<GLfloat, GLfloat>(xsai, ysai);
}*/


pair<GLfloat, GLfloat>/*int***/ reflection(Ponto a, Ponto b, double xrec, double yrec) {
	Ponto vetor;
	GLfloat xsai, ysai;
	int dx = 0, dy = 0;
	double matrixa[3][3];
	double matrixb[3][3];
	double matrixc[3][3];
	double matrixd[3][3];
	double matrixe[3][3];
	double matrixaux[3][3];
	double matrix_c[3][1];
	double m = (b.y - a.y) / (b.x - a.x);
	double y = (m*a.x) + a.y;
	if (/*a.x <= b.x &&*/ a.y <= b.y) {
		vetor.x = b.x - a.x;
		vetor.y = b.y - a.y;
	}
	else {
		vetor.x = a.x - b.x;
		vetor.y = a.y - b.y;
	}

	/*matrixa[0][0] = 1;
	matrixa[0][1] = 0;
	matrixa[0][2] -= 0;
	matrixa[1][0] = 0;
	matrixa[1][1] = 1;
	matrixa[1][2] -= y;
	matrixa[2][0] = 0;
	matrixa[2][1] = 0;
	matrixa[2][2] = 1;

	matrixe[0][0] = 1;
	matrixe[0][1] = 0;
	matrixe[0][2] = 0;
	matrixe[1][0] = 0;
	matrixe[1][1] = 1;
	matrixe[1][2] = y;
	matrixe[2][0] = 0;
	matrixe[2][1] = 0;
	matrixe[2][2] = 1;
	*/
	matrixb[0][0] = ((vetor.x * 0 + vetor.y * 1 + 1) / (sqrt(pow(vetor.x, 2) + pow(vetor.y, 2) + 1)*(sqrt(2))));
	matrixb[0][1] = -(sqrt(1 - (pow(((vetor.x * 0 + vetor.y * 1 + 1) / (sqrt(pow(vetor.x, 2) + pow(vetor.y, 2) + 1)*(sqrt(2)))), 2))));
	matrixb[0][2] = 0;
	matrixb[1][0] = sqrt(1 - (pow(((vetor.x * 0 + vetor.y * 1 + 1) / (sqrt(pow(vetor.x, 2) + pow(vetor.y, 2) + 1)*(sqrt(2)))), 2)));
	matrixb[1][1] = ((vetor.x * 0 + vetor.y * 1 + 1) / (sqrt(pow(vetor.x, 2) + pow(vetor.y, 2) + 1)*(sqrt(2))));
	matrixb[1][2] -= y;
	matrixb[2][0] = 0;
	matrixb[2][1] = 0;
	matrixb[2][2] = 1;

	matrixd[0][0] = ((vetor.x * 0 + vetor.y * 1 + 1) / (sqrt(pow(vetor.x, 2) + pow(vetor.y, 2) + 1)*(sqrt(2))));
	matrixd[0][1] = sqrt(1 - (pow(((vetor.x * 0 + vetor.y * 1 + 1) / (sqrt(pow(vetor.x, 2) + pow(vetor.y, 2) + 1)*(sqrt(2)))), 2)));
	matrixd[0][2] = 0;
	matrixd[1][0] = -(sqrt(1 - (pow(((vetor.x * 0 + vetor.y * 1 + 1) / (sqrt(pow(vetor.x, 2) + pow(vetor.y, 2) + 1)*(sqrt(2)))), 2))));
	matrixd[1][1] = ((vetor.x * 0 + vetor.y * 1 + 1) / (sqrt(pow(vetor.x, 2) + pow(vetor.y, 2) + 1)*(sqrt(2))));
	matrixd[1][2] = y;
	matrixd[2][0] = 0;
	matrixd[2][1] = 0;
	matrixd[2][2] = 1;

	matrixc[0][0] = -1;
	matrixc[0][1] = 0;
	matrixc[0][2] = 0;
	matrixc[1][0] = 0;
	matrixc[1][1] = 1;
	matrixc[1][2] = 0;
	matrixc[2][0] = 0;
	matrixc[2][1] = 0;
	matrixc[2][2] = 1;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			double sum = 0;
			for (int k = 0; k < 3; k++) {
				sum += (matrixd[i][j] * matrixc[j][k]);
				matrixa[i][j] = sum;
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			double sum = 0;
			for (int k = 0; k < 3; k++) {
				sum += (matrixa[i][j] * matrixb[j][k]);
				t_matrix[i][j] = sum;
			}
		}
	}

	for (int i = 0; i < 3; i++) {
		double sum = 0;
		for (int k = 0; k < 3; k++) {
			//sum += (matrix[i][k] * t_matrix[k][0]);
			//matrix_c[i][0] = sum;
			if (k == 0) {
				sum += (t_matrix[i][k] * xrec);
			}
			else if (k == 1) {
				sum += (t_matrix[i][k] * yrec);
			}
		}
		if (i == 0) xsai = sum;
		else if (i == 1) ysai = sum;
	}
	clear();
	return pair<GLfloat, GLfloat>(xsai, ysai);
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

	//verifica se o arquivo existe
	if (!file) {
		printf("Nao existe essa imagem!\n");
		return 0;
	}
	//verifica se o header possui 54 bytes, que eh o que deveria conter
	if (fread(header, 1, 54, file) != 54) {
		printf("Nao eh uma imagem valida!\n");
		return 0;
	}
	//imagens BMP comecam com "BM"
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

void myinit() {
	srand(time(NULL));
	//qtdQuadrados = 0;	
	cont = 0;
	movimentoX1 = 0.0;
	movimentoY1 = 0.0;
	movimentoX2 = 0.0;
	movimentoY2 = 0.0;
	movimentoX3 = 0.0;
	movimentoY3 = 0.0;
	movimentoX4 = 0.0;
	movimentoY4 = 0.0;
	emMovimento = false;
	emEscala = false;
	emRotacao = false;
	emCisalhamento = false;
	comando = 'n';
	parametro1 = 0.0;
	parametro2 = 0.0;
	padraoPassos = 10;
	clear();
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
	//colocando na tela o quadrado	
	glColor3f(1.0f, 1.0f, 1.0f);
	//colocando a textura
	GLuint texture1 = loadTexture("C:/Users/TMB/Documents/ProjetoPG/ProjetoPG/Template2D/images/dandy.bmp");
	GLuint texture2 = loadTexture("C:/Users/TMB/Documents/ProjetoPG/ProjetoPG/Template2D/images/cage5.bmp");
	GLuint texture3 = loadTexture("C:/Users/TMB/Documents/ProjetoPG/ProjetoPG/Template2D/images/lapprand.bmp");
	
	glBindTexture(GL_TEXTURE_2D, texture1);
	//imagem animada
	if(cont < 10) glBindTexture(GL_TEXTURE_2D, texture1);
	else if(cont >= 10 && cont < 20) glBindTexture(GL_TEXTURE_2D, texture2);
	else if(cont >= 20 && cont < 30) glBindTexture(GL_TEXTURE_2D, texture3);
	
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

//acho que nao precisa
void handleMotion(int x, int y) {
	if ((estado != MODIFIED) && (estado != IDLE)) {
		quad/*[estado]*/.x = (((((GLfloat)x) / window_width)*2.0) - 1.0) + mouse_x;
		quad/*[estado]*/.y = (-(((((GLfloat)y) / window_height)*2.0) - 1.0)) + mouse_y;
	}
}

void doScale(double sx, double sy) {
	pair<GLfloat, GLfloat> ret = scale(sx, sy, quad2.x1, quad2.y1);
	movimentoX1 = ((ret.first - quad2.x1) / padraoPassos);
	movimentoY1 = ((ret.second - quad2.y1) / padraoPassos);
	ret = scale(sx, sy, quad2.x2, quad2.y2);
	movimentoX2 = ((ret.first - quad2.x2) / padraoPassos);
	movimentoY2 = ((ret.second - quad2.y2) / padraoPassos);
	ret = scale(sx, sy, quad2.x3, quad2.y3);
	movimentoX3 = ((ret.first - quad2.x3) / padraoPassos);
	movimentoY3 = ((ret.second - quad2.y3) / padraoPassos);
	ret = scale(sx, sy, quad2.x4, quad2.y4);
	movimentoX4 = ((ret.first - quad2.x4) / padraoPassos);
	movimentoY4 = ((ret.second - quad2.y4) / padraoPassos);
	emEscala = true;
	estado = MODIFIED;
}

void doRotation(double angle) {
	pair<GLfloat, GLfloat> ret = rotation(angle, quad2.x1, quad2.y1);
	movimentoX1 = ((ret.first - quad2.x1) / padraoPassos);
	movimentoY1 = ((ret.second - quad2.y1) / padraoPassos);
	ret = rotation(angle, quad2.x2, quad2.y2);
	movimentoX2 = ((ret.first - quad2.x2) / padraoPassos);
	movimentoY2 = ((ret.second - quad2.y2) / padraoPassos);
	ret = rotation(angle, quad2.x3, quad2.y3);
	movimentoX3 = ((ret.first - quad2.x3) / padraoPassos);
	movimentoY3 = ((ret.second - quad2.y3) / padraoPassos);
	ret = rotation(angle, quad2.x4, quad2.y4);
	movimentoX4 = ((ret.first - quad2.x4) / padraoPassos);
	movimentoY4 = ((ret.second - quad2.y4) / padraoPassos);
	emRotacao = true;
	estado = MODIFIED;
}

void doShear(double kx, double ky) {
	pair<GLfloat, GLfloat> ret = shear(kx, ky, quad2.x1, quad2.y1);
	movimentoX1 = ((ret.first - quad2.x1) / padraoPassos);
	movimentoY1 = ((ret.second - quad2.y1) / padraoPassos);
	ret = shear(kx, ky, quad2.x2, quad2.y2);
	movimentoX2 = ((ret.first - quad2.x2) / padraoPassos);
	movimentoY2 = ((ret.second - quad2.y2) / padraoPassos);
	ret = shear(kx, ky, quad2.x3, quad2.y3);
	movimentoX3 = ((ret.first - quad2.x3) / padraoPassos);
	movimentoY3 = ((ret.second - quad2.y3) / padraoPassos);
	ret = shear(kx, ky, quad2.x4, quad2.y4);
	movimentoX4 = ((ret.first - quad2.x4) / padraoPassos);
	movimentoY4 = ((ret.second - quad2.y4) / padraoPassos);
	emCisalhamento = true;
	estado = MODIFIED;
}

void doReflection() {
	pair<GLfloat, GLfloat> ret = reflection(ponto[0], ponto[1], quad2.x1, quad2.y1);
	quad2.x1 = ret.first;
	quad2.y1 = ret.second;
	ret = reflection(ponto[0], ponto[1], quad2.x2, quad2.y2);
	quad2.x2 = ret.first;
	quad2.y2 = ret.second;
	ret = reflection(ponto[0], ponto[1], quad2.x3, quad2.y3);
	quad2.x3 = ret.first;
	quad2.y3 = ret.second;
	ret = reflection(ponto[0], ponto[1], quad2.x4, quad2.y4);
	quad2.x4 = ret.first;
	quad2.y4 = ret.second;
	estado = MODIFIED;
}

//x e y sao as coordenas onde cliquei na tela
//x2 e y2 eh onde cliquei so que ele converte pra as coordenadas de tela do OpenGL
void handleMouse(int btn, int state, int x, int y) {
	//provavelmente nao precisa desse if
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
				movimentoX1 = ((mouse_x - quad2.x1) + 0.0) / padraoPassos;
				movimentoY1 = ((mouse_y - quad2.y1) + 0.0) / padraoPassos;
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
			if (qtdPontos == 2) {
				doReflection();
				qtdPontos = 0;
				estado = MODIFIED;
			}
			//estado = MODIFIED;
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

double parsingNumber() {
	list<char>::iterator it;
	int cont = 0, expoente = 0;
	double base = 10.0, numeroFinal = 0.0;
	for (it = entradaNumero.begin(); it != entradaNumero.end(); ++it) {
		if ((*it) == '.') {
			break;
		}
		else {
			if((*it) != '-') cont++;
		}
	}
	expoente = cont - 1;
	base = pow(base, expoente);
	bool negativo = false;
	for (it = entradaNumero.begin(); it != entradaNumero.end(); ++it) {
		if ((*it) == '-') negativo = true;
		if ((*it) != '.' && (*it) != '-') {
			numeroFinal += (((*it) - '0') * base);
			base /= 10;
		}
	}
	if (negativo) numeroFinal *= -1;
	for (int i = 0; i < entradaNumero.size();) {
		entradaNumero.pop_front();
	}
	return numeroFinal;
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
	else if (key == 'p') { //settar a quantidade de passos
		comando = 'p';
	}
	else if (key == 'o') { //pode realizar o comando
		if (comando == 'r') {
			parametro1 = parsingNumber();
			doRotation(parametro1);
		}
		else if (comando == 'e') {
			parametro2 = parsingNumber();
			doScale(parametro1, parametro2);
		}
		else if (comando == 'c') {
			parametro2 = parsingNumber();
			doShear(parametro1, parametro2);
		}
		else if (comando == 'p') {
			parametro1 = parsingNumber();
			padraoPassos = (int) parametro1;
		}
		else {
			printf("Primeiro escolha a transformacao desejada!\n");
		}
	}
	else if (key == 't') { //funcao para testes :P
		//ponto_t.first = quad2.x1;
		//ponto_t.second = quad2.y1;
		printf("loucura (%lf,%lf)\n", quad2.x1, quad2.y1);
		//printf("loucura2 (%lf,%lf)\n", ponto_t.first, ponto_t.second);
		pair<GLfloat, GLfloat> ret = scale(0.5, 0.5/*, pair<GLfloat, GLfloat>(quad2.x1, quad2.y1)*/, quad2.x1, quad2.y1);
		quad2.x1 = ret.first;
		quad2.y1 = ret.second;
		printf("Ponto 1 - (%lf,%lf)\n", quad2.x1, quad2.y1);
		ret = scale(0.5, 0.5/*, pair<GLfloat, GLfloat>(quad2.x2, quad2.y2)*/, quad2.x2, quad2.y2);
		quad2.x2 = ret.first;
		quad2.y2 = ret.second;
		printf("Ponto 2 - (%lf,%lf)\n", quad2.x2, quad2.y2);
		ret = scale(0.5, 0.5/*, pair<GLfloat, GLfloat>(quad2.x3, quad2.y3)*/, quad2.x3, quad2.y3);
		quad2.x3 = ret.first;
		quad2.y3 = ret.second;
		printf("Ponto 3 - (%lf,%lf)\n", quad2.x3, quad2.y3);
		ret = scale(0.5, 0.5/*, pair<GLfloat, GLfloat>(quad2.x4, quad2.y4)*/, quad2.x4, quad2.y4);
		quad2.x4 = ret.first;
		quad2.y4 = ret.second;
		printf("Ponto 4 - (%lf,%lf)\n", quad2.x4, quad2.y4);
		estado = MODIFIED;
	}
	else if (key >= '0' && key <= '9' || key == '.' || key == '-') {
		entradaNumero.push_back(key);
	}else if(key == '/'){
		parametro1 = parsingNumber();
	}
}

void hadleSpecialKeyboard(int key, int x, int y) {
	if (key == GLUT_KEY_F5) {
		myinit();
	}
}

void inMotion() {
	qtdPassos++;
	quad2.x1 += movimentoX1;
	quad2.y1 += movimentoY1;
	quad2.x2 += movimentoX2;
	quad2.y2 += movimentoY2;
	quad2.x3 += movimentoX3;
	quad2.y3 += movimentoY3;
	quad2.x4 += movimentoX4;
	quad2.y4 += movimentoY4;
	estado = MODIFIED;
}

void endMotion() {
	qtdPassos = 0;
	movimentoX1 = 0.0;
	movimentoY1 = 0.0;
	movimentoX2 = 0.0;
	movimentoY2 = 0.0;
	movimentoX3 = 0.0;
	movimentoY3 = 0.0;
	movimentoX4 = 0.0;
	movimentoY4 = 0.0;
}

void loop(int id) {
	if (emMovimento) {
		qtdPassos++;
		quad2.x1 += movimentoX1;
		quad2.y1 += movimentoY1;
		quad2.x2 += movimentoX1;
		quad2.y2 += movimentoY1;
		quad2.x3 += movimentoX1;
		quad2.y3 += movimentoY1;
		quad2.x4 += movimentoX1;
		quad2.y4 += movimentoY1;
		estado = MODIFIED;
		if (qtdPassos == padraoPassos) {
			endMotion();
			emMovimento = false;
		}
	}else if(emEscala){
		inMotion();
		if (qtdPassos == padraoPassos) {
			endMotion();
			emEscala = false;
		}
	}
	else if (emCisalhamento) {
		inMotion();
		if (qtdPassos == padraoPassos) {
			endMotion();
			emCisalhamento = false;
		}
	}
	else if (emRotacao) {
		inMotion();
		if (qtdPassos == padraoPassos) {
			endMotion();
			emRotacao = false;
		}
	}
	if	(estado == MODIFIED) {
		mydisplay();
		estado = IDLE;
	}
	else if (estado != IDLE) {
		mydisplay();
	}
	cont = (cont + 1) % 30;
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