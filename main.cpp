#include <SFML/Graphics.hpp>
#include "gui/Widget.h"

class StatWindow : public gui::Widget
{
public:
};

int main()
{
    // Create the main window
    sf::RenderWindow App(sf::VideoMode(800, 600), "SFML window");
    gui::Widget window(sf::FloatRect(200,0,App.GetWidth(),App.GetHeight()));
    gui::Label label(sf::FloatRect(-150,0,200,40),sf::String("Hallo"));
    window.addChild(label);
    // Load a sprite to display
    sf::Image Image;
    if (!Image.LoadFromFile("cb.bmp"))
        return EXIT_FAILURE;
    sf::Sprite Sprite(Image);
    int i = 0;
	// Start the game loop
    while (App.IsOpened())
    {
        // Process events
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();
        }

        App.Clear();
        //App.Draw(Sprite);
        App.SetView(App.GetDefaultView());
        label.string.SetText(text::add((std::string)sf::Unicode::Text("Nummer: "),i++));
        window.draw(App);
        App.Display();
    }

    return EXIT_SUCCESS;
}
