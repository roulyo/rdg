#pragma once

#include <rdg/dungeontypes.h>

namespace rdg
{
    //----------------------------------------------------------------------------
    class DungeonGenerator
    {
    public:
        static Dungeon::Ptr Create(unsigned _seed = 42, const DungeonGenerationParam& _param = DungeonGenerationParam());

    private:
        static void GenerateRooms(const Dungeon::Ptr& _dungeon);
        static void LinkRooms(const Dungeon::Ptr& _dungeon);
        static void GenerateBubbles(const Dungeon::Ptr& _dungeon);

        static Room CreateRoom();
        static Door CreateDoor(const Room& _room);
        static Room CreateBubble();

    private:
        static DungeonGenerationParam ms_Param;

    };

}