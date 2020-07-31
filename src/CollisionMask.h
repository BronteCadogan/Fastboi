#pragma once

#include <concepts>
#include <cstdint>
#include <type_traits>
#include "Application.h"

namespace Fastboi 
{
    //Things that can be collided with
    enum class CollisionLayer 
    {
        //why not just 1-8?
        ALL = 1,
        WALLS = 2,
        PLAYER = 4,
        UNITS = 8,
        ITEMS = 16,
        NONE = 32,
        TRIGGERS = 64,
        PARTICLES = 128
    };

    struct CollisionMask {
       // private:
        //error is here
        //inclusions is equal to the layer ALL
         //CollisionLayer inclusions = CollisionLayer::ALL;

        public:
        CollisionLayer inclusions = CollisionLayer::ALL;
        CollisionLayer layer;
        
        constexpr CollisionMask(CollisionLayer layer) : layer(layer) 
        {
            //Wait why wouldn't it be all AND none because you can't be both at the same time
            //or are we talking logicial equivalences here?

             if (layer == inclusions && layer == CollisionLayer::NONE)
                Application::ThrowRuntimeException("CollisionMask layer can't be ALL and NONE.", Application::INVALID_COL_LAYER);
            
            //if (layer == CollisionLayer::ALL || layer == CollisionLayer::NONE)
                //Application::ThrowRuntimeException("CollisionMask layer can't be ALL or NONE.", Application::INVALID_COL_LAYER);
        };

        template<typename... EnumClass>
        requires std::same_as<std::common_type_t<EnumClass...>, CollisionLayer>
        CollisionMask& Include(EnumClass... layers) //layers is a passed in variable 
         {
            using CT = std::common_type_t<EnumClass...>;
            using UT = std::underlying_type_t<CT>;

            if (((layers == CollisionLayer::ALL) || ...))
                inclusions = CollisionLayer::ALL;
            else if (((layers == CollisionLayer::NONE) || ...))
                inclusions = CollisionLayer::NONE;
            else {
                UT merged = (static_cast<UT>(layers) | ...);
                
                if (inclusions == CollisionLayer::ALL)
                    inclusions = static_cast<CT>(merged);
                else
                    inclusions = static_cast<CT>(static_cast<UT>(inclusions) | merged);
            };

            return *this;
        };

        template<typename... EnumClass>
        requires std::same_as<std::common_type_t<EnumClass...>, CollisionLayer>
        CollisionMask& Exclude(EnumClass... layers) 
        { // *! I swear to god Dylan the fact that you don''t put this on a newline infuriates me
            if (inclusions == CollisionLayer::NONE) return *this;

            using CT = std::common_type_t<EnumClass...>;
            using UT = std::underlying_type_t<CT>;

            if (((layers == CollisionLayer::ALL) || ...))
                inclusions = CollisionLayer::NONE;
            else if (((layers == CollisionLayer::NONE) || ...))
                inclusions = CollisionLayer::ALL;
            else {
                UT exclusions = (static_cast<UT>(layers) | ...);
                UT result;

                exclusions = ~exclusions; // Flip all excluded bits to 0

                if (inclusions == CollisionLayer::ALL)
                    result = ~static_cast<UT>(0x0); // Set all bits to 1 if the inclusions is ALL
                else
                    result = static_cast<UT>(inclusions);

                inclusions = static_cast<CT>(result & exclusions);
            }

            return *this;
        }

        CollisionMask& Clear() 
        { //set all the layers to be nothing nada not anymore
            inclusions = CollisionLayer::NONE;

            return *this;
        }

// will a and b collide?
        static bool CanCollide(const CollisionMask& a, const CollisionMask& b);

        private:
        bool CanCollide(CollisionLayer layer) const;
    };
}

/*

           o
          o    |
           \   |
            \  |
             \.|-.
             (\|  )
    .==================.
    | .--------------. |
    | |::.::.::.::.::| |
    | |'::'::'::'::':| |
    | |::.::.::.::.::| |
    | |:'::'::'::'::'| |
    | |::.::.::.::.::| |
    | '--------------'o|
    | LI LI """""""   o|
    |==================|
jgs |  .------------.  |
    | /              \ |
    |/                \|
    "                  "


    you're welcome via the ascii art archive



*/