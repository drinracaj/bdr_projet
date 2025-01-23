#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QMainWindow>
#include <QSpinBox>
#include <QTabWidget>
#include <QTableWidget>

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:
  void on_menu_clicked();
  void on_apply_btn_clicked();

 private:
  QTabWidget* tab_widget_;
  QAction* menu_;
  QTableWidget* table_widget_;
  QComboBox* building_cb_;
  QSpinBox* year_picker_;
  void setup();
  void create_tabs();
};

#endif  // MAINWINDOW_H