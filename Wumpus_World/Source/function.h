#include<fstream>
#include<istream>
#include<string.h>
#include<string>
#include <sstream>
#include<vector>
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include<iterator>
#include<algorithm>
#include<Windows.h>
#include"windows.h"
#include<iostream>

using namespace std;

#define INT MAX_SIZE = 10;

struct Objects {

	int x, y;
	Objects() = default;
	Objects(int x, int y) {
		this->x = x;
		this->y = y;
	};
	void Drawobjs(int x, int y);
	void draw();
};
struct Agent :Objects {
public:
	Agent() = default;
	Agent(int x, int y) : Objects(x, y) {

	};
	void Drawobjs(int x, int y);
};


struct Cell {
	int x, y;
	string value;
	bool exist_frontier, visited;
	bool breeze, stench, scream, glitter;
	bool potential_pit, potential_wumpus;
	int count_potential_pit;
	bool pit, wumpus, gold;
	bool ok;
	Cell(int x, int y, string value) {
		this->x = x;
		this->y = y;
		this->value = value;
		exist_frontier = false;
		visited = false;
		this->ok = true;
		this->breeze = false; 
		this->stench = false;
		this->scream = false;
		this->glitter = false;
		this->pit = false;
		this->wumpus = false;
		this->gold = false;

		this->count_potential_pit = 0;
		this->potential_pit = false;
		this->potential_wumpus = false;
	};
	Cell() { //default constructor

	};
	Cell &operator=(const Cell &c) {
		if (this == &c)
			return *this;
		this->x = c.x;
		this->y = c.y;
		this->value = c.value;
		this->ok = c.ok;
		this->breeze = c.breeze;
		this->stench = c.stench;
		this->scream = c.scream;
		this->glitter = c.glitter;
		this->pit = c.pit;
		this->wumpus = c.wumpus;
		this->gold = c.gold;
		this->count_potential_pit = c.count_potential_pit;

		this->potential_pit = c.potential_pit;
		this->potential_wumpus = c.potential_wumpus;

		this->exist_frontier = false;
		this->visited = false;

		return *this;
	}
	bool operator==(const Cell &c) {
		if (this->value == c.value && this->x == c.x && this->y == c.y)
			return true;
		return false;
	};

	bool check_equal_coord(Cell c) {
		if (this->x == c.x && this->y == c.y)
			return true;
		return false;
	}
};

struct Queue {
	void push(Cell c); //enqueue
	Cell pop(); //dequeue
	void display();
	bool empty();
	int size();
	Cell at(int);
	vector<Cell> data;
	void swap(Cell *a, Cell *b);

	/*void quickSort(int low, int high);
	int partition(int low, int high);*/
};

struct backtrack_tuple {
	Cell key;
	vector<Cell> previous;
	backtrack_tuple(Cell key, vector<Cell> previous) {
		this->key = key;
		this->previous = previous;
	};

	void display() {
		cout << "Cell (" << key.x << "," << key.y << "," << key.value << ") | ";
		for (auto it : previous) {
			cout << "Cell (" << it.x << "," << it.y << "," << it.value << ") ";
		}
		cout << endl;
	};

	void pop() {
		previous.pop_back();
	}

	Cell end_element() {
		return previous.back();
	}
};

struct backtrack_table {
	vector<backtrack_tuple> tuples;
	Cell search_key(Cell search) {
		for (int i = 0; i < tuples.size(); i++) {
			if (search == tuples[i].key) {
				if (!tuples[i].previous.empty()) {
					Cell result = tuples[i].end_element();
					if (tuples[i].previous.size() != 1)
						tuples[i].pop();
					return result;
				}
			}
		}
	};
	void push(backtrack_tuple t) {
		//tuples.push_back(t);
		if (tuples.size() == 0) {
			tuples.push_back(t);
			return;
		}
		for (int i = 0; i < tuples.size(); i++) {
			if (tuples[i].key == t.key) {
				Cell c = t.previous.back();
				tuples[i].previous.push_back(c);
			/*	cout << "Tuples: " << i << endl;
				tuples[i].display();*/
				return;
			}
		}

		tuples.push_back(t);
		return;

	};
	void display() {
		cout << "BACKTRACK table: " << endl;
		/*for (auto it : tuples) {
			cout << "Cell (" << it.key.x << "," << it.key.y << "," << it.key.value << ") | "
				<< "Cell (" << it.previous.x << "," << it.previous.y << "," << it.previous.value << ")" << endl;
		}*/
		for (auto it : tuples) {
			it.display();
		}
	}
};

class PGraphics {
public:
	Agent P;
	void initmap(Cell first_position, vector<vector<Cell> > matrix, int row, int col);
	void showresult(vector<Cell> return_path, int row);
	//void showresultLevel3(vector<Cell> rp, int row, vector<return_path> ghosts);
};




class LogicalSearch {
public:
	Cell current_position;
	Cell previous;
	backtrack_table table;
	int score = 0;
	Queue frontier; //self implement queue
	vector<Cell> explored;
	vector<vector<Cell>> world_map;
	vector<vector<Cell>> agent_map;
	Cell start;
	PGraphics g;
	
	int numofmoves = 0;

	bool isAlive, isInCave;

	int numActions;

	LogicalSearch(Cell start_position, vector<vector<Cell>> world_map) {
		this->current_position = start_position;
		this->world_map = world_map;
	};

	bool game_over() {
		if (!isAlive || !isInCave)
			return true;
		return false;
	};
	bool check_empty(Queue q) {
		if (q.empty())
			return true;
		return false;
	};

	bool check_exists_frontier_or_explored(Cell child) {
		if (child.visited || child.exist_frontier)
			return true;

		return false;
		/*for (int i = 0; i < frontier.size();i++) {
			if (frontier.at(i).check_equal_coord(child))
				return true;
		}

		for (auto it : explored) {
			if (it.check_equal_coord(child))
				return true;
		}
*/
		return false;
	};
	Cell action(Cell temp, vector<vector<Cell>> matrix, int possible_moves) {
		if (possible_moves == 2) {
			return matrix[10-temp.x - 1][temp.y-1];
		}
		else if (possible_moves == 1) {
			return matrix[10-temp.x][temp.y + 1 - 1];
		}
		else if (possible_moves == 4) {
			return  matrix[10-temp.x + 1][temp.y - 1];
		}
		else if (possible_moves == 3) {
			return matrix[10-temp.x][temp.y - 1 - 1];
		}
	};

	vector<Cell> backtrack(Cell start, Cell goal) {
		vector<Cell> return_path;
		return_path.push_back(goal);
		Cell temp = table.search_key(goal);

		while (!(temp == start)) {
			return_path.push_back(temp);
			temp = table.search_key(temp);
		}

		return_path.push_back(start);

		return return_path;
	};

	void pick_up_gold(Cell &c) {
		string val = c.value;
		val.erase(remove(val.begin(), val.end(), 'G'), val.end());
		if (val == "") val = "-";
		agent_map[c.x][c.y].value = val;
		world_map[10 - c.x][c.y - 1].value = val;
		c.value = val;
		score += 100;
		cout << "Score after pick gold: " << score << endl;
	};

	void reasoning(Cell &c, bool &check_go_next, int &score) {
		
		Cell left, right, up, down;

		if (c.y - 1 < 1)
			left = Cell(0, 0, "");
		else
			left = agent_map[c.x][c.y - 1];
		
		if (c.y + 1 > 10)
			right = Cell(0, 0, "");
		else
			right = agent_map[c.x][c.y + 1];

		if (c.x - 1 < 1)
			down = Cell(0, 0, "");
		else
			down = agent_map[c.x - 1][c.y];

		if (c.x + 1 > 10)
			up = Cell(0, 0, "");
		else
			up = agent_map[c.x + 1][c.y];

		left.ok = false;
		right.ok = false;
		up.ok = false;
		down.ok = false;

		if (c.value.find("G") != string::npos) { //find G in KB, push in frontier for later pick up
			/*string val = c.value;
			agent_map[c.x][c.y].value = val;*/

			check_go_next = true;
			//push_to_frontier(c,c);
			/*agent_map[c.x][c.y].gold = true;
			agent_map[c.x][c.y].ok = true;
			check_go_next = true;
			score += 100;*/
		}

		if (c.value.find("B") != string::npos) {

			if (left.x != 0&& left.value != "A") {
				left.potential_pit = true;
				agent_map[left.x][left.y].potential_pit = true;
				agent_map[left.x][left.y].count_potential_pit++;
				if (check_contradiction(left) && !c.gold) {
					left.ok = true;
					agent_map[left.x][left.y].ok = true;
					//c = agent_map[left.x][left.y];
					//check_go_next = true;
				}

				if (agent_map[left.x][left.y].count_potential_pit == 2) {
					agent_map[left.x][left.y].pit = true;
				}
				else if (agent_map[right.x][right.y].value.find("A") != string::npos) {
					agent_map[right.x][right.y].pit = true;
				}
				/*else {
					push_to_frontier(left, c);
				}*/
			}
			
			if (right.x != 0 && right.value != "A") {
				right.potential_pit = true;
				agent_map[right.x][right.y].potential_pit = true;
				if (check_contradiction(right) && !c.gold) {
					right.ok = true;
					agent_map[right.x][right.y].ok = true;
					//c = agent_map[right.x][right.y];
					//check_go_next = true;
				}

				if (agent_map[right.x][right.y].count_potential_pit == 2) {
					agent_map[right.x][right.y].pit = true;
				}
				else if (agent_map[left.x][left.y].value.find("A") != string::npos) {
					agent_map[right.x][right.y].pit = true;
				}
				/*else
					push_to_frontier(right, c);*/
			}

			if (up.x != 0  && up.value != "A") {
				up.potential_pit = true;
				agent_map[up.x][up.y].potential_pit = true;
				if (check_contradiction(up) && !c.gold) {
					up.ok = true;
					agent_map[up.x][up.y].ok = true;
					//c = agent_map[up.x][up.y];
					//check_go_next = true;
				}

				if (agent_map[up.x][up.y].count_potential_pit == 2) {
					agent_map[up.x][up.y].pit = true;
				}
				else if (agent_map[up.x][up.y].value.find("A") != string::npos) {
					agent_map[down.x][down.y].pit = true;
				}
				/*else
					push_to_frontier(up, c);*/
			}

			if (down.x != 0 && down.value != "A") {
				down.potential_pit = true;
				agent_map[down.x][down.y].potential_pit = true;
				if (check_contradiction(down) && !c.gold) {
					down.ok = true;
					agent_map[down.x][down.y].ok = true;
					//c = agent_map[down.x][down.y];
					//check_go_next = true;

				}
				if (agent_map[down.x][down.y].count_potential_pit == 2) {
					agent_map[down.x][down.y].pit = true;
				}
				else if (agent_map[down.x][down.y].value.find("A") != string::npos) {
					agent_map[up.x][up.y].pit = true;
				}
				/*else
					push_to_frontier(down, c);*/
			}
		}

		if (c.value.find("S") != string::npos) {

			if (up.x != 0 && up.value != "A") {
				up.potential_wumpus = true;
				agent_map[up.x][up.y].potential_wumpus = true;
				if (check_contradiction(up) && !c.gold) {
					up.ok = true;
					agent_map[up.x][up.y].ok = true;
					//check_go_next = true;
					//c = agent_map[up.x][up.y];
				}

				//if (up.value == "W") {
				kill_wumpus(up);
				score -= 1000;
				cout << "Score after shoot an arrow at (" << up.x << "," << up.y << "): "<< score << endl;

				//push_to_frontier(up, c);
				//}*/
				
			}

			if (down.x != 0 && down.value != "A") {
				down.potential_wumpus = true;
				agent_map[down.x][down.y].potential_wumpus = true;
				if (check_contradiction(down) && !c.gold) {
					down.ok = true;
					agent_map[down.x][down.y].ok = true;
					//check_go_next = true;
					//c = agent_map[down.x][down.y];
				}

				//if (down.value == "W") {
				kill_wumpus(down);
				score -= 1000;
				cout << "Score after shoot an arrow at (" << down.x << "," << down.y << "): " << score << endl;

				//push_to_frontier(down, c);
				//}*/
			}

			if (right.x != 0 && right.value != "A") {
				right.potential_wumpus = true;
				agent_map[right.x][right.y].potential_wumpus = true;
				if (check_contradiction(right) && !c.gold) {
					right.ok = true;
					agent_map[right.x][right.y].ok = true;
					//c = agent_map[right.x][right.y];
				}

				/*if (right.value == "W") {*/
				kill_wumpus(right);
				score -= 1000;
				cout << "Score after shoot an arrow at (" << right.x << "," << right.y << "): " << score << endl;

				//push_to_frontier(right, c);
				//}*/
			}

			if (left.x != 0 && left.value != "A") {
				left.potential_wumpus = true;
				agent_map[left.x][left.y].potential_wumpus = true;
				if (check_contradiction(left) && !c.gold) {
					left.ok = true;
					agent_map[left.x][left.y].ok = true;
					//c = agent_map[left.x][left.y];
				}

				//if (left.value == "W") {
				kill_wumpus(left);
				score -= 1000;
				cout << "Score after shoot an arrow at (" << left.x << "," << left.y << "): " << score << endl;
				//push_to_frontier(left, c);
				//}
			}
			/*c.stench = false;
			c.ok = true;*/ //co ve co roi, trong kill wumpus
			string val = world_map[10 - c.x][c.y - 1].value;
			val.erase(remove(val.begin(), val.end(), 'S'), val.end());
			if (val == "") val = "-";
			c.value = val;

			check_go_next = true;
		}
		
		if (c.value == "-" || c.value == "W")
			check_go_next = true;


	};

	void display() {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				cout << world_map[i][j].value << " ";
			}
			cout << endl;
		}
	};

	void push_to_frontier(Cell c, Cell p) {
		//if (!check_exists_frontier_or_explored(c)) {
		//	//if (child.gold) {
		//	//	table.push(backtrack_tuple(Cell(child), Cell(temp)));
		//	//	//table.display();
		//	//	return backtrack(agent_map[1][1], child); // child == goal
		//	//}
		//	agent_map[c.x][c.y].exist_frontier = true;
		//	frontier.push(c);
		//	cout << "Push frontier: (" << c.x << "," << c.y << ")" << " " << endl;

		//	table.push(backtrack_tuple(Cell(c), Cell(p)));
		//}
	};

	void update_position(Cell &child) {
		if (child.value == "-") {
			child.value = "A";
			world_map[10 - child.x][child.y - 1].value = "A";
		}
		else {
			child.value += "A";
			world_map[10 - child.x][child.y - 1].value += "A";
		}

		string val = previous.value;
		val.erase(remove(val.begin(), val.end(), 'A'), val.end());
		if (val == "") val = "-";

		previous.value = val;
		world_map[10 - previous.x][previous.y - 1].value = val;
	};

	void kill_wumpus(Cell &c) {
		Cell left, right, up, down;

		agent_map[c.x][c.y].ok = true;
		string val = world_map[10-c.x][c.y-1].value;
		val.erase(remove(val.begin(), val.end(), 'W'), val.end());
		if (val == "") val = "-";
		c.value = val;
		agent_map[c.x][c.y].value = val;
		world_map[10 - c.x][c.y - 1].value = val;
		agent_map[c.x][c.y].ok = true;
		agent_map[c.x][c.y].wumpus = false;
		world_map[10-c.x][c.y-1].ok = true;
		world_map[10-c.x][c.y-1].wumpus = false;

		if (c.y - 1 < 1)
			left = Cell(0, 0, "");
		else
			left = world_map[10-c.x][c.y - 1-1];

		if (c.y + 1 > 10)
			right = Cell(0, 0, "");
		else
			right = world_map[10-c.x][c.y + 1-1];

		if (c.x - 1 < 1)
			down = Cell(0, 0, "");
		else
			down = world_map[10-c.x + 1][c.y-1];

		if (c.x + 1 > 10)
			up = Cell(0, 0, "");
		else
			up = world_map[10-c.x - 1][c.y-1];

		if (left.x != 0) {
			string val = left.value;
			val.erase(remove(val.begin(), val.end(), 'S'), val.end());
			if (val == "") val = "-";

			agent_map[left.x][left.y] = Cell(left.x, left.y, val);
			world_map[10-left.x][left.y-1] = Cell(left.x, left.y, val);

			agent_map[left.x][left.y].ok = true;
			agent_map[left.x][left.y].stench = false;
			world_map[10-left.x][left.y-1].ok = true;
			world_map[10-left.x][left.y-1].stench = false;
		}
		if (right.x != 0) {
			string val = right.value;
			val.erase(remove(val.begin(), val.end(), 'S'), val.end());
			if (val == "") val = "-";

			agent_map[right.x][right.y] = Cell(right.x, right.y, val);
			world_map[10-right.x][right.y-1] = Cell(right.x, right.y, val);

			agent_map[right.x][right.y].ok = true;
			agent_map[right.x][right.y].stench = false;
			world_map[10 - right.x][right.y - 1].ok = true;
			world_map[10 - right.x][right.y - 1].stench = false;
		}
		if (up.x != 0) {
			string val = up.value;
			val.erase(remove(val.begin(), val.end(), 'S'), val.end());
			if (val == "") val = "-";

			agent_map[up.x][up.y] = Cell(up.x, up.y, val);
			world_map[10 - up.x][up.y - 1] = Cell(up.x, up.y, val);

			agent_map[up.x][up.y].ok = true;
			agent_map[up.x][up.y].stench = false;
			world_map[10 - up.x][up.y - 1].ok = true;
			world_map[10 - up.x][up.y - 1].stench = false;
		}
		if (down.x != 0) {
			string val = down.value;
			val.erase(remove(val.begin(), val.end(), 'S'), val.end());
			if (val == "") val = "-";

			agent_map[down.x][down.y] = Cell(down.x, down.y, val);
			world_map[10 - down.x][down.y - 1] = Cell(down.x, down.y, val);

			agent_map[down.x][down.y].ok = true;
			agent_map[down.x][down.y].stench = false;
			world_map[10-down.x][down.y - 1].ok = true;
			world_map[10 - down.x][down.y - 1].stench = false;
		}
	};

	bool check_contradiction(Cell c) {
		if (c.potential_pit && c.potential_wumpus)
			return true;
		return false;
	};
	vector<Cell> search(int&s, bool &is_dead);
	void choose_possible_moves(Cell temp, vector<int> &possible_moves);
};


void read_file(vector<vector<Cell>> &world_map, Cell &start_position);
void display_file(vector<vector<Cell>> world_map, Cell start_position);
void SetupRC(int row, int column);
void graphics_display();
void display_final_return_path(vector<Cell> return_path, vector<vector<Cell>> world_map, int score, bool is_dead);
void return_to_exit_path(vector<Cell> &return_path, vector<vector<Cell>> world_map);