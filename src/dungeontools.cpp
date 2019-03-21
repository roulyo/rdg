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
            for (int i = _room.m_Coord.x; i < _room.m_Coord.x + _room.m_Size.w; ++i)
            {
                for (int j = _room.m_Coord.y; j < _room.m_Coord.y + _room.m_Size.h; ++j)
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
                Room vectorAL({ corridor.m_EndpointA.m_Coord, corridor.m_LTurnCoord });
                Room vectorLB({ corridor.m_LTurnCoord, corridor.m_EndpointB.m_Coord });

                DumpRoom(vectorAL, _outMatrix);
                DumpRoom(vectorLB, _outMatrix);
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