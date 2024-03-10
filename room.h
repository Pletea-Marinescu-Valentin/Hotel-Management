#ifndef ROOM_H
#define ROOM_H

#include <QString>

class Room {
public:
    Room();
    Room(QString roomNumber, QString roomType, QString guestName, double price, bool available);

    QString getRoomNumber() const;
    void setRoomNumber(const QString &roomNumber);

    QString getRoomType() const;
    void setRoomType(const QString &roomType);

    QString getGuestName() const;
    void setGuestName(const QString &guestName);

    double getPrice() const;
    void setPrice(double price);

    bool isAvailable() const;
    void setAvailable(bool available);

private:
    QString roomNumber;
    QString roomType;
    QString guestName;
    double price;
    bool available;
};

#endif
