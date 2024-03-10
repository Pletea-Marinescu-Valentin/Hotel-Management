#ifndef ROOMMODEL_H
#define ROOMMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "room.h"

class RoomModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit RoomModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void setRooms(const QList<Room> &rooms);
    void removeRoom(int index);

    QList<Room> getRooms() const;

    void sort(int column, Qt::SortOrder order);

private:
    QList<Room> m_rooms;
    QMap<int, Qt::SortOrder> sortOrders;
    int lastSortedColumn;
    Qt::SortOrder lastSortOrder;
};

#endif
