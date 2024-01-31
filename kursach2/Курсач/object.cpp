#include "object.h"
MovingObject::MovingObject(SDL_Renderer* renderer, int x, int y, int w, int h) : renderer(renderer) {
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}
void MovingObject::draw() {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
void MovingObject::move() {

}
Snowball::Snowball(SDL_Renderer* renderer, int x, int y, int w, int h) : MovingObject(renderer, x, y, w, h) {
    SDL_Surface* surface = SDL_LoadBMP("snowball2.bmp");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}
void Snowball::move()  {
    rect.y += 5;
}
Player::Player(SDL_Renderer* renderer, int x, int y, int w, int h) : MovingObject(renderer, x, y, w, h) {
    SDL_Surface* surface = IMG_Load("playerr.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}
void Player::moveright();