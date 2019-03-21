#include <rdg/dungeontools.h>
#include <rdg/dungeontypes.h>

#include <iostream>

namespace rdg
{

    namespace DungeonToolsHelper
    {

        //----------------------------------------------------------------------------
        static void DumpRoom(const Room& _room, DungeonMatrix& _outMatrix)
        {
            for (int i = _room.GetCoord().x; i < _room.GetCoord().x + _room.GetSize().w; ++i)
            {
                for (int j = _room.GetCoord().y; j < _room.GetCoord().y + _room.GetSize().h; ++j)
                {
                    _outMatrix[i][j] = true;
                }
            }
        }

        //----------------------------------------------------------------------------
        static void DumpRooms(const std::vector<Room>& _rooms, DungeonMatrix& _outMatrix)
        {
            for (const Room& room : _rooms)
            {
                DumpRoom(room, _outMatrix);
            }
        }

        //----------------------------------------------------------------------------
        static void DumpCorridors(const std::vector<Corridor>& _corridors, DungeonMatrix& _outMatrix)
        {
            for (const Corridor& corridor : _corridors)
            {
                Vector2d vecAL = { corridor.GetEndpointA().GetCoord(), corridor.GetTurnCoord() };
                Vector2d vecLB = { corridor.GetTurnCoord(), corridor.GetEndpointB().GetCoord() };

                DumpRoom(Room(vecAL), _outMatrix);
                DumpRoom(Room(vecLB), _outMatrix);
            }
        }

    }


    //----------------------------------------------------------------------------
    void DungeonTools::Print(const Dungeon& _dungeon)
    {
        DungeonMatrix* world = new DungeonMatrix(_dungeon.m_Size.w);
        GetDungeonMatrix(_dungeon, *world);

        Print(*world);

        delete world;
    }

    //----------------------------------------------------------------------------
    void DungeonTools::Print(const DungeonMatrix& _matrix)
    {
        for (unsigned j = 0; j < _matrix.GetSize(); ++j)
        {
            for (unsigned i = 0; i < _matrix.GetSize(); ++i)
            {
                std::cout << (_matrix[i][j] ? "X" : " ");
            }

            std::cout << std::endl;
        }
    }

    //----------------------------------------------------------------------------
    void DungeonTools::GetDungeonMatrix(const Dungeon& _dungeon, DungeonMatrix& _outMatrix)
    {
        DungeonToolsHelper::DumpRooms(_dungeon.m_Rooms, _outMatrix);
        DungeonToolsHelper::DumpRooms(_dungeon.m_Bubbles, _outMatrix);
        DungeonToolsHelper::DumpCorridors(_dungeon.m_Corridors, _outMatrix);
    }

}