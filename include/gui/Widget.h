#ifndef GUI_WIDGET_H
#define GUI_WIDGET_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
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
    protected:
        int id;
        static int nextID;
    };

    class Layout
    {
    public:
        // Standard layout, does nothing
        virtual sf::View adjust(const sf::View& view, const Widget& widget) const
        {
            return view;
        }
    };

    /* AABBCC
       AABBCC
       AABBCC */
    class HorizontalLayout : public Layout
    {
    public:
        virtual sf::View adjust(const sf::View& view_, const Widget& widget) const
        {
            sf::View view = view_;
            view.Move(-widget.rect.Right,0);
            return view;
        }
    };

    /* AAAAAA
       BBBBBB
       CCCCCC */
    class VerticalLayout : public Layout
    {
    public:
        virtual sf::View adjust(const sf::View& view_, const Widget& widget) const
        {
            sf::View view = view_;
            view.Move(0,-widget.rect.Bottom);
            return view;
        }
    };


    class Container : public Widget
    {
    public:
        Container(const sf::FloatRect& rect_, const Layout* layout = new Layout());
        virtual ~Container();

        virtual void onDraw(sf::RenderWindow& render);

        void addChild(Widget& widget);
        void removeChild(Widget& widget);
    private:
        std::list<Widget*> childs;
        const Layout* layout;
    };

    class Label : public Widget
    {
    public:
        Label(const sf::FloatRect& rect, const sf::String& string_ = sf::String());
        virtual ~Label();

        virtual void onDraw(sf::RenderWindow& render);

        sf::String string;
    };

    class Button : public Widget
    {
    public:
        Button(const sf::FloatRect& rect);
        virtual ~Button();

        virtual void onDraw(sf::RenderWindow& render);
    };
}

template <class T>
std::string text(const T& t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}
template <class T, class U>
std::string text(const T& t, const U& u) {
    std::stringstream ss;
    ss << t << u;
    return ss.str();
}


template <class T, class U, class V>
std::string text(const T& t, const U& u, const V& v) {
    std::stringstream ss;
    ss << t << u << v;
    return ss.str();
}

#endif
