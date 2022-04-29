//
// Created by gunSlaveUnit on 13.04.2022.
//

#include "MainWindow.h"

MainWindow::MainWindow() {
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setWindowTitle(WINDOW_TITLE);

    centralWidget = new QGraphicsView(this);
    centralWidget->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    centralLayout = new QGridLayout(centralWidget);

    fileLabel = new QLabel(tr("File: "));
    centralLayout->addWidget(fileLabel, 0, 0);
    selectedFileName = new QLabel("not selected");
    centralLayout->addWidget(selectedFileName, 0, 1);

    selectFileButton = new QPushButton(tr("Select"));
    centralLayout->addWidget(selectFileButton, 1, 0);
    connect(selectFileButton, SIGNAL(clicked(bool)), this, SLOT(openFileDialog()));

    splitter = new QSplitter;
    centralLayout->addWidget(splitter, 2, 0, 1, 2);

    startButton = new QPushButton(tr("Start"));
    centralLayout->addWidget(startButton, 3, 0);

    closeButton = new QPushButton(tr("Close"));
    centralLayout->addWidget(closeButton, 3, 1);
    connect(closeButton, &QPushButton::clicked, this,
            [this]() {
                this->close();
            });

    connectMethodDependMode();
}

MainWindow::~MainWindow() {
    delete splitter;
    delete selectedFileName;
    delete selectFileButton;
    delete closeButton;
    delete centralLayout;
    delete centralWidget;
}

void MainWindow::openFileDialog() {
    auto dialog = QFileDialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);

    auto filename = dialog.getOpenFileName();
    QFileInfo fi(filename);
    filename = fi.fileName();
    selectedFileName->setText(filename.isEmpty() ? "Not Selected" : filename);
    auto ext = fi.completeSuffix();
    setWorkingModeDependFileExt(ext);
}

void MainWindow::setWorkingModeDependFileExt(const QString &ext) {
    if (ext == "ahf")
        MODE = DECODE;
    else
        MODE = ENCODE;

    connectMethodDependMode();
}

void MainWindow::connectMethodDependMode() {
    startButton->disconnect();

    if (MODE == ENCODE)
        connect(startButton, &QPushButton::clicked, this,
                [this]() {
                    coder.encode(selectedFileName->text().toStdString());
                });
    else
        connect(startButton, &QPushButton::clicked, this,
                [this]() {
                    coder.decode(selectedFileName->text().toStdString());
                });
}
