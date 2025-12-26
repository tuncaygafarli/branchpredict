#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "../cpu/cpu.h"

class instruction_box_t {
public :
	instruction_box_t() = default;
	void draw_instructions(sf::RenderWindow& window, std::vector<std::string> instructions);
	void add_instruction(std::string asm_code);

	std::vector<std::string> instructions = { "MOV EAX, 10", "MOV [EBX], EAX" };

private:
	struct instruction_t {
		sf::RectangleShape background;
		sf::Text text;
		sf::Color bg_color;
		bool selected = false;
		int id;
	};
};