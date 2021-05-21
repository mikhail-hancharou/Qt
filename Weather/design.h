#ifndef DESIGN_H
#define DESIGN_H

#endif // DESIGN_H
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
#include <QDebug>
void setDesign(QDoubleSpinBox *setLat, QDoubleSpinBox *setLon, QRadioButton *current, QRadioButton *minutely,
               QRadioButton *hourly, QRadioButton *daily, QPushButton *setl, QComboBox *locations,
               QLineEdit *cityName, QLabel *label, QLabel *label_2, QGroupBox *groupBox)
{
    label->setStyleSheet("QLabel { color: #483D8B; background-color: #66CDAA;"
                             "border-style: outset;"
                             "border-width: 1px;"
                             "border-radius: 5px;"
                             "border-color: #483D8B;"
                             "font: bold 12px;}");

    label_2->setStyleSheet("QLabel { color: #483D8B; background-color: #66CDAA;"
                             "border-style: outset;"
                             "border-width: 1px;"
                             "border-radius: 5px;"
                             "border-color: #483D8B;"
                             "font: bold 12px;}");

    groupBox->setStyleSheet("QGroupBox { color: #483D8B; "
                                "border-style: outset;"
                                "border-width: 1px;"
                                "border-radius: 5px;"
                                "border-color: #483D8B; font: bold 12px;}"

                                "QGroupBox:title { subcontrol-position: top center; }");
    cityName->setStyleSheet("QLineEdit { color: #483D8B; background-color: #66CDAA;"
                                "border-style: outset;"
                                "border-width: 1px;"
                                "border-radius: 5px;"
                                "border-color: #483D8B;"
                                "font: bold 14px;"
                                "min-width: 5em;"
                                "padding: 1px;}");

    locations->setStyleSheet("QComboBox { color: #483D8B; background-color: #66CDAA;"
                             "border-style: outset;"
                             "border-width: 1px;"
                             "border-radius: 5px;"
                             "border-color: #483D8B;"
                             "font: bold 14px; }"

                             "QComboBox:drop-down { background-color: #66CDAA;"
                             "border-style: outset;"
                             "border-radius: 2px;"
                             "border-width: 1px;"
                             "border-color: #483D8B;"
                             "width: 15; height: 18;}"

                             "QComboBox:drop-down:hover { background-color: #008080;"
                             "border-style: outset;"
                             "border-radius: 2px;"
                             "border-width: 1px;"
                             "border-color: #483D8B;"
                             "width: 15; height: 18;}"

                             "QComboBox:drop-down:pressed { background-color: #008080;"
                             "border-style: outset;"
                             "border-radius: 2px;"
                             "border-width: 1px;"
                             "border-color: #483D8B;"
                             "width: 14; height: 17;}"

                             "QComboBox:down-arrow { background-color: #66CDAA;"
                             "border-left: 4px solid none;"
                             "border-right: 4px solid none;"
                             "border-top: 3px solid #483D8B;"
                             "width: 0px; "
                             "height: 0px;}"

                             "QComboBox:down-arrow:hover { background-color: #008080;"
                             "border-left: 4px solid none;"
                             "border-right: 4px solid none; "
                             "border-top: 3px solid #66CDAA; "
                             "width: 0px; "
                             "height: 0px;}"

                             "QComboBox:down-arrow:pressed { background-color: #008080;"
                             "border-left: 4px solid none;"
                             "border-right: 4px solid none; "
                             "border-top: 3px solid #66CDAA; "
                             "width: 0px; "
                             "height: 0px;}");

    setl->setStyleSheet("QPushButton { color: #483D8B;"
                        "background-color: #66CDAA;"
                        "border-style: outset;"
                        "border-width: 1px;"
                        "border-radius: 8px;"
                        "border-color: #483D8B;"
                        "font: bold 14px;"
                        "min-width: 5em;"
                        "padding: 6px; }"

                        "QPushButton:hover { color: #66CDAA;"
                        "background-color: #008080;"
                        "border-style: outset;"
                        "border-radius: 8px;"
                        "border-width: 1px;"
                        "border-color: #483D8B;"
                        "font: bold 14px;"
                        "min-width: 5em;"
                        "padding: 6px;}"

                        "QPushButton:pressed { color: #66CDAA;"
                        "background-color: #008080;"
                        "border-style: outset;"
                        "border-radius: 8px;"
                        "border-width: 2px;"
                        "border-color: #483D8B;"
                        "font: bold 14px;"
                        "min-width: 5em;"
                        "padding: 6px;}"
    );

    current->setStyleSheet("QRadioButton { font: bold 14px; color: #483D8B; border-style: outset;"
                               "border-radius: 4px; background-color: #66CDAA; }"
                           "QRadioButton:indicator { border-style: outset;"
                           "border-color: #483D8B;"
                           "border-radius: 8px;"
                           "border-width: 2px;"
                           "background-color: #66CDAA; }"

                           "QRadioButton:indicator:checked { border-style: outset;"
                           "border-color: #483D8B;"
                           "border-radius: 8px;"
                           "border-width: 2px;"
                           "background-color: #6A5ACD; }");

    minutely->setStyleSheet("QRadioButton { font: bold 14px; color: #483D8B; border-style: outset;"
                            "border-radius: 4px; background-color: #66CDAA; }"
                        "QRadioButton:indicator { border-style: outset;"
                        "border-color: #483D8B;"
                        "border-radius: 8px;"
                        "border-width: 2px;"
                        "background-color: #66CDAA; }"

                        "QRadioButton:indicator:checked { border-style: outset;"
                        "border-color: #483D8B;"
                        "border-radius: 8px;"
                        "border-width: 2px;"
                        "background-color: #6A5ACD; }");

    hourly->setStyleSheet("QRadioButton { font: bold 14px; color: #483D8B; border-style: outset;"
                          "border-radius: 4px; background-color: #66CDAA; }"
                      "QRadioButton:indicator { border-style: outset;"
                      "border-color: #483D8B;"
                      "border-radius: 8px;"
                      "border-width: 2px;"
                      "background-color: #66CDAA; }"

                      "QRadioButton:indicator:checked { border-style: outset;"
                      "border-color: #483D8B;"
                      "border-radius: 8px;"
                      "border-width: 2px;"
                      "background-color: #6A5ACD; }");

    daily->setStyleSheet("QRadioButton { font: bold 14px; color: #483D8B; border-style: outset;"
                         "border-radius: 4px; background-color: #66CDAA; }"
                     "QRadioButton:indicator { border-style: outset;"
                     "border-color: #483D8B;"
                     "border-radius: 8px;"
                     "border-width: 2px;"
                     "background-color: #66CDAA; }"

                     "QRadioButton:indicator:checked { border-style: outset;"
                     "border-color: #483D8B;"
                     "border-radius: 8px;"
                     "border-width: 2px;"
                     "background-color: #6A5ACD; }");

    setLat->setStyleSheet("QDoubleSpinBox { color: #483D8B; border-style: outset; background-color: #66CDAA;"
                          "border-width: 1px;"
                          "border-radius: 5px;"
                          "border-color: #483D8B;"
                          "min-width: 80;"
                          "min-height: 22;"
                          "font: bold 12px;}"

                          "QDoubleSpinBox:up-button { background-color: #66CDAA;"
                          "border-style: outset;"
                          "border-radius: 2px;"
                          "border-width: 1px;"
                          "border-color: #483D8B;"
                          "width: 15; height: 8;}"

                          "QDoubleSpinBox:up-button:hover { background-color: #008080;"
                          "border-style: outset;"
                          "border-radius: 2px;"
                          "border-width: 1px;"
                          "border-color: #483D8B;"
                          "width: 15; height: 8;}"

                          "QDoubleSpinBox:up-button:pressed { background-color: #008080;"
                          "border-style: outset;"
                          "border-radius: 3px;"
                          "border-width: 1px;"
                          "border-color: #483D8B;"
                          "width: 14; height: 7;}"

                          "QDoubleSpinBox:up-arrow { background-color: #66CDAA;"
                          "border-left: 4px solid none;"
                          "border-right: 4px solid none;"
                          "border-bottom: 3px solid #483D8B;"
                          "width: 0px; height: 0px;}"

                          "QDoubleSpinBox:up-arrow:hover { background-color: #008080;"
                          "border-left: 4px solid none;"
                          "border-right: 4px solid none; "
                          "border-bottom: 3px solid #66CDAA; "
                          "width: 0px; "
                          "height: 0px;}"

                          "QDoubleSpinBox:up-arrow:pressed { background-color: #008080;"
                          "border-left: 4px solid none;"
                          "border-right: 4px solid none; "
                          "border-bottom: 3px solid #66CDAA; "
                          "width: 0px;"
                          "height: 0px;}"

                          "QDoubleSpinBox:down-button { background-color: #66CDAA;"
                          "border-style: outset;"
                          "border-radius: 2px;"
                          "border-width: 1px;"
                          "border-color: #483D8B;"
                          "width: 15; height: 8;}"

                          "QDoubleSpinBox:down-button:hover { background-color: #008080;"
                          "border-style: outset;"
                          "border-radius: 2px;"
                          "border-width: 1px;"
                          "border-color: #483D8B;"
                          "width: 15; height: 8;}"

                          "QDoubleSpinBox:down-button:pressed { background-color: #008080;"
                          "border-style: outset;"
                          "border-radius: 3px;"
                          "border-width: 1px;"
                          "border-color: #483D8B;"
                          "width: 14; height: 7;}"

                          "QDoubleSpinBox:down-arrow { background-color: #66CDAA;"
                          "border-left: 4px solid none;"
                          "border-right: 4px solid none;"
                          "border-top: 3px solid #483D8B;"
                          "width: 0px; height: 0px;}"

                          "QDoubleSpinBox:down-arrow:hover { background-color: #008080;"
                          "border-left: 4px solid none;"
                          "border-right: 4px solid none; "
                          "border-top: 3px solid #66CDAA; "
                          "width: 0px; "
                          "height: 0px;}"

                          "QDoubleSpinBox:down-arrow:pressed { background-color: #008080;"
                          "border-left: 4px solid none;"
                          "border-right: 4px solid none; "
                          "border-top: 3px solid #66CDAA; "
                          "width: 0px; "
                          "height: 0px;}");

    setLon->setStyleSheet("QDoubleSpinBox { color: #483D8B; border-style: outset; background-color: #66CDAA;"
                          "border-width: 1px;"
                          "border-radius: 5px;"
                          "border-color: #483D8B;"
                          "min-width: 80;"
                          "min-height: 22;"
                          "font: bold 12px;}"

                          "QDoubleSpinBox:up-button { background-color: #66CDAA;"
                          "border-style: outset;"
                          "border-radius: 2px;"
                          "border-width: 1px;"
                          "border-color: #483D8B;"
                          "width: 15; height: 8;}"

                          "QDoubleSpinBox:up-button:hover { background-color: #008080;"
                          "border-style: outset;"
                          "border-radius: 2px;"
                          "border-width: 1px;"
                          "border-color: #483D8B;"
                          "width: 15; height: 8;}"

                          "QDoubleSpinBox:up-button:pressed { background-color: #008080;"
                          "border-style: outset;"
                          "border-radius: 3px;"
                          "border-width: 1px;"
                          "border-color: #483D8B;"
                          "width: 14; height: 7;}"

                          "QDoubleSpinBox:up-arrow { background-color: #66CDAA;"
                          "border-left: 4px solid none;"
                          "border-right: 4px solid none;"
                          "border-bottom: 3px solid #483D8B;"
                          "width: 0px; height: 0px;}"

                          "QDoubleSpinBox:up-arrow:hover { background-color: #008080;"
                          "border-left: 4px solid none;"
                          "border-right: 4px solid none; "
                          "border-bottom: 3px solid #66CDAA; "
                          "width: 0px; "
                          "height: 0px;}"

                          "QDoubleSpinBox:up-arrow:pressed { background-color: #008080;"
                          "border-left: 4px solid none;"
                          "border-right: 4px solid none; "
                          "border-bottom: 3px solid #66CDAA; "
                          "width: 0px; "
                          "height: 0px;}"

                          "QDoubleSpinBox:down-button { background-color: #66CDAA;"
                          "border-style: outset;"
                          "border-radius: 2px;"
                          "border-width: 1px;"
                          "border-color: #483D8B;"
                          "width: 15; height: 8;}"

                          "QDoubleSpinBox:down-button:hover { background-color: #008080;"
                          "border-style: outset;"
                          "border-radius: 2px;"
                          "border-width: 1px;"
                          "border-color: #483D8B;"
                          "width: 15; height: 8;}"

                          "QDoubleSpinBox:down-button:pressed { background-color: #008080;"
                          "border-style: outset;"
                          "border-radius: 3px;"
                          "border-width: 1px;"
                          "border-color: #483D8B;"
                          "width: 14; height: 7;}"

                          "QDoubleSpinBox:down-arrow { background-color: #66CDAA;"
                          "border-left: 4px solid none;"
                          "border-right: 4px solid none;"
                          "border-top: 3px solid #483D8B;"
                          "width: 0px; height: 0px;}"

                          "QDoubleSpinBox:down-arrow:hover { background-color: #008080;"
                          "border-left: 4px solid none;"
                          "border-right: 4px solid none; "
                          "border-top: 3px solid #66CDAA; "
                          "width: 0px; "
                          "height: 0px;}"

                          "QDoubleSpinBox:down-arrow:pressed { background-color: #008080;"
                          "border-left: 4px solid none;"
                          "border-right: 4px solid none; "
                          "border-top: 3px solid #66CDAA; "
                          "width: 0px; "
                          "height: 0px;}");
}
