//
// Created by gunSlaveUnit on 13.04.2022.
//

#ifndef ADAPTIVE_HUFFMAN_MAINWINDOW_H
#define ADAPTIVE_HUFFMAN_MAINWINDOW_H

#include <QMainWindow>


class MainWindow final: public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
private:
    const qint32 WINDOW_WIDTH = 300, WINDOW_HEIGHT = 200;
    const QString WINDOW_TITLE = "ZipFile";
};


#endif //ADAPTIVE_HUFFMAN_MAINWINDOW_H
