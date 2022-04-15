//
// Created by gunSlaveUnit on 13.04.2022.
//

#ifndef ADAPTIVE_HUFFMAN_MAINWINDOW_H
#define ADAPTIVE_HUFFMAN_MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QGraphicsView>
#include <QLabel>
#include <QFileDialog>

#include "../coder/AdaptiveHuffmanCoder.h"


class MainWindow final: public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow() override;
public slots:
    void openFileDialog();
private:
    const qint32 WINDOW_WIDTH = 300, WINDOW_HEIGHT = 200;
    const QString WINDOW_TITLE = "ZipFile";

    AdaptiveHuffmanCoder coder;

    QGraphicsView *centralWidget;
    QGridLayout *centralLayout;
    QLabel *selectedFileName;
    QPushButton *selectFileButton;
    QPushButton *startButton;
};


#endif //ADAPTIVE_HUFFMAN_MAINWINDOW_H
