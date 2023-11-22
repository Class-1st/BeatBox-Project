#include "Game.h"
#include<deque>
#include<time.h>

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

private:

	// ���� �Էµ� Ű�� �������� Ȯ���ϴ� �Լ�
	bool CheckArrow(int idx)
	{
		if (questions.empty()) return false;
		if (idx == questions[0].direction)
		{
			
			return true;
		}
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
	void Init()
	{
		questions.clear();
	}

	// Ű �Է��� Ȯ���ϰ� ���� ���θ� ��ȯ�ϴ� �Լ�
	bool CheckKey()
	{
		if (Keyboard::isKeyPressed(Keyboard::Up))
			return CheckArrow(Dir::UP);
		
		if (Keyboard::isKeyPressed(Keyboard::Down))
			return CheckArrow(Dir::DOWN);
		
		if (Keyboard::isKeyPressed(Keyboard::Left))
			return CheckArrow(Dir::LEFT);
		
		if (Keyboard::isKeyPressed(Keyboard::Right))
			return CheckArrow(Dir::RIGHT);
		
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
	{//��ó�� ȭ��ǥ�� ������� ������ ��Ÿ�����ϱ⋚���� ����� ��
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
	ArrowShape GetFirstArrowPos()
	{
		return questions[0];
	}

	// ȭ�� ���� �Լ�
	void RemoveFirstArrow()
	{
		if (questions.empty()) return;
		questions.pop_front();
	}

	// ȭ���� ����ִ��� ���� ��ȯ �Լ�
	bool IsEmpty() const
	{
		return questions.empty();
	}
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
	void IncreaseScore()
	{
		score++;
	}
	void DecreaseHealth()
	{
		health--;
	}
	int GetHealth() const
	{
		return health;
	}

	int GetScore() const
	{
		return score;
	}
	// �ʱ�ȭ �Լ�
	void Init()
	{
		pArrow->Init();
	}

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
			//game over.
			state = NONE;
			imgWrong.setPosition(firstArrowPos.x, firstArrowPos.y);
			window.draw(imgWrong);
			isGameOver = true;
			DecreaseHealth();
			break;
		case CORRECT:
			//score up.
			IncreaseScore();
			state = NONE;
			break;
		}
		pArrow->Draw(window);

		window.draw(imgDeadLine);
	}
	void CheckGameOver()
	{
		if(GetHealth() <= 0)
		{
			
			sleep(seconds(3));
			isGameOver = true;
			Init(); // Reset the game
		}
		else if (isGameOver)
		{
			sleep(seconds(3));
			isGameOver = false;
			Init(); // Reset the game
			}
	}
};


void Game::run(RenderWindow& window)
{
	Font font;
	font.loadFromFile("C:\\windows\\Fonts\\Arial.ttf");

	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setPosition(10, 10);
	scoreText.setFillColor(Color::White);

	Text healthText;
	healthText.setFont(font);
	healthText.setCharacterSize(24);
	healthText.setPosition(WIDTH - 150, 10);
	healthText.setFillColor(Color::White);

	window.create(VideoMode(WIDTH, HEIGHT), "game");
	window.setFramerateLimit(30);

	GameManager* pGameMgr = new GameManager();
	pGameMgr->Init();

	while (window.isOpen())
	{
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				window.close();
			}
			if (e.type == Event::KeyPressed)
			{
				pGameMgr->CheckKey();
			}
		}
		//logic.
		pGameMgr->Move();

		//draw.
		window.clear(Color::Black);
		pGameMgr->Draw(window);
		healthText.setString("Health: " + to_string(pGameMgr->GetHealth()));
		window.draw(healthText);
		// Display the score on the window
		scoreText.setString("Score: " + to_string(pGameMgr->GetScore()));
		window.draw(scoreText);
		window.display();

		pGameMgr->CheckGameOver();
	}

	delete(pGameMgr);
}