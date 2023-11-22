// made in : ��μ�

#include "Ending.h"

void Ending::run(RenderWindow& window) {
	window.create(VideoMode(WIDTH, HEIGHT), "end");

	// object
	Object background = Object(0, 0, WIDTH, HEIGHT, "ending_background.jpg");
	Button restartBtn = Button(430, 550, 250, 210, "restartbtn.png");

	// ���� ���
	Music music;
	music.openFromFile("audio/gameover.wav");
	music.play(); 

	while (window.isOpen())
	{
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed) {
				window.close();

			}

			// �ٽ� ���� ��ư Ŭ�� ���� �� �ʿ��� ������ �� ������
			restartBtn.mousePos = Mouse::getPosition(window);
			restartBtn.clickBtn("main");

			// �ٽ� �������� ���ư���
			if (restartBtn.getNext() == 6) Main().run(window);

		}

		window.clear();

		window.draw(background.sprite_);
		window.draw(restartBtn.sprite_);

		window.display();
	}
}