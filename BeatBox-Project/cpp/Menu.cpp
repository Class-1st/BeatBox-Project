#include "Menu.h"

void Menu::run(RenderWindow& window) {
	window.create(VideoMode(WIDTH, HEIGHT), "menu");

	Object background = Object(0, 0, WIDTH, HEIGHT, "menu_background.png");
	Button mainBtn = Button(330, 410, 250, 200, "startbtn.png");

	while (window.isOpen())
	{
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();

			mainBtn.mousePos = Mouse::getPosition(window);

			mainBtn.clickBtn("main");
			if (mainBtn.getNext() == 6) {
				Main().run(window);
			}
		}

		window.clear();

		window.draw(background.sprite_);
		window.draw(mainBtn.sprite_);

		window.display();
	}
}