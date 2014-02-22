#include "DataTables.hpp"
#include "Aircraft.h"
#include "Pickup.h"

std::vector<AircraftData> initializeAircraftData()
{
    std::vector<AircraftData> data(Aircraft::TypeCount);

    data[Aircraft::Umag].hitpoints = 10000;
    data[Aircraft::Umag].speed = 600.f;
    data[Aircraft::Umag].texture = Textures::Umag;

    data[Aircraft::Disk].hitpoints = 20;
    data[Aircraft::Disk].speed = 300.f;
    data[Aircraft::Disk].texture = Textures::Disk;

    data[Aircraft::Disk].directions.push_back(Direction(60, 200));
    data[Aircraft::Disk].directions.push_back(Direction(0, 10));
    data[Aircraft::Disk].directions.push_back(Direction(-60, 400));
    data[Aircraft::Disk].directions.push_back(Direction(0, 10));

    return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
    std::vector<ProjectileData> data(Projectile::TypeCount);

    data[Projectile::AlliedBullet].damage = 10;
    data[Projectile::AlliedBullet].speed = 1000.f;
    data[Projectile::AlliedBullet].texture = Textures::Bullet;

    data[Projectile::EnemyBullet].damage = 10;
    data[Projectile::EnemyBullet].speed = 1000.f;
    data[Projectile::EnemyBullet].texture = Textures::Bullet;

    data[Projectile::Missile].damage = 10;
    data[Projectile::Missile].speed = 1000.f;
    data[Projectile::Missile].texture = Textures::Missile;

    return data;
}

std::vector<PickupData> initializePickupData()
{
    std::vector<PickupData> data(Pickup::TypeCount);

    data[Pickup::HealthRefill].texture = Textures::HealthRefill;
    data[Pickup::HealthRefill].action = [] (Aircraft& player) {player.repair(25);};

    data[Pickup::MissileRefill].texture = Textures::MissileRefill;
    data[Pickup::MissileRefill].action = [] (Aircraft& player) {player.collectMissiles(3);};

    data[Pickup::FireSpread].texture = Textures::FireSpread;
    data[Pickup::FireSpread].action = [] (Aircraft& player) {player.increaseFireSpread();};


    return data;
}
