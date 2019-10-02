#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEnginePage>

class QWebEnginePage;
class SensorDataController;

namespace Ui {
class MainWindow;
}
///////////////////////////////////////////////////////

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  QWebEnginePage *m_page;
  SensorDataController *m_sdc;

private slots:
  void pageFeaturePermissionRequested(const QUrl &securityOrigin, QWebEnginePage::Feature feature);
  void exportTraceToHtmlFile();

  void btnRefresh_pressed();
  void btnSave_pressed();
  void btnLoadData_pressed();
};

#endif // MAINWINDOW_H
