#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include "cache.h"
#include "updategui.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(UpdateGUI *ug, QWidget *parent = 0);
    ~MainWindow();
    UpdateGUI *updategui;

private:
    Ui::MainWindow *ui;

public slots:
    void updateCPUs(std::vector<Processor*> CPUs);
    void updateCaches(std::vector<CacheBlock*> cacheBlocks);
    void updateCC(std::vector<CacheController*> cacheControllers);
    void updateMemory(std::string *memory);
    void updateBus(std::string str);

private slots:
    void on_cb_clk_clicked();
    void on_btn_clk_clicked();
};

#endif // MAINWINDOW_H
