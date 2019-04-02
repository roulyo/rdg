#include <rdg/dungeontypes.h>
#include <rdg/dungeongenerator.h>
#include <rdg/dungeontools.h>


int main(int argc, char** argv)
{
    unsigned seed = argc > 1 ? atoi(argv[1]) : 0;

    rdg::DungeonGenerationParam param;
    param.BubbleIntensity = argc > 2 ? atoi(argv[2]) : 100;
    param.RoomDoorsCount = argc > 3 ? atoi(argv[3]) : 3;

    rdg::Dungeon::Ptr dungeon = rdg::DungeonGenerator::Create(seed, param);

    rdg::DungeonTools::Print(*dungeon);

    return 0;
}
