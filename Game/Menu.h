#pragma once
#include "Button.h"
#include "constants.h"
#include <fstream>
#include <vector>
#include <algorithm>
using namespace sf;
using namespace std;
using namespace constants;

void WriteToFile(vector<int> &l1) {
	ofstream f1("score.txt", ios::trunc | ios::beg);
	if (f1.is_open()) {
		for (auto i : l1) {
			f1 << i << endl;
		}
		f1.close();
	}
}
void ReadFromFile(vector<int> &score) {
	string line;
	ifstream f1("score.txt");
	score.clear();
	//файл існує
	if (f1.is_open()) {
		while (getline(f1, line))
			score.push_back(atoi(line.c_str()));

		f1.close();
		sort(score.rbegin(), score.rend());
		if (score.size() > 10) {
			score.resize(10);
		}
	}
	//файл відсутній - створимо його
	else {
		ofstream f1("score.txt", std::ios::trunc);
		if (f1.is_open()) {
			f1.close();
		}
	}
}
void pushScore(Text &title, vector<Text*> &score, Font &font, vector<int> &iScore, int playerscore) {
	char count = 0;
	while (count < 10) {
		Text* t = new Text();
		t->setString(to_string(count + 1) + ". " + to_string(iScore[count]));
		t->setFont(font);
		t->setCharacterSize(30);
		if (playerscore == iScore[count]) t->setFillColor(Color::Red);
		else t->setFillColor(Color(13, 182, 222));
		if (score.size() == 0)
			t->setPosition(WIDTH / 2 - title.getLocalBounds().width / 2, title.getPosition().y + title.getLocalBounds().height + 15);
		else
			t->setPosition(WIDTH / 2 - title.getLocalBounds().width / 2, score[score.size() - 1]->getPosition().y + t->getLocalBounds().height + 10);
		score.push_back(t);
		count++;
	}

}
bool scoreMenu(RenderWindow &window, bool status, int playerscore) {
	vector<int> iScore;
	vector<Text*> score;
	ReadFromFile(iScore);

	Font font;
	font.loadFromFile("rec/arial.ttf");
	Text title;
	title.setFont(font);
	title.setStyle(Text::Bold);
	title.setCharacterSize(60);

	title.setString("Records");
	title.setFillColor(Color(13, 182, 222));
	title.setPosition(WIDTH / 2 - title.getLocalBounds().width / 2, HEIGHT / 15 - title.getLocalBounds().height / 2);

	bool isScore = true;

	bool pass = true;
	for (char i = 0; i < iScore.size(); i++)
		if (playerscore == iScore[i]) pass = false;

	if (pass && playerscore > 0)iScore.push_back(playerscore);
	sort(iScore.rbegin(), iScore.rend());

	pushScore(title, score, font, iScore, playerscore);
	if (status) {
		WriteToFile(iScore);
	}
	if (iScore.size() > 0) {
		isScore = false;
	}


	Button bExit("Exit", "image/button.png", font,
		Color(13, 182, 222), Color::White,
		537, 117,
		WIDTH / 2,
		title.getPosition().y + title.getLocalBounds().height + 30,
		0.4f);
	Button bReset("Reset", "image/button.png", font,
		Color(13, 182, 222), Color::White,
		537, 117,
		bExit.x + bExit.width + 50,
		title.getPosition().y + title.getLocalBounds().height + 30,
		0.4f);
	if (!isScore) {
		bExit.setPosition(WIDTH / 2 - (537 * 0.35f), score[score.size() - 1]->getPosition().y + score[score.size() - 1]->getLocalBounds().height + 30);
		bReset.setY(score[score.size() - 1]->getPosition().y + score[score.size() - 1]->getLocalBounds().height + 30);
	}
	if (status) {
		bExit.setX(WIDTH / 2);
	}



	while (!(bExit.isHover(window) && Mouse::isButtonPressed(Mouse::Left)) && !Keyboard::isKeyPressed(Keyboard::Escape)) {
		while (window.pollEvent(event))
			if (event.type == Event::Closed) { window.close(); return false; }

		window.clear(Color::White);

		if (status) {
			Text yourS;
			yourS.setFont(font);
			yourS.setFillColor(Color::Red);
			yourS.setString("Your score: " + to_string(playerscore));
			if (score.size() == 0)
				yourS.setPosition(WIDTH / 2 - yourS.getLocalBounds().width / 2, title.getPosition().y + (title.getLocalBounds().height * 2));
			else
				yourS.setPosition(WIDTH / 2 - yourS.getLocalBounds().width / 2, score[score.size() - 1]->getPosition().y + (yourS.getLocalBounds().height * 2));

			bExit.setY(yourS.getPosition().y + yourS.getLocalBounds().height * 2);
			window.draw(yourS);
		}
		if (bReset.isHover(window) && Mouse::isButtonPressed(Mouse::Left)) {
			std::ofstream ofs;
			ofs.open("score.txt", ofstream::out | ofstream::trunc);
			ofs.close();
			score.clear();
			isScore = true;
		}


		if (isScore) {
			Text noS;
			noS.setFont(font);
			noS.setFillColor(Color::Red);
			noS.setString("No score yet ;)");
			noS.setPosition(WIDTH / 2 - noS.getLocalBounds().width / 2, title.getPosition().y + (title.getLocalBounds().height * 2));
			window.draw(noS);
			bExit.setY(noS.getPosition().y + noS.getLocalBounds().height + 30);
			bExit.setX(WIDTH / 2);
		}
		if (!isScore && !status) {
			bReset.draw(window);
		}
		bExit.draw(window);
		window.draw(title);
		for (auto i : score) {
			Text t = *i;
			window.draw(t);
		}
		window.display();
	}
	return true;
}
bool mainMenu(RenderWindow &window) {
	Font font;
	font.loadFromFile("rec/arial.ttf");
	Text title;

	Button bPlay("Play", "image/button.png", font, Color(13, 182, 222), Color::White, 537, 117, WIDTH / 2, HEIGHT / (3 + 3) * 2, 0.4f);
	Button bRecords("Records", "image/button.png", font, Color(13, 182, 222), Color::White, 537, 117, WIDTH / 2, bPlay.y + bPlay.height + 30, 0.4f);
	Button bExit("Exit", "image/button.png", font, Color(13, 182, 222), Color::White, 537, 117, WIDTH / 2, bRecords.y + bRecords.height + 30, 0.4f);
	title.setFont(font);
	title.setStyle(Text::Bold);
	title.setCharacterSize(60);

	title.setString("Hangry Shark");
	title.setFillColor(Color(13, 182, 222));
	title.setPosition(WIDTH / 2 - title.getLocalBounds().width / 2, HEIGHT / (3 + 3) * 1.5f - title.getLocalBounds().height / 2);
	bool isMenu = true;
	float anti = 0;
	while (isMenu)
	{
		Clock clock;
		float time = clock.getElapsedTime().asMicroseconds();
		time /= 800;
		anti += time;
		while (window.pollEvent(event))
			if (event.type == Event::Closed) { isMenu = false;	window.close(); return false; }

		if (bPlay.isHover(window) && Mouse::isButtonPressed(Mouse::Left) && anti > 0.2f) { anti = 0; isMenu = false; return true; }//если нажали первую кнопку, то выходим из меню 
		if (bRecords.isHover(window) && Mouse::isButtonPressed(Mouse::Left) && anti > 0.2f) {
			anti = 0;
			scoreMenu(window, 0, 0);
			
		}
		if (bExit.isHover(window) && Mouse::isButtonPressed(Mouse::Left) && anti > 0.2f) { window.close(); isMenu = false; anti = 0; return false; }
		window.clear(Color::White);

		window.draw(title);

		bPlay.draw(window);
		bRecords.draw(window);
		bExit.draw(window);

		window.display();
	}

}
bool pauseMenu(RenderWindow &window) {
	Font font;
	font.loadFromFile("rec/arial.ttf");
	Text title;

	Button bPlay("Resume", "image/button.png", font, Color(13, 182, 222), Color::White, 537, 117, WIDTH / 2, HEIGHT / (3 + 3) * 2, 0.4f);
	Button bExit("Main menu", "image/button.png", font, Color(13, 182, 222), Color::White, 537, 117, WIDTH / 2, bPlay.y + bPlay.height + 30, 0.4f);

	bool isMenu = 1;
	title.setFont(font);
	title.setStyle(Text::Bold);
	title.setCharacterSize(60);
	title.setString("Pause");
	title.setFillColor(Color(13, 182, 222));
	title.setPosition(WIDTH / 2 - title.getLocalBounds().width / 2, HEIGHT / (3 + 3) * 1.5f - title.getLocalBounds().height / 2);
	while (isMenu)
	{
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed) { isMenu = false;	window.close(); }
		window.clear(Color::White);

		if (bPlay.isHover(window) && Mouse::isButtonPressed(Mouse::Left)) { isMenu = false; return false; }//если нажали первую кнопку, то выходим из меню 
		if (bExit.isHover(window) && Mouse::isButtonPressed(Mouse::Left)) { isMenu = false; return true; }

		window.draw(title);
		bPlay.draw(window);
		bExit.draw(window);

		window.display();
	}
}



