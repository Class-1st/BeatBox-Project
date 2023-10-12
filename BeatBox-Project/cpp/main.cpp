#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

const int WIDTH = 900;
const int HEIGHT = 600;
int isNext = 0;

class Object {
public:
	int width_;
	int height_;
	RectangleShape sprite_;
	Texture texture;

	Object(int x, int y, int width, int height, String path) {
		sprite_.setPosition(x, y);
		sprite_.setSize(Vector2f(width, height));
		sprite_.setTexture(setImage(path));
	}

	Texture* setImage(string path) {
		if (texture.loadFromFile("image/" + path)) return &texture;
	}
};

class Button : public Object {
public:
	Vector2i mousePos;

	Button(int x, int y, int width, int height, String path) : Object(x, y, width, height, path) {}

	void clickBtn(String str) {
		if (sprite_.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
			if (Mouse::isButtonPressed(Mouse::Left)) {
				if (str == "start") isNext = 1;
				else isNext = 2;
			}
		}
	}
};

int main() {
	RenderWindow  window(VideoMode(WIDTH, HEIGHT), "MusicTokTok");
	window.setFramerateLimit(15);

	Object background = Object(0, 0, WIDTH, HEIGHT, "main_background.jpg");
	
	Button startBtn = Button(120, 370, 300, 260, "startbtn.png");
	Button explainBtn = Button(500, 370, 300, 260, "explainbtn.png");
	
	Music music;
	music.openFromFile("audio/happy.wav");
	music.play();

	while (window.isOpen())
	{
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();

			startBtn.mousePos = Mouse::getPosition(window);
			explainBtn.mousePos = Mouse::getPosition(window);

			startBtn.clickBtn("start");
			explainBtn.clickBtn("explain");

		}

		window.clear();

		window.draw(background.sprite_);
		window.draw(startBtn.sprite_);
		window.draw(explainBtn.sprite_);

		window.display();
	}

	return 0;
}