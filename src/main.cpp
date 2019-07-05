/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <assert.h>
#include <stdlib.h>

#include "mainwindow.h"
#include <QApplication>

#include "GeohashTest.h"
#include "MatrixTest.h"
#include "SensorControllerTest.h"
#include "CoordinatesTest.h"
#include "SensorController.h"
#include "MadgwickAHRS.h"
#include "Coordinates.h"

#include <gtest/gtest.h>

enum launch_type_t {
  lt_app = 0,
  lt_tests,
};

template<launch_type_t lt> int run(int argc, char *argv[]);

template <>
int run<lt_app>(int argc, char *argv[]) {
  std::srand(unsigned(std::time(nullptr)));
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication app(argc, argv);
  MainWindow w;
  w.show();
  return app.exec();
}
///////////////////////////////////////////////////////

template <>
int run<lt_tests>(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
///////////////////////////////////////////////////////

//#define TESTMODE
int main(int argc, char *argv[]) {
  try {
#ifdef TESTMODE
    return run<lt_tests>(argc, argv);
#else
    return run<lt_app>(argc, argv);
#endif
  } catch (std::exception &exc) {
    std::cout << exc.what() << std::endl;
    return -1;
  } catch (...) {
    std::cout << "something went totally wrong. unhandled/untyped exception" << std::endl;
    return -2;
  }
}
//////////////////////////////////////////////////////////////////////////
