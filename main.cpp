#include <rdg/dungeontypes.h>
#include <rdg/dungeongenerator.h>
#include <rdg/dungeontools.h>


int main(int argc, char** argv)
{
    int seed = argc > 1 ? atoi(argv[1]) : 0;

    rdg::Dungeon::Ptr dungeon = rdg::DungeonGenerator::Create(seed);

    rdg::DungeonTools::Print(*dungeon);

    return 0;
}
