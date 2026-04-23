```cpp

#include <SFML/Graphics.hpp> //
#include <SFML/Window.hpp> // sfml libraries for working with Window



int main(){


//----------------------- INITIALIZE ------------------------------
//This happens once at the very start. 
//This is where you create your window and set up your initial settings

//Rendering the Window

sf::RenderWindow "name"(sf::VideoMode({width, height}), "title");
//----------------------- INITIALIZE ------------------------------



//-------------------------- LOAD ---------------------------------
//Also happens once before the loop starts. 
//You load your textures, sounds, and fonts from your hard drive into the computer's RAM.


//-------------------------- LOAD ---------------------------------




//-------------------------- GAME LOOP ---------------------------------

// -------------------------- UPDATE ---------------------------------



/* This is the first part of the Loop.
It’s where "the brain" of the game happens.
->Events: Poll the window to see if the user clicked "Close" or pressed a key.
->Logic: Move your characters, check for collisions, or update scores. */

//----------- UPDATE EVENT-------------

while(window.isOpen()){

    while (const std::optional event = window.pollEvent()) // the ear
        {
            if (event -> is <sf::Event::Closed>())
                window.close();
        }

//----------- UPDATE EVENT-------------

//----------- UPDATE LOGIC-------------
//logic here
//----------- UPDATE LOGIC-------------

// -------------------------- UPDATE ---------------------------------

//-------------------------- DRAW ---------------------------------
/* The final part of the Loop. 
This is where you clear the old frame and paint the new one. 
In SFML, this always follows a strict 
Clear -> Draw -> Display pattern. */

window.clear(sf::Color::Black); // CLEAR
window.draw(shape); // DRAW
window.display(); // DISPLAY

//-------------------------- DRAW ---------------------------------
}

//-------------------------- GAME LOOP ---------------------------------


return 0;

}


