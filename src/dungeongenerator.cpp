#include <rdg/dungeongenerator.h>

#include <cassert>
#include <iostream>

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
        GenerateAccesses(dungeon);
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
        static const unsigned aimedSurface(ms_Param.WorldSize * ms_Param.WorldSize * ms_Param.BubbleIntensity / 100);
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
            if (bubble.Contains(_dungeon->GetEntrance()) || bubble.Contains(_dungeon->GetExit()))
            {
                continue;
            }

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
                        _dungeon->AddBubble(bubble);
                    }
                }
            }
        }
    }

    //----------------------------------------------------------------------------
    static bool CouldBlockCorridor(const Coord2d& _point, const Dungeon::Ptr& _dungeon)
    {
        for (const Room& room : _dungeon->GetRooms())
        {
            for (const Door& door : room.GetDoors())
            {
                if (_point.x == door.GetCoord().x || _point.y == door.GetCoord().y)
                {
                    return true;
                }
            }
        }

        return false;
    }


    //----------------------------------------------------------------------------
    void DungeonGenerator::GenerateAccesses(const Dungeon::Ptr& _dungeon)
    {
        const Room& entranceRoom = _dungeon->GetRooms()[rand() % _dungeon->GetRooms().size()];
        Coord2d entrance;

        do
        {
            entrance = CreateDoor(entranceRoom).GetCoord();

        } while (CouldBlockCorridor(entrance, _dungeon));

        const Room& exitRoom = _dungeon->GetRooms()[rand() % _dungeon->GetRooms().size()];
        Coord2d exit;

        do
        {
            exit = CreateDoor(exitRoom).GetCoord();

        } while (CouldBlockCorridor(exit, _dungeon));

        _dungeon->SetAccesses(entrance, exit);
    }

    //----------------------------------------------------------------------------
    Room DungeonGenerator::CreateRoom()
    {
        Coord2d size;
        Coord2d coord;

        static unsigned minMaxDiff = ms_Param.RoomSizeMax - ms_Param.RoomSizeMin + 1;

        size.w = ms_Param.RoomSizeMin + (rand() % minMaxDiff);
        size.h = ms_Param.RoomSizeMin + (rand() % minMaxDiff);

        coord.x = rand() % (ms_Param.WorldSize - size.w);
        coord.y = rand() % (ms_Param.WorldSize - size.h);

        Room room(coord, size);

        unsigned maxDoor = (rand() % ms_Param.RoomDoorsCount);

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
            doorCoord.x = _room.GetCoord().x + ((_room.GetSize().w - 1) * (rand() % 2));
            doorCoord.y = _room.GetCoord().y + (rand() % _room.GetSize().h);
        }
        else
        {
            doorCoord.x = _room.GetCoord().x + (rand() % _room.GetSize().w);
            doorCoord.y = _room.GetCoord().y + ((_room.GetSize().h - 1) * (rand() % 2));
        }

        return Door(doorCoord);;
    }

    //----------------------------------------------------------------------------
    Room DungeonGenerator::CreateBubble()
    {   
        static const float fBubbleSizeMin((ms_Param.RoomSizeMin * ms_Param.BubbleRoomRatio) / 100.f);
        static const float fBubbleSizeMax((ms_Param.RoomSizeMax * ms_Param.BubbleRoomRatio) / 100.f);

        static const unsigned bubbleSizeMin(std::lround(fBubbleSizeMin));
        static const unsigned bubbleSizeMax(std::lround(fBubbleSizeMax));

        static const unsigned minMaxDiff(bubbleSizeMax - bubbleSizeMin + 1);

        Coord2d size;
        size.w = bubbleSizeMin + (rand() % minMaxDiff);
        size.h = bubbleSizeMin + (rand() % minMaxDiff);

        Coord2d coord;
        coord.x = rand() % (ms_Param.WorldSize - size.w);
        coord.y = rand() % (ms_Param.WorldSize - size.h);

        return Room(coord, size);
    }

}