// made in : ������

#include "LevelUp.h"
#include "Select.h"
#include "Game.h"
#include "HappyEnding.h"

void LevelUp::run(RenderWindow& window) {
	window.create(VideoMode(WIDTH, HEIGHT), "levelup");

	// object
	Object background = Object(0, 0, WIDTH, HEIGHT, "levelup.png");
	Button nextBtn = Button(310, 370, 300, 260, "nextbtn.png");

	// ���� ���
	Music music;
	music.openFromFile("audio/levelup.wav");
	music.play();

	LevelUp::levelUp();
	if (getLevelUp() == 3) HappyEnding().run(window);

	if (level == 2) Arrow().setSpeed(0.8f);
	else Arrow().setSpeed(0.9f);

	while (window.isOpen())
	{
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();


			// ���� �������� �Ѿ�� ��ư Ŭ�� ���� �� �ʿ��� ������ �� ������
			nextBtn.mousePos = Mouse::getPosition(window);
			nextBtn.clickBtn("start");

			// �뷡 ���� ȭ������ ��ȯ
			if (nextBtn.getNext() == 1) Select().run(window);
		}

		window.clear();

		window.draw(background.sprite_);
		window.draw(nextBtn.sprite_);

		window.display();
	}
}
