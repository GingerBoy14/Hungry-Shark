#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <time.h>
#include "constants.h"

#include "Menu.h"
#include "Animal.h"

using namespace sf;
using namespace constants;
using namespace std;



short getRandomPos(vector<Fish*> fishes) {
	short random = rand() % (HEIGHT-70) + 50;
	for (char loop = 0; loop < fishes.size(); loop++) {
		if (fishes[loop]->y + 35 > random && fishes[loop]->y - 35 < random)
		{
			return getRandomPos(fishes);
		}
	}
	return random;
}

bool game(RenderWindow &window) {

	srand(time(0));				//for init rand() create different nums every time
	if (mainMenu(window));
	else return false;
	

	Font font;
	font.loadFromFile("rec/arial.ttf");
	Vector2f blackrec;

	RectangleShape life;
	life.setPosition(Vector2f(WIDTH - 105, 2));
	Vertex rect[] =
	{
	 Vertex(Vector2f(WIDTH - 105, 2)),
	 Vertex(Vector2f(WIDTH - 5, 2)),
	 Vertex(Vector2f(WIDTH - 105, 1)),
	 Vertex(Vector2f(WIDTH - 105, 22)),
	 Vertex(Vector2f(WIDTH - 105, 22)),
	 Vertex(Vector2f(WIDTH - 5, 22)),
	 Vertex(Vector2f(WIDTH - 5, 1)),
	 Vertex(Vector2f(WIDTH - 5, 22))
	};

	Text score;
	score.setFont(font);
	score.setCharacterSize(20);

	Texture t1, t2, t3, t4;
	t1.loadFromFile("image/Shark1.png");
	t2.loadFromFile("image/fish.png");
	t3.loadFromFile("image/Shark_eat1.png");
	t4.loadFromFile("image/background/Background 0001.jpg");

	t1.setSmooth(true);
	t2.setSmooth(true);
	t3.setSmooth(true);
	t4.setSmooth(true);

	Sprite background(t4);

	Animation sPlayer(t1, 0, 0, 300, 105, 8, 0.02f);
	Animation sPlayer_eat(t3, 0, 0, 300, 105, 3, 0.025f);
	Animation sFish(t2, 0, 0, 250, 170, 3, 0.02f);
	Shark player;
	player.settings(sPlayer, window.getSize().x / 2, window.getSize().y / 2);
	player.sprite.setScale(0.65f, 0.65f);

	vector<Fish*> fishes;   //	array of fishes
			/*		loop for generate first group of fish after start		*/
	for (int i = 0; i < rand() % 15 + 1; i++)
	{
		Fish *a = new Fish();
		a->settings(sFish, rand() % WIDTH, getRandomPos(fishes));	// create fish
		a->sprite.setScale(-0.2f, 0.2f);
		a->fishType = 1;
		fishes.push_back(a);	// Add element at the end
	}

	int iScore = 0;
	score.setString("Score: " + to_string(iScore));

	/**************					MAIN LOOP, FORM IS OPEN								*****************/
	while (window.isOpen())
	{
		Clock clock;
		float time = clock.getElapsedTime().asMicroseconds();
		time /= 800;
		player.subtractLifeTimer += time;
		life.setPosition(Vector2f(WIDTH - player.health - 5, 2));
		life.setSize(Vector2f(player.health, 20));

		if(player.health <= 15)	 life.setFillColor(Color::Red);
		else if(player.health <= 50)	life.setFillColor(Color::Yellow);
		else life.setFillColor(Color::Green);

		char bufsize = fishes.size();
		while (window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed){	window.close(); return false;	}
			if(event.type == Event::KeyPressed)
				if(event.key.code == Keyboard::Escape) {
					if(pauseMenu(window)) return true;	//pause
					 break;
				}
		}
		if(event.type == Event::MouseMoved) {
			player.DX = event.mouseMove.x - player.x;//вектор , колинеарный прямой, которая пересекает спрайт и курсор
			player.DY = event.mouseMove.y - player.y;//он же, координата y
		}
	
		
		/*			loop for fish move and eating			*/
		for(auto i = fishes.begin(); i != fishes.end();)
		{
			Fish *f = *i;
			f->update();
			if ((*i)->getRect().intersects(player.getRect())) {
				f->health = 0;
				player.settings(sPlayer_eat, player.x, player.y);
				switch (f->fishType)
				{
				case 1: {
					if (player.health + 5 <= 100)
						player.health += 5;
					else
						player.health = 100;
					iScore += 100;
					break;
				}
				case 2: {
					if (player.health + 9 <= 100)
						player.health += 9;
					else
						player.health = 100;
					iScore += 200;
					break;
				}
				case 3: {
					if (player.health + 21 <= 100)
						player.health += 21;
					else
						player.health = 100;
					iScore += 500;
					break;
				}
				case 4: {
					if (player.health + 45 <= 100)
						player.health += 45;
					else
						player.health = 100;
					iScore += 1000; 
					break;	
				}
				}
				score.setString("Score: " + to_string(iScore));
				
		
			}
			if (player.animation.isEnd()) player.settings(sPlayer, player.x, player.y);
			if (f->health <= 0) { i = fishes.erase(i); delete f; }// если этот объект мертв, то удаляем его
			else i++;
		}
		if ((bufsize > fishes.size() && rand() % 2) || fishes.size() == 0) {
			for (int i = 0; i < rand() % 10; i++)
			{
				Fish *a = new Fish();
				a->settings(sFish, rand() % WIDTH, getRandomPos(fishes));				// create fish
				a->sprite.setScale(-0.2f, 0.2f);
				switch (rand() % 10)
				{
				case 1:
				case 2:
				case 3:
				case 5: {
					a->setTexture("image/fish2.png");
					a->fishType = 2;	break;
				}
				case 7: {
					a->setTexture("image/fish3.png");
					a->fishType = 3;	break;	
				}		
				case 9: {
					a->setTexture("image/fish4.png");
					a->fishType = 4;	break;	
				}
				default: {
					a->setTexture("image/fish.png");
					a->fishType = 1;	break;
				}
				}
				fishes.push_back(a);	// Add element at the end
				if (fishes.size() > 15)break;
			}
		}

		player.update();


		/*			 draw			*/
		window.clear();
		window.draw(background);
		window.draw(score);
		window.draw(life);
		window.draw(rect, 8, Lines); //outline

		if (player.health >= 0) {
			window.draw(player.sprite);   //draw Shark
			for (auto i : fishes) window.draw(i->sprite);	//draw fishes
		}
		else {
			if(scoreMenu(window, 1, iScore))return true;
			else return false;
		}
		window.display();			  //repaint the form
	}
}

void playGame(RenderWindow &window) {
	if (game(window))playGame(window);
}
int main(){
	RenderWindow window(VideoMode(WIDTH, HEIGHT), "Hungry Shark", Style::Fullscreen);

	playGame(window);
	return 0;
}