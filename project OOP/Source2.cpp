#include <SFML/Graphics.hpp> // library for graphics 2d
#include <time.h> // for srand(time(0))
#include <list> //use container list
#include <cmath> // use in sin, cos,sqrt
using namespace sf; //Để dùng các lớp của SFML mà không cần sf:: prefix

//Sprite	Đại diện cho một hình ảnh có thể vẽ được	sf::Sprite
//Texture	Lưu trữ hình ảnh dùng cho sprite	sf::Texture
//IntRect	Một hình chữ nhật nguyên dùng để cắt frame từ texture	sf::IntRect


//Một vòng lặp game = 1 lần gọi :
//├── e->update()         cập nhật logic vị trí, tốc độ
//├── e->anim.update()    cập nhật animation (nếu speed cho phép)
//├── app.draw(...)       chuẩn bị vẽ
//└── app.display()       hiển thị hình ảnh lên màn hình
//Và đoạn này chạy 60 lần / giây nếu bạn giới hạn bằng setFramerateLimit(60)


const int W = 1200; // Chiều rộng cửa sổ game
const int H = 800;  // Chiều cao cửa sổ game

float DEGTORAD = 0.017453f; // Đổi độ sang radian: 1° ≈ 0.01745 rad

class Animation // quan ly khung hinh
{
public:
    float Frame, speed; // Vị trí frame hiện tại (float để mượt hơn), và tốc độ chuyển frame
    Sprite sprite;  // Hình ảnh hiện tại của animation
    std::vector<IntRect> frames;  // Danh sách các khung hình (IntRect: hình chữ nhật trong texture)

    Animation() {}
    //  t: texture chứa các frame
    // (x, y): toạ độ frame đầu tiên
    // (w, h): kích thước 1 frame
    // count: số frame
    // Speed: tốc độ chuyển frame
    Animation(Texture& t, int x, int y, int w, int h, int count, float Speed)
    {
        Frame = 0; // gan frame and speed
        speed = Speed;

        for (int i = 0; i < count; i++)
            //Tạo danh sách các IntRect đại diện cho từng frame
            frames.push_back(IntRect(x + i * w, y, w, h)); //cắt ngang theo chiều ngang (x+i*w)

        sprite.setTexture(t);
        sprite.setOrigin(w / 2, h / 2); // Đặt tâm hình tại giữa frame để dễ xoay
        sprite.setTextureRect(frames[0]); // Hiển thị frame đầu tiên
    }


    void update() //chuyen sang khung hinh tiep theo
    {
        Frame += speed; // increase follow time 
        int n = frames.size();
        if (Frame >= n) Frame -= n; // quay vong neu toi cuoi
        if (n > 0) sprite.setTextureRect(frames[int(Frame)]); // Cập nhật frame mới
    }

    bool isEnd() //animation ket thuc
    {
        return Frame + speed >= frames.size();
    }

};


class Entity //lop cha cho moi doi tuong
{
public:
    float x, y, dx, dy, R, angle; // Vị trí, vận tốc, bán kính, góc quay
    bool life; // Đối tượng còn sống hay không
    std::string name;  // Tên (dạng chuỗi để phân biệt kiểu đối tượng)
    Animation anim; // Animation đi kèm

    Entity()
    {
        life = 1; // for one life
    }

    void settings(Animation& a, int X, int Y, float Angle = 0, int radius = 1) // khoi tao doi tuong
    {
        anim = a;
        x = X; y = Y;
        angle = Angle;
        R = radius;
    }

    virtual void update() {};  // cap nhat vi tri (kh sai se bi ghi de)

    void draw(RenderWindow& app) // ve doi tuong len man hinh
    {
        //anim.sprite là một hình ảnh (Sprite)
        anim.sprite.setPosition(x, y);// anh do se duoc dat toa do x y
        anim.sprite.setRotation(angle + 90);//xoay hinh
        app.draw(anim.sprite); // request draw into window  RenderWindow app
        //but is request not showing up right away 

        //draw circle that repersent the range within which the entity can be collided with other entities  
        CircleShape circle(R); // create circle with radius R
        circle.setFillColor(Color(255, 0, 0, 170)); // set color with alpha 170, 170  is transparent,//250,0,0 is red
        circle.setPosition(x, y); // set position of circle to the entity's position
        circle.setOrigin(R, R); // set origin of circle to the center of the circle
        //app.draw(circle);
    }
    virtual ~Entity() {};
};


class asteroid : public Entity //thien thach
{
public:
    asteroid()
    {
        dx = rand() % 8 - 4;
        dy = rand() % 8 - 4;
        name = "asteroid";
    }

    void update()
    {
        x += dx;
        y += dy;

        if (x > W) x = 0;  if (x < 0) x = W;  // Quay vòng khi ra khỏi màn hình
        if (y > H) y = 0;  if (y < 0) y = H;
    }

};


class bullet : public Entity //vien dan
{
public:
    bullet()
    {
        name = "bullet";
    }

    void  update()
    {
        dx = cos(angle * DEGTORAD) * 6; // ban theo goc cua nguoi choi
        dy = sin(angle * DEGTORAD) * 6;
        // angle+=rand()%7-3;  /*try this*/
        x += dx;
        y += dy;

        if (x > W || x<0 || y>H || y < 0) life = 0; // neu ra khoi man hinh bi xoa
    }

};


class player : public Entity
{
public:
    bool thrust; // co dang nhan nut thrust hay khong

    player()
    {
        name = "player";
    }

    void update()
    {
        if (thrust)
        {
            dx += cos(angle * DEGTORAD) * 0.2;
            dy += sin(angle * DEGTORAD) * 0.2;
        } // tang toc ne
        else
        {
            dx *= 0.99;
            dy *= 0.99;
        } // Trượt dần chậm lại nếu không nhấn

        int maxSpeed = 15; //Giới hạn tốc độ tối đa
        float speed = sqrt(dx * dx + dy * dy);
        if (speed > maxSpeed)
        {
            dx *= maxSpeed / speed;
            dy *= maxSpeed / speed;
        }

        x += dx;
        y += dy;
        //Di chuyển, tự xoay vòng nếu ra biên
        if (x > W) x = 0; if (x < 0) x = W; // Nếu ra biên trái/phải thì quay vòng
        if (y > H) y = 0; if (y < 0) y = H; // Nếu ra biên trên/dưới thì quay vòng
    }

};

class Egg : public Entity
{
public:
    Egg() {
        name = "egg";
    }
    void update() override {
        dy = 2;// toc do roi co dinh
        y += dy;//vi tri hien tai roi
        if (y > H) { // chieu cao cua cua so 
            life = false;
        }
    }
};

class Chicken : public Entity
{
public:
    float firetimer = 0;
    Animation* eggAnim = nullptr;
    std::list<Entity*>* entityList = nullptr;

    float dx = 1.0f;   // Tốc độ bay ngang
    float moveTimer = 0;

    Chicken(Animation* eggOri, std::list<Entity*>* entityListReal)
    {
        name = "chicken";
        eggAnim = eggOri;
        entityList = entityListReal;
    }

    void update() override
    {
        moveTimer += 0.1f;
        x += dx; // di chuyển ngang
        y += sin(x / 20) * 0.5f; // hiệu ứng nhấp nhô nhẹ

        // Nếu tới biên, đổi hướng và xuống hàng
        if (x < 50 || x > W - 50) {
            dx = -dx;
            y += 20; // đi xuống 1 bước
        }

        // Bắn trứng theo thời gian
        firetimer += 0.1f;
        if (firetimer > 60) {
            firetimer = 0;
            Egg* e = new Egg();
            e->settings(*eggAnim, x, y + 30, 90, 10);
            entityList->push_back(e);
        }
    }
};

class Boss : public Entity
{
public:
    Animation* egg = nullptr;
	Animation* laser = nullptr; // Animation cho tia laze
    std::list<Entity*>* entityList = nullptr;
    Boss(Animation* eggReal, std :: list<Entity*> * entityListReal, Animation* lazeReal) {
        name = "boss";
        egg = eggReal;
        entityList = entityListReal;
        laser = lazeReal;
    }
    float fireEggTimer = 0;
	float fireLaserTimer = 0; // Thời gian bắn tia laze
    int hp = 100;
	bool isUsingLaser = false; // Biến để xác định xem boss đang sử dụng tia laze hay trứng
    void update() override
    {
        y += sin(x / 40) * 0.3;

        fireEggTimer += 0.1f;
        fireLaserTimer += 0.1f;

        // Boss đổi kiểu tấn công mỗi vài giây
        if (fireLaserTimer > 200) {
            fireLaserTimer = 0;
            isUsingLaser = !isUsingLaser;
        }

        if (isUsingLaser)
        {
            if ((int)fireLaserTimer % 10 == 0) // bắn mỗi 10 frame
            {
                bullet* b = new bullet();
                b->settings(*laser, x, y + 60, 90, 10);
                b->dy = 4;
                b->dx = 0;
                entityList->push_back(b);
            }
        }
        else
        {
            if (fireEggTimer > 50)
            {
                fireEggTimer = 0;
                Egg* e = new Egg();
                e->settings(*egg, x, y + 60, 90, 15);
                entityList->push_back(e);
            }
        }
    }
    void draw(RenderWindow& app) {
        Entity::draw(app); // Gọi hàm vẽ của lớp cha để vẽ hình ảnh
        RectangleShape hpBack(Vector2f(100, 10));
        hpBack.setFillColor(Color::Red); // Màu nền của thanh máu
        hpBack.setPosition(x - 50, y - 60); // Vị trí của thanh máu

        RectangleShape hpFront(Vector2f(hp, 10));
        hpFront.setFillColor(Color::Green);
        hpFront.setPosition(x - 50, y - 60);

        app.draw(hpBack);
		app.draw(hpFront); // Vẽ thanh máu
    }
};
//So sánh khoảng cách giữa 2 tâm hình tròn và bán kính
bool isCollide(Entity* a, Entity* b)
{
    return (b->x - a->x) * (b->x - a->x) +
        (b->y - a->y) * (b->y - a->y) <
        (a->R + b->R) * (a->R + b->R);
}

// while (app.isOpen()) {
//     // 1. Bắt sự kiện (event)
//     // 2. Cập nhật logic (update)
//     // 3. Vẽ (render)
// }

int chickenWave = 0;
const int maxWaves = 3;
bool bossAppeared = false;
bool bossDefeated = false;

class GameManager {
public:
    static void spawnChickenWave(std::list<Entity*>& entities, Animation& sEgg,
        Animation& sChicken1, Animation& sChicken2, Animation& sChicken3,
        Animation& sChicken4, Animation& sChicken5, Animation& sChicken6)
    {
        for (int i = 0; i < 10; i++) {
            Chicken* c = new Chicken(&sEgg, &entities);
            int r = rand() % 6;
            Animation* a[] = { &sChicken1, &sChicken2, &sChicken3, &sChicken4, &sChicken5, &sChicken6 };
            c->settings(*a[r], 100 + i * 80, 50, 0, 25);
            entities.push_back(c);
        }
    }
};

int main()
{
    // Khởi tạo bộ sinh số ngẫu nhiên, dùng cho việc random tốc độ, hướng của thiên thạch
    srand(time(0));
    //Tạo cửa sổ SFML, giới hạn FPS
    RenderWindow app(VideoMode(W, H), "Asteroids!");
    //FPS (Frames Per Second) là số khung hình vẽ ra trong 1 giây
    app.setFramerateLimit(60);//limit program draw maximum 60 frames per second 

    Texture t1, t2, t3, t4, t5, t6, t7, t8;
    t1.loadFromFile("images/spaceship.png");
    t2.loadFromFile("images/background.jpg");
    t3.loadFromFile("images/explosions/type_C.png");
    t4.loadFromFile("images/rock.png");
    t5.loadFromFile("images/fire_blue.png");
    t6.loadFromFile("images/rock_small.png");
    t7.loadFromFile("images/explosions/type_B.png");
    t8.loadFromFile("images/typeChiken/egg.png");

    Texture tChicken1, tChicken2, tChicken3, tChicken4, tChicken5, tChicken6, boss, laze;
    tChicken1.loadFromFile("images/typeChiken/chicken1.png");
    tChicken2.loadFromFile("images/typeChiken/chicken2.png");
    tChicken3.loadFromFile("images/typeChiken/chicken3.png");
    tChicken4.loadFromFile("images/typeChiken/chicken4.png");
    tChicken5.loadFromFile("images/typeChiken/chicken5.png");
    tChicken6.loadFromFile("images/typeChiken/chicken6.png");
	boss.loadFromFile("images/typeChiken/boss.png");
	laze.loadFromFile("images/typeChiken/laze.png");
    t1.setSmooth(true); //Làm mịn hình ảnh khi phóng to/thu nhỏ → đẹp hơn.
    t2.setSmooth(true);

    Sprite background(t2);

    //Animation() {}
    //  t: texture chứa các frame
    // (x, y): toạ độ frame đầu tiên
    // (w, h): kích thước 1 frame
    // count: số frame
    // Speed: tốc độ chuyển frame
    Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.5);
    Animation sRock(t4, 0, 0, 64, 64, 16, 0.2); // thien thach
    Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
    Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);
    Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
    Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
    Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);
    Animation sEgg(t8, 0, 0,40,40, 1, 0);// Animation cho trứng
    Animation sChicken1(tChicken1, 0, 0, 72, 72, 1, 0); // Animation cho gà
    Animation sChicken2(tChicken2, 0, 0, 72, 72, 1, 0);
    Animation sChicken3(tChicken3, 0, 0, 72, 72, 1, 0);
    Animation sChicken4(tChicken4, 0, 0, 72, 72, 1, 0);
    Animation sChicken5(tChicken5, 0, 0, 72, 72, 1, 0);
	Animation sChicken6(tChicken6, 0, 0, 72, 72, 1, 0);
    Animation sBoss(boss, 0, 0, 128, 128, 1, 0);     
    Animation sBossLaser(laze, 0, 0, 32, 128, 1, 0);
    //e->settings(sEgg, x, y + 30, 90, 10); // Tạo trứng tại vị trí gà



    std::list<Entity*> entities;

    for (int i = 0; i < 15; i++) // for 15 asteroid 
    {
        asteroid* a = new asteroid();
        a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
        entities.push_back(a);
    }
    //for (int i = 0; i < 10; i++) {
    //    Chicken* c = new Chicken(&sEgg, &entities); // Truyền con trỏ tới animation trứng

    //    int r = rand() % 6;
    //    if (r == 0) c->settings(sChicken1, 100 + i * 80, 50, 0, 25);
    //    else if (r == 1) c->settings(sChicken2, 100 + i * 80, 50, 0, 25);
    //    else if (r == 2) c->settings(sChicken3, 100 + i * 80, 50, 0, 25);
    //    else if (r == 3) c->settings(sChicken4, 100 + i * 80, 50, 0, 25);
    //    else if (r == 4) c->settings(sChicken5, 100 + i * 80, 50, 0, 25);
    //    else c->settings(sChicken5, 100 + i * 80, 50, 0, 25);

    //    entities.push_back(c);
    //}

	
    player* p = new player();
    p->settings(sPlayer, 200, 200, 0, 20);
    entities.push_back(p);

    GameManager::spawnChickenWave(entities, sEgg, sChicken1, sChicken2, sChicken3, sChicken4, sChicken5, sChicken6);

    /////main loop/////
    //Vòng lặp game
    while (app.isOpen())
    {
        Event event;
        //pollEvent sẽ xử lý tất cả sự kiện chờ xử lý (nhấn phím, đóng cửa sổ...)
        while (app.pollEvent(event))
        {
            //Khi bạn nhấn một phím, SFML sẽ sinh ra
            // một Event::KeyPressed. Bạn kiểm tra phím nào được nhấn bằng event.key.code
            if (event.type == Event::Closed)
                app.close();

            if (event.type == Event::KeyPressed)
                if (event.key.code == Keyboard::Space)
                {
                    bullet* b = new bullet();
                    b->settings(sBullet, p->x, p->y, p->angle, 10);
                    entities.push_back(b);
                }
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) p->angle += 3;
        if (Keyboard::isKeyPressed(Keyboard::Left))  p->angle -= 3;
        if (Keyboard::isKeyPressed(Keyboard::Up)) p->thrust = true;
        else p->thrust = false;


        for (auto a : entities)
            for (auto b : entities)
            {
                //Cả hai bị xóa
                //Tạo hiệu ứng nổ
                //Nếu thiên thạch lớn, tạo 2 thiên thạch nhỏ
                if (a->name == "asteroid" && b->name == "bullet")
                    if (isCollide(a, b))
                    {
                        a->life = false;           // Đánh dấu thiên thạch đã bị phá hủy
                        b->life = false;           // Đánh dấu viên đạn đã va chạm và biến mất

                        Entity* e = new Entity();
                        e->settings(sExplosion, a->x, a->y); // Tạo hiệu ứng nổ tại vị trí thiên thạch
                        e->name = "explosion";
                        entities.push_back(e);   // Thêm hiệu ứng nổ vào danh sách đối tượng

                        // Nếu thiên thạch lớn, tạo ra 2 thiên thạch nhỏ tại vị trí nổ
                        for (int i = 0; i < 2; i++)
                        {
                            if (a->R == 15) continue; // Nếu đã là thiên thạch nhỏ thì bỏ qua
                            Entity* e = new asteroid();
                            e->settings(sRock_small, a->x, a->y, rand() % 360, 15); // Tạo thiên thạch nhỏ với hướng ngẫu nhiên
                            entities.push_back(e); // Thêm vào danh sách đối tượng
                        }

                    }
                // player is reseted to the middle 
                //create explosion effect
                if (a->name == "player" && b->name == "asteroid")
                    if (isCollide(a, b))
                    {
                        b->life = false; // Đánh dấu thiên thạch đã va chạm với player là đã chết

                        Entity* e = new Entity();
                        e->settings(sExplosion_ship, a->x, a->y); // Tạo hiệu ứng nổ tàu tại vị trí va chạm
                        e->name = "explosion";
                        entities.push_back(e); // Thêm hiệu ứng nổ vào danh sách đối tượng

                        p->settings(sPlayer, W / 2, H / 2, 0, 20); // Đưa player về giữa màn hình, reset trạng thái
                        p->dx = 0; p->dy = 0; // Đặt lại vận tốc về 0
                    }
                if (a->name == "egg" && b->name == "player")
                {
                    if (isCollide(a, b))
                    {
                        a->life = false; // Xóa trứng khi va chạm với người chơi
                        Entity* e = new Entity();
                        e->settings(sExplosion, a->x, a->y); // Tạo hiệu ứng nổ tại vị trí trứng
                        e->name = "explosion";
                        entities.push_back(e); // Thêm hiệu ứng nổ vào danh sách đối tượng

                        p->settings(sPlayer, W / 2, H / 2, 0, 20); // Đưa player về giữa màn hình, reset trạng thái
                        p->dx = 0; p->dy = 0; // Đặt lại vận tốc về 0
                    }
                }
                if (a->name == "chicken" && b->name == "bullet")
                {
                    if (isCollide(a, b))
                    {
                        a->life = false; // Đánh dấu gà đã bị bắn
                        b->life = false; // Đánh dấu viên đạn đã va chạm và biến mất
                        Entity* e = new Entity();
                        e->settings(sExplosion, a->x, a->y); // Tạo hiệu ứng nổ tại vị trí gà
                        e->name = "explosion";
                        entities.push_back(e);   // Thêm hiệu ứng nổ vào danh sách đối tượng
                    }
                }
                if (a->name == "boss" && b->name == "player")
                {
                    if (isCollide(a, b))
                    {
                        b->life = false; // Đánh dấu boss đã bị va chạm
                        Entity* e = new Entity();
                        e->settings(sExplosion_ship, b->x, b->y);
                        e->name = "explosion";
                        entities.push_back(e);   // Thêm hiệu ứng nổ vào danh sách đối tượng
                        
                        p->settings(sPlayer, W / 2, H / 2, 0, 20); // Đưa player về giữa màn hình, reset trạng thái
                        p->dx = 0; p->dy = 0;
                    }
                }
                if (a->name == "boss" && b->name == "bullet")
                {
                    if (isCollide(a, b))
                    {
                        b->life = false;
                        Boss* boss = dynamic_cast<Boss*>(a);
                        if (boss)
                        {
                            boss->hp -= 10;
                            if (boss->hp <= 0)
                            {
                                boss->life = false;
                                Entity* e = new Entity();
                                e->settings(sExplosion, boss->x, boss->y);
                                e->name = "explosion";
                                entities.push_back(e);
                                bossDefeated = true;
                            }
                        }
                    }
                }
            }


        if (p->thrust)  p->anim = sPlayer_go; // Nếu đang nhấn lên, đổi animation tàu bay
        else   p->anim = sPlayer; // Nếu không, animation tàu đứng yên


        if(!bossAppeared){
            bool chickenRemaining = false;
            for (auto e : entities) {
                if (e->name == "chicken") {
                    chickenRemaining = true;
					break; // Nếu còn gà thì không spawn boss
                }
               
            }
            if (!chickenRemaining) {
                if (chickenWave < maxWaves) {
                    
                    GameManager::spawnChickenWave(entities, sEgg, sChicken1, sChicken2, sChicken3, sChicken4, sChicken5, sChicken6);
                    chickenWave++;
                }
                else {
                    // Spawn boss sau khi hết gà
                    bossAppeared = true;
					Boss* bos = new Boss(&sEgg, &entities, &sBossLaser);
					bos->settings(sBoss, W / 2, 100, 0, 50); // Đặt boss
					entities.push_back(bos); // Thêm boss vào danh sách đối tượng
                }
            }
        }
        //process explosion effect
        for (auto e : entities)
            if (e->name == "explosion")
                if (e->anim.isEnd()) e->life = 0; // Nếu hiệu ứng nổ kết thúc thì xóa đối tượng

        //create asteriod random
        if (rand() % 150 == 0)
        {
            asteroid* a = new asteroid();
            a->settings(sRock, 0, rand() % H, rand() % 360, 25); // Tạo thiên thạch mới xuất hiện ở cạnh trái, vị trí ngẫu nhiên
            entities.push_back(a); // Thêm vào danh sách đối tượng
        }
        // process list object
        for (auto i = entities.begin(); i != entities.end();)
        {
            Entity* e = *i;

            e->update();         // Cập nhật vị trí, trạng thái của đối tượng (player, asteroid, bullet, explosion)
            e->anim.update();    // Cập nhật animation cho đối tượng

            if (e->life == false) { i = entities.erase(i); delete e; } // Nếu đối tượng đã chết thì xóa khỏi danh sách và giải phóng bộ nhớ
            else i++;           // Nếu còn sống thì chuyển sang đối tượng tiếp theo
        }

        //////draw//////
        //draw background
       //Draw all objects
       //app.display() to update the frame
       // it mean will draw for the game, all above is upload for vscode, and send request 
        app.draw(background); // ve nen
        for (auto i : entities) i->draw(app); //ve moi doi tuong
        app.display(); // cap nhat khung hinh, lenh duy nhat thuc su dua ra man hinh

        if (bossDefeated) app.close();
    }

    return 0;
}