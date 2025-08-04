#include"Bullet.h"
// IMPLEMENTATION
Bullet::Bullet()
{
    can_move_ = true;
    alive_ = true;
    direction_ = 0.0f; // Mặc định hướng lên trên
    speed_ = 0.0f;
    damage_ = 0;
    lifetime_max_ = 5000; // 5 giây
    type_ = ARROW;
}

Bullet::~Bullet()
{
}

void Bullet::setpos(float x, float y)
{
    sprite_.setPosition(x, y);
}

void Bullet::setrotation(float theta)
{
    sprite_.setRotation(theta);
}

float Bullet::getrotation() const
{
    return sprite_.getRotation();
}

sf::Sprite Bullet::getSprite() const
{
    return sprite_;
}

bool Bullet::load_static_bullet_picture(const std::string& file_path)
{
    if (!texture_.loadFromFile(file_path)) {
        std::cerr << "Error: Could not load " << file_path << std::endl;
        return false;
    }
    sprite_.setTexture(texture_);
    sprite_.setOrigin(texture_.getSize().x / 2.0f, texture_.getSize().y / 2.0f);
    return true;
}

void Bullet::update(float deltaTime)
{
    if (!can_move_ || !alive_) return;

    // Di chuyển đạn theo hướng và tốc độ
    float rad = direction_ * 3.14159265f / 180.0f;
    sprite_.move(std::cos(rad) * speed_ * deltaTime, -std::sin(rad) * speed_ * deltaTime);

    // Kiểm tra thời gian sống
    if (lifetime_clock_.getElapsedTime().asMilliseconds() > lifetime_max_)
    {
        alive_ = false;
    }

    // Kiểm tra ra khỏi màn hình
    sf::FloatRect bounds = sprite_.getGlobalBounds();
    if (bounds.left < 0 || bounds.left + bounds.width > 1920 ||
        bounds.top < 0 || bounds.top + bounds.height > 1080)
    {
        alive_ = false;
    }
}

void Bullet::render(sf::RenderWindow& window)
{
    if (alive_)
    {
        window.draw(sprite_);
    }
};

