#pragma once

#include "Main.h"
#include<deque>
#include<time.h>

class Game {
public:
	void run(RenderWindow& window);
};

// ȭ���� ������ ��� ����ü
struct ArrowShape
{
	int direction; // ȭ���� ������ ��Ÿ���� �� (0: UP, 1: DOWN, 2: LEFT, 3: RIGHT)
	int x; // ȭ���� x ��ǥ
	int y; // ȭ���� y ��ǥ
};

// ȭ�� Ŭ���� ����
class Arrow
{
private:
	enum Dir
	{
		UP = 0,
		DOWN,
		LEFT,
		RIGHT,
	};

	Texture t; // ȭ�� �̹����� �ؽ�ó
	Sprite s; // ȭ�� �̹����� ��������Ʈ
	deque<ArrowShape> questions; // ȭ���� ������ ��� �� (ť)

	float frame; // ȭ�� �ִϸ��̼� ������
	float frameSpeed; // ȭ�� �ִϸ��̼� �ӵ�
	int frameLimit; // ȭ�� �ִϸ��̼� ������ ����
	int dir[4] = { -90, 90, 180, 0 }; // ȭ�� ���⿡ ���� ����

	// ���� �Էµ� Ű�� �������� Ȯ���ϴ� �Լ�
	bool CheckArrow(int idx)
	{
		if (questions.empty()) return false;
		if (idx == questions[0].direction) return true;

		return false;
	}

public:
	// ������
	Arrow()
	{
		srand(static_cast<unsigned int>(time(nullptr)));

		t.loadFromFile("image/right.png");
		s = Sprite(t);
		Rect<float> rect = s.getGlobalBounds();
		s.setOrigin(Vector2f(rect.width / 2, rect.height / 2));
		frame = 20.f;
		frameSpeed = 0.6f;
		frameLimit = 20;
	}

	// �Ҹ���
	~Arrow() {}

	// �ʱ�ȭ �Լ�
	void Init() { questions.clear(); }

	// Ű �Է��� Ȯ���ϰ� ���� ���θ� ��ȯ�ϴ� �Լ�
	bool CheckKey()
	{
		if (Keyboard::isKeyPressed(Keyboard::Up)) return CheckArrow(Dir::UP);
		else if (Keyboard::isKeyPressed(Keyboard::Down)) return CheckArrow(Dir::DOWN);
		else if (Keyboard::isKeyPressed(Keyboard::Left)) return CheckArrow(Dir::LEFT);
		else if (Keyboard::isKeyPressed(Keyboard::Right)) return CheckArrow(Dir::RIGHT);

		return false;
	}

	// ȭ���� ���� �� �ִϸ��̼� ó�� �Լ�
	void ArrowSetting()
	{
		// �ִϸ��̼� ������ üũ
		frame += frameSpeed;
		if (frame <= frameLimit) return;
		frame -= frameLimit;

		// ȭ�� ���� üũ
		if (questions.size() > 10) return;
		ArrowShape as;

		// �������� ȭ���� ���� ����
		int dirIdx = rand() % 4;
		as.direction = dirIdx;
		as.x = WIDTH;
		as.y = 100;
		s.setPosition(as.x, as.y);
		questions.push_back(as);
	}

	// ȭ�� �̵� �Լ�
	void Move(const int& speed, bool& isGameOver)
	{
		//��ó�� ȭ��ǥ�� ������� ������ ��Ÿ�����ϱ⋚���� ����� ��
		deque<ArrowShape>::iterator iter;
		for (iter = questions.begin(); iter != questions.end(); ++iter)
		{
			iter->x -= speed;
			if (iter->x <= 0)
			{
				// ���� ����
				isGameOver = true;
				break;
			}
		}
	}

	// ȭ�� �׸��� �Լ�
	void Draw(RenderWindow& _window)
	{
		for (auto iter = questions.begin(); iter != questions.end(); ++iter)
		{
			s.setRotation(dir[iter->direction]);
			s.setPosition(iter->x, iter->y);
			_window.draw(s);
		}
	}

	// ȭ���� ��ġ ���� ��ȯ �Լ�
	ArrowShape GetFirstArrowPos() { return questions[0]; }

	// ȭ�� ���� �Լ�
	void RemoveFirstArrow()
	{
		if (questions.empty()) return;
		questions.pop_front();
	}

	// ȭ���� ����ִ��� ���� ��ȯ �Լ�
	bool IsEmpty() const { return questions.empty(); }
};

// ���� �Ŵ��� Ŭ���� ����
class GameManager
{
private:
	enum GameState
	{
		NONE,
		CORRECT,
		WRONG,
	};

	int score;
	int health;
	int arrowSpeed;
	bool isGameOver;
	Arrow* pArrow;

	Texture tw, tw2, tw3, tw4;
	Texture tl;
	Sprite imgWrong;
	Sprite imgDeadLine;
	GameState state;
	ArrowShape firstArrowPos;

public:
	GameManager()
	{
		score = 0;
		health = 3;
		arrowSpeed = 10;
		isGameOver = false;
		pArrow = new Arrow();
		state = NONE;

		// �ؽ�ó �� ��������Ʈ �ʱ�ȭ
		tw.loadFromFile("image/wrong.png");
		tl.loadFromFile("image/player.png");
		imgWrong = Sprite(tw);
		imgDeadLine = Sprite(tl);

		imgDeadLine = Sprite(tl);
		imgDeadLine.setPosition(0, 45);

		Rect<float> rect = imgWrong.getGlobalBounds();
		imgWrong.setOrigin(Vector2f(rect.width / 2, rect.height / 2));
	}

	void IncreaseScore() { score++; }// ���� ���� �Լ�
	void DecreaseHealth() { health--; }// ü�� ���� �Լ�
	
	int GetHealth() const { return health; }// ���� ü�� ��ȯ �Լ�
	int GetScore() const { return score; }// ���� ���� ��ȯ �Լ�

	// �ʱ�ȭ �Լ�
	void Init() { pArrow->Init(); }

	// Ű �Է� Ȯ�� �Լ�
	void CheckKey()
	{
		if (pArrow->IsEmpty()) return;

		if (pArrow->CheckKey())
		{
			// ������ ���
			firstArrowPos = pArrow->GetFirstArrowPos();
			pArrow->RemoveFirstArrow();
			state = CORRECT;
		}

		else
		{
			// ������ ���
			firstArrowPos = pArrow->GetFirstArrowPos();
			pArrow->RemoveFirstArrow();
			state = WRONG;
		}
	}
	// ȭ�� �̵� �Լ�
	void Move()
	{
		pArrow->ArrowSetting();
		pArrow->Move(arrowSpeed, isGameOver);
	}

	void Draw(RenderWindow& window)
	{
		switch (state)
		{
		case WRONG:
			// ���� �� ���� ����
			state = NONE;
			imgWrong.setPosition(firstArrowPos.x, firstArrowPos.y);
			window.draw(imgWrong);
			isGameOver = true;
			DecreaseHealth();
			break;
		case CORRECT:
			// ���� �� ���� ����
			IncreaseScore();
			state = NONE;
			break;
		}
		pArrow->Draw(window);

		window.draw(imgDeadLine);
	}

	void CheckGameOver()
	{
		if (GetHealth() <= 0)
		{
			// ü���� 0 ������ ���
			sleep(seconds(3));
			isGameOver = true;
			Init(); // ���� �����
		}

		else if (isGameOver)
		{
			// ���� ���� �� 3�� ��� �� ���� �����
			sleep(seconds(3));
			isGameOver = false;
			Init(); // ���� �����
		}
	}
};