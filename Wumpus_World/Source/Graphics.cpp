#include"function.h"

void Objects::Drawobjs(int x, int y) {
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}
void Objects::draw() {
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}
void Agent::Drawobjs(int x, int y) {
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}

void PGraphics::initmap(Cell first_position, vector<vector<Cell>> matrix, int row, int col) {
	
	glPointSize(20);

	glColor3f(1, 1, 0.16);
	P = Agent(10 - first_position.x, first_position.y - 1);
	P.Drawobjs(first_position.y, first_position.x);
	glColor3f(1, 1, 1);


	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			if (matrix[10-i][j-1].value.find("G") != string::npos) {
				glColor3f(1, 0, 0);
				Objects gold(j,i);
				gold.draw();
				glColor3f(1, 1, 1);

			}
			else if (matrix[10 - i][j - 1].value.find("W") != string::npos) {
				glColor3f(0, 0, 1);
				Objects wumpus(j, i);
				wumpus.draw();
				glColor3f(1, 1, 1);
			}

		}
		//Sleep(300);
	}
}
void PGraphics::showresult(vector<Cell> return_path, int row) {
	int size = return_path.size() - 1;
	for (int i = size; i >= 0; i--) {
		glColor3f(0, 0, 0);
		P.Drawobjs(P.x, (row - 1) - P.y);

		glColor3f(1, 1, 0.16);
		if (return_path[i].value == "W") {
			glColor3f(0, 0, 1);
		}
		P.Drawobjs(return_path[i].x, (row - 1) - return_path[i].y);
		glColor3f(1, 1, 1);
		P.x = return_path[i].x;
		P.y = return_path[i].y;
		glFlush();
		Sleep(500);
	}
}

//void PGraphics::showresultLevel3(vector<Cell> rp, int row, vector<return_path> ghosts) {
//	int size = rp.size();
//	int count = 0;
//	vector<Objects> DrawGhost;
//	for (int i = 0; i < ghosts.size(); i++) {
//		DrawGhost.push_back(Objects(ghosts[i].path[0].x, ghosts[i].path[0].y));
//	}
//	for (int i = 0; i < size; i++) {
//
//		if (rp[i].value == 3) {
//			glColor3f(0, 0, 0);
//			DrawGhost[count].Drawobjs(DrawGhost[count].x, (row - 1) - DrawGhost[count].y);
//			glColor3f(0, 0, 1);
//			DrawGhost[count].x = rp[i].x;
//			DrawGhost[count].y = rp[i].y;
//			DrawGhost[count].Drawobjs(DrawGhost[count].x, (row - 1) - DrawGhost[count].y);
//			count++;
//			if (count == ghosts.size()) {
//				count = 0;
//				Sleep(300);
//			}
//		}
//		else if (rp[i].value == 0 || rp[i].value == 2) {
//			glColor3f(0, 0, 0);
//			P.Drawobjs(P.x, (row - 1) - P.y);
//			glColor3f(1, 1, 0.16);
//			P.Drawobjs(rp[i].x, (row - 1) - rp[i].y);
//			P.x = rp[i].x;
//			P.y = rp[i].y;
//			Sleep(300);
//
//		}
//		glColor3f(1, 1, 1);
//		glFlush();
//	}
//
//}

