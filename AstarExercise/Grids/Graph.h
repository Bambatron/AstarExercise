#pragma once


template<typename L, typename C, typename T>
class Graph {
public:
	using location_t = L;
	using cost_t = C;
	using tile_t = T;

	Graph(bool _weighted = false) : weighted(_weighted) {}

	virtual bool IsInBounds(const location_t& loc) const { return (nodes.find(loc) != nodes.end()); }

	virtual std::vector<location_t> Neighbors(const location_t& loc) const { return edges.at(loc); }

	virtual cost_t Weight(const location_t& loc) const {
		if (weighted) return nodes.at(loc);
		else return 1;
	}

	virtual cost_t Cost(const location_t& start, const location_t& goal) const {
		if (weighted) return nodes.at(goal) + nodes.at(start);
		else return 1;
	}

	virtual cost_t Heuristic(const location_t& start, const location_t& goal) const = 0;

	virtual const std::unordered_map<location_t, cost_t>& VisitNode() const { return nodes; }

protected:
	bool weighted;

	std::unordered_map<location_t, cost_t> nodes;
	std::unordered_map <location_t, std::vector<location_t>> edges;
};
