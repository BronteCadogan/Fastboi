#pragma once

#include <concepts>
#include <cstdint>
#include <type_traits>
#include "Application.h"

<<<<<<< HEAD
namespace Fastboi {

    enum class CollisionLayer {
=======
namespace Fastboi 
{

    enum class CollisionLayer 
    {
>>>>>>> 445fa0f100c23a73d20d21451bee3945ff46f5ce
    
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
        private:
<<<<<<< HEAD

         std::underlying_type_t<CollisionLayer> inclusions = CollisionLayer::ALL;
=======
    
         std::underlying_type_t inclusions = CollisionLayer;
>>>>>>> 445fa0f100c23a73d20d21451bee3945ff46f5ce

        public:
      
        CollisionLayer layer;
        
<<<<<<< HEAD
        constexpr CollisionMask(CollisionLayer layer) : layer(layer) {
=======
        constexpr CollisionMask(CollisionLayer layer) : layer(layer) 
        {
>>>>>>> 445fa0f100c23a73d20d21451bee3945ff46f5ce
            
            if (layer == CollisionLayer::ALL || layer == CollisionLayer::NONE)
                Application::ThrowRuntimeException("CollisionMask layer can't be ALL or NONE.", Application::INVALID_COL_LAYER);
        };

        template<typename... EnumClass>
        requires std::same_as<std::common_type_t<EnumClass...>, CollisionLayer>
        CollisionMask& Include(EnumClass... layers) 
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
<<<<<<< HEAD
        CollisionMask& Exclude(EnumClass... layers) { 
=======
        CollisionMask& Exclude(EnumClass... layers) 
        { 
>>>>>>> 445fa0f100c23a73d20d21451bee3945ff46f5ce
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

                exclusions = ~exclusions;

                if (inclusions == CollisionLayer::ALL)
                    result = ~static_cast<UT>(0x0);
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