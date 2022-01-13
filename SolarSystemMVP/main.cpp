// SolarSystem.cpp : Defines the entry point for the application.
//


#include<SFML/Graphics.hpp>
#include<vector>
#include"vector.hpp"
#include<functional>
#include<iostream>

class object
{
public:
	sf::CircleShape representation;
	sf::Texture texture;
public:
	object(vector2df p, vector2df v, float m, float r, sf::Texture&& tex) : pos{ p }, vel{ v }, mass{ m }
	{
		texture.swap(tex);

		representation.setTexture(&texture);
		representation.setRadius(r);
		representation.setOrigin(r,r);
	}

	void setPosition(vector2df v)
	{
		pos = v;
		representation.setPosition(v);
	}
	void movePos(vector2df v)
	{
		setPosition(pos + v);
	}

	operator sf::Drawable& ()
	{
		return representation;
	}


	vector2df pos;
	vector2df vel;
	float mass;

};
class RenderSystem
{
	sf::RenderWindow& window;
	std::vector<std::reference_wrapper<object>> objs;
	sf::Sprite back;
	sf::Texture back_t;
public:

	RenderSystem(sf::RenderWindow& ref): window{ref}
	{

	}
	void update()
	{
		window.clear();
		window.draw(back);
		for (auto el: objs)
		{
			window.draw(el.get());////////////
		}
		window.display();
	}
	void add(object& some)
	{
		objs.push_back(std::ref(some));
	}
	void setBackground(sf::Texture tex)
	{
		back_t.swap(tex);
		back.setTexture(back_t);
	}


};



class SystemUpdateMechanic
{
	constexpr static inline float G = 10;
	static 	vector2df Force(const object& lhs, const object& rhs)
	{
		return (rhs.pos - lhs.pos) * ((G * lhs.mass * rhs.mass) / (std::pow((lhs.pos - rhs.pos).abs(), 3)));
	}
	std::vector<object*> entity;
public:

	void update(double dt)
	{
		std::vector<vector2df> Fsum; Fsum.resize(entity.size());

		for (int i = 0; i < entity.size(); ++i)
		{
			for (int j = 0; j < entity.size(); ++j)
			{
				if (i == j) continue;
				Fsum[i] += Force(*entity[i],*entity[j]);
			}
		}
		for (int i = 0; i < entity.size(); ++i)
		{
			entity[i]->vel = entity[i]->vel + Fsum[i] * dt / entity[i]->mass;
			entity[i]->setPosition(entity[i]->pos + entity[i]->vel * dt);
		}
	}
	void add(object& some)
	{
		entity.push_back(&some);
	}

};







int main()
{
	const int width = 1920, height = 1080;
	sf::RenderWindow window(sf::VideoMode(width, height), "Test");
	window.setFramerateLimit(144);
	SystemUpdateMechanic ph;
	RenderSystem rend(window);
	rend.setBackground([] {sf::Texture t; t.loadFromFile("gamedata\\space.png"); return t; }());
	float dt = 0.001;
	object Sun({ 500,500 }, { 0,7000 }, 1000, 35, []() {sf::Texture t; t.loadFromFile("gamedata\\player.png"); return t; }());
	object Sun2({ width/2,height/2 }, { 0,0 }, 3000000000, 150, []() {sf::Texture t; t.loadFromFile("gamedata\\sunPic.png"); return t; }());

	ph.add(Sun2);
	rend.add(Sun2);
	ph.add(Sun);
	rend.add(Sun);
	while (window.isOpen())
	{
		sf::Event ev;
		while (window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
			{
				window.close();
			}
			if (ev.type == sf::Event::MouseWheelMoved)
			{
				dt *= std::pow(1.2, ev.mouseWheel.delta);
				dt = std::clamp(dt, 0.000035f, 0.006f);
				//Sun.representation.setRadius(Sun.representation.getRadius() + ev.mouseWheel.delta);
			}
		}
		Sun.representation.rotate(1);
		//std::cout << dt << '\n';
		ph.update(dt);
		rend.update();
	}
	return 0;
}
