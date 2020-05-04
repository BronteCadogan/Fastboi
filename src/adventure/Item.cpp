#include "Item.h"
#include "FastboiComps.h"
#include "Gate.h"
#include "Player.h"
#include "Room.h"

using namespace Adventure;
using namespace Fastboi;
using namespace Fastboi::Components;

Item::Item(GORef&& go) : go(std::move(go)) { };

void Item::Start() {
    go().GetComponent<Collider>().collisionSignal.connect<&Item::Collision>(this);

    releaseLsn.signal.connect<&Item::Release>(this);
}

void Item::Collision(const CollisionEvent& e) {
    Gameobject& colgo = e.collider.gameobject();

    if (e.type == CollisionEvent::BEGIN && colgo.name == "Player") {
        printf("Collided!\n");
        go().transform->Parent(colgo.transform);
        go().transform->position += e.penetration.normalized() * 50.f;

        isHeld = true;
    }
}

void Item::Release(const KeyEvent& e) { 
    if (e.type == KeyEvent::DOWN && isHeld) {
        go().transform->Parent(nullptr);
    }
}

constexpr Vec<int> keySpriteSize(16, 6);

Key::Key(Fastboi::GORef&& go, KeyColor color) : go(std::move(go)), color(color) { }

void Key::Start() {
    if (!Reqs::HasRequiredComponents(go())) {
        Application::ThrowRuntimeException("Key missing reqs", Application::REQUIREMENTS_NOT_FULFILLED, Reqs::GetMissingNamesString(go()).c_str());
    }

    go().AddComponent<Item>();

    SpriteRenderer& sr = go().GetComponent<SpriteRenderer>();
    sr.cutout = Rect(0, (int) color * keySpriteSize.y, keySpriteSize.x, keySpriteSize.y);
}

void Adventure::KeyGO(Gameobject& go, const Position& pos, KeyColor color) {
    switch (color) {
        case KeyColor::GOLD:
            go.name = "Gold key";
            break;
        case KeyColor::BLACK:
            go.name = "Black key";
            break;
        case KeyColor::WHITE:
            go.name = "White key";
            break;
    }

    constexpr Size keySize(Room::GetTileSize().x, Room::GetTileSize().y * (float) keySpriteSize.y / (float) keySpriteSize.x);

    go.AddComponent<Transform>(pos, keySize, 0);
    go.AddComponent<Collider>(Collider::TRIGGER);
    go.AddComponent<SpriteRenderer>(RenderData(RenderOrder::OBJECTS_OVER, 0), "Keys", Rect(0, 0, keySpriteSize.x, keySpriteSize.y));
    go.AddComponent<Key>(color);
}