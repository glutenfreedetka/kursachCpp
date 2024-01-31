#include "button.h"
Window::Window() = default;

Window::Window(SDL_Rect rect, SDL_Renderer* renderer, TTF_Font* font)
    :m_texture(nullptr), m_renderer(renderer), m_rect(rect), m_font(font), m_textTexture(nullptr)
{
}
void Window::SetTexture(SDL_Texture* texture) {
    m_texture = texture;
}

void Window::Draw() {
    SDL_RenderCopy(m_renderer, m_texture, nullptr, &m_rect);
}
void Window::SetText(const char* text)
{
    setlocale(LC_ALL, "Russian");
    if (m_textTexture)
    {
        SDL_DestroyTexture(m_textTexture);
        m_textTexture = nullptr;
    }

    SDL_Surface* surface = TTF_RenderUTF8_Solid(m_font, text, { 0, 0, 0 });
    if (!surface)
    {
        SDL_Log("Failed to create text surface: %s", TTF_GetError());
        return;
    }

    m_textTexture = SDL_CreateTextureFromSurface(m_renderer, surface);
    if (!m_textTexture)
    {
        SDL_Log("Failed to create text texture: %s", SDL_GetError());
        return;
    }

    SDL_FreeSurface(surface);
}
Button::Button(SDL_Rect rect, SDL_Renderer* renderer, TTF_Font* font)
    : m_rect(rect), m_texture(nullptr), m_renderer(renderer), m_font(font), m_textTexture(nullptr)
{
}
void Button::SetText(const char* text)
{
    setlocale(LC_ALL, "Russian");
    if (m_textTexture)
    {
        SDL_DestroyTexture(m_textTexture);
        m_textTexture = nullptr;
    }
    
    SDL_Surface* surface = TTF_RenderText_Blended(m_font, text, { 0, 0, 0 });
    if (!surface)
    {
        SDL_Log("Failed to create text surface: %s", TTF_GetError());
        return;
    }

    m_textTexture = SDL_CreateTextureFromSurface(m_renderer, surface);
    if (!m_textTexture)
    {
        SDL_Log("Failed to create text texture: %s", SDL_GetError());
        return;
    }

    SDL_FreeSurface(surface);
}
void Button::SetTexture(SDL_Texture* texture) {
    m_texture = texture;
}
bool Button::HandleEvent(SDL_Event& event)
{
    if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
    {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        bool inside = (mouseX >= m_rect.x && mouseX < (m_rect.x + m_rect.w) && mouseY >= m_rect.y && mouseY < (m_rect.y + m_rect.h));

        if (inside)
        {
            if (event.type == SDL_MOUSEMOTION)
            {
                m_state = ButtonState::Highlighted;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                m_state = ButtonState::Pressed;
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                m_state = ButtonState::Highlighted;
                return true;
            }
        }
        else
        {
            m_state = ButtonState::Normal;
        }

        return false;
    }

    return false;
}
void Button::Draw()
    {
        if (m_texture)
        {
            switch (m_state)
            {
            case ButtonState::Normal:
                SDL_RenderCopy(m_renderer, m_texture, nullptr, &m_rect);
                break;

            case ButtonState::Highlighted:
                SDL_RenderCopy(m_renderer, m_textureHighlighted, nullptr, &m_rect);
                break;

                /*case ButtonState::Pressed:
                    SDL_SetTextureColorMod(m_texture, 150, 150, 150);
                    SDL_RenderCopy(m_renderer, m_texture, nullptr, &m_rect);
                    break;*/
            }
        }

        if (m_textTexture)
        {
            int textWidth = 0;
            int textHeight = 0;

            SDL_QueryTexture(m_textTexture, nullptr, nullptr, &textWidth, &textHeight);

            SDL_Rect textRect{ m_rect.x + (m_rect.w - textWidth) / 2, m_rect.y + (m_rect.h - textHeight) / 2, textWidth, textHeight };

            SDL_RenderCopy(m_renderer, m_textTexture, nullptr, &textRect);
        }
    }