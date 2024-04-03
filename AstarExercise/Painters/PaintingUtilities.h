#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <SFML/Graphics.hpp>

struct Arrow {
public:
	/// <summary>
	/// Static factory method for an arrow between two point
	/// </summary>
	/// <param name="start">Starting position</param>
	/// <param name="end">End position, where the arrow points to</param>
	/// <returns></returns>
	static Arrow makeBasicArrow(sf::Vector2f start, sf::Vector2f end) { return Arrow(start, end); }

	sf::VertexArray& getBody() { return body; }

private:
	sf::VertexArray body;

	Arrow(sf::Vector2f start = sf::Vector2f(0, 0), sf::Vector2f end = sf::Vector2f(0,0), sf::Color color = sf::Color(255, 255,0),
		float headTailLengthRatio = 0.2, float tailWidthLengthRatio = 0.1, float headWidthTailWidthRatio = 3.f,
		float outlineThickness = 1.f, sf::Color outlineColor = sf::Color::Black)
		: body(sf::PrimitiveType::TriangleStrip, 7) {
		
		// Length of the arrow
		sf::Vector2f direction(end.x - start.x, end.y - start.y);
		float length = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
		float headLength = length * headTailLengthRatio;	// Length of the head
		float tailLength = length - headLength;	// Length of the tail

		// Widths of the arrow
		float tailWidth = tailWidthLengthRatio * length;	// Width of the tail
		float headWidth = headWidthTailWidthRatio * tailWidth; // Width of the tail

		// Angle to the x axis
		float angle = atan2(direction.y, direction.x);	// This is in radiants
		angle *= (180.f / M_PI);	//This in degrees



		// Tail part
		sf::Transform tailTransform;
		tailTransform.rotate(angle, start);

		sf::Vector2f p0 = start + sf::Vector2f(0 , -tailWidth / 2.f);
		sf::Vector2f p1 = start + sf::Vector2f(0, +tailWidth / 2.f);
		sf::Vector2f p2= start + sf::Vector2f(tailLength, - tailWidth / 2.f);
		sf::Vector2f p3 = start + sf::Vector2f(tailLength, +tailWidth / 2.f);
		p0 = tailTransform.transformPoint(p0);
		p1 = tailTransform.transformPoint(p1);
		p2 = tailTransform.transformPoint(p2);
		p3 = tailTransform.transformPoint(p3);

		// Head part
		sf::Transform headTransform;
		headTransform.rotate(angle, end);

		sf::Vector2f p4 = end;
		sf::Vector2f p5 = end + sf::Vector2f(-headLength, +headWidth / 2.f);
		sf::Vector2f p6 = end + sf::Vector2f(-headLength, -headWidth / 2.f);
		p4 = headTransform.transformPoint(p4);
		p5 = headTransform.transformPoint(p5);
		p6 = headTransform.transformPoint(p6);



		// Sets the position of the vertex
		body[0].position = p0;
		body[1].position = p1;
		body[2].position = p2;
		body[3].position = p3;
		body[4].position = p4;
		body[5].position = p5;
		body[6].position = p6;

		// Sets the color
		body[0].color = color;
		body[1].color = color;
		body[2].color = color;
		body[3].color = color;
		body[4].color = color;
		body[5].color = color;
		body[6].color = color;
	}
};