#include "Select.h"
#include "Game.h"

Music music1;
String song[] = { "Antifreeze", "Steal The Show", "Every Summertime", "OMG", "Rush", "All I Want For Christmas Is You", "Rabbit Dance" };
TextString title = TextString(0, 100, 30, song[0]);
Button leftBtn = Button(50, 170, 200, 200, "left.png");
Button rightBtn = Button(650, 170, 200, 200, "right.png");
Object album = Object(320, 160, 280, 280, "1.png");

float Arrow::frameSpeed = 0.6f;

void Select::run(RenderWindow& window) {
	window.create(VideoMode(WIDTH, HEIGHT), "Choose Level");

	// object
	Object background = Object(0, 0, WIDTH, HEIGHT, "select_background.jpg");
	Button gameBtn = Button(330, 410, 250, 200, "startbtn.png");

	// ���� ���
	music1.openFromFile("audio/1.wav");
	music1.play();

	while (window.isOpen())
	{
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();

			// ���� / ������ ��ư Ŭ�� ���� �� �ʿ��� ������ �� ������
			leftBtn.mousePos = Mouse::getPosition(window);
			rightBtn.mousePos = Mouse::getPosition(window);

			// �뷡 ���� ��ư Ŭ�� ���� �� �ʿ��� ������ �� ������
			gameBtn.mousePos = Mouse::getPosition(window);

			leftBtn.clickBtn("left");
			rightBtn.clickBtn("right");
			gameBtn.clickBtn("game");

			// ���� ��ư ������ ����������
			if (leftBtn.getNext() == 3) nextSong(-1);

			// ������ ��ư ������ ����������
			else if (rightBtn.getNext() == 4) nextSong(1);

			// �뷡 ���� Ŭ�� �� ���� ȭ������
			else if (gameBtn.getNext() == 5) {
				playSong();
				Game().run(window);
			}
		}

		window.clear();

		window.draw(background.sprite_);
		window.draw(gameBtn.sprite_);
		window.draw(leftBtn.sprite_);
		window.draw(rightBtn.sprite_);
		window.draw(album.sprite_);
		window.draw(title.text);

		window.display();
	}
}

// ���� / ���� ��
void Select::nextSong(int next) {

	// �������� ���� ��� ���������� ���ư���
	if (nextAlbum + next < 1) {
		leftBtn.isNext = 0;
		nextAlbum = 8;
	}

	// �������� ���� ��� ó������ ���ư���
	else if (nextAlbum + next > 7) {
		rightBtn.isNext = 0;
		nextAlbum = 0;
	}

	nextAlbum += next;

	title.setText(0, 100, song[nextAlbum - 1]);

	album.sprite_.setTexture(album.setImage(to_string(nextAlbum) + ".png"));
	leftBtn.setNext(0);
	rightBtn.setNext(0);
	playSong();
}

void Select::playSong() {
	music1.stop();
	music1.openFromFile("audio/" + to_string(nextAlbum) + ".wav");
	music1.play();
}

void Select::stopSong() {
	music1.stop();
}