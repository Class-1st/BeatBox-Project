// made in : 김민서

#pragma once

#include "Main.h"
#include<deque>
#include<time.h>

class Game {
public:
	void run(RenderWindow& window);
};

// 화살의 정보를 담는 구조체
struct ArrowShape
{
	int direction; // 화살의 방향을 나타내는 값 (0: UP, 1: DOWN, 2: LEFT, 3: RIGHT)
	int x; // 화살의 x 좌표
	int y; // 화살의 y 좌표
};

// 화살 클래스 정의
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

	Texture t; // 화살 이미지의 텍스처
	Sprite s; // 화살 이미지의 스프라이트
	deque<ArrowShape> questions; // 화살의 정보를 담는 덱 (큐)

	float frame; // 화살 애니메이션 프레임
	int frameLimit; // 화살 애니메이션 프레임 제한
	static float frameSpeed; // 화살 애니메이션 속도
	int dir[4] = { -90, 90, 180, 0 }; // 화살 방향에 대한 각도

	// 현재 입력된 키가 정답인지 확인하는 함수
	bool CheckArrow(int idx)
	{
		if (questions.empty()) return false;
		if (idx == questions[0].direction) return true;

		return false;
	}

public:

	// 생성자
	Arrow()
	{
		srand(static_cast<unsigned int>(time(nullptr)));

		t.loadFromFile("image/right.png");
		s = Sprite(t);
		Rect<float> rect = s.getGlobalBounds();
		s.setOrigin(Vector2f(rect.width / 2, rect.height / 2));
		frame = 20.f;
		frameLimit = 20;
	}

	// 소멸자
	~Arrow() {}

	// 초기화 함수
	void Init() { questions.clear(); }

	void setSpeed(float speed) { frameSpeed = speed; }

	// 키 입력을 확인하고 정답 여부를 반환하는 함수
	bool CheckKey()
	{
		if (Keyboard::isKeyPressed(Keyboard::Up)) return CheckArrow(Dir::UP);
		else if (Keyboard::isKeyPressed(Keyboard::Down)) return CheckArrow(Dir::DOWN);
		else if (Keyboard::isKeyPressed(Keyboard::Left)) return CheckArrow(Dir::LEFT);
		else if (Keyboard::isKeyPressed(Keyboard::Right)) return CheckArrow(Dir::RIGHT);

		return false;
	}

	// 화살의 설정 및 애니메이션 처리 함수
	void ArrowSetting()
	{
		// 애니메이션 프레임 체크
		frame += frameSpeed;
		if (frame <= frameLimit) return;
		frame -= frameLimit;

		// 화살 개수 체크
		if (questions.size() > 10) return;
		ArrowShape as;

		// 랜덤으로 화살의 방향 설정
		int dirIdx = rand() % 4;
		as.direction = dirIdx;
		as.x = WIDTH;
		as.y = 100;
		s.setPosition(as.x, as.y);
		questions.push_back(as);
	}

	// 화살 이동 함수
	void Move(const int& speed, bool& isGameOver)
	{
		//맨처음 화살표가 사라지면 끝에서 나타나야하기떄문에 사용함 ¡
		deque<ArrowShape>::iterator iter;
		for (iter = questions.begin(); iter != questions.end(); ++iter)
		{
			iter->x -= speed;
			if (iter->x <= 0)
			{
				// 게임 오버
				isGameOver = true;
				break;
			}
		}
	}

	// 화살 그리기 함수
	void Draw(RenderWindow& _window)
	{
		for (auto iter = questions.begin(); iter != questions.end(); ++iter)
		{
			s.setRotation(dir[iter->direction]);
			s.setPosition(iter->x, iter->y);
			_window.draw(s);
		}
	}

	// 화살의 위치 정보 반환 함수
	ArrowShape GetFirstArrowPos() { return questions[0]; }

	// 화살 제거 함수
	void RemoveFirstArrow()
	{
		if (questions.empty()) return;
		questions.pop_front();
	}

	// 화살이 비어있는지 여부 반환 함수
	bool IsEmpty() const { return questions.empty(); }
};

// 게임 매니저 클래스 정의
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

		// 텍스처 및 스프라이트 초기화
		tw.loadFromFile("image/wrong.png");
		tl.loadFromFile("image/player.png");
		imgWrong = Sprite(tw);
		imgDeadLine = Sprite(tl);

		imgDeadLine = Sprite(tl);
		imgDeadLine.setPosition(0, 45);

		Rect<float> rect = imgWrong.getGlobalBounds();
		imgWrong.setOrigin(Vector2f(rect.width / 2, rect.height / 2));
	}

	void IncreaseScore() { score++; }// 점수 증가 함수
	void DecreaseHealth() { health--; }// 체력 감소 함수
	
	int GetHealth() const { return health; }// 현재 체력 반환 함수
	int GetScore() const { return score; }// 현재 점수 반환 함수

	// 초기화 함수
	void Init() { pArrow->Init(); }

	// 키 입력 확인 함수
	void CheckKey()
	{
		if (pArrow->IsEmpty()) return;

		if (pArrow->CheckKey())
		{
			// 정답인 경우
			firstArrowPos = pArrow->GetFirstArrowPos();
			pArrow->RemoveFirstArrow();
			state = CORRECT;
		}

		else
		{
			// 오답인 경우
			firstArrowPos = pArrow->GetFirstArrowPos();
			pArrow->RemoveFirstArrow();
			state = WRONG;
		}
	}
	// 화살 이동 함수
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
			// 오답 시 게임 오버
			state = NONE;
			imgWrong.setPosition(firstArrowPos.x, firstArrowPos.y);
			window.draw(imgWrong);
			isGameOver = true;
			DecreaseHealth();
			break;
		case CORRECT:
			// 정답 시 점수 증가
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
			// 체력이 0 이하인 경우
			sleep(seconds(3));
			isGameOver = true;
			Init(); // 게임 재시작
		}

		else if (isGameOver)
		{
			// 게임 오버 후 3초 대기 후 게임 재시작
			sleep(seconds(3));
			isGameOver = false;
			Init(); // 게임 재시작
		}
	}
};