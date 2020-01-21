#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Animation{
private:
	bool change = true;
	float frameCount;
public:
	float Frame, speed;
	short width, height, x, y;
	Sprite sprite;

	Animation() {}
	Animation(Texture &t, int X, int Y, int w, int h, int count, float Speed){
		x = X; y = Y;
		width = w; height = h;
		Frame = 0;
		speed = Speed;
		frameCount = count;
		sprite.setTexture(t);
		sprite.setOrigin(w, h / 2);
		sprite.setTextureRect(IntRect(x, y, w, h));
	}

	void update(){
		if (change) {
			Frame += speed;
			change = true;
			if (int(Frame) > frameCount) {
				Frame = frameCount;   //something like stabilization
				change = false;
			}
		}
		else {
			Frame -= speed;
			change = false;
			if (int(Frame) < 1) {
				Frame = 0;  //something like stabilization
				change = true;
			}
		}
	}

	bool isEnd(){
		return Frame + speed >= frameCount;
	}
};
