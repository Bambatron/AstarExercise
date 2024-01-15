#pragma once

#include <queue>
#include <unordered_map>
#include <vector>

template<typename Graph, typename Location>
void BreadthFirstSearch(Graph& graph, Location& start) {
	std::queue<Location> frontier;
	frontier.push(start);

	std::set<Location> reached;
	reached.insert(start);

	while (!frontier.empty()) {	//Continue the search until all locations are visited
		// Pop the current location from the front of the queue
		Location current = frontier.front();
		frontier.pop();

		current.Read();	 // Visualize the current location

		// For each neighbor if it has not been reached before
		for (Location next : graph.Neighbors(current)) {	
			if (reached.find(next) == reached.end()) {
				// Add the neighbor to the queue and mark it as reached
				frontier.push(next);
				reached.insert(next);
			}
		}
	}
}

// Start of the BFS recursive
template<typename Graph, typename Location>
void BreadthFirstSearchRecursive(Graph& graph, Location& start) {
	std::queue<Location> frontier;
	frontier.push(start);

	std::set<Location> reached;
	reached.insert(start);

	std::cout << "Starting BreadthFirstSearchRecursive" << std::endl;

	RecursiveBFS(graph, frontier, reached);
}

// Perform BFS recursively on the graph
template<typename Graph, typename Location>
void RecursiveBFS(Graph& graph, std::queue<Location>& frontier, std::set<Location>& reached) {
	
	if (frontier.empty()) {
		return;
	}

	// Dequeue front node and print it
	Location current = frontier.front();
	frontier.pop();
	
	current.Read();

	// do for every edge (v, u)
	for (Location next : graph.Neighbors(current)) {
		if (reached.find(next) == reached.end()) { // Mark neighbor as reached and enqueue it
			frontier.push(next);
			reached.insert(next);
		}
	}

	RecursiveBFS(graph, frontier, reached);
}

// Start of the BFS recursive with visual rappresentation
template<typename Map, typename Location>
void BreadthFirstSearchRecursiveVisual(Map& map, Location& start, sf::RenderWindow& target, sf::Time frameRate) {
	std::queue<Location> frontier;
	frontier.push(start);

	std::set<Location> reached;
	reached.insert(start);

	std::cout << "Starting BreadthFirstSearchRecursive" << std::endl;

	sf::Clock clock;
	sf::Time elapsedTime = sf::Time::Zero;

	int rad = map.GetTile().Radius() / 5;
	sf::CircleShape circle(rad);
	circle.setOrigin(rad, rad);
	circle.setOutlineColor(sf::Color::Black);
	circle.setOutlineThickness(1);		
		
	while (RecursiveBFSVisual(map.GetGrid(), frontier, reached)){
		//Render
		target.clear();
		
		map.Render(target);

		for (auto it : reached) {
			circle.setPosition(map.HexToPixel(it));
			circle.setFillColor(sf::Color::Red);
			target.draw(circle);
		}

		// Create a temporary copy of the queue
		std::queue<Location> tempFrontier = frontier;
		// Visit elements in the temporary copy
		while (!tempFrontier.empty()) {
			circle.setPosition(map.HexToPixel(tempFrontier.front()));
			circle.setFillColor(sf::Color::Blue);
			target.draw(circle);
			tempFrontier.pop();
		}
		
		target.display();

		elapsedTime += clock.restart();
		while (elapsedTime < frameRate) {
			elapsedTime += clock.restart();
		}
		elapsedTime = sf::Time::Zero;
		clock.restart();
	}
}

template<typename Graph, typename Location>
bool RecursiveBFSVisual(Graph& graph, std::queue<Location>& frontier, std::set<Location>& reached) {

	if (frontier.empty()) {
		return false;
	}

	// Dequeue front node and print it
	Location current = frontier.front();
	frontier.pop();

	current.Read();

	// do for every edge (v, u)
	for (Location next : graph.Neighbors(current)) {
		if (reached.find(next) == reached.end()) { // Mark neighbor as reached and enqueue it
			frontier.push(next);
			reached.insert(next);
		}
	}
	return true;
	//RecursiveBFS(graph, frontier, reached);
}


template<typename Graph, typename Location>
std::unordered_map<Location, Location> BreadthFirstSearchToGoal(Graph& graph, Location& start, Location& goal) {
	std::queue<Location> frontier;
	frontier.push(start);

	std::unordered_map<Location, Location> cameFrom;
	cameFrom[start] = start;

	while (!frontier.empty()) {
		Location current = frontier.front();
		frontier.pop();

		if (current == goal) {
			break;
		}

		for (Location next : graph.Neighbors(current)) {
			if (cameFrom.find(next) == cameFrom.end()) {
				frontier.push(next);
				cameFrom[next] = current;
			}
		}
	}

	return cameFrom;
}

template<typename Graph, typename Location>
void AstarSearch(Graph graph, Location start, Location goal, std::unordered_map<Location, Location>& cameFrom, std::unordered_map<Location, int>& costSoFar) {
	PriorityQueue<Location, int> frontier;
	frontier.put(start, 0);

	cameFrom[start] = start;
	costSoFar[start] = 0;

	while (!frontier.isEmpty()) {
		Location current = frontier.get();

		if (current == goal) {
			break;
		}

		for (Location next : graph.Neighbors(current)) {
			int newCost = costSoFar[current] + graph.Cost(current, next);
			if (costSoFar.find(next) == costSoFar.end() ||
				newCost < costSoFar[next]) {
				costSoFar[next] = newCost;
				int priority = newCost + Heuristic(next, goal);
				frontier.put(next, priority);
				cameFrom[next] = current;
			}
		}
	}
}

template<typename Location>
std::vector<Location> ReconstructPath(const std::unordered_map<Location, Location>& path, Location& start, Location& goal) {
	std::vector<Location> reconstructedPath;

	Location current = goal;

	//Walk the path backwards 
	while (current != start) {
		reconstructedPath.push_back(current);
		current = path.at(current);
	}

	//Add the starting square
	reconstructedPath.push_back(start);

	//Reverse the path so that it goes from start to goal
	std::reverse(reconstructedPath.begin(), reconstructedPath.end());

	return reconstructedPath;
}
