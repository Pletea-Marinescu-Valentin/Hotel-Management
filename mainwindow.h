#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "roommodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addRooms();
    void onRemoveRoomsButtonClicked();
    void onEditRoomsButtonClicked();
    void addReservation();
    void checkIn();
    void checkOut();
    void sortRoomsByColumn(int);

private:
    Ui::MainWindow *ui;
    RoomModel *roomModel;

    void setupRoomModel();
    void saveTableDataToFile(const QString &);
    void loadTableDataFromFile(const QString &);
    void loadTableData();
    void saveTableData();
};

#endif
