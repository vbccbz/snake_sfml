#include <SFML/Graphics.hpp>
#include <time.h>

#include <stdlib.h>
#include <iostream>

int spriteSize = 32;
int W = 10;
int H = 10;

class Snake
{
private:

public:
  struct element
  {
    struct coordinate
    {
      int row;
      int column;
    } position;
    //int number;
  } body[100];

  size_t length;

  enum class direction
  {
    left,
    right,
    up,
    down
  } direction;

  int life;
  int hit;

public:
  Snake(void)
  {
    life = 3;
    direction = direction::right;
    int r = 0;
    int c = 5;
    length = 4;
    for (int i = 0; i < length; i++)
    {
      body[i].position.row = r;
      body[i].position.column = c--; //body[c].position.row
    }
  };
  ~Snake(void)
  {
    ;
  }
};
class Fruit {
public:
  struct element
  {
    struct coordinate
    {
      int row;
      int column;
    } position;
  };
  element body;
  //bool life;

public:
  Fruit(void)
  {
    // srand(time(NULL));
    // body.position.row = rand() % 10;
    // body.position.column = rand() % 10;

    body.position.row = 5;
    body.position.column = 5;

    //life = true;
  }
  ~Fruit(void)
  {
    ;
  }
  void generate(void)
  {
    // srand(time(NULL));
    body.position.row = rand() % 10;
    body.position.column = rand() % 10;
    //life = true;
  }
  //bool lifeState(void)
  //{
  //	return life ? true : false;
  //}
};
bool collide(Snake& s, Fruit& f)
{
  bool result = false;
  if (s.body[0].position.column == f.body.position.column &&
    s.body[0].position.row == f.body.position.row)
  {
    result = true;
  }
  return result;
}
bool collide(Snake& s)
{
  bool result = false;
  for (int i = 2; i < s.length; ++i)
  {
    if (s.body[0].position.row == s.body[i].position.row &&
      s.body[0].position.column == s.body[i].position.column)
    {
      result = true;
      s.hit = i;
    }
  }
  return result;
}
int main(void)
{
  sf::RenderWindow window(sf::VideoMode(spriteSize * W, spriteSize * H), "Snaaaake!");
  sf::Clock clock;

  Snake snake;
  Fruit fruit;

  sf::Texture snakeTexture;
  sf::Texture fruitTexture;
  sf::Texture worldTexture;

  snakeTexture.loadFromFile("images/green.png");
  fruitTexture.loadFromFile("images/red.png");
  worldTexture.loadFromFile("images/white.png");

  sf::Sprite snakeSprite;
  sf::Sprite fruitSprite;
  sf::Sprite worldSprite;

  snakeSprite.setTexture(snakeTexture);
  fruitSprite.setTexture(fruitTexture);
  worldSprite.setTexture(worldTexture);

  float timer = 0;
  float delay = 0.5;

  while (window.isOpen())
  {
    sf::Event e;
    while (window.pollEvent(e))
    {
      if (e.type == sf::Event::Closed)
      {
        window.close();
      }
    }

    // set tne direction
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
      snake.direction = Snake::direction::left;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
      snake.direction = Snake::direction::right;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
      snake.direction = Snake::direction::up;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
      snake.direction = Snake::direction::down;
    }

    // get an elapsed time
    float elapsed = clock.getElapsedTime().asSeconds();
    clock.restart();
    timer += elapsed;

    if (timer >= delay)
    {
      // reset timer
      timer = 0;

      // LOGIC

      // body must be update before head
      Snake::element temp1;
      Snake::element temp2;
      temp1.position = snake.body[0].position;
      for (size_t i = 1; i <= snake.length; ++i)
      {
        temp2.position = snake.body[i].position;
        snake.body[i].position = temp1.position;
        temp1.position = temp2.position;
      }

      // update head and do wrapping
      switch (snake.direction)
      {
      case Snake::direction::up:
        snake.body[0].position.row -= 1;
        if (snake.body[0].position.row < 0)
        {
          snake.body[0].position.row = H - 1;
        }
        break;
      case Snake::direction::down:
        snake.body[0].position.row += 1;
        if (snake.body[0].position.row == H)
        {
          snake.body[0].position.row = 0;
        }
        break;
      case Snake::direction::left:
        snake.body[0].position.column -= 1;
        if (snake.body[0].position.column < 0)
        {
          snake.body[0].position.column = W - 1;
        }
        break;
      case Snake::direction::right:
        snake.body[0].position.column += 1;
        if (snake.body[0].position.column == W)
        {
          snake.body[0].position.column = 0;
        }
        break;
      default:
        break;
      }

      // check collision with another object
      if (collide(snake, fruit))
      {
        snake.length++;
        fruit.generate();
      }

      // check collision with self
      if (collide(snake))
      {
        snake.length = snake.hit;
        snake.life--;
      }

      std::cout << " life: " << snake.life << '\n';
      std::cout << " c: " << snake.body[0].position.column << '\n';
      std::cout << " r: " << snake.body[0].position.row << '\n';



      // DRAWING
      window.clear();

      for (int c = 0; c < W; ++c)
      {
        for (int r = 0; r < H; ++r)
        {
          worldSprite.setPosition(c * spriteSize, r * spriteSize);
          window.draw(worldSprite);
        }
      }

      for (int i = 0; i < snake.length; ++i)
      {
        snakeSprite.setPosition(snake.body[i].position.column * spriteSize, snake.body[i].position.row * spriteSize);
        window.draw(snakeSprite);
      }

      fruitSprite.setPosition(fruit.body.position.column * spriteSize, fruit.body.position.row * spriteSize);
      window.draw(fruitSprite);

      window.display();
    }
  }
  return 0;
}
