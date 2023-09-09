#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define TICTACTOE_IMPLEMENTATION
#include "tictactoe.h"

#define SQ_HEIGHT   7
#define SQ_WIDTH    15
#define SQ_VSTRIDE  (SQ_HEIGHT + 0)
#define SQ_HSTRIDE  (SQ_WIDTH  + 1)

#define LOOP for (;;)

const char *x_square = "┌─────────────┐\n"
                       "│  @@     @@  │\n"
                       "│   @@@ @@@   │\n"
                       "│     @@@     │\n"
                       "│   @@@ @@@   │\n"
                       "│  @@     @@  │\n"
                       "└─────────────┘\n";

const char *o_square = "┌─────────────┐\n"
                       "│     @@@     │\n"
                       "│   @@   @@   │\n"
                       "│  @@     @@  │\n"
                       "│   @@   @@   │\n"
                       "│     @@@     │\n"
                       "└─────────────┘\n";

void draw_rectangle(size_t y, size_t x, size_t dy, size_t dx)
{
    dy -= 1;
    dx -= 1;
    mvhline(     y,      x, 0, dx);
    mvhline(y + dy,      x, 0, dx);
    mvvline(     y,      x, 0, dy);
    mvvline(     y, x + dx, 0, dy);
    mvaddch(     y,      x, ACS_ULCORNER);
    mvaddch(     y, x + dx, ACS_URCORNER);
    mvaddch(y + dy,      x, ACS_LLCORNER);
    mvaddch(y + dy, x + dx, ACS_LRCORNER);
}

void draw_board_square(Game *g, size_t row, size_t col) {
    draw_rectangle(row * SQ_VSTRIDE, col * SQ_HSTRIDE, SQ_HEIGHT, SQ_WIDTH);

    const char *sq;
    switch (g->board[row][col]) {
      case Player_X:    sq = x_square; break;
      case Player_O:    sq = o_square; break;
      case Player_NULL:                return;
      default: unreachable();
    }

    char copy[strlen(sq) + 1];
    char *save_ptr;
    strcpy(copy, sq);

    int r = 0;
    for (char *p = copy; r < SQ_HEIGHT; ++r, p = NULL) {
        char *line = strtok_r(p, "\n", &save_ptr);
        assert(line && "Somehow my handmade square string null-terminated early");

        move(row * SQ_VSTRIDE + r, col * SQ_HSTRIDE);
        printw("%s", line);
    }
}

void draw_board(Game *g)
{
    for (size_t r = 0; r < 3; ++r)
        for (size_t c = 0; c < 3; ++c)
            draw_board_square(g, r, c);
    refresh();
}

bool dialog_yesno(const char *msg)
{
    int height = 2 * SQ_HEIGHT;
    int width = 2 * SQ_WIDTH;
    WINDOW *diag = newwin(height, width, LINES / 2 - height / 2, COLS / 2 - width / 2);
    box(diag, 0, 0);
    wprintw(diag, "%s\n", msg);
    wprintw(diag, "[Y]es    ");
    wprintw(diag, "[N]o");
    wrefresh(diag);

    LOOP {
        switch (getch()) {
          case 'y': // FALLTHROUGH
          case 'Y': { delwin(diag); erase(); return true;  } break;

          case 'n': // FALLTHROUGH
          case 'N': { delwin(diag); erase(); return false; } break;
        }
    }
}

int main(void)
{
    Game g;
    enum {
        ColorPair_Normal,
        ColorPair_Selected,
    };

    setlocale(LC_ALL, "");

    initscr();
    cbreak();
    noecho();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);

    start_color();
    init_pair(ColorPair_Normal, COLOR_WHITE, COLOR_BLACK);
    init_pair(ColorPair_Selected, COLOR_RED, COLOR_BLACK);
    int old_curs = curs_set(0);

    Game_init(&g);
    draw_board(&g);

    int sel_r = 1;
    int sel_c = 1;
    LOOP {
        attron(COLOR_PAIR(ColorPair_Selected) | A_BOLD);
        draw_board_square(&g, sel_r, sel_c);
        attroff(COLOR_PAIR(ColorPair_Selected) | A_BOLD);

        int key = getch();

        attron(COLOR_PAIR(ColorPair_Normal));
        draw_board_square(&g, sel_r, sel_c);
        attroff(COLOR_PAIR(ColorPair_Normal));

        // TODO: add mouse integration poggers
        switch (key) {
          case KEY_DOWN: // FALLTHROUGH
          case 'j': { if (sel_r < 2) sel_r += 1; } continue;

          case KEY_UP: // FALLTHROUGH
          case 'k': { if (sel_r > 0) sel_r -= 1; } continue;

          case KEY_LEFT: // FALLTHROUGH
          case 'h': { if (sel_c > 0) sel_c -= 1; } continue;

          case KEY_RIGHT: // FALLTHROUGH
          case 'l': { if (sel_c < 2) sel_c += 1; } continue;

          case KEY_ENTER: // FALLTHROUGH
          case ' ': { Game_play(&g, sel_r, sel_c); } break;

          case 'q': // FALLTHROUGH
          case 'Q': {
            if (dialog_yesno("Are you sure about that?"))
                goto clean_exit;
            draw_board(&g);
          }
        }
    }

clean_exit:
    curs_set(old_curs);
    endwin();

    return 0;
}
