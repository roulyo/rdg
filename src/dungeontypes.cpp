#include <rdg/dungeontypes.h>

#include <algorithm>

namespace rdg
{

    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    DungeonMatrix::DungeonMatrix(unsigned _size)
    {
        m_Matrix.resize(_size);

        for (std::vector<TyleType>& row : m_Matrix)
        {
            row.resize(_size, TyleType::Empty);
        }
    }

    //----------------------------------------------------------------------------
    const std::vector<std::vector<DungeonMatrix::TyleType>>& DungeonMatrix::operator*() const
    {
        return m_Matrix;
    }

    //----------------------------------------------------------------------------
    std::vector<std::vector<DungeonMatrix::TyleType>>& DungeonMatrix::operator*()
    {
        return m_Matrix;
    }

    //----------------------------------------------------------------------------
    const std::vector<DungeonMatrix::TyleType>& DungeonMatrix::operator[](unsigned _index) const
    {
        return m_Matrix[_index];
    }

    //----------------------------------------------------------------------------
    std::vector<DungeonMatrix::TyleType>& DungeonMatrix::operator[](unsigned _index)
    {
        return m_Matrix[_index];
    }

    //----------------------------------------------------------------------------
    unsigned DungeonMatrix::GetSize() const
    {
        return m_Matrix.size();
    }




    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    Door::Door(const Coord2d& _coord)
        : m_Coord(_coord)
    {
    }

    //----------------------------------------------------------------------------
    const Coord2d& Door::GetCoord() const
    {
        return m_Coord;
    }




    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    int Room::ms_IdCount = 0;

    //----------------------------------------------------------------------------
    Room::Room(const Coord2d& _coord, const Coord2d& _size)
        : m_Id(ms_IdCount++)
        , m_Coord(_coord)
        , m_Size(_size)
    {
    }

    //----------------------------------------------------------------------------
    Room::Room(const Vector2d& _diagonal)
        : m_Id(ms_IdCount++)
    {
        int minX = std::min(_diagonal.Origin.x, _diagonal.End.x);
        int minY = std::min(_diagonal.Origin.y, _diagonal.End.y);
        int maxX = std::max(_diagonal.Origin.x, _diagonal.End.x);
        int maxY = std::max(_diagonal.Origin.y, _diagonal.End.y);

        m_Coord.x = minX;
        m_Coord.y = minY;

        m_Size.w = maxX - minX + 1;
        m_Size.h = maxY - minY + 1;
    }

    //----------------------------------------------------------------------------
    bool Room::Intersects(const Room& _other) const
    {
        int interLeft = std::max(_other.m_Coord.x, m_Coord.x);
        int interTop = std::max(_other.m_Coord.y, m_Coord.y);
        int interRight = std::min(_other.m_Coord.x + _other.m_Size.w - 1, m_Coord.x + m_Size.w - 1);
        int interBottom = std::min(_other.m_Coord.y + _other.m_Size.h - 1, m_Coord.y + m_Size.h - 1);

        return ((interLeft <= interRight) && (interTop <= interBottom));
    }

    //----------------------------------------------------------------------------
    bool Room::Contains(const Coord2d& _point) const
    {
        return (_point.x >= m_Coord.x)
            && (_point.x < (m_Coord.x + m_Size.w))
            && (_point.y >= m_Coord.y)
            && (_point.y < (m_Coord.y + m_Size.h));
    }

    //----------------------------------------------------------------------------
    bool Room::Contains(const Room& _other) const
    {
        Coord2d topLeft = { _other.m_Coord.x, _other.m_Coord.y };
        Coord2d topRight = { _other.m_Coord.x + _other.m_Size.w - 1, _other.m_Coord.y };
        Coord2d bottomLeft = { _other.m_Coord.x, _other.m_Coord.y + _other.m_Size.h - 1 };
        Coord2d bottomRight = { _other.m_Coord.x + _other.m_Size.w - 1, _other.m_Coord.y + _other.m_Size.h - 1 };

        return Contains(topLeft) && Contains(topRight) && Contains(bottomLeft) && Contains(bottomRight);
    }

    //----------------------------------------------------------------------------
    int Room::GetId() const
    {
        return m_Id;
    }

    //----------------------------------------------------------------------------
    void Room::AddDoor(const Door& _door)
    {
        m_Doors.push_back(_door);
    }

    //----------------------------------------------------------------------------
    const Coord2d& Room::GetCoord() const
    {
        return m_Coord;
    }

    //----------------------------------------------------------------------------
    const Coord2d& Room::GetSize() const
    {
        return m_Size;
    }

    const std::vector<Door>& Room::GetDoors() const
    {
        return m_Doors;
    }



    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    Corridor::Corridor(const Door& _doorA, const Door& _doorB)
        : m_EndpointA(_doorA)
        , m_EndpointB(_doorB)
    {
        if (rand() % 2)
        {
            m_TurnCoord = { m_EndpointA.GetCoord().x, m_EndpointB.GetCoord().y };
        }
        else
        {
            m_TurnCoord = { m_EndpointB.GetCoord().x, m_EndpointA.GetCoord().y };
        }
    }

    //----------------------------------------------------------------------------
    const Door& Corridor::GetEndpointA() const
    {
        return m_EndpointA;
    }

    //----------------------------------------------------------------------------
    const Door& Corridor::GetEndpointB() const
    {
        return m_EndpointB;
    }

    //----------------------------------------------------------------------------
    const Coord2d& Corridor::GetTurnCoord() const
    {
        return m_TurnCoord;
    }



    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    Dungeon::Dungeon(const Coord2d& _size)
        : m_Size(_size)
    {
    }

    //----------------------------------------------------------------------------
    void Dungeon::AddRoom(const Room& _room)
    {
        m_Rooms.push_back(_room);
    }

    //----------------------------------------------------------------------------
    void Dungeon::AddBubble(const Room& _bubble)
    {
        m_Bubbles.push_back(_bubble);
    }

    //----------------------------------------------------------------------------
    void Dungeon::AddCorridor(const Corridor& _corridor)
    {
        m_Corridors.push_back(_corridor);
    }

    //----------------------------------------------------------------------------
    void Dungeon::SetAccesses(const Coord2d& _entrance, const Coord2d& _exit)
    {
        m_Entrance = _entrance;
        m_Exit = _exit;
    }

    //----------------------------------------------------------------------------
    const Coord2d& Dungeon::GetSize() const
    {
        return m_Size;
    }

    //----------------------------------------------------------------------------
    const std::vector<Room>& Dungeon::GetRooms() const
    {
        return m_Rooms;
    }

    //----------------------------------------------------------------------------
    const std::vector<Room>& Dungeon::GetBubbles() const
    {
        return m_Bubbles;
    }

    //----------------------------------------------------------------------------
    const std::vector<Corridor>& Dungeon::GetCorridors() const
    {
        return m_Corridors;
    }

    //----------------------------------------------------------------------------
    const Coord2d& Dungeon::GetEntrance() const
    {
        return m_Entrance;
    }

    //----------------------------------------------------------------------------
    const Coord2d& Dungeon::GetExit() const
    {
        return m_Exit;
    }

}