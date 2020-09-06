#pragma once
#include "SFML_Tools.hpp"

namespace Sono {
	static sf::Shader button_shader;
	static sf::Color Button_Color = { 163,163,169,20 };
	static sf::Color Button_Border_Color = Button_Color;
	static sf::Color Button_Font_Color = { 202,237,255,200 };
	static sf::Color Button_Hovered_Color = { 163,163,169,80 };
	static sf::Color Button_Hovered_Border_Color = sf::Color::White;
	static sf::Color Button_Hovered_Font_Color = sf::Color::White;
	static sf::Color Button_Clicked_Color = sf::Color::Cyan;
	static sf::Color Button_Clicked_Border_Color = sf::Color::White;
	static sf::Color Button_Clicked_Font_Color = sf::Color::Black;
	static float default_border_thickness = 1;
	static sf::Vector2i m_pos = { 0,0 };
	static const bool Default = 0;
	static const bool NoBorder = 1;
	static int mouse_circle_radius = 50;
	static int default_radius = 0;
	static sf::CircleShape mouse_circle(mouse_circle_radius);
	static std::vector<sf::VertexArray> arrayBuff;
	static std::vector<sf::Text> textBuff;
	static std::vector<bool> buttonHBuff;
	bool Hovered(sf::Rect<float> rect, sf::Vector2i mouse_pos);
	bool Button(std::string text, int pos_x, int pos_y, int size_x, int size_y);
	/* bool Button(std::string text);
	 bool Button(std::string text, int pos_x, int pos_y);
	 bool Button(std::string text, sf::Vector2i pos, sf::Vector2i size);*/
	sf::Color scaleColor(sf::Color input, float r, float g, float b, float a);

	class SonoFont
	{
	public:
		SonoFont() {
			loadDefaultFonts();
		}
		// ~SonoFont();
		sf::Font& operator[](size_t index) {
			return this->Storage[index];
		}
		void loadDefaultFonts();
		void add(sf::Font font);
		void add(std::string path);
		bool del(int index);
		bool del(std::string name);
		bool setEmpty();
		bool isEmpty();

	private:
		std::vector<sf::Font> Storage;
		std::vector<std::string> names;

	};

	static SonoFont SonoFonts;

	class Window
	{
	public:
		Window(std::string, int, int);
		Window(std::string, int, int, bool);
		bool create(std::string name, int width, int height, bool style);
		void resize();
		void move();
		void update();
		float getFPS() {
			return fps;
		}
		SonoFont _Fonts;
		~Window();
		sf::Clock time, tex_time;
		sf::Event event;
		sf::RenderWindow win;
		sf::Mouse mouse;
		sf::ContextSettings settings;
		sf::Vector2i mousepos;
		sf::Vector2i windowsize;
		sf::Vector2i windowpos;
		sf::Vector2i last_mousepos;
		int width, height, grabsize = 15, min_width = 100, min_height = 100;
		float dt1 = 0.f, dt2 = 0.f, fps = 0.f;
	private:
		sf::View view;
		bool grab = false, resizex = false, resizey = false, resizecorner = false;

	};

	class RoundedRectangle
	{
	public:
		bool create(float pos_x, float pos_y, float size_x, float size_y, int round);
		RoundedRectangle() { radius = 5; }
		RoundedRectangle(float pos_x, float pos_y, float size_x, float size_y, int round) {
			create(pos_x, pos_y, size_x, size_y, round);
		}
		RoundedRectangle(float pos_x, float pos_y, float size_x, float size_y) {
			create(pos_x, pos_y, size_x, size_y, default_radius);
		}
		RoundedRectangle(sf::Vector2f position, sf::Vector2f size) {
			create(position.x, position.y, size.x, size.y, default_radius);
		}
		RoundedRectangle(sf::Vector2f position, sf::Vector2f size, int round) {
			create(position.x, position.y, size.x, size.y, round);
		}
		bool create(float pos_x, float pos_y, float size_x, float size_y) {
			return create(pos_x, pos_y, size_x, size_y, 5);
		}
		bool create(sf::Vector2f position, sf::Vector2f size, int round) {
			return create(position.x, position.y, size.x, size.y, round);
		}
		bool create(sf::Vector2f position, sf::Vector2f size) {
			return create(position.x, position.y, size.x, size.y, 5);
		}
		void setThickness(int value) {
			thick = value;
			update();
		}
		void setResolution(int value) {
			resolution = value;
			update();
		}
		void setRoundness(int value) {
			radius = value;
			update();
		}
		void setFillColor(sf::Color color) {
			FillColor = color;
			updateColors();
		}
		void setFillColor(int R, int G, int B) {
			setFillColor(sf::Color(R, G, B, 255));
			updateColors();
		}
		void setFillColor(int R, int G, int B, int A) {
			setFillColor(sf::Color(R, G, B, A));
			updateColors();
		}
		void setBorderColor(sf::Color color) {
			BorderColor = color;
			updateColors();
		}
		void setBorderColor(float R, float G, float B) {
			setBorderColor(sf::Color(R, G, B, 255));
			updateColors();
		}
		void setBorderColor(float R, float G, float B, float A) {
			setBorderColor(sf::Color(R, G, B, A));
			updateColors();
		}
		void setHoveredRadius(int radius) {
			hovered_radius = radius;
		}
		void resize(float width, float height) {
			size.x = width;
			rect = sf::Rect<float>(pos.x - (size.x / 2), pos.y - (size.y / 2), size.x, size.y);
			size.y = height;
			update();
		}
		void resize(sf::Vector2f Size) {
			size.x = Size.x;
			size.y = Size.y;
			rect = sf::Rect<float>(pos.x - (size.x / 2), pos.y - (size.y / 2), size.x, size.y);
			update();
		}
		void setRect(sf::Rect<float> n_rect) {
			rect = n_rect;
			update();
		}
		bool isHovered() { return _Hovered; }
		bool isClicked() { return _clicked; }
		float getThickness() { return thick; }
		void effects();
		void update();
		sf::Color getFillColor() { return FillColor; }
		sf::Color getBorderColor() { return BorderColor; }
		sf::Rect<float> getRect() { return rect; }
		sf::Vector2f getPosition() { return pos; }
		sf::Vector2f getSize() { return size; }
		sf::VertexArray FillShape, BorderShape;
	private:
		void updateBorders();
		void updateRectBorders();
		void updateColors();

		int radius = default_radius, hovered_radius = 50;
		int resolution = 30;
		float angle = 90.f / resolution;
		float thick = default_border_thickness;
		sf::Color FillColor = Button_Color;
		sf::Color BorderColor = FillColor;
		sf::Color HoveredBorder = Button_Hovered_Border_Color;
		sf::Color HoveredRect = Button_Hovered_Color;//scaleColor(FillColor, 1.2f, 1.2f, 1.2f, 1.f);
		sf::Vector2f pos, size;
		sf::Rect<float> rect;
		sf::Texture texture;
		bool _Hovered = false, _clicked = false, clicked_effect = false;
	};

	class rawText : public sf::Drawable, public sf::Transformable {
	public:
		sf::Text operator[](size_t index) {
			return raw_text[index];
		}
		rawText() {}
		rawText(std::string text, sf::Font font, int size);
		rawText(std::string text, sf::Font font) {
			rawText(text, font, default_size);
		}
		rawText(std::string text) {
			rawText(text, default_font, default_size);
		}
		void setString(std::string word);
		void setFont(sf::Font font);
		void setPosition(int x, int y);
		void setFontSize(int size);
		void setSize(int width, int heiht);
	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{

			// apply the entity's transform -- combine it with the one that was passed by the caller
			states.transform *= getTransform(); // getTransform() is defined by sf::Transformable
			// apply the texture
	//        states.texture = &m_texture;
			// draw the vertex array
			for (size_t i = 0; i < raw_text.size(); i++)
			{
				//target.draw(raw_text[i], states);
			}

			//target.draw(BorderShape, states);
		}
		std::vector<sf::Text> raw_text;
		sf::Vector2i position, size;
		int default_size = 12;
		sf::Font default_font;
	};

	class _Button : public RoundedRectangle {
	public:
		void crea(std::string text, float pos_x, float pos_y, float size_x, float size_y, int round);
		//_Button() { }
		_Button(std::string text, float pos_x, float pos_y, float size_x, float size_y, int round) {
			crea(text, pos_x, pos_y, size_x, size_y, round);
		}
		_Button(std::string text, float pos_x, float pos_y, float size_x, float size_y) {
			crea(text, pos_x, pos_y, size_x, size_y, default_radius);
		}
		_Button(std::string text, sf::Vector2f position, sf::Vector2f size) {
			crea(text, position.x, position.y, size.x, size.y, default_radius);
		}
		_Button(std::string text, sf::Vector2f position, sf::Vector2f size, int round) {
			crea(text, position.x, position.y, size.x, size.y, round);
		}
		sf::Text Label;
		void update();
	private:
		int font_size = 12;
		sf::Color fontColor =Button_Font_Color;
		sf::Color hoveredFontColor = Button_Hovered_Font_Color;
	};
}


