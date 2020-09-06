#include <SonoGUI_window.hpp>

int main() {
    Sono::Window mainw("Main", 600, 400,1);
    //mainw.win.setVerticalSyncEnabled(false);
    sf::CircleShape shape(50.f);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(sf::Vector2f(200.f, 200.f));
    sf::Font tt = mainw._Fonts[2];
    sf::Text tex;
    tex.setFont(mainw._Fonts[0]);
    
    sf::FloatRect bound = tex.getGlobalBounds();
    //tex.setOrigin((bound.width / 2),(bound.height / 2));
    tex.setPosition(sf::Vector2f(10, 10));
    
    sf::BlendMode hh=sf::BlendAdd;
    hh.colorEquation = sf::BlendMode::Subtract;
    //hh.colorDstFactor = sf::BlendMode::Factor::OneMinusSrcColor;
      //  hh.alphaEquation = sf::BlendMode::ReverseSubtract;
    std::string d = "START";
    while (mainw.win.isOpen())
    {
        //mainw.win.draw(shape);
        std::string tt = std::to_string(mainw.getFPS());
        tex.setString(tt);
        Sono::Button(d, 350, 100, 100, 35);
        Sono::Button("m",190, 100, 100, 35);
        mainw.win.draw(tex);
        mainw.update();
    }

    return 0;
}