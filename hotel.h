#ifndef HOTEL_H
#define HOTEL_H

#include <vector>
#include <QString>
#include "room.h"
#include <QDateTime> // For reservation date/time
#include <utility> // For std::pair

class Room;

class Hotel
{
public:
    void addRoom(const Room& room);
    void checkInRoom(int roomNumber);
    void checkOutRoom(int roomNumber);
    std::vector<std::pair<int, QString>> getAllRooms() const;
    void makeReservation(int roomNumber, const QDateTime &startDate, const QDateTime &endDate);
    void cancelReservation(int roomNumber);
    std::vector<std::pair<int, QString>> getReservations() const;

private:
    std::vector<Room> m_rooms;
    // You may need additional data structures for managing reservations, such as a vector of reservations
};

#endif // HOTEL_H
