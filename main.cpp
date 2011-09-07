#include <SFML/Graphics.hpp>
#include <iostream>
#include "gui/Widget.h"
#include "Character.h"

class StatWindow : public gui::Container
{
public:
    StatWindow(const sf::FloatRect& rect, const Char& character_)
        : gui::Container(rect,new gui::HorizontalLayout()), character(character_),
        S(sf::FloatRect(0,0,rect.Right-fontsize,rect.Bottom),new gui::VerticalLayout()), V(sf::FloatRect(0,0,fontsize,rect.Bottom),new gui::VerticalLayout()),
        labelS0(sf::FloatRect(padding,padding,max_descr_length,fontsize),sf::String("Strength:",sf::Font::GetDefaultFont(),fontsize)),
        labelV0(sf::FloatRect(padding,padding,fontsize,fontsize),sf::String("",sf::Font::GetDefaultFont(),fontsize)),
        labelS1(sf::FloatRect(padding,padding,max_descr_length,fontsize),sf::String("Toughness:",sf::Font::GetDefaultFont(),fontsize)),
        labelV1(sf::FloatRect(padding,padding,fontsize,fontsize),sf::String("",sf::Font::GetDefaultFont(),fontsize)),
        labelS2(sf::FloatRect(padding,padding,max_descr_length,fontsize),sf::String("Concentration:",sf::Font::GetDefaultFont(),fontsize)),
        labelV2(sf::FloatRect(padding,padding,fontsize,fontsize),sf::String("",sf::Font::GetDefaultFont(),fontsize)),
        labelS3(sf::FloatRect(padding,padding,max_descr_length,fontsize),sf::String("Dexterity:",sf::Font::GetDefaultFont(),fontsize)),
        labelV3(sf::FloatRect(padding,padding,fontsize,fontsize),sf::String("",sf::Font::GetDefaultFont(),fontsize)),
        labelS4(sf::FloatRect(padding,padding,max_descr_length,fontsize),sf::String("Reasoning:",sf::Font::GetDefaultFont(),fontsize)),
        labelV4(sf::FloatRect(padding,padding,fontsize,fontsize),sf::String("",sf::Font::GetDefaultFont(),fontsize)),
        labelS5(sf::FloatRect(padding,padding,max_descr_length,fontsize),sf::String("Luck:",sf::Font::GetDefaultFont(),fontsize)),
        labelV5(sf::FloatRect(padding,padding,fontsize,fontsize),sf::String("",sf::Font::GetDefaultFont(),fontsize))
    {
        S.addChild(labelS0); V.addChild(labelV0);
        S.addChild(labelS1); V.addChild(labelV1);
        S.addChild(labelS2); V.addChild(labelV2);
        S.addChild(labelS3); V.addChild(labelV3);
        S.addChild(labelS4); V.addChild(labelV4);
        S.addChild(labelS5); V.addChild(labelV5);
        addChild(S);         addChild(V);
    }w

    virtual void onDraw(sf::RenderWindow& render) {
        labelV0.string.SetText(text(character.attributes.str));
        labelV1.string.SetText(text(character.attributes.tgh));
        labelV2.string.SetText(text(character.attributes.con));
        labelV3.string.SetText(text(character.attributes.dex));
        labelV4.string.SetText(text(character.attributes.rsn));
        labelV5.string.SetText(text(character.attributes.lck));
        gui::Container::onDraw(render);
    }
private:
    static const int fontsize = 15;
    static const int padding = 2;
    static const int max_descr_length = 80;
    const Char& character;
    gui::Container S,V;
    gui::Label labelS0,labelS1,labelS2,labelS3,labelS4,labelS5;
    gui::Label labelV0,labelV1,labelV2,labelV3,labelV4,labelV5;
};

int main()
{
    // Create the main window
    sf::RenderWindow App(sf::VideoMode(800, 600), "SFML window");
    gui::Container window(sf::FloatRect(0,0,App.GetWidth(),App.GetHeight()),new gui::HorizontalLayout());
    Char peter;
    StatWindow stats(sf::FloatRect(0,0,160,300),peter);
    window.addChild(stats);
    int i = 0;
    while (App.IsOpened())
    {
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            if (Event.Type == sf::Event::Closed)
                App.Close();
        }

        App.Clear();
        App.SetView(App.GetDefaultView());
        window.draw(App);
        App.Display();
    }

    return EXIT_SUCCESS;
}
