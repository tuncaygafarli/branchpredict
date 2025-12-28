#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

#include "gui_command_parser.h"
#include "gui_render.h"
#include "../cpu/cpu.h"
#include "../parser/parser.h"

parser_t parser;

GUICommandParser::GUICommandParser(GUIRender& gui_render, CPU& cpu, parser_t& parser) : gui_render(gui_render), cpu(cpu), parser(parser) {
    std::cout << "All classes initialized!" << std::endl;
}

void GUICommandParser::execute(const std::string& command_line) {}

void GUICommandParser::parse(const std::string& command_line) {
    std::istringstream iss(command_line);
    std::string cmd;
    iss >> cmd;

    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

    if (cmd == "load") {
        std::string filename;
        iss >> filename;

        std::cout << "Filename: " << filename << std::endl;

        if (filename.empty()) {
            std::cout << "ERROR: No filename!" << std::endl;
            return;
        }

        gui_render.instruction_codes.clear();
        cpu.reset();
        cpu.load_program(parser.parse_program(filename, gui_render));
        gui_render.update_instructions(cpu);
        gui_render.update_registers(cpu);
    }

    else if (cmd == "run") {
        std::string filename;
        iss >> filename;

        std::cout << "Filename: " << filename << std::endl;

        if (filename.empty()) {
            std::cout << "ERROR: No filename!" << std::endl;
            return;
        }

        gui_render.instruction_codes.clear();
        cpu.reset();
        cpu.load_program(parser.parse_program(filename, gui_render));
        gui_render.update_instructions(cpu);
        gui_render.update_registers(cpu);
    }
}