#pragma once

#include <cassert>
#include <iostream>
#include <sstream>
#include <array>

#include <SFML/Graphics.hpp>

//Cube coordinates
struct Hex {
	int q, r, s;

	Hex(int _q, int _r, int _s) : q(_q), r(_r), s(_s) {	//Cube constructor
		assert(q + r + s == 0);
	}
	Hex(int _q = 0, int _r = 0) : Hex(_q, _r, -_q - _r) {}	//Axial constructor
	Hex(const Hex& original) : q(original.q), r(original.r), s(original.s) {
		assert(q + r + s == 0);
	}
	Hex(const nlohmann::json& obj) : q(obj["q"]), r(obj["r"]) {
		s = -q - r;
		assert(q + r + s == 0);
	}

	bool operator==(const Hex& other) const {
		return (this->q == other.q &&
			this->r == other.r &&
			this->s == other.s);
	}
	bool operator!=(const Hex& other) { return !(*this == other); }
	bool operator <(const Hex& other) const {
		return (this->q < other.q ||
			(this->q == other.q && this->r < other.r) ||
			(this->q == other.q && this->r == other.r && this->s < other.s));
	}

	Hex operator+(const Hex& other) const {
		return Hex(
			this->q + other.q,
			this->r + other.r,
			this->s + other.s);
	}
	Hex operator-(const Hex& other) const {
		return Hex(
			this->q - other.q,
			this->r - other.r,
			this->s - other.s);
	}
	Hex operator* (int k) const {
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

	const std::string PrintOut() const {
		std::ostringstream oss;
		oss << "(" << this->q << ", " << this->r << ", " << this->s << ")" << std::dec;

		return oss.str();
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
	if (q_diff > r_diff && q_diff > s_diff) {
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

	double q = ((pixelPos.x * sqrt(3) / 3) - (pixelPos.y * 1. / 3)) / tileRadius;
	double r = (pixelPos.y * 2. * 1. / 3) / tileRadius;

	return HexRound(q, r, -q - r);
}



struct HexTile {
	sf::CircleShape body;

	HexTile(float radius) : body(radius, 6) {
		body.setFillColor(sf::Color::Transparent);
		body.setOutlineColor(sf::Color::Black);
		body.setOutlineThickness(1);
		body.setOrigin(radius, radius);
	}
	HexTile(const HexTile& other) : body(other.body) {}

	// Getters
	sf::Vector2f Position() const { return body.getPosition(); }

	float Radius() const { return body.getRadius(); }
	float Apothem() const { return ((body.getRadius() * sqrt(3.)) / 2.); }

	sf::Vector2f TopLeftSide() const {
		float a = Apothem();
		//Should be (-a/2, -a sqrt(3)/2) however it goes outside the tile
		return sf::Vector2f(
			(-a / 2.) + 2.,
			(-a * sqrt(2) / 2.) + 5.);
	}
	sf::Vector2f DownLeftSide()const {
		float a = Apothem();
		//Should be (-a/2, a sqrt(3)/2) however it goes outside the tile
		return sf::Vector2f(
			(-a / 2.) + 2.,
			(a * sqrt(2) / 2.) - 5.);
	}
	sf::Vector2f RightSide() const {
		float a = Apothem();
		//Should be (a, 0) however it goes outside the tile
		return sf::Vector2f((a * sqrt(2) / 2.) - 2., 0);
	}

	// Setters
	void SetOrigin(sf::Vector2f origin) { body.setOrigin(origin); }
	void SetPosition(sf::Vector2f pos) { body.setPosition(pos); }

	void SetFillColor(const sf::Color& color) { body.setFillColor(color); }
	void SetOutlineColor(const sf::Color& color) { body.setOutlineColor(color); }
	
	void SetOutlineThickness(float size) { body.setOutlineThickness(size); }
	void SetRadius(unsigned int radius) {
		body.setOrigin(-body.getRadius(), -body.getRadius());   //Reset the origin to the top left corner
		body.setRadius(radius); //Update the radius 
		body.setOrigin(body.getRadius(), body.getRadius());   //Set the origin to the new center 
	}
};