#pragma once

#include <cassert>
#include <iostream>
#include <sstream>
#include <array>

#include <SFML/Graphics.hpp>

#include "Node.h"

//Cube coordinates
struct Hex : public Node {
	int q, r, s;

	Hex(int _q, int _r, int _s) : q(_q), r(_r), s(_s) {	//Cube constructor
		assert(q + r + s == 0);
	}

	Hex(int _q = 0, int _r = 0) : Hex(_q, _r, -_q - _r) {}	//Axial constructor

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
		return (this->q < other.q ||
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

	const std::string PrintOut() const override {
		std::ostringstream oss;
		oss << "(" << this->q << ", " << this->r << ", " << this->s << ")" << std::dec;

		return oss.str();
	}

	std::vector<int>& GetCoordinates() override { 
		std::vector<int>tmp;
		tmp.push_back(q);
		tmp.push_back(r);
		tmp.push_back(s);
		return tmp;
	}
	void ChangeCoordinates(std::vector<int> newCoordinates) {
		q = newCoordinates[1];
		r = newCoordinates[2];
		s = newCoordinates[3];
	
	}
};

//Basic hash 
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

sf::Vector2f HexToPixel(const Hex& hex, int tileRadius, sf::Vector2i windowCenter) {
	float x, y;
	int size = tileRadius;

	x = size * (sqrt(3) * hex.q + sqrt(3) / 2 * hex.r);
	y = size * (3. / 2 * hex.r);

	x += windowCenter.x;
	y += windowCenter.y;

	return sf::Vector2f(x, y);
}

Hex PixelToHex(sf::Vector2f pixelPos, int tileRadius, sf::Vector2i windowCenter) {
	pixelPos.x -= windowCenter.x;
	pixelPos.y -= windowCenter.y;

	int radius = tileRadius;

	double q = ((pixelPos.x * sqrt(3) / 3) - (pixelPos.y * 1. / 3)) / radius;
	double r = (pixelPos.y * 2. * 1. / 3) / radius;

	return HexRound(q, r, -q - r);
}

//Counter-clock wise
const std::array<Hex, 6> DIRS = {
	Hex(1, 0, -1),	//Pointy: right		| Flat: down-right
	Hex(1, -1, 0),	//Pointy: up-right	| Flat: up-right
	Hex(0, -1, 1),	//Pointy: up-left	| Flat: up
	Hex(-1, 0, 1),	//Pointy: left		| Flat: up-left
	Hex(-1, 1, 0),	//Pointy: down-left	| Flat: down-left
	Hex(0, 1, -1)	//Pointy: down-right| Flat: down
};

