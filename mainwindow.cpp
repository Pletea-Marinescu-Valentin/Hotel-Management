#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "roommodel.h"
#include "roomdialog.h"
#include "QInputDialog"
#include "QMessageBox"
#include "QFile"
#include "QStandardItem"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupRoomModel();
    connect(ui->addRoomsButton, &QPushButton::clicked, this, &MainWindow::addRooms);
    connect(ui->removeRoomsButton, &QPushButton::clicked, this, &MainWindow::onRemoveRoomsButtonClicked);
    connect(ui->editRoomsButton, &QPushButton::clicked, this, &MainWindow::onEditRoomsButtonClicked);
    connect(ui->checkIn, &QPushButton::clicked, this, &MainWindow::checkIn);
    connect(ui->checkOut, &QPushButton::clicked, this, &MainWindow::checkOut);
    connect(ui->addReservation, &QPushButton::clicked, this, &MainWindow::addReservation);
    connect(ui->tableViewRooms->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::sortRoomsByColumn);


    connect(qApp, &QApplication::aboutToQuit, this, &MainWindow::saveTableData);

    loadTableData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupRoomModel()
{
    roomModel = new RoomModel(this);
    ui->tableViewRooms->setModel(roomModel);
    ui->tableViewRooms->setSortingEnabled(true);
}

void MainWindow::addRooms()
{
    RoomDialog dialog(roomModel->getRooms(), this, false);
    if (dialog.exec() == QDialog::Accepted) {
        Room room = dialog.getRoom();
        QList<Room> rooms = roomModel->getRooms();
        rooms.append(room);
        roomModel->setRooms(rooms);
    }
}

void MainWindow::onRemoveRoomsButtonClicked()
{
    bool ok;
    int roomNumber = QInputDialog::getInt(this, tr("Remove Room"), tr("Enter the room number to remove:"), 0, 0, INT_MAX, 1, &ok);
    if (ok) {
        for (int i = 0; i < roomModel->rowCount(); ++i) {
            QModelIndex index = roomModel->index(i, 0);
            if (roomModel->data(index).toInt() == roomNumber) {
                roomModel->removeRoom(i);
                return;
            }
        }
        QMessageBox::warning(this, tr("Room Not Found"), tr("The room number entered does not exist."));
    }
}

void MainWindow::onEditRoomsButtonClicked()
{
    bool ok;
    int roomNumber = QInputDialog::getInt(this, tr("Edit Room"), tr("Enter the room number to edit:"), 0, 0, INT_MAX, 1, &ok);
    if (ok) {
        int rowIndex = -1;
        QList<Room> rooms = roomModel->getRooms();
        for (int i = 0; i < rooms.size(); ++i) {
            if (rooms[i].getRoomNumber().toInt() == roomNumber) {
                rowIndex = i;
                break;
            }
        }

        if (rowIndex != -1) {
            Room room = rooms.at(rowIndex);

            RoomDialog dialog(roomModel->getRooms(), this, true);
            if (dialog.exec() == QDialog::Accepted) {
                Room editedRoom = dialog.getRoom();

                rooms[rowIndex] = editedRoom;
                roomModel->setRooms(rooms);
            }
        } else {
            QMessageBox::warning(this, tr("Room Not Found"), tr("The room number entered does not exist."));
        }
    }
}

void MainWindow::saveTableDataToFile(const QString& filename) {
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int row = 0; row < roomModel->rowCount(); ++row) {
            QStringList rowData;
            for (int column = 0; column < roomModel->columnCount(); ++column) {
                QModelIndex index = roomModel->index(row, column);
                rowData << roomModel->data(index).toString();
            }
            out << rowData.join(",") << "\n";
        }
        file.close();
    } else {
        qDebug() << "Failed to open file for writing:" << file.errorString();
    }
}

void MainWindow::loadTableDataFromFile(const QString& filename) {
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        roomModel->removeRows(0, roomModel->rowCount()); // Clear existing data
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList rowData = line.split(",", Qt::SkipEmptyParts);
            QList<QStandardItem*> items;
            for (const QString& data : rowData) {
                items.append(new QStandardItem(data));
            }
        }
        file.close();
    } else {
        qDebug() << "Failed to open file for reading:" << file.errorString();
    }
}

void MainWindow::saveTableData() {
    QFile file("roomData.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to save table data."));
        return;
    }

    QTextStream out(&file);
    QList<Room> rooms = roomModel->getRooms();
    for (const Room &room : rooms) {
        out << room.getRoomNumber() << "," << room.getRoomType() << "," << room.getGuestName() << "," << room.getPrice() << "," << room.isAvailable() << "\n";
    }

    file.close();
}

void MainWindow::loadTableData() {
    QFile file("roomData.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to load table data."));
        return;
    }

    QList<Room> rooms;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        if (fields.size() == 5) {
            QString roomNumber = fields[0];
            QString roomType = fields[1];
            QString guestName = fields[2];
            double price = fields[3].toDouble();
            bool available = (fields[4].toInt() == 1) ? true : false;
            rooms.append(Room(roomNumber, roomType, guestName, price, available));
        }
    }

    file.close();
    roomModel->setRooms(rooms);
}

void MainWindow::addReservation() {
    QString guestName = QInputDialog::getText(this, tr("Add Reservation"), tr("Enter guest name:"));
    if (!guestName.isEmpty()) {
        QModelIndexList selectedIndexes = ui->tableViewRooms->selectionModel()->selectedIndexes();

        if (selectedIndexes.size() == 1) {
            int rowIndex = selectedIndexes.first().row();

            Room room = roomModel->getRooms().at(rowIndex);

            room.setGuestName(guestName);
            room.setAvailable(false);

            QList<Room> rooms = roomModel->getRooms();
            rooms[rowIndex] = room;
            roomModel->setRooms(rooms);

            QMessageBox::information(this, tr("Reservation Successful"), tr("Reservation added successfully."));
        } else {
            QMessageBox::warning(this, tr("Select a Room"), tr("Please select a single room to add reservation."));
        }
    }
}

void MainWindow::checkIn() {
    QModelIndexList selectedIndexes = ui->tableViewRooms->selectionModel()->selectedIndexes();
    if (selectedIndexes.size() == 1) {
        int rowIndex = selectedIndexes.first().row();
        Room room = roomModel->getRooms().at(rowIndex);
        room.setAvailable(false);
        QList<Room> rooms = roomModel->getRooms();
        rooms[rowIndex] = room;
        roomModel->setRooms(rooms);
        QMessageBox::information(this, tr("Check-in Successful"), tr("Room checked in successfully."));
    } else {
        QMessageBox::warning(this, tr("Select a Room"), tr("Please select a single room to check in."));
    }
}

void MainWindow::checkOut() {
    QModelIndexList selectedIndexes = ui->tableViewRooms->selectionModel()->selectedIndexes();

    if (selectedIndexes.size() == 1) {
        int rowIndex = selectedIndexes.first().row();

        Room room = roomModel->getRooms().at(rowIndex);

        room.setAvailable(true);
        room.setGuestName("");

        // Update the room data in the model
        QList<Room> rooms = roomModel->getRooms();
        rooms[rowIndex] = room;
        roomModel->setRooms(rooms);

        // Inform the user that the check-out process is successful
        QMessageBox::information(this, tr("Check-out Successful"), tr("Room checked out successfully."));
    } else {
        // Inform the user to select a single room
        QMessageBox::warning(this, tr("Select a Room"), tr("Please select a single room to check out."));
    }
}

void MainWindow::sortRoomsByColumn(int column)
{
    ui->tableViewRooms->sortByColumn(column, Qt::AscendingOrder);
}






