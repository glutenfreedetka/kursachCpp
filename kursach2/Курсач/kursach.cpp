#include <iostream>
#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <functional>
#include <SDL_ttf.h>
#include "button1.cpp"
#include <SDL_image.h>
#include <sstream>
#include <list>
#include <iomanip>
using namespace std;
struct elem {
	string name;
	int rate;
};
using namespace std;
const int SCREEN_WIDTH = 1200; // Ширина окна
const int SCREEN_HEIGHT = 675; // Высота окна
const int PLAYER_WIDTH = 120; // Ширина игрока
const int PLAYER_HEIGHT = 120; // Высота игрока
const int SNOWBALL_SIZE = 50; // Размер снежка

bool hasIntersection(const SDL_Rect& rect1, const SDL_Rect& rect2) {
	return !(rect1.x + rect1.w <= rect2.x || rect2.x + rect2.w <= rect1.x ||
		rect1.y + rect1.h <= rect2.y || rect2.y + rect2.h <= rect1.y);
}
bool compare(const elem& left, const elem& right)
{
	return left.rate < right.rate;
}

void saveScores(const std::vector<int>& scores) {
	std::ofstream file("scores.txt");
	if (file.is_open()) {
		for (int score : scores) {
			file << score << std::endl;
		}
		file.close();
	}
}
vector<elem> read_file(const std::string& filename) {
	std::vector<elem> result;
	std::ifstream file(filename);
	if (file.is_open()) {
		std::string line;
		while (getline(file, line)) {
			elem row;
			std::stringstream ss(line);
			ss >> row.name >> row.rate;
			result.push_back(row);
		}
		file.close();
	}
	return result;
}
void write_file(const string& filename, const std::vector<elem>& data_vec) {
	std::ofstream file(filename);
	if (file.is_open()) {
		for (const auto& row : data_vec) {
			file << row.name << " " << row.rate << std::endl;
		}
		file.close();
	}
	else {
		std::cout << "Unable to open file for writing!" << std::endl;
	}
}

void write_to_print(const string& filename, const std::vector<elem>& data_vec) {
	std::ofstream file(filename);
	if (file.is_open()) {
		for (const auto& row : data_vec) {
			file << row.name << "\t| " << row.rate << std::endl;
		}
		file.close();
	}
	else {
		std::cout << "Unable to open file for writing!" << std::endl;
	}
}

bool isMouseOver(SDL_Rect rect) {
	int x, y;
	SDL_GetMouseState(&x, &y);
	return (x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h);
}

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO); // Инициализация SDL
	TTF_Init();
	setlocale(LC_ALL, "rus");
	SDL_Window* window = SDL_CreateWindow("Snow on a head", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	// Создание рендера
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Surface* surface = SDL_LoadBMP("backgr.bmp");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_Surface* surface1 = SDL_LoadBMP("ingame.bmp");
	SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
	SDL_Surface* surface2 = SDL_LoadBMP("ingame2.bmp");
	SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
	SDL_Surface* surfaceinf = SDL_LoadBMP("info.bmp");
	SDL_Texture* textureinf = SDL_CreateTextureFromSurface(renderer, surfaceinf);
	SDL_Surface* surfaceover = IMG_Load("gameover.png");
	SDL_Texture* textureover = SDL_CreateTextureFromSurface(renderer, surfaceover);
	SDL_Surface* surface3 = IMG_Load("ingamen.png");
	SDL_Texture* texture3 = SDL_CreateTextureFromSurface(renderer, surface3);
	SDL_Surface* surfaceovern = IMG_Load("gameovern.png");
	SDL_Texture* textureovern = SDL_CreateTextureFromSurface(renderer, surfaceovern);
	SDL_Surface* surfaceent = IMG_Load("enterback.png");
	SDL_Texture* textureent = SDL_CreateTextureFromSurface(renderer, surfaceent);
	SDL_Surface* surfacesc = IMG_Load("records.png");
	SDL_Texture* texturesc = SDL_CreateTextureFromSurface(renderer, surfacesc);
	SDL_FreeSurface(surface);
	SDL_FreeSurface(surface1);
	SDL_FreeSurface(surfaceinf);

	// Создание текстуры для игрока
	SDL_Surface* playerSurfacer = IMG_Load("playerr.png");
	SDL_Surface* playerSurfacel = IMG_Load("playerl.png");
	SDL_Texture* playerTexturer = SDL_CreateTextureFromSurface(renderer, playerSurfacer);
	//SDL_FreeSurface(playerSurfacer);

	// Создание текстуры для снежка
	SDL_Surface* snowballSurface =
		SDL_LoadBMP("snowball2.bmp");
	SDL_Texture* snowballTexture = SDL_CreateTextureFromSurface(renderer, snowballSurface);
	SDL_FreeSurface(snowballSurface);


	TTF_Font* font = TTF_OpenFont("mcfont.ttf", 24);
	SDL_Color text_color = { 255,255,255 };

	Button playbutton({ SCREEN_WIDTH / 2 - 150, 200, 300, 50 }, renderer, font);
	SDL_Surface* surfaceplay = IMG_Load("button.png");
	SDL_Texture* textureplay = SDL_CreateTextureFromSurface(renderer, surfaceplay);
	playbutton.SetTexture(textureplay);
	playbutton.SetText("Play");

	Button changebutton({ SCREEN_WIDTH / 2 - 150, 270, 300, 50 }, renderer, font);
	SDL_Surface* surfacechange = IMG_Load("button.png");
	SDL_Texture* texturechange = SDL_CreateTextureFromSurface(renderer, surfacechange);
	changebutton.SetTexture(texturechange);
	changebutton.SetText("Change player");

	Button ratebutton({ SCREEN_WIDTH / 2 - 150, 340, 300, 50 }, renderer, font);
	SDL_Surface* surfacerate = IMG_Load("button.png");
	SDL_Texture* texturerate = SDL_CreateTextureFromSurface(renderer, surfacerate);
	ratebutton.SetTexture(texturerate);
	ratebutton.SetText("Scores");

	Button infobutton({ SCREEN_WIDTH / 2 - 150, 410, 300, 50 }, renderer, font);
	SDL_Surface* surfaceinfo = IMG_Load("button.png");
	SDL_Texture* textureinfo = SDL_CreateTextureFromSurface(renderer, surfaceinfo);
	infobutton.SetTexture(textureinfo);
	infobutton.SetText("Info");

	Button optionsbutton({ SCREEN_WIDTH / 2 - 150, 480, 300, 50 }, renderer, font);
	SDL_Surface* surfaceopt = IMG_Load("button.png");
	SDL_Texture* textureopt = SDL_CreateTextureFromSurface(renderer, surfaceopt);
	optionsbutton.SetTexture(textureopt);
	optionsbutton.SetText("Options");

	Button escbutton({ SCREEN_WIDTH / 2 - 150, 550, 300, 50 }, renderer, font);
	SDL_Surface* surfaceesc = IMG_Load("button.png");
	SDL_Texture* textureesc = SDL_CreateTextureFromSurface(renderer, surfaceesc);
	escbutton.SetTexture(textureesc);
	escbutton.SetText("Exit");

	Window score({ SCREEN_WIDTH / 2 - 300, 300, 600, 300 }, renderer, font);
	SDL_Surface* surfacescore = IMG_Load("button.png");
	SDL_Texture* texturescore = SDL_CreateTextureFromSurface(renderer, surfacescore);
	score.SetTexture(texturescore);
	score.SetText("Game over! Your score:");

	Button okbutton({ SCREEN_WIDTH / 2 - 150, 600, 300, 50 }, renderer, font);
	SDL_Surface* surfaceok = IMG_Load("button.png");
	SDL_Texture* textureok = SDL_CreateTextureFromSurface(renderer, surfaceok);
	okbutton.SetTexture(textureok);
	okbutton.SetText("OK");

	Button printbutton({ SCREEN_WIDTH / 2 + 70, 600, 300, 50 }, renderer, font);
	SDL_Surface* surfacepr = IMG_Load("button.png");
	SDL_Texture* texturepr = SDL_CreateTextureFromSurface(renderer, surfacepr);
	printbutton.SetTexture(texturepr);
	printbutton.SetText("Prepare to print");

	Button backbutton({ SCREEN_WIDTH / 2 - 320, 600, 300, 50 }, renderer, font);
	SDL_Surface* surfaceback = IMG_Load("button.png");
	SDL_Texture* textureback = SDL_CreateTextureFromSurface(renderer, surfaceback);
	backbutton.SetTexture(textureback);
	backbutton.SetText("Back");

	Button backbutton1({ SCREEN_WIDTH / 2 - 150, 600, 300, 50 }, renderer, font);
	SDL_Surface* surfaceback1 = IMG_Load("button.png");
	SDL_Texture* textureback1 = SDL_CreateTextureFromSurface(renderer, surfaceback1);
	backbutton1.SetTexture(textureback);
	backbutton1.SetText("Back");

	Button chooseday({ SCREEN_WIDTH / 2 - 250, 200, 500, 100 }, renderer, font);
	SDL_Surface* surfacech = IMG_Load("button.png");
	SDL_Texture* texturech = SDL_CreateTextureFromSurface(renderer, surfacech);
	chooseday.SetTexture(texturech);
	chooseday.SetText("Choose a day mode");

	Button day({ SCREEN_WIDTH / 2 - 350, 400, 300, 50 }, renderer, font);
	SDL_Surface* surfaced = IMG_Load("button.png");
	SDL_Texture* textured = SDL_CreateTextureFromSurface(renderer, surfaced);
	day.SetTexture(textured);
	day.SetText("Day");

	Button night({ SCREEN_WIDTH / 2 + 40, 400, 300, 50 }, renderer, font);
	SDL_Surface* surfacen = IMG_Load("button.png");
	SDL_Texture* texturen = SDL_CreateTextureFromSurface(renderer, surfacen);
	night.SetTexture(textured);
	night.SetText("Night");

	int playerX = (SCREEN_WIDTH - PLAYER_WIDTH) / 2; // Начальная позиция игрока
	std::vector<SDL_Rect> snowballs; // Вектор для хранения снежков

	bool isGameOver = false;
	bool isGameStarted = false;
	bool esc = false;
	bool info = false;
	bool change = false;
	bool rate = false;
	bool over = false;
	bool options = false;
	bool records = true;
	bool isday = true;
	int k = 2;
	int rates = 0;
	SDL_Color color = { 0, 0, 0 }; // цвет текста
	string input;
	vector<elem> mylist;
	mylist = read_file("scores.txt");

	srand(time(0)); // Инициализация генератора случайных чисел

	// Загрузка рекордов

	// Основной игровой цикл
	while (!esc) {
		//bool isGameOver = false;
		//bool isGameStarted = false;
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				isGameStarted = false;
				SDL_RenderClear(renderer);
				snowballs.clear();
				playerX = (SCREEN_WIDTH - PLAYER_WIDTH) / 2;
				rates = 0;
			}
			if (escbutton.HandleEvent(event))
				esc = true;
			if (playbutton.HandleEvent(event)) {
				isGameStarted = true;
				rates = 0;
			}
			if (infobutton.HandleEvent(event))
				info = true;
			if (changebutton.HandleEvent(event))
				change = true;
			if (ratebutton.HandleEvent(event))
				rate = true;
			if (optionsbutton.HandleEvent(event))
				options = true;

		}
		while (info) {
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer,
				textureinf, NULL, NULL);
			okbutton.Draw();
			SDL_RenderPresent(renderer);
			SDL_Event event1;
			while (SDL_PollEvent(&event1))
				if (okbutton.HandleEvent(event1))
					info = false;
		}
		while (options) {
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			chooseday.Draw();
			day.Draw();
			night.Draw();
			backbutton1.Draw();
			SDL_RenderPresent(renderer);
			SDL_Event event1;
			while (SDL_PollEvent(&event1)) {
				if (backbutton1.HandleEvent(event1))
					options = false;
				if (day.HandleEvent(event1))
					isday = true;
				if (night.HandleEvent(event1))
					isday = false;
			}
		}
		while (change) {
			SDL_RenderCopy(renderer, textureent, NULL, NULL);
			SDL_Rect rect = { SCREEN_WIDTH / 2 - 195, 235, 420, 70 };
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &rect);
			SDL_RenderPresent(renderer);
			SDL_Rect nickRect;
			SDL_Surface* nickSurf;
			SDL_Texture* nickTex;
			SDL_Color col = { 0, 0, 0 };
			string in;
			int i = 0;
			bool flag = false;
			bool changename = true;
			SDL_StartTextInput();
			SDL_Event event1;
			while (changename) {
				while (SDL_PollEvent(&event1)) {
					if (event1.key.keysym.sym == SDLK_ESCAPE)
						change = false;
					if (event1.key.keysym.sym == SDLK_RETURN)
						changename = false;
					if (event1.type == SDL_TEXTINPUT) {
						if (i < 10) {
							in += event1.text.text;
							i++;
						}
						const char* change1 = in.c_str();
						nickSurf = TTF_RenderText_Solid(font, change1, col);
						nickTex = SDL_CreateTextureFromSurface(renderer, nickSurf);
						nickRect = { 300, 100, 200, 200 };
						SDL_RenderCopy(renderer, textureent, NULL, NULL);
						SDL_RenderFillRect(renderer, &rect);
						SDL_RenderCopy(renderer, nickTex, NULL, &rect);
						if (flag) {
							SDL_DestroyTexture(nickTex);
							//SDL_RenderPresent(renderer);
						}
						SDL_RenderPresent(renderer);
					}
					else if (event1.type == SDL_KEYDOWN && event1.key.keysym.sym == SDLK_BACKSPACE && in.size()) {
						in.pop_back();
						flag = true;
						i--;
						nickSurf = TTF_RenderText_Solid(font, in.c_str(), col);
						nickTex = SDL_CreateTextureFromSurface(renderer, nickSurf);
						nickRect = { 300, 100, 200, 200 };
						SDL_RenderCopy(renderer, textureent, NULL, NULL);
						SDL_RenderFillRect(renderer, &rect);
						SDL_RenderCopy(renderer, nickTex, NULL, &rect);
						SDL_RenderPresent(renderer);
					}
				}
			}
			SDL_StopTextInput();
			input = in;
			int ks = sizeof(input);
			const char* changes = input.c_str();
			nickSurf = TTF_RenderText_Solid(font, changes, col);
			nickTex = SDL_CreateTextureFromSurface(renderer, nickSurf);
			nickRect = { 300, 300, 200, 200 };
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, textureent, NULL, NULL);
			SDL_RenderFillRect(renderer, &rect);
			SDL_RenderCopy(renderer, nickTex, NULL, &rect);
			SDL_RenderPresent(renderer);
			change = false;
		}
		while (rate) {
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texturesc, NULL, NULL);
			printbutton.Draw();
			backbutton.Draw();
			SDL_RenderPresent(renderer);
			SDL_Rect nickRect; SDL_Rect scoreRect;
			SDL_Surface* nickSurf; SDL_Surface* scoreSurf;
			SDL_Texture* nickTex; SDL_Texture* scoreTex;
			char buffer[5];
			SDL_Color col = { 255, 255, 255 };
			for (int i = 0; i < mylist.size(); i++) {
				sprintf(buffer, "%d", mylist[i].rate);
				if (mylist[i].rate == 0) {
					nickSurf = TTF_RenderText_Solid(font, "---", col);
					scoreSurf = TTF_RenderText_Solid(font, buffer, col);
					nickTex = SDL_CreateTextureFromSurface(renderer, nickSurf);
					nickRect = { SCREEN_WIDTH / 4, 35 * (i + 1) + 130, 100, 30 };
					scoreTex = SDL_CreateTextureFromSurface(renderer, scoreSurf);
					if (mylist[i].rate > 1000) {
						scoreRect = { SCREEN_WIDTH / 2 + 200, 35 * (i + 1) + 130, 100, 30 };
					}
					else {
						scoreRect = { SCREEN_WIDTH / 2 + 200, 35 * (i + 1) + 130, 100, 30 };;
					}
				}
				else {
					const char* nick = mylist[i].name.c_str();
					nickSurf = TTF_RenderText_Solid(font, nick, col);
					scoreSurf = TTF_RenderText_Solid(font, buffer, col);
					if (!nickSurf && !scoreSurf) {
						cout << "Failed to render text: " << TTF_GetError() << endl;
						return 1;
					}
					nickTex = SDL_CreateTextureFromSurface(renderer, nickSurf);
					nickRect = { SCREEN_WIDTH / 4, 35 * (i + 1) + 130, 100, 30 };
					scoreTex = SDL_CreateTextureFromSurface(renderer, scoreSurf);
					if (mylist[i].rate > 1000) {
						scoreRect = { SCREEN_WIDTH / 2 + 200, 35 * (i + 1) + 130, 100, 30 };
					}
					else {
						scoreRect = { SCREEN_WIDTH / 2 + 200, 35 * (i + 1) + 130, 100, 30 };
					}
				}
				SDL_RenderCopy(renderer, nickTex, NULL, &nickRect);
				SDL_RenderCopy(renderer, scoreTex, NULL, &scoreRect);
			}
			SDL_RenderPresent(renderer);
			SDL_Delay(100);
			SDL_Event event1;
			while (SDL_PollEvent(&event1)) {
				if (backbutton.HandleEvent(event1))
					rate = false;
				if (printbutton.HandleEvent(event1))
					write_to_print("print.txt", mylist);
			}


		}
		if (isGameStarted) {
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
			if (currentKeyStates[SDL_SCANCODE_LEFT] && playerX > 0) {
				playerX -= 10; // Движение игрока влево
				playerTexturer = SDL_CreateTextureFromSurface(renderer, playerSurfacel);
			}
			if (currentKeyStates[SDL_SCANCODE_RIGHT] && playerX < SCREEN_WIDTH - PLAYER_WIDTH) {
				playerX += 10; // Движение игрока вправо
				playerTexturer = SDL_CreateTextureFromSurface(renderer, playerSurfacer);
			}
			// Генерация новых снежков
			if (rand() % 100 < 5) { // Вероятность генерации нового снежка
				if (rates > 10)
					k = 3;
				if (rates > 20)
					k = 4;
				if (rates > 30)
					k = 5;
				if (rates > 50)
					k = 6;
				int numSnowballs = rand() % k + 1; // Количество снежков от 1 до 3
				//rates += numSnowballs;
				for (int i = 0; i < numSnowballs; ++i) {
					SDL_Rect snowballRect = { rand() % (SCREEN_WIDTH - SNOWBALL_SIZE), 0, SNOWBALL_SIZE, SNOWBALL_SIZE };
					snowballs.push_back(snowballRect);
				}
			}
			// Проверка на столкновение игрока с снежками
			if (!isGameOver)
				for (const SDL_Rect& snowballRect : snowballs) {
					if (hasIntersection({ playerX, SCREEN_HEIGHT - PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT }, snowballRect)) {
						//isGameOver = true;
						elem sc;
						SDL_RenderClear(renderer);
						isGameStarted = false;
						over = true;
						snowballs.clear();
						playerX = (SCREEN_WIDTH - PLAYER_WIDTH) / 2;
						sc.rate = rates;
						if (input.empty())
							sc.name = mylist[0].name;
						else
							sc.name = input;
						mylist.push_back(sc);
						sort(mylist.rbegin(), mylist.rend(), compare);
						if (mylist.size() > 10)
							mylist.pop_back();// Ограничение до 10 рекордов
						write_file("scores.txt", mylist);
						string text = to_string(rates);
						const char* char_text = text.c_str();
						SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, char_text, color);
						SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
						SDL_FreeSurface(surfaceMessage);
						SDL_Surface* surfaceo = SDL_CreateRGBSurfaceWithFormat(0, 50, 50, 32, SDL_PIXELFORMAT_RGBA32); // создаем прозрачный прямоугольник
						SDL_Rect Background_rect = { 0, 0, 50, 50 }; // прямоугольник фона
						SDL_FillRect(surfaceo, NULL, SDL_MapRGBA(surfaceo->format, 0, 0, 0, 0)); // прозрачный цвет фона
						SDL_Texture* background = SDL_CreateTextureFromSurface(renderer, surfaceo);
						SDL_FreeSurface(surfaceo);


						SDL_RenderPresent(renderer);
						while (over) {
							if (isday)
								SDL_RenderCopy(renderer, textureover, NULL, NULL);
							else
								SDL_RenderCopy(renderer, textureovern, NULL, NULL);
							SDL_RenderCopy(renderer, background, NULL, &Background_rect);

							SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // чёрный цвет обводки прямоугольника
							//SDL_RenderDrawRect(renderer, &Background_rect);

							SDL_Rect Text_rect = { SCREEN_WIDTH / 2 - 20, 310, 50, 50 }; // прямоугольник текста с отступом 10 пикселей по каждой стороне
							SDL_RenderCopy(renderer, Message, NULL, &Text_rect);
							okbutton.Draw();
							SDL_RenderPresent(renderer);
							SDL_Event event1;
							while (SDL_PollEvent(&event1))
								if (okbutton.HandleEvent(event1))
									over = false;
						}
					}
				}


			// Обновление позиции снежков
			for (int i = 0; i < snowballs.size(); ++i) {
				snowballs[i].y += 7;
				if (snowballs[i].y >= SCREEN_HEIGHT) {
					snowballs.erase(snowballs.begin() + i);
					rates++;
					--i;
				}
			}
		}

		// Очистка экрана
		//SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		if ((!isGameStarted || isGameOver) && !info) {
			playbutton.Draw();
			changebutton.Draw();
			ratebutton.Draw();
			infobutton.Draw();
			escbutton.Draw();
			optionsbutton.Draw();
			//SDL_RenderPresent(renderer);
		}
		if (isGameStarted)
		{
			SDL_RenderClear(renderer);
			if (isday)
				SDL_RenderCopy(renderer, texture2, NULL, NULL);
			else
				SDL_RenderCopy(renderer, texture3, NULL, NULL);
			SDL_RenderPresent(renderer);
			// Отрисовка игрока
			SDL_Rect playerRect = { playerX, SCREEN_HEIGHT - PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
			SDL_RenderCopy(renderer, playerTexturer, NULL, &playerRect);

			// Отрисовка снежков
			for (const SDL_Rect& snowballRect : snowballs) {
				SDL_RenderCopy(renderer, snowballTexture, NULL, &snowballRect);
			}
		}

		// Обновление экрана
		SDL_RenderPresent(renderer);

		// Задержка для плавного обновления
		SDL_Delay(20);
	}

	// Освобождение ресурсов
	SDL_DestroyTexture(playerTexturer);
	SDL_DestroyTexture(snowballTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}