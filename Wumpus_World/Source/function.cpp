#include"function.h"

void read_file(vector<vector<Cell>> &world_map, Cell &start_position) {
	ifstream fin;

	fin.open("map1.txt");

	if (!fin.is_open()) {
		cout << "Can't open file to read" << endl;
		return;
	}

	string input, output;

	while (!fin.eof()) {
		for (int i = 0; i < 10; i++) {
			getline(fin, input);
			stringstream s(input);
			vector<Cell> temp;
			int j = 0;
			while (getline(s, output, '.')) {
				Cell c = Cell(10 - i, j + 1, output);
				if (output == "B") {
					c.breeze = true;
				}
				else if (output == "S") {
					c.stench = true;
				}
				else if (output == "BS" || output == "SB") {
					c.breeze = true;
					c.stench = true;
				}
				else if (output == "SG" || output == "GS") {
					c.stench = true;
					c.gold = true;
				}
				else if (output == "GB" || output == "BG") {
					c.gold = true;
					c.breeze = true;
				}
				else if (output == "BSG" ) {
					c.breeze = true;
					c.stench = true;
					c.gold = true;
				}
				else if (output == "P") {
					c.pit = true;
				}
				else if (output == "G") {
					c.gold = true;
				}
				else if (output == "W") {
					c.wumpus = true;
				}

				temp.push_back(c);

				if (output == "A")
					start_position = c;
				
				j++;
			}
			world_map.push_back(temp);
		}
	}

	fin.close();
}

void display_file(vector<vector<Cell>> world_map, Cell start_position) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			cout << world_map[i][j].value << " ";
		}
		cout << endl;
	}

	cout << "Agent start at: (" << start_position.x << "," << start_position.y << ")" << endl << endl;
}

int once = 0;

void SetupRC(int row, int column) {
	glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(-(column - 1), (column - 1) * 2, -(row - 1), ((row - 1) * 2));
	if (column >= row)
		gluOrtho2D(-1, column, -1, column);
	else gluOrtho2D(-1, row, -1, row);
}

void graphics_display() {
	if (once == 0) {
		vector<vector<Cell>> world_map;
		Cell start_position;
		Cell goal;

		//init(world_map);
		read_file(world_map, start_position);
		display_file(world_map, start_position);
		LogicalSearch ls(start_position, world_map);

		int score = 0;
		bool is_dead = false;
		vector<Cell> return_path = ls.search(score, is_dead);
	
		display_final_return_path(return_path, world_map, score, is_dead);
		
		cout << endl << endl << endl;
		once++;
	}
	else return;
}
void return_to_exit_path(vector<Cell> &return_path, vector<vector<Cell>> world_map) {
	Cell start = return_path[0], end = return_path.back();
	Cell temp = start;

	cout << endl << endl;
	cout << "Agent is coming back to start!!!" << endl;

	while (temp.x > 1) {
		temp = world_map[10 - temp.x + 1][temp.y - 1];
		cout << "Back to cell (" << temp.x << "," << temp.y << ")" << endl;
	}

	while (temp.y > 1) {
		temp = world_map[10 - temp.x][temp.y - 1 - 1];
		cout << "Back to cell (" << temp.x << "," << temp.y << ")" << endl;
	}
}

void display_final_return_path(vector<Cell> return_path, vector<vector<Cell>> world_map, int score, bool is_dead) {

	cout << endl << endl;
	cout << "***************************************** Return path *****************************************" << endl << endl;

	if (return_path.size() == 0) {
		cout << "Agent is afraid of moving!!!" << endl << endl;
		cout << "***************************************** Total Score : " << score << " *****************************************" << endl << endl;
		return;
	}

	cout << "Agent go to (" << return_path[0].x << "," << return_path[0].y << ")" << endl;
	for (int i = return_path.size() - 1; i >= 0; i--)
		cout << "(" << return_path[i].x << "," << return_path[i].y << ")" << " ";
	cout << endl;

	if (!is_dead)
		return_to_exit_path(return_path, world_map);
	else 
		cout << "Agent is eaten by a wumpus at (" << return_path[0].x << "," << return_path[0].y << ")" << endl;

	cout << endl;
	cout << "***************************************** Total Score : " << score << " *****************************************" << endl << endl;

}

