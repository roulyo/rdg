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
                    _outMatrix[i][j] = DungeonMatrix::TyleType::Floor;
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

        //----------------------------------------------------------------------------
        static void DumpEntrance(const Coord2d& _entrance, DungeonMatrix& _outMatrix)
        {
            _outMatrix[_entrance.x][_entrance.y] = DungeonMatrix::TyleType::Entrance;
        }

        //----------------------------------------------------------------------------
        static void DumpExit(const Coord2d& _exit, DungeonMatrix& _outMatrix)
        {
            _outMatrix[_exit.x][_exit.y] = DungeonMatrix::TyleType::Exit;
        }

        //----------------------------------------------------------------------------
        static char Print(DungeonMatrix::TyleType _tyleType)
        {
            switch (_tyleType)
            {
            case DungeonMatrix::TyleType::Empty:
                return ' ';
            case DungeonMatrix::TyleType::Floor:
                return '.';
            case DungeonMatrix::TyleType::Entrance:
                return 'S';
            case DungeonMatrix::TyleType::Exit:
                return 'F';
            default:
                abort();
            }
        }
    }


    //----------------------------------------------------------------------------
    void DungeonTools::Print(const Dungeon& _dungeon)
    {
        DungeonMatrix* world = new DungeonMatrix(_dungeon.GetSize().w);
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
                std::cout << DungeonToolsHelper::Print(_matrix[i][j]);
            }

            std::cout << std::endl;
        }
    }

    //----------------------------------------------------------------------------
    void DungeonTools::GetDungeonMatrix(const Dungeon& _dungeon, DungeonMatrix& _outMatrix)
    {
        DungeonToolsHelper::DumpRooms(_dungeon.GetRooms(), _outMatrix);
        DungeonToolsHelper::DumpRooms(_dungeon.GetBubbles(), _outMatrix);
        DungeonToolsHelper::DumpCorridors(_dungeon.GetCorridors(), _outMatrix);
        DungeonToolsHelper::DumpEntrance(_dungeon.GetEntrance(), _outMatrix);
        DungeonToolsHelper::DumpExit(_dungeon.GetExit(), _outMatrix);
    }

}