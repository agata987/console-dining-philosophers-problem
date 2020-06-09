#include <iostream>
#include <array>
#include <csignal>
#include <cstdio>

#include "include/Philosopher.h"

void make_hungry(bool *stop, int * hunger, bool * started)
{
	while(!*stop)
	{
		for(int i=0;i<3;i++)
		if(!*stop)
		std::this_thread::sleep_for(std::chrono::milliseconds(300));

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		for(int i=0;i<5;i++)
		if(hunger[i]>0 && started[i])
		hunger[i] = hunger[i]-10;
	}
}

bool * stop = new bool(false);

void stop_()
{
	char stop_c='a';

	do
	{
		std::cin>>stop_c;

	} while (!(stop_c=='Q'||stop_c=='q'));
	
	*stop = true;
}

int main()
{

	initscr();
	start_color();
	init_pair(1,COLOR_GREEN,COLOR_BLACK);
	init_pair(2,COLOR_RED,COLOR_BLACK);
	init_pair(3,COLOR_BLUE,COLOR_BLACK);


	if(!has_colors())
	{
		move(2,3);
		printw("Terminal does not support color");
		getch();
		return -1;
	}


	Fork forks[5];

	// tablica (wskaznik glodu)		0 - zaglodzony		100- najedzony	<40 - stan krytyczny
	// z kazda sekunda wskaznik spada o 10
	int * hunger = new int[5];

	int * progress = new int[5];
	bool * started = new bool[5];

	for(int i=0;i<5;i++)
	{
		progress[i]=0;

		// wasrtosc poczatkowa glodu: 100
		hunger[i]=100;

		started[i] = false;
	}

	std::array<Philosopher, 5> philosophers =
	{{
		{"Arystoteles", forks[0], forks[1],stop, hunger,0,progress,started},
		{"Sokrates", forks[1], forks[2],stop, hunger,1,progress,started},
		{"Pitagoras", forks[2], forks[3],stop, hunger,2,progress,started},
		{"Platon", forks[3], forks[4],stop, hunger,3,progress,started},
		{"Demokrates", forks[4], forks[0],stop, hunger,4,progress,started},
	}};

	std::thread hunger_t(make_hungry,stop,hunger,started);
	std::thread stop_t(stop_);

	int fork_n;


	while(!*stop)
	{	

		for(int i=0;i<5;i++)
		{
			move(4,9);
			printw("hunger");

			move(6+2*i,18);
			printw(philosophers[i].name.c_str());

			move(6+2*i,10);
			if(hunger[i]<100 && hunger[i]>=10)
				printw("%d  %% ",hunger[i]);
			else if(hunger[i]>=100)
				printw("%d %%  ",hunger[i]);
			else
				printw("%d   %%",hunger[i]);

			move(6+2*i,32);

			if(philosophers[i].currentState==2)
			{
					attron(COLOR_PAIR(1));
					printw(philosophers[i].states[philosophers[i].currentState].c_str());
					attroff(COLOR_PAIR(1));

			} else if (philosophers[i].currentState==0)
			{		attron(COLOR_PAIR(3));
					printw(philosophers[i].states[philosophers[i].currentState].c_str());
					attroff(COLOR_PAIR(3));
			} else
			printw(philosophers[i].states[philosophers[i].currentState].c_str());

			move(4,44);
			printw("progress");
			
			move(6+2*i,46);
			if(philosophers[i].currentState != 0)
			{
				if(progress[i]<100 && progress[i]>=10)
					printw("%d  %% ",progress[i]);
				else if(progress[i]>=100)
					printw("%d %%  ",progress[i]);
				else
					printw("%d   %%",progress[i]);
			} else 
					printw("---    ");


			move(4,56);
			printw("forks");

			fork_n=i+1;
			if(fork_n==5)
			fork_n=0;

			move(6+2*i,57);

			int sasiad_l = i-1;
			int sasiad_p = i+1;

			if(sasiad_l<0)
			sasiad_l=4;

			if(sasiad_p>4)
			sasiad_p=0;

			if(philosophers[i].currentState==2)
			{
				attron(COLOR_PAIR(1));
				printw("%d %d",i,fork_n);
				attroff(COLOR_PAIR(1));
			}else 
			{
				if(philosophers[sasiad_l].currentState==2 && philosophers[i].currentState==0)
				{
					attron(COLOR_PAIR(2));
					printw("%d",i);
					attroff(COLOR_PAIR(2));
				} else
				{
					printw("%d",i);
				}
				
				move(6+2*i,59);

				if(philosophers[sasiad_p].currentState==2 && philosophers[i].currentState==0)
				{
					attron(COLOR_PAIR(2));
					printw("%d",fork_n);
					attroff(COLOR_PAIR(2));
				} else
				{
					printw("%d",fork_n);
				}
				

			}
			
			move(16,63);
			printw("*");
		
		}	
		refresh();
	}


	endwin();
	std::cout<<"Exiting..."<<std::endl;

	if(hunger_t.joinable())
	hunger_t.join();


    return 0;
}
