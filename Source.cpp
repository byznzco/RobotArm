#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

#define D2R 0.01745329

// State of Some Keys
bool aKey = false, wKey = false, sKey = false, dKey = false, plusKey = false, minusKey = false;
bool leftKey = false, rightKey = false, upKey = false, downKey = false;

// 3D Text with a Vector Font
void text(const char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
}

// 2D Text with a Bitmap Font
void print(int x, int y, void *font, const char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

void circle(float x, float y, float r) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i += 10) {
		glVertex2f(x + r * cos(i * D2R), y + r * sin(i * D2R));
	}
	glEnd();
}
void ArmPart() {
	glColor3f(0.5, 0.5, 1.0);
	glRectf(0, 30, 250, -30);
	circle(250, 0, 50);
	
}
void ArmPartLeft() {
	
	glColor3f(0.5, 0.5, 1.0);
	glRectf(0, -30, -250, +30);
	circle(-250, 0, 50);
	
}
void UpperArm() {
	ArmPart();
}

void UpperArmLeft() {
	ArmPartLeft();
}

float upper_angle = 235, lower_angle = 280;

void RobotArmRight() {

	glRotatef(upper_angle, 0, 0, 1);
	UpperArm();
	glTranslatef(230, 0, 0);
	glRotatef(lower_angle, 0, 0, 1);

}

void RobotArmLeft() {

	glRotatef(-upper_angle, 0, 0, 1);
	glTranslatef(0, 0, 0);
	UpperArmLeft();
	glTranslatef(-230, 0, 0);
	glRotatef(-lower_angle, 0, 0, 1);
}
void Axes()
{
	glLineWidth(3);
	glColor3f(0.8, 0.8, 0.8);

	glBegin(GL_LINES);

	// x-axis
	glVertex2f(600,0);
	glVertex2f(-600,0);

	// x-axis
	glVertex2f(0, 400);
	glVertex2f(0, -400);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(600, 400);
	glVertex2f(-600, 400);
	glVertex2f(-600, -400);
	glVertex2f(600, -400);
	glEnd();
}
void Label() 
{

	glColor3f(1, 1, 0);

	glTranslatef(-500, -8, 0);
	glScalef(0.20, 0.15, 1);
	text("Model Transformations");
}
float rx = 0, ry = 100;
void Scene() 
{
	Axes();
	glColor3f(0.5, 0.5, 1.0);
	glTranslatef(rx, ry, 0);
	circle(0, 300, 100);
	glRectf(-40, 220, 40, -220);

	glPushMatrix();
	glTranslatef(0, 200, 0);
	RobotArmRight();
	
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 200, 0);
	RobotArmLeft();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -200, 0);
	RobotArmRight();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -200, 0);
	RobotArmLeft();
	glPopMatrix();

	glLoadIdentity();
	glTranslatef(0, -350, 0);
	Label();
}
float zoom = 1.0;
void display() {
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	//Scene
	glScalef(zoom, zoom, 1);
	Scene();

	glutSwapBuffers();
}

void onResize(int w, int h) {
	// printf("Resize : %d %d\n", w, h);
	glViewport(0, 0, w, h); // use whole window

	// Update Projection Matrix in the Rendering Pipeline
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2.0, w / 2.0, -h / 2.0, h / 2.0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
}

void onClick(int button, int state, int x, int y) {
	// printf("Click : %d %d (%d %d)\n", button, state, x, y);
}

void onMoveDown(int x, int y) {
	// printf("MoveDown : %d %d\n", x, y);
}

void onMove(int x, int y) {
	// printf("Move : %d %d\n", x, y);
}

void onWheel(int button, int dir, int x, int y) {
	// printf("Wheel: %d\n", dir);
}

void onSpecialKeyDown(int key, int x, int y) {
	// printf("SpecialKeyDown: %d\n", key);

	switch (key) {
	case GLUT_KEY_LEFT: leftKey = true; break;
	case GLUT_KEY_RIGHT: rightKey = true; break;
	case GLUT_KEY_DOWN: downKey = true;  break;
	case GLUT_KEY_UP: upKey = true; break;
	}
}
void onSpecialKeyUp(int key, int x, int y) {
	// printf("SpecialKeyUp: %d\n", key);

	switch (key) {
	case GLUT_KEY_LEFT: leftKey = false; break;
	case GLUT_KEY_RIGHT: rightKey = false; break;
	case GLUT_KEY_DOWN: downKey = false; break;
	case GLUT_KEY_UP: upKey = false; break;
	}
}

void onKeyDown(unsigned char key, int x, int y) {
	// printf("ASCII-Key-Down: %c (%d)\n", key, key);
	if (key == 27) exit(0);

	switch (key) {
	case 'a': aKey = true; break;
	case 'd': dKey = true; break;
	case 's': sKey = true; break;
	case 'w': wKey = true; break;

	case '+': plusKey = true; break;
	case '-': minusKey = true; break;
	}
}

void onKeyUp(unsigned char key, int x, int y) {
	// printf("ASCII-Key-Up: %c (%d)\n", key, key);
	switch (key) {
	case 'a': aKey = false; break;
	case 'd': dKey = false; break;
	case 's': sKey = false; break;
	case 'w': wKey = false; break;

	case '+': plusKey = false; break;
	case '-': minusKey = false; break;
	}
}

void gameLoop(int v) {
	glutTimerFunc(16, gameLoop, 0);

	if (upKey) upper_angle++;
	if (downKey) upper_angle--;

	if (rightKey) lower_angle++;
	if (leftKey) lower_angle--;

	if (aKey) rx--;
	if (dKey) rx++;
	if (wKey) ry++;
	if (sKey) ry--;

	if (plusKey && zoom < 1.5) zoom += 0.01;
	if (minusKey && zoom > 0.6) zoom -= 0.01;

	glutPostRedisplay();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(2000, 2000);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutCreateWindow("Freeglut");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);
	/* Mouse related Events are registered */
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);
	glutMouseWheelFunc(onWheel);

	/* Keyboard related Events */
	glutSpecialFunc(onSpecialKeyDown);
	glutSpecialUpFunc(onSpecialKeyUp);

	glutKeyboardFunc(onKeyDown);
	glutKeyboardUpFunc(onKeyUp);

	// Timer  setTimeout(fn, period)
	// frame locking. ( 16 ms --> 60 fps )
	glutTimerFunc(16, gameLoop, 0);

	glutMainLoop();

	return 0;
}