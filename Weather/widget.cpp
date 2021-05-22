#include "widget.h"
#include "ui_widget.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QUrl>
#include <QDoubleSpinBox>
#include <ctime>
#include <QDate>
#include "design.h"
#include <QPainter>
#include <QWheelEvent>
#include <QThread>
#include <QDebug>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    this->setStyleSheet("background-color: #5F9EA0;");
    ui->setupUi(this);
    ui->setLat->setRange(-180, 180);
    ui->setLon->setRange(-180, 180);
    ui->setLat->setSingleStep(0.1);
    ui->setLon->setSingleStep(0.1);
    ui->setLat->setDecimals(4);
    ui->setLon->setDecimals(4);
    ui->progressBar->setRange(0, 10);

    ui->progressBar->setStyleSheet("QProgressBar{color:rgba(255,255,250,0);"
                                   "border-radius: 5px;"
                                   "border-width: 3px;"
                                   "background-color: #5F9EA0}"
                                   "QProgressBar::chunk {background-color: #FFCF40}");
    ui->progressBar->setTextVisible(false);
    ui->progressBar->setMaximumHeight(3);
    //#FF8800
    ui->textBrowser->setStyleSheet("QTextBrowser { color: #B0E0E6; background-color: rgba(95,158,160,80);"
                                   "border-style: outset;"
                                   "border-width: 2px;"
                                   "border-radius: 5px;"
                                   "border-color: #483D8B;"
                                   "font: bold 18px;}");

    ui->textBrowser_2->setStyleSheet("QTextBrowser { color: #B0E0E6; background-color: rgba(95,158,160,0);"
                                   "border-style: outset;"
                                   "border-width: 0px;"
                                   "border-radius: 5px;"
                                   "border-color: rgba(95,158,160,0);"
                                   "font: bold 22px;}");

    QStringList loc = {"...", "London", "Minsk", "Chicago", "Paris", "San Francisco", "Los Angeles"};
    ui->locations->addItems(loc);
    setDesign(ui->setLat, ui->setLon, ui->current, ui->minutely, ui->hourly, ui->daily,
              ui->setl, ui->locations, ui->cityName, ui->label, ui->label_2, ui->groupBox);

    networkManager = new QNetworkAccessManager();
    networkManagerCoord = new QNetworkAccessManager();
    connect(networkManager, &QNetworkAccessManager::finished, this, &Widget::onResult);
    connect(networkManagerCoord, &QNetworkAccessManager::finished, this, &Widget::onResultCoord);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setBackground(QPainter *qp)
{
     QString skyC[5] = {"#00FFFF", "#00BFFF", "#B04F4F", "#4B0082", "#000000"};//color hex: morning, day, sunset, evening, night
     QString cloudC[5] = {"#AFEEEE", "#87CEFA", "#8E5757", "#5A445F", "#293133"};

     //color alghortitm
     int skyCase;
     if (forecastCase == 0)
     {
         i = 0;
     }
     QString str = dt[i].left(2);
     int hours = str.toInt();
     if (hours >= 5 && hours <= 10)
     {
         skyCase = 0;
     }
     else if (hours >= 11 && hours <= 17)
     {
         skyCase = 1;
     }
     else if (hours >= 18 && hours <= 20)
     {
         skyCase = 2;
     }
     else if ((hours >= 21 && hours <= 24) || hours <= 1)
     {
         skyCase = 3;
     }
     else if (hours >= 2 && hours <= 4)
     {
         skyCase = 4;
     }
     if (clouds[i] >= 30 && forecastCase != 1)
     {
          col = cloudC[skyCase];
     }
     else
     {
         col = skyC[skyCase];
     }

     if (existInfo == true)
     {
         if (skyCase == 0 || skyCase == 1)
         {
             ui->textBrowser->setStyleSheet("QTextBrowser { color: #483D8B; background-color: rgba(95,158,160,80);"
                                            "border-style: outset;"
                                            "border-width: 2px;"
                                            "border-radius: 5px;"
                                            "border-color: #483D8B;"
                                            "font: bold 18px;}");

             ui->textBrowser_2->setStyleSheet("QTextBrowser { color: #483D8B; background-color: rgba(95,158,160,0);"
                                            "border-style: outset;"
                                            "border-width: 0px;"
                                            "border-radius: 5px;"
                                            "border-color: rgba(95,158,160,0);"
                                            "font: bold 22px;}");
         }
         else
         {
             ui->textBrowser->setStyleSheet("QTextBrowser { color: #B0E0E6; background-color: rgba(95,158,160,80);"
                                            "border-style: outset;"
                                            "border-width: 2px;"
                                            "border-radius: 5px;"
                                            "border-color: #483D8B;"
                                            "font: bold 18px;}");

             ui->textBrowser_2->setStyleSheet("QTextBrowser { color: #B0E0E6; background-color: rgba(95,158,160,0);"
                                            "border-style: outset;"
                                            "border-width: 0px;"
                                            "border-radius: 5px;"
                                            "border-color: rgba(95,158,160,0);"
                                            "font: bold 22px;}");
         }
         QString offset = QString::number(timezone_offset);
         if (forecastCase != 1)
         {
             ui->textBrowser->setText("Data: " + data[i].leftJustified(23, ' ') + "Time: " + dt[i].leftJustified(20, ' ') + "\n"
                                  "Timezone offset: " + offset.leftJustified(4, ' ') + "Timezone: " + timezone + "\n"
                                  "Weather: " + description[i] + "\n");
         }
         else
         {
             ui->textBrowser->setText("Data: " + data[i].leftJustified(23, ' ') + "Time: " + dt[i].leftJustified(20, ' ') + "\n"
                                  "Timezone offset: " + offset.leftJustified(4, ' ') + "Timezone: " + timezone + "\n");
         }
         if (forecastCase != 1 && forecastCase != 3)
         {
             ui->textBrowser_2->setText("Temp: " + QString::number(temp[i]).rightJustified(12) + "\n"
                                    "Feels like: " + QString::number(feels_like[i]));
         }
         if (forecastCase == 3)
         {
             ui->textBrowser_2->setText("Temp: " + QString::number(tempDay[i]).rightJustified(12) + "\n"
                                    "Feels like: " + QString::number(flDay[i]));
         }
     }

     //
     QPainterPath path;
     path.addRoundedRect(QRectF(25, 25, 800, 600), 25, 25);
     qp->setPen(col);
     QBrush brush(col);
     qp->fillPath(path, brush);
     qp->drawPath(path);
}

void Widget::wheelEvent(QWheelEvent *ev)
{
    if (existInfo != false)
    {
        if(ev->angleDelta().y() / 120 > 0)
        {
            i--;
            n -= 9;// up Wheel
        }
        else if(ev->angleDelta().y() / 120 < 0)
        {
            i++;
            n += 9;//down Wheel
        }

        if (n < 0) n = 0;
        i = n / 90;

        if (forecastCase == 0) n = 0;
        else if (forecastCase == 1 && n > 60 * 90) n = 60 * 90;
        else if (forecastCase == 2 && n > 47 * 90) n = 47 * 90;
        else if (forecastCase == 3 && n > 7 * 90) n = 7 * 90;

        if (i < 0 || forecastCase == 0) i = 0;
        else if (forecastCase == 1 && i > 60) i = 60;
        else if (forecastCase == 2 && i > 47) i = 47;
        else if (forecastCase == 3 && i > 7) i = 7;
    }
}

int count = 0;

void Widget::caseInfo(QPainter *qp)
{
    qDebug() << "loop" << count++;
    if (existInfo == true)
    {
    int bound = 0;
    int line;
    if (forecastCase == 1) bound = 60;
    else if (forecastCase == 2) bound = 47;
    else if (forecastCase == 3) bound = 7;
    line = 400 - n % 90;
    currentCaseOfInfo(line - 40, bound, true, i, qp);

    QPainterPath path;
    QColor cl;
    //debug line
    //cl.setRgb(100, 20, 200, 255);
    //qp->setPen(cl);
    //qp->drawLine(50, 400, 800, 400);

    path.addRect(QRectF(50, 270, 750, 80));//top gradient
    QLinearGradient lGrad(QPointF(400, 285), QPointF(400, 355));
    lGrad.setColorAt(0, col);
    cl.setRgb(0, 0, 0, 0);
    qp->setPen(cl);
    lGrad.setColorAt(1, cl);
    QBrush brush(lGrad);
    qp->fillPath(path, brush);
    qp->drawPath(path);

    qp->fillRect(50, 180, 750, 90, col);//top rect to hide

    path.addRect(QRectF(50, 450, 750, 80));//bottom gradient
    QLinearGradient lGrad2(QPointF(400, 520), QPointF(400, 445));
    lGrad2.setColorAt(0, col);
    cl.setRgb(0, 0, 0, 0);
    qp->setPen(cl);
    lGrad2.setColorAt(1, cl);
    QBrush br(lGrad2);
    qp->fillPath(path, br);
    qp->drawPath(path);

    qp->fillRect(50, 530, 750, 90, col);//bottom rect to hide
    update();
    }
}

void Widget::currentCaseOfInfo(int cLine, int bound, bool side, int num, QPainter *qp)
{
    QPen p;
    p.setWidth(2);
    QColor cl;
    QPainterPath path, path1;
    QBrush brush("#5F9EA0");
    cl.setRgb(0, 0, 0, 0);
    p.setColor(cl);
    qp->setPen(p);

    if(side == true && forecastCase != 0)
    {
        if (i != 0)
        {
            currentCaseOfInfo(cLine - 90, bound, false, i - 1, qp);//1
        }
        if (i != bound)
        {
            currentCaseOfInfo(cLine + 90, bound, false, i + 1, qp);//3
        }
        if (i != bound - 1 && i != bound && cLine < 380)
        {
            currentCaseOfInfo(cLine + 180, bound, false, i + 2, qp);//4
        }
    }
    path.addRoundedRect(QRectF(50, cLine, 750, 80), 10, 10);//2
    qp->fillPath(path, brush);
    qp->drawPath(path);

    p.setColor("#483D8B");
    qp->setFont(QFont("Times", 12, QFont::Bold));
    qp->setPen(p);
    if (forecastCase == 0)
    {
        qp->drawText(60, cLine + 30, "Sunrise: " + sunrise[num] + ";");
        qp->drawText(260, cLine + 30, "Wind m/s: " + QString::number(wind_speed[num]));
        drawWindAngle(450, cLine + 20, num);
        qp->drawText(480, cLine + 30, "Uvi: ");
        drawUvi(540, cLine + 20, num);

        qp->drawText(60, cLine + 65, "Sunset: " + sunset[num].rightJustified(9) + ";");
        qp->drawText(260, cLine + 65, "Press: " + QString::number(pressure[num]).rightJustified(5) + ";");
        qp->drawText(400, cLine + 65, "Hum: " + QString::number(humidity[num]) + ";");
    }
    else if (forecastCase == 1)
    {
        QString curPrec = QString::number(precipitation[num]);
        qp->drawText(220, cLine + 47, "Time: " + dt[num]);
        qp->drawText(480, cLine + 47, "Precipitation: " + curPrec);
    }
    else if (forecastCase == 2)
    {
        qp->drawText(60, cLine + 30, "Wind m/s: " + QString::number(wind_speed[num]).rightJustified(5));
        drawWindAngle(250, cLine + 20, num);
        qp->drawText(280, cLine + 30, "Uvi: ");
        drawUvi(340, cLine + 20, num);
        qp->drawText(365, cLine + 30, "Vision: " + QString::number(visibility[num]).rightJustified(5));

        qp->drawText(60, cLine + 65, "Gust m/s: " + QString::number(wind_gust[num]));
        qp->drawText(240, cLine + 65, "Press: " + QString::number(pressure[num]).rightJustified(5) + ";");
        qp->drawText(380, cLine + 65, "Hum: " + QString::number(humidity[num]) + ";");
        qp->drawText(500, cLine + 65, "Dew p.(C): " + QString::number(dew_point[num]));

        drawCloud(760, cLine + 39, num);
    }
    else
    {
        qp->drawText(60, cLine + 30, "Sunrise: " + sunrise[num] + ";");
        qp->drawText(260, cLine + 30, "Wind m/s: " + QString::number(wind_speed[num]));
        drawWindAngle(450, cLine + 20, num);
        qp->drawText(480, cLine + 30, "Uvi: ");
        drawUvi(540, cLine + 20, num);
        qp->drawText(565, cLine + 30, "Moon: ");
        drawMoon(650, cLine + 20, num);

        qp->drawText(60, cLine + 65, "Sunset: " + sunset[num].rightJustified(9) + ";");
        qp->drawText(260, cLine + 65, "Press: " + QString::number(pressure[num]).rightJustified(5) + ";");
        qp->drawText(400, cLine + 65, "Hum: " + QString::number(humidity[num]) + ";");
        qp->drawText(500, cLine + 65, "Max/Min(C): " + QString::number(tempMax[num]) + "/" + QString::number(tempMin[num]));

        drawCloud(760, cLine + 28, num);
    }
}

void Widget::drawWindAngle(int space, int cLine, int num)
{
    qDebug() << forecastCase << "case" << wind_deg[num];
    static const QPoint arrow[3] = {
             QPoint(3, 0),
             QPoint(-3, 0),
             QPoint(0, - 15)
         };
    static const QPoint arrowEnd[4] = {
             QPoint(3, 0),
             QPoint(-3, 0),
             QPoint(-5, 12),
             QPoint(5, 12)
         };
    qp->save();
    qp->translate(space, cLine);
    qp->rotate(360 - wind_deg[num] + 90);
    qp->setPen(Qt::NoPen);
    QColor cl("#483D8B");
    qp->setBrush(cl);
    qp->drawEllipse(QPoint(0, 0), 16, 16);
    cl.setNamedColor("#6A5ACD");
    qp->setBrush(cl);
    qp->drawEllipse(QPoint(0, 0), 14, 14);
    cl.setNamedColor("#FF4F00");
    qp->setBrush(cl);
    qp->drawConvexPolygon(arrow, 3);
    qp->drawConvexPolygon(arrowEnd, 4);
    qp->translate(-space, -cLine);
    qp->restore();
}

void Widget::drawUvi(int space, int cLine, int num)
{
    qp->save();
    qp->setPen(Qt::NoPen);
    QColor cl("#483D8B");
    qp->setBrush(cl);
    qp->drawEllipse(QPoint(space, cLine), 16, 16);
    if (uvi[num] <= 2)
    {
        cl.setNamedColor("#8CCB5E");
    }
    else if (uvi[num] <= 5)
    {
        cl.setNamedColor("#FFCF48");
    }
    else if (uvi[num] <= 7)
    {
        cl.setNamedColor("#ED760E");
    }
    else if (uvi[num] <= 10)
    {
        cl.setNamedColor("#C41E3A");
    }
    else
    {
        cl.setNamedColor("#993366");
    }
    qp->setBrush(cl);
    qp->drawEllipse(QPoint(space, cLine), 14, 14);
    QPen p;
    p.setColor("#483D8B");
    qp->setFont(QFont("Times", 8, QFont::Bold));
    qp->setPen(p);
    int prec;
    int correct;
    if (uvi[num] >= 10)
    {
        prec = 0;
        correct = 2;
    }
    else
    {
        prec = 1;
        correct = 0;
    }
    qp->drawText(space - 10 + correct, cLine + 6, QString::number(uvi[num], 'd', prec));
    qp->restore();
}

void Widget::drawMoon(int space, int cLine, int num)
{
    qp->save();
    QPainterPath path;
    qp->setBrush(Qt::black);
    qp->drawEllipse(QPoint(space, cLine), 16, 16);
    qp->setBrush(Qt::white);
    path.moveTo(space, cLine - 15);
    if(moon_phase[num] < 0.5)
    {
        path.cubicTo(space + 20, cLine - 15, space + 20, cLine + 15, space, cLine + 15);
        path.moveTo(space, cLine - 15);
        path.cubicTo(space + 20 * ((0.25 - moon_phase[num]) / 0.25), cLine - 15, space + 20 * ((0.25 - moon_phase[num]) / 0.25), cLine + 15, space, cLine + 15);
    }
    else if (moon_phase[num] > 0.5)
    {
        path.cubicTo(space - 20, cLine - 15, space - 20, cLine + 15, space, cLine + 15);
        path.moveTo(space, cLine - 15);
        path.cubicTo(space + 20 * ((0.75 - moon_phase[num]) / 0.25), cLine - 15, space + 20 * ((0.75 - moon_phase[num]) / 0.25), cLine + 15, space, cLine + 15);
    }
    else
    {
        qp->drawEllipse(QPoint(space, cLine), 15, 15);
    }
    qp->drawPath(path);
    qp->restore();
}

void Widget::drawCloud(int space, int cLine, int num)
{
    qp->save();
    qp->setPen(Qt::NoPen);
    qp->translate(space, cLine);
    if (clouds[num] >= 50)
    {
        qp->setBrush(Qt::black);
        qp->drawEllipse(QPoint(10, -8), 5, 5);
        qp->drawEllipse(QPoint(18, -6), 4, 4);
        qp->drawEllipse(QPoint(23, -2), 4, 4);
        qp->fillRect(5, -4, 20, 6, Qt::black);
        qp->setBrush(Qt::white);
        qp->drawEllipse(QPoint(0, 0), 5, 5);
        qp->drawEllipse(QPoint(5, -5), 5, 5);
        qp->drawEllipse(QPoint(13, -3), 4, 4);
        qp->drawEllipse(QPoint(18, 1), 4, 4);
        qp->fillRect(0, -1, 20, 6, Qt::white);
    }
    else
    {
        if (forecastCase == 2)
        {
            QString str = dt[num].left(2);
            int hours = str.toInt();
            qp->setBrush(Qt::yellow);
            if (hours >= 6 && hours < 18)
            {
                qp->drawEllipse(QPoint(13, -8), 7, 7);
            }
            else if (hours >= 18 && hours < 22)
            {
                QBrush br("#FF8800");
                qp->setBrush(br);
                qp->drawEllipse(QPoint(13, -8), 7, 7);
            }
            else
            {
                qp->drawEllipse(QPoint(13, -8), 7, 7);
                QBrush br("#5F9EA0");
                qp->setBrush(br);
                qp->drawEllipse(QPoint(16, -8), 5, 5);
            }
        }
        else
        {
            qp->setBrush(Qt::yellow);
            qp->drawEllipse(QPoint(13, -8), 7, 7);
        }
        if (clouds[num] >= 25)
        {
            qp->setBrush(Qt::white);
            qp->drawEllipse(QPoint(0, 0), 5, 5);
            qp->drawEllipse(QPoint(5, -5), 5, 5);
            qp->drawEllipse(QPoint(13, -3), 4, 4);
            qp->drawEllipse(QPoint(18, 1), 4, 4);
            qp->fillRect(0, -1, 20, 6, Qt::white);
        }
        else
        {
            qp->setBrush(Qt::white);
            qp->drawEllipse(QPoint(0, 0), 4, 4);
            qp->fillRect(0, -1, 15, 5, Qt::white);
            qp->drawEllipse(QPoint(15, 0), 4, 4);
            qp->drawEllipse(QPoint(7, -3), 6, 6);
        }
    }

    QPen p;
    if (forecastCase != 0 && pop[num] >= 0.5)
    {
        p.setColor("#1E90FF");
        p.setWidth(2);
        qp->setPen(p);
        qp->drawLine(3, 7, 2, 10);
        qp->drawLine(8, 7, 7, 10);
        qp->drawLine(13, 7, 12, 10);
    }
    p.setColor("#483D8B");
    qp->setFont(QFont("Times", 6, QFont::Bold));
    qp->setPen(p);
    qp->drawText(-3, 20, QString::number(pop[num] * 100) + "%");
    qp->translate(-space, -cLine);
    qp->restore();
}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter qp(this);
    Widget::qp = &qp;

    qp.save();

    QPainterPath path;
    QColor cl(Qt::black);

    qp.setPen(Qt::NoPen);
    path.addRoundedRect(QRectF(870, 525, 200, 100), 15, 15);
    QBrush brushh(cl);
    qp.fillPath(path, brushh);
    qp.drawPath(path);
    path = QPainterPath();

    QFont font("Cursive", 14, -1, true);
    qp.setFont(font);

    cl.setNamedColor("#C51D34");
    path.addText(925, 575, font, "Michael");
    font = QFont("Cursive", 11, -1, true);
    path.addText(925, 589, font, "industries");
    QLinearGradient lGrad(QPointF(925, 580), QPointF(970, 580));
    lGrad.setColorAt(1, cl);
    cl.setNamedColor("#FF8800");
    lGrad.setColorAt(0, cl);
    QBrush brush(lGrad);
    QPolygonF polygon;
    polygon << QPointF(915, 555) << QPointF(1012, 555) << QPointF(1001, 592) << QPointF(1020, 550);
    path.addPolygon(polygon);
    qp.fillPath(path, brush);
    qp.drawPath(path);
    qp.restore();

    setBackground(&qp);
    caseInfo(&qp);
}

QString fromUnix(int data, int offset)
{
    QDateTime dt;
    if (data > 0)
    {
        dt.setTime_t(data - 10800 + offset * 3600);
        qDebug() << dt;
        return dt.time().toString();
    }
    else
    {
        dt.setTime_t(-data);
        return dt.date().toString();
    }
}

void Widget::setData(QJsonValue jvv, int i)
{
    QJsonObject joo = jvv.toObject();
    QJsonArray jaa;
    if (forecastCase == 2)
    {
        jaa = jvv.toArray();
    }
    else if (forecastCase == 3)
    {
        jaa = jvv.toArray();
    }

    if (forecastCase != 0)
    {
        joo = jaa.at(i).toObject();
    }
    int dtInt = joo.value("dt").toInt();
    qDebug() << dtInt;
    dt[i] = fromUnix(dtInt, timezone_offset);
    data[i] = fromUnix(-dtInt, timezone_offset);
    qDebug() << "data " << data[i];
    qDebug() << "dt " << dt[i];
    temp[i] = joo.value("temp").toDouble() - 273.15;
    qDebug() << "temp " << temp[i];
    feels_like[i] = joo.value("feels_like").toDouble() - 273.15;
    qDebug() << "feels_like " << feels_like[i];
    pressure[i] = joo.value("pressure").toInt();
    qDebug() << "pressure " << pressure[i];
    humidity[i] = joo.value("humidity").toInt();
    qDebug() << "humidity " << humidity[i];
    dew_point[i] = joo.value("dew_point").toDouble() - 273.15;
    qDebug() << "dew_point " << dew_point[i];
    uvi[i] = joo.value("uvi").toDouble();
    qDebug() << "uvi " << uvi[i];
    clouds[i] = joo.value("clouds").toInt();
    qDebug() << "clouds " << clouds[i];
    visibility[i] = joo.value("visibility").toInt();
    qDebug() << "visibility " << visibility[i];
    wind_speed[i] = joo.value("wind_speed").toDouble();
    qDebug() << "wind_speed " << wind_speed[i];
    wind_deg[i] = joo.value("wind_deg").toInt();
    qDebug() << "wind_deg " << wind_deg[i];
    wind_gust[i] = joo.value("wind_gust").toDouble();
    qDebug() << "wind_gust " << wind_gust[i];
    jvv = joo.value("weather");
    if(jvv.isArray())
    {
        jaa = jvv.toArray();
        joo = jaa.at(0).toObject();
        id[i] = joo.value("id").toInt();
        qDebug() << "id " << id[i];
        main[i] = joo.value("main").toString();
        qDebug() << "main " << main[i];
        description[i] = joo.value("description").toString();
        qDebug() << "description " << description[i];
        icon[i] = joo.value("icon").toString();
        qDebug() << "icon " << icon[i];
    }
}

void Widget::onResultCoord(QNetworkReply *reply)
{
    //i = 0;
    ui->progressBar->setValue(6);
    if(!reply->error())
    {
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonObject root = document.object();
        jv = root.value("coord");
        jo = jv.toObject();
        qDebug() << root;
        lat = jo.value("lat").toDouble();
        qDebug() << lat;
        lon = jo.value("lon").toDouble();
        qDebug() << lon;
    }
    chooseData();
    reply->deleteLater();
}

void Widget::onResult(QNetworkReply *reply)
{
    ui->progressBar->setValue(10);
    if(!reply->error())
    {
        existInfo = true;
        i = 0;
        prev = 0;
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonObject root = document.object();

        lat = root.value("lat").toDouble();
        qDebug() << lat;
        lon = root.value("lon").toDouble();
        qDebug() << lon;
        timezone = root.value("timezone").toString();
        qDebug() << timezone;
        timezone_offset = root.value("timezone_offset").toInt() / 3600;
        qDebug() << timezone_offset;

        if (forecastCase == 0)
        {
            jv = root.value("current");
            jo = jv.toObject();
            setData(jv, 0);
            int dtInt = jo.value("sunrise").toInt();
            sunrise[0] = fromUnix(dtInt, timezone_offset);
            qDebug() << "sunrise " << sunrise[0];
            dtInt = jo.value("sunset").toInt();
            sunset[0] = fromUnix(dtInt, timezone_offset);
            qDebug() << "sunset " << sunset[0];
        }
        else if (forecastCase == 1)
        {
            QJsonValue jv = root.value("minutely");
            QJsonObject jo;
            if(jv.isArray())
            {
                existInfo = true;
                QJsonArray ja = jv.toArray();
                qDebug() << ja.count();
                for (int i = 0; i < ja.count(); i++)
                {
                    jo = ja.at(i).toObject();
                    int dtInt = jo.value("dt").toInt();
                    dt[i] = fromUnix(dtInt, timezone_offset);
                    data[i] = fromUnix(-dtInt, timezone_offset);
                    qDebug() << "dt " << dt[i];
                    precipitation[i] = jo.value("precipitation").toInt();
                    qDebug() << "precipitation " << precipitation[i];
                }
            }
            else
            {
                existInfo = false;
            }
        }
        else if (forecastCase == 2)
        {
            jv = root.value("hourly");
            if(jv.isArray())
            {
                existInfo = true;
                ja = jv.toArray();
                int border = ja.count();
                for (int i = 0; i < border; i++)
                {
                    jo = ja.at(i).toObject();
                    setData(jv, i);
                    pop[i] = jo.value("pop").toDouble();
                    qDebug() << "pop " << pop[i];
                }
            }
            else
            {
                existInfo = false;
            }
        }
        else
        {
            jv = root.value("daily");
            if(jv.isArray())
            {
                existInfo = true;
                ja = jv.toArray();
                int border = ja.count();
                for (int i = 0; i < border; i++)
                {
                    jo = ja.at(i).toObject();
                    qDebug() << jo;
                    setData(jv, i);
                    qDebug() << jo;
                    int dtInt = jo.value("sunrise").toInt();
                    sunrise[i] = fromUnix(dtInt, timezone_offset);
                    qDebug() << "sunrise " << sunrise[i];
                    dtInt = jo.value("sunset").toInt();
                    sunset[i] = fromUnix(dtInt, timezone_offset);
                    qDebug() << "sunset " << sunset[i];
                    dtInt = jo.value("moonrise").toInt();
                    moonrise[i] = fromUnix(dtInt, timezone_offset);
                    qDebug() << "moonrise " << moonrise[i];
                    dtInt = jo.value("moonset").toInt();
                    moonset[i] = fromUnix(dtInt, timezone_offset);
                    qDebug() << "moonset " << moonset[i];
                    moon_phase[i] = jo.value("moon_phase").toDouble();
                    qDebug() << "moon_phase " << moon_phase[i];

                    QJsonValue jvH = jo.value("temp");
                    qDebug() << jvH;
                    QJsonObject joH = jvH.toObject();
                    tempDay[i] = joH.value("day").toDouble() - 273.15;
                    qDebug() << "tempDay " << tempDay[i];
                    tempMin[i] = joH.value("min").toDouble() - 273.15;
                    qDebug() << "tempMin " << tempMin[i];
                    tempMax[i] = joH.value("max").toDouble() - 273.15;
                    qDebug() << "tempMax " << tempMax[i];
                    tempNight[i] = joH.value("night").toDouble() - 273.15;
                    qDebug() << "tempNight " << tempNight[i];
                    tempEve[i] = joH.value("eve").toDouble() - 273.15;
                    qDebug() << "tempEve " << tempEve[i];
                    tempMorn[i] = joH.value("morn").toDouble() - 273.15;
                    qDebug() << "tempMorn " << tempMorn[i];

                    jvH = jo.value("feels_like");
                    qDebug() << jvH;
                    joH = jvH.toObject();
                    flDay[i] = joH.value("day").toDouble() - 273.15;
                    qDebug() << "flDay " << flDay[i];
                    flNight[i] = joH.value("night").toDouble() - 273.15;
                    qDebug() << "flNight " << flNight[i];
                    flEve[i] = joH.value("eve").toDouble() - 273.15;
                    qDebug() << "flEve " << flEve[i];
                    flMorn[i] = joH.value("morn").toDouble() - 273.15;
                    qDebug() << "flMorn " << flMorn[i];
                    pop[i] = jo.value("pop").toDouble();
                    qDebug() << "pop " << pop[i];
                }
            }
        }
    }
    ui->setLon->setValue(lon);
    ui->setLat->setValue(lat);
    lastClickLE = true;
    reply->deleteLater();
    i = 0;
    n = 0;
    ui->progressBar->setValue(0);
}


void Widget::chooseData()
{
    switch(forecastCase)
    {
    case 0:
        on_current_clicked();
        break;
    case 1:
        on_minutely_clicked();
        break;
    case 2:
        on_hourly_clicked();
        break;
    case 3:
        on_daily_clicked();
        break;
    default:
        on_current_clicked();
        break;
    }
    ui->progressBar->setValue(8);
    networkManager->get(QNetworkRequest(QUrl(url)));
}

void Widget::on_setl_clicked()
{
    ui->progressBar->setValue(2);
    QThread::msleep(20);
    forecastCase = forecastCaseTemp;
    if (lastClickLE == true)
    {
        ui->locations->setCurrentText("...");
        qDebug() << text;
        url = "http://api.openweathermap.org/data/2.5/weather?q=" + text + "&appid=57e9a6f3b67fc7f28dc242e7ab0ec508";
        networkManagerCoord->get(QNetworkRequest(QUrl(url)));
    }
    else
    {
        ui->cityName->setText("");
        lat = tempLat;
        lon = tempLon;
        chooseData();
    }
}

void Widget::on_setLat_valueChanged(double arg1)
{
    tempLat = arg1;
    lastClickLE = false;
}

void Widget::on_setLon_valueChanged(double arg1)
{
    tempLon = arg1;
    lastClickLE = false;
}

void Widget::on_current_clicked()
{
    QString toStrLat = QString::number(lat,'g',4);
    QString toStrLon = QString::number(lon,'g',4);
    url = "http://api.openweathermap.org/data/2.5/onecall?lat=" + toStrLat + "&lon=" + toStrLon + "&exclude=alerts,minutely,hourly,daily&appid=57e9a6f3b67fc7f28dc242e7ab0ec508";
    forecastCaseTemp = 0;
}

void Widget::on_minutely_clicked()
{
    QString toStrLat = QString::number(lat,'g',4);
    QString toStrLon = QString::number(lon,'g',4);
    url = "http://api.openweathermap.org/data/2.5/onecall?lat=" + toStrLat + "&lon=" + toStrLon + "&exclude=alerts,current,hourly,daily&appid=57e9a6f3b67fc7f28dc242e7ab0ec508";
    forecastCaseTemp = 1;
}

void Widget::on_hourly_clicked()
{
    QString toStrLat = QString::number(lat,'g',4);
    QString toStrLon = QString::number(lon,'g',4);
    url = "http://api.openweathermap.org/data/2.5/onecall?lat=" + toStrLat + "&lon=" + toStrLon + "&exclude=alerts,minutely,current,daily&appid=57e9a6f3b67fc7f28dc242e7ab0ec508";
    forecastCaseTemp = 2;
}

void Widget::on_daily_clicked()
{
    QString toStrLat = QString::number(lat,'g',4);
    QString toStrLon = QString::number(lon,'g',4);
    url = "http://api.openweathermap.org/data/2.5/onecall?lat=" + toStrLat + "&lon=" + toStrLon + "&exclude=alerts,minutely,hourly,current&appid=57e9a6f3b67fc7f28dc242e7ab0ec508";
    forecastCaseTemp = 3;
}

void Widget::on_locations_activated(const QString &arg1)
{
    if (arg1 == "...")
    {
        tempLat = 0;
        tempLon = 0;
    }
    if (arg1 == "London")
    {
        tempLat = 51.5085;
        tempLon = -0.1257;
    }
    else if (arg1 == "Minsk")
    {
        tempLat = 53.9;
        tempLon = 27.5667;
    }
    else if (arg1 == "Chicago")
    {
        tempLat = 41.85;
        tempLon = -87.65;
    }
    else if (arg1 == "Paris")
    {
        tempLat = 48.8534;
        tempLon = 2.3488;
    }
    else if (arg1 == "San Francisco")
    {
        tempLat = 37.7749;
        tempLon = -122.419;
    }
    else if (arg1 == "Los Angeles")
    {
        tempLat = 34.05;
        tempLon = -118.2;
    }
    lastClickLE = false;
}

void Widget::on_cityName_textEdited(const QString &arg1)
{
    lastClickLE = true;
    text = arg1;
}

