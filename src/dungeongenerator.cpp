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

            for (const Room& otherRoom : _dungeon->m_Rooms)
            {
                if (room.Intersects(otherRoom))
                {
                    intersectsRoom = true;
                    break;
                }
            }

            if (!intersectsRoom)
            {
                _dungeon->m_Rooms.push_back(room);
                ++i;
            }
        }
    }

    //----------------------------------------------------------------------------
    void DungeonGenerator::LinkRooms(const Dungeon::Ptr& _dungeon)
    {
        for (Room& room : _dungeon->m_Rooms)
        {
            for (Door& door : room.m_Doors)
            {
                //if (door.m_Linked)
                //{
                //    continue;
                //}

                Room* anotherRoomPtr = nullptr;
                do
                {
                    anotherRoomPtr = &(_dungeon->m_Rooms[rand() % _dungeon->m_Rooms.size()]);

                } while (room.m_Id == anotherRoomPtr->m_Id);

                Door* anotherDoorPtr = nullptr;
                //do
                //{
                anotherDoorPtr = &(anotherRoomPtr->m_Doors[rand() % anotherRoomPtr->m_Doors.size()]);

                //} while (doorPtr->m_Linked);

                Corridor corridor(door, *anotherDoorPtr);
                //corridor.m_Orientation = (rand() % 2) ? Corridor::Orientation::NW : Corridor::Orientation::SE;

                //door.m_Linked = true;
                //doorPtr->m_Linked = true;

                _dungeon->m_Corridors.push_back(corridor);
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
            totalSurface += bubble.m_Size.h * bubble.m_Size.w;

        }

        for (const Room& bubble : tmpBubbles)
        {
            bool selected = false;

            for (const Room& room : _dungeon->m_Rooms)
            {
                if (room.Intersects(bubble) && !room.Contains(bubble))
                {
                    selected = true;
                    _dungeon->m_Bubbles.push_back(bubble);
                }
            }

            if (!selected)
            {
                for (const Corridor& corridor : _dungeon->m_Corridors)
                {
                    Room vectorAL({ corridor.m_EndpointA.m_Coord, corridor.m_LTurnCoord });
                    Room vectorLB({ corridor.m_LTurnCoord, corridor.m_EndpointB.m_Coord });

                    if (vectorAL.Intersects(bubble) || vectorLB.Intersects(bubble))
                    {
                        selected = true;
                        _dungeon->m_Bubbles.push_back(bubble);
                    }
                }
            }
        }
    }

    //----------------------------------------------------------------------------
    Room DungeonGenerator::CreateRoom()
    {
        static int roomId = 0;

        Room room;

        room.m_Size.w = ms_Param.RoomSizeMin + (rand() % (ms_Param.RoomSizeMax - ms_Param.RoomSizeMin));
        room.m_Size.h = ms_Param.RoomSizeMin + (rand() % (ms_Param.RoomSizeMax - ms_Param.RoomSizeMin));

        room.m_Coord.x = rand() % (ms_Param.WorldSize - room.m_Size.w);
        room.m_Coord.y = rand() % (ms_Param.WorldSize - room.m_Size.h);

        room.m_Doors.push_back(CreateDoor(room));

        int maxDoor = rand() % ms_Param.RoomDoorsCount;

        for (int i = 0; i < maxDoor; ++i)
        {
            room.m_Doors.push_back(CreateDoor(room));
        }

        room.m_Id = roomId++;

        return room;
    }

    //----------------------------------------------------------------------------
    Door DungeonGenerator::CreateDoor(const Room& _room)
    {
        Door door;
        Coord2d doorCoord;

        if (rand() % 2)
        {
            doorCoord.x = _room.m_Coord.x + (_room.m_Size.w * (rand() % 2));
            doorCoord.y = _room.m_Coord.y + (rand() % _room.m_Size.h);
        }
        else
        {
            doorCoord.x = _room.m_Coord.x + (rand() % _room.m_Size.w);
            doorCoord.y = _room.m_Coord.y + (_room.m_Size.h * (rand() % 2));
        }

        door.m_Coord = doorCoord;

        return door;
    }

    //----------------------------------------------------------------------------
    Room DungeonGenerator::CreateBubble()
    {
        static int bubbleId = 0;

        Room bubble;

        bubble.m_Size.w = (rand() % 3) + 2;
        bubble.m_Size.h = (rand() % 3) + 2;

        bubble.m_Coord.x = rand() % (ms_Param.WorldSize - bubble.m_Size.w);
        bubble.m_Coord.y = rand() % (ms_Param.WorldSize - bubble.m_Size.h);

        bubble.m_Id = bubbleId++;

        return bubble;
    }

}