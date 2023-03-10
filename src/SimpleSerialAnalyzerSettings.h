#ifndef SIMPLESERIAL_ANALYZER_SETTINGS
#define SIMPLESERIAL_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class SimpleSerialAnalyzerSettings : public AnalyzerSettings
{
public:
	SimpleSerialAnalyzerSettings();
	virtual ~SimpleSerialAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

  Channel mReadClockChan, mReadDataChan, mReadGateChan;

protected:
  std::unique_ptr<AnalyzerSettingInterfaceChannel> mReadClock;
  std::unique_ptr<AnalyzerSettingInterfaceChannel> mReadData;
  std::unique_ptr<AnalyzerSettingInterfaceChannel> mReadGate;
  //std::unique_ptr< AnalyzerSettingInterfaceInteger >	mBitRateInterface;
};

#endif //SIMPLESERIAL_ANALYZER_SETTINGS
