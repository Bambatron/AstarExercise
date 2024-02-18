#pragma once
#pragma once

#include <vector>

struct Node {
public:
	virtual ~Node() = default;
	virtual const std::string PrintOut() const = 0;
	virtual std::vector<int>& GetCoordinates() = 0;
	virtual void ChangeCoordinates(std::vector<int> newCoordinates) = 0;
};