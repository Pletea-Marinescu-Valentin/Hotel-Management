#include "roomdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>

RoomDialog::RoomDialog(const QList<Room>& existingRooms, QWidget *parent, bool editMode) :
    QDialog(parent), existingRooms(existingRooms), isEditMode(editMode)
{
    setWindowTitle("Add Room");

    lineEditRoomNumber = new QLineEdit;
    comboBoxRoomType = new QComboBox;
    comboBoxRoomType->addItem("Single");
    comboBoxRoomType->addItem("Double");
    comboBoxRoomType->addItem("Suite");
    lineEditGuestName = new QLineEdit;
    doubleSpinBoxPrice = new QDoubleSpinBox;
    checkBoxAvailable = new QCheckBox("Available");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel("Room Number:"));
    mainLayout->addWidget(lineEditRoomNumber);
    mainLayout->addWidget(new QLabel("Room Type:"));
    mainLayout->addWidget(comboBoxRoomType);
    mainLayout->addWidget(new QLabel("Guest Name:"));
    mainLayout->addWidget(lineEditGuestName);
    mainLayout->addWidget(new QLabel("Price:"));
    mainLayout->addWidget(doubleSpinBoxPrice);
    mainLayout->addWidget(checkBoxAvailable);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *okButton = new QPushButton("OK");
    QPushButton *cancelButton = new QPushButton("Cancel");
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(okButton, &QPushButton::clicked, this, &RoomDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &RoomDialog::reject);
}

Room RoomDialog::getRoom() const
{
    QString roomNumber = lineEditRoomNumber->text();
    QString roomType = comboBoxRoomType->currentText();
    QString guestName = lineEditGuestName->text();
    double price = doubleSpinBoxPrice->value();
    bool available = checkBoxAvailable->isChecked();

    return Room(roomNumber, roomType, guestName, price, available);
}

void RoomDialog::accept()
{
    if (lineEditRoomNumber->text().isEmpty() || doubleSpinBoxPrice->value() <= 0) {
        QMessageBox::critical(this, "Error", "Please fill in all mandatory fields.");
        return;
    }

    for (const Room &existingRoom : existingRooms) {
        if (existingRoom.getRoomNumber() == lineEditRoomNumber->text() && !isEditMode) {
            QMessageBox::critical(this, "Error", "A room with the same number already exists.");
            return;
        }
    }

    QString roomNumber = lineEditRoomNumber->text();
    if (!roomNumber.contains(QRegularExpression("^\\d+$"))) {
        QMessageBox::critical(this, "Error", "Please enter a valid room number.");
        return;
    }

    QDialog::accept();
}
