#pragma once

namespace rdg
{

    //----------------------------------------------------------------------------
    class DungeonMatrix;
    class Dungeon;

    //----------------------------------------------------------------------------
    class DungeonTools
    {
    public:
        static void Print(const Dungeon& _dungeon);
        static void Print(const DungeonMatrix& _matrix);

        static void GetDungeonMatrix(const Dungeon& _dungeon, DungeonMatrix& _outMatrix);

    };

}