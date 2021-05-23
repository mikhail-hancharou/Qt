#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonArray>
#include<QPainter>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:  
    void setData(QJsonValue jv, int i);

    void caseInfo(QPainter *qp);

    void currentCaseOfInfo(int cLine, int bound, bool borderSides, int i, QPainter *qp);

    void chooseData();

    //void paintEvent(QPaintEvent *event);

    void setBackground(QPainter *qp);

    void drawWindAngle(int space, int cLine, int num);

    void drawUvi(int space, int cLine, int num);

    void drawMoon(int space, int cLine, int num);

    void drawCloud(int space, int cLine, int num);

    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void wheelEvent(QWheelEvent *ev);
    void paintEvent(QPaintEvent *event);

private slots:
    // Обработчик данных полученных от объекта QNetworkAccessManager
    void onResult(QNetworkReply *reply);

    void onResultCoord(QNetworkReply *reply);

    void on_setl_clicked();

    void on_setLat_valueChanged(double arg1);

    void on_setLon_valueChanged(double arg1);

    void on_current_clicked();

    void on_minutely_clicked();

    void on_hourly_clicked();

    void on_daily_clicked();

    void on_locations_activated(const QString &arg1);

    //void on_cityName_editingFinished();

    //void on_cityName_textChanged(const QString &arg1);

    void on_cityName_textEdited(const QString &arg1);

private:
    Ui::Widget *ui;
    QNetworkAccessManager *networkManager;
    QNetworkAccessManager *networkManagerCoord;//for getting coords
    QString url;

    //drawing
    int i = 0;
    int  n = 0;
    bool animation = false;
    int prev = 0;
    QColor col;
    QPainter *qp;

    //for getting data
    QJsonArray ja;
    QJsonObject jo;
    QJsonValue jv;
    QString text;
    bool lastClickLE = false;//lineEdit

    //general
    int forecastCase = 0;
    int forecastCaseTemp = 0;
    double lat = 0;
    double lon = 0;
    double tempLat = 0;//temporary
    double tempLon = 0;//temporary
    QString timezone;
    int timezone_offset;
    bool existInfo = false;

    //current, hourly, daily
    QString data[61];//partially minutely
    QString dt[61];//partially minutely
    QString sunrise[8];//ain't in hourly
    QString sunset[8];//ain't in hourly
    double temp[48];
    double feels_like[48];
    int pressure[48];
    int humidity[48];
    double dew_point[48];
    double uvi[48];
    int clouds[48];
    int visibility[48];
    double wind_speed[48];
    int wind_deg[48];
    double wind_gust[48];
    int id[48];
    QString main[48];
    QString description[48];
    QString icon[48];

    //minutely only
    int precipitation[61];

    //hourlu & daily only
    double pop[48];

    //daily only
    QString moonrise[8];
    QString moonset[8];
    double moon_phase[8];
    double tempDay[8];
    double tempMin[8];
    double tempMax[8];
    double tempNight[8];
    double tempEve[8];
    double tempMorn[8];
    double flDay[8];//feels_like
    double flNight[8];//feels_like
    double flEve[8];//feels_like
    double flMorn[8];//feels_like

    QString message;
};

#endif // WIDGET_H
