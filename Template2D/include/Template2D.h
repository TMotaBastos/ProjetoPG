/*
-----------------------------------------------------------------------------
OpenGL Tutorial
VOXAR Labs
Computer Science Center - CIn
Federal University of Pernambuco - UFPE
http://www.cin.ufpe.br/~voxarlabs
@author Tullio Lucena - tjsl
-----------------------------------------------------------------------------
*/
#ifndef _OPENGL_TUTORIAL_H_
#define _OPENGL_TUTORIAL_H_
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <gl/glut.h>
//#include <vector>
using namespace std;
// Constantes do programa
#define FPS 30 // Quantidade de atualizacoes por segundo do programa
#define ESC 27 // Valor da tecla ESC
#define IDLE -2 // Nada a fazer
#define MODIFIED -1 // A tela foi modificada
struct Quadrado {
	GLfloat lado, x, y, r, g, b;
	Quadrado(GLfloat lado = 0.0, GLfloat x = 0.0, GLfloat y = 0.0, GLfloat r = 0.0, GLfloat g = 0.0, GLfloat b = 0.0) :lado(lado), x(x), y(y), r(r), g(g), b(b) {}
};
/*
(x4,y4)		(x3,y3)
	 _______
	|		|
	|		|
	|_______|
(x1,y1)		(x2,y2)
*/
struct QuadradoAvancado {
	GLfloat x1, y1, x2, y2, x3, y3, x4, y4;
	QuadradoAvancado(GLfloat x1 = 0.0, GLfloat y1 = 0.0, GLfloat x2 = 0.0, GLfloat y2 = 0.0, GLfloat x3 = 0.0, GLfloat y3 = 0.0, GLfloat x4 = 0.0, GLfloat y4 = 0.0) : x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3), x4(x4), y4(y4){}
};

struct Ponto {
	GLfloat x, y;
	Ponto(GLfloat x = 0.0, GLfloat y = 0.0) : x(x), y(y) {}
};
/**
* Funcao para inicializacao do programa.
*/
void myinit();
/**
* Esta funcao vai ser chamada quando a tela for redimensionada.
* @param w Largura atual da tela.
* @param h Altura atual da tela.
*/
void myreshape(GLsizei w, GLsizei h);
/**
* Esta � a funcao responsavel por pintar os objetos na tela.
*/
void mydisplay();
/**
* Esta funcao vai ser chamada toda vez que o mouse se mover com uma tecla apertada.
* Vai atualizar a posicao do quadrado que esta sendo movido.
* @param x Coordenada x atual do mouse.
* @param y Coordenada y atual do mouse.
*/
void handleMotion(int x, int y);
/**
* Esta funcao vai ser chamada toda vez que uma tecla do mouse seja clicada ou levantada.
* Com o botao direito do mouse, um quadrado pode ser criado na posicao atual do mouse, com uma cor e tamanho aleatorio.
* Com o botao esquerdo do mouse, um quadrado pode ser movimentado.
* @param btn Tecla levantada. Ex: GLUT_LEFT_BUTTON.
* @param state Estado da tecla. Ex: GLUT_DOWN.
* @param x Coordenada x atual do mouse.
* @param y Coordenada y atual do mouse.
*/
void handleMouse(int btn, int state, int x, int y);
/**
* Esta funcao vai ser chamada toda vez que uma tecla comum seja levantada.
* @param key Tecla levantada.
* @param x Coordenada x atual do mouse.
* @param y Coordenada y atual do mouse.
*/
void hadleKeyboard(unsigned char key, int x, int y);
/**
* Esta funcao vai ser chamada toda vez que uma tecla especial seja levantada.
* Botao F5 o programa volta ao estado inicial.
* @param key Tecla levantada. Ex: GLUT_KEY_F5.
* @param x Coordenada x atual do mouse.
* @param y Coordenada y atual do mouse.
*/
void hadleSpecialKeyboard(int key, int x, int y);
/**
* Representa uma maquina de estados do programa. Vai ser executada FPS vezes por segundo.
* @param id Identificador da maquina de estados.
*/
void loop(int id);
#endif //_OPENGL_TUTORIAL_H_