#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

using namespace sf;

const int ROWS = 20;
const int COLUMNS = 10;

int field[ROWS][COLUMNS] = { 0 };
int tileSize = 34;

struct Point {
    int x, y;
} currentShape[4], previousShape[4];

int shapes[7][4] = {
    1,3,5,7,  // I
    2,4,5,7,  // Z
    3,5,4,6,  // S
    3,5,4,7,  // T
    2,3,5,7,  // L
    3,5,7,6,  // J
    2,3,4,5,  // O
};

bool isValidPosition() {
    for (int i = 0; i < 4; i++) {
        if (currentShape[i].x < 0 || currentShape[i].x >= COLUMNS || currentShape[i].y >= ROWS)
            return false;
        else if (field[currentShape[i].y][currentShape[i].x])
            return false;
    }
    return true;
}

int main() {
    srand(time(0));

    RenderWindow window(VideoMode(COLUMNS * tileSize, ROWS * tileSize), "Tetris!");

    Texture tileTexture;
    tileTexture.loadFromFile("C:/Users/admin/OneDrive/Desktop/Paint/cubes.png");
    Sprite tileSprite(tileTexture);

    int moveDirection = 0, currentColor = 1;
    bool rotate = false;
    float timer = 0, delay = 0.3;
    Clock clock;
    bool isFirstShape = true;

    int gameOverCounter = 0;

    Texture gameOverTexture;
    gameOverTexture.loadFromFile("C:/Users/admin/OneDrive/Desktop/Paint/gameover.png");
    Sprite gameOverSprite(gameOverTexture);
    gameOverSprite.setPosition(10, 300);
    gameOverSprite.setScale(0.4, 0.4);

    while (window.isOpen()) {
        float elapsedTime = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += elapsedTime;

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Up)
                    rotate = true;
                else if (event.key.code == Keyboard::Right)
                    moveDirection = 1;
                else if (event.key.code == Keyboard::Left)
                    moveDirection = -1;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
            delay = 0.05;

        // Move the shape
        for (int i = 0; i < 4; i++) {
            previousShape[i] = currentShape[i];
            currentShape[i].x += moveDirection;
        }

        if (!isValidPosition()) {
            for (int i = 0; i < 4; i++)
                currentShape[i] = previousShape[i];
        }

        // Rotate the shape
        if (rotate) {
            Point center = currentShape[1];
            for (int i = 0; i < 4; i++) {
                int x = currentShape[i].y - center.y;
                int y = currentShape[i].x - center.x;

                currentShape[i].x = center.x - x;
                currentShape[i].y = center.y + y;
            }

            if (!isValidPosition()) {
                for (int i = 0; i < 4; i++)
                    currentShape[i] = previousShape[i];
            }
        }

        // Move shape down
        if (timer > delay) {
            for (int i = 0; i < 4; i++) {
                previousShape[i] = currentShape[i];
                currentShape[i].y += 1;
            }

            if (!isValidPosition()) {
                for (int i = 0; i < 4; i++)
                    field[previousShape[i].y][previousShape[i].x] = currentColor;

                currentColor = 1 + rand() % 7;
                int shapeType = rand() % 7;
                for (int i = 0; i < 4; i++) {
                    currentShape[i].x = shapes[shapeType][i] % 2;
                    currentShape[i].y = shapes[shapeType][i] / 2;
                }
            }

            timer = 0;
        }

        // Check for game over condition
        for (int i = 0; i < 4; i++) {
            if (currentShape[i].y > 0 && currentShape[i].y < 4)
                gameOverCounter++;
            else if (currentShape[i].y >= 4)
                gameOverCounter = 0;
        }

        std::cout << gameOverCounter << "\n"; // Display game over counter value

        // Initialize the first shape
        if (isFirstShape) {
            int shapeType = rand() % 7;
            if (currentShape[0].x == 0) {
                for (int i = 0; i < 4; i++) {
                    currentShape[i].x = shapes[shapeType][i] % 2;
                    currentShape[i].y = shapes[shapeType][i] / 2;
                }
            }
            isFirstShape = false;
        }

        // Remove filled lines
        int line = ROWS - 1;
        for (int i = ROWS - 1; i > 0; i--) {
            int filledBlocks = 0;
            for (int j = 0; j < COLUMNS; j++) {
                if (field[i][j])
                    filledBlocks++;
                field[line][j] = field[i][j];
            }
            if (filledBlocks < COLUMNS)
                line--;
        }

        moveDirection = 0;
        rotate = false;
        delay = 0.3;

        // Draw everything
        window.clear(Color::White);

        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLUMNS; j++) {
                if (field[i][j] == 0)
                    continue;
                tileSprite.setTextureRect(IntRect(field[i][j] * tileSize, 0, tileSize, tileSize));
                tileSprite.setPosition(j * tileSize, i * tileSize);
                window.draw(tileSprite);
            }

        for (int i = 0; i < 4; i++) {
            tileSprite.setTextureRect(IntRect(currentColor * tileSize, 0, tileSize, tileSize));
            tileSprite.setPosition(currentShape[i].x * tileSize, currentShape[i].y * tileSize);
            window.draw(tileSprite);
        }

        if (gameOverCounter > 100) // Display game over screen when counter is too high
            window.draw(gameOverSprite);

        window.display();
    }

    return 0;
}
