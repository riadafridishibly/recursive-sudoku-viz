#include "raylib.h"
#include <iostream>
#include <stack>
#include <string>
#include <vector>

#define N 9
#define W 9
#define H 9
#define GRID_FULL (-1)

int idx(int r, int c);
int get_next_empty_cell(const std::vector<int> &g);
bool is_valid(const std::vector<int> &puzzle, int p, int val);
bool is_puzzle_valid(const std::vector<int> &puzzle);
bool in_3x3(const std::vector<int> &puzzle, int r, int c, int val);
bool in_row(const std::vector<int> &puzzle, int r, int val);
bool in_col(const std::vector<int> &puzzle, int c, int val);
void draw_board(const std::vector<int> &v);

struct State {
    int pos;
    int val;
    State(int _pos, int _val) : pos{_pos}, val{_val} {}
};

constexpr float w = 50;
bool solved = false;
std::stack<State> stack;
bool original_grid[N * N];

int main(void) {
    const int screenWidth = int(w * N);
    const int screenHeight = int(w * N);

    auto grid = std::vector<int>{
        5, 3, 0, 0, 7, 0, 0, 0, 0,
        6, 0, 0, 1, 9, 5, 0, 0, 0,
        0, 9, 8, 0, 0, 0, 0, 6, 0,
        8, 0, 0, 0, 6, 0, 0, 0, 3,
        4, 0, 0, 8, 0, 3, 0, 0, 1,
        7, 0, 0, 0, 2, 0, 0, 0, 6,
        0, 6, 0, 0, 0, 0, 2, 8, 0,
        0, 0, 0, 4, 1, 9, 0, 0, 5,
        0, 0, 0, 0, 8, 0, 0, 7, 9,
    };

    if (!is_puzzle_valid(grid)) {
        std::cerr << "[Error] Puzzle is not valid\n";
        return 1;
    }

    InitWindow(screenWidth, screenHeight, "Recursive Sudoku Solver Visualizer");

    // SetTargetFPS(60);

    // It's a mark for the initially given values
    // to draw the fixed grid
    for (int i = 0; i < N * N; ++i) {
        original_grid[i] = grid[i] != 0;
    }

    // assume the first grid accept 1.
    // and push it to the stack.
    int pos = get_next_empty_cell(grid);
    stack.push(State(pos, 1));

    while (!WindowShouldClose()) {

        // if it is not solved yet
        if (!solved) {

            // we have state to deal with
            if (!stack.empty()) {

                State curr = stack.top();
                int position = curr.pos;
                int val = curr.val;

                if (is_valid(grid, position, val)) {
                    grid[position] = val;

                    int next_empty = get_next_empty_cell(grid);

                    if (next_empty == GRID_FULL) {
                        solved = true;
                    }

                    stack.push(State(next_empty, 1));
                } else {
                    // current cell is not valid
                    // check if the value is 9
                    // if it is we can't increment it further
                    // because sudoku accepts value [1, 9]

                    State prev = stack.top();
                    stack.pop();

                    if (prev.val == 9) {
                      grid[prev.pos] = 0;
                    } else {
                      prev.val++;
                      stack.push(prev);
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);
        draw_board(grid);
        EndDrawing();
    }

    // TakeScreenshot("screenshot.png");

    CloseWindow();

    return 0;
}

int idx(int r, int c) { return r * N + c; }

int get_next_empty_cell(const std::vector<int> &g) {

    for (int i = 0; i < N * N; ++i) {
        if (g[i] == 0)
            return i; // got an index
    }
    return GRID_FULL;
}

// Check if the value 'val' is in r-th row
bool in_row(const std::vector<int> &puzzle, int r, int val) {
    for (int i = 0; i < N; ++i) {
        if (puzzle[idx(r, i)] == val)
            return true;
    }
    return false;
}

// Check if the value 'val' is in c-th column
bool in_col(const std::vector<int> &puzzle, int c, int val) {
    for (int i = 0; i < N; ++i) {
        if (puzzle[idx(i, c)] == val)
            return true;
    }
    return false;
}

// Check if the value 'val' is in 3x3 box
bool in_3x3(const std::vector<int> &puzzle, int r, int c, int val) {
    for (int i = r; i < r + 3; ++i) {
        for (int j = c; j < c + 3; ++j) {
            if (puzzle[idx(i, j)] == val)
                return true;
        }
    }
    return false;
}

// Check if p position is a valid position
bool is_valid(const std::vector<int> &puzzle, int p, int val) {

    // Trick to map 1D index to 2D index
    int c = p % N;
    int r = p / N;

    return !in_row(puzzle, r, val) && !in_col(puzzle, c, val) &&
           !in_3x3(puzzle, r - r % 3, c - c % 3, val) && val >= 0 && val <= 9;
}

bool is_puzzle_valid(const std::vector<int> &puzzle) {

  for (int pos = 0; pos < static_cast<int>(puzzle.size()); ++pos) {
    int val = puzzle[pos];
    if (val != 0) {

      int c = pos % N;
      int r = pos / N;

      int row_count = 0;
      for (int i = 0; i < N; ++i) {
        if (puzzle[idx(r, i)] == val)
          row_count++;
      }

      int col_count = 0;
      for (int i = 0; i < N; ++i) {
        if (puzzle[idx(i, c)] == val)
          col_count++;
      }

      int box_count = 0;
      int corner_row = r - r % 3;
      int corner_col = c - c % 3;
      for (int i = corner_row; i < corner_row + 3; ++i) {
        for (int j = corner_col; j < corner_col + 3; ++j) {
          if (puzzle[idx(i, j)] == val)
            box_count++;
        }
      }
      if (row_count != 1 || col_count != 1 || box_count != 1) {
        return false;
      }
    }
  }
    return true;
}

void draw_board(const std::vector<int> &grid) {

    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < N; ++i) {

            auto rec = Rectangle{
                i * w, // x
                j * w, // y
                w,     // width
                w      // height
            };

            if (grid[idx(j, i)] != 0) {

                if (original_grid[idx(j, i)]) {
                    DrawRectangleRec(rec, DARKPURPLE);
                }
                auto str = std::to_string(grid[idx(j, i)]);
                auto font_size = 20;
                int str_size = MeasureText(str.c_str(), font_size);
                DrawText(str.c_str(), i * w + w / 2.0 - str_size / 2.0,
                        j * w + w / 2 - font_size / 2, font_size, RAYWHITE);
            }

            DrawRectangleLinesEx(rec, 2, BLUE);
        }
    }
}
