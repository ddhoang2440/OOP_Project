#ifndef BULLET_H
#define BULLET_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

const int ARROW_WIDTH = 20; // Giả định kích thước đạn
const int ARROW_HEIGHT = 40;
class Bullet:public Entity{
    protected:
        sf::Texture texture_;
        bool can_move_;
        bool alive_;
        float direction_; // Góc di chuyển (độ)
        float speed_;     // Tốc độ di chuyển (pixel/giây)
        int damage_;     // Sát thương của đạn
        sf::Clock lifetime_clock_; // Thời gian sống của đạn
        int lifetime_max_; // Thời gian sống tối đa (ms)
    public:
        enum BulletType
        {
            ARROW = 1,
            BORON = 2,
            NEURON = 3
        };
        BulletType type_;

        Bullet();
        virtual ~Bullet();

        // Setter và Getter cho sprite và vị trí
        void setpos(float x, float y);
        void setrotation(float theta);
        float getrotation() const;
        sf::Sprite getSprite() const;

        // Setter và Getter cho các thuộc tính khác
        void set_type(BulletType type) { type_ = type; }
        BulletType get_type() const { return type_; }
        void set_can_move(bool can_move) { can_move_ = can_move; }
        bool get_can_move() const { return can_move_; }
        void set_speed(float speed) { speed_ = speed; }
        float get_speed() const { return speed_; }
        void set_damage(int damage) { damage_ = damage; }
        int get_damage() const { return damage_; }
        void set_alive(bool alive) { alive_ = alive; }
        bool is_alive() const { return alive_; }
        void set_direction(float direction) { direction_ = direction; }
        float get_direction() const { return direction_; }

        // Load texture
        virtual bool load_static_bullet_picture(const std::string& file_path);

        // Update và render
        virtual void update(float deltaTime);
        virtual void render(sf::RenderWindow& window);

        // Get kích thước và vị trí cho va chạm
        sf::FloatRect get_rect() const { return anim_.sprite_.getGlobalBounds(); }
};
// CLASS ARROW BULLET
class ArrowBullet : public Bullet
{
public:
    ArrowBullet()
    {
        type_ = ARROW;
        speed_ = 10.0f;
        damage_ = 1;
        load_static_bullet_picture("res/image/arrow.png");
    }
};

// CLASS BLASER BULLET
// class BlaserBullet : public Bullet
// {
// public:
//     BlaserBullet()
//     {
//         type_ = BLASER;
//         speed_ = 15.0f;
//         damage_ = 2;
//         load_static_bullet_picture("res/image/blaser.png");
//     }
// };

// CLASS BORON BULLET
class BoronBullet : public Bullet
{
public:
    BoronBullet()
    {
        type_ = BORON;
        speed_ = 10.0f;
        damage_ = 2; // Mặc định, có thể điều chỉnh theo ammo_level
        load_static_bullet_picture("res/image/BORON0.png");
    }
};

// CLASS NEURON BULLET
class NeuronBullet : public Bullet
{
public:
    NeuronBullet()
    {
        type_ = NEURON;
        speed_ = 20.0f;
        damage_ = 1; // Mặc định, có thể điều chỉnh theo ammo_level
        load_static_bullet_picture("res/image/NEUTRON0.png");
    }
};

#endif