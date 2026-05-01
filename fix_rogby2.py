import re

with open("src/Rooms.cpp", "r") as f:
    text = f.read()

old_block = r"""    // Draw Riddle writing effect screen darken
    if \(showROGBYPopup && riddleIsWriting\) \{
        sf::RectangleShape darken\(sf::Vector2f\(1920\.f, 1080\.f\)\);
        darken\.setFillColor\(sf::Color\(0, 0, 0, 220\)\); // Strong darken
        window\.draw\(darken\);
        
        // Center text on screen
        sf::FloatRect textBounds = riddleScreenText\.getLocalBounds\(\);
        riddleScreenText\.setOrigin\(\{textBounds\.size\.x / 2\.0f, textBounds\.size\.y / 2\.0f\}\);
        riddleScreenText\.setPosition\(\{1920\.f / 2\.f, 1080\.f / 2\.f\}\);
        
        window\.draw\(riddleScreenText\);
    \}"""

new_block = """    // Draw Riddle writing effect screen darken
    if (showROGBYPopup && riddleIsWriting) {
        sf::RectangleShape darken(sf::Vector2f(1920.f, 1080.f));
        // Use the animated alpha for a dramatic fade instead of immediate
        darken.setFillColor(sf::Color(0, 0, 0, static_cast<unsigned char>(riddleDarkenAlpha)));
        window.draw(darken);
        
        // Only draw the text if it has content, helps avoid small dot
        if (!currentRiddleText.empty()) {
            // Center text on screen
            sf::FloatRect textBounds = riddleScreenText.getLocalBounds();
            riddleScreenText.setOrigin({textBounds.size.x / 2.0f, textBounds.size.y / 2.0f});
            riddleScreenText.setPosition({1920.f / 2.f, 1080.f / 2.f});
            
            // Fade text in alongside darkness for a cooler effect
            unsigned char textAlpha = static_cast<unsigned char>(255.f * (riddleDarkenAlpha / 220.f));
            riddleScreenText.setFillColor(sf::Color(255, 255, 255, textAlpha));
            riddleScreenText.setOutlineColor(sf::Color(0, 0, 0, textAlpha));
            
            window.draw(riddleScreenText);
        }
    }"""

text = re.sub(old_block, new_block, text, count=1, flags=re.DOTALL)

with open("src/Rooms.cpp", "w") as f:
    f.write(text)
print("Updated drawOverlay!")
