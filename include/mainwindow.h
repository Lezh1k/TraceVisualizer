#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEnginePage>

class QWebEnginePage;
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

  void initMap();

private slots:
  void pageFeaturePermissionRequested(const QUrl &securityOrigin, QWebEnginePage::Feature feature);
  void exportTraceToHtmlFile();
};

#endif // MAINWINDOW_H
