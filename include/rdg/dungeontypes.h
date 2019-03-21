#pragma once

#include <vector>
#include <memory>

namespace rdg
{
    //----------------------------------------------------------------------------
    struct DungeonGenerationParam
    {
        unsigned WorldSize = 80;

        unsigned RoomCountMin = 4;
        unsigned RoomCountMax = 6;

        unsigned RoomSizeMin = 8;
        unsigned RoomSizeMax = 12;

        unsigned RoomDoorsCount = 2;

        unsigned BubbleIntensity = 80;
    };

    //----------------------------------------------------------------------------
    struct Coord2d
    {
    public:
        union
        {
            int x;
            int w;
        };

        union
        {
            int y;
            int h;
        };

    };

    //----------------------------------------------------------------------------
    struct Vector2d
    {
        Coord2d Origin;
        Coord2d End;
    };

    //----------------------------------------------------------------------------
    class DungeonMatrix
    {
    private:
        typedef std::vector<std::vector<bool> >  MatrixType;

    public:
        DungeonMatrix(unsigned _size);

        const std::vector<std::vector<bool> >& operator*() const;
        std::vector<std::vector<bool> >& operator*();

        const std::vector<bool>& operator[](unsigned _index) const;
        std::vector<bool>& operator[](unsigned _index);

        unsigned GetSize() const;

    private:
        MatrixType  m_Matrix;

    };

    //----------------------------------------------------------------------------
    class Door
    {
    public:
        Door(const Coord2d& _coord);

        const Coord2d& GetCoord() const;

    private:
        Coord2d   m_Coord;

    };

    //----------------------------------------------------------------------------
    class Room
    {
    public:
        Room(const Coord2d& _coord, const Coord2d& _size);
        Room(const Vector2d& _diagonal);

        bool Intersects(const Room& _other) const;
        bool Contains(const Coord2d& _point) const;
        bool Contains(const Room& _other) const;

        void AddDoor(const Door& _door);

        int GetId() const;
        const Coord2d& GetCoord() const;
        const Coord2d& GetSize() const;
        const std::vector<Door>& GetDoors() const;

    private:
        static int  ms_IdCount;
        int         m_Id;

        Coord2d     m_Coord;
        Coord2d     m_Size;

        std::vector<Door>  m_Doors;

    };

    //----------------------------------------------------------------------------
    class Corridor
    {
    public:
        Corridor(const Door& _doorA, const Door& _doorB);

        const Door& GetEndpointA() const;
        const Door& GetEndpointB() const;
        const Coord2d& GetTurnCoord() const;

    private:
        Door    m_EndpointA;
        Door    m_EndpointB;

        Coord2d m_TurnCoord;

    };

    //----------------------------------------------------------------------------
    class Dungeon
    {
    public:
        typedef std::unique_ptr<Dungeon>    Ptr;

        Dungeon(const Coord2d& _size);

    public:
        Coord2d                 m_Size;

        std::vector<Room>       m_Rooms;
        std::vector<Room>       m_Bubbles;
        std::vector<Corridor>   m_Corridors;

    };


}