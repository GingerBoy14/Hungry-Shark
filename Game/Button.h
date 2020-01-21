#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
class Button {
private:
	short iwidth, iheight;
	Color sColor;
	Color hColor;
	Font font;
	Text text;
	Texture tButton;
	Sprite sButton;

	void centerText() {
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 1.5f);
		text.setPosition(sButton.getPosition().x + width / 2, sButton.getPosition().y + height / 2);
	}

public:
	short width, height, x, y;
	Button(String text, String iPath, Font &font, Color standartColor, Color hoverColor, short W, short H, short X, short Y, float scale) {
		width = W * scale;
		height = H * scale;
		iwidth = W;
		iheight = H;
		x = X - (width / 2); y = Y;
		sColor = standartColor;
		hColor = hoverColor;
		this->font = font;
		this->text.setFont(this->font);
		this->text.setString(text);
		this->text.setFillColor(sColor);

		tButton.loadFromFile(iPath);
		tButton.setSmooth(true);

		sButton.setTexture(tButton);
		sButton.setTextureRect(IntRect(0, 0, W, H));
		sButton.setPosition(x, y);

		sButton.setScale(scale, scale);
		centerText();

	}
	void draw(RenderWindow &window) {
		window.draw(sButton);
		window.draw(text);
	}
	void setPosition(short X,short Y) {
		x = X - (width / 2); y = Y;
		sButton.setPosition(x, y);
		centerText();
	}
	void setY(short Y) {
		y = Y;
		sButton.setPosition(x, y);
		centerText();
	}
	void setX(short X) {
		x = X - (width / 2);
		sButton.setPosition(x, y);
		centerText();
	}
	bool isHover(RenderWindow &window) {
		if (IntRect(x, y, width, height).contains(Mouse::getPosition(window))) {
			sButton.setTextureRect(IntRect(iwidth, 0, iwidth, iheight));
			text.setFillColor(hColor);
			return true;
		}
		else {
			sButton.setTextureRect(IntRect(0, 0, iwidth, iheight));
			text.setFillColor(sColor);
			return false;
		}
	}
};