#include"function.h"

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(500, 200);
	glutInitWindowSize(500, 500);
	glewInit();

	glutCreateWindow("Pacman Game");

	glutDisplayFunc(graphics_display);
	SetupRC(10, 10);
	glutMainLoop();
	

	system("pause");
	return 0;
}