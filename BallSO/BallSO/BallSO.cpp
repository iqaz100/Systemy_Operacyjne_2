#include "pch.h"
#include <iostream>
#include <curses.h>
#include <thread>  //watki
#include <cstdlib>  //losowanie liczby
#include <Windows.h>  //zamienic na #include <unistd.h> 
//pubs.opengroup.org/onlinepubs/007908799/xsh/usleep.html
#include <vector>

using namespace std;

	static const int WINDOW_WIDTH = 80, WINDOW_HEIGHT = 24;
	bool cancel = false;
	int numberOfBalls = 30;

	class Ball;

	vector <Ball> balls;
	vector <thread> ballThreads;

	class Ball
	{
	public:
		static const char symbol = '*';
		int x, y, dx, dy;
		int speed;

		int getSpeed()
		{
			return speed;
		}

		Ball() { x = -1; y = -1; }

		int direction_set()
		{
			int direction;
			srand(time(NULL));
			direction = rand() % 7;

			switch (direction)
			{
			case 0:
				dx = -2;
				dy = 1;
				break;
			case 1:
				dx = -1;
				dy = 1;
				break;
			case 2:
				dx = -1;
				dy = 2;
				break;
			case 3:
				dx = 0;
				dy = 1;
				break;
			case 4:
				dx = 1;
				dy = 2;
				break;
			case 5:
				dx = 1;
				dy = 1;
				break;
			case 6:
				dx = 2;
				dy = 1;
				break;
			default:
				break;
			}
			return dx, dy;
		}

		void update_ball() 
		{
			const auto fx = x + dx;
			const auto fy = y + dy;

			if ((fx <= 0) || (fx >= WINDOW_WIDTH - 1))
			{
				dx = -dx;
				slowingDown(speed);
			}


			if ((fy <= 0 )||( fy >= WINDOW_HEIGHT - 1))
			{
				dy = -dy;
				slowingDown(speed);
			}


			x += dx;
			y += dy;
		}

		int slowingDown(int speedBall)
		{
			speed = speed * 2;
			return speedBall;
		}

		int checkSpeed()
		{
			if (speed > 999)
			{
				return false;
			}
			return true;
		}

		void poistionAndSpeedSet()
		{
			x = 40; y = 1; speed = 100;
		}

		void runBall()
		{
			poistionAndSpeedSet();
			direction_set();
			while (checkSpeed())
			{
				update_ball();	
				Sleep(speed); //zamienic na usleep w linux
			}

			if (!checkSpeed())
			{
				x = -1;
				y = -1;
			}


		}

		void show()
		{
			mvaddch(y, x, symbol);
		}

		void stopBall()
		{
			speed = 10000;
		}
	};

	void printBalls()
	{
		while (true)
		{
			refresh();
			Sleep(100);
			clear();

			
			border(0, 0, 0, 0, 0, 0, 0, 0);
			for (int i = 0; i < balls.size(); i++)
				balls.at(i).show();
		}
	}

	void addBall()
	{
		balls.push_back(Ball());
		//thread *th = new thread (runBall, ball, )
	}

	void addThread()
	{
		ballThreads.emplace_back(&Ball::runBall, &(balls.at(ballThreads.size())));
		//ballThreads.push_back(thread(&Ball::runBall, &balls.back()));
	}

	void quit() 
	{
		if (getch() == 'q') {
			cancel = true;

			for (auto &b : balls)
			{
				b.stopBall();
			}

			for (auto &ballThread : ballThreads)
			{
				ballThread.join();
			}
			Sleep(1000);
			ballThreads.clear();
			balls.clear();
			exit(0);
		}
	}

	int main()
	{
		
		initscr();
		curs_set(0);	//ukrycie kursora

		for (int i = 0; i < numberOfBalls; i++)
			addBall();

		thread quitMonitor(quit);
		thread thMonitor(printBalls);

		for (int i = 0; i < numberOfBalls; i++)
		{
			if (cancel)
				break;
			addThread();
			Sleep(1653);
		}

		quitMonitor.join();
		thMonitor.join();



		endwin();

		return 0;
	}
