#include <stdio.h>
#include <math.h>
#include <random>
#include <glut.h>
#include <string.h>
#include <iostream>

#define PI 3.14159265358
static int radius = 10;
float rotationAngle = 0.0f;
float walkingverticle = 0;
float walkinghorizontal = 0;
float firstPx, firstPy;
float secondPx, secondPy;
float lifex = 0;
int numObstacles = 6;
int numcollectables = 6;
float collectablex[6], collectabley[6];
float obstacleX[6], obstacleY[6];
int rotateplayer = 0;
int gameTime = 60;
int score = 0;
int personSpeed = 10;
bool won = false;
int gamefreeze = 0;
int oldcollectabley[6], oldgoaly, oldfirsty, oldsecondy;
int currenty = -120;

void rotatePlayer(int value);
void speedo(int speedtime);
void drawPowerUp(float firstx, float firsty, float secondx, float secondy);
void collisioned(float playerX, float playerY, float objectX, float objectY, float objectWidth, float objectHeight, std::string nameOfObject, std::string nameOfPower);
void drawPlayer() {
	const double scaleFactor = 0.4;
	double scaledRadius = radius * scaleFactor;
	glPushMatrix();
	glTranslated(400 + walkinghorizontal, 300 + walkingverticle, 0);
	glBegin(GL_POLYGON);  // <--------------Body (rectangle)
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(50.0, 50.0, 0.0);
	glVertex3f(55.0, 50.0, 0.0);
	glVertex3f(55.0, 70.0, 0.0);
	glVertex3f(50.0, 70.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);  //<---------------Head (circle)
	glColor3f(1.0f, 1.0f, 1.0f);
	for (double i = 0; i < 2 * PI; i += PI / 24) {
		glVertex3f((cos(i) * scaledRadius) + 52.5, (sin(i) * scaledRadius) + 70.0, 0.0);
	}
	glEnd();

	glBegin(GL_LINES);
	for (int i = 0; i < 2; i++) {  //<------------- Arms (lines)
		glColor3f(1.0f, 1.0f, 1.0f);
		double x1 = 52.0 + (i * 2.5);
		double x2 = 48.0 + (i * 15);
		double y2 = 58.0 + (i * 10);
		glVertex3f(x1, 60.0, 0.0);
		glVertex3f(x2, y2, 0.0);
	}
	glEnd();

	glBegin(GL_TRIANGLES);  //<----------- Leg (triangle)
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(53.0, 60.0, 0.0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(45.0, 40.0, 0.0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(58.0, 40.0, 0.0);
	glEnd();
	glPopMatrix();
}

void drawPrize() {
	glPushMatrix();
	glTranslated(750.0, currenty, 0.0);
	glBegin(GL_POLYGON);//<--------------------square
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(200.0f, 200.0f, 0.0f);
	glVertex3f(150.0f, 200.0f, 0.0f);
	glVertex3f(150.0f, 150.0f, 0.0f);
	glVertex3f(200.0f, 150.0f, 0.0f);
	glEnd();
	glBegin(GL_TRIANGLES);//<--------------------triangle1
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(175.0f, 200.0f, 0.0f);
	glVertex3f(200.0f, 150.0f, 0.0f);
	glVertex3f(150.0f, 150.0f, 0.0f);
	glEnd();
	glBegin(GL_TRIANGLES);//<--------------------triangle2
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(150.0f, 200.0f, 0.0f);
	glVertex3f(200.0f, 200.0f, 0.0f);
	glVertex3f(175.0f, 150.0f, 0.0f);
	glEnd();
	glBegin(GL_LINE_LOOP);//<--------------------frame
	glColor3f(0.8f, 0.8f, 0.8f);
	glVertex3f(200.0f, 200.0f, 0.0f);
	glVertex3f(150.0f, 200.0f, 0.0f);
	glVertex3f(150.0f, 150.0f, 0.0f);
	glVertex3f(200.0f, 150.0f, 0.0f);
	glEnd();
	glBegin(GL_POLYGON);//<--------------------rhombus
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(175.0f, 180.0f, 0.0f);
	glVertex3f(170.0f, 175.0f, 0.0f);
	glVertex3f(175.0f, 170.0f, 0.0f);
	glVertex3f(180.0f, 175.0f, 0.0f);
	glEnd();
	glPopMatrix();

}

void drawPowerUp(float firstx, float firsty, float secondx, float secondy) {
	glPushMatrix();
	glTranslated(firstx-250, firsty-250, 0);
	glScaled(5, 5, 1);
	glBegin(GL_POLYGON);//<-------------------lightning for extra speed
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(50.0, 50.0, 0.0);
	glVertex3f(55.0, 50.0, 0.0);
	glVertex3f(52.5, 49.5, 0.0);
	glVertex3f(50.0, 49.5, 0.0);
	glVertex3f(52.5, 49.0, 0.0);
	glVertex3f(50.0, 49.0, 0.0);
	glVertex3f(48.5, 48.5, 0.0);
	glVertex3f(50.0, 48.5, 0.0);
	glVertex3f(52.5, 48.0, 0.0);
	glVertex3f(50.0, 48.0, 0.0);
	glVertex3f(55.0, 47.5, 0.0);
	glVertex3f(50.0, 47.5, 0.0);
	glVertex3f(48.5, 47.0, 0.0);
	glVertex3f(50.0, 47.0, 0.0);
	glVertex3f(52.5, 46.5, 0.0);
	glVertex3f(50.0, 46.5, 0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslated(secondx, secondy, 0);
	glBegin(GL_TRIANGLES);//<-------top part of the sandclock to slower the time
	glColor3f(0.8f, 0.8f, 0.8f);
	glVertex3f(150.0, 150.0, 0.0);
	glVertex3f(160.0, 150.0, 0.0);
	glVertex3f(155.0, 140.0, 0.0);
	glEnd();
	glBegin(GL_TRIANGLES);//<-------bottom part of the sandclock
	glVertex3f(160.0, 130.0, 0.0);
	glVertex3f(150.0, 130.0, 0.0);
	glVertex3f(155.0, 140.0, 0.0);
	glEnd();
	glPopMatrix();
}

void drawObstacle() {
	const double scaleFactor = 0.3;
	double scaledRadius = radius * scaleFactor;
	int numSpikes = 12;
	for (int i = 0; i < numObstacles; i++) {
		glPushMatrix();

		glTranslated(obstacleX[i], obstacleY[i], 0);

		glRotated(rotationAngle, 0, 0, 1);

		glScaled(2, 1, 1);
		glBegin(GL_POLYGON);  // <--------------- Body (circle)
		for (double j = 0; j < 2 * PI; j += PI / 24) {
			glColor3f(1.0f, 1.0f, 1.0f);
			double scaledRadius = radius * scaleFactor;
			glVertex3f((cos(j) * scaledRadius), (sin(j) * scaledRadius), 0.0);
		}
		glEnd();

		glBegin(GL_TRIANGLES);  //<------------- spikes (triangles)
		for (int j = 0; j < numSpikes; j++) {
			glColor3f(1.0f, 0.0f, 0.0f);
			double angle = j * (2 * PI / numSpikes);
			double x1 = cos(angle) * scaledRadius * 1.2;
			double y1 = sin(angle) * scaledRadius * 1.2;
			double x2 = cos(angle) * (scaledRadius + 10);
			double y2 = sin(angle) * (scaledRadius + 10);
			double x3 = cos(angle + (2 * PI / numSpikes)) * scaledRadius * 1.2;
			double y3 = sin(angle + (2 * PI / numSpikes)) * scaledRadius * 1.2;
			glVertex3f(x1, y1, 0.0);
			glVertex3f(x2, y2, 0.0);
			glVertex3f(x3, y3, 0.0);
		}
		glEnd();

		glPopMatrix();
	}
}

void drawCollectables() {

	for (int i = 0; i < numcollectables; i++) {

		glPushMatrix();
		glTranslated(collectablex[i]-85, collectabley[i]-75, 0);
		glBegin(GL_POLYGON);//<--------------------square
		glColor3f(0.2f, 0.8f, 1.0f);
		glVertex3f(70.0f, 80.0f, 0.0f);
		glVertex3f(100.0f, 80.0f, 0.0f);
		glVertex3f(100.0f, 60.0f, 0.0f);
		glVertex3f(70.0f, 60.0f, 0.0f);
		glEnd();
		glBegin(GL_POLYGON); //<---------------new nail
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(75.0f, 75.0f, 0.0);
		glVertex3f(80.0f, 75.0f, 0.0);
		glVertex3f(77.0f, 80.0f, 0.0);
		glVertex3f(80.0f, 65.0f, 0.0);
		glVertex3f(75.0f, 65.0f, 0.0);
		glVertex3f(95.0, 70.0f, 0.0);
		glEnd();
		glPopMatrix();
	}
}

void drawSceneBoundries() {//<------------ready
	glBegin(GL_POLYGON);//<-------bottom
	glColor3f(0.8f, 0.8f, 0.8f);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 10.0, 0.0);
	glVertex3f(1000.0, 10.0, 0.0);
	glVertex3f(1000.0, 0.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);//<--------top
	glColor3f(0.8f, 0.8f, 0.8f);
	glVertex3f(0.0, 600.0, 0.0);
	glVertex3f(1000.0, 600.0, 0.0);
	glVertex3f(1000.0, 580.0, 0.0);
	glVertex3f(0.0, 580.0, 0.0);
	glEnd();


	glBegin(GL_POLYGON);//<--------left
	glColor3f(0.8f, 0.8f, 0.8f);
	glVertex3f(0.0, 0.0, .0);
	glVertex3f(0.0, 600.0, 0.0);
	glVertex3f(10.0, 600.0, 0.0);
	glVertex3f(10.0, 0.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);//<----------Right
	glColor3f(0.8f, 0.8f, 0.8f);
	glVertex3f(1000.0, 0.0, 0.0);
	glVertex3f(990.0, 0.0, 0.0);
	glVertex3f(990.0, 600.0, 0.0);
	glVertex3f(1000.0, 600.0, 0.0);
	glEnd();

	glBegin(GL_LINE_LOOP);//<-------bottom border
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 10.0, 0.0);
	glVertex3f(1000.0, 10.0, 0.0);
	glVertex3f(1000.0, 0.0, 0.0);
	glEnd();

	glBegin(GL_LINE_LOOP);//<--------top border
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0, 600.0, 0.0);
	glVertex3f(1000.0, 600.0, 0.0);
	glVertex3f(1000.0, 580.0, 0.0);
	glVertex3f(0.0, 580.0, 0.0);
	glEnd();


	glBegin(GL_LINE_LOOP);//<--------left border
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0, 0.0, .0);
	glVertex3f(0.0, 600.0, 0.0);
	glVertex3f(10.0, 600.0, 0.0);
	glVertex3f(10.0, 0.0, 0.0);
	glEnd();

	glBegin(GL_LINE_LOOP);//<----------Right border
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1000.0, 0.0, 0.0);
	glVertex3f(990.0, 0.0, 0.0);
	glVertex3f(990.0, 600.0, 0.0);
	glVertex3f(1000.0, 600.0, 0.0);
	glEnd();


}

void drawHealthBar() {//<--------ready
	glBegin(GL_QUADS);//<----------------greenbar
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(10.0f, 580.0f, 0.0f);
	glVertex3f(300.0 - lifex, 580.0, 0.0f);
	glVertex3f(300.0 - lifex, 550.0, 0.0f);
	glVertex3f(10.0f, 550.0f, 0.0f);
	glEnd();
	glBegin(GL_LINE_LOOP);//<-----------border of the health bar
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(10.0f, 580.0f, 0.0f);
	glVertex3f(300.0 - lifex, 580.0f, 0.0f);
	glVertex3f(300.0 - lifex, 550.0f, 0.0f);
	glVertex3f(10.0f, 550.0f, 0.0f);
	glEnd();

}

void print(int x, int y, char* string)
{
	int len, i;
	glColor3f(1, 0, 0);

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

void walking(unsigned char key, int x, int y) {
	if ((key == 'w' || key == 'W' || key == GLUT_KEY_UP) && walkingverticle < 160) {
		walkingverticle += personSpeed;
	}
	else if ((key == 's' || key == 'S' || key == GLUT_KEY_DOWN) && walkingverticle > -330) {
		walkingverticle -= personSpeed;
	}
	else if ((key == 'a' || key == 'A' || key == GLUT_KEY_RIGHT) && walkinghorizontal > -430) {
		walkinghorizontal -= personSpeed;
		rotatePlayer(180);
	}
	else if ((key == 'd' || key == 'D' || key == GLUT_KEY_LEFT) && walkinghorizontal < 520) {
		walkinghorizontal += personSpeed;
		rotatePlayer(-180);
	}
	for (int i = 0; i < numcollectables; i++) {
		std::cout << i << std::endl;
		std::cout << collectablex[i] << std::endl;
		std::cout << collectabley[i] << std::endl;
		std::cout << "------------------" << std::endl;
	}

	for (int i = 0; i < numObstacles; i++) {
		collisioned(400 + walkinghorizontal, 300 + walkingverticle, obstacleX[i] - 10, obstacleY[i] - 10, 2.0, 2.0, "obstacle", "");
	}
	for (int i = 0; i < numcollectables; i++) {

		collisioned(400 + walkinghorizontal, 300 + walkingverticle, collectablex[i] - 85, collectabley[i] - 75, 60.0, 40.0, "collectable", "");
	}
	collisioned(400 + walkinghorizontal, 300 + walkingverticle, firstPx, firstPy, 5.5, 5.5, "powerup", "lightning");


	collisioned(400 + walkinghorizontal, 300 + walkingverticle, secondPx + 150, 130 + secondPy, 10.0, 20.0, "powerup", "time-freeze");
	collisioned(400 + walkinghorizontal, 300 + walkingverticle, 890, 10, 50.0, 50.0, "prize", "");
	glutPostRedisplay();
}

void healthUpdate(int xx) {
	if (lifex < 290) {
		lifex += xx;
	}

	glutTimerFunc(1000, healthUpdate, 0);
	glutPostRedisplay();

}

void randomPositions() {
	srand(time(0));

	firstPx = rand() % 800 +20;
	firstPy = rand() % 400 + 20;
	secondPx = rand() % (1000 - 200) + 20;
	secondPy = rand() % (600 - 200) +20;
	oldgoaly = -120;
	oldfirsty = firstPy;
	oldsecondy= secondPy;
	for (int i = 0; i < numObstacles; i++) {

		obstacleX[i] = rand() % (1000 - 200) + 100;
		obstacleY[i] = rand() % (600 - 200) + 100;
	}
	for (int i = 0; i < numcollectables; i++) {

		collectablex[i] = rand() % 800 + 20;
		collectabley[i] = rand() % 400 + 20;
		oldcollectabley[i] = collectabley[i];
	}
}

void updateRotation(int value) {
	rotationAngle += 5.0f;
	if (currenty == -120) {
		currenty == -140;
	}
	else {
		currenty = oldgoaly;
	}

	if (firstPy == oldfirsty) {
		firstPy == oldfirsty + 20;
	}
	else {
		firstPy = oldfirsty;
	}

	if (secondPy == oldsecondy) {
		secondPy == oldsecondy + 20;
	}
	else {
		secondPy = oldsecondy;
	}

	for (int i = 0; i < numcollectables; i++) {


		if (oldcollectabley[i] == collectabley[i]) {
			collectabley[i] == oldcollectabley[i] + 20;
		}
		else {
			collectabley[i] = oldcollectabley[i];
		}
	}

	glutTimerFunc(100, updateRotation, 0);
	glutPostRedisplay();
}

void rotatePlayer(int value) {
	rotateplayer += value;

	glutPostRedisplay();
}

void updateGameTime(int value) {
	if (gameTime > 0) {
		if (gamefreeze > 0) {
			gamefreeze--;
		}
		else {
			gameTime--;
		}
	}

	glutTimerFunc(1000, updateGameTime, 0);
	glutPostRedisplay();
}

void timer() {
	glPushMatrix();
	char gameTimeStr[20];
	sprintf(gameTimeStr, "Time: %d sec", gameTime);
	print(700, 550, gameTimeStr);
	glPopMatrix();
}

void drawbackground() {
	const double scaleFactor = 4;
	double scaledRadius = radius * scaleFactor;
	glPushMatrix();
	glRotated(rotationAngle, 0, 0, 1);
	glPushMatrix();
	glTranslated(500, 300, 0);
	glBegin(GL_POLYGON);  //(circle)
	for (double j = 0; j < 2 * PI; j += PI / 24) {
		glColor3f(1.0f, 0.0f, 1.0f);
		double scaledRadius = radius * scaleFactor;
		glVertex3f((cos(j) * scaledRadius), (sin(j) * scaledRadius), 0.0);
	}
	glEnd();
	glPopMatrix();
	glBegin(GL_LINES); 
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(515.0f, 300.0f, 0.0f); 
	glVertex3f(550.0f, 300.0f, 0.0f); 
	glEnd(); 
	glPopMatrix();
}

void collisioned(float playerX, float playerY, float objectX, float objectY, float objectWidth, float objectHeight, std::string nameOfObject, std::string nameOfPower) {

	float playerLeft = playerX;
	float playerRight = playerX + 55;
	float playerTop = playerY + 70;
	float playerBottom = playerY;
	float objectLeft = objectX;
	float objectRight = objectX + objectWidth;
	float objectTop = objectY + objectHeight;
	float objectBottom = objectY;


	if (playerRight < objectLeft || playerLeft > objectRight || playerTop < objectBottom || playerBottom > objectTop) {

	}
	else {
		if (nameOfObject == "powerup") {
			if (nameOfPower == "lightning") {//<--------remove it from the scene by translating it out of the border then giving the charachter speed for 10 sec 
				firstPx = 20000;
				firstPy = 20000;
				personSpeed = 30;
				speedo(1000);
			}
			else {//<--------remove it from the scene by translating it out of the border then giving the charachter more time like 10 sec 
				secondPx = 2000;
				secondPy = 2000;
				gamefreeze += 10;
			
			}
		}
		else {
			if (nameOfObject == "obstacle") {//<---------------reduces the health bar with 1 life and returning to the first position with the oiginal rotation
				healthUpdate(73);
				walkingverticle = 0;
				walkinghorizontal = 0;
				rotateplayer = 0;
			}
			else {//<---------It should be for collectables to add score then remove that obsticle from the screen by removing it of the list of x and y
				if (nameOfObject == "collectable") {
					int indexToRemove = -1;
					for (int i = 0; i < numcollectables; i++) {
						if (collectablex[i] <= objectLeft+80 && collectablex[i]>= objectLeft-80) {
							indexToRemove = i;
							break;
						}
					}
					if (indexToRemove != -1) {
						for (int i = indexToRemove; i < numcollectables; i++) {
							collectablex[i] = collectablex[i + 1];
						}

					}
					 indexToRemove = -1;
					for (int i = 0; i < numcollectables; i++) {
						if (collectabley[i] == objectBottom+75) {
							indexToRemove = i;
							break;
						}
					}
					if (indexToRemove != -1) {
						for (int i = indexToRemove; i < numcollectables; i++) {
							collectabley[i] = collectabley[i + 1];
						}
						numcollectables -= 1;
					}
					indexToRemove = -1;
					score += 10;
				}
				else
				{
					won = true;
				}
			}

		}

	}
}
void speedo(int speedtime) {
	if (speedtime > 0) {
		speedtime--;
	}
	else {
		personSpeed = 10;
	}
	glutTimerFunc(1000, speedo, 0);
	glutPostRedisplay();
}
bool f = true;
void Display() {
	glClear(GL_COLOR_BUFFER_BIT);
	if (won == false) {
		if (gameTime == 0||lifex>290) {
			glClearColor(0.0, 0.0, 0.0, 0.0);
			print(300, 400, "You lost! Better Luck Next Time  :( ");
		}
		else {
			drawbackground();
			drawSceneBoundries();
			drawHealthBar();
			char* p0s[20];
			sprintf((char*)p0s, "Score: %d", score);
			print(400, 550, (char*)p0s);
			timer();
			drawPlayer();
			drawObstacle();
			
			glPushMatrix();
			if (f == true) {

				f = false;
				glTranslated(0, 5, 0);
			}
			else {
				f = true;
				glTranslated(0, -5, 0);
			}
			drawPowerUp(firstPx, firstPy, secondPx, secondPy);
			drawPrize();
			drawCollectables();
			glPopMatrix();
			

	
		}
	}
	else {
		glClearColor(0.0,1.0,0.0,0.0);
		print(300, 400, "You won! Congratulations :) ");
	}
	glFlush();

}
void main(int argc, char** argr) {
	glutInit(&argc, argr);
	glutInitWindowSize(1000, 600);
	glutCreateWindow("The Chess Mode Is On");
	randomPositions();
	glutDisplayFunc(Display);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gluOrtho2D(0.0, 1000, 0.0, 600);
	glutTimerFunc(0, updateRotation, 0);
	glutKeyboardFunc(walking);
	glutTimerFunc(1000, updateGameTime, 0);
	glutMainLoop();
}