#include "raylib.h"
#include <stdio.h>

#define TILE_SIZE 64
#define GRID_WIDTH 10
#define GRID_HEIGHT 10

// File to save/load map
#define MAP_FILE "map.txt"

// Define tile types
typedef enum {
    TILE_EMPTY = 0,
    TILE_GRASS,
    TILE_WATER,
    TILE_COUNT
} TileType;

Color tileColors[TILE_COUNT] = {
    WHITE,    // TILE_EMPTY
    DARKGRAY,    // TILE_GRASS
    SKYBLUE      // TILE_WATER
};

int map[GRID_WIDTH][GRID_HEIGHT] = {0}; // Grid initialized to TILE_EMPTY
int currentTile = TILE_GRASS;

void SaveMapToFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file for writing.\n");
        return;
    }

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            fprintf(file, "%d ", map[x][y]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Map saved to %s\n", filename);
}

void LoadMapFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file for reading.\n");
        return;
    }

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            fscanf(file, "%d", &map[x][y]);
        }
    }

    fclose(file);
    printf("Map loaded from %s\n", filename);
}

int main() {
    InitWindow(GRID_WIDTH * TILE_SIZE, GRID_HEIGHT * TILE_SIZE + 40, "Tile Map Editor - File I/O Example");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Input handling
        if (IsKeyPressed(KEY_ONE)) currentTile = TILE_EMPTY;
        if (IsKeyPressed(KEY_TWO)) currentTile = TILE_GRASS;
        if (IsKeyPressed(KEY_THREE)) currentTile = TILE_WATER;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            int gridX = mouse.x / TILE_SIZE;
            int gridY = mouse.y / TILE_SIZE;

            if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT) {
                map[gridX][gridY] = currentTile;
            }
        }

        if (IsKeyPressed(KEY_S)) SaveMapToFile(MAP_FILE);
        if (IsKeyPressed(KEY_L)) LoadMapFromFile(MAP_FILE);

        // Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw grid and tiles
        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                Rectangle tile = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                DrawRectangleRec(tile, tileColors[map[x][y]]);
                DrawRectangleLines(tile.x, tile.y, tile.width, tile.height, LIGHTGRAY);
            }
        }

        // Draw current tile info
        DrawRectangle(0, GRID_HEIGHT * TILE_SIZE, GetScreenWidth(), 40, GRAY);
        DrawText(TextFormat("Current Tile: %d (Press 1/2/3) | Press S = Save | L = Load", currentTile),
                 10, GRID_HEIGHT * TILE_SIZE + 10, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
