#include"function.h"

const int MAX_ROOMS = 50;


vector<Cell> LogicalSearch::search(int &s, bool &is_dead) {
	//initialize agent map
	for (int i = 0; i < 11; i++) {
		vector<Cell> temp;
		for (int j = 0; j < 11; j++) {
			temp.push_back(Cell(i, j, "-"));
		}
		agent_map.push_back(temp);
	}

	vector<Cell> return_path;

	

	agent_map[current_position.x][current_position.y] = world_map[10-current_position.x][current_position.y-1];
	/*cout << "World at : " << world_map[current_position.x][current_position.y].x <<  " , " <<
		world_map[current_position.x][current_position.y].y << world_map[current_position.x][current_position.y].value
		<< endl;*/
	start = agent_map[current_position.x][current_position.y];
	//Cell debug1 = agent_map[current_position.x][current_position.y];


	agent_map[current_position.x][current_position.y].ok = true;

	agent_map[current_position.x][current_position.y].exist_frontier = true;
	world_map[10 - current_position.x][current_position.y - 1].exist_frontier = true;
	current_position.exist_frontier = true;



	frontier.push(current_position);

	vector<Cell> prev;
	prev.push_back(Cell(current_position.x, current_position.y, "-"));
	table.push(backtrack_tuple(Cell(current_position.x, current_position.y, "-"), prev));

	//previous = Cell(1, 1, "A");
	g.initmap(current_position, world_map, 10, 10);

	while (frontier.size() > 0) {
		vector<int> possible_moves; //1: right, 2: top, 3: left, 4: bottom

		for (auto it : possible_moves) {
			cout << "Possible move: " << it << " ";
		}

		if (check_empty(frontier))
			return return_path; //failure so check if empty then failure

		Cell temp = frontier.pop();
		cout << endl;
		cout << "//////////////////////////////////////////////// BEGIN ////////////////////////////////////////////////" << endl;
		/*cout << "Frontier: "; frontier.display();*/
		cout << "Expand at: (" << temp.x << "," << temp.y << ") = " <<temp.value<< endl;

		if (temp.value.find("G") != string::npos)
			pick_up_gold(temp);

		if (numofmoves != 0) {
			update_position(temp);
			glClear(GL_COLOR_BUFFER_BIT);
			g.initmap(temp, world_map, 10, 10);
			display();
		}

		previous = temp;

		

		numofmoves++;

		agent_map[temp.x][temp.y].exist_frontier = false;
		world_map[10-temp.x][temp.y-1].exist_frontier = false;
		temp.exist_frontier = false;

		agent_map[temp.x][temp.y].visited = true;
		world_map[10-temp.x][temp.y-1].visited = true;
		temp.visited = true;

		explored.push_back(temp);

		choose_possible_moves(temp, possible_moves);

		for (auto it : possible_moves) {
			Cell child_temp = action(temp, world_map, it);
			agent_map[child_temp.x][child_temp.y] = world_map[10 - child_temp.x][child_temp.y - 1];

			//Cell debug2 = agent_map[child_temp.x][child_temp.y];

			//Cell child = agent_map[child_temp.x][child_temp.y];
			Cell child = world_map[10-child_temp.x][child_temp.y-1];


			//reasoning
			bool check_go_next = false;
			

			//update_position(child);
			//display(); //show update on going to child
			
 			reasoning(child, check_go_next, score);

			//previous = world_map[10-child.x][child.y-1];


			if (!check_exists_frontier_or_explored(child)&& check_go_next) {
				if (numofmoves >= MAX_ROOMS/2 || child.value.find("W") != string ::npos) {
					vector<Cell> previous;
					previous.push_back(Cell(temp.x, temp.y, "-"));
					table.push(backtrack_tuple(Cell(child.x, child.y, "-"), previous));
					//table.display();
					cout << endl;
					cout << "//////////////////////////////////////////////// END ////////////////////////////////////////////////" << endl << endl;
					
					if (child.value.find("W") != string::npos) {
						s = score - 10000;
						is_dead = true;
					}
					else
						s = score;

					return backtrack(agent_map[1][1], Cell(child.x, child.y, "-")); // child == goal
				}
				agent_map[child.x][child.y].exist_frontier = true;
				world_map[10-child.x][child.y-1].exist_frontier = true;

				frontier.push(child);
				//cout << "Push frontier: (" << child.x << "," << child.y << ")" << " " << endl;
				vector<Cell> previous;
				previous.push_back(Cell(temp.x, temp.y, "-"));
				table.push(backtrack_tuple(Cell(child.x, child.y, "-"), previous));
				//table.display();
			}
			else {
				vector<Cell> previous;
				previous.push_back(Cell(temp.x, temp.y, "-"));
				table.push(backtrack_tuple(Cell(child.x, child.y, "-"), previous));
				//table.display();
			}
		}
		cout << "//////////////////////////////////////////////// END ////////////////////////////////////////////////" << endl << endl;
	}

	cout << "Score: " << score << endl;

	return return_path;
}

void LogicalSearch::choose_possible_moves(Cell temp, vector<int> &possible_moves) {
	if (temp.x + 1 <= 10) {
		possible_moves.push_back(2);
	}

	if (temp.x - 1 >= 1) {
		possible_moves.push_back(4);
	}

	if (temp.y + 1 <= 10) {
		possible_moves.push_back(1);
	}

	if (temp.y - 1 >= 1) {
		possible_moves.push_back(3);
	}
}
