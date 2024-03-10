#include "room.h"

Room::Room() {
    roomNumber = "";
    roomType = "";
    guestName = "";
    price = 0.0;
    available = false;
}

Room::Room(QString roomNumber, QString roomType, QString guestName, double price, bool available) {
    this->roomNumber = roomNumber;
    this->roomType = roomType;
    this->guestName = guestName;
    this->price = price;
    this->available = available;
}

QString Room::getRoomNumber() const {
    return roomNumber;
}

void Room::setRoomNumber(const QString &roomNumber) {
    this->roomNumber = roomNumber;
}

QString Room::getRoomType() const {
    return roomType;
}

void Room::setRoomType(const QString &roomType) {
    this->roomType = roomType;
}

QString Room::getGuestName() const {
    return guestName;
}

void Room::setGuestName(const QString &guestName) {
    this->guestName = guestName;
}

double Room::getPrice() const {
    return price;
}

void Room::setPrice(double price) {
    this->price = price;
}

bool Room::isAvailable() const {
    return available;
}

void Room::setAvailable(bool available) {
    this->available = available;
}
