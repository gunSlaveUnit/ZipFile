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

    selectedFileName = new QLabel("Not Selected");
    centralLayout->addWidget(selectedFileName, 0, 0);

    selectFileButton = new QPushButton(tr("Select"));
    centralLayout->addWidget(selectFileButton, 0, 1);
    connect(selectFileButton, SIGNAL(clicked(bool)), this, SLOT(openFileDialog()));
}

MainWindow::~MainWindow() {
    delete selectedFileName;
    delete selectFileButton;
    delete centralLayout;
    delete centralWidget;
}

void MainWindow::openFileDialog() {
    auto dialog = QFileDialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    auto filename = dialog.getOpenFileName();
    selectedFileName->setText(filename.isEmpty() ? "Not Selected" : filename);
}
