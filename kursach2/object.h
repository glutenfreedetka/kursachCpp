#pragma once
class MovingObject {
public:
    MovingObject(SDL_Renderer* renderer, int x, int y, int w, int h);

    virtual void move();

    void draw();
    SDL_Rect rect;

protected:
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    
};
class Snowball : public MovingObject {
public:
    Snowball(SDL_Renderer* renderer, int x, int y, int w, int h);

    void move() override;
};
class Player : public MovingObject {
public:
    Player(SDL_Renderer* renderer, int x, int y, int w, int h);

    void moveright();
    void moveleft();
};