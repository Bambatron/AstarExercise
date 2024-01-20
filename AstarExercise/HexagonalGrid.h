#pragma once

#include <iostream>
#include <sstream>
#include <functional> 
#include <cassert>

#include <unordered_map>
#include <unordered_set>
#include <array>
#include <queue>
#include <set>

struct Hex {
	int q, r, s;

	Hex(int _q, int _r, int _s) : q(_q), r(_r), s(_s) {	//Cube constructor
		assert(q + r + s == 0);
	}
	
	Hex(int _q = 0	, int _r = 0) : Hex(_q, _r, -_q-_r){}

	Hex(const Hex& original) : q(original.q), r(original.r), s(original.s) {
		assert(q + r + s == 0);
	}

	bool operator==(const Hex& other) const {
		return (this->q == other.q &&
				this->r == other.r &&
				this->s == other.s);
	}

	bool operator!=(const Hex& other) {
		return !(*this == other);
	}

	bool operator <(const Hex& other) const {
		return (this->q< other.q ||
			(this->q == other.q && this->r < other.r) ||
			(this->q == other.q && this->r == other.r && this->s < other.s));
	}

	Hex operator+(const Hex& other) {
		return Hex(
			this->q + other.q,
			this->r + other.r,
			this->s + other.s);
	}

	Hex operator-(const Hex& other) {
		return Hex(
			this->q - other.q,
			this->r - other.r,
			this->s - other.s);
	}

	Hex operator* (int k) {
		return Hex(
			this->q * k,
			this->r * k,
			this->s * k);
	}

	Hex& operator=(const Hex& other) {
		this->q = other.q;
		this->r = other.r;
		this->s = other.s;
		return *this;
	}

	std::string Read() const {
		std::ostringstream oss;
		oss << "(" << this->q << ", " << this->r << ", " << this->s << ")" << std::dec;
		// Add more information if needed based on your struct members
		return oss.str();
	}
};

namespace std {
	template <> struct hash<Hex> {
		size_t operator()(const Hex& h) const {
			hash<int> int_hash;
			size_t hq = int_hash(h.q);
			size_t hr = int_hash(h.r);
			return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
		}
	};
}

//Rounds hex coordinates of the pixel conversion to the correct hex
Hex HexRound(double fractQ, double fractR, double fractS) {
	// Round the input parameters to integers
	int q = int(round(fractQ));
	int r = int(round(fractR));
	int s = int(round(fractS));

	// Calculate differences between rounded values and original values
	double q_diff = abs(q - fractQ);
	double r_diff = abs(r - fractR);
	double s_diff = abs(s - fractS);

	// Adjust the ccordinates
	if (q_diff > r_diff and q_diff > s_diff) {
		q = -r - s;
	}
	else if (r_diff > s_diff) {
		r = -q - s;
	}
	else {
		s = -q - r;
	}

	return Hex(q, r, s);
}

//Hexagonal shaped grid
class HexGrid {
public:
	static const std::array<Hex, 6> DIRS;

	HexGrid(int _radius = 4, const Hex& _origin = Hex(0,0,0)) : radius(_radius), origin(_origin) {
		MakeGraph();
	}

	HexGrid(const std::string& jsonFilePath);

	void MakeGraph();

	virtual void ReadGrid();

	std::vector<Hex> Neighbors(Hex& hex);

	int Distance(Hex& start, Hex& goal) {
		int dist = std::max(abs(start.q - goal.q), abs(start.r - goal.r));
		dist = std::max(dist, abs(start.s - goal.s));
		return dist;
	}

	bool IsInBound(Hex& hex) {
		if (Distance(origin, hex) <= radius) {
			return true;
		}
		return false;
	}

	const std::unordered_set<Hex>& VisitNodes() const { return nodes; }

protected:
	int radius;
	Hex origin;

	std::unordered_set<Hex> nodes;
};

//These are all counter-clock wise
const std::array<Hex, 6> HexGrid::DIRS = {
	Hex(1, 0, -1),	//Pointy: right		| Flat: down-right
	Hex(1, -1, 0),	//Pointy: up-right	| Flat: up-right
	Hex(0, -1, 1),	//Pointy: up-left	| Flat: up
	Hex(-1, 0, 1),	//Pointy: left		| Flat: up-left
	Hex(-1, 1, 0),	//Pointy: down-left	| Flat: down-left
	Hex(0, 1, -1)	//Pointy: down-right| Flat: down

};

HexGrid::HexGrid(const std::string& jsonFilePath) {
	// Read the JSON file
	std::ifstream file(jsonFilePath);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << jsonFilePath << std::endl;
	}

	// Parse the JSON data
	nlohmann::json jsonData;
	try {
		file >> jsonData;
		// Accessing individual values
		this->radius = jsonData["radius"];
		std::cout << "Radius: " << radius << std::endl;

		// Accessing values inside the "origin" object
		
		int q = jsonData["origin"]["q"];
		int r = jsonData["origin"]["r"];

		this->origin = Hex(q, r, -q - r);

		std::cout << "Origin: " << origin.Read() << std::endl;

		MakeGraph();
	}
	catch (const std::exception& e) {
		std::cerr << "Error parsing JSON: " << e.what() << std::endl;
	}
}

void HexGrid::MakeGraph() {
	//distance = max(abs(q), abs(r), abs(s))
	for (int q = -radius; q <= radius; q++) {
		int start = std::max(-radius, -q - radius);
		int end = std::min(radius, radius - q);
		for (int r = start; r <= end; r++) {
			int s = -q - r;

			Hex current(q, r, s);

			this->nodes.insert(current + this->origin);
		}
	}
}

void HexGrid::ReadGrid() {
	std::cout << "Nodes: " << std::endl;

	for (const auto& hex : nodes) {
		std::cout << hex.Read() << "\t";
	}
	
	std::cout << std::endl;
}

std::vector<Hex> HexGrid::Neighbors(Hex& hex) {
	std::vector<Hex> neighbors;

	for (auto dir : DIRS) {
		Hex next(hex + dir);
		if (IsInBound(next)) {
			neighbors.push_back(next);
		}
	}

	return neighbors;
}
