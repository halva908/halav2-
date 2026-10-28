#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QTableView>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include "database.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConnectClicked();
    void onClearClicked();
    void onCategoryChanged(int index);

private:
    void setupUI();
    void setupModels();

    QTableView  *m_tableView;
    QPushButton *m_connectBtn;
    QPushButton *m_clearBtn;
    QComboBox   *m_categoryCombo;
    QLabel      *m_statusLabel;

    QSqlDatabase      m_db;
    QSqlTableModel   *m_tableModel;
    QSqlQueryModel  *m_queryModel;
};

#endif
