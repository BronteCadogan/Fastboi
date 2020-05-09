#pragma once

#include "Fastboi.h"

using namespace Fastboi;
using namespace Fastboi::Components;

namespace Adventure {
    struct RoomChangeEvent;

    struct Player final {
        inline static const float speed = 760.f;

        private:
        Rigidbody* rigidbody = nullptr;

        bool isEaten = false;

        public:
        GORef gameobject;

        Player(GORef&& go);
        ~Player();

        void Start();
        void Update();

        void Eat();
        void RoomChanged(const RoomChangeEvent& e);

        static void Inst(Gameobject& go, const Position& pos);
    };

};