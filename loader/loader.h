#pragma once

#include <vector>

#include "token.h"
#include "lookup.h"
class loader_t {
public:
    void load_program(const std::string& src,CPU& cpu);
private:
    void        load_instruction();
    void        load_mem_instruction();
    void        load_alu_instruction();
    void        load_branch_instruction();
    void        load_jump_instruction();
    void        load_load_upperimm_instruction();
    void        load_auipc_instruction();
    void        load_label();
    void        tokenize_line_text(const std::string &line_raw);
    void        advance();
    label_id_t  unique_label_id();
private:
    token_t&                                    _current_token;
    size_t                                      _current_index;
    std::unordered_map<std::string, label_id_t> _label_map;
    std::vector<token_t>                        _line_tokens;
    program_t                                   _program;
};