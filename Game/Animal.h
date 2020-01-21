#pragma once
#include "Animation.h"
#include <SFML/Graphics.hpp>
#include "constants.h"

using namespace sf;
using namespace constants;


class Creature {
public:
	float DX, DY, x, y, speed, scale;
	char health;
	Animation animation;
	Sprite sprite;

	Creature() { health = 100; }
	virtual void settings() {};
	virtual void update() {};
};

class Shark : public Creature {
private:
	int maxSpeed = 8;

public:
	float subtractLifeTimer = 0;

	float rotation = 0;
	Shark() : Creature() {
		DX = 0, DY = 0;
	}
	void settings(Animation &a, float X, float Y) {
		animation = a;
		sprite = animation.sprite;
		x = X; y = Y;
		scale = 0.65f;
	}
	void update() {
		if (subtractLifeTimer >= 0.22 && health >= 0) {
			health -= 1;
			subtractLifeTimer = 0;
		}
		sprite.setScale(scale, scale);

		speed = sqrt(DX * DX + DY * DY);

		if (speed > maxSpeed) {
			DX *= maxSpeed / speed;
			DY *= maxSpeed / speed;
		}
		x += 0.1f*DX;
		y += 0.1f*DY;
		if (x > WIDTH)x = WIDTH; if (x < 0)x = 0;
		if (y > HEIGHT)y = HEIGHT; if (y < 0)y = 0;

		sprite.setPosition(x, y);

		rotation = atan2(DY, DX) * 180 / 3.14159265f;//получаем угол в радианах и переводим его в градусы
		sprite.setRotation(rotation);

		sprite.setTextureRect(IntRect(int(animation.Frame) * animation.width,
			rotation > 90 || rotation < -90 && rotation < 180 ? animation.height : 0,
			animation.width,
			rotation > 90 || rotation < -90 && rotation < 180 ? -animation.height : animation.height));
		animation.update();
	}

	FloatRect getRect() {
		return FloatRect(rotation > 90 || rotation < -90 && rotation < 180 ? x+15 : x - animation.width / 2, y, (float)animation.width * scale, (float)animation.height * scale);
	}
};


class Fish : public Creature {

private:
	float moveTimer = 0;
	Texture t;

	bool moveDir() {
		int random = rand() % 6000;
		bool answer = false;
		if (rand() % 2 == 1) {
			moveTimer += speed / random;
		}
		if (moveTimer > rand() % 1000 + rand() % 300) {
			answer = true;
		}
		return answer;
	}

	void checkCollisionWithMap(float Dx)//ф-ция проверки столкновений с картой
	{
		if (Dx > WIDTH - ((animation.width*0.2f / 2))) {
			speed = -float(0.2f + rand() / (RAND_MAX / (0.3f - 0.2f))),
				sprite.setScale(0.2f, 0.2f);
		}
		if (Dx < 0 + ((animation.width*0.2f / 2))) {
			speed = float(0.2f + rand() / (RAND_MAX / (0.3f - 0.2f))),
				sprite.setScale(-0.2f, 0.2f);
		}
	}

public:
	char fishType;
	Fish() : Creature() {}

	void settings(Animation &a, float X, float Y) {
		scale = 0.2f;
		animation = a;
		sprite = animation.sprite;
		x = X; y = Y;
		speed = float(scale + rand() / (RAND_MAX / (0.3f - scale)));
	}

	void update() {
		checkCollisionWithMap(x);
		if (moveDir()) {
			speed *= -1;
			moveTimer = 0;
			sprite.setScale(speed > 0 ? -scale : scale, scale);
		}
		x += 2.2f*speed;
		sprite.setPosition(x, y);
		sprite.setTextureRect(IntRect(animation.width * int(animation.Frame), 0, animation.width, animation.height));
		animation.update();
	}
	void setTexture( String path) {
		t.loadFromFile(path);
		sprite.setTexture(t);
	}

	FloatRect getRect() {
		return FloatRect(x, y, (float)animation.width * scale, (float)animation.height * scale);
	}
};