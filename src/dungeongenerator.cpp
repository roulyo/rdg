#include <rdg/dungeongenerator.h>

#include <cassert>

namespace rdg
{

    //----------------------------------------------------------------------------
    DungeonGenerationParam DungeonGenerator::ms_Param = DungeonGenerationParam();

    //----------------------------------------------------------------------------
    Dungeon::Ptr DungeonGenerator::Create(unsigned _seed /* = 42*/, const DungeonGenerationParam& _param /* = DungeonGenerationParam()*/)
    {
        srand(_seed);
        ms_Param = _param;

        Dungeon::Ptr dungeon;
        dungeon.reset(new Dungeon({ static_cast<int>(ms_Param.WorldSize), static_cast<int>(ms_Param.WorldSize) }));

        GenerateRooms(dungeon);
        LinkRooms(dungeon);
        GenerateBubbles(dungeon);

        return dungeon;
    }


    //----------------------------------------------------------------------------
    void DungeonGenerator::GenerateRooms(const Dungeon::Ptr& _dungeon)
    {
        assert(_dungeon != nullptr);

        int nbRoom = ms_Param.RoomCountMin + (rand() % (ms_Param.RoomCountMax - ms_Param.RoomCountMin));
        int i = 0;

        while (i < nbRoom)
        {
            Room room = CreateRoom();

            bool intersectsRoom = false;

            for (const Room& otherRoom : _dungeon->GetRooms())
            {
                if (room.Intersects(otherRoom))
                {
                    intersectsRoom = true;
                    break;
                }
            }

            if (!intersectsRoom)
            {
                _dungeon->AddRoom(room);
                ++i;
            }
        }
    }

    //----------------------------------------------------------------------------
    void DungeonGenerator::LinkRooms(const Dungeon::Ptr& _dungeon)
    {
        for (const Room& room : _dungeon->GetRooms())
        {
            for (const Door& door : room.GetDoors())
            {
                const Room* anotherRoomPtr = nullptr;
                do
                {
                    anotherRoomPtr = &(_dungeon->GetRooms()[rand() % _dungeon->GetRooms().size()]);

                } while (room.GetId() == anotherRoomPtr->GetId());

                const Door* anotherDoorPtr = nullptr;
                anotherDoorPtr = &(anotherRoomPtr->GetDoors()[rand() % anotherRoomPtr->GetDoors().size()]);

                Corridor corridor(door, *anotherDoorPtr);

                _dungeon->AddCorridor(corridor);
            }
        }
    }

    //----------------------------------------------------------------------------
    void DungeonGenerator::GenerateBubbles(const Dungeon::Ptr& _dungeon)
    {
        const unsigned aimedSurface = ms_Param.WorldSize * ms_Param.WorldSize * ms_Param.BubbleIntensity / 100;
        unsigned totalSurface = 0;

        std::vector<Room> tmpBubbles;

        while (totalSurface < aimedSurface)
        {
            Room bubble = CreateBubble();

            tmpBubbles.push_back(bubble);
            totalSurface += bubble.GetSize().h * bubble.GetSize().w;

        }

        for (const Room& bubble : tmpBubbles)
        {
            bool selected = false;

            for (const Room& room : _dungeon->GetRooms())
            {
                if (room.Intersects(bubble) && !room.Contains(bubble))
                {
                    selected = true;
                    _dungeon->AddBubble(bubble);
                }
            }

            if (!selected)
            {
                for (const Corridor& corridor : _dungeon->GetCorridors())
                {
                    Vector2d vectorAL = { corridor.GetEndpointA().GetCoord(), corridor.GetTurnCoord() };
                    Vector2d vectorLB = { corridor.GetTurnCoord(), corridor.GetEndpointB().GetCoord() };

                    if (Room(vectorAL).Intersects(bubble) || Room(vectorLB).Intersects(bubble))
                    {
                        selected = true;
                        _dungeon->AddBubble(bubble);
                    }
                }
            }
        }
    }

    //----------------------------------------------------------------------------
    Room DungeonGenerator::CreateRoom()
    {
        Coord2d size;
        Coord2d coord;

        size.w = ms_Param.RoomSizeMin + (rand() % (ms_Param.RoomSizeMax - ms_Param.RoomSizeMin));
        size.h = ms_Param.RoomSizeMin + (rand() % (ms_Param.RoomSizeMax - ms_Param.RoomSizeMin));

        coord.x = rand() % (ms_Param.WorldSize - size.w);
        coord.y = rand() % (ms_Param.WorldSize - size.h);

        Room room(coord, size);

        unsigned maxDoor = (rand() % ms_Param.RoomDoorsCount) + 1;

        do 
        {
            room.AddDoor(CreateDoor(room));

        } while (room.GetDoors().size() < maxDoor);
        
        return room;
    }

    //----------------------------------------------------------------------------
    Door DungeonGenerator::CreateDoor(const Room& _room)
    {
        Coord2d doorCoord;

        if (rand() % 2)
        {
            doorCoord.x = _room.GetCoord().x + (_room.GetSize().w * (rand() % 2));
            doorCoord.y = _room.GetCoord().y + (rand() % _room.GetSize().h);
        }
        else
        {
            doorCoord.x = _room.GetCoord().x + (rand() % _room.GetSize().w);
            doorCoord.y = _room.GetCoord().y + (_room.GetSize().h * (rand() % 2));
        }

        return Door(doorCoord);;
    }

    //----------------------------------------------------------------------------
    Room DungeonGenerator::CreateBubble()
    {
        Coord2d size;
        Coord2d coord;

        size.w = (rand() % 3) + 2;
        size.h = (rand() % 3) + 2;

        coord.x = rand() % (ms_Param.WorldSize - size.w);
        coord.y = rand() % (ms_Param.WorldSize - size.h);

        return Room(coord, size);
    }

}