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

  Channel mBit0Chan, mBit1Chan, mSectIdxChan;

protected:
  std::unique_ptr<AnalyzerSettingInterfaceChannel> mBit0;
  std::unique_ptr<AnalyzerSettingInterfaceChannel> mBit1;
  std::unique_ptr<AnalyzerSettingInterfaceChannel> mSectorIndex;
  //std::unique_ptr< AnalyzerSettingInterfaceInteger >	mBitRateInterface;
};

#endif //SIMPLESERIAL_ANALYZER_SETTINGS
