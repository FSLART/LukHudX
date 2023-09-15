#ifndef STORE_H
#define STORE_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <thread>
#include <cstdio>
#include <sys/types.h>
#include <vector>
#include <QCoreApplication>
#include <QErrorMessage>
#include <QFile>
#include <QObject>

#include <QDebug>
#include <QSerialPort>
#include <QDateTime>
#include <QThread>
//QVBoxLayout
#include <QVBoxLayout>
#include <nlohmann/json.hpp>
#include "references/bson_var.h"
#if !defined __arm__ || !defined __aarch64__
    #ifdef _WIN32
		#define DEFAULT_DEVICE "COM3"
    #elif defined __linux__
		#ifdef __LART_DEPLOY__
			#define DEFAULT_DEVICE "/dev/ttyS0"
		#else
            #define DEFAULT_DEVICE "/dev/ttyACM0"
		#endif
	#endif
#else
    #define DEFAULT_DEVICE "/dev/ttyS0"
#endif

typedef union {
	float decoded;
	int32_t encoded;
} EncodingUnion;


#define BSON_WARNING "\xFF\xFF\xFF\xFF"
#define LOG_MAX_RETRIES 4
#define BSON_SKIP_BYTES 9
/* !
        \class store
        \brief Basicamente uma store contendo as vários dados necessários a serem transmitidos ou representados graficamente

		Comecemos com o resumo pois se estás a ler isto provavelmente não e de completa livre vontade,
		esta classe representa um ponto centralizado dos dados semelhante a ideologia que e muito usada em sistemas distribuídos(especialmente falando em JS, Vue, React, etc...)
		
		\fields
*/

class store: public QObject{
    Q_OBJECT;
    Q_PROPERTY(int  m_rotationsPerMinute READ getRpm WRITE setRpm NOTIFY rpmChanged);



	public:
        enum error_severity {
            INFO=0,
            WARNING=1,
            MINOR=2,
            MAJOR=3,
            CRITICAL=100
        };
        QString dev;
		QSerialPort* port=nullptr;
		void handleReadyRead();
		void forceRead(qint64 len);
		void handleError(QSerialPort::SerialPortError serialPortError);
		
		QByteArray serialLog;
		QByteArray lastMessage;
		QByteArray bufferMessage;
		char * markerBSON_WARNING=nullptr; 
		
		void parseBson(std::vector<std::uint8_t> v);
                void bsonMining();
                int requestSlotAttachment();
        qint64 scribeError(QString error, error_severity severity=error_severity::INFO);
        explicit store(QString dev="", QSerialPort::BaudRate baud = QSerialPort::Baud115200, QObject *parent = nullptr);
		~store();
		
		//getters and setters
		QSerialPort::BaudRate getBaudRate() const;
		//getters wire variables
		int getRpm() const;
		int getEngineTemperature() const;
		float getBatteryVoltage() const;
		int getVehicleSpeed() const;
		//setters wire variables
		void setRpm(int rpm);
		void setEngineTemperature(int engineTemperature);
		void setBatteryVoltage(float batteryVoltage);
		void setVehicleSpeed(int vehicleVelocity);
		#ifdef __LART_T14__
			int getGearShift() const;			
			float getOilPressure() const;
			float getOilTemperature() const;
			int getDataLoggerStatus() const;
			float getLambda() const;
			int getTcSlip() const;
			int getTcLaunch() const;
			void setGearShift(int gearShift);
			void setOilPressure(float oilPressure);
			void setOilTemperature(float oilTemperature);
			void setDataLoggerStatus(int dataLoggerStatus);
			void setLambda(float lambda);
			void setTcSlip(int tcSlip);
			void setTcLaunch(int tcLaunch);

		#endif
		#ifdef __LART_T24__
			float getSoc() const;
			float getBatteryTemperature() const;
			int getInverterTemperature() const;
			short getPower() const;
			int getLapTime() const;
			short getLapCount() const;
			//int getTyreTemperature() const;
			void setSoc(float soc);
			void setBatteryTemperature(float batteryTemperature);
			void setInverterTemperature(int inverterTemperature);
			void setPower(short power);
			void setLapTime(int lapTime);
			void setLapCount(short lapCount);
			//void setTyreTemperature(int tyreTemperature);
		#endif

		void setBaudRate(QSerialPort::BaudRate baud);

	protected:
        int startGeneralErrorLog(uint depth=0);
		void stopGeneralErrorLog();
                int setupSerial();
        //        bool setSlots=false;
        //        int setupSlots();
		int closeSerial();

	signals:
		void rpmChanged(int newRpm, int oldRpm);
		void engineTemperatureChanged(int newEngineTemperature, int oldEngineTemperature);
		void batteryVoltageChanged(float newBatteryVoltage, float oldBatteryVoltage); 
		void vehicleSpeedChanged(int newVehicleSpeed, int oldVehicleSpeed);
		#ifdef __LART_T14__
			void gearShiftChanged(int newGearShift, int oldGearShift);
			void oilTemperatureChanged(float newOilTemperature, float oldOilTemperature); 
			void oilPressureChanged(float newOilPressure, float oldOilPressure); 
			void dataLoggerChanged(int newDataLogger, int oldDataLogger); 
			void lambdaChanged(float newLambda, float oldLambda); 
			void tcSlipChanged(int newTCSlip, int oldTCSlip); 
			void tcLaunchChanged(int newTcLaunch, int oldTcLaunch);  
		#endif
		#ifdef __LART_T24__
			void socChanged(float newSoc, float oldSoc);
			void batteryTemperatureChanged(float newBatteryTemperature, float oldBatteryTemperature);
			void inverterTemperatureChanged(int newInverterTemperature, int oldInverterTemperature);
			void powerChanged(short newPower, short oldPower);
			void lapTimeChanged(int newLapTime, int oldLapTime);
			void lapCountChanged(short newLapCount, short oldLapCount);
			//void tyreTemperatureChanged(int newTyreTemperature, int oldTyreTemperature); 
		#endif
		
		
		 
		
		
		
		 
		
		
		


    private:
		QSerialPort::BaudRate baud;
        QFile errorLog;
        int m_rotationsPerMinute=0;
        int m_gearShift=0;
        int m_engineTemperature=0;
        float m_oilPressure=0;
        float m_oilTemperature=0;
        float m_batteryVoltage=0;
        int m_vehicleVelocity=0;
        int m_dataLoggerStatus=0;
        float m_lambdaMixtureAirFuel=0;
        int m_tractionSlip=0;
        int m_tractionLaunch=0;

};

// Logging Macros
#define __LART_STORE_SETRPM_ERROR__ "store::setRpm(int rpm)->Rpm is negative"
#define __LART_STORE_SETGEARSHIFT_ERROR__ "store::setGearShift(int gearShift)->GearShift is out of bounds"

#endif // STORE_H
