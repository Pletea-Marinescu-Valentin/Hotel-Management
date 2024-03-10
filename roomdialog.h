#ifndef ROOMDIALOG_H
#define ROOMDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include "room.h"

class RoomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RoomDialog(const QList<Room>& existingRooms, QWidget *parent = nullptr, bool editMode = false);
    Room getRoom() const;

private slots:
    void accept();

private:
    QLineEdit *lineEditRoomNumber;
    QComboBox *comboBoxRoomType;
    QLineEdit *lineEditGuestName;
    QDoubleSpinBox *doubleSpinBoxPrice;
    QCheckBox *checkBoxAvailable;
    QList<Room> existingRooms;
    bool isEditMode;
};

#endif
