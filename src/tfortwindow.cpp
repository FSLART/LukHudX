/**
* @file mainwindow.cpp
* @brief This file contains the logic for the MainWindow class
* @see mainwindow.h
* @see store.h
* @see flabel.h
* @author João Vieira
* This piece of software was developed for the T24e project belonging to the LART Team
**/
#include "tfortwindow.h"
#include "./ui_tfortwindow.h"
#include "./store.h"
#include <cstddef>
#include <qmainwindow.h>
#include "flabel.h"

static store* store_ref;
/**
* @brief Constructor for the MainWindow Class.
*        @b Connects @b **most** variables from the store to the FLabels on the screen.
**/
TfortWindow::TfortWindow(QWidget *parent, QString serialDev)
    : QMainWindow(parent)
    , ui(new Ui::TfortWindow){

    store_ref = new store(serialDev);

    ui->setupUi(this);
    //store_ref->setParent(this);
    //store_ref->requestSlotAttachment();
		
		FLabel* EngineTemperature_Label= this->findChild<FLabel*>("EngineTemperature_Label");
		FLabel* BatteryVoltage_Label = this->findChild<FLabel*>("BatteryVoltage_Label");
		FLabel* VehicleSpeed_Label = this->findChild<FLabel*>("VehicleSpeed_Label");
		connect(store_ref, &store::engineTemperatureChanged, EngineTemperature_Label, (void (FLabel::*)(int, int))&FLabel::setVisual);
		connect(store_ref, &store::batteryVoltageChanged, BatteryVoltage_Label, (void (FLabel::*)(float, float))&FLabel::setVisual);
		connect(store_ref, &store::vehicleSpeedChanged, VehicleSpeed_Label, (void (FLabel::*)(int, int))&FLabel::setVisual);
		/**
		* @brief bellow is the spaggeti code thats preprocessed to permit portable use between vehicles 
	    *        The magic occurs in qmake through the DEFINES variable.
		* @see bson_var.h
		**/
		#ifdef __LART_T14__
			FLabel* OilPressure_Label = this->findChild<FLabel*>("OilPressure_Label");
			FLabel* OilTemperature_Label = this->findChild<FLabel*>("OilTemperature_Label");
			FLabel* DataLogger_Label = this->findChild<FLabel*>("DataLogger_Label");
			FLabel* Lambda_Label = this->findChild<FLabel*>("Lambda_Label");
			FLabel* TcSlip_Label = this->findChild<FLabel*>("TcSlip_Label");
			FLabel* TcLaunch_Label = this->findChild<FLabel*>("TcLaunch_Label");

			connect(store_ref, &store::dataLoggerChanged, DataLogger_Label, (void (FLabel::*)(int, int))&FLabel::setVisual);
			connect(store_ref, &store::lambdaChanged, Lambda_Label, (void (FLabel::*)(float, float))&FLabel::setVisual);
			connect(store_ref, &store::tcSlipChanged, TcSlip_Label, (void (FLabel::*)(int, int))&FLabel::setVisual);
			connect(store_ref, &store::tcLaunchChanged, TcLaunch_Label, (void (FLabel::*)(int, int))&FLabel::setVisual);

			connect(store_ref, &store::oilPressureChanged, OilPressure_Label, (void (FLabel::*)(float, float))&FLabel::setVisual);
			connect(store_ref, &store::oilTemperatureChanged, OilTemperature_Label, (void (FLabel::*)(float, float))&FLabel::setVisual);
		#endif
		

		
}


/**
* @brief A getter for a store pointer. <b> use with caution, </b>
* 		 <em> with great power comes great *frickery... This function is by reference and should be used for startup stuff</em> 
* @returns a pointer to the store object
**/
store* TfortWindow::getStore(){
    return store_ref;
}
/**
* @brief Destructor for the MainWindow Class.
*        @b Deletes the store object. which can cause some odd behaviour to happen
**/
TfortWindow::~TfortWindow(){
    store_ref->~store(); 
    delete ui;
}
