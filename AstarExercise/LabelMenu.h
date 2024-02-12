#pragma once

#include <iostream>

#include<SFML/Graphics.hpp>

class LabelMenu {
public:
    LabelMenu(sf::Vector2f size, sf::Vector2f position, sf::Vector2f _buffer = sf::Vector2f(10, 10), sf::Vector2f _padding = sf::Vector2f(5,5));
    ~LabelMenu() {}

    bool IsInBounds(sf::Vector2i mousePosition) {
        if (IsOpen()) {
            sf::FloatRect bounds = body.getGlobalBounds();

            if (mousePosition.x >= (bounds.left - buffer.x) &&
                mousePosition.x <= (bounds.left + bounds.width + buffer.x) &&
                mousePosition.y >= (bounds.top - buffer.y) && 
                mousePosition.y <= (bounds.top + bounds.top + buffer.y)) {
                return true;
            }
        }

        return false;
    }

    const sf::Vector2f GetPosition() const { return body.getPosition(); }
    void SetPosition(sf::Vector2f pos) {
        body.setPosition(pos);
        text.setPosition(pos.x + padding.x, pos.y + padding.y);
    }
    
    const sf::Vector2f GetSize() const { return body.getSize(); }
    
    const sf::Color& GetBackgroundColor() const { return body.getFillColor(); }
    void SetBackgroundColor(sf::Color& color) { body.setFillColor(color); }
    
    const sf::Color& GetBorderColor() const { return body.getOutlineColor(); }
    void SetBorderColor(sf::Color& color) { body.setOutlineColor(color); }
    
    const float GetBorderSize() { return -body.getOutlineThickness(); }
    void SetBorderSize(float size) { body.setOutlineThickness(-size); }

    const sf::Vector2f GetBuffer() const { return buffer; }
    void SetBuffer(sf::Vector2f bufferSize) {
        buffer.x = (bufferSize.x > 0) ? bufferSize.x : 0;
        buffer.y = (bufferSize.y > 0) ? bufferSize.y : 0;
    }

    const sf::Vector2f GetPadding() const { return padding; }
    void SetPadding(sf::Vector2f paddingSize) {
        padding.x = (paddingSize.x > 0) ? paddingSize.x : 0;
        padding.y = (paddingSize.y > 0) ? paddingSize.y : 0;
    }

    bool IsOpen() const { return _openend; }
    void Open() { _openend = true; }
    void Close() { _openend = false; }
    void ToggleOpeneded() { _openend = !_openend; }

    template<typename... Args>
    void AddParameters(std::string name, const Args&... parameters);
    const std::vector<std::pair<std::string, std::vector<std::string>>>& GetInfo() const { return info; }
    void ClearParameters() {
        info.clear();
        text.setString("");
    }

    void Render(sf::RenderWindow& target);
    
private:
    void AddParameter();
    template<typename T, typename... Args>
    void AddParameter(const T& parameter, const Args&... parameters);

    bool _openend;

    sf::RectangleShape body;
    sf::Vector2f buffer;
    sf::Vector2f padding;

    std::vector<std::pair<std::string, std::vector<std::string>>> info;

    sf::Font font;
    sf::Text text;
};

LabelMenu::LabelMenu(sf::Vector2f size, sf::Vector2f position, sf::Vector2f _buffer, sf::Vector2f _padding) : body(size), buffer(_buffer), padding(_padding) {
    body.setPosition(position);
    body.setFillColor(sf::Color(103,103,103, 192.5));
    body.setOutlineColor(sf::Color::Black);
    body.setOutlineThickness(3);
    
    _openend = false;

    if (!font.loadFromFile("wowsers.ttf")) {    //Error
        std::cout << "Error loading map font" << std::endl;
    }
    text.setFont(font);
    float textSize = 15;
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(textSize);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1);
}

template<typename... Args>
void LabelMenu::AddParameters(std::string name, const Args&... parameters) {
    text.setString(text.getString() + name);
    AddParameter(parameters...);
    text.setString(text.getString() + "\n");

    sf::FloatRect textBounds(text.getGlobalBounds());
    body.setSize(sf::Vector2f(textBounds.width + (padding.x * 2), textBounds.height + (padding.y * 2)));
}

void LabelMenu::AddParameter() {}

template<typename T, typename... Args>
void LabelMenu::AddParameter(const T& parameter, const Args&... parameters) {
    // Concatenate the current parameter into the text string
    text.setString(text.getString() + std::to_string(parameter) + ", ");
    // Recursively call DrawParameters with the remaining parameters
    AddParameter(parameters...);
}

void LabelMenu::Render(sf::RenderWindow& target) {
    target.draw(body);
    target.draw(text);
}

