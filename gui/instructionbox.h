#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class instruction_box_t {
public :
	void render_box(sf::RenderWindow& window);

private:
	struct instruction_element_t {
		sf::Color bg_color;
		float width;
		float height;
		std::string asm_code;
	};

	float width;
	float height;
};