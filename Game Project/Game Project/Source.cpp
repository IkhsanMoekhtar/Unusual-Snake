#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <vector>

using namespace sf;
using namespace std;

	

class Bullet
{
	public:
	Sprite shape;
	

	Bullet(Texture *texture,Vector2f pos)
	{
		this->shape.setTexture(*texture);
		this->shape.setScale(0.1f, 0.1f);
		this->shape.setPosition(pos);
		

	}

	~Bullet(){}
};

class Player 
{

public:
	
	Sprite shape;
	Texture* texture;
	int HP;
	int HPmax;

	vector<Bullet> bullets;

	Player(Texture *texture)
	{
		this->HPmax = 10;
		this->HP = this->HPmax;
		this->texture = texture;
		this->shape.setTexture(*texture);
		this->shape.setScale(0.15f, 0.15f);
		
	}

	~Player(){}
	
};

class Boss
{
public:
	Sprite shape;
	int HP;
	int HPmax;

	Boss(Texture *texture,Vector2u windowSize)
	{
		this->HPmax = 10;
		this->HP = this->HPmax;
		this->shape.setTexture(*texture);
		this->shape.setScale(0.5f, 0.5f);
		this->shape.setPosition(windowSize.x - this->shape.getGlobalBounds().width + 120, rand() % (int)(windowSize.y - this->shape.getGlobalBounds().height));
	}

	~Boss(){}
};

class Enemy
{
public:
	Sprite shape;
	int HP;
	int HPmax;
	int movement;

	

	Enemy(Texture* texture,Vector2u windowSize)
	{
		this->HPmax = rand() % 3 + 1;
		this->HP = this->HPmax;
		this->shape.setTexture(*texture);
		this->shape.setScale(0.1f, 0.1f);
		this->shape.setPosition(windowSize.x - this->shape.getGlobalBounds().width +120  , rand()% (int)(windowSize.y - this->shape.getGlobalBounds().height));
		this->movement = rand() % 3 + 1;
		if (this->HPmax == 1)
		{
			this->shape.setScale(0.1f, 0.1f);
		}
		if (this->HPmax == 2)
		{
			this->shape.setScale(0.2f, 0.2f);
		}
		if (this->HPmax == 3)
		{
			this->shape.setScale(0.3f, 0.3f);
		}
	}

	~Enemy(){}

};

int main()
{

	//make a clock
	Clock clock;
	Clock clock2;
	Clock clock3;

	srand(time(NULL));

	//window setting
	sf::VideoMode mode(1280, 720);
	sf::RenderWindow window(mode, "Unusual Snake");
	window.setFramerateLimit(60);


	//make cursor gone
	window.setMouseCursorVisible(true);

	//load background
	Texture BackgroundTex;
	BackgroundTex.loadFromFile("Textures/BackgroundTex.jpg");
	if (!BackgroundTex.loadFromFile("Textures/BackgroundTex.jpg"))
	{
		cout << "Load Failed" << endl;
		system("pause");
	}
	BackgroundTex.setRepeated(true);
	IntRect ibounds(0.0, 0.0, 1280, 720);
	Sprite background(BackgroundTex,ibounds);
	
	

	//load Font
	Font font;
	font.loadFromFile("Fonts/Fruit-Days.ttf");

	//load player Tex
	Texture PlayerTex;
	PlayerTex.loadFromFile("Textures/PlayerTEx.png");

	//load Enemy Tex
	Texture EnemyTex;
	EnemyTex.loadFromFile("Textures/EnemyTex.png");

	//load Bullet Tex
	Texture BulletTex;
	BulletTex.loadFromFile("Textures/BulletTex.png");

	//load Boss Tex
	Texture BossTex;
	BossTex.loadFromFile("Textures/BossTex.png");



	//UI load
	Text score;
	score.setFont(font);
	score.setCharacterSize(40);
	score.setFillColor(Color::White);
	score.setPosition(10.f, 10.f);

	//Game Over
	Text gameover;
	gameover.setFont(font);
	gameover.setCharacterSize(100);
	gameover.setFillColor(Color::Red);
	gameover.setPosition(window.getSize().x/3.5, window.getSize().y / 2.5f);
	gameover.setString("GAME OVER!");
	
	
	//player load
	int point = 0;
	float hit = -30.f;
	Player player(&PlayerTex);
	Text playerHp;
	playerHp.setFont(font);
	playerHp.setCharacterSize(20);
	playerHp.setFillColor(Color::White);
	RectangleShape HPbar;
	HPbar.setSize(Vector2f(-30.f, 10.f));
	HPbar.setFillColor(Color::Red);
	HPbar.setOutlineThickness(1.5f);
	HPbar.setOutlineColor(Color::Black);

	//enemy load
	Text enemyHp;
	enemyHp.setFont(font);
	enemyHp.setCharacterSize(20);
	enemyHp.setFillColor(Color::White);
	int enemyspwntimer = 0;
	vector<Enemy> enemies;
	enemies.push_back(Enemy(&EnemyTex, window.getSize()+ (Vector2u(120,120))));

	//Boss load
	vector<Boss> bosses;
	int BossSpwnTimer = 0;
	bosses.push_back(Boss(&BossTex, window.getSize() + (Vector2u(120, 120))));



	//------------------------------------------GAME LOOP------------------------------------------------
	while (window.isOpen())
	{

		//WINDOW EXIT PROGRAM
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}

		}

		
		//UPDATE CONTROL

		//player dead
		if (player.HP > 0) {


			//PLAYER MOVEMENT
			//make input mouse follow player
				player.shape.setOrigin(player.shape.getLocalBounds().width / 2, player.shape.getLocalBounds().height / 2);
				player.shape.setPosition(Mouse::getPosition().x - window.getSize().x/4 , Mouse::getPosition().y - window.getSize().y / 2.6f);
				cout << "Player Hp:" << player.HP << endl;

			//player HP position set
			playerHp.setPosition(player.shape.getPosition().x, player.shape.getPosition().y - playerHp.getGlobalBounds().height * 5.f);
			playerHp.setString(to_string(player.HP) + "/" + to_string(player.HPmax));

			//player HP bar Position set
			HPbar.setPosition(player.shape.getPosition().x + 15.f, player.shape.getPosition().y - playerHp.getGlobalBounds().height * 3.5f);



			//window collide
			int maxX = window.getSize().x;
			int maxY = window.getSize().y;

			int mX = sf::Mouse::getPosition(window).x;
			int mY = sf::Mouse::getPosition(window).y;

			if (mX < 20 || mY < -20 || mX > maxX - 20 || mY > maxY - 20)
			{
				if (mX < 20)
					mX = 20;
				if (mX > maxX - 20)
					mX = maxX - 20;

				if (mY < -20)
					mY = -20;
				if (mY > maxY - 20)
					mY = maxY - 20;

				sf::Mouse::setPosition(sf::Vector2i(mX, mY), window);
			}
			//SHOOT PROGRAM
			//shoot timer
			Time elapsed = clock.getElapsedTime();
			int delay = 120;
			if (Mouse::isButtonPressed(Mouse::Left) && elapsed.asMilliseconds() >= delay)
			{
				player.bullets.push_back(Bullet(&BulletTex, player.shape.getPosition()));
				clock.restart();

			}

			Time elapsed3 = clock3.getElapsedTime();

			//BULLETS
			for (size_t i = 0;i < player.bullets.size();i++)
			{
				//bullet move
				player.bullets[i].shape.move(20.f, 0.f);

				// out of window collision
				if (player.bullets[i].shape.getPosition().x > window.getSize().x)
				{
					player.bullets.erase(player.bullets.begin() + i);
					break;
				}
				//ENEMY COLLISION
				for (size_t k = 0; k < enemies.size();k++)
				{
					if (player.bullets[i].shape.getGlobalBounds().intersects(enemies[k].shape.getGlobalBounds()))
					{


						if (enemies[k].HP <= 1)
						{
							point += enemies[k].HPmax;
							enemies.erase(enemies.begin() + k);
						}
						else
						{
							enemies[k].HP--;

						}


						player.bullets.erase(player.bullets.begin() + i);
						break;

					}
					

					
					
				}
				for (size_t j = 0; j < bosses.size();j++)
				{
					if (player.bullets[i].shape.getGlobalBounds().intersects(bosses[j].shape.getGlobalBounds()))
					{
						if (bosses[j].HP <= 1)
						{
							point += bosses[j].HPmax;
							bosses.erase(bosses.begin() + j);
						}
						else
						{
							bosses[j].HP--;
						}

						player.bullets.erase(player.bullets.begin() + i);
						break;
					}
				}
				


			}


			//ENEMY
			if (enemyspwntimer < 20)
			{
				enemyspwntimer++;
			}
			if (enemyspwntimer >= 20)
			{
				enemies.push_back(Enemy(&EnemyTex, window.getSize()));
				enemyspwntimer = 0;
			}

			
			//enemy movement
			for (size_t i = 0,k=0;i < enemies.size();i++,k++)
			{
				
				if (enemies[i].movement == 1)
				{
					enemies[i].shape.move(-3.f, 0.0f);
					
				}
				if (enemies[i].movement == 2)
				{
						enemies[i].shape.move(-3.f, (rand()% 2 + 1));
				}
				if (enemies[i].movement == 3)
				{
					enemies[i].shape.move(-3.f, -(rand()% 2 + 1));
				}

				
				
				//remove enemy if collision with the window
				if (enemies[i].shape.getPosition().x <= 0 - enemies[i].shape.getGlobalBounds().width)
				{
					enemies.erase(enemies.begin() + i);
				}
				//player collide to enemy
				if (enemies[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
				{
					enemies.erase(enemies.begin() + i);

					//player get damage
					player.HP--;
					HPbar.setSize(Vector2f(hit+=3, 10.f));
					break;
				}
				//player collide to boss
				if (bosses[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
				{
					//bosses.erase(bosses.begin() + i);

					//player get damage
					player.HP--;
					HPbar.setSize(Vector2f(hit += 3, 10.f));
					break;
				}

			}
			//BOSS
			if (BossSpwnTimer < 1000)
			{
				BossSpwnTimer++;
			}
			if (BossSpwnTimer >= 1000)
			{
				bosses.push_back(Boss(&BossTex, window.getSize()));
				BossSpwnTimer = 0;
			}

			//boss movement
			for (size_t i = 0;i < bosses.size();i++)
			{
				bosses[i].shape.move(-0.2f, 0.0f);
				//remove boss if collide with the window
				if (bosses[i].shape.getPosition().x <= 0 - bosses[i].shape.getGlobalBounds().width)
				{
					bosses.erase(bosses.begin() + i);
				}
			}

			//UI update
			score.setString("Score :" + to_string(point));

		}
		//----------------------DRAW-----------------------
		//display enemy bullet
		window.clear();

		//display background
		window.draw(background);

		//display UI
		//window.draw(playerHp);
		window.draw(score);
		window.draw(HPbar);



		//display player
		window.draw(player.shape);

		//display bullet
		for (size_t i = 0;i < player.bullets.size();i++)
		{
			window.draw(player.bullets[i].shape);
		}

		//display boss
		for (size_t i = 0;i < bosses.size();i++)
		{
			window.draw(bosses[i].shape);
		}

		//display enemy
		for (size_t i = 0;i < enemies.size();i++)
		{
			enemyHp.setString(to_string(enemies[i].HP) + "/" + to_string(enemies[i].HPmax));
			enemyHp.setPosition(enemies[i].shape.getPosition().x, enemies[i].shape.getPosition().y - enemyHp.getGlobalBounds().height);
			window.draw(enemyHp);
			window.draw(enemies[i].shape);
		}
		if (player.HP <= 0)
		{
			window.draw(gameover);
		}

		window.display();
	}
	return 0;
} 