#pragma once
enum class ButtonState {
    Normal,
    Highlighted,
    Pressed,
};
class Window {
public:
    Window(SDL_Rect rect, SDL_Renderer* renderer, TTF_Font* font);
    void SetTexture(SDL_Texture* texture);
    void Draw();
    void SetText(const char* text);
private:
    SDL_Texture* m_texture;
    SDL_Renderer* m_renderer;
    SDL_Rect m_rect;
    TTF_Font* m_font;
    SDL_Texture* m_textTexture;
protected:
    Window();
};
class Button : public Window {
public:
    Button(SDL_Rect rect, SDL_Renderer* renderer, TTF_Font* font);
    void SetText(const char* text);
    void Draw();
    void SetTexture(SDL_Texture* texture);
    bool HandleEvent(SDL_Event& event);
private:
    SDL_Rect m_rect;
    SDL_Texture* m_texture;
    SDL_Texture* m_textureHighlighted;
    SDL_Renderer* m_renderer;
    TTF_Font* m_font;
    SDL_Texture* m_textTexture;
    ButtonState m_state;
};