#pragma once

#include <cassert>
#include <iostream>
#include <sstream>
#include <array>

#include <SFML/Graphics.hpp>

struct Square {
	int x, y;

	Square(int _x = 0, int _y = 0) : x(_x), y(_y) {}
	Square(const Square& original) : x(original.x), y(original.y) {}
	Square(const nlohmann::json& obj): x(obj["x"]), y(obj["y"]) {}

	bool operator == (const Square& other) const {
		return (this->x == other.x &&
			this->y == other.y);
	}

	bool operator !=(const Square& other) const { return !(*this == other); }

	bool operator <(const Square& other) const {
		return (this->x < other.x ||
			(this->x == other.x && this->y < other.y));
	}

	Square operator+ (const Square& other) {
		return Square(
			this->x + other.x,
			this->y + other.y);
	}

	Square operator- (const Square& other) {
		return Square(
			this->x - other.x,
			this->y - other.y);
	}

	Square operator* (int k) {
		return Square(
			this->x * k,
			this->y * k);
	}

	Square operator= (const Square& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	const std::string PrintOut() const {
		std::ostringstream oss;
		oss << "(" << this->x << ", " << this->y << ")" << std::dec;

		return oss.str();
	}
};

namespace std {
	template <> struct hash<Square> {
		size_t operator()(const Square& s) const {
			hash<int> int_hash;
			size_t hash_x = int_hash(s.x);
			size_t hash_y = int_hash(s.y);
			return hash_x ^ (hash_y + 0x9e3779b9 + (hash_x << 6) + (hash_x >> 2));
		}
	};
}

sf::Vector2f SquareToPixel(const Square& square, int tileSide, sf::Vector2i windowCenter) {
	float px, py;

	px = square.x * tileSide;
	py = square.y * tileSide;

	px += windowCenter.x;
	py += windowCenter.y;
	
	return sf::Vector2f(px, py);
}

Square PixelToSquare(sf::Vector2f pixelPos, int tileSide, sf::Vector2i windowCenter) {
	pixelPos.x -= windowCenter.x;
	pixelPos.y -= windowCenter.y;
	
	double x = pixelPos.x / tileSide;
	double y = pixelPos.y / tileSide;

	return Square(int(floor(x)), int(floor(y)));
}


struct SquareTile {
	sf::RectangleShape body;

	SquareTile(float side) : body(sf::Vector2f(side, side)) {
		body.setOrigin(side / 2., side / 2.);
		body.setFillColor(sf::Color::Transparent);
		body.setOutlineColor(sf::Color::Black);
		body.setOutlineThickness(1);
	}
	SquareTile(const SquareTile& other) : body(other.body) {}

	void SetOrigin(sf::Vector2f origin) { body.setOrigin(origin); }
	void SetPosition(sf::Vector2f pos) { body.setPosition(pos); }
	void SetFillColor(const sf::Color& color) { body.setFillColor(color); }
	void SetOutlineColor(const sf::Color& color) { body.setOutlineColor(color); }
	void SetOutlineThickness(float size) { body.setOutlineThickness(size); }
	sf::Vector2f Position() { return body.getPosition(); }

	void SetSide(float side) {
		body.setOrigin((-body.getSize().x) / 2., (-body.getSize().x) / 2.);
		body.setSize(sf::Vector2f(side, side)); 
		body.setOrigin(side / 2., side / 2.);

	}
	float Side() { return body.getSize().x; }
	float Diagonal() { return (Side() * sqrt(2.)); }

	sf::Vector2f TopHalfSide() { return sf::Vector2f(0, -(Side() / 2.)); }
	sf::Vector2f RightHalfSide() { return sf::Vector2f((Side() / 2.), 0); }
	sf::Vector2f BottomHalfSide() { return sf::Vector2f(0, +(Side() / 2.)); }
	sf::Vector2f LeftHalfSide() { return sf::Vector2f(-(Side() / 2.), 0); }
};