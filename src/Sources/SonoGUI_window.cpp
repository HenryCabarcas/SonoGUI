#include "SonoGUI_window.hpp"
#define TRANSPARENT_DRAWING
using namespace  Sono;

sf::Color Sono::scaleColor(sf::Color input, float r, float g, float b, float a)
{
	sf::Color output;
	output.r = input.r * r;
	output.g = input.g * g;
	output.b = input.b * b;
	output.a = input.a * a;
	return output;
}

bool Window::create(std::string name, int width, int height, bool style) {
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 8;
	settings.majorVersion = 4;
	settings.minorVersion = 0;
	if (style)
	{
		win.create(sf::VideoMode(width, height), name, sf::Style::None, settings);
	}
	else {
		win.create(sf::VideoMode(width, height), name, sf::Style::Default, settings);
	}
	win.setVerticalSyncEnabled(true);
	win.setActive(true);
	//DwmSetWindowAttribute(win.getSystemHandle(), DWMWA_CLOAK, (LPCVOID)DWMWA_CLOAK, sizeof(DWMWA_CLOAK));
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0, 0, 0, 30);
	//button_shader.loadFromFile("./src/Resources/Shaders/button_shader.frag", sf::Shader::Fragment);
	return true;
}

Window::Window(std::string name, int width, int height, bool style = NoBorder)
{
	create(name, width, height, style);
}

Window::Window(std::string name, int width, int height)
{
	create(name, width, height, NoBorder);
}

Window::~Window()
{
}

void Window::update() {
	//button_shader.setParameter("u_resolution", sf::Vector2f(win.getSize()));
	//button_shader.setParameter("u_mouse", sf::Vector2f(std::abs(int(m_pos.x)),std::abs(int(m_pos.y-win.getSize().y))));
	//button_shader.setParameter("u_time", tex_time.getElapsedTime().asSeconds());
	for (size_t i = 0; i < arrayBuff.size(); i++)
	{
		/*if(buttonHBuff[i])*/win.draw(arrayBuff[i], &button_shader);
		//else win.draw(arrayBuff[i]);
		
	}
	for (size_t i = 0; i < textBuff.size(); i++)
	{
		win.draw(textBuff[i]);
	}
	win.display();
	arrayBuff.clear();
	textBuff.clear();
	while (win.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			win.close();
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				last_mousepos = mouse.getPosition(win);
			}
		}
		if (event.type == sf::Event::MouseMoved) {
			mousepos = mouse.getPosition(win);
			if (grab && mouse.isButtonPressed(sf::Mouse::Left)) {

				//if ((mouse.getPosition(win).x != mousepos.x) || (mouse.getPosition(win).y != mousepos.y)) {
				sf::Vector2i pos;
				pos.x = mouse.getPosition().x - last_mousepos.x;
				pos.y = mouse.getPosition().y - last_mousepos.y;
				win.setPosition(pos);

				//}
			}
			else {
				grab = false;
			}
		}
		if (event.type == sf::Event::Resized)
		{
			sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			win.setView(sf::View(visibleArea));
			// adjust the viewport when the window is resized
			//glViewport(0, 0, event.size.width, event.size.height);
			win.clear(sf::Color::Transparent);

		}

	}
	//win.clear({ 0, 0, 0, 100});
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0, 0, 0);
	win.clear({ 0, 0, 0, 0 });
	//DWM_BLURBEHIND bb = { 0 };
	//bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
	//bb.fEnable = true;
	//bb.hRgnBlur = NULL;//CreateRectRgn(0, 0, -1, -1);//DWMCreateRectRgn(0, 0, -1, -1);
	//DwmEnableBlurBehindWindow(win.getSystemHandle(), &bb);
	//view.setViewport(sf::Rect<float>(0, 0, win.getSize().x, win.getSize().y));
	//win.setView(view);
	resize();
	move();
	m_pos = mouse.getPosition(win);
	fps = 1.f / time.getElapsedTime().asMilliseconds() * 1000.f;
	time.restart();
}

void Window::resize() {
	windowsize = sf::Vector2i(win.getSize());
	windowpos = win.getPosition();
	int comx = abs(INT(windowsize.x - mousepos.x));
	int comy = abs(INT(windowsize.y - mousepos.y));
	if (comx <= 5 && comy <= 5 && !grab && !resizey && !resizex && !resizecorner)
	{
		resizecorner = true;
	}
	if (comx <= 5 && !grab && !resizey && !resizecorner)
	{
		resizex = true;
	}
	else if (comy <= 5 && !grab && !resizex)
	{
		resizey = true;
	}
	if (resizecorner && mouse.isButtonPressed(sf::Mouse::Left))
	{
		win.setSize(sf::Vector2u(mouse.getPosition(win).x, mouse.getPosition(win).y));
	}
	else if (resizex && mouse.isButtonPressed(sf::Mouse::Left))
	{
		if (win.getSize().y >= min_height)
			win.setSize(sf::Vector2u(mouse.getPosition(win).x, windowsize.y));
	}
	else if (resizey && mouse.isButtonPressed(sf::Mouse::Left))
	{
		if (win.getSize().y >= min_height)
			win.setSize(sf::Vector2u(windowsize.x, mouse.getPosition(win).y));
	}
	else {
		resizecorner = false;
		resizex = false;
		resizey = false;
	}
	if (win.getSize().y < min_height) {
		win.setSize(sf::Vector2u(windowsize.x, min_height));
	}
	if (win.getSize().x < min_width) {
		win.setSize(sf::Vector2u(min_width, windowsize.y));
	}

	//glViewport(0, 0, win.getSize().x, win.getSize().y);
}

void Window::move() {
	windowpos = win.getPosition();
	windowsize = sf::Vector2i(win.getSize());
	//std::cout << mousepos.x << "|" << mousepos.y << "[ ";
	//std::cout << windowpos.x << "|" << windowpos.y <<"]"<< std::endl;
	if (mousepos.y < grabsize && !resizex && !resizey && !resizecorner && !grab)
	{
		grab = true;
	}

}

void SonoFont::loadDefaultFonts() {
	std::vector<std::string> paths = readFile("src/Resources/Fonts/FontList.txt");
	for (size_t i = 0; i < paths.size(); i++)
	{

		sf::Font temp_font;
		if (temp_font.loadFromFile(paths[i])) {
			Storage.push_back(temp_font);
			int a = paths[i].find_last_of("/");
			names.push_back(paths[i].substr(a + 1));
			std::cout << names[i] << std::endl;
		}
	}
}

bool RoundedRectangle::create(float pos_x, float pos_y, float size_x, float size_y, int round) {
	FillShape.setPrimitiveType(sf::TriangleFan);
	BorderShape.setPrimitiveType(sf::TriangleStrip);
	radius = round;
	pos = { pos_x,pos_y };
	size = { size_x,size_y };
	rect = sf::Rect<float>(pos_x - (size_x / 2), pos_y - (size_y / 2), size_x, size_y);
	texture.create(size_x, size_y);
	update();
	return true;
}

void RoundedRectangle::update() {
	BorderShape.resize(4 * (4 * radius + 8) + (4 * 12));
	if (radius == 0)
	{
		BorderShape.resize(4 * (4 * resolution + 8) + (4 * 12));
		FillShape.resize(4);
		FillShape.setPrimitiveType(sf::Quads);
		FillShape[0].position = sf::Vector2f(pos.x - (size.x / 2), pos.y - (size.y / 2));
		FillShape[1].position = sf::Vector2f(pos.x + (size.x / 2), pos.y - (size.y / 2));
		FillShape[2].position = sf::Vector2f(pos.x + (size.x / 2), pos.y + (size.y / 2));
		FillShape[3].position = sf::Vector2f(pos.x - (size.x / 2), pos.y + (size.y / 2));
		sf::Thread task01(&RoundedRectangle::updateRectBorders, this);
		task01.launch();
	}
	else {
		if (thick > 0)
		{
			sf::Thread task02(&RoundedRectangle::updateBorders, this);
			task02.launch();
		}
		FillShape.resize(4 * radius + 8);
		float x_pos, y_pos;
		angle = 90 / radius;
		for (size_t i = 0; i < FillShape.getVertexCount(); i++)
		{
			if (i < radius)
			{
				x_pos = pos.x + radius - (std::cos(angle * M_PI / 180.f) * radius) - (size.x / 2);
				y_pos = pos.y + radius - (std::sin(angle * M_PI / 180.f) * radius) - (size.y / 2);
				FillShape[i].position = sf::Vector2f(x_pos, y_pos);
				angle += 90.f / radius;
			}
			else if (i == radius)
			{
				x_pos = pos.x + radius - (size.x / 2);
				y_pos = pos.y - (size.y / 2);
				FillShape[i].position = sf::Vector2f(x_pos, y_pos);
			}
			else if (i == radius + 1) {
				x_pos = pos.x - radius + (size.x / 2);
				y_pos = pos.y - (size.y / 2);
				FillShape[i].position = sf::Vector2f(x_pos, y_pos);
				angle = (90.f / radius);
			}
			else if (i > (radius + 1) && i <= 1 + (2 * radius))
			{
				x_pos = pos.x + size.x / 2 - radius + (std::sin(angle * M_PI / 180.f) * radius);
				y_pos = pos.y + radius - (std::cos(angle * M_PI / 180.f) * radius) - (size.y / 2);
				FillShape[i].position = sf::Vector2f(x_pos, y_pos);
				angle += 90.f / radius;
			}
			else if (i == (2 * radius) + 2)
			{
				x_pos = pos.x + (size.x / 2);
				y_pos = pos.y - (size.y / 2) + radius;
				FillShape[i].position = sf::Vector2f(x_pos, y_pos);
			}
			else if (i == (2 * radius) + 3) {
				x_pos = pos.x + (size.x / 2);
				y_pos = pos.y + (size.y / 2) - radius;
				FillShape[i].position = sf::Vector2f(x_pos, y_pos);
				angle = 90.f / radius;
			}
			else if (i > (2 * radius) + 3 && i <= 3 + (3 * radius))
			{
				x_pos = pos.x - radius + (std::cos(angle * M_PI / 180.f) * radius) + (size.x / 2);
				y_pos = pos.y - radius + (std::sin(angle * M_PI / 180.f) * radius) + (size.y / 2);
				FillShape[i].position = sf::Vector2f(x_pos, y_pos);
				angle += 90.f / radius;
			}
			else if (i == (3 * radius) + 4)
			{
				x_pos = pos.x - radius + (size.x / 2);
				y_pos = pos.y + (size.y / 2);
				FillShape[i].position = sf::Vector2f(x_pos, y_pos);
			}
			else if (i == (3 * radius) + 5) {
				x_pos = pos.x + radius - (size.x / 2);
				y_pos = pos.y + (size.y / 2);
				FillShape[i].position = sf::Vector2f(x_pos, y_pos);
				angle = 90.f / radius;
			}
			else if (i > (3 * radius) + 5 && i <= 5 + (4 * radius))
			{
				x_pos = pos.x - (size.x / 2) + radius - (std::sin(angle * M_PI / 180.f) * radius);
				y_pos = pos.y - radius + (std::cos(angle * M_PI / 180.f) * radius) + (size.y / 2);
				FillShape[i].position = sf::Vector2f(x_pos, y_pos);
				angle += 90.f / radius;
			}
			else if (i == (4 * radius) + 6) {
				x_pos = pos.x - (size.x / 2);
				y_pos = pos.y - radius + (size.y / 2);
				FillShape[i].position = sf::Vector2f(x_pos, y_pos);
			}
			else if (i == (4 * radius) + 7) {
				x_pos = pos.x - (size.x / 2);
				y_pos = pos.y + radius - (size.y / 2);
				FillShape[i].position = sf::Vector2f(x_pos, y_pos);
			}
		}
	}
	sf::Thread task03(&RoundedRectangle::updateColors, this);
	task03.launch();

}

void RoundedRectangle::updateBorders() {
	float x_pos, y_pos;
	sf::Vector2f real(size.x - 2 * radius, size.y - 2 * radius);
	float longx = real.x / 12;
	float longy = real.y / 8;
	angle = 90 / (2 * radius);
	for (size_t i = 0; i < BorderShape.getVertexCount(); i += 2)
	{
		if (i < 4 * radius)
		{
			x_pos = pos.x + radius - (std::cos(angle * M_PI / 180.f) * radius) - (size.x / 2);
			y_pos = pos.y + radius - (std::sin(angle * M_PI / 180.f) * radius) - (size.y / 2);
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos + thick * std::cos(angle * M_PI / 180.f),
				y_pos + thick * std::sin(angle * M_PI / 180.f));
			angle += 90.f / (2 * radius);
		}
		else if (i == 4 * radius)
		{
			x_pos = pos.x + radius - (size.x / 2);
			y_pos = pos.y - (size.y / 2);
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos, y_pos + thick);
		}
		else if (i > 4 * radius && i < 4 * radius + 24)
		{
			x_pos = pos.x + radius - (size.x / 2);
			y_pos = pos.y - (size.y / 2);
			BorderShape[i].position = sf::Vector2f(x_pos + longx, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos + longx, y_pos + thick);
			longx += real.x / 12;
		}
		else if (i == 4 * radius + 24) {
			x_pos = pos.x - radius + (size.x / 2);
			y_pos = pos.y - (size.y / 2);
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos, y_pos + thick);
			angle = (90.f / (2 * radius));
			longx = real.x / 12;
		}
		else if (i > (4 * radius + 24) && i <= 24 + (4 * 2 * radius))
		{
			x_pos = pos.x + size.x / 2 - radius + (std::sin(angle * M_PI / 180.f) * radius);
			y_pos = pos.y + radius - (std::cos(angle * M_PI / 180.f) * radius) - (size.y / 2);
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos - thick * std::sin(angle * M_PI / 180.f),
				y_pos + thick * std::cos(angle * M_PI / 180.f));
			angle += 90.f / (radius * 2);
		}
		else if (i == (4 * 2 * radius) + 24)
		{
			x_pos = pos.x + (size.x / 2);
			y_pos = pos.y - (size.y / 2) + radius;
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos - thick, y_pos);
		}
		else if (i > (4 * 2 * radius) + 24 && i < (4 * 2 * radius) + 40)
		{
			x_pos = pos.x + (size.x / 2);
			y_pos = pos.y - (size.y / 2) + radius;
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos + longy);
			BorderShape[i + 1].position = sf::Vector2f(x_pos - thick, y_pos + longy);
			longy += real.y / 8;
		}
		else if (i == (4 * 2 * radius) + 40) {
			x_pos = pos.x + (size.x / 2);
			y_pos = pos.y + (size.y / 2) - radius;
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos - thick, y_pos);
			longy = real.y / 8;
			angle = 90.f / (2 * radius);
		}
		else if (i > (4 * 2 * radius) + 40 && i <= 40 + (4 * 3 * radius))
		{
			x_pos = pos.x - radius + (std::cos(angle * M_PI / 180.f) * radius) + (size.x / 2);
			y_pos = pos.y - radius + (std::sin(angle * M_PI / 180.f) * radius) + (size.y / 2);
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos - thick * std::cos(angle * M_PI / 180.f),
				y_pos - thick * std::sin(angle * M_PI / 180.f));
			angle += 90.f / (2 * radius);
		}
		else if (i == (4 * 3 * radius) + 40)
		{
			x_pos = pos.x - radius + (size.x / 2);
			y_pos = pos.y + (size.y / 2);
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos, y_pos - thick);
		}
		else if (i > (4 * 3 * radius) + 40 && i < (4 * 3 * radius) + 64)
		{
			x_pos = pos.x - radius + (size.x / 2);
			y_pos = pos.y + (size.y / 2);
			BorderShape[i].position = sf::Vector2f(x_pos - longx, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos - longx, y_pos - thick);
			longx += real.x / 12;
		}
		else if (i == (4 * 3 * radius) + 64) {
			x_pos = pos.x + radius - (size.x / 2);
			y_pos = pos.y + (size.y / 2);
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos, y_pos - thick);
			longx = real.x / 12;
			angle = 90.f / (2 * radius);
		}
		else if (i > (4 * 3 * radius) + 64 && i <= 64 + (4 * 4 * radius))
		{
			x_pos = pos.x - (size.x / 2) + radius - (std::sin(angle * M_PI / 180.f) * radius);
			y_pos = pos.y - radius + (std::cos(angle * M_PI / 180.f) * radius) + (size.y / 2);
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos + thick * std::sin(angle * M_PI / 180.f),
				y_pos - thick * std::cos(angle * M_PI / 180.f));
			angle += 90.f / (2 * radius);
		}
		else if (i == (4 * 4 * radius) + 64) {
			x_pos = pos.x - (size.x / 2);
			y_pos = pos.y - radius + (size.y / 2);
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos + thick, y_pos);
		}
		else if (i > (4 * 4 * radius) + 64 && i < (4 * 4 * radius) + 78) {
			x_pos = pos.x - (size.x / 2);
			y_pos = pos.y - radius + (size.y / 2);
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos - longy);
			BorderShape[i + 1].position = sf::Vector2f(x_pos + thick, y_pos - longy);
			longy += real.y / 8;
		}
		else if (i == (4 * 4 * radius) + 78) {
			x_pos = pos.x - (size.x / 2);
			y_pos = pos.y + radius - (size.y / 2);
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos - 2);
			BorderShape[i + 1].position = sf::Vector2f(x_pos + thick, y_pos - 2);
		}
	}
}

void RoundedRectangle::updateRectBorders() {
	float x_pos, y_pos;
	int portion = BorderShape.getVertexCount() / 4;
	float longc = thick, dx = (size.x - thick) / portion, dy = (size.y - thick) / portion;
	for (int i = 0; i < BorderShape.getVertexCount(); i += 2)
	{
		if (i < portion) {
			x_pos = rect.left + longc;
			y_pos = rect.top;
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos, y_pos + thick);
			x_pos = 0, y_pos = 0;
			longc += 2 * dx;
		}
		else if (i == portion)
		{
			x_pos = rect.left + rect.width;
			y_pos = rect.top;
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos, y_pos + thick);
			longc = thick;
		}
		else if (i > portion && i < (2 * portion)) {
			x_pos = rect.left + rect.width;
			y_pos = rect.top + longc;
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos - thick, y_pos);
			x_pos = 0, y_pos = 0;
			longc += 2 * dy;
		}
		else if (i == 2 * portion)
		{
			x_pos = rect.left + rect.width;
			y_pos = rect.top + rect.height;
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos - thick, y_pos);
			longc = thick;
		}
		else if (i > 2 * portion && i < 3 * portion) {
			x_pos = rect.left + rect.width - longc;
			y_pos = rect.top + rect.height;
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos, y_pos - thick);
			x_pos = 0, y_pos = 0;
			longc += 2 * dx;
		}
		else if (i == 3 * portion)
		{
			x_pos = rect.left;
			y_pos = rect.top + rect.height;
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos, y_pos - thick);
			longc = thick;
		}
		else if (i > 3 * portion && i < 4 * portion - 2) {
			x_pos = rect.left;
			y_pos = rect.top + rect.height - longc;
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos + thick, y_pos);
			x_pos = 0, y_pos = 0;
			longc += 2 * dy;
		}
		else if (i == 4 * portion - 2)
		{
			x_pos = rect.left;
			y_pos = rect.top;
			BorderShape[i].position = sf::Vector2f(x_pos, y_pos);
			BorderShape[i + 1].position = sf::Vector2f(x_pos + thick, y_pos);
			x_pos = 0, y_pos = 0;
		}
	}
}

void RoundedRectangle::updateColors() {
	if (radius == 0)
	{
		FillShape[0].color = sf::Color::Transparent;
		FillShape[1].color = sf::Color::Transparent;
		FillShape[2].color = sf::Color::Transparent;
		FillShape[3].color = sf::Color::Transparent;
	}
	else {
		for (size_t i = 0; i < FillShape.getVertexCount(); i++)
		{
			FillShape[i].color = FillColor;
		}
	}
	if (thick > 0)
	{
		for (size_t i = 0; i < BorderShape.getVertexCount(); i += 2)
		{
			BorderShape[i].color = BorderColor;
			BorderShape[i + 1].color = BorderColor;
		}
	}
}

void RoundedRectangle::effects() {
	_Hovered = Hovered(rect, m_pos);
	size_t _j = FillShape.getVertexCount(), j = 0;
	if (_Hovered)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			_clicked = true;
			for (size_t i = 0; i < BorderShape.getVertexCount(); i++) {
				BorderShape[i].color = HoveredBorder;
				if (j < _j) {
					FillShape[j].color = HoveredRect;
					j++;
				}
			}
		}
		else {
			_clicked = false;
			j = 0;
			/*for (size_t i = 0; i < BorderShape.getVertexCount(); i++) {
				tempc->BorderShape[i].color = HoveredBorder;
				if (j < _j) {
					tempc->FillShape[j].color = HoveredRect;
					j++;
				}
			}*/
		}
	}
	/*else*/ {
		j = 0;
		for (size_t i = 0; i < BorderShape.getVertexCount(); i++)
		{
			if (j < _j) {
				FillShape[j].color = FillColor;
				j++;
			}
			float varx = BorderShape[i].position.x - m_pos.x;
			float vary = BorderShape[i].position.y - m_pos.y;

			int dist = sqrt((varx * varx) + (vary * vary));
			if (dist <= hovered_radius)
			{
				BorderShape[i].color = HoveredBorder;
				int alp = 9.f * HoveredBorder.a / dist;
				//std::cout << alp << std::endl;
				if (alp > 250 || alp < 0)alp = 250;
				BorderShape[i].color.a = alp;
				if (_Hovered) {
					float y_dis = BorderShape[i].position.y - m_pos.y;
					float x_dis = BorderShape[i].position.x - m_pos.x;
					float new_pos = 15.f / (dist);
					if (new_pos > (rect.width / rect.height))new_pos = (rect.width / rect.height);
					if (abs(y_dis) > thick) {
						if (y_dis > 0) {
							BorderShape[i].position.y += new_pos;
						}
						else {
							BorderShape[i].position.y -= new_pos;
						}
					}
					if (abs(x_dis) > thick) {
						if (x_dis > 0) {
							BorderShape[i].position.x += new_pos;
						}
						else {
							BorderShape[i].position.x -= new_pos;
						}
					}

				}
				//tempc->BorderShape[i].color.a *= hovered_radius / dist;
			}
			else {
				/*if (tempc->_Hovered) {
					tempc->BorderShape[i].color = HoveredRect;
				}
				else {*/
				BorderShape[i].color = BorderColor;
				//}
			}
			i++;
		}
	}


}

bool Sono::Hovered(sf::Rect<float> rect, sf::Vector2i mouse_pos) {
	if (mouse_pos.x >= rect.left && mouse_pos.x <= (rect.left + rect.width)
		&& mouse_pos.y >= rect.top && mouse_pos.y <= (rect.top + rect.height))
	{
		return true;
	}
	return false;
}

rawText::rawText(std::string text, sf::Font font, int size) {
	setString(text);
	setFont(font);
	setFontSize(size);
}

void rawText::setString(std::string word) {
	sf::Text temp_text;
	for (size_t i = 0; i < word.size(); i++)
	{
		temp_text.setString(word[i]);
		raw_text.push_back(temp_text);
	}
}

void rawText::setFont(sf::Font font) {
	for (size_t i = 0; i < raw_text.size(); i++)
	{
		raw_text[i].setFont(font);
	}
}

void rawText::setFontSize(int size) {
	for (size_t i = 0; i < raw_text.size(); i++)
	{
		raw_text[i].setCharacterSize(size);
	}
}

void _Button::crea(std::string text, float pos_x, float pos_y, float size_x, float size_y, int round) {
	create(pos_x, pos_y, size_x, size_y, round);
	Label.setString(text);
	Label.setFont(SonoFonts[0]);
	Label.setFillColor(fontColor);
	Label.setCharacterSize(font_size);
	float x = Label.getGlobalBounds().width, y = Label.getGlobalBounds().height;
	Label.setOrigin(sf::Vector2f(x / 2, y / 2));
	Label.setPosition(pos_x, pos_y);
}

void _Button::update() {
	if (getThickness() > 0) effects();
	if (isHovered()) {
		//fontColor = DefaultHoveredFontColor;
		Label.setFillColor(hoveredFontColor);
		if (isClicked())
		{
			setFillColor(Button_Clicked_Color);
			setBorderColor(Button_Clicked_Border_Color);
			Label.setColor(Button_Clicked_Font_Color);
		}
	}
	else {
		//fontColor = DefaultFontColor;
		Label.setColor(fontColor);
	}

}

bool Sono::Button(std::string text)
{
	_Button tempb(text, sf::Vector2f(100, 200), Button_Size);
	tempb.update();
	
	{ buttonHBuff.push_back(true); buttonHBuff.push_back(true); }
	//else { buttonHBuff.push_back(false); buttonHBuff.push_back(false); }
	
	arrayBuff.push_back(tempb.FillShape);
	arrayBuff.push_back(tempb.BorderShape);
	textBuff.push_back(tempb.Label);
	return tempb.isClicked();
}
