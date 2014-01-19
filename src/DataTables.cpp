#include "DataTables.hpp"
#include "Aircraft.h"

std::vector<AircraftData> initializeAircraftData()
{
    std::vector<AircraftData> data(Aircraft::TypeCount);

    data[Aircraft::Umag].hitpoints = 100;
    data[Aircraft::Umag].speed = 600.f;
    data[Aircraft::Umag].texture = Textures::Umag;

    data[Aircraft::Disk].hitpoints = 20;
    data[Aircraft::Disk].speed = 500.f;
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
