#include "roommodel.h"
#include <QMap>
#include <QVariant>
#include <QDebug>
#include <algorithm>

RoomModel::RoomModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int RoomModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_rooms.size();
}

int RoomModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 5;
}

QVariant RoomModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        const Room &room = m_rooms[index.row()];
        switch (index.column()) {
        case 0: return room.getRoomNumber();
        case 1: return room.getRoomType();
        case 2: return room.getGuestName();
        case 3: return room.getPrice();
        case 4: return room.isAvailable() ? "Yes" : "No";
        default: return QVariant();
        }
    }

    return QVariant();
}

QVariant RoomModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "Room Number";
        case 1: return "Room Type";
        case 2: return "Guest Name";
        case 3: return "Price";
        case 4: return "Available";
        default: return QVariant();
        }
    }

    return QVariant();
}

void RoomModel::setRooms(const QList<Room> &rooms)
{
    beginResetModel();
    m_rooms = rooms;
    endResetModel();
}

void RoomModel::removeRoom(int index)
{
    if (index < 0 || index >= m_rooms.size())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_rooms.removeAt(index);
    endRemoveRows();
}

QList<Room> RoomModel::getRooms() const
{
    return m_rooms;
}

void RoomModel::sort(int column, Qt::SortOrder order)
{
    if (lastSortedColumn == column) {
        order = (lastSortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
    }

    sortOrders[column] = order;
    lastSortedColumn = column;
    lastSortOrder = order;

    std::sort(m_rooms.begin(), m_rooms.end(), [column, order](const Room &r1, const Room &r2) {
        QVariant v1, v2;
        switch (column) {
        case 0:
            v1 = QVariant(r1.getRoomNumber().toInt());
            v2 = QVariant(r2.getRoomNumber().toInt());
            break;
        case 1:
            v1 = r1.getRoomType();
            v2 = r2.getRoomType();
            break;
        case 2:
            v1 = r1.getGuestName();
            v2 = r2.getGuestName();
            break;
        case 3:
            v1 = QVariant(r1.getPrice());
            v2 = QVariant(r2.getPrice());
            break;
        case 4:
            v1 = QVariant(r1.isAvailable() ? 1 : 0);
            v2 = QVariant(r2.isAvailable() ? 1 : 0);
            break;
        default:
            return false;
        }

        bool lessThan;
        if (v1.typeId() == QVariant::String) {
            QString str1 = v1.toString();
            QString str2 = v2.toString();
            lessThan = str1.localeAwareCompare(str2) < 0;
        } else if (v1.typeId() == QVariant::Int || v1.typeId() == QVariant::Double) {
            double num1 = v1.toDouble();
            double num2 = v2.toDouble();
            lessThan = num1 < num2;
        } else if (v1.typeId() == QVariant::Bool) {
            bool bool1 = v1.toBool();
            bool bool2 = v2.toBool();
            lessThan = bool1 < bool2;
        } else {
            return false;
        }

        return (order == Qt::AscendingOrder) ? lessThan : !lessThan;
    });
    emit layoutChanged();
}

