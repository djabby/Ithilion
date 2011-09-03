#include "gui/Widget.h"

namespace gui
{
    int Widget::nextID = 0;

    Widget::Widget(const sf::FloatRect& rect_)
        : rect(rect_), id(nextID++), visible(true)
    {
    }

    Widget::~Widget()
    {
    }

    void Widget::onDraw(sf::RenderWindow& render)
    {
    }

    void Widget::draw(sf::RenderWindow& render)
    {
        if(!visible) return;
        onDraw(render);
        sf::View view = render.GetView();
        view.Move(-rect.Left,-rect.Top);
        render.SetView(view);
        for(std::list<Widget*>::iterator i = childs.begin(); i != childs.end() ; ++i)
        {
            (*i)->draw(render);
        }
    }

    void Widget::addChild(Widget& widget)
    {
        childs.push_back(&widget);
    }

    void Widget::removeChild(Widget& widget)
    {
        childs.remove(&widget);
    }

    bool Widget::operator ==(const Widget& widget)
    {
        return id == widget.id;
    }
    bool Widget::operator !=(const Widget& widget)
    {
        return !(*this == widget);
    }


    Label::Label(const sf::FloatRect& rect, const sf::String& string_)
        : Widget(rect), string(string_)
    {
        string.SetPosition(sf::Vector2f(rect.Left,rect.Top));
    }
    Label::~Label()
    {
    }

    void Label::onDraw(sf::RenderWindow& render)
    {
        render.Draw(string);
    }
}
