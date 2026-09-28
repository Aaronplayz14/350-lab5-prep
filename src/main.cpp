#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <optional>
#include <functional>

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;
const int FRAMES_PER_ANIMATION = 120;
std::uint64_t currentFrame = 0;
// global tween function
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    return (1 - t) * a + t * b;
};

void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        }

        const auto* keyPressed = event->getIf<sf::Event::KeyPressed>();
        if (!keyPressed) {
            continue;
        }
        
        // ====== ====== ======
        // TODO: (Q2)
        //  implement key presses (1-9) that replace the tween function
        //  with different alternate tween functions.
        //  Functions can be from lecture or from https://easings.net/#
        // ====== ====== ======
        constexpr float PI = 3.14159265358979323846f;
        if (keyPressed->code == sf::Keyboard::Key::Num1 || keyPressed->code == sf::Keyboard::Key::Numpad1) {
            tween = [](float a, float b, float t) {
                        return (1.0f - t) * a + t * b;
                    };
        }
        if (keyPressed->code == sf::Keyboard::Key::Num2 || keyPressed->code == sf::Keyboard::Key::Numpad2) {
            tween = [](float a, float b, float t) {
                        float eased = t * t;

                        return (1.0f - eased) * a + eased * b;
                    };
        }
        if (keyPressed->code == sf::Keyboard::Key::Num3 || keyPressed->code == sf::Keyboard::Key::Numpad3) {
            tween = [](float a, float b, float t) {
                        float inverse = 1.0f - t;
                        float eased = 1.0f - inverse * inverse;

                        return (1.0f - eased) * a + eased * b;
                    };
        }
        if (keyPressed->code == sf::Keyboard::Key::Num4 || keyPressed->code == sf::Keyboard::Key::Numpad4) {
            tween = [](float a, float b, float t) {
                        float eased;

                        if (t < 0.5f)
                        {
                            eased = 2.0f * t * t;
                        }
                        else
                        {
                            float value = -2.0f * t + 2.0f;

                            eased = 1.0f - (value * value) / 2.0f;
                        }

                        return (1.0f - eased) * a + eased * b;
                    };
        }
        if (keyPressed->code == sf::Keyboard::Key::Num5 || keyPressed->code == sf::Keyboard::Key::Numpad5){
            tween = [](float a, float b, float t) {
                        float eased = t * t * t;

                        return (1.0f - eased) * a + eased * b;
                    };
        }
        if (keyPressed->code == sf::Keyboard::Key::Num6 || keyPressed->code == sf::Keyboard::Key::Numpad6){
            tween = [](float a, float b, float t) {
                        float inverse = 1.0f - t;

                        float eased = 1.0f - inverse * inverse * inverse;

                        return (1.0f - eased) * a + eased * b;
                    };
        }
        if (keyPressed->code == sf::Keyboard::Key::Num7 || keyPressed->code == sf::Keyboard::Key::Numpad7){
            tween = [](float a, float b, float t) {
                        float eased;

                        if (t < 0.5f)
                        {
                            eased = 4.0f * t * t * t;
                        }
                        else
                        {
                            float value = -2.0f * t + 2.0f;

                            eased = 1.0f - (value * value * value) / 2.0f;
                        }

                        return (1.0f - eased) * a + eased * b;
                    };
        }
        if (keyPressed->code == sf::Keyboard::Key::Num8 || keyPressed->code == sf::Keyboard::Key::Numpad8){
            tween = [](float a, float b, float t) {
                        float eased = std::sin(t * PI / 2.0f);

                        return (1.0f - eased) * a + eased * b;
                    };
        }
        if (keyPressed->code == sf::Keyboard::Key::Num9 || keyPressed->code == sf::Keyboard::Key::Numpad9){
            tween = [](float a, float b, float t) {
                        float eased = std::cos(t * PI / 2.0f);
                        return (1.0f - eased) * a + eased * b;
                    };
        }

    }
}
void drawTweenGraph(sf::RenderWindow& window, float animationTime)
{
    const float left = 100.0f;
    const float right = 700.0f;
    const float top = 500.0f;
    const float bottom = 740.0f;
    const float graphWidth = right - left;
    const float graphHeight = bottom - top;

    sf::VertexArray axes(sf::PrimitiveType::Lines);
    axes.append(sf::Vertex{{left, bottom}, sf::Color::White});
    axes.append(sf::Vertex{{right, bottom}, sf::Color::White});
    axes.append(sf::Vertex{{left, bottom}, sf::Color::White});
    axes.append(sf::Vertex{{left, top}, sf::Color::White});

    window.draw(axes);

    sf::VertexArray curve(sf::PrimitiveType::LineStrip);
    constexpr int NUM_SAMPLES = 100;
    for (int i = 0; i <= NUM_SAMPLES; ++i)
    {
        float t = static_cast<float>(i)/ static_cast<float>(NUM_SAMPLES);
        float yValue = tween(0.0f, 1.0f, t);
        float x = left + t * graphWidth;
        float y = bottom - yValue * graphHeight;

        curve.append(sf::Vertex{{x, y}, sf::Color::White});
    }
    window.draw(curve);

    float currentY = tween(0.0f, 1.0f, animationTime);
    float dotX = left + animationTime * graphWidth;
    float dotY = bottom - currentY * graphHeight;

    sf::CircleShape dot(6.0f);
    dot.setFillColor(sf::Color::Red);
    dot.setPosition({dotX - 6.0f, dotY - 6.0f});
    window.draw(dot);
}

void render(sf::RenderWindow& window) {
    // Clear with blue background (sky)
    window.clear(sf::Color::Black);
    // ====== ====== ======
    // TODO: (Q1) Draw circle that moves between
    // the left/right half of the screen.
    // Movement should be governed by the tween function.
    // ====== ====== ======
    int frameInAnimation = static_cast<int>(currentFrame % FRAMES_PER_ANIMATION);

    float animationTime = static_cast<float>(frameInAnimation) / static_cast<float>(FRAMES_PER_ANIMATION - 1);
    const float leftX = 100.0f;
    const float rightX = 700.0f;
    const float y = WINDOW_HEIGHT / 3.0f;
    float x = tween(leftX, rightX, animationTime); 
    sf::CircleShape circle(25.0f);
    circle.setPosition({x - 25.0f, y - 25.0f});

    window.draw(circle);
    // ====== ====== ======
    // TODO: (Q3) Draw tween function graph with a dot
    // on the current portion of the curve
    // ====== ====== ======
    drawTweenGraph(
        window,
        animationTime
    );

    window.display();
    ++currentFrame;
}

int main() {
    sf::RenderWindow window;

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Tween");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);

        bool shouldQuit = false;
        // Main game loop
        while (window.isOpen()) {
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }
            render(window);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
