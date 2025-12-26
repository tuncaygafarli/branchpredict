#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

#include "instruction_box.h"

instruction_box_t instruction_box;

void instruction_box_t::draw_instructions(sf::RenderWindow& window, std::vector<std::string> instructions) {
	sf::Vector2f size(window.getSize().x, 50.f);
	sf::Font font;

	if (!font.loadFromFile("C:\\Users\\Admin\\Downloads\\BigBlueTerminal\\BigBlueTermPlusNerdFontMono-Regular.ttf"))
	{
		std::cerr << "Failed to load font." << "\n";
	}

	float startX = 0.f;
	float startY = 0.f;
	float boxWidth = window.getSize().x;
	float boxHeight = 40.f;  
	float spacing = 5.f;


	for (int i = 0; i < instructions.size(); i++) {

		float yPos = startY + i * (boxHeight + spacing);

		sf::RectangleShape box(size);
		box.setPosition(startX, yPos);
		box.setFillColor(sf::Color(45, 45, 50));
		window.draw(box);

		sf::Text text;
		text.setFont(font);
		text.setString(instructions[i]);
		text.setCharacterSize(24);
		text.setPosition(box.getSize().x / 2, box.getSize().y / 2);
		text.setFillColor(sf::Color::White);

		sf::FloatRect textBounds = text.getLocalBounds();
		float textX = startX + (boxWidth - textBounds.width) / 2.f;
		float textY = yPos + (boxHeight - textBounds.height) / 2.f - 5.f;

		text.setPosition(textX, textY);
		window.draw(text);
	}
}

void instruction_box_t::add_instruction(std::string asm_code) {
	instruction_box.instructions.emplace_back(asm_code);
}