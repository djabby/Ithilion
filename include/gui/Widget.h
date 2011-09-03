#ifndef GUI_WIDGET_H
#define GUI_WIDGET_H

#include <SFML/Graphics.hpp>
#include <list>
#include <sstream>

namespace gui
{
    class Widget
    {
    public:
        Widget(const sf::FloatRect& rect_);
        virtual ~Widget();

        virtual void onDraw(sf::RenderWindow& render);
        void draw(sf::RenderWindow& render);

        void addChild(Widget& widget);
        void removeChild(Widget& widget);

        sf::FloatRect rect;
        bool visible;

        bool operator ==(const Widget& widget);
        bool operator !=(const Widget& widget);
    private:
        int id;
        static int nextID;
        std::list<Widget*> childs;
    };

    class Label : public Widget
    {
    public:
        Label(const sf::FloatRect& rect, const sf::String& string_);
        virtual ~Label();

        virtual void onDraw(sf::RenderWindow& render);

        sf::String string;
    };
}

namespace text
{
    template <class T, class U>
    std::string add(const T& t, const U& u) {
        std::stringstream ss;
        ss << t << u;
        return ss.str();
    }
}

#endif
