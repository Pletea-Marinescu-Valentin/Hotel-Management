#include "hotel.h"
#include "room.h" // Include Room class definition

void Hotel::addRoom(const Room& room)
{
    m_rooms.push_back(room);
    // Optionally, you can perform additional operations here
    // For example, you might want to update the UI or save room information to a file/database
}

void Hotel::checkInRoom(int roomNumber)
{
    // Find the room with the specified roomNumber
    for (Room& room : m_rooms) {
        if (room.getNumber() == roomNumber) {
            // Set the room as occupied
            room.setOccupied(true);
            // Optionally, update the UI or perform additional operations
            break;
        }
    }
}

void Hotel::checkOutRoom(int roomNumber)
{
    // Find the room with the specified roomNumber
    for (Room& room : m_rooms) {
        if (room.getNumber() == roomNumber) {
            // Set the room as vacant
            room.setOccupied(false);
            // Optionally, update the UI or perform additional operations
            break;
        }
    }
}

std::vector<std::pair<int, QString>> Hotel::getAllRooms() const
{
    std::vector<std::pair<int, QString>> roomsInfo;
    for (const Room& room : m_rooms) {
        // Get room number and status (occupied or vacant)
        roomsInfo.push_back(std::make_pair(room.getNumber(), room.isOccupied() ? "Occupied" : "Vacant"));
    }
    return roomsInfo;
}

void Hotel::makeReservation(int roomNumber, const QDateTime &startDate, const QDateTime &endDate)
{
    // Find the room with the specified roomNumber
    for (Room& room : m_rooms) {
        if (room.getNumber() == roomNumber) {
            // Update room status or manage reservations
            // For example, you can add the reservation to a vector of reservations
            // Each reservation can be represented as a pair of QDateTime objects
            room.setReserved(true);
            room.setReservationStartDate(startDate);
            room.setReservationEndDate(endDate);
            // Optionally, update the UI or perform additional operations
            break;
        }
    }
}

void Hotel::cancelReservation(int roomNumber)
{
    // Find the room with the specified roomNumber
    for (Room& room : m_rooms) {
        if (room.getNumber() == roomNumber) {
            // Update room status or manage reservations
            room.setReserved(false);
            // Optionally, update the UI or perform additional operations
            break;
        }
    }
}

std::vector<std::pair<int, QString>> Hotel::getReservations() const {
    // Implement logic to retrieve reservations
    std::vector<std::pair<int, QString>> reservations;

    // Example: Adding dummy reservations
    reservations.push_back(std::make_pair(101, "Reserved"));
    reservations.push_back(std::make_pair(102, "Reserved"));

    return reservations;
}
