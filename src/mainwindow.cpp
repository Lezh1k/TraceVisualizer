#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QPushButton>

#include "coordinates/Coordinates.h"
#include "coordinates/Geohash.h"
#include "commons/SensorController.h"

static const QString g_mapDiv = "megamap";
static const QString g_baseHtml = "<!DOCTYPE html>\n"
                                  "<html>\n"
                                  "<head>\n"
                                  "  <style>\n"
                                  "     #%1 {\n"
                                  "      height: 600px;\n"
                                  "      width: 100%;\n"
                                  "     }\n"
                                  "  </style>\n"
                                  "</head>\n"
                                  "<body>\n"
                                  "  <h3>Trace visualizer google map</h3>\n"
                                  "  <div id=\"%1\"></div>\n"
                                  "  <script>\n"
                                  "    function initMap() {\n"
                                  "      var bishkek = {lat: %3, lng: %4};\n"
                                  "      var map = new google.maps.Map(document.getElementById('%1'), {\n"
                                  "        zoom: 14,\n"
                                  "        center: bishkek\n"
                                  "      });\n"
                                  "       %2\n"
                                  "    }\n"                                  "  </script>\n"
                                  "  <script async defer\n"
                                  "  src=\"https://maps.googleapis.com/maps/api/js?callback=initMap\">\n"
                                  "  </script>\n"
                                  "</body>\n"
                                  "</html>\n";

static QString jsCoordsString(const std::vector<geopoint_t>& lst,
                              const QString& prefix,
                              const QString& color);

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);  
  m_page = ui->m_webView->page();
  connect(m_page, &QWebEnginePage::featurePermissionRequested,
          this, &MainWindow::pageFeaturePermissionRequested);

  connect(ui->m_btnRefresh, &QPushButton::pressed, this, &MainWindow::btnRefresh_pressed);
  connect(ui->m_btnSave, &QPushButton::pressed, this, &MainWindow::btnSave_pressed);
}
///////////////////////////////////////////////////////

MainWindow::~MainWindow() {
  delete ui;
}
///////////////////////////////////////////////////////

void MainWindow::pageFeaturePermissionRequested(const QUrl &securityOrigin,
                                                QWebEnginePage::Feature feature) {
  if (feature != QWebEnginePage::Geolocation)
    return;
  QMessageBox msgBox(this);
  msgBox.setText(tr("%1 wants to know your location").arg(securityOrigin.host()));
  msgBox.setInformativeText(tr("Do you want to send your current location to this website?"));
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::Yes);
  QWebEnginePage::PermissionPolicy p =
      msgBox.exec() == QMessageBox::Yes ? QWebEnginePage::PermissionGrantedByUser : QWebEnginePage::PermissionDeniedByUser;
  m_page->setFeaturePermission(securityOrigin, feature, p);
}
///////////////////////////////////////////////////////

void MainWindow::exportTraceToHtmlFile() {
  QString fs = QFileDialog::getSaveFileName(nullptr, "Save route as html", QString(), "Web (*.html)");
  if (fs.isEmpty())
    return;
  m_page->save(fs);
}
///////////////////////////////////////////////////////

void MainWindow::btnRefresh_pressed() {
  //!todo read from file.
  std::vector<geopoint_t> lstCoords;
  qDebug() << "Src distance : " << CoordCaclulateDistance(lstCoords);
  //!todo filter with geohash JUST FOR DISPLAY! so need to set big precision.
  QString srcCoordsStr = jsCoordsString(lstCoords, "src", "#FF0000");
  QString allCoordsStr = srcCoordsStr;
  geopoint_t p(42.87336, 74.61873);
  if (!lstCoords.empty()) p = lstCoords[0];
  QString html = g_baseHtml.arg(g_mapDiv).arg(allCoordsStr).arg(p.Latitude).arg(p.Longitude);
  m_page->setHtml(html);
}
///////////////////////////////////////////////////////

void MainWindow::btnSave_pressed() {
  exportTraceToHtmlFile();
}
///////////////////////////////////////////////////////

QString jsCoordsString(const std::vector<geopoint_t>& lst,
                       const QString& prefix,
                       const QString& color) {
  QString coordsStr = QString("var lst%1 = {\n").arg(prefix);
  for (size_t i = 0; i < lst.size(); ++i) {
    coordsStr += QString("%1coord%2: {\n"
                         "  center: {lat: %3, lng: %4}\n"
                         "},\n ").arg(prefix).arg(i).arg(lst[i].Latitude).arg(lst[i].Longitude);
  }
  coordsStr += "};\n";
  coordsStr += QString("for (var c in lst%1) {\n"
                       "  var circle = new google.maps.Circle({\n"
                       "  strokeColor: '%2',\n"
                       "  strokeOpacity: 0.8,\n"
                       "  strokeWeight: 2,\n"
                       "  fillColor: '%2',\n"
                       "  fillOpacity: 0.35,\n"
                       "  map: map,\n"
                       "  center: lst%1[c].center,\n"
                       "  radius: 10\n"
                       "});\n").arg(prefix).arg(color);
  coordsStr += "}\n";
  return coordsStr;
}
///////////////////////////////////////////////////////
