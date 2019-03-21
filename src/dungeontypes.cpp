#include <rdg/dungeontypes.h>

#include <algorithm>

namespace rdg
{

    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    DungeonMatrix::DungeonMatrix(unsigned _size)
    {
        m_Matrix.resize(_size);

        for (std::vector<bool>& row : m_Matrix)
        {
            row.resize(_size, false);
        }
    }

    //----------------------------------------------------------------------------
    const std::vector<std::vector<bool>>& DungeonMatrix::operator*() const
    {
        return m_Matrix;
    }

    //----------------------------------------------------------------------------
    std::vector<std::vector<bool>>& DungeonMatrix::operator*()
    {
        return m_Matrix;
    }

    //----------------------------------------------------------------------------
    const std::vector<bool>& DungeonMatrix::operator[](unsigned _index) const
    {
        return m_Matrix[_index];
    }

    //----------------------------------------------------------------------------
    std::vector<bool>& DungeonMatrix::operator[](unsigned _index)
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
    Room::Room()
    {
    }

    //----------------------------------------------------------------------------
    Room::Room(const Vector2d& _diagonal)
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
        int interRight = std::min(_other.m_Coord.x + _other.m_Size.w, m_Coord.x + m_Size.w);
        int interBottom = std::min(_other.m_Coord.y + _other.m_Size.h, m_Coord.y + m_Size.h);

        return ((interLeft <= interRight) && (interTop <= interBottom));
    }

    //----------------------------------------------------------------------------
    bool Room::Contains(const Coord2d& _point) const
    {
        return (_point.x >= m_Coord.x)
            && (_point.x <= (m_Coord.x + m_Size.w))
            && (_point.y >= m_Coord.y)
            && (_point.y <= (m_Coord.y + m_Size.h));
    }

    //----------------------------------------------------------------------------
    bool Room::Contains(const Room& _other) const
    {
        Coord2d topLeft = { _other.m_Coord.x, _other.m_Coord.y };
        Coord2d topRight = { _other.m_Coord.x + _other.m_Size.w, _other.m_Coord.y };
        Coord2d bottomLeft = { _other.m_Coord.x, _other.m_Coord.y + _other.m_Size.h };
        Coord2d bottomRight = { _other.m_Coord.x + _other.m_Size.w, _other.m_Coord.y + _other.m_Size.h };

        return Contains(topLeft) && Contains(topRight) && Contains(bottomLeft) && Contains(bottomRight);
    }




    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    Corridor::Corridor(const Door& _doorA, const Door& _doorB)
        : m_EndpointA(_doorA)
        , m_EndpointB(_doorB)
    {
        if (rand() % 2)
        {
            m_LTurnCoord = { m_EndpointA.m_Coord.x, m_EndpointB.m_Coord.y };
        }
        else
        {
            m_LTurnCoord = { m_EndpointB.m_Coord.x, m_EndpointA.m_Coord.y };
        }
    }




    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    Dungeon::Dungeon(const Coord2d& _size)
        : m_Size(_size)
    {
    }

}