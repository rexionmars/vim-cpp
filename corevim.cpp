#include "corevim.hpp"
#include <curses.h>

CoreVim::CoreVim(const std::string& file) {
    lines.push_back("");
    x = y = 0;
    mode = 'n';
    status = "NORMAL";

    if (file.empty()){
        filename = "untitle";
    } else{
        filename = file;
    }

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);
}

CoreVim::~CoreVim(){
    refresh();
    endwin();
}

void CoreVim::run(){
    while(mode != 'q'){
        update();
        statusline();
        print();
        int c = getch();
        input(c);
    }
}

void CoreVim::update(){
    switch (mode){
        case 27:
        case 'n':
            status = "NORMAL";
            break;
        case 'i':
            status = "INSERT";
            break;
        case 'q':
            break;
    }
}

void CoreVim::statusline(){
    attron(A_REVERSE);
    mvprintw(LINES -1, 0, status.c_str());
    attroff(A_REVERSE);
}

void CoreVim::input(int c){
    switch (c){
        case KEY_UP:
            up();
            return;
        case KEY_LEFT:
            left();
            return;
        case KEY_RIGHT:
            right();
            return;
        case KEY_DOWN:
            down();
            return;
    }

    switch (mode){
        case 27:
        case 'n':
            switch (c){
                case 27:
                case 'q':
                    mode = 'q';
                    break;
                case 'i':
                    mode = 'i';
                    break;
                case 'w':
                    mode = 'w';
                    break;
            }
        //return;
        case 'i':
            switch (c){
                case 27:
                    mode = 'n';
                    break;
                case 127:
                case KEY_BACKSPACE:
                    if(x == 0 && y > 0){
                        x = lines[y - 1].length();
                        lines[y - 1] += lines[y];
                        m_remove(y);
                    } else if(x > 0){
                        lines[y].erase(--x, 1);
                    }
                break;
                default:
                    lines[y].insert(x, 1, c);
                    ++x;
            }
    }
}

void CoreVim::print(){
    for (size_t i {}; i < (size_t)LINES - 1; ++i){
        if (i >=  lines.size()){
            move(i, 0);
            clrtoeol();
        } else {
            mvprintw(i, 0, lines[i].c_str());
        }
        clrtoeol();
    }

    move(y, x);
}

void CoreVim::m_remove(int number){
    lines.erase(lines.begin() + number);
}

std::string CoreVim::m_tabs(std::string& line){
    std::size_t tab = line.find('\t');
    return tab == line.npos ? line : m_tabs(line.replace(tab, 1, "  "));
}

void CoreVim::m_insert(std::string line, int number){
    line = m_tabs(line);
    lines.insert(lines.begin() + number, line);
}

void CoreVim::m_append(std::string& line){
    line = m_tabs(line);
    lines.push_back(line);
}

void CoreVim::up(){
    if(y > 0){
        --y;
    }
    if(x >= lines[y].length()){
        x = lines[y].length();
    }
    move(y, x);
}

void CoreVim::left(){
    if(x > 0){
        --x;
        move(y, x);
    }
}

void CoreVim::right(){
    if((int)x <= COLS && x <= lines[y].length() -1){
        ++x;
        move(y, x);
    }
}

void CoreVim::down(){
    if((int)y < LINES && y < lines.size() -1){
        ++y;
    }
    if(x >= lines[y].length()){
        x = lines[y].length();
        move(y, x);
    }
}
