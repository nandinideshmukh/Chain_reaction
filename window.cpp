#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

const sf::Vector2f rectSize(65.0f, 65.0f);
const float WIDTH    = 600; 
const float HEIGHT  = 1000;

const float BORDER_WIDTH = 2.0f; //Outline width



int main()
{   
    vector<vector<sf::RectangleShape>> vt;

    sf::RenderWindow window(sf::VideoMode(600, 810), "Chain Reaction grid started",sf::Style::Default);
    window.setVerticalSyncEnabled(true); 
    window.setFramerateLimit(40);

    sf::RectangleShape rectangle(sf::Vector2f(450.f, 650.f));
    rectangle.setFillColor(sf::Color::Transparent); // Set the color of the rectangle
    rectangle.setOutlineColor(sf::Color::White); // Set the color of the rectangle
    rectangle.setOutlineThickness(2.f);

    // Two users
    sf::CircleShape circle(20);

    // change the size to 75 by 75
    rectangle.setPosition(75.f,75.f);

    // int m,n;
    // cout<<"How many rows you want in your game? ";
    // cin>>m;
    // cout<<"How many columns you want in your game? ";
    // cin>>n;

    for(int i=0;i<10;i++){
        vector<sf::RectangleShape> st;
        for(int j = 0;j<6;j++){

            
        sf::RectangleShape rect(rectSize);
        rect.setFillColor(sf::Color(0,0,0));
        rect.setOutlineThickness(BORDER_WIDTH);
        rect.setOutlineColor(sf::Color(0,255,255));
        st.push_back(rect);
        }
        vt.push_back(st);
    }
     
    vector<sf::CircleShape> store_circles;
    int click_count = 0;  // Add this before the main loop


    // run the program as long as the window is open
    while (window.isOpen())
    {
        
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color:: Black);

       

        for(int i=0;i<10;i++){
        for(int j = 0;j<6;j++){
        float x = (rectSize.x + BORDER_WIDTH)*i;
        float y = (rectSize.y + BORDER_WIDTH)*j;

        vt[i][j].setPosition(y+100,x+75);
        window.draw(vt[i][j]);
        }
    }

        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            // Get mouse position relative to window
            sf::Vector2i mousePos = sf::Mouse ::getPosition(window);
            circle.setPosition(mousePos.x - circle.getRadius(), mousePos.y - circle.getRadius());
            
            if(click_count%2==0)
            circle.setFillColor(sf::Color::Green);
            else 
            circle.setFillColor(sf::Color::Blue);

            circle.setPosition(mousePos.x - circle.getRadius(), mousePos.y - circle.getRadius());

            store_circles.push_back(circle);
            click_count++;
        }

       for(auto &it:store_circles)
        window.draw(it);
    
        

        window.display();
    }

    return 0;
}