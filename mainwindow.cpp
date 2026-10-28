#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSqlError>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_tableModel(nullptr)
    , m_queryModel(nullptr)
{
    setupUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setWindowTitle(QStringLiteral("Работа с БД — Фильмы"));
    resize(700, 500);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Верхняя панель управления
    QHBoxLayout *topLayout = new QHBoxLayout();

    m_connectBtn = new QPushButton(QStringLiteral("Подключиться"), this);
    m_clearBtn   = new QPushButton(QStringLiteral("Очистить"), this);

    m_categoryCombo = new QComboBox(this);
    m_categoryCombo->addItem(QStringLiteral("Все"));
    m_categoryCombo->addItem(QStringLiteral("Комедии"));
    m_categoryCombo->addItem(QStringLiteral("Ужасы"));

    topLayout->addWidget(m_connectBtn);
    topLayout->addWidget(m_categoryCombo);
    topLayout->addWidget(m_clearBtn);

    // Таблица отображения
    m_tableView = new QTableView(this);

    // Строка статуса
    m_statusLabel = new QLabel(QStringLiteral("Статус: Нет подключения"), this);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(m_tableView);
    mainLayout->addWidget(m_statusLabel);

    // Соединяем сигналы со слотами (QObject::connect)
    connect(m_connectBtn, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connect(m_clearBtn, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(m_categoryCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onCategoryChanged);
}

void MainWindow::onConnectClicked()
{
    if (m_db.isOpen()) {
        m_db.close();
    }

    m_db = QSqlDatabase::addDatabase("QPSQL");
    DbData data;

    m_db.setHostName(data.host);
    m_db.setDatabaseName(data.dbName);
    m_db.setUserName(data.login);
    m_db.setPassword(data.pass);
    m_db.setPort(data.port);

    if (m_db.open()) {
        m_statusLabel->setText(QStringLiteral("Статус: Успешное подключение!"));
        setupModels();
        onCategoryChanged(m_categoryCombo->currentIndex());
    } else {
        m_statusLabel->setText(QStringLiteral("Статус: Ошибка подключения!"));
        QMessageBox::critical(this, QStringLiteral("Ошибка подключения"),
                              m_db.lastError().text());
    }
}

void MainWindow::setupModels()
{
    m_tableModel = new QSqlTableModel(this, m_db);
    m_tableModel->setTable("film");
    m_tableModel->select();
    m_tableModel->setHeaderData(1, Qt::Horizontal, QStringLiteral("Название фильма"));
    m_tableModel->setHeaderData(2, Qt::Horizontal, QStringLiteral("Описание фильма"));

    m_queryModel = new QSqlQueryModel(this);
    m_queryModel->setHeaderData(0, Qt::Horizontal, QStringLiteral("Название фильма"));
    m_queryModel->setHeaderData(1, Qt::Horizontal, QStringLiteral("Описание фильма"));
}

void MainWindow::onCategoryChanged(int index)
{
    if (!m_db.isOpen()) return;

    if (index == 0) {
        // Загружаем Все фильмы через QSqlTableModel
        m_tableView->setModel(m_tableModel);

        for (int i = 0; i < m_tableModel->columnCount(); ++i) {
            if (i != 1 && i != 2) {
                m_tableView->setColumnHidden(i, true);
            } else {
                m_tableView->setColumnHidden(i, false);
            }
        }
    } else {
        QString genre = (index == 1) ? "Comedy" : "Horror";

        QString queryStr = QString(
            "SELECT title, description FROM film f "
            "JOIN film_category fc ON f.film_id = fc.film_id "
            "JOIN category c ON c.category_id = fc.category_id "
            "WHERE c.name = '%1'").arg(genre);

        m_queryModel->setQuery(queryStr, m_db);
        m_tableView->setModel(m_queryModel);

        m_tableView->setColumnHidden(0, false);
        m_tableView->setColumnHidden(1, false);
    }

    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::onClearClicked()
{
    m_tableView->setModel(nullptr);
}
