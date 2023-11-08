#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace std;
using namespace sf;

const int WIDTH = 1100;
const int HEIGHT = 800;
//�߰��� �ϱ�
class Object {
public:
	int width_;
	int height_;
	RectangleShape sprite_;
	Texture texture;

	Texture* setImage(string path) {
		if (texture.loadFromFile("image/" + path)) return &texture;
	}
};


//���� ��ư �����
class Button : public Object {
public:
	Vector2i mouse_pos;

	void clickBtn() {
		if (sprite_.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {
			if (Mouse::isButtonPressed(Mouse::Left)) {

			}
		}
	}
};
//TODO : ���� ���� , �̸� �Ȱ��� ! 

int main() {
	RenderWindow  window(VideoMode(WIDTH, HEIGHT), "levelup");
	window.setFramerateLimit(15);
	//���
	Object background;
	background.sprite_.setPosition(0, 0);
	background.sprite_.setSize(Vector2f(WIDTH, HEIGHT));
	background.sprite_.setTexture(background.setImage("levelup.png"));//�̸� �ٲٱ� 
	//��ư
	Button nextBtn;
	nextBtn.sprite_.setPosition(430, 550);//��ġ
	nextBtn.sprite_.setSize(Vector2f(250, 210));//������
	nextBtn.sprite_.setTexture(nextBtn.setImage("next.png"));//�̰͵�

	Music music;
	music.openFromFile("audio/levelup.wav");
	music.play();




	while (window.isOpen())
	{
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed) {
				window.close();

			}


			nextBtn.mouse_pos = Mouse::getPosition(window);
			nextBtn.clickBtn();


		}

		window.clear();

		window.draw(background.sprite_);
		window.draw(nextBtn.sprite_);

		window.display();
	}
	return 0;
}
