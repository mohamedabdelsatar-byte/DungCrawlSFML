import sys

with open('src/Rooms.cpp', 'r') as f:
    text = f.read()

# Fix the rendering logic
old_str = """    // Draw Riddle writing effect screen darken
    if (showRiddlePopup && riddleIsWriting) {
        sf::RectangleShape darken(sf::Vector2f(1920.f, 1080.f));
        darken.setFillColor(sf::Color(0, 0, 0, 220)); // Strong darken
        window.draw(darken);
        
        // Center text on screen
        sf::FloatRect textBounds = riddleScreenText.getLocalBounds();
        riddleScreenText.setOrigin({textBounds.size.x / 2.0f, textBounds.size.y / 2.0f});
        riddleScreenText.setPosition({1920.f / 2.f, 1080.f / 2.f});
        
        window.draw(riddleScreenText);
    }"""

new_str = """    // Draw Riddle writing effect screen darken
    if (showROGBYPopup && riddleIsWriting) {
        sf::RectangleShape darken(sf::Vector2f(1920.f, 1080.f));
        darken.setFillColor(sf::Color(0, 0, 0, 220)); // Strong darken
        window.draw(darken);
        
        // Center text on screen
        sf::FloatRect textBounds = riddleScreenText.getLocalBounds();
        riddleScreenText.setOrigin({textBounds.size.x / 2.0f, textBounds.size.y / 2.0f});
        riddleScreenText.setPosition({1920.f / 2.f, 1080.f / 2.f});
        
        window.draw(riddleScreenText);
    }"""

if old_str in text:
    with open('src/Rooms.cpp', 'w') as f:
        f.write(text.replace(old_str, new_str))
    print("Success rendering fix")
else:
    print("Fail: Rendering logic string not found")

